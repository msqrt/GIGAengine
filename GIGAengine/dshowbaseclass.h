//------------------------------------------------------------------------------
// File: Streams.h
//
// Desc: DirectShow base classes - defines overall streams architecture.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __STREAMS__
#define __STREAMS__

#ifdef	_MSC_VER
// disable some level-4 warnings, use #pragma warning(enable:###) to re-enable
#pragma warning(disable:4100) // warning C4100: unreferenced formal parameter
#pragma warning(disable:4201) // warning C4201: nonstandard extension used : nameless struct/union
#pragma warning(disable:4511) // warning C4511: copy constructor could not be generated
#pragma warning(disable:4512) // warning C4512: assignment operator could not be generated
#pragma warning(disable:4514) // warning C4514: "unreferenced inline function has been removed"

#if _MSC_VER>=1100
#define AM_NOVTABLE __declspec(novtable)
#else
#define AM_NOVTABLE
#endif
#endif	// MSC_VER


// Because of differences between Visual C++ and older Microsoft SDKs,
// you may have defined _DEBUG without defining DEBUG.  This logic
// ensures that both will be set if Visual C++ sets _DEBUG.
#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif




#ifndef NUMELMS
#if _WIN32_WINNT < 0x0600
   #define NUMELMS(aa) (sizeof(aa)/sizeof((aa)[0]))
#else
   #define NUMELMS(aa) ARRAYSIZE(aa)
#endif   
#endif

///////////////////////////////////////////////////////////////////////////
// The following definitions come from the Platform SDK and are required if
// the applicaiton is being compiled with the headers from Visual C++ 6.0.
/////////////////////////////////////////////////// ////////////////////////
#ifndef InterlockedExchangePointer
	#define InterlockedExchangePointer(Target, Value) \
   (PVOID)InterlockedExchange((PLONG)(Target), (LONG)(Value))
#endif

#ifndef _WAVEFORMATEXTENSIBLE_
#define _WAVEFORMATEXTENSIBLE_
typedef struct {
    WAVEFORMATEX    Format;
    union {
        WORD wValidBitsPerSample;       /* bits of precision  */
        WORD wSamplesPerBlock;          /* valid if wBitsPerSample==0 */
        WORD wReserved;                 /* If neither applies, set to zero. */
    } Samples;
    DWORD           dwChannelMask;      /* which channels are */
                                        /* present in stream  */
    GUID            SubFormat;
} WAVEFORMATEXTENSIBLE, *PWAVEFORMATEXTENSIBLE;
#endif // !_WAVEFORMATEXTENSIBLE_

#if !defined(WAVE_FORMAT_EXTENSIBLE)
#define  WAVE_FORMAT_EXTENSIBLE                 0xFFFE
#endif // !defined(WAVE_FORMAT_EXTENSIBLE)

#ifndef GetWindowLongPtr
  #define GetWindowLongPtrA   GetWindowLongA
  #define GetWindowLongPtrW   GetWindowLongW
  #ifdef UNICODE
    #define GetWindowLongPtr  GetWindowLongPtrW
  #else
    #define GetWindowLongPtr  GetWindowLongPtrA
  #endif // !UNICODE
#endif // !GetWindowLongPtr

#ifndef SetWindowLongPtr
  #define SetWindowLongPtrA   SetWindowLongA
  #define SetWindowLongPtrW   SetWindowLongW
  #ifdef UNICODE
    #define SetWindowLongPtr  SetWindowLongPtrW
  #else
    #define SetWindowLongPtr  SetWindowLongPtrA
  #endif // !UNICODE
#endif // !SetWindowLongPtr

#ifndef GWLP_WNDPROC
  #define GWLP_WNDPROC        (-4)
#endif
#ifndef GWLP_HINSTANCE
  #define GWLP_HINSTANCE      (-6)
#endif
#ifndef GWLP_HWNDPARENT
  #define GWLP_HWNDPARENT     (-8)
#endif
#ifndef GWLP_USERDATA
  #define GWLP_USERDATA       (-21)
#endif
#ifndef GWLP_ID
  #define GWLP_ID             (-12)
#endif
#ifndef DWLP_MSGRESULT
  #define DWLP_MSGRESULT  0
#endif
#ifndef DWLP_DLGPROC 
  #define DWLP_DLGPROC    DWLP_MSGRESULT + sizeof(LRESULT)
#endif
#ifndef DWLP_USER
  #define DWLP_USER       DWLP_DLGPROC + sizeof(DLGPROC)
#endif


#pragma warning(push)
#pragma warning(disable: 4312 4244)
// _GetWindowLongPtr
// Templated version of GetWindowLongPtr, to suppress spurious compiler warning.
template <class T>
T _GetWindowLongPtr(HWND hwnd, int nIndex)
{
    return (T)GetWindowLongPtr(hwnd, nIndex);
}

// _SetWindowLongPtr
// Templated version of SetWindowLongPtr, to suppress spurious compiler warning.
template <class T>
LONG_PTR _SetWindowLongPtr(HWND hwnd, int nIndex, T p)
{
    return SetWindowLongPtr(hwnd, nIndex, (LONG_PTR)p);
}
#pragma warning(pop)

///////////////////////////////////////////////////////////////////////////
// End Platform SDK definitions
///////////////////////////////////////////////////////////////////////////



//------------------------------------------------------------------------------
// File: RefTime.h
//
// Desc: DirectShow base classes - defines CRefTime, a class that manages
//       reference times.
//
// Copyright (c) 1992-2001 Microsoft Corporation. All rights reserved.
//------------------------------------------------------------------------------


//
// CRefTime
//
// Manage reference times.
// Shares same data layout as REFERENCE_TIME, but adds some (nonvirtual)
// functions providing simple comparison, conversion and arithmetic.
//
// A reference time (at the moment) is a unit of seconds represented in
// 100ns units as is used in the Win32 FILETIME structure. BUT the time
// a REFERENCE_TIME represents is NOT the time elapsed since 1/1/1601 it
// will either be stream time or reference time depending upon context
//
// This class provides simple arithmetic operations on reference times
//
// keep non-virtual otherwise the data layout will not be the same as
// REFERENCE_TIME


// -----
// note that you are safe to cast a CRefTime* to a REFERENCE_TIME*, but
// you will need to do so explicitly
// -----


#ifndef __REFTIME__
#define __REFTIME__


const LONGLONG MILLISECONDS = (1000);            // 10 ^ 3
const LONGLONG NANOSECONDS = (1000000000);       // 10 ^ 9
const LONGLONG UNITS = (NANOSECONDS / 100);      // 10 ^ 7

/*  Unfortunately an inline function here generates a call to __allmul
    - even for constants!
*/
#define MILLISECONDS_TO_100NS_UNITS(lMs) \
    Int32x32To64((lMs), (UNITS / MILLISECONDS))

class CRefTime
{
public:

    // *MUST* be the only data member so that this class is exactly
    // equivalent to a REFERENCE_TIME.
    // Also, must be *no virtual functions*

    REFERENCE_TIME m_time;

    inline CRefTime()
    {
        // default to 0 time
        m_time = 0;
    };

    inline CRefTime(LONG msecs)
    {
        m_time = MILLISECONDS_TO_100NS_UNITS(msecs);
    };

    inline CRefTime(REFERENCE_TIME rt)
    {
        m_time = rt;
    };

    inline operator REFERENCE_TIME() const
    {
        return m_time;
    };

    inline CRefTime& operator=(const CRefTime& rt)
    {
        m_time = rt.m_time;
        return *this;
    };

    inline CRefTime& operator=(const LONGLONG ll)
    {
        m_time = ll;
        return *this;
    };

    inline CRefTime& operator+=(const CRefTime& rt)
    {
        return (*this = *this + rt);
    };

    inline CRefTime& operator-=(const CRefTime& rt)
    {
        return (*this = *this - rt);
    };

    inline LONG Millisecs(void)
    {
        return (LONG)(m_time / (UNITS / MILLISECONDS));
    };

    inline LONGLONG GetUnits(void)
    {
        return m_time;
    };
};

const LONGLONG TimeZero = 0;

#endif /* __REFTIME__ */

//------------------------------------------------------------------------------
// File: WXDebug.h
//
// Desc: DirectShow base classes - provides debugging facilities.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __WXDEBUG__
#define __WXDEBUG__

// This library provides fairly straight forward debugging functionality, this
// is split into two main sections. The first is assertion handling, there are
// three types of assertions provided here. The most commonly used one is the
// ASSERT(condition) macro which will pop up a message box including the file
// and line number if the condition evaluates to FALSE. Then there is the
// EXECUTE_ASSERT macro which is the same as ASSERT except the condition will
// still be executed in NON debug builds. The final type of assertion is the
// KASSERT macro which is more suitable for pure (perhaps kernel) filters as
// the condition is printed onto the debugger rather than in a message box.
//
// The other part of the debug module facilties is general purpose logging.
// This is accessed by calling DbgLog(). The function takes a type and level
// field which define the type of informational string you are presenting and
// it's relative importance. The type field can be a combination (one or more)
// of LOG_TIMING, LOG_TRACE, LOG_MEMORY, LOG_LOCKING and LOG_ERROR. The level
// is a DWORD value where zero defines highest important. Use of zero as the
// debug logging level is to be encouraged ONLY for major errors or events as
// they will ALWAYS be displayed on the debugger. Other debug output has it's
// level matched against the current debug output level stored in the registry
// for this module and if less than the current setting it will be displayed.
//
// Each module or executable has it's own debug output level for each of the
// five types. These are read in when the DbgInitialise function is called
// for DLLs linking to STRMBASE.LIB this is done automatically when the DLL
// is loaded, executables must call it explicitely with the module instance
// handle given to them through the WINMAIN entry point. An executable must
// also call DbgTerminate when they have finished to clean up the resources
// the debug library uses, once again this is done automatically for DLLs

// These are the five different categories of logging information

enum {  LOG_TIMING = 0x01,    // Timing and performance measurements
        LOG_TRACE = 0x02,     // General step point call tracing
        LOG_MEMORY =  0x04,   // Memory and object allocation/destruction
        LOG_LOCKING = 0x08,   // Locking/unlocking of critical sections
        LOG_ERROR = 0x10,     // Debug error notification
        LOG_CUSTOM1 = 0x20,
        LOG_CUSTOM2 = 0x40,
        LOG_CUSTOM3 = 0x80,
        LOG_CUSTOM4 = 0x100,
        LOG_CUSTOM5 = 0x200,
};

#define LOG_FORCIBLY_SET 0x80000000

enum {  CDISP_HEX = 0x01,
        CDISP_DEC = 0x02};

// For each object created derived from CBaseObject (in debug builds) we
// create a descriptor that holds it's name (statically allocated memory)
// and a cookie we assign it. We keep a list of all the active objects
// we have registered so that we can dump a list of remaining objects

typedef struct tag_ObjectDesc {
    LPCSTR m_szName;
    LPCWSTR m_wszName;
    DWORD m_dwCookie;
    tag_ObjectDesc *m_pNext;
} ObjectDesc;

#define DLLIMPORT __declspec(dllimport)
#define DLLEXPORT __declspec(dllexport)

#ifdef DEBUG

    #define NAME(x) TEXT(x)

    // These are used internally by the debug library (PRIVATE)

    void WINAPI DbgInitKeyLevels(HKEY hKey, bool fTakeMax);
    void WINAPI DbgInitGlobalSettings(bool fTakeMax);
    void WINAPI DbgInitModuleSettings(bool fTakeMax);
    void WINAPI DbgInitModuleName();
    DWORD WINAPI DbgRegisterObjectCreation(
        LPCSTR szObjectName, LPCWSTR wszObjectName);

    BOOL WINAPI DbgRegisterObjectDestruction(DWORD dwCookie);

    // These are the PUBLIC entry points

    BOOL WINAPI DbgCheckModuleLevel(DWORD Type,DWORD Level);
    void WINAPI DbgSetModuleLevel(DWORD Type,DWORD Level);
    void WINAPI DbgSetAutoRefreshLevels(bool fAuto);

    // Initialise the library with the module handle

    void WINAPI DbgInitialise(HINSTANCE hInst);
    void WINAPI DbgTerminate();

    void WINAPI DbgDumpObjectRegister();

    // Display error and logging to the user

    void WINAPI DbgAssert(LPCTSTR pCondition,LPCTSTR pFileName,INT iLine);
    void WINAPI DbgBreakPoint(LPCTSTR pCondition,LPCTSTR pFileName,INT iLine);
    void WINAPI DbgBreakPoint(LPCTSTR pFileName,INT iLine,__format_string LPCTSTR  szFormatString,...);

    void WINAPI DbgKernelAssert(LPCTSTR pCondition,LPCTSTR pFileName,INT iLine);
    void WINAPI DbgLogInfo(DWORD Type,DWORD Level,__format_string LPCTSTR pFormat,...);
#ifdef UNICODE
    void WINAPI DbgLogInfo(DWORD Type,DWORD Level,__format_string LPCSTR pFormat,...);
    void WINAPI DbgAssert(LPCSTR pCondition,LPCSTR pFileName,INT iLine);
    void WINAPI DbgBreakPoint(LPCSTR pCondition,LPCSTR pFileName,INT iLine);
    void WINAPI DbgKernelAssert(LPCSTR pCondition,LPCSTR pFileName,INT iLine);
#endif
    void WINAPI DbgOutString(LPCTSTR psz);

    //  Debug infinite wait stuff
    DWORD WINAPI DbgWaitForSingleObject(HANDLE h);
    DWORD WINAPI DbgWaitForMultipleObjects(DWORD nCount,
                                    __in_ecount(nCount) CONST HANDLE *lpHandles,
                                    BOOL bWaitAll);
    void WINAPI DbgSetWaitTimeout(DWORD dwTimeout);

#ifdef __strmif_h__
    // Display a media type: Terse at level 2, verbose at level 5
    void WINAPI DisplayType(LPCTSTR label, const AM_MEDIA_TYPE *pmtIn);

    // Dump lots of information about a filter graph
    void WINAPI DumpGraph(IFilterGraph *pGraph, DWORD dwLevel);
#endif

    #define KASSERT(_x_) if (!(_x_))         \
        DbgKernelAssert(TEXT(#_x_),TEXT(__FILE__),__LINE__)

    //  Break on the debugger without putting up a message box
    //  message goes to debugger instead

    #define KDbgBreak(_x_)                   \
        DbgKernelAssert(TEXT(#_x_),TEXT(__FILE__),__LINE__)

    // We chose a common name for our ASSERT macro, MFC also uses this name
    // So long as the implementation evaluates the condition and handles it
    // then we will be ok. Rather than override the behaviour expected we
    // will leave whatever first defines ASSERT as the handler (i.e. MFC)
    #ifndef ASSERT
        #define ASSERT(_x_) if (!(_x_))         \
            DbgAssert(TEXT(#_x_),TEXT(__FILE__),__LINE__)
    #endif

    #define DbgAssertAligned( _ptr_, _alignment_ ) ASSERT( ((DWORD_PTR) (_ptr_)) % (_alignment_) == 0)

    //  Put up a message box informing the user of a halt
    //  condition in the program

    #define DbgBreak(_x_)                   \
        DbgBreakPoint(TEXT(#_x_),TEXT(__FILE__),__LINE__)

    #define EXECUTE_ASSERT(_x_) ASSERT(_x_)
    #define DbgLog(_x_) DbgLogInfo _x_
    // MFC style trace macros

    #define NOTE(_x_)             DbgLog((LOG_TRACE,5,TEXT(_x_)))
    #define NOTE1(_x_,a)          DbgLog((LOG_TRACE,5,TEXT(_x_),a))
    #define NOTE2(_x_,a,b)        DbgLog((LOG_TRACE,5,TEXT(_x_),a,b))
    #define NOTE3(_x_,a,b,c)      DbgLog((LOG_TRACE,5,TEXT(_x_),a,b,c))
    #define NOTE4(_x_,a,b,c,d)    DbgLog((LOG_TRACE,5,TEXT(_x_),a,b,c,d))
    #define NOTE5(_x_,a,b,c,d,e)  DbgLog((LOG_TRACE,5,TEXT(_x_),a,b,c,d,e))

#else

    // Retail builds make public debug functions inert  - WARNING the source
    // files do not define or build any of the entry points in debug builds
    // (public entry points compile to nothing) so if you go trying to call
    // any of the private entry points in your source they won't compile

    #define NAME(_x_) ((LPTSTR) NULL)

    #define DbgInitialise(hInst)
    #define DbgTerminate()
    #define DbgLog(_x_) 0
    #define DbgOutString(psz)
    #define DbgAssertAligned( _ptr_, _alignment_ ) 0

    #define DbgRegisterObjectCreation(pObjectName)
    #define DbgRegisterObjectDestruction(dwCookie)
    #define DbgDumpObjectRegister()

    #define DbgCheckModuleLevel(Type,Level)
    #define DbgSetModuleLevel(Type,Level)
    #define DbgSetAutoRefreshLevels(fAuto)

    #define DbgWaitForSingleObject(h)  WaitForSingleObject(h, INFINITE)
    #define DbgWaitForMultipleObjects(nCount, lpHandles, bWaitAll)     \
               WaitForMultipleObjects(nCount, lpHandles, bWaitAll, INFINITE)
    #define DbgSetWaitTimeout(dwTimeout)

    #define KDbgBreak(_x_)
    #define DbgBreak(_x_)

    #define KASSERT(_x_) ((void)0)
    #ifndef ASSERT
	#define ASSERT(_x_) ((void)0)
    #endif
    #define EXECUTE_ASSERT(_x_) ((void)(_x_))

    // MFC style trace macros

    #define NOTE(_x_) ((void)0)
    #define NOTE1(_x_,a) ((void)0)
    #define NOTE2(_x_,a,b) ((void)0)
    #define NOTE3(_x_,a,b,c) ((void)0)
    #define NOTE4(_x_,a,b,c,d) ((void)0)
    #define NOTE5(_x_,a,b,c,d,e) ((void)0)

    #define DisplayType(label, pmtIn) ((void)0)
    #define DumpGraph(pGraph, label) ((void)0)
#endif


// Checks a pointer which should be non NULL - can be used as follows.

#define CheckPointer(p,ret) {if((p)==NULL) return (ret);}

//   HRESULT Foo(VOID *pBar)
//   {
//       CheckPointer(pBar,E_INVALIDARG)
//   }
//
//   Or if the function returns a boolean
//
//   BOOL Foo(VOID *pBar)
//   {
//       CheckPointer(pBar,FALSE)
//   }

#define ValidateReadPtr(p,cb) 0
#define ValidateWritePtr(p,cb) 0
#define ValidateReadWritePtr(p,cb) 0
#define ValidateStringPtr(p) 0
#define ValidateStringPtrA(p) 0
#define ValidateStringPtrW(p) 0


#ifdef _OBJBASE_H_

    //  Outputting GUID names.  If you want to include the name
    //  associated with a GUID (eg CLSID_...) then
    //
    //      GuidNames[yourGUID]
    //
    //  Returns the name defined in uuids.h as a string

    typedef struct {
        CHAR   *szName;
        GUID    guid;
    } GUID_STRING_ENTRY;

    class CGuidNameList {
    public:
        CHAR *operator [] (const GUID& guid);
    };

    extern CGuidNameList GuidNames;

#endif

#ifndef REMIND
    //  REMIND macro - generates warning as reminder to complete coding
    //  (eg) usage:
    //
    //  #pragma message (REMIND("Add automation support"))


    #define QUOTE(x) #x
    #define QQUOTE(y) QUOTE(y)
    #define REMIND(str) __FILE__ "(" QQUOTE(__LINE__) ") :  " str
#endif

//  Method to display objects in a useful format
//
//  eg If you want to display a LONGLONG ll in a debug string do (eg)
//
//  DbgLog((LOG_TRACE, n, TEXT("Value is %s"), (LPCTSTR)CDisp(ll, CDISP_HEX)));


class CDispBasic
{
public:
    CDispBasic() { m_pString = m_String; };
    ~CDispBasic();
protected:
    PTCHAR m_pString;  // normally points to m_String... unless too much data
    TCHAR m_String[50];
};
class CDisp : public CDispBasic
{
public:
    CDisp(LONGLONG ll, int Format = CDISP_HEX); // Display a LONGLONG in CDISP_HEX or CDISP_DEC form
    CDisp(REFCLSID clsid);      // Display a GUID
    CDisp(double d);            // Display a floating point number
#ifdef __strmif_h__
#ifdef __STREAMS__
    CDisp(CRefTime t);          // Display a Reference Time
#endif
    CDisp(IPin *pPin);          // Display a pin as {filter clsid}(pin name)
    CDisp(IUnknown *pUnk);      // Display a filter or pin
#endif // __strmif_h__
    ~CDisp();

    //  Implement cast to (LPCTSTR) as parameter to logger
    operator LPCTSTR()
    {
        return (LPCTSTR)m_pString;
    };
};


#if defined(DEBUG)
class CAutoTrace
{
private:
    LPCTSTR  _szBlkName;
    const int _level;
    static const TCHAR _szEntering[];
    static const TCHAR _szLeaving[];
public:
    CAutoTrace(LPCTSTR szBlkName, const int level = 15)
        : _szBlkName(szBlkName), _level(level)
    {DbgLog((LOG_TRACE, _level, _szEntering, _szBlkName));}

    ~CAutoTrace()
    {DbgLog((LOG_TRACE, _level, _szLeaving, _szBlkName));}
};

#if defined (__FUNCTION__)

#define AMTRACEFN()  CAutoTrace __trace(TEXT(__FUNCTION__))
#define AMTRACE(_x_) CAutoTrace __trace(TEXT(__FUNCTION__))

#else

#define AMTRACE(_x_) CAutoTrace __trace _x_
#define AMTRACEFN()

#endif

#else

#define AMTRACE(_x_)
#define AMTRACEFN()

#endif

#endif // __WXDEBUG__


//include amaudio.h explicitly if you need it.  it requires the DX SDK.
//#include <amaudio.h>    // ActiveMovie audio interfaces and definitions
//------------------------------------------------------------------------------
// File: WXUtil.h
//
// Desc: DirectShow base classes - defines helper classes and functions for
//       building multimedia filters.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __WXUTIL__
#define __WXUTIL__

// eliminate spurious "statement has no effect" warnings.
#pragma warning(disable: 4705)

// wrapper for whatever critical section we have
class CCritSec {

    // make copy constructor and assignment operator inaccessible

    CCritSec(const CCritSec &refCritSec);
    CCritSec &operator=(const CCritSec &refCritSec);

    CRITICAL_SECTION m_CritSec;

#ifdef DEBUG
public:
    DWORD   m_currentOwner;
    DWORD   m_lockCount;
    BOOL    m_fTrace;        // Trace this one
public:
    CCritSec();
    ~CCritSec();
    void Lock();
    void Unlock();
#else

public:
    CCritSec() {
        InitializeCriticalSection(&m_CritSec);
    };

    ~CCritSec() {
        DeleteCriticalSection(&m_CritSec);
    };

    void Lock() {
        EnterCriticalSection(&m_CritSec);
    };

    void Unlock() {
        LeaveCriticalSection(&m_CritSec);
    };
#endif
};

//
// To make deadlocks easier to track it is useful to insert in the
// code an assertion that says whether we own a critical section or
// not.  We make the routines that do the checking globals to avoid
// having different numbers of member functions in the debug and
// retail class implementations of CCritSec.  In addition we provide
// a routine that allows usage of specific critical sections to be
// traced.  This is NOT on by default - there are far too many.
//

#ifdef DEBUG
    BOOL WINAPI CritCheckIn(CCritSec * pcCrit);
    BOOL WINAPI CritCheckIn(const CCritSec * pcCrit);
    BOOL WINAPI CritCheckOut(CCritSec * pcCrit);
    BOOL WINAPI CritCheckOut(const CCritSec * pcCrit);
    void WINAPI DbgLockTrace(CCritSec * pcCrit, BOOL fTrace);
#else
    #define CritCheckIn(x) TRUE
    #define CritCheckOut(x) TRUE
    #define DbgLockTrace(pc, fT)
#endif


// locks a critical section, and unlocks it automatically
// when the lock goes out of scope
class CAutoLock {

    // make copy constructor and assignment operator inaccessible

    CAutoLock(const CAutoLock &refAutoLock);
    CAutoLock &operator=(const CAutoLock &refAutoLock);

protected:
    CCritSec * m_pLock;

public:
    CAutoLock(CCritSec * plock)
    {
        m_pLock = plock;
        m_pLock->Lock();
    };

    ~CAutoLock() {
        m_pLock->Unlock();
    };
};



// wrapper for event objects
class CAMEvent
{

    // make copy constructor and assignment operator inaccessible

    CAMEvent(const CAMEvent &refEvent);
    CAMEvent &operator=(const CAMEvent &refEvent);

protected:
    HANDLE m_hEvent;
public:
    CAMEvent(BOOL fManualReset = FALSE, __inout_opt HRESULT *phr = NULL);
    CAMEvent(__inout_opt HRESULT *phr);
    ~CAMEvent();

    // Cast to HANDLE - we don't support this as an lvalue
    operator HANDLE () const { return m_hEvent; };

    void Set() {EXECUTE_ASSERT(SetEvent(m_hEvent));};
    BOOL Wait(DWORD dwTimeout = INFINITE) {
	return (WaitForSingleObject(m_hEvent, dwTimeout) == WAIT_OBJECT_0);
    };
    void Reset() { ResetEvent(m_hEvent); };
    BOOL Check() { return Wait(0); };
};


// wrapper for event objects that do message processing
// This adds ONE method to the CAMEvent object to allow sent
// messages to be processed while waiting

class CAMMsgEvent : public CAMEvent
{

public:

    CAMMsgEvent(__inout_opt HRESULT *phr = NULL);

    // Allow SEND messages to be processed while waiting
    BOOL WaitMsg(DWORD dwTimeout = INFINITE);
};

// old name supported for the time being
#define CTimeoutEvent CAMEvent

// support for a worker thread

#ifdef AM_NOVTABLE
// simple thread class supports creation of worker thread, synchronization
// and communication. Can be derived to simplify parameter passing
class AM_NOVTABLE CAMThread {

    // make copy constructor and assignment operator inaccessible

    CAMThread(const CAMThread &refThread);
    CAMThread &operator=(const CAMThread &refThread);

    CAMEvent m_EventSend;
    CAMEvent m_EventComplete;

    DWORD m_dwParam;
    DWORD m_dwReturnVal;

protected:
    HANDLE m_hThread;

    // thread will run this function on startup
    // must be supplied by derived class
    virtual DWORD ThreadProc() = 0;

public:
    CAMThread(__inout_opt HRESULT *phr = NULL);
    virtual ~CAMThread();

    CCritSec m_AccessLock;	// locks access by client threads
    CCritSec m_WorkerLock;	// locks access to shared objects

    // thread initially runs this. param is actually 'this'. function
    // just gets this and calls ThreadProc
    static DWORD WINAPI InitialThreadProc(__inout LPVOID pv);

    // start thread running  - error if already running
    BOOL Create();

    // signal the thread, and block for a response
    //
    DWORD CallWorker(DWORD);

    // accessor thread calls this when done with thread (having told thread
    // to exit)
    void Close() {

        // Disable warning: Conversion from LONG to PVOID of greater size
#pragma warning(push)
#pragma warning(disable: 4312)
        HANDLE hThread = (HANDLE)InterlockedExchangePointer(&m_hThread, 0);
#pragma warning(pop)

        if (hThread) {
            WaitForSingleObject(hThread, INFINITE);
            CloseHandle(hThread);
        }
    };

    // ThreadExists
    // Return TRUE if the thread exists. FALSE otherwise
    BOOL ThreadExists(void) const
    {
        if (m_hThread == 0) {
            return FALSE;
        } else {
            return TRUE;
        }
    }

    // wait for the next request
    DWORD GetRequest();

    // is there a request?
    BOOL CheckRequest(__out_opt DWORD * pParam);

    // reply to the request
    void Reply(DWORD);

    // If you want to do WaitForMultipleObjects you'll need to include
    // this handle in your wait list or you won't be responsive
    HANDLE GetRequestHandle() const { return m_EventSend; };

    // Find out what the request was
    DWORD GetRequestParam() const { return m_dwParam; };

    // call CoInitializeEx (COINIT_DISABLE_OLE1DDE) if
    // available. S_FALSE means it's not available.
    static HRESULT CoInitializeHelper();
};
#endif // AM_NOVTABLE


// CQueue
//
// Implements a simple Queue ADT.  The queue contains a finite number of
// objects, access to which is controlled by a semaphore.  The semaphore
// is created with an initial count (N).  Each time an object is added
// a call to WaitForSingleObject is made on the semaphore's handle.  When
// this function returns a slot has been reserved in the queue for the new
// object.  If no slots are available the function blocks until one becomes
// available.  Each time an object is removed from the queue ReleaseSemaphore
// is called on the semaphore's handle, thus freeing a slot in the queue.
// If no objects are present in the queue the function blocks until an
// object has been added.

#define DEFAULT_QUEUESIZE   2

template <class T> class CQueue {
private:
    HANDLE          hSemPut;        // Semaphore controlling queue "putting"
    HANDLE          hSemGet;        // Semaphore controlling queue "getting"
    CRITICAL_SECTION CritSect;      // Thread seriallization
    int             nMax;           // Max objects allowed in queue
    int             iNextPut;       // Array index of next "PutMsg"
    int             iNextGet;       // Array index of next "GetMsg"
    T              *QueueObjects;   // Array of objects (ptr's to void)

    void Initialize(int n) {
        iNextPut = iNextGet = 0;
        nMax = n;
        InitializeCriticalSection(&CritSect);
        hSemPut = CreateSemaphore(NULL, n, n, NULL);
        hSemGet = CreateSemaphore(NULL, 0, n, NULL);
        QueueObjects = new T[n];
    }


public:
    CQueue(int n) {
        Initialize(n);
    }

    CQueue() {
        Initialize(DEFAULT_QUEUESIZE);
    }

    ~CQueue() {
        delete [] QueueObjects;
        DeleteCriticalSection(&CritSect);
        CloseHandle(hSemPut);
        CloseHandle(hSemGet);
    }

    T GetQueueObject() {
        int iSlot;
        T Object;
        LONG lPrevious;

        // Wait for someone to put something on our queue, returns straight
        // away is there is already an object on the queue.
        //
        WaitForSingleObject(hSemGet, INFINITE);

        EnterCriticalSection(&CritSect);
        iSlot = iNextGet++ % nMax;
        Object = QueueObjects[iSlot];
        LeaveCriticalSection(&CritSect);

        // Release anyone waiting to put an object onto our queue as there
        // is now space available in the queue.
        //
        ReleaseSemaphore(hSemPut, 1L, &lPrevious);
        return Object;
    }

    void PutQueueObject(T Object) {
        int iSlot;
        LONG lPrevious;

        // Wait for someone to get something from our queue, returns straight
        // away is there is already an empty slot on the queue.
        //
        WaitForSingleObject(hSemPut, INFINITE);

        EnterCriticalSection(&CritSect);
        iSlot = iNextPut++ % nMax;
        QueueObjects[iSlot] = Object;
        LeaveCriticalSection(&CritSect);

        // Release anyone waiting to remove an object from our queue as there
        // is now an object available to be removed.
        //
        ReleaseSemaphore(hSemGet, 1L, &lPrevious);
    }
};

// Ensures that memory is not read past the length source buffer
// and that memory is not written past the length of the dst buffer
//   dst - buffer to copy to
//   dst_size - total size of destination buffer
//   cb_dst_offset - offset, first byte copied to dst+cb_dst_offset
//   src - buffer to copy from
//   src_size - total size of source buffer
//   cb_src_offset - offset, first byte copied from src+cb_src_offset
//   count - number of bytes to copy
//
// Returns:
//    S_OK          - no error
//    E_INVALIDARG  - values passed would lead to overrun
HRESULT AMSafeMemMoveOffset(
    __in_bcount(dst_size) void * dst,
    __in size_t dst_size,
    __in DWORD cb_dst_offset,
    __in_bcount(src_size) const void * src,
    __in size_t src_size,
    __in DWORD cb_src_offset,
    __in size_t count);

extern "C"
void * __stdcall memmoveInternal(void *, const void *, size_t);

inline void * __cdecl memchrInternal(const void *buf, int chr, size_t cnt)
{
#ifdef _X86_
    void *pRet = NULL;

    _asm {
        cld                 // make sure we get the direction right
        mov     ecx, cnt    // num of bytes to scan
        mov     edi, buf    // pointer byte stream
        mov     eax, chr    // byte to scan for
        repne   scasb       // look for the byte in the byte stream
        jnz     exit_memchr // Z flag set if byte found
        dec     edi         // scasb always increments edi even when it
                            // finds the required byte
        mov     pRet, edi
exit_memchr:
    }
    return pRet;

#else
    while ( cnt && (*(unsigned char *)buf != (unsigned char)chr) ) {
        buf = (unsigned char *)buf + 1;
        cnt--;
    }

    return(cnt ? (void *)buf : NULL);
#endif
}

void WINAPI IntToWstr(int i, __out_ecount(12) LPWSTR wstr);

#define WstrToInt(sz) _wtoi(sz)
#define atoiW(sz) _wtoi(sz)
#define atoiA(sz) atoi(sz)

// These are available to help managing bitmap VIDEOINFOHEADER media structures

extern const DWORD bits555[3];
extern const DWORD bits565[3];
extern const DWORD bits888[3];

// These help convert between VIDEOINFOHEADER and BITMAPINFO structures

STDAPI_(const GUID) GetTrueColorType(const BITMAPINFOHEADER *pbmiHeader);
STDAPI_(const GUID) GetBitmapSubtype(const BITMAPINFOHEADER *pbmiHeader);
STDAPI_(WORD) GetBitCount(const GUID *pSubtype);

// strmbase.lib implements this for compatibility with people who
// managed to link to this directly.  we don't want to advertise it.
//
// STDAPI_(/* T */ CHAR *) GetSubtypeName(const GUID *pSubtype);

STDAPI_(CHAR *) GetSubtypeNameA(const GUID *pSubtype);
STDAPI_(WCHAR *) GetSubtypeNameW(const GUID *pSubtype);

#ifdef UNICODE
#define GetSubtypeName GetSubtypeNameW
#else
#define GetSubtypeName GetSubtypeNameA
#endif

STDAPI_(LONG) GetBitmapFormatSize(const BITMAPINFOHEADER *pHeader);
STDAPI_(DWORD) GetBitmapSize(const BITMAPINFOHEADER *pHeader);

#ifdef __AMVIDEO__
STDAPI_(BOOL) ContainsPalette(const VIDEOINFOHEADER *pVideoInfo);
STDAPI_(const RGBQUAD *) GetBitmapPalette(const VIDEOINFOHEADER *pVideoInfo);
#endif // __AMVIDEO__


// Compares two interfaces and returns TRUE if they are on the same object
BOOL WINAPI IsEqualObject(IUnknown *pFirst, IUnknown *pSecond);

// This is for comparing pins
#define EqualPins(pPin1, pPin2) IsEqualObject(pPin1, pPin2)


// Arithmetic helper functions

// Compute (a * b + rnd) / c
LONGLONG WINAPI llMulDiv(LONGLONG a, LONGLONG b, LONGLONG c, LONGLONG rnd);
LONGLONG WINAPI Int64x32Div32(LONGLONG a, LONG b, LONG c, LONG rnd);


// Avoids us dyna-linking to SysAllocString to copy BSTR strings
STDAPI WriteBSTR(__deref_out BSTR * pstrDest, LPCWSTR szSrc);
STDAPI FreeBSTR(__deref_in BSTR* pstr);

// Return a wide string - allocating memory for it
// Returns:
//    S_OK          - no error
//    E_POINTER     - ppszReturn == NULL
//    E_OUTOFMEMORY - can't allocate memory for returned string
STDAPI AMGetWideString(LPCWSTR pszString, __deref_out LPWSTR *ppszReturn);

// Special wait for objects owning windows
DWORD WINAPI WaitDispatchingMessages(
    HANDLE hObject,
    DWORD dwWait,
    HWND hwnd = NULL,
    UINT uMsg = 0,
    HANDLE hEvent = NULL);

// HRESULT_FROM_WIN32 converts ERROR_SUCCESS to a success code, but in
// our use of HRESULT_FROM_WIN32, it typically means a function failed
// to call SetLastError(), and we still want a failure code.
//
#define AmHresultFromWin32(x) (MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, x))

// call GetLastError and return an HRESULT value that will fail the
// SUCCEEDED() macro.
HRESULT AmGetLastErrorToHResult(void);

// duplicate of ATL's CComPtr to avoid linker conflicts.

IUnknown* QzAtlComPtrAssign(__deref_inout_opt IUnknown** pp, __in_opt IUnknown* lp);

template <class T>
class QzCComPtr
{
public:
	typedef T _PtrClass;
	QzCComPtr() {p=NULL;}
	QzCComPtr(T* lp)
	{
		if ((p = lp) != NULL)
			p->AddRef();
	}
	QzCComPtr(const QzCComPtr<T>& lp)
	{
		if ((p = lp.p) != NULL)
			p->AddRef();
	}
	~QzCComPtr() {if (p) p->Release();}
	void Release() {if (p) p->Release(); p=NULL;}
	operator T*() {return (T*)p;}
	T& operator*() {ASSERT(p!=NULL); return *p; }
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&() { ASSERT(p==NULL); return &p; }
	T* operator->() { ASSERT(p!=NULL); return p; }
	T* operator=(T* lp){return (T*)QzAtlComPtrAssign((IUnknown**)&p, lp);}
	T* operator=(const QzCComPtr<T>& lp)
	{
		return (T*)QzAtlComPtrAssign((IUnknown**)&p, lp.p);
	}
#if _MSC_VER>1020
	bool operator!(){return (p == NULL);}
#else
	BOOL operator!(){return (p == NULL) ? TRUE : FALSE;}
#endif
	T* p;
};

MMRESULT CompatibleTimeSetEvent( UINT uDelay, UINT uResolution, __in LPTIMECALLBACK lpTimeProc, DWORD_PTR dwUser, UINT fuEvent );
bool TimeKillSynchronousFlagAvailable( void );

//  Helper to replace lstrcpmi
__inline int lstrcmpiLocaleIndependentW(LPCWSTR lpsz1, LPCWSTR lpsz2)
{
    return  CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE, lpsz1, -1, lpsz2, -1) - CSTR_EQUAL;
}
__inline int lstrcmpiLocaleIndependentA(LPCSTR lpsz1, LPCSTR lpsz2)
{
    return  CompareStringA(LOCALE_INVARIANT, NORM_IGNORECASE, lpsz1, -1, lpsz2, -1) - CSTR_EQUAL;
}

#endif /* __WXUTIL__ */
//------------------------------------------------------------------------------
// File: ComBase.h
//
// Desc: DirectShow base classes - defines a class hierarchy for creating
//       COM objects.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


/*

a. Derive your COM object from CUnknown

b. Make a static CreateInstance function that takes an LPUNKNOWN, an HRESULT *
   and a TCHAR *. The LPUNKNOWN defines the object to delegate IUnknown calls
   to. The HRESULT * allows error codes to be passed around constructors and
   the TCHAR * is a descriptive name that can be printed on the debugger.

   It is important that constructors only change the HRESULT * if they have
   to set an ERROR code, if it was successful then leave it alone or you may
   overwrite an error code from an object previously created.

   When you call a constructor the descriptive name should be in static store
   as we do not copy the string. To stop large amounts of memory being used
   in retail builds by all these static strings use the NAME macro,

   CMyFilter = new CImplFilter(NAME("My filter"),pUnknown,phr);
   if (FAILED(hr)) {
       return hr;
   }

   In retail builds NAME(_x_) compiles to NULL, the base CBaseObject class
   knows not to do anything with objects that don't have a name.

c. Have a constructor for your object that passes the LPUNKNOWN, HRESULT * and
   TCHAR * to the CUnknown constructor. You can set the HRESULT if you have an
   error, or just simply pass it through to the constructor.

   The object creation will fail in the class factory if the HRESULT indicates
   an error (ie FAILED(HRESULT) == TRUE)

d. Create a FactoryTemplate with your object's class id and CreateInstance
   function.

Then (for each interface) either

Multiple inheritance

1. Also derive it from ISomeInterface
2. Include DECLARE_IUNKNOWN in your class definition to declare
   implementations of QueryInterface, AddRef and Release that
   call the outer unknown
3. Override NonDelegatingQueryInterface to expose ISomeInterface by
   code something like

     if (riid == IID_ISomeInterface) {
         return GetInterface((ISomeInterface *) this, ppv);
     } else {
         return CUnknown::NonDelegatingQueryInterface(riid, ppv);
     }

4. Declare and implement the member functions of ISomeInterface.

or: Nested interfaces

1. Declare a class derived from CUnknown
2. Include DECLARE_IUNKNOWN in your class definition
3. Override NonDelegatingQueryInterface to expose ISomeInterface by
   code something like

     if (riid == IID_ISomeInterface) {
         return GetInterface((ISomeInterface *) this, ppv);
     } else {
         return CUnknown::NonDelegatingQueryInterface(riid, ppv);
     }

4. Implement the member functions of ISomeInterface. Use GetOwner() to
   access the COM object class.

And in your COM object class:

5. Make the nested class a friend of the COM object class, and declare
   an instance of the nested class as a member of the COM object class.

   NOTE that because you must always pass the outer unknown and an hResult
   to the CUnknown constructor you cannot use a default constructor, in
   other words you will have to make the member variable a pointer to the
   class and make a NEW call in your constructor to actually create it.

6. override the NonDelegatingQueryInterface with code like this:

     if (riid == IID_ISomeInterface) {
         return m_pImplFilter->
            NonDelegatingQueryInterface(IID_ISomeInterface, ppv);
     } else {
         return CUnknown::NonDelegatingQueryInterface(riid, ppv);
     }

You can have mixed classes which support some interfaces via multiple
inheritance and some via nested classes

*/

#ifndef __COMBASE__
#define __COMBASE__

// Filter Setup data structures no defined in axextend.idl

typedef REGPINTYPES
AMOVIESETUP_MEDIATYPE, * PAMOVIESETUP_MEDIATYPE, * FAR LPAMOVIESETUP_MEDIATYPE;

typedef REGFILTERPINS
AMOVIESETUP_PIN, * PAMOVIESETUP_PIN, * FAR LPAMOVIESETUP_PIN;

typedef struct _AMOVIESETUP_FILTER
{
  const CLSID * clsID;
  const WCHAR * strName;
  DWORD      dwMerit;
  UINT       nPins;
  const AMOVIESETUP_PIN * lpPin;
}
AMOVIESETUP_FILTER, * PAMOVIESETUP_FILTER, * FAR LPAMOVIESETUP_FILTER;

/* The DLLENTRY module initialises the module handle on loading */

extern HINSTANCE g_hInst;

/* On DLL load remember which platform we are running on */

extern DWORD g_amPlatform;
extern OSVERSIONINFO g_osInfo;     // Filled in by GetVersionEx

/* Version of IUnknown that is renamed to allow a class to support both
   non delegating and delegating IUnknowns in the same COM object */

#ifndef INONDELEGATINGUNKNOWN_DEFINED
DECLARE_INTERFACE(INonDelegatingUnknown)
{
    STDMETHOD(NonDelegatingQueryInterface) (THIS_ REFIID, LPVOID *) PURE;
    STDMETHOD_(ULONG, NonDelegatingAddRef)(THIS) PURE;
    STDMETHOD_(ULONG, NonDelegatingRelease)(THIS) PURE;
};
#define INONDELEGATINGUNKNOWN_DEFINED
#endif

typedef INonDelegatingUnknown *PNDUNKNOWN;


/* This is the base object class that supports active object counting. As
   part of the debug facilities we trace every time a C++ object is created
   or destroyed. The name of the object has to be passed up through the class
   derivation list during construction as you cannot call virtual functions
   in the constructor. The downside of all this is that every single object
   constructor has to take an object name parameter that describes it */

class CBaseObject
{

private:

    // Disable the copy constructor and assignment by default so you will get
    //   compiler errors instead of unexpected behaviour if you pass objects
    //   by value or assign objects.
    CBaseObject(const CBaseObject& objectSrc);          // no implementation
    void operator=(const CBaseObject& objectSrc);       // no implementation

private:
    static LONG m_cObjects;     /* Total number of objects active */

protected:
#ifdef DEBUG
    DWORD m_dwCookie;           /* Cookie identifying this object */
#endif


public:

    /* These increment and decrement the number of active objects */

    CBaseObject(__in_opt LPCTSTR pName);
#ifdef UNICODE
    CBaseObject(__in_opt LPCSTR pName);
#endif
    ~CBaseObject();

    /* Call this to find if there are any CUnknown derived objects active */

    static LONG ObjectsActive() {
        return m_cObjects;
    };
};


/* An object that supports one or more COM interfaces will be based on
   this class. It supports counting of total objects for DLLCanUnloadNow
   support, and an implementation of the core non delegating IUnknown */

class AM_NOVTABLE CUnknown : public INonDelegatingUnknown,
                 public CBaseObject
{
private:
    const LPUNKNOWN m_pUnknown; /* Owner of this object */

protected:                      /* So we can override NonDelegatingRelease() */
    volatile LONG m_cRef;       /* Number of reference counts */

public:

    CUnknown(__in_opt LPCTSTR pName, __in_opt LPUNKNOWN pUnk);
    virtual ~CUnknown() {};

    // This is redundant, just use the other constructor
    //   as we never touch the HRESULT in this anyway
    CUnknown(__in_opt LPCTSTR Name, __in_opt LPUNKNOWN pUnk, __inout_opt HRESULT *phr);
#ifdef UNICODE
    CUnknown(__in_opt LPCSTR pName, __in_opt LPUNKNOWN pUnk);
    CUnknown(__in_opt LPCSTR pName, __in_opt LPUNKNOWN pUnk,__inout_opt HRESULT *phr);
#endif

    /* Return the owner of this object */

    LPUNKNOWN GetOwner() const {
        return m_pUnknown;
    };

    /* Called from the class factory to create a new instance, it is
       pure virtual so it must be overriden in your derived class */

    /* static CUnknown *CreateInstance(LPUNKNOWN, HRESULT *) */

    /* Non delegating unknown implementation */

    STDMETHODIMP NonDelegatingQueryInterface(REFIID, __deref_out void **);
    STDMETHODIMP_(ULONG) NonDelegatingAddRef();
    STDMETHODIMP_(ULONG) NonDelegatingRelease();
};

/* Return an interface pointer to a requesting client
   performing a thread safe AddRef as necessary */

STDAPI GetInterface(LPUNKNOWN pUnk, __out void **ppv);

/* A function that can create a new COM object */

typedef CUnknown *(CALLBACK *LPFNNewCOMObject)(__in_opt LPUNKNOWN pUnkOuter, __inout_opt HRESULT *phr);

/*  A function (can be NULL) which is called from the DLL entrypoint
    routine for each factory template:

    bLoading - TRUE on DLL load, FALSE on DLL unload
    rclsid   - the m_ClsID of the entry
*/
typedef void (CALLBACK *LPFNInitRoutine)(BOOL bLoading, const CLSID *rclsid);

/* Create one of these per object class in an array so that
   the default class factory code can create new instances */

class CFactoryTemplate {

public:

    const WCHAR *              m_Name;
    const CLSID *              m_ClsID;
    LPFNNewCOMObject           m_lpfnNew;
    LPFNInitRoutine            m_lpfnInit;
    const AMOVIESETUP_FILTER * m_pAMovieSetup_Filter;

    BOOL IsClassID(REFCLSID rclsid) const {
        return (IsEqualCLSID(*m_ClsID,rclsid));
    };

    CUnknown *CreateInstance(__inout_opt LPUNKNOWN pUnk, __inout_opt HRESULT *phr) const {
        CheckPointer(phr,NULL);
        return m_lpfnNew(pUnk, phr);
    };
};


/* You must override the (pure virtual) NonDelegatingQueryInterface to return
   interface pointers (using GetInterface) to the interfaces your derived
   class supports (the default implementation only supports IUnknown) */

#define DECLARE_IUNKNOWN                                        \
    STDMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppv) {      \
        return GetOwner()->QueryInterface(riid,ppv);            \
    };                                                          \
    STDMETHODIMP_(ULONG) AddRef() {                             \
        return GetOwner()->AddRef();                            \
    };                                                          \
    STDMETHODIMP_(ULONG) Release() {                            \
        return GetOwner()->Release();                           \
    };



HINSTANCE	LoadOLEAut32();


#endif /* __COMBASE__ */




//------------------------------------------------------------------------------
// File: DllSetup.h
//
// Desc: DirectShow base classes.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


// To be self registering, OLE servers must
// export functions named DllRegisterServer
// and DllUnregisterServer.  To allow use of
// custom and default implementations the
// defaults are named AMovieDllRegisterServer
// and AMovieDllUnregisterServer.
//
// To the use the default implementation you
// must provide stub functions.
//
// i.e. STDAPI DllRegisterServer()
//      {
//        return AMovieDllRegisterServer();
//      }
//
//      STDAPI DllUnregisterServer()
//      {
//        return AMovieDllUnregisterServer();
//      }
//
//
// AMovieDllRegisterServer   calls IAMovieSetup.Register(), and
// AMovieDllUnregisterServer calls IAMovieSetup.Unregister().

STDAPI AMovieDllRegisterServer2( BOOL );
STDAPI AMovieDllRegisterServer();
STDAPI AMovieDllUnregisterServer();

// helper functions
STDAPI EliminateSubKey( HKEY, LPCTSTR );


STDAPI
AMovieSetupRegisterFilter2( const AMOVIESETUP_FILTER * const psetupdata
                          , IFilterMapper2 *         pIFM2
                          , BOOL                             bRegister  );

//------------------------------------------------------------------------------
// File: Measure.h
//
// Desc: DirectShow base classes.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


/*
   The idea is to pepper the source code with interesting measurements and
   have the last few thousand of these recorded in a circular buffer that
   can be post-processed to give interesting numbers.

   WHAT THE LOG LOOKS LIKE:

  Time (sec)   Type        Delta  Incident_Name
    0.055,41  NOTE      -.       Incident Nine  - Another note
    0.055,42  NOTE      0.000,01 Incident Nine  - Another note
    0.055,44  NOTE      0.000,02 Incident Nine  - Another note
    0.055,45  STOP      -.       Incident Eight - Also random
    0.055,47  START     -.       Incident Seven - Random
    0.055,49  NOTE      0.000,05 Incident Nine  - Another note
    ------- <etc.  there is a lot of this> ----------------
    0.125,60  STOP      0.000,03 Msr_Stop
    0.125,62  START     -.       Msr_Start
    0.125,63  START     -.       Incident Two   - Start/Stop
    0.125,65  STOP      0.000,03 Msr_Start
    0.125,66  START     -.       Msr_Stop
    0.125,68  STOP      0.000,05 Incident Two   - Start/Stop
    0.125,70  STOP      0.000,04 Msr_Stop
    0.125,72  START     -.       Msr_Start
    0.125,73  START     -.       Incident Two   - Start/Stop
    0.125,75  STOP      0.000,03 Msr_Start
    0.125,77  START     -.       Msr_Stop
    0.125,78  STOP      0.000,05 Incident Two   - Start/Stop
    0.125,80  STOP      0.000,03 Msr_Stop
    0.125,81  NOTE      -.       Incident Three - single Note
    0.125,83  START     -.       Incident Four  - Start, no stop
    0.125,85  START     -.       Incident Five  - Single Start/Stop
    0.125,87  STOP      0.000,02 Incident Five  - Single Start/Stop

Number      Average       StdDev     Smallest      Largest Incident_Name
    10     0.000,58     0.000,10     0.000,55     0.000,85 Incident One   - Note
    50     0.000,05     0.000,00     0.000,05     0.000,05 Incident Two   - Start/Stop
     1     -.           -.           -.           -.       Incident Three - single Note
     0     -.           -.           -.           -.       Incident Four  - Start, no stop
     1     0.000,02     -.           0.000,02     0.000,02 Incident Five  - Single Start/Stop
     0     -.           -.           -.           -.       Incident Six   - zero occurrences
   100     0.000,25     0.000,12     0.000,02     0.000,62 Incident Seven - Random
   100     0.000,79     0.000,48     0.000,02     0.001,92 Incident Eight - Also random
  5895     0.000,01     0.000,01     0.000,01     0.000,56 Incident Nine  - Another note
    10     0.000,03     0.000,00     0.000,03     0.000,04 Msr_Note
    50     0.000,03     0.000,00     0.000,03     0.000,04 Msr_Start
    50     0.000,04     0.000,03     0.000,03     0.000,31 Msr_Stop

  WHAT IT MEANS:
    The log shows what happened and when.  Each line shows the time at which
    something happened (see WHAT YOU CODE below) what it was that happened
    and (if approporate) the time since the corresponding previous event
    (that's the delta column).

    The statistics show how many times each event occurred, what the average
    delta time was, also the standard deviation, largest and smalles delta.

   WHAT YOU CODE:

   Before anything else executes: - register your ids

    int id1     = Msr_Register("Incident One   - Note");
    int id2     = Msr_Register("Incident Two   - Start/Stop");
    int id3     = Msr_Register("Incident Three - single Note");
    etc.

   At interesting moments:

       // To measure a repetitive event - e.g. end of bitblt to screen
       Msr_Note(Id9);             // e.g. "video frame hiting the screen NOW!"

           or

       // To measure an elapsed time e.g. time taken to decode an MPEG B-frame
       Msr_Start(Id2);            // e.g. "Starting to decode MPEG B-frame"
         . . .
       MsrStop(Id2);              //      "Finished MPEG decode"

   At the end:

       HANDLE hFile;
       hFile = CreateFile("Perf.log", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
       Msr_Dump(hFile);           // This writes the log out to the file
       CloseHandle(hFile);

           or

       Msr_Dump(NULL);            // This writes it to DbgLog((LOG_TRACE,0, ... ));
                                  // but if you are writing it out to the debugger
                                  // then the times are probably all garbage because
                                  // the debugger can make things run awfully slow.

    A given id should be used either for start / stop or Note calls.  If Notes
    are mixed in with Starts and Stops their statistics will be gibberish.

    If you code the calls in upper case i.e. MSR_START(idMunge); then you get
    macros which will turn into nothing unless PERF is defined.

    You can reset the statistical counts for a given id by calling Reset(Id).
    They are reset by default at the start.
    It logs Reset as a special incident, so you can see it in the log.

    The log is a circular buffer in storage (to try to minimise disk I/O).
    It overwrites the oldest entries once full.  The statistics include ALL
    incidents since the last Reset, whether still visible in the log or not.
*/

#ifndef __MEASURE__
#define __MEASURE__

#ifdef PERF
#define MSR_INIT() Msr_Init()
#define MSR_TERMINATE() Msr_Terminate()
#define MSR_REGISTER(a) Msr_Register(a)
#define MSR_RESET(a) Msr_Reset(a)
#define MSR_CONTROL(a) Msr_Control(a)
#define MSR_START(a) Msr_Start(a)
#define MSR_STOP(a) Msr_Stop(a)
#define MSR_NOTE(a) Msr_Note(a)
#define MSR_INTEGER(a,b) Msr_Integer(a,b)
#define MSR_DUMP(a) Msr_Dump(a)
#define MSR_DUMPSTATS(a) Msr_DumpStats(a)
#else
#define MSR_INIT() ((void)0)
#define MSR_TERMINATE() ((void)0)
#define MSR_REGISTER(a) 0
#define MSR_RESET(a) ((void)0)
#define MSR_CONTROL(a) ((void)0)
#define MSR_START(a) ((void)0)
#define MSR_STOP(a) ((void)0)
#define MSR_NOTE(a) ((void)0)
#define MSR_INTEGER(a,b) ((void)0)
#define MSR_DUMP(a) ((void)0)
#define MSR_DUMPSTATS(a) ((void)0)
#endif

#ifdef __cplusplus
extern "C" {
#endif

// This must be called first - (called by the DllEntry)

void WINAPI Msr_Init(void);


// Call this last to clean up (or just let it fall off the end - who cares?)

void WINAPI Msr_Terminate(void);


// Call this to get an Id for an "incident" that you can pass to Start, Stop or Note
// everything that's logged is called an "incident".

int  WINAPI Msr_Register(__in LPTSTR Incident);


// Reset the statistical counts for an incident

void WINAPI Msr_Reset(int Id);


// Reset all the counts for all incidents
#define MSR_RESET_ALL 0
#define MSR_PAUSE 1
#define MSR_RUN 2

void WINAPI Msr_Control(int iAction);


// log the start of an operation

void WINAPI Msr_Start(int Id);


// log the end of an operation

void WINAPI Msr_Stop(int Id);


// log a one-off or repetitive operation

void WINAPI Msr_Note(int Id);


// log an integer (on which we can see statistics later)
void WINAPI Msr_Integer(int Id, int n);


// print out all the vaialable log (it may have wrapped) and then the statistics.
// When the log wraps you lose log but the statistics are still complete.
// hFIle==NULL => use DbgLog
// otherwise hFile must have come from CreateFile or OpenFile.

void WINAPI Msr_Dump(HANDLE hFile);


// just dump the statistics - never mind the log

void WINAPI Msr_DumpStats(HANDLE hFile);

// Type definitions in case you want to declare a pointer to the dump functions
// (makes it a trifle easier to do dynamic linking
// i.e. LoadModule, GetProcAddress and call that)

// Typedefs so can declare MSR_DUMPPROC *MsrDumpStats; or whatever
typedef void WINAPI MSR_DUMPPROC(HANDLE hFile);
typedef void WINAPI MSR_CONTROLPROC(int iAction);


#ifdef __cplusplus
}
#endif

#endif // __MEASURE__

//------------------------------------------------------------------------------
// File: Cache.h
//
// Desc: DirectShow base classes - efines a non-MFC generic cache class.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


/* This class implements a simple cache. A cache object is instantiated
   with the number of items it is to hold. An item is a pointer to an
   object derived from CBaseObject (helps reduce memory leaks). The cache
   can then have objects added to it and removed from it. The cache size
   is fixed at construction time and may therefore run out or be flooded.
   If it runs out it returns a NULL pointer, if it fills up it also returns
   a NULL pointer instead of a pointer to the object just inserted */

/* Making these classes inherit from CBaseObject does nothing for their
   functionality but it allows us to check there are no memory leaks */

/* WARNING Be very careful when using this class, what it lets you do is
   store and retrieve objects so that you can minimise object creation
   which in turns improves efficiency. However the object you store is
   exactly the same as the object you get back which means that it short
   circuits the constructor initialisation phase. This means any class
   variables the object has (eg pointers) are highly likely to be invalid.
   Therefore ensure you reinitialise the object before using it again */


#ifndef __CACHE__
#define __CACHE__


class CCache : CBaseObject {

    /* Make copy constructor and assignment operator inaccessible */

    CCache(const CCache &refCache);
    CCache &operator=(const CCache &refCache);

private:

    /* These are initialised in the constructor. The first variable points to
       an array of pointers, each of which points to a CBaseObject derived
       object. The m_iCacheSize is the static fixed size for the cache and the
       m_iUsed defines the number of places filled with objects at any time.
       We fill the array of pointers from the start (ie m_ppObjects[0] first)
       and then only add and remove objects from the end position, so in this
       respect the array of object pointers should be treated as a stack */

    CBaseObject **m_ppObjects;
    const INT m_iCacheSize;
    INT m_iUsed;

public:

    CCache(__in_opt LPCTSTR pName,INT iItems);
    virtual ~CCache();

    /* Add an item to the cache */
    CBaseObject *AddToCache(__in CBaseObject *pObject);

    /* Remove an item from the cache */
    CBaseObject *RemoveFromCache();

    /* Delete all the objects held in the cache */
    void RemoveAll(void);

    /* Return the cache size which is set during construction */
    INT GetCacheSize(void) const {return m_iCacheSize;};
};

#endif /* __CACHE__ */

//------------------------------------------------------------------------------
// File: WXList.h
//
// Desc: DirectShow base classes - defines a non-MFC generic template list
//       class.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


/* A generic list of pointers to objects.
   No storage management or copying is done on the objects pointed to.
   Objectives: avoid using MFC libraries in ndm kernel mode and
   provide a really useful list type.

   The class is thread safe in that separate threads may add and
   delete items in the list concurrently although the application
   must ensure that constructor and destructor access is suitably
   synchronised. An application can cause deadlock with operations
   which use two lists by simultaneously calling
   list1->Operation(list2) and list2->Operation(list1).  So don't!

   The names must not conflict with MFC classes as an application
   may use both.
   */

#ifndef __WXLIST__
#define __WXLIST__

   /* A POSITION represents (in some fashion that's opaque) a cursor
      on the list that can be set to identify any element.  NULL is
      a valid value and several operations regard NULL as the position
      "one step off the end of the list".  (In an n element list there
      are n+1 places to insert and NULL is that "n+1-th" value).
      The POSITION of an element in the list is only invalidated if
      that element is deleted.  Move operations may mean that what
      was a valid POSITION in one list is now a valid POSITION in
      a different list.

      Some operations which at first sight are illegal are allowed as
      harmless no-ops.  For instance RemoveHead is legal on an empty
      list and it returns NULL.  This allows an atomic way to test if
      there is an element there, and if so, get it.  The two operations
      AddTail and RemoveHead thus implement a MONITOR (See Hoare's paper).

      Single element operations return POSITIONs, non-NULL means it worked.
      whole list operations return a BOOL.  TRUE means it all worked.

      This definition is the same as the POSITION type for MFCs, so we must
      avoid defining it twice.
   */
#ifndef __AFX_H__
struct __POSITION { int unused; };
typedef __POSITION* POSITION;
#endif

const int DEFAULTCACHE = 10;    /* Default node object cache size */

/* A class representing one node in a list.
   Each node knows a pointer to it's adjacent nodes and also a pointer
   to the object that it looks after.
   All of these pointers can be retrieved or set through member functions.
*/
class CBaseList 
#ifdef DEBUG
    : public CBaseObject
#endif
{
    /* Making these classes inherit from CBaseObject does nothing
       functionally but it allows us to check there are no memory
       leaks in debug builds. 
    */

public:

#ifdef DEBUG
    class CNode : public CBaseObject {
#else
    class CNode {
#endif

        CNode *m_pPrev;         /* Previous node in the list */
        CNode *m_pNext;         /* Next node in the list */
        void *m_pObject;      /* Pointer to the object */

    public:

        /* Constructor - initialise the object's pointers */
        CNode()
#ifdef DEBUG
            : CBaseObject(NAME("List node"))
#endif
        {
        };


        /* Return the previous node before this one */
        __out CNode *Prev() const { return m_pPrev; };


        /* Return the next node after this one */
        __out CNode *Next() const { return m_pNext; };


        /* Set the previous node before this one */
        void SetPrev(__in_opt CNode *p) { m_pPrev = p; };


        /* Set the next node after this one */
        void SetNext(__in_opt CNode *p) { m_pNext = p; };


        /* Get the pointer to the object for this node */
        __out void *GetData() const { return m_pObject; };


        /* Set the pointer to the object for this node */
        void SetData(__in void *p) { m_pObject = p; };
    };

    class CNodeCache
    {
    public:
        CNodeCache(INT iCacheSize) : m_iCacheSize(iCacheSize),
                                     m_pHead(NULL),
                                     m_iUsed(0)
                                     {};
        ~CNodeCache() {
            CNode *pNode = m_pHead;
            while (pNode) {
                CNode *pCurrent = pNode;
                pNode = pNode->Next();
                delete pCurrent;
            }
        };
        void AddToCache(__inout CNode *pNode)
        {
            if (m_iUsed < m_iCacheSize) {
                pNode->SetNext(m_pHead);
                m_pHead = pNode;
                m_iUsed++;
            } else {
                delete pNode;
            }
        };
        CNode *RemoveFromCache()
        {
            CNode *pNode = m_pHead;
            if (pNode != NULL) {
                m_pHead = pNode->Next();
                m_iUsed--;
                ASSERT(m_iUsed >= 0);
            } else {
                ASSERT(m_iUsed == 0);
            }
            return pNode;
        };
    private:
        INT m_iCacheSize;
        INT m_iUsed;
        CNode *m_pHead;
    };

protected:

    CNode* m_pFirst;    /* Pointer to first node in the list */
    CNode* m_pLast;     /* Pointer to the last node in the list */
    LONG m_Count;       /* Number of nodes currently in the list */

private:

    CNodeCache m_Cache; /* Cache of unused node pointers */

private:

    /* These override the default copy constructor and assignment
       operator for all list classes. They are in the private class
       declaration section so that anybody trying to pass a list
       object by value will generate a compile time error of
       "cannot access the private member function". If these were
       not here then the compiler will create default constructors
       and assignment operators which when executed first take a
       copy of all member variables and then during destruction
       delete them all. This must not be done for any heap
       allocated data.
    */
    CBaseList(const CBaseList &refList);
    CBaseList &operator=(const CBaseList &refList);

public:

    CBaseList(__in_opt LPCTSTR pName,
              INT iItems);

    CBaseList(__in_opt LPCTSTR pName);
#ifdef UNICODE
    CBaseList(__in_opt LPCSTR pName,
              INT iItems);

    CBaseList(__in_opt LPCSTR pName);
#endif
    ~CBaseList();

    /* Remove all the nodes from *this i.e. make the list empty */
    void RemoveAll();


    /* Return a cursor which identifies the first element of *this */
    __out_opt POSITION GetHeadPositionI() const;


    /* Return a cursor which identifies the last element of *this */
    __out_opt POSITION GetTailPositionI() const;


    /* Return the number of objects in *this */
    int GetCountI() const;

protected:
    /* Return the pointer to the object at rp,
       Update rp to the next node in *this
       but make it NULL if it was at the end of *this.
       This is a wart retained for backwards compatibility.
       GetPrev is not implemented.
       Use Next, Prev and Get separately.
    */
    __out void *GetNextI(__inout POSITION& rp) const;


    /* Return a pointer to the object at p
       Asking for the object at NULL will return NULL harmlessly.
    */
    __out_opt void *GetI(__in_opt POSITION p) const;
    __out void *GetValidI(__in POSITION p) const;

public:
    /* return the next / prev position in *this
       return NULL when going past the end/start.
       Next(NULL) is same as GetHeadPosition()
       Prev(NULL) is same as GetTailPosition()
       An n element list therefore behaves like a n+1 element
       cycle with NULL at the start/end.

       !!WARNING!! - This handling of NULL is DIFFERENT from GetNext.

       Some reasons are:
       1. For a list of n items there are n+1 positions to insert
          These are conveniently encoded as the n POSITIONs and NULL.
       2. If you are keeping a list sorted (fairly common) and you
          search forward for an element to insert before and don't
          find it you finish up with NULL as the element before which
          to insert.  You then want that NULL to be a valid POSITION
          so that you can insert before it and you want that insertion
          point to mean the (n+1)-th one that doesn't have a POSITION.
          (symmetrically if you are working backwards through the list).
       3. It simplifies the algebra which the methods generate.
          e.g. AddBefore(p,x) is identical to AddAfter(Prev(p),x)
          in ALL cases.  All the other arguments probably are reflections
          of the algebraic point.
    */
    __out_opt POSITION Next(__in_opt POSITION pos) const
    {
        if (pos == NULL) {
            return (POSITION) m_pFirst;
        }
        CNode *pn = (CNode *) pos;
        return (POSITION) pn->Next();
    } //Next

    // See Next
    __out_opt POSITION Prev(__in_opt POSITION pos) const
    {
        if (pos == NULL) {
            return (POSITION) m_pLast;
        }
        CNode *pn = (CNode *) pos;
        return (POSITION) pn->Prev();
    } //Prev


    /* Return the first position in *this which holds the given
       pointer.  Return NULL if the pointer was not not found.
    */
protected:
    __out_opt POSITION FindI( __in void * pObj) const;

    // ??? Should there be (or even should there be only)
    // ??? POSITION FindNextAfter(void * pObj, POSITION p)
    // ??? And of course FindPrevBefore too.
    // ??? List.Find(&Obj) then becomes List.FindNextAfter(&Obj, NULL)


    /* Remove the first node in *this (deletes the pointer to its
       object from the list, does not free the object itself).
       Return the pointer to its object.
       If *this was already empty it will harmlessly return NULL.
    */
    __out_opt void *RemoveHeadI();


    /* Remove the last node in *this (deletes the pointer to its
       object from the list, does not free the object itself).
       Return the pointer to its object.
       If *this was already empty it will harmlessly return NULL.
    */
    __out_opt void *RemoveTailI();


    /* Remove the node identified by p from the list (deletes the pointer
       to its object from the list, does not free the object itself).
       Asking to Remove the object at NULL will harmlessly return NULL.
       Return the pointer to the object removed.
    */
    __out_opt void *RemoveI(__in_opt POSITION p);

    /* Add single object *pObj to become a new last element of the list.
       Return the new tail position, NULL if it fails.
       If you are adding a COM objects, you might want AddRef it first.
       Other existing POSITIONs in *this are still valid
    */
    __out_opt POSITION AddTailI(__in void * pObj);
public:


    /* Add all the elements in *pList to the tail of *this.
       This duplicates all the nodes in *pList (i.e. duplicates
       all its pointers to objects).  It does not duplicate the objects.
       If you are adding a list of pointers to a COM object into the list
       it's a good idea to AddRef them all  it when you AddTail it.
       Return TRUE if it all worked, FALSE if it didn't.
       If it fails some elements may have been added.
       Existing POSITIONs in *this are still valid

       If you actually want to MOVE the elements, use MoveToTail instead.
    */
    BOOL AddTail(__in CBaseList *pList);


    /* Mirror images of AddHead: */

    /* Add single object to become a new first element of the list.
       Return the new head position, NULL if it fails.
       Existing POSITIONs in *this are still valid
    */
protected:
    __out_opt POSITION AddHeadI(__in void * pObj);
public:

    /* Add all the elements in *pList to the head of *this.
       Same warnings apply as for AddTail.
       Return TRUE if it all worked, FALSE if it didn't.
       If it fails some of the objects may have been added.

       If you actually want to MOVE the elements, use MoveToHead instead.
    */
    BOOL AddHead(__in CBaseList *pList);


    /* Add the object *pObj to *this after position p in *this.
       AddAfter(NULL,x) adds x to the start - equivalent to AddHead
       Return the position of the object added, NULL if it failed.
       Existing POSITIONs in *this are undisturbed, including p.
    */
protected:
    __out_opt POSITION AddAfterI(__in_opt POSITION p, __in void * pObj);
public:

    /* Add the list *pList to *this after position p in *this
       AddAfter(NULL,x) adds x to the start - equivalent to AddHead
       Return TRUE if it all worked, FALSE if it didn't.
       If it fails, some of the objects may be added
       Existing POSITIONs in *this are undisturbed, including p.
    */
    BOOL AddAfter(__in_opt POSITION p, __in CBaseList *pList);


    /* Mirror images:
       Add the object *pObj to this-List after position p in *this.
       AddBefore(NULL,x) adds x to the end - equivalent to AddTail
       Return the position of the new object, NULL if it fails
       Existing POSITIONs in *this are undisturbed, including p.
    */
    protected:
    __out_opt POSITION AddBeforeI(__in_opt POSITION p, __in void * pObj);
    public:

    /* Add the list *pList to *this before position p in *this
       AddAfter(NULL,x) adds x to the start - equivalent to AddHead
       Return TRUE if it all worked, FALSE if it didn't.
       If it fails, some of the objects may be added
       Existing POSITIONs in *this are undisturbed, including p.
    */
    BOOL AddBefore(__in_opt POSITION p, __in CBaseList *pList);


    /* Note that AddAfter(p,x) is equivalent to AddBefore(Next(p),x)
       even in cases where p is NULL or Next(p) is NULL.
       Similarly for mirror images etc.
       This may make it easier to argue about programs.
    */



    /* The following operations do not copy any elements.
       They move existing blocks of elements around by switching pointers.
       They are fairly efficient for long lists as for short lists.
       (Alas, the Count slows things down).

       They split the list into two parts.
       One part remains as the original list, the other part
       is appended to the second list.  There are eight possible
       variations:
       Split the list {after/before} a given element
       keep the {head/tail} portion in the original list
       append the rest to the {head/tail} of the new list.

       Since After is strictly equivalent to Before Next
       we are not in serious need of the Before/After variants.
       That leaves only four.

       If you are processing a list left to right and dumping
       the bits that you have processed into another list as
       you go, the Tail/Tail variant gives the most natural result.
       If you are processing in reverse order, Head/Head is best.

       By using NULL positions and empty lists judiciously either
       of the other two can be built up in two operations.

       The definition of NULL (see Next/Prev etc) means that
       degenerate cases include
          "move all elements to new list"
          "Split a list into two lists"
          "Concatenate two lists"
          (and quite a few no-ops)

       !!WARNING!! The type checking won't buy you much if you get list
       positions muddled up - e.g. use a POSITION that's in a different
       list and see what a mess you get!
    */

    /* Split *this after position p in *this
       Retain as *this the tail portion of the original *this
       Add the head portion to the tail end of *pList
       Return TRUE if it all worked, FALSE if it didn't.

       e.g.
          foo->MoveToTail(foo->GetHeadPosition(), bar);
              moves one element from the head of foo to the tail of bar
          foo->MoveToTail(NULL, bar);
              is a no-op, returns NULL
          foo->MoveToTail(foo->GetTailPosition, bar);
              concatenates foo onto the end of bar and empties foo.

       A better, except excessively long name might be
           MoveElementsFromHeadThroughPositionToOtherTail
    */
    BOOL MoveToTail(__in_opt POSITION pos, __in CBaseList *pList);


    /* Mirror image:
       Split *this before position p in *this.
       Retain in *this the head portion of the original *this
       Add the tail portion to the start (i.e. head) of *pList

       e.g.
          foo->MoveToHead(foo->GetTailPosition(), bar);
              moves one element from the tail of foo to the head of bar
          foo->MoveToHead(NULL, bar);
              is a no-op, returns NULL
          foo->MoveToHead(foo->GetHeadPosition, bar);
              concatenates foo onto the start of bar and empties foo.
    */
    BOOL MoveToHead(__in_opt POSITION pos, __in CBaseList *pList);


    /* Reverse the order of the [pointers to] objects in *this
    */
    void Reverse();


    /* set cursor to the position of each element of list in turn  */
    #define TRAVERSELIST(list, cursor)               \
    for ( cursor = (list).GetHeadPosition()           \
        ; cursor!=NULL                               \
        ; cursor = (list).Next(cursor)                \
        )


    /* set cursor to the position of each element of list in turn
       in reverse order
    */
    #define REVERSETRAVERSELIST(list, cursor)        \
    for ( cursor = (list).GetTailPosition()           \
        ; cursor!=NULL                               \
        ; cursor = (list).Prev(cursor)                \
        )

}; // end of class declaration

template<class OBJECT> class CGenericList : public CBaseList
{
public:
    CGenericList(__in_opt LPCTSTR pName,
                 INT iItems,
                 BOOL bLock = TRUE,
                 BOOL bAlert = FALSE) :
                     CBaseList(pName, iItems) {
        UNREFERENCED_PARAMETER(bAlert);
        UNREFERENCED_PARAMETER(bLock);
    };
    CGenericList(__in_opt LPCTSTR pName) :
                     CBaseList(pName) {
    };

    __out_opt POSITION GetHeadPosition() const { return (POSITION)m_pFirst; }
    __out_opt POSITION GetTailPosition() const { return (POSITION)m_pLast; }
    int GetCount() const { return m_Count; }

    __out OBJECT *GetNext(__inout POSITION& rp) const { return (OBJECT *) GetNextI(rp); }

    __out_opt OBJECT *Get(__in_opt POSITION p) const { return (OBJECT *) GetI(p); }
    __out OBJECT *GetValid(__in POSITION p) const { return (OBJECT *) GetValidI(p); }
    __out_opt OBJECT *GetHead() const  { return Get(GetHeadPosition()); }

    __out_opt OBJECT *RemoveHead() { return (OBJECT *) RemoveHeadI(); }

    __out_opt OBJECT *RemoveTail() { return (OBJECT *) RemoveTailI(); }

    __out_opt OBJECT *Remove(__in_opt POSITION p) { return (OBJECT *) RemoveI(p); }
    __out_opt POSITION AddBefore(__in_opt POSITION p, __in OBJECT * pObj) { return AddBeforeI(p, pObj); }
    __out_opt POSITION AddAfter(__in_opt POSITION p, __in OBJECT * pObj)  { return AddAfterI(p, pObj); }
    __out_opt POSITION AddHead(__in OBJECT * pObj) { return AddHeadI(pObj); }
    __out_opt POSITION AddTail(__in OBJECT * pObj)  { return AddTailI(pObj); }
    BOOL AddTail(__in CGenericList<OBJECT> *pList)
            { return CBaseList::AddTail((CBaseList *) pList); }
    BOOL AddHead(__in CGenericList<OBJECT> *pList)
            { return CBaseList::AddHead((CBaseList *) pList); }
    BOOL AddAfter(__in_opt POSITION p, __in CGenericList<OBJECT> *pList)
            { return CBaseList::AddAfter(p, (CBaseList *) pList); };
    BOOL AddBefore(__in_opt POSITION p, __in CGenericList<OBJECT> *pList)
            { return CBaseList::AddBefore(p, (CBaseList *) pList); };
    __out_opt POSITION Find( __in OBJECT * pObj) const { return FindI(pObj); }
}; // end of class declaration



/* These define the standard list types */

typedef CGenericList<CBaseObject> CBaseObjectList;
typedef CGenericList<IUnknown> CBaseInterfaceList;

#endif /* __WXLIST__ */

//------------------------------------------------------------------------------
// File: MtType.h
//
// Desc: DirectShow base classes - defines a class that holds and manages
//       media type information.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __MTYPE__
#define __MTYPE__

/* Helper class that derived pin objects can use to compare media
   types etc. Has same data members as the struct AM_MEDIA_TYPE defined
   in the streams IDL file, but also has (non-virtual) functions */

class CMediaType : public _AMMediaType {

public:

    ~CMediaType();
    CMediaType();
    CMediaType(const GUID * majortype);
    CMediaType(const AM_MEDIA_TYPE&, __out_opt HRESULT* phr = NULL);
    CMediaType(const CMediaType&, __out_opt HRESULT* phr = NULL);

    CMediaType& operator=(const CMediaType&);
    CMediaType& operator=(const AM_MEDIA_TYPE&);

    BOOL operator == (const CMediaType&) const;
    BOOL operator != (const CMediaType&) const;

    HRESULT Set(const CMediaType& rt);
    HRESULT Set(const AM_MEDIA_TYPE& rt);

    BOOL IsValid() const;

    const GUID *Type() const { return &majortype;} ;
    void SetType(const GUID *);
    const GUID *Subtype() const { return &subtype;} ;
    void SetSubtype(const GUID *);

    BOOL IsFixedSize() const {return bFixedSizeSamples; };
    BOOL IsTemporalCompressed() const {return bTemporalCompression; };
    ULONG GetSampleSize() const;

    void SetSampleSize(ULONG sz);
    void SetVariableSize();
    void SetTemporalCompression(BOOL bCompressed);

    // read/write pointer to format - can't change length without
    // calling SetFormat, AllocFormatBuffer or ReallocFormatBuffer

    BYTE*   Format() const {return pbFormat; };
    ULONG   FormatLength() const { return cbFormat; };

    void SetFormatType(const GUID *);
    const GUID *FormatType() const {return &formattype; };
    BOOL SetFormat(__in_bcount(length) BYTE *pFormat, ULONG length);
    void ResetFormatBuffer();
    BYTE* AllocFormatBuffer(ULONG length);
    BYTE* ReallocFormatBuffer(ULONG length);

    void InitMediaType();

    BOOL MatchesPartial(const CMediaType* ppartial) const;
    BOOL IsPartiallySpecified(void) const;
};


/* General purpose functions to copy and delete a task allocated AM_MEDIA_TYPE
   structure which is useful when using the IEnumMediaFormats interface as
   the implementation allocates the structures which you must later delete */

void WINAPI DeleteMediaType(__inout_opt AM_MEDIA_TYPE *pmt);
AM_MEDIA_TYPE * WINAPI CreateMediaType(AM_MEDIA_TYPE const *pSrc);
HRESULT WINAPI CopyMediaType(__out AM_MEDIA_TYPE *pmtTarget, const AM_MEDIA_TYPE *pmtSource);
void WINAPI FreeMediaType(__inout AM_MEDIA_TYPE& mt);

//  Initialize a media type from a WAVEFORMATEX

STDAPI CreateAudioMediaType(
    const WAVEFORMATEX *pwfx,
    __out AM_MEDIA_TYPE *pmt,
    BOOL bSetFormat);

#endif /* __MTYPE__ */

//------------------------------------------------------------------------------
// File: FourCC.h
//
// Desc: DirectShow base classes.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


// FOURCCMap
//
// provides a mapping between old-style multimedia format DWORDs
// and new-style GUIDs.
//
// A range of 4 billion GUIDs has been allocated to ensure that this
// mapping can be done straightforwardly one-to-one in both directions.
//
// January 95


#ifndef __FOURCC__
#define __FOURCC__


// Multimedia format types are marked with DWORDs built from four 8-bit
// chars and known as FOURCCs. New multimedia AM_MEDIA_TYPE definitions include
// a subtype GUID. In order to simplify the mapping, GUIDs in the range:
//    XXXXXXXX-0000-0010-8000-00AA00389B71
// are reserved for FOURCCs.

class FOURCCMap : public GUID
{

public:
    FOURCCMap();
    FOURCCMap(DWORD Fourcc);
    FOURCCMap(const GUID *);


    DWORD GetFOURCC(void);
    void SetFOURCC(DWORD fourcc);
    void SetFOURCC(const GUID *);

private:
    void InitGUID();
};

#define GUID_Data2      0
#define GUID_Data3     0x10
#define GUID_Data4_1   0xaa000080
#define GUID_Data4_2   0x719b3800

inline void
FOURCCMap::InitGUID() {
    Data2 = GUID_Data2;
    Data3 = GUID_Data3;
    ((DWORD *)Data4)[0] = GUID_Data4_1;
    ((DWORD *)Data4)[1] = GUID_Data4_2;
}

inline
FOURCCMap::FOURCCMap() {
    InitGUID();
    SetFOURCC( DWORD(0));
}

inline
FOURCCMap::FOURCCMap(DWORD fourcc)
{
    InitGUID();
    SetFOURCC(fourcc);
}

inline
FOURCCMap::FOURCCMap(const GUID * pGuid)
{
    InitGUID();
    SetFOURCC(pGuid);
}

inline void
FOURCCMap::SetFOURCC(const GUID * pGuid)
{
    FOURCCMap * p = (FOURCCMap*) pGuid;
    SetFOURCC(p->GetFOURCC());
}

inline void
FOURCCMap::SetFOURCC(DWORD fourcc)
{
    Data1 = fourcc;
}

inline DWORD
FOURCCMap::GetFOURCC(void)
{
    return Data1;
}

#endif /* __FOURCC__ */

//------------------------------------------------------------------------------
// File: CtlUtil.h
//
// Desc: DirectShow base classes.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


// Base classes implementing IDispatch parsing for the basic control dual
// interfaces. Derive from these and implement just the custom method and
// property methods. We also implement CPosPassThru that can be used by
// renderers and transforms to pass by IMediaPosition and IMediaSeeking

#ifndef __CTLUTIL__
#define __CTLUTIL__

// OLE Automation has different ideas of TRUE and FALSE

#define OATRUE (-1)
#define OAFALSE (0)


// It's possible that we could replace this class with CreateStdDispatch

class CBaseDispatch
{
    ITypeInfo * m_pti;

public:

    CBaseDispatch() : m_pti(NULL) {}
    ~CBaseDispatch();

    /* IDispatch methods */
    STDMETHODIMP GetTypeInfoCount(__out UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      REFIID riid,
      UINT itinfo,
      LCID lcid,
      __deref_out ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      __in_ecount(cNames) LPOLESTR * rgszNames,
      UINT cNames,
      LCID lcid,
      __out_ecount(cNames) DISPID * rgdispid);
};


class AM_NOVTABLE CMediaControl :
    public IMediaControl,
    public CUnknown
{
    CBaseDispatch m_basedisp;

public:

    CMediaControl(const TCHAR *, LPUNKNOWN);

    DECLARE_IUNKNOWN

    // override this to publicise our interfaces
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);

    /* IDispatch methods */
    STDMETHODIMP GetTypeInfoCount(__out UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      UINT itinfo,
      LCID lcid,
      __deref_out ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      __in_ecount(cNames) LPOLESTR * rgszNames,
      UINT cNames,
      LCID lcid,
      __out_ecount(cNames) DISPID * rgdispid);

    STDMETHODIMP Invoke(
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      __in DISPPARAMS * pdispparams,
      __out_opt VARIANT * pvarResult,
      __out_opt EXCEPINFO * pexcepinfo,
      __out_opt UINT * puArgErr);
};


class AM_NOVTABLE CMediaEvent :
    public IMediaEventEx,
    public CUnknown
{
    CBaseDispatch m_basedisp;

public:

    CMediaEvent(__in_opt LPCTSTR, __in_opt LPUNKNOWN);

    DECLARE_IUNKNOWN

    // override this to publicise our interfaces
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);

    /* IDispatch methods */
    STDMETHODIMP GetTypeInfoCount(__out UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      UINT itinfo,
      LCID lcid,
      __deref_out ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      __in_ecount(cNames) LPOLESTR * rgszNames,
      UINT cNames,
      LCID lcid,
      __out_ecount(cNames) DISPID * rgdispid);

    STDMETHODIMP Invoke(
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      __in DISPPARAMS * pdispparams,
      __out_opt VARIANT * pvarResult,
      __out_opt EXCEPINFO * pexcepinfo,
      __out_opt UINT * puArgErr);
};


class AM_NOVTABLE CMediaPosition :
    public IMediaPosition,
    public CUnknown
{
    CBaseDispatch m_basedisp;


public:

    CMediaPosition(__in_opt LPCTSTR, __in_opt LPUNKNOWN);
    CMediaPosition(__in_opt LPCTSTR, __in_opt LPUNKNOWN, __inout HRESULT *phr);

    DECLARE_IUNKNOWN

    // override this to publicise our interfaces
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);

    /* IDispatch methods */
    STDMETHODIMP GetTypeInfoCount(__out UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      UINT itinfo,
      LCID lcid,
      __deref_out ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      __in_ecount(cNames) LPOLESTR * rgszNames,
      UINT cNames,
      LCID lcid,
      __out_ecount(cNames) DISPID * rgdispid);

    STDMETHODIMP Invoke(
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      __in DISPPARAMS * pdispparams,
      __out_opt VARIANT * pvarResult,
      __out_opt EXCEPINFO * pexcepinfo,
      __out_opt UINT * puArgErr);

};


// OA-compatibility means that we must use double as the RefTime value,
// and REFERENCE_TIME (essentially a LONGLONG) within filters.
// this class converts between the two

class COARefTime : public CRefTime {
public:

    COARefTime() {
    };

    COARefTime(CRefTime t)
        : CRefTime(t)
    {
    };

    COARefTime(REFERENCE_TIME t)
        : CRefTime(t)
    {
    };

    COARefTime(double d) {
        m_time = (LONGLONG) (d * 10000000);
    };

    operator double() {
        return double(m_time) / 10000000;
    };

    operator REFERENCE_TIME() {
        return m_time;
    };

    COARefTime& operator=(const double& rd)  {
        m_time = (LONGLONG) (rd * 10000000);
        return *this;
    }

    COARefTime& operator=(const REFERENCE_TIME& rt)  {
        m_time = rt;
        return *this;
    }

    inline BOOL operator==(const COARefTime& rt)
    {
        return m_time == rt.m_time;
    };

    inline BOOL operator!=(const COARefTime& rt)
    {
        return m_time != rt.m_time;
    };

    inline BOOL operator < (const COARefTime& rt)
    {
        return m_time < rt.m_time;
    };

    inline BOOL operator > (const COARefTime& rt)
    {
        return m_time > rt.m_time;
    };

    inline BOOL operator >= (const COARefTime& rt)
    {
        return m_time >= rt.m_time;
    };

    inline BOOL operator <= (const COARefTime& rt)
    {
        return m_time <= rt.m_time;
    };

    inline COARefTime operator+(const COARefTime& rt)
    {
        return COARefTime(m_time + rt.m_time);
    };

    inline COARefTime operator-(const COARefTime& rt)
    {
        return COARefTime(m_time - rt.m_time);
    };

    inline COARefTime operator*(LONG l)
    {
        return COARefTime(m_time * l);
    };

    inline COARefTime operator/(LONG l)
    {
        return COARefTime(m_time / l);
    };

private:
    //  Prevent bugs from constructing from LONG (which gets
    //  converted to double and then multiplied by 10000000
    COARefTime(LONG);
    LONG operator=(LONG);
};


// A utility class that handles IMediaPosition and IMediaSeeking on behalf
// of single-input pin renderers, or transform filters.
//
// Renderers will expose this from the filter; transform filters will
// expose it from the output pin and not the renderer.
//
// Create one of these, giving it your IPin* for your input pin, and delegate
// all IMediaPosition methods to it. It will query the input pin for
// IMediaPosition and respond appropriately.
//
// Call ForceRefresh if the pin connection changes.
//
// This class no longer caches the upstream IMediaPosition or IMediaSeeking
// it acquires it on each method call. This means ForceRefresh is not needed.
// The method is kept for source compatibility and to minimise the changes
// if we need to put it back later for performance reasons.

class CPosPassThru : public IMediaSeeking, public CMediaPosition
{
    IPin *m_pPin;

    HRESULT GetPeer(__deref_out IMediaPosition **ppMP);
    HRESULT GetPeerSeeking(__deref_out IMediaSeeking **ppMS);

public:

    CPosPassThru(__in_opt LPCTSTR, __in_opt LPUNKNOWN, __inout HRESULT*, IPin *);
    DECLARE_IUNKNOWN

    HRESULT ForceRefresh() {
        return S_OK;
    };

    // override to return an accurate current position
    virtual HRESULT GetMediaTime(__out LONGLONG *pStartTime, __out_opt LONGLONG *pEndTime) {
        return E_FAIL;
    }

    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid,__deref_out void **ppv);

    // IMediaSeeking methods
    STDMETHODIMP GetCapabilities( __out DWORD * pCapabilities );
    STDMETHODIMP CheckCapabilities( __inout DWORD * pCapabilities );
    STDMETHODIMP SetTimeFormat(const GUID * pFormat);
    STDMETHODIMP GetTimeFormat(__out GUID *pFormat);
    STDMETHODIMP IsUsingTimeFormat(const GUID * pFormat);
    STDMETHODIMP IsFormatSupported( const GUID * pFormat);
    STDMETHODIMP QueryPreferredFormat( __out GUID *pFormat);
    STDMETHODIMP ConvertTimeFormat(__out LONGLONG * pTarget, 
                                   __in_opt const GUID * pTargetFormat,
                                   LONGLONG Source, 
                                   __in_opt const GUID * pSourceFormat );
    STDMETHODIMP SetPositions( __inout_opt LONGLONG * pCurrent, DWORD CurrentFlags
                             , __inout_opt LONGLONG * pStop, DWORD StopFlags );

    STDMETHODIMP GetPositions( __out_opt LONGLONG * pCurrent, __out_opt LONGLONG * pStop );
    STDMETHODIMP GetCurrentPosition( __out LONGLONG * pCurrent );
    STDMETHODIMP GetStopPosition( __out LONGLONG * pStop );
    STDMETHODIMP SetRate( double dRate);
    STDMETHODIMP GetRate( __out double * pdRate);
    STDMETHODIMP GetDuration( __out LONGLONG *pDuration);
    STDMETHODIMP GetAvailable( __out_opt LONGLONG *pEarliest, __out_opt LONGLONG *pLatest );
    STDMETHODIMP GetPreroll( __out LONGLONG *pllPreroll );

    // IMediaPosition properties
    STDMETHODIMP get_Duration(__out REFTIME * plength);
    STDMETHODIMP put_CurrentPosition(REFTIME llTime);
    STDMETHODIMP get_StopTime(__out REFTIME * pllTime);
    STDMETHODIMP put_StopTime(REFTIME llTime);
    STDMETHODIMP get_PrerollTime(__out REFTIME * pllTime);
    STDMETHODIMP put_PrerollTime(REFTIME llTime);
    STDMETHODIMP get_Rate(__out double * pdRate);
    STDMETHODIMP put_Rate(double dRate);
    STDMETHODIMP get_CurrentPosition(__out REFTIME * pllTime);
    STDMETHODIMP CanSeekForward(__out LONG *pCanSeekForward);
    STDMETHODIMP CanSeekBackward(__out LONG *pCanSeekBackward);

private:
    HRESULT GetSeekingLongLong( HRESULT (__stdcall IMediaSeeking::*pMethod)( LONGLONG * ),
                                __out LONGLONG * pll );
};


// Adds the ability to return a current position

class CRendererPosPassThru : public CPosPassThru
{
    CCritSec m_PositionLock;    // Locks access to our position
    LONGLONG m_StartMedia;      // Start media time last seen
    LONGLONG m_EndMedia;        // And likewise the end media
    BOOL m_bReset;              // Have media times been set

public:

    // Used to help with passing media times through graph

    CRendererPosPassThru(__in_opt LPCTSTR, __in_opt LPUNKNOWN, __inout HRESULT*, IPin *);
    HRESULT RegisterMediaTime(IMediaSample *pMediaSample);
    HRESULT RegisterMediaTime(LONGLONG StartTime,LONGLONG EndTime);
    HRESULT GetMediaTime(__out LONGLONG *pStartTime,__out_opt LONGLONG *pEndTime);
    HRESULT ResetMediaTime();
    HRESULT EOS();
};

STDAPI CreatePosPassThru(
    __in_opt LPUNKNOWN pAgg,
    BOOL bRenderer,
    IPin *pPin,
    __deref_out IUnknown **ppPassThru
);

// A class that handles the IDispatch part of IBasicAudio and leaves the
// properties and methods themselves pure virtual.

class AM_NOVTABLE CBasicAudio : public IBasicAudio, public CUnknown
{
    CBaseDispatch m_basedisp;

public:

    CBasicAudio(__in_opt LPCTSTR, __in_opt LPUNKNOWN);

    DECLARE_IUNKNOWN

    // override this to publicise our interfaces
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);

    /* IDispatch methods */
    STDMETHODIMP GetTypeInfoCount(__out UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      UINT itinfo,
      LCID lcid,
      __deref_out ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      __in_ecount(cNames) LPOLESTR * rgszNames,
      UINT cNames,
      LCID lcid,
      __out_ecount(cNames) DISPID * rgdispid);

    STDMETHODIMP Invoke(
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      __in DISPPARAMS * pdispparams,
      __out_opt VARIANT * pvarResult,
      __out_opt EXCEPINFO * pexcepinfo,
      __out_opt UINT * puArgErr);
};


// A class that handles the IDispatch part of IBasicVideo and leaves the
// properties and methods themselves pure virtual.

class AM_NOVTABLE CBaseBasicVideo : public IBasicVideo2, public CUnknown
{
    CBaseDispatch m_basedisp;

public:

    CBaseBasicVideo(__in_opt LPCTSTR, __in_opt LPUNKNOWN);

    DECLARE_IUNKNOWN

    // override this to publicise our interfaces
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);

    /* IDispatch methods */
    STDMETHODIMP GetTypeInfoCount(__out UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      UINT itinfo,
      LCID lcid,
      __deref_out ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      __in_ecount(cNames) LPOLESTR * rgszNames,
      UINT cNames,
      LCID lcid,
      __out_ecount(cNames) DISPID * rgdispid);

    STDMETHODIMP Invoke(
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      __in DISPPARAMS * pdispparams,
      __out_opt VARIANT * pvarResult,
      __out_opt EXCEPINFO * pexcepinfo,
      __out_opt UINT * puArgErr);

    STDMETHODIMP GetPreferredAspectRatio(
      __out long *plAspectX,
      __out long *plAspectY)
    {
        return E_NOTIMPL;
    }
};


// A class that handles the IDispatch part of IVideoWindow and leaves the
// properties and methods themselves pure virtual.

class AM_NOVTABLE CBaseVideoWindow : public IVideoWindow, public CUnknown
{
    CBaseDispatch m_basedisp;

public:

    CBaseVideoWindow(__in_opt LPCTSTR, __in_opt LPUNKNOWN);

    DECLARE_IUNKNOWN

    // override this to publicise our interfaces
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);

    /* IDispatch methods */
    STDMETHODIMP GetTypeInfoCount(__out UINT * pctinfo);

    STDMETHODIMP GetTypeInfo(
      UINT itinfo,
      LCID lcid,
      __deref_out ITypeInfo ** pptinfo);

    STDMETHODIMP GetIDsOfNames(
      REFIID riid,
      __in_ecount(cNames) LPOLESTR * rgszNames,
      UINT cNames,
      LCID lcid,
      __out_ecount(cNames) DISPID * rgdispid);

    STDMETHODIMP Invoke(
      DISPID dispidMember,
      REFIID riid,
      LCID lcid,
      WORD wFlags,
      __in DISPPARAMS * pdispparams,
      __out_opt VARIANT * pvarResult,
      __out_opt EXCEPINFO * pexcepinfo,
      __out_opt UINT * puArgErr);
};


// abstract class to help source filters with their implementation
// of IMediaPosition. Derive from this and set the duration (and stop
// position). Also override NotifyChange to do something when the properties
// change.

class AM_NOVTABLE CSourcePosition : public CMediaPosition
{

public:
    CSourcePosition(__in_opt LPCTSTR, __in_opt LPUNKNOWN, __inout HRESULT*, __in CCritSec *);

    // IMediaPosition methods
    STDMETHODIMP get_Duration(__out REFTIME * plength);
    STDMETHODIMP put_CurrentPosition(REFTIME llTime);
    STDMETHODIMP get_StopTime(__out REFTIME * pllTime);
    STDMETHODIMP put_StopTime(REFTIME llTime);
    STDMETHODIMP get_PrerollTime(__out REFTIME * pllTime);
    STDMETHODIMP put_PrerollTime(REFTIME llTime);
    STDMETHODIMP get_Rate(__out double * pdRate);
    STDMETHODIMP put_Rate(double dRate);
    STDMETHODIMP CanSeekForward(__out LONG *pCanSeekForward);
    STDMETHODIMP CanSeekBackward(__out LONG *pCanSeekBackward);

    // override if you can return the data you are actually working on
    STDMETHODIMP get_CurrentPosition(__out REFTIME * pllTime) {
        return E_NOTIMPL;
    };

protected:

    // we call this to notify changes. Override to handle them
    virtual HRESULT ChangeStart() PURE;
    virtual HRESULT ChangeStop() PURE;
    virtual HRESULT ChangeRate() PURE;

    COARefTime m_Duration;
    COARefTime m_Start;
    COARefTime m_Stop;
    double m_Rate;

    CCritSec * m_pLock;
};

class AM_NOVTABLE CSourceSeeking :
    public IMediaSeeking,
    public CUnknown
{

public:

    DECLARE_IUNKNOWN;
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);

    // IMediaSeeking methods

    STDMETHODIMP IsFormatSupported(const GUID * pFormat);
    STDMETHODIMP QueryPreferredFormat(__out GUID *pFormat);
    STDMETHODIMP SetTimeFormat(const GUID * pFormat);
    STDMETHODIMP IsUsingTimeFormat(const GUID * pFormat);
    STDMETHODIMP GetTimeFormat(__out GUID *pFormat);
    STDMETHODIMP GetDuration(__out LONGLONG *pDuration);
    STDMETHODIMP GetStopPosition(__out LONGLONG *pStop);
    STDMETHODIMP GetCurrentPosition(__out LONGLONG *pCurrent);
    STDMETHODIMP GetCapabilities( __out DWORD * pCapabilities );
    STDMETHODIMP CheckCapabilities( __inout DWORD * pCapabilities );
    STDMETHODIMP ConvertTimeFormat( __out LONGLONG * pTarget, 
                                    __in_opt const GUID * pTargetFormat,
                                    LONGLONG Source, 
                                    __in_opt const GUID * pSourceFormat );

    STDMETHODIMP SetPositions( __inout_opt LONGLONG * pCurrent,  DWORD CurrentFlags
			     , __inout_opt LONGLONG * pStop,  DWORD StopFlags );

    STDMETHODIMP GetPositions( __out_opt LONGLONG * pCurrent, __out_opt LONGLONG * pStop );

    STDMETHODIMP GetAvailable( __out_opt LONGLONG * pEarliest, __out_opt LONGLONG * pLatest );
    STDMETHODIMP SetRate( double dRate);
    STDMETHODIMP GetRate( __out double * pdRate);
    STDMETHODIMP GetPreroll(__out LONGLONG *pPreroll);


protected:

    // ctor
    CSourceSeeking(__in_opt LPCTSTR, __in_opt LPUNKNOWN, __inout HRESULT*, __in CCritSec *);

    // we call this to notify changes. Override to handle them
    virtual HRESULT ChangeStart() PURE;
    virtual HRESULT ChangeStop() PURE;
    virtual HRESULT ChangeRate() PURE;

    CRefTime m_rtDuration;      // length of stream
    CRefTime m_rtStart;         // source will start here
    CRefTime m_rtStop;          // source will stop here
    double m_dRateSeeking;

    // seeking capabilities
    DWORD m_dwSeekingCaps;

    CCritSec * m_pLock;
};


// Base classes supporting Deferred commands.

// Deferred commands are queued by calls to methods on the IQueueCommand
// interface, exposed by the filtergraph and by some filters. A successful
// call to one of these methods will return an IDeferredCommand interface
// representing the queued command.
//
// A CDeferredCommand object represents a single deferred command, and exposes
// the IDeferredCommand interface as well as other methods permitting time
// checks and actual execution. It contains a reference to the CCommandQueue
// object on which it is queued.
//
// CCommandQueue is a base class providing a queue of CDeferredCommand
// objects, and methods to add, remove, check status and invoke the queued
// commands. A CCommandQueue object would be part of an object that
// implemented IQueueCommand.

class CCmdQueue;

// take a copy of the params and store them. Release any allocated
// memory in destructor

class CDispParams : public DISPPARAMS
{
public:
    CDispParams(UINT nArgs, __in_ecount(nArgs) VARIANT* pArgs, __inout_opt HRESULT *phr = NULL);
    ~CDispParams();
};


// CDeferredCommand lifetime is controlled by refcounts. Caller of
// InvokeAt.. gets a refcounted interface pointer, and the CCmdQueue
// object also holds a refcount on us. Calling Cancel or Invoke takes
// us off the CCmdQueue and thus reduces the refcount by 1. Once taken
// off the queue we cannot be put back on the queue.

class CDeferredCommand
    : public CUnknown,
      public IDeferredCommand
{
public:

    CDeferredCommand(
        __inout CCmdQueue * pQ,
        __in_opt LPUNKNOWN   pUnk,               // aggregation outer unk
        __inout HRESULT *   phr,
        __in LPUNKNOWN   pUnkExecutor,       // object that will execute this cmd
        REFTIME     time,
        __in GUID*       iid,
        long        dispidMethod,
        short       wFlags,
        long        cArgs,
        __in_ecount(cArgs) VARIANT*    pDispParams,
        __out VARIANT*    pvarResult,
        __out short*      puArgErr,
        BOOL        bStream
        );

    DECLARE_IUNKNOWN

    // override this to publicise our interfaces
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __out void **ppv);

    // IDeferredCommand methods
    STDMETHODIMP Cancel();
    STDMETHODIMP Confidence(
                    __out LONG* pConfidence);
    STDMETHODIMP Postpone(
                    REFTIME newtime);
    STDMETHODIMP GetHResult(
                    __out HRESULT* phrResult);

    // other public methods

    HRESULT Invoke();

    // access methods

    // returns TRUE if streamtime, FALSE if presentation time
    BOOL IsStreamTime() {
       return m_bStream;
    };

    CRefTime GetTime() {
        return m_time;
    };

    REFIID GetIID() {
        return *m_iid;
    };

    long GetMethod() {
        return m_dispidMethod;
    };

    short GetFlags() {
        return m_wFlags;
    };

    DISPPARAMS* GetParams() {
        return &m_DispParams;
    };

    VARIANT* GetResult() {
        return m_pvarResult;
    };

protected:

    CCmdQueue* m_pQueue;

    // pUnk for the interface that we will execute the command on
    LPUNKNOWN   m_pUnk;

    // stored command data
    REFERENCE_TIME     m_time;
    GUID*       m_iid;
    long        m_dispidMethod;
    short       m_wFlags;
    VARIANT*    m_pvarResult;
    BOOL        m_bStream;
    CDispParams m_DispParams;
    DISPID      m_DispId;         //  For get and put

    // we use this for ITypeInfo access
    CBaseDispatch   m_Dispatch;

    // save retval here
    HRESULT     m_hrResult;
};


// a list of CDeferredCommand objects. this is a base class providing
// the basics of access to the list. If you want to use CDeferredCommand
// objects then your queue needs to be derived from this class.

class AM_NOVTABLE CCmdQueue
{
public:
    CCmdQueue(__inout_opt HRESULT *phr = NULL);
    virtual ~CCmdQueue();

    // returns a new CDeferredCommand object that will be initialised with
    // the parameters and will be added to the queue during construction.
    // returns S_OK if successfully created otherwise an error and
    // no object has been queued.
    virtual HRESULT  New(
        __out CDeferredCommand **ppCmd,
        __in LPUNKNOWN   pUnk,
        REFTIME     time,
        __in GUID*       iid,
        long        dispidMethod,
        short       wFlags,
        long        cArgs,
        __in_ecount(cArgs) VARIANT*    pDispParams,
        __out VARIANT*    pvarResult,
        __out short*      puArgErr,
        BOOL        bStream
    );

    // called by the CDeferredCommand object to add and remove itself
    // from the queue
    virtual HRESULT Insert(__in CDeferredCommand* pCmd);
    virtual HRESULT Remove(__in CDeferredCommand* pCmd);

    // Command-Due Checking
    //
    // There are two schemes of synchronisation: coarse and accurate. In
    // coarse mode, you wait till the time arrives and then execute the cmd.
    // In accurate mode, you wait until you are processing the sample that
    // will appear at the time, and then execute the command. It's up to the
    // filter which one it will implement. The filtergraph will always
    // implement coarse mode for commands queued at the filtergraph.
    //
    // If you want coarse sync, you probably want to wait until there is a
    // command due, and then execute it. You can do this by calling
    // GetDueCommand. If you have several things to wait for, get the
    // event handle from GetDueHandle() and when this is signalled then call
    // GetDueCommand. Stream time will only advance between calls to Run and
    // EndRun. Note that to avoid an extra thread there is no guarantee that
    // if the handle is set there will be a command ready. Each time the
    // event is signalled, call GetDueCommand (probably with a 0 timeout);
    // This may return E_ABORT.
    //
    // If you want accurate sync, you must call GetCommandDueFor, passing
    // as a parameter the stream time of the samples you are about to process.
    // This will return:
    //   -- a stream-time command due at or before that stream time
    //   -- a presentation-time command due at or before the
    //      time that stream time will be presented (only between Run
    //      and EndRun calls, since outside of this, the mapping from
    //      stream time to presentation time is not known.
    //   -- any presentation-time command due now.
    // This means that if you want accurate synchronisation on samples that
    // might be processed during Paused mode, you need to use
    // stream-time commands.
    //
    // In all cases, commands remain queued until Invoked or Cancelled. The
    // setting and resetting of the event handle is managed entirely by this
    // queue object.

    // set the clock used for timing
    virtual HRESULT SetSyncSource(__in_opt IReferenceClock*);

    // switch to run mode. Streamtime to Presentation time mapping known.
    virtual HRESULT Run(REFERENCE_TIME tStreamTimeOffset);

    // switch to Stopped or Paused mode. Time mapping not known.
    virtual HRESULT EndRun();

    // return a pointer to the next due command. Blocks for msTimeout
    // milliseconds until there is a due command.
    // Stream-time commands will only become due between Run and Endrun calls.
    // The command remains queued until invoked or cancelled.
    // Returns E_ABORT if timeout occurs, otherwise S_OK (or other error).
    // Returns an AddRef-ed object
    virtual HRESULT GetDueCommand(__out CDeferredCommand ** ppCmd, long msTimeout);

    // return the event handle that will be signalled whenever
    // there are deferred commands due for execution (when GetDueCommand
    // will not block).
    HANDLE GetDueHandle() {
        return HANDLE(m_evDue);
    };

    // return a pointer to a command that will be due for a given time.
    // Pass in a stream time here. The stream time offset will be passed
    // in via the Run method.
    // Commands remain queued until invoked or cancelled.
    // This method will not block. It will report VFW_E_NOT_FOUND if there
    // are no commands due yet.
    // Returns an AddRef-ed object
    virtual HRESULT GetCommandDueFor(REFERENCE_TIME tStream, __out CDeferredCommand**ppCmd);

    // check if a given time is due (TRUE if it is due yet)
    BOOL CheckTime(CRefTime time, BOOL bStream) {

        // if no clock, nothing is due!
        if (!m_pClock) {
            return FALSE;
        }

        // stream time
        if (bStream) {

            // not valid if not running
            if (!m_bRunning) {
                return FALSE;
            }
            // add on known stream time offset to get presentation time
            time += m_StreamTimeOffset;
        }

        CRefTime Now;
        m_pClock->GetTime((REFERENCE_TIME*)&Now);
        return (time <= Now);
    };

protected:

    // protect access to lists etc
    CCritSec m_Lock;

    // commands queued in presentation time are stored here
    CGenericList<CDeferredCommand> m_listPresentation;

    // commands queued in stream time are stored here
    CGenericList<CDeferredCommand> m_listStream;

    // set when any commands are due
    CAMEvent m_evDue;

    // creates an advise for the earliest time required, if any
    void SetTimeAdvise(void);

    // advise id from reference clock (0 if no outstanding advise)
    DWORD_PTR m_dwAdvise;

    // advise time is for this presentation time
    CRefTime m_tCurrentAdvise;

    // the reference clock we are using (addrefed)
    IReferenceClock* m_pClock;

    // true when running
    BOOL m_bRunning;

    // contains stream time offset when m_bRunning is true
    CRefTime m_StreamTimeOffset;
};

#endif // __CTLUTIL__
//------------------------------------------------------------------------------
// File: AMFilter.h
//
// Desc: DirectShow base classes - efines class hierarchy for streams
//       architecture.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __FILTER__
#define __FILTER__

/* The following classes are declared in this header: */

class CBaseMediaFilter;     // IMediaFilter support
class CBaseFilter;          // IBaseFilter,IMediaFilter support
class CBasePin;             // Abstract base class for IPin interface
class CEnumPins;            // Enumerate input and output pins
class CEnumMediaTypes;      // Enumerate the pin's preferred formats
class CBaseOutputPin;       // Adds data provider member functions
class CBaseInputPin;        // Implements IMemInputPin interface
class CMediaSample;         // Basic transport unit for IMemInputPin
class CBaseAllocator;       // General list guff for most allocators
class CMemAllocator;        // Implements memory buffer allocation


//=====================================================================
//=====================================================================
//
// QueryFilterInfo and QueryPinInfo AddRef the interface pointers
// they return.  You can use the macro below to release the interface.
//
//=====================================================================
//=====================================================================

#define QueryFilterInfoReleaseGraph(fi) if ((fi).pGraph) (fi).pGraph->Release();

#define QueryPinInfoReleaseFilter(pi) if ((pi).pFilter) (pi).pFilter->Release();

//=====================================================================
//=====================================================================
// Defines CBaseMediaFilter
//
// Abstract base class implementing IMediaFilter.
//
// Typically you will derive your filter from CBaseFilter rather than
// this,  unless you are implementing an object such as a plug-in
// distributor that needs to support IMediaFilter but not IBaseFilter.
//
// Note that IMediaFilter is derived from IPersist to allow query of
// class id.
//=====================================================================
//=====================================================================

class AM_NOVTABLE CBaseMediaFilter : public CUnknown,
                                     public IMediaFilter
{

protected:

    FILTER_STATE    m_State;            // current state: running, paused
    IReferenceClock *m_pClock;          // this filter's reference clock
    // note: all filters in a filter graph use the same clock

    // offset from stream time to reference time
    CRefTime        m_tStart;

    CLSID	    m_clsid;            // This filters clsid
                                        // used for serialization
    CCritSec        *m_pLock;           // Object we use for locking

public:

    CBaseMediaFilter(
        __in_opt LPCTSTR pName,
        __inout_opt LPUNKNOWN pUnk,
        __in CCritSec  *pLock,
	REFCLSID   clsid);

    virtual ~CBaseMediaFilter();

    DECLARE_IUNKNOWN

    // override this to say what interfaces we support where
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void ** ppv);

    //
    // --- IPersist method ---
    //

    STDMETHODIMP GetClassID(__out CLSID *pClsID);

    // --- IMediaFilter methods ---

    STDMETHODIMP GetState(DWORD dwMSecs, __out FILTER_STATE *State);

    STDMETHODIMP SetSyncSource(__inout_opt IReferenceClock *pClock);

    STDMETHODIMP GetSyncSource(__deref_out_opt IReferenceClock **pClock);

    // default implementation of Stop and Pause just record the
    // state. Override to activate or de-activate your filter.
    // Note that Run when called from Stopped state will call Pause
    // to ensure activation, so if you are a source or transform
    // you will probably not need to override Run.
    STDMETHODIMP Stop();
    STDMETHODIMP Pause();


    // the start parameter is the difference to be added to the
    // sample's stream time to get the reference time for
    // its presentation
    STDMETHODIMP Run(REFERENCE_TIME tStart);

    // --- helper methods ---

    // return the current stream time - ie find out what
    // stream time should be appearing now
    virtual HRESULT StreamTime(CRefTime& rtStream);

    // Is the filter currently active? (running or paused)
    BOOL IsActive() {
        CAutoLock cObjectLock(m_pLock);
        return ((m_State == State_Paused) || (m_State == State_Running));
    };
};

//=====================================================================
//=====================================================================
// Defines CBaseFilter
//
// An abstract class providing basic IBaseFilter support for pin
// enumeration and filter information reading.
//
// We cannot derive from CBaseMediaFilter since methods in IMediaFilter
// are also in IBaseFilter and would be ambiguous. Since much of the code
// assumes that they derive from a class that has m_State and other state
// directly available, we duplicate code from CBaseMediaFilter rather than
// having a member variable.
//
// Derive your filter from this, or from a derived object such as
// CTransformFilter.
//=====================================================================
//=====================================================================


class AM_NOVTABLE CBaseFilter : public CUnknown,        // Handles an IUnknown
                    public IBaseFilter,     // The Filter Interface
                    public IAMovieSetup     // For un/registration
{

friend class CBasePin;

protected:
    FILTER_STATE    m_State;            // current state: running, paused
    IReferenceClock *m_pClock;          // this graph's ref clock
    CRefTime        m_tStart;           // offset from stream time to reference time
    CLSID	    m_clsid;            // This filters clsid
                                        // used for serialization
    CCritSec        *m_pLock;           // Object we use for locking

    WCHAR           *m_pName;           // Full filter name
    IFilterGraph    *m_pGraph;          // Graph we belong to
    IMediaEventSink *m_pSink;           // Called with notify events
    LONG            m_PinVersion;       // Current pin version

public:

    CBaseFilter(
        __in_opt LPCTSTR pName,   // Object description
        __inout_opt LPUNKNOWN pUnk,  // IUnknown of delegating object
        __in CCritSec  *pLock,    // Object who maintains lock
	REFCLSID   clsid);        // The clsid to be used to serialize this filter

    CBaseFilter(
        __in_opt LPCTSTR pName,    // Object description
        __in_opt LPUNKNOWN pUnk,  // IUnknown of delegating object
        __in CCritSec  *pLock,    // Object who maintains lock
	REFCLSID   clsid,         // The clsid to be used to serialize this filter
        __inout HRESULT   *phr);  // General OLE return code
#ifdef UNICODE
    CBaseFilter(
        __in_opt LPCSTR pName,    // Object description
        __in_opt LPUNKNOWN pUnk,  // IUnknown of delegating object
        __in CCritSec  *pLock,    // Object who maintains lock
	REFCLSID   clsid);        // The clsid to be used to serialize this filter

    CBaseFilter(
        __in_opt LPCSTR pName,     // Object description
        __in_opt LPUNKNOWN pUnk,  // IUnknown of delegating object
        __in CCritSec  *pLock,    // Object who maintains lock
	REFCLSID   clsid,         // The clsid to be used to serialize this filter
        __inout HRESULT   *phr);  // General OLE return code
#endif
    ~CBaseFilter();

	DECLARE_IUNKNOWN

	/*STDMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppv) {
        return GetOwner()->QueryInterface(riid,ppv);
    };
    STDMETHODIMP_(ULONG) AddRef() {
		printf("tää toimii!");
		GetOwner();
		printf("ja tää!");
		GetOwner()->AddRef();
		printf("tää ei :(!");
        return GetOwner()->AddRef();
    };
    STDMETHODIMP_(ULONG) Release() {
        return GetOwner()->Release();
    };*/


    // override this to say what interfaces we support where
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void ** ppv);
#ifdef DEBUG
    STDMETHODIMP_(ULONG) NonDelegatingRelease();
#endif

    //
    // --- IPersist method ---
    //

    STDMETHODIMP GetClassID(__out CLSID *pClsID);

    // --- IMediaFilter methods ---

    STDMETHODIMP GetState(DWORD dwMSecs, __out FILTER_STATE *State);

    STDMETHODIMP SetSyncSource(__in_opt IReferenceClock *pClock);

    STDMETHODIMP GetSyncSource(__deref_out_opt IReferenceClock **pClock);


    // override Stop and Pause so we can activate the pins.
    // Note that Run will call Pause first if activation needed.
    // Override these if you want to activate your filter rather than
    // your pins.
    STDMETHODIMP Stop();
    STDMETHODIMP Pause();

    // the start parameter is the difference to be added to the
    // sample's stream time to get the reference time for
    // its presentation
    STDMETHODIMP Run(REFERENCE_TIME tStart);

    // --- helper methods ---

    // return the current stream time - ie find out what
    // stream time should be appearing now
    virtual HRESULT StreamTime(CRefTime& rtStream);

    // Is the filter currently active?
    BOOL IsActive() {
        CAutoLock cObjectLock(m_pLock);
        return ((m_State == State_Paused) || (m_State == State_Running));
    };

    // Is this filter stopped (without locking)
    BOOL IsStopped() {
        return (m_State == State_Stopped);
    };

    //
    // --- IBaseFilter methods ---
    //

    // pin enumerator
    STDMETHODIMP EnumPins(
                    __deref_out IEnumPins ** ppEnum);


    // default behaviour of FindPin assumes pin ids are their names
    STDMETHODIMP FindPin(
        LPCWSTR Id,
        __deref_out IPin ** ppPin
    );

    STDMETHODIMP QueryFilterInfo(
                    __out FILTER_INFO * pInfo);

    STDMETHODIMP JoinFilterGraph(
                    __inout_opt IFilterGraph * pGraph,
                    __in_opt LPCWSTR pName);

    // return a Vendor information string. Optional - may return E_NOTIMPL.
    // memory returned should be freed using CoTaskMemFree
    // default implementation returns E_NOTIMPL
    STDMETHODIMP QueryVendorInfo(
                    __deref_out LPWSTR* pVendorInfo
            );

    // --- helper methods ---

    // send an event notification to the filter graph if we know about it.
    // returns S_OK if delivered, S_FALSE if the filter graph does not sink
    // events, or an error otherwise.
    HRESULT NotifyEvent(
        long EventCode,
        LONG_PTR EventParam1,
        LONG_PTR EventParam2);

    // return the filter graph we belong to
    __out_opt IFilterGraph *GetFilterGraph() {
        return m_pGraph;
    }

    // Request reconnect
    // pPin is the pin to reconnect
    // pmt is the type to reconnect with - can be NULL
    // Calls ReconnectEx on the filter graph
    HRESULT ReconnectPin(IPin *pPin, __in_opt AM_MEDIA_TYPE const *pmt);

    // find out the current pin version (used by enumerators)
    virtual LONG GetPinVersion();
    void IncrementPinVersion();

    // you need to supply these to access the pins from the enumerator
    // and for default Stop and Pause/Run activation.
    virtual int GetPinCount() PURE;
    virtual CBasePin *GetPin(int n) PURE;

    // --- IAMovieSetup methods ---

    STDMETHODIMP Register();    // ask filter to register itself
    STDMETHODIMP Unregister();  // and unregister itself

    // --- setup helper methods ---
    // (override to return filters setup data)

    virtual __out_opt LPAMOVIESETUP_FILTER GetSetupData(){ return NULL; }

};


//=====================================================================
//=====================================================================
// Defines CBasePin
//
// Abstract class that supports the basics of IPin
//=====================================================================
//=====================================================================

class  AM_NOVTABLE CBasePin : public CUnknown, public IPin, public IQualityControl
{

protected:

    WCHAR *         m_pName;		        // This pin's name
    IPin            *m_Connected;               // Pin we have connected to
    PIN_DIRECTION   m_dir;                      // Direction of this pin
    CCritSec        *m_pLock;                   // Object we use for locking
    bool            m_bRunTimeError;            // Run time error generated
    bool            m_bCanReconnectWhenActive;  // OK to reconnect when active
    bool            m_bTryMyTypesFirst;         // When connecting enumerate
                                                // this pin's types first
    CBaseFilter    *m_pFilter;                  // Filter we were created by
    IQualityControl *m_pQSink;                  // Target for Quality messages
    LONG            m_TypeVersion;              // Holds current type version
    CMediaType      m_mt;                       // Media type of connection

    CRefTime        m_tStart;                   // time from NewSegment call
    CRefTime        m_tStop;                    // time from NewSegment
    double          m_dRate;                    // rate from NewSegment

#ifdef DEBUG
    LONG            m_cRef;                     // Ref count tracing
#endif

    // displays pin connection information

#ifdef DEBUG
    void DisplayPinInfo(IPin *pReceivePin);
    void DisplayTypeInfo(IPin *pPin, const CMediaType *pmt);
#else
    void DisplayPinInfo(IPin *pReceivePin) {};
    void DisplayTypeInfo(IPin *pPin, const CMediaType *pmt) {};
#endif

    // used to agree a media type for a pin connection

    // given a specific media type, attempt a connection (includes
    // checking that the type is acceptable to this pin)
    HRESULT
    AttemptConnection(
        IPin* pReceivePin,      // connect to this pin
        const CMediaType* pmt   // using this type
    );

    // try all the media types in this enumerator - for each that
    // we accept, try to connect using ReceiveConnection.
    HRESULT TryMediaTypes(
                        IPin *pReceivePin,          // connect to this pin
                        __in_opt const CMediaType *pmt,  // proposed type from Connect
                        IEnumMediaTypes *pEnum);    // try this enumerator

    // establish a connection with a suitable mediatype. Needs to
    // propose a media type if the pmt pointer is null or partially
    // specified - use TryMediaTypes on both our and then the other pin's
    // enumerator until we find one that works.
    HRESULT AgreeMediaType(
                        IPin *pReceivePin,      // connect to this pin
                        const CMediaType *pmt);      // proposed type from Connect

public:

    CBasePin(
        __in_opt LPCTSTR pObjectName,         // Object description
        __in CBaseFilter *pFilter,       // Owning filter who knows about pins
        __in CCritSec *pLock,            // Object who implements the lock
        __inout HRESULT *phr,               // General OLE return code
        __in_opt LPCWSTR pName,              // Pin name for us
        PIN_DIRECTION dir);         // Either PINDIR_INPUT or PINDIR_OUTPUT
#ifdef UNICODE
    CBasePin(
        __in_opt LPCSTR pObjectName,         // Object description
        __in CBaseFilter *pFilter,       // Owning filter who knows about pins
        __in CCritSec *pLock,            // Object who implements the lock
        __inout HRESULT *phr,               // General OLE return code
        __in_opt LPCWSTR pName,              // Pin name for us
        PIN_DIRECTION dir);         // Either PINDIR_INPUT or PINDIR_OUTPUT
#endif
    virtual ~CBasePin();

    DECLARE_IUNKNOWN

    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void ** ppv);
    STDMETHODIMP_(ULONG) NonDelegatingRelease();
    STDMETHODIMP_(ULONG) NonDelegatingAddRef();

    // --- IPin methods ---

    // take lead role in establishing a connection. Media type pointer
    // may be null, or may point to partially-specified mediatype
    // (subtype or format type may be GUID_NULL).
    STDMETHODIMP Connect(
        IPin * pReceivePin,
        __in_opt const AM_MEDIA_TYPE *pmt   // optional media type
    );

    // (passive) accept a connection from another pin
    STDMETHODIMP ReceiveConnection(
        IPin * pConnector,      // this is the initiating connecting pin
        const AM_MEDIA_TYPE *pmt   // this is the media type we will exchange
    );

    STDMETHODIMP Disconnect();

    STDMETHODIMP ConnectedTo(__deref_out IPin **pPin);

    STDMETHODIMP ConnectionMediaType(__out AM_MEDIA_TYPE *pmt);

    STDMETHODIMP QueryPinInfo(
        __out PIN_INFO * pInfo
    );

    STDMETHODIMP QueryDirection(
    	__out PIN_DIRECTION * pPinDir
    );

    STDMETHODIMP QueryId(
        __deref_out LPWSTR * Id
    );

    // does the pin support this media type
    STDMETHODIMP QueryAccept(
        const AM_MEDIA_TYPE *pmt
    );

    // return an enumerator for this pins preferred media types
    STDMETHODIMP EnumMediaTypes(
        __deref_out IEnumMediaTypes **ppEnum
    );

    // return an array of IPin* - the pins that this pin internally connects to
    // All pins put in the array must be AddReffed (but no others)
    // Errors: "Can't say" - FAIL, not enough slots - return S_FALSE
    // Default: return E_NOTIMPL
    // The filter graph will interpret NOT_IMPL as any input pin connects to
    // all visible output pins and vice versa.
    // apPin can be NULL if nPin==0 (not otherwise).
    STDMETHODIMP QueryInternalConnections(
        __out_ecount_part(*nPin,*nPin) IPin* *apPin,     // array of IPin*
        __inout ULONG *nPin                  // on input, the number of slots
                                             // on output  the number of pins
    ) { return E_NOTIMPL; }

    // Called when no more data will be sent
    STDMETHODIMP EndOfStream(void);

    // Begin/EndFlush still PURE

    // NewSegment notifies of the start/stop/rate applying to the data
    // about to be received. Default implementation records data and
    // returns S_OK.
    // Override this to pass downstream.
    STDMETHODIMP NewSegment(
                    REFERENCE_TIME tStart,
                    REFERENCE_TIME tStop,
                    double dRate);

    //================================================================================
    // IQualityControl methods
    //================================================================================

    STDMETHODIMP Notify(IBaseFilter * pSender, Quality q);

    STDMETHODIMP SetSink(IQualityControl * piqc);

    // --- helper methods ---

    // Returns true if the pin is connected. false otherwise.
    BOOL IsConnected(void) {return (m_Connected != NULL); };
    // Return the pin this is connected to (if any)
    IPin * GetConnected() { return m_Connected; };

    // Check if our filter is currently stopped
    BOOL IsStopped() {
        return (m_pFilter->m_State == State_Stopped);
    };

    // find out the current type version (used by enumerators)
    virtual LONG GetMediaTypeVersion();
    void IncrementTypeVersion();

    // switch the pin to active (paused or running) mode
    // not an error to call this if already active
    virtual HRESULT Active(void);

    // switch the pin to inactive state - may already be inactive
    virtual HRESULT Inactive(void);

    // Notify of Run() from filter
    virtual HRESULT Run(REFERENCE_TIME tStart);

    // check if the pin can support this specific proposed type and format
    virtual HRESULT CheckMediaType(const CMediaType *) PURE;

    // set the connection to use this format (previously agreed)
    virtual HRESULT SetMediaType(const CMediaType *);

    // check that the connection is ok before verifying it
    // can be overridden eg to check what interfaces will be supported.
    virtual HRESULT CheckConnect(IPin *);

    // Set and release resources required for a connection
    virtual HRESULT BreakConnect();
    virtual HRESULT CompleteConnect(IPin *pReceivePin);

    // returns the preferred formats for a pin
    virtual HRESULT GetMediaType(int iPosition, __inout CMediaType *pMediaType);

    // access to NewSegment values
    REFERENCE_TIME CurrentStopTime() {
        return m_tStop;
    }
    REFERENCE_TIME CurrentStartTime() {
        return m_tStart;
    }
    double CurrentRate() {
        return m_dRate;
    }

    //  Access name
    LPWSTR Name() { return m_pName; };

    //  Can reconnectwhen active?
    void SetReconnectWhenActive(bool bCanReconnect)
    {
        m_bCanReconnectWhenActive = bCanReconnect;
    }

    bool CanReconnectWhenActive()
    {
        return m_bCanReconnectWhenActive;
    }

protected:
    STDMETHODIMP DisconnectInternal();
};


//=====================================================================
//=====================================================================
// Defines CEnumPins
//
// Pin enumerator class that works by calling CBaseFilter. This interface
// is provided by CBaseFilter::EnumPins and calls GetPinCount() and
// GetPin() to enumerate existing pins. Needs to be a separate object so
// that it can be cloned (creating an existing object at the same
// position in the enumeration)
//
//=====================================================================
//=====================================================================

class CEnumPins : public IEnumPins      // The interface we support
{
    int m_Position;                 // Current ordinal position
    int m_PinCount;                 // Number of pins available
    CBaseFilter *m_pFilter;         // The filter who owns us
    LONG m_Version;                 // Pin version information
    LONG m_cRef;

    typedef CGenericList<CBasePin> CPinList;

    CPinList m_PinCache;	    // These pointers have not been AddRef'ed and
				    // so they should not be dereferenced.  They are
				    // merely kept to ID which pins have been enumerated.

#ifdef DEBUG
    DWORD m_dwCookie;
#endif

    /* If while we are retrieving a pin for example from the filter an error
       occurs we assume that our internal state is stale with respect to the
       filter (someone may have deleted all the pins). We can check before
       starting whether or not the operation is likely to fail by asking the
       filter what it's current version number is. If the filter has not
       overriden the GetPinVersion method then this will always match */

    BOOL AreWeOutOfSync() {
        return (m_pFilter->GetPinVersion() == m_Version ? FALSE : TRUE);
    };

    /* This method performs the same operations as Reset, except is does not clear
       the cache of pins already enumerated. */

    STDMETHODIMP Refresh();

public:

    CEnumPins(
        __in CBaseFilter *pFilter,
        __in_opt CEnumPins *pEnumPins);

    virtual ~CEnumPins();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IEnumPins
    STDMETHODIMP Next(
        ULONG cPins,         // place this many pins...
        __out_ecount(cPins) IPin ** ppPins,    // ...in this array of IPin*
        __out_opt ULONG * pcFetched    // actual count passed returned here
    );

    STDMETHODIMP Skip(ULONG cPins);
    STDMETHODIMP Reset();
    STDMETHODIMP Clone(__deref_out IEnumPins **ppEnum);


};


//=====================================================================
//=====================================================================
// Defines CEnumMediaTypes
//
// Enumerates the preferred formats for input and output pins
//=====================================================================
//=====================================================================

class CEnumMediaTypes : public IEnumMediaTypes    // The interface we support
{
    int m_Position;           // Current ordinal position
    CBasePin *m_pPin;         // The pin who owns us
    LONG m_Version;           // Media type version value
    LONG m_cRef;
#ifdef DEBUG
    DWORD m_dwCookie;
#endif

    /* The media types a filter supports can be quite dynamic so we add to
       the general IEnumXXXX interface the ability to be signaled when they
       change via an event handle the connected filter supplies. Until the
       Reset method is called after the state changes all further calls to
       the enumerator (except Reset) will return E_UNEXPECTED error code */

    BOOL AreWeOutOfSync() {
        return (m_pPin->GetMediaTypeVersion() == m_Version ? FALSE : TRUE);
    };

public:

    CEnumMediaTypes(
        __in CBasePin *pPin,
        __in_opt CEnumMediaTypes *pEnumMediaTypes);

    virtual ~CEnumMediaTypes();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // IEnumMediaTypes
    STDMETHODIMP Next(
        ULONG cMediaTypes,          // place this many pins...
        __out_ecount(cMediaTypes) AM_MEDIA_TYPE ** ppMediaTypes,  // ...in this array
        __out_opt ULONG * pcFetched           // actual count passed
    );

    STDMETHODIMP Skip(ULONG cMediaTypes);
    STDMETHODIMP Reset();
    STDMETHODIMP Clone(__deref_out IEnumMediaTypes **ppEnum);
};




//=====================================================================
//=====================================================================
// Defines CBaseOutputPin
//
// class derived from CBasePin that can pass buffers to a connected pin
// that supports IMemInputPin. Supports IPin.
//
// Derive your output pin from this.
//
//=====================================================================
//=====================================================================

class  AM_NOVTABLE CBaseOutputPin : public CBasePin
{

protected:

    IMemAllocator *m_pAllocator;
    IMemInputPin *m_pInputPin;        // interface on the downstreaminput pin
                                      // set up in CheckConnect when we connect.

public:

    CBaseOutputPin(
        __in_opt LPCTSTR pObjectName,
        __in CBaseFilter *pFilter,
        __in CCritSec *pLock,
        __inout HRESULT *phr,
        __in_opt LPCWSTR pName);
#ifdef UNICODE
    CBaseOutputPin(
        __in_opt LPCSTR pObjectName,
        __in CBaseFilter *pFilter,
        __in CCritSec *pLock,
        __inout HRESULT *phr,
        __in_opt LPCWSTR pName);
#endif
    // override CompleteConnect() so we can negotiate an allocator
    virtual HRESULT CompleteConnect(IPin *pReceivePin);

    // negotiate the allocator and its buffer size/count and other properties
    // Calls DecideBufferSize to set properties
    virtual HRESULT DecideAllocator(IMemInputPin * pPin, __deref_out IMemAllocator ** pAlloc);

    // override this to set the buffer size and count. Return an error
    // if the size/count is not to your liking.
    // The allocator properties passed in are those requested by the
    // input pin - use eg the alignment and prefix members if you have
    // no preference on these.
    virtual HRESULT DecideBufferSize(
        IMemAllocator * pAlloc,
        __inout ALLOCATOR_PROPERTIES * ppropInputRequest
    ) PURE;

    // returns an empty sample buffer from the allocator
    virtual HRESULT GetDeliveryBuffer(__deref_out IMediaSample ** ppSample,
                                      __in_opt REFERENCE_TIME * pStartTime,
                                      __in_opt REFERENCE_TIME * pEndTime,
                                      DWORD dwFlags);

    // deliver a filled-in sample to the connected input pin
    // note - you need to release it after calling this. The receiving
    // pin will addref the sample if it needs to hold it beyond the
    // call.
    virtual HRESULT Deliver(IMediaSample *);

    // override this to control the connection
    virtual HRESULT InitAllocator(__deref_out IMemAllocator **ppAlloc);
    HRESULT CheckConnect(IPin *pPin);
    HRESULT BreakConnect();

    // override to call Commit and Decommit
    HRESULT Active(void);
    HRESULT Inactive(void);

    // we have a default handling of EndOfStream which is to return
    // an error, since this should be called on input pins only
    STDMETHODIMP EndOfStream(void);

    // called from elsewhere in our filter to pass EOS downstream to
    // our connected input pin
    virtual HRESULT DeliverEndOfStream(void);

    // same for Begin/EndFlush - we handle Begin/EndFlush since it
    // is an error on an output pin, and we have Deliver methods to
    // call the methods on the connected pin
    STDMETHODIMP BeginFlush(void);
    STDMETHODIMP EndFlush(void);
    virtual HRESULT DeliverBeginFlush(void);
    virtual HRESULT DeliverEndFlush(void);

    // deliver NewSegment to connected pin - you will need to
    // override this if you queue any data in your output pin.
    virtual HRESULT DeliverNewSegment(
                        REFERENCE_TIME tStart,
                        REFERENCE_TIME tStop,
                        double dRate);

    //================================================================================
    // IQualityControl methods
    //================================================================================

    // All inherited from CBasePin and not overridden here.
    // STDMETHODIMP Notify(IBaseFilter * pSender, Quality q);
    // STDMETHODIMP SetSink(IQualityControl * piqc);
};


//=====================================================================
//=====================================================================
// Defines CBaseInputPin
//
// derive your standard input pin from this.
// you need to supply GetMediaType and CheckConnect etc (see CBasePin),
// and you need to supply Receive to do something more useful.
//
//=====================================================================
//=====================================================================

class AM_NOVTABLE CBaseInputPin : public CBasePin,
                                  public IMemInputPin
{

protected:

    IMemAllocator *m_pAllocator;    // Default memory allocator

    // allocator is read-only, so received samples
    // cannot be modified (probably only relevant to in-place
    // transforms
    BYTE m_bReadOnly;

    // in flushing state (between BeginFlush and EndFlush)
    // if TRUE, all Receives are returned with S_FALSE
    BYTE m_bFlushing;

    // Sample properties - initalized in Receive
    AM_SAMPLE2_PROPERTIES m_SampleProps;

public:

    CBaseInputPin(
        __in_opt LPCTSTR pObjectName,
        __in CBaseFilter *pFilter,
        __in CCritSec *pLock,
        __inout HRESULT *phr,
        __in_opt LPCWSTR pName);
#ifdef UNICODE
    CBaseInputPin(
        __in_opt LPCSTR pObjectName,
        __in CBaseFilter *pFilter,
        __in CCritSec *pLock,
        __inout HRESULT *phr,
        __in_opt LPCWSTR pName);
#endif
    virtual ~CBaseInputPin();

    DECLARE_IUNKNOWN

    // override this to publicise our interfaces
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);

    // return the allocator interface that this input pin
    // would like the output pin to use
    STDMETHODIMP GetAllocator(__deref_out IMemAllocator ** ppAllocator);

    // tell the input pin which allocator the output pin is actually
    // going to use.
    STDMETHODIMP NotifyAllocator(
                    IMemAllocator * pAllocator,
                    BOOL bReadOnly);

    // do something with this media sample
    STDMETHODIMP Receive(IMediaSample *pSample);

    // do something with these media samples
    STDMETHODIMP ReceiveMultiple (
        __in_ecount(nSamples) IMediaSample **pSamples,
        long nSamples,
        __out long *nSamplesProcessed);

    // See if Receive() blocks
    STDMETHODIMP ReceiveCanBlock();

    // Default handling for BeginFlush - call at the beginning
    // of your implementation (makes sure that all Receive calls
    // fail). After calling this, you need to free any queued data
    // and then call downstream.
    STDMETHODIMP BeginFlush(void);

    // default handling for EndFlush - call at end of your implementation
    // - before calling this, ensure that there is no queued data and no thread
    // pushing any more without a further receive, then call downstream,
    // then call this method to clear the m_bFlushing flag and re-enable
    // receives
    STDMETHODIMP EndFlush(void);

    // this method is optional (can return E_NOTIMPL).
    // default implementation returns E_NOTIMPL. Override if you have
    // specific alignment or prefix needs, but could use an upstream
    // allocator
    STDMETHODIMP GetAllocatorRequirements(__out ALLOCATOR_PROPERTIES*pProps);

    // Release the pin's allocator.
    HRESULT BreakConnect();

    // helper method to check the read-only flag
    BOOL IsReadOnly() {
        return m_bReadOnly;
    };

    // helper method to see if we are flushing
    BOOL IsFlushing() {
        return m_bFlushing;
    };

    //  Override this for checking whether it's OK to process samples
    //  Also call this from EndOfStream.
    virtual HRESULT CheckStreaming();

    // Pass a Quality notification on to the appropriate sink
    HRESULT PassNotify(Quality& q);


    //================================================================================
    // IQualityControl methods (from CBasePin)
    //================================================================================

    STDMETHODIMP Notify(IBaseFilter * pSender, Quality q);

    // no need to override:
    // STDMETHODIMP SetSink(IQualityControl * piqc);


    // switch the pin to inactive state - may already be inactive
    virtual HRESULT Inactive(void);

    // Return sample properties pointer
    AM_SAMPLE2_PROPERTIES * SampleProps() {
        ASSERT(m_SampleProps.cbData != 0);
        return &m_SampleProps;
    }

};

///////////////////////////////////////////////////////////////////////////
// CDynamicOutputPin
//

class CDynamicOutputPin : public CBaseOutputPin,
                          public IPinFlowControl
{
public:
#ifdef UNICODE
    CDynamicOutputPin(
        __in_opt LPCSTR pObjectName,
        __in CBaseFilter *pFilter,
        __in CCritSec *pLock,
        __inout HRESULT *phr,
        __in_opt LPCWSTR pName);
#endif

    CDynamicOutputPin(
        __in_opt LPCTSTR pObjectName,
        __in CBaseFilter *pFilter,
        __in CCritSec *pLock,
        __inout HRESULT *phr,
        __in_opt LPCWSTR pName);

    ~CDynamicOutputPin();

    // IUnknown Methods
    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);

    // IPin Methods
    STDMETHODIMP Disconnect(void);

    // IPinFlowControl Methods
    STDMETHODIMP Block(DWORD dwBlockFlags, HANDLE hEvent);

    //  Set graph config info
    void SetConfigInfo(IGraphConfig *pGraphConfig, HANDLE hStopEvent);

    #ifdef DEBUG
    virtual HRESULT Deliver(IMediaSample *pSample);
    virtual HRESULT DeliverEndOfStream(void);
    virtual HRESULT DeliverNewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate);
    #endif // DEBUG

    HRESULT DeliverBeginFlush(void);
    HRESULT DeliverEndFlush(void);

    HRESULT Inactive(void);
    HRESULT Active(void);
    virtual HRESULT CompleteConnect(IPin *pReceivePin);

    virtual HRESULT StartUsingOutputPin(void);
    virtual void StopUsingOutputPin(void);
    virtual bool StreamingThreadUsingOutputPin(void);

    HRESULT ChangeOutputFormat
        (
        const AM_MEDIA_TYPE *pmt,
        REFERENCE_TIME tSegmentStart,
        REFERENCE_TIME tSegmentStop,
        double dSegmentRate
        );
    HRESULT ChangeMediaType(const CMediaType *pmt);
    HRESULT DynamicReconnect(const CMediaType *pmt);

protected:
    HRESULT SynchronousBlockOutputPin(void);
    HRESULT AsynchronousBlockOutputPin(HANDLE hNotifyCallerPinBlockedEvent);
    HRESULT UnblockOutputPin(void);

    void BlockOutputPin(void);
    void ResetBlockState(void);

    static HRESULT WaitEvent(HANDLE hEvent);

    enum BLOCK_STATE
    {
        NOT_BLOCKED,
        PENDING,
        BLOCKED
    };

    // This lock should be held when the following class members are
    // being used: m_hNotifyCallerPinBlockedEvent, m_BlockState,
    // m_dwBlockCallerThreadID and m_dwNumOutstandingOutputPinUsers.
    CCritSec m_BlockStateLock;

    // This event should be signaled when the output pin is
    // not blocked.  This is a manual reset event.  For more
    // information on events, see the documentation for
    // CreateEvent() in the Windows SDK.
    HANDLE m_hUnblockOutputPinEvent;

    // This event will be signaled when block operation succeedes or
    // when the user cancels the block operation.  The block operation
    // can be canceled by calling IPinFlowControl2::Block( 0, NULL )
    // while the block operation is pending.
    HANDLE m_hNotifyCallerPinBlockedEvent;

    // The state of the current block operation.
    BLOCK_STATE m_BlockState;

    // The ID of the thread which last called IPinFlowControl::Block().
    // For more information on thread IDs, see the documentation for
    // GetCurrentThreadID() in the Windows SDK.
    DWORD m_dwBlockCallerThreadID;

    // The number of times StartUsingOutputPin() has been sucessfully
    // called and a corresponding call to StopUsingOutputPin() has not
    // been made.  When this variable is greater than 0, the streaming
    // thread is calling IPin::NewSegment(), IPin::EndOfStream(),
    // IMemInputPin::Receive() or IMemInputPin::ReceiveMultiple().  The
    // streaming thread could also be calling: DynamicReconnect(),
    // ChangeMediaType() or ChangeOutputFormat().  The output pin cannot
    // be blocked while the output pin is being used.
    DWORD m_dwNumOutstandingOutputPinUsers;

    // This event should be set when the IMediaFilter::Stop() is called.
    // This is a manual reset event.  It is also set when the output pin
    // delivers a flush to the connected input pin.
    HANDLE m_hStopEvent;
    IGraphConfig* m_pGraphConfig;

    // TRUE if the output pin's allocator's samples are read only.
    // Otherwise FALSE.  For more information, see the documentation
    // for IMemInputPin::NotifyAllocator().
    BOOL m_bPinUsesReadOnlyAllocator;

private:
    HRESULT Initialize(void);
    HRESULT ChangeMediaTypeHelper(const CMediaType *pmt);

    #ifdef DEBUG
    void AssertValid(void);
    #endif // DEBUG
};

class CAutoUsingOutputPin
{
public:
    CAutoUsingOutputPin( __in CDynamicOutputPin* pOutputPin, __inout HRESULT* phr );
    ~CAutoUsingOutputPin();

private:
    CDynamicOutputPin* m_pOutputPin;
};

inline CAutoUsingOutputPin::CAutoUsingOutputPin( __in CDynamicOutputPin* pOutputPin, __inout HRESULT* phr ) :
    m_pOutputPin(NULL)
{
    // The caller should always pass in valid pointers.
    ASSERT( NULL != pOutputPin );
    ASSERT( NULL != phr );

    // Make sure the user initialized phr.
    ASSERT( S_OK == *phr );

    HRESULT hr = pOutputPin->StartUsingOutputPin();
    if( FAILED( hr ) )
    {
        *phr = hr;
        return;
    }

    m_pOutputPin = pOutputPin;
}

inline CAutoUsingOutputPin::~CAutoUsingOutputPin()
{
    if( NULL != m_pOutputPin )
    {
        m_pOutputPin->StopUsingOutputPin();
    }
}

#ifdef DEBUG

inline HRESULT CDynamicOutputPin::Deliver(IMediaSample *pSample)
{
    // The caller should call StartUsingOutputPin() before calling this
    // method.
    ASSERT(StreamingThreadUsingOutputPin());

    return CBaseOutputPin::Deliver(pSample);
}

inline HRESULT CDynamicOutputPin::DeliverEndOfStream(void)
{
    // The caller should call StartUsingOutputPin() before calling this
    // method.
    ASSERT( StreamingThreadUsingOutputPin() );

    return CBaseOutputPin::DeliverEndOfStream();
}

inline HRESULT CDynamicOutputPin::DeliverNewSegment(REFERENCE_TIME tStart, REFERENCE_TIME tStop, double dRate)
{
    // The caller should call StartUsingOutputPin() before calling this
    // method.
    ASSERT(StreamingThreadUsingOutputPin());

    return CBaseOutputPin::DeliverNewSegment(tStart, tStop, dRate);
}

#endif // DEBUG

//=====================================================================
//=====================================================================
// Memory allocators
//
// the shared memory transport between pins requires the input pin
// to provide a memory allocator that can provide sample objects. A
// sample object supports the IMediaSample interface.
//
// CBaseAllocator handles the management of free and busy samples. It
// allocates CMediaSample objects. CBaseAllocator is an abstract class:
// in particular it has no method of initializing the list of free
// samples. CMemAllocator is derived from CBaseAllocator and initializes
// the list of samples using memory from the standard IMalloc interface.
//
// If you want your buffers to live in some special area of memory,
// derive your allocator object from CBaseAllocator. If you derive your
// IMemInputPin interface object from CBaseMemInputPin, you will get
// CMemAllocator-based allocation etc for free and will just need to
// supply the Receive handling, and media type / format negotiation.
//=====================================================================
//=====================================================================


//=====================================================================
//=====================================================================
// Defines CMediaSample
//
// an object of this class supports IMediaSample and represents a buffer
// for media data with some associated properties. Releasing it returns
// it to a freelist managed by a CBaseAllocator derived object.
//=====================================================================
//=====================================================================

class CMediaSample : public IMediaSample2    // The interface we support
{

protected:

    friend class CBaseAllocator;

    /*  Values for dwFlags - these are used for backward compatiblity
        only now - use AM_SAMPLE_xxx
    */
    enum { Sample_SyncPoint       = 0x01,   /* Is this a sync point */
           Sample_Preroll         = 0x02,   /* Is this a preroll sample */
           Sample_Discontinuity   = 0x04,   /* Set if start of new segment */
           Sample_TypeChanged     = 0x08,   /* Has the type changed */
           Sample_TimeValid       = 0x10,   /* Set if time is valid */
           Sample_MediaTimeValid  = 0x20,   /* Is the media time valid */
           Sample_TimeDiscontinuity = 0x40, /* Time discontinuity */
           Sample_StopValid       = 0x100,  /* Stop time valid */
           Sample_ValidFlags      = 0x1FF
         };

    /* Properties, the media sample class can be a container for a format
       change in which case we take a copy of a type through the SetMediaType
       interface function and then return it when GetMediaType is called. As
       we do no internal processing on it we leave it as a pointer */

    DWORD            m_dwFlags;         /* Flags for this sample */
                                        /* Type specific flags are packed
                                           into the top word
                                        */
    DWORD            m_dwTypeSpecificFlags; /* Media type specific flags */
    __field_ecount_opt(m_cbBuffer) LPBYTE           m_pBuffer;         /* Pointer to the complete buffer */
    LONG             m_lActual;         /* Length of data in this sample */
    LONG             m_cbBuffer;        /* Size of the buffer */
    CBaseAllocator  *m_pAllocator;      /* The allocator who owns us */
    CMediaSample     *m_pNext;          /* Chaining in free list */
    REFERENCE_TIME   m_Start;           /* Start sample time */
    REFERENCE_TIME   m_End;             /* End sample time */
    LONGLONG         m_MediaStart;      /* Real media start position */
    LONG             m_MediaEnd;        /* A difference to get the end */
    AM_MEDIA_TYPE    *m_pMediaType;     /* Media type change data */
    DWORD            m_dwStreamId;      /* Stream id */
public:
    LONG             m_cRef;            /* Reference count */


public:

    CMediaSample(
        __in_opt LPCTSTR pName,
        __in_opt CBaseAllocator *pAllocator,
        __inout_opt HRESULT *phr,
        __in_bcount_opt(length) LPBYTE pBuffer = NULL,
        LONG length = 0);
#ifdef UNICODE
    CMediaSample(
        __in_opt LPCSTR pName,
        __in_opt CBaseAllocator *pAllocator,
        __inout_opt HRESULT *phr,
        __in_bcount_opt(length) LPBYTE pBuffer = NULL,
        LONG length = 0);
#endif

    virtual ~CMediaSample();

    /* Note the media sample does not delegate to its owner */

    STDMETHODIMP QueryInterface(REFIID riid, __deref_out void **ppv);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();

    // set the buffer pointer and length. Used by allocators that
    // want variable sized pointers or pointers into already-read data.
    // This is only available through a CMediaSample* not an IMediaSample*
    // and so cannot be changed by clients.
    HRESULT SetPointer(__in_bcount(cBytes) BYTE * ptr, LONG cBytes);

    // Get me a read/write pointer to this buffer's memory.
    STDMETHODIMP GetPointer(__deref_out BYTE ** ppBuffer);

    STDMETHODIMP_(LONG) GetSize(void);

    // get the stream time at which this sample should start and finish.
    STDMETHODIMP GetTime(
        __out REFERENCE_TIME * pTimeStart,     // put time here
        __out REFERENCE_TIME * pTimeEnd
    );

    // Set the stream time at which this sample should start and finish.
    STDMETHODIMP SetTime(
        __in_opt REFERENCE_TIME * pTimeStart,     // put time here
        __in_opt REFERENCE_TIME * pTimeEnd
    );
    STDMETHODIMP IsSyncPoint(void);
    STDMETHODIMP SetSyncPoint(BOOL bIsSyncPoint);
    STDMETHODIMP IsPreroll(void);
    STDMETHODIMP SetPreroll(BOOL bIsPreroll);

    STDMETHODIMP_(LONG) GetActualDataLength(void);
    STDMETHODIMP SetActualDataLength(LONG lActual);

    // these allow for limited format changes in band

    STDMETHODIMP GetMediaType(__deref_out AM_MEDIA_TYPE **ppMediaType);
    STDMETHODIMP SetMediaType(__in_opt AM_MEDIA_TYPE *pMediaType);

    // returns S_OK if there is a discontinuity in the data (this same is
    // not a continuation of the previous stream of data
    // - there has been a seek).
    STDMETHODIMP IsDiscontinuity(void);
    // set the discontinuity property - TRUE if this sample is not a
    // continuation, but a new sample after a seek.
    STDMETHODIMP SetDiscontinuity(BOOL bDiscontinuity);

    // get the media times for this sample
    STDMETHODIMP GetMediaTime(
    	__out LONGLONG * pTimeStart,
	    __out LONGLONG * pTimeEnd
    );

    // Set the media times for this sample
    STDMETHODIMP SetMediaTime(
    	__in_opt LONGLONG * pTimeStart,
	    __in_opt LONGLONG * pTimeEnd
    );

    // Set and get properties (IMediaSample2)
    STDMETHODIMP GetProperties(
        DWORD cbProperties,
        __out_bcount(cbProperties) BYTE * pbProperties
    );

    STDMETHODIMP SetProperties(
        DWORD cbProperties,
        __in_bcount(cbProperties) const BYTE * pbProperties
    );
};


//=====================================================================
//=====================================================================
// Defines CBaseAllocator
//
// Abstract base class that manages a list of media samples
//
// This class provides support for getting buffers from the free list,
// including handling of commit and (asynchronous) decommit.
//
// Derive from this class and override the Alloc and Free functions to
// allocate your CMediaSample (or derived) objects and add them to the
// free list, preparing them as necessary.
//=====================================================================
//=====================================================================

class AM_NOVTABLE CBaseAllocator : public CUnknown,// A non delegating IUnknown
                       public IMemAllocatorCallbackTemp, // The interface we support
                       public CCritSec             // Provides object locking
{
    class CSampleList;
    friend class CSampleList;

    /*  Trick to get at protected member in CMediaSample */
    static CMediaSample * &NextSample(__in CMediaSample *pSample)
    {
        return pSample->m_pNext;
    };

    /*  Mini list class for the free list */
    class CSampleList
    {
    public:
        CSampleList() : m_List(NULL), m_nOnList(0) {};
#ifdef DEBUG
        ~CSampleList()
        {
            ASSERT(m_nOnList == 0);
        };
#endif
        CMediaSample *Head() const { return m_List; };
        CMediaSample *Next(__in CMediaSample *pSample) const { return CBaseAllocator::NextSample(pSample); };
        int GetCount() const { return m_nOnList; };
        void Add(__inout CMediaSample *pSample)
        {
            ASSERT(pSample != NULL);
            CBaseAllocator::NextSample(pSample) = m_List;
            m_List = pSample;
            m_nOnList++;
        };
        CMediaSample *RemoveHead()
        {
            CMediaSample *pSample = m_List;
            if (pSample != NULL) {
                m_List = CBaseAllocator::NextSample(m_List);
                m_nOnList--;
            }
            return pSample;
        };
        void Remove(__inout CMediaSample *pSample);

    public:
        CMediaSample *m_List;
        int           m_nOnList;
    };
protected:

    CSampleList m_lFree;        // Free list

    /*  Note to overriders of CBaseAllocator.

        We use a lazy signalling mechanism for waiting for samples.
        This means we don't call the OS if no waits occur.

        In order to implement this:

        1. When a new sample is added to m_lFree call NotifySample() which
           calls ReleaseSemaphore on m_hSem with a count of m_lWaiting and
           sets m_lWaiting to 0.
           This must all be done holding the allocator's critical section.

        2. When waiting for a sample call SetWaiting() which increments
           m_lWaiting BEFORE leaving the allocator's critical section.

        3. Actually wait by calling WaitForSingleObject(m_hSem, INFINITE)
           having left the allocator's critical section.  The effect of
           this is to remove 1 from the semaphore's count.  You MUST call
           this once having incremented m_lWaiting.

        The following are then true when the critical section is not held :
            (let nWaiting = number about to wait or waiting)

            (1) if (m_lFree.GetCount() != 0) then (m_lWaiting == 0)
            (2) m_lWaiting + Semaphore count == nWaiting

        We would deadlock if
           nWaiting != 0 &&
           m_lFree.GetCount() != 0 &&
           Semaphore count == 0

           But from (1) if m_lFree.GetCount() != 0 then m_lWaiting == 0 so
           from (2) Semaphore count == nWaiting (which is non-0) so the
           deadlock can't happen.
    */

    HANDLE m_hSem;              // For signalling
    long m_lWaiting;            // Waiting for a free element
    long m_lCount;              // how many buffers we have agreed to provide
    long m_lAllocated;          // how many buffers are currently allocated
    long m_lSize;               // agreed size of each buffer
    long m_lAlignment;          // agreed alignment
    long m_lPrefix;             // agreed prefix (preceeds GetPointer() value)
    BOOL m_bChanged;            // Have the buffer requirements changed

    // if true, we are decommitted and can't allocate memory
    BOOL m_bCommitted;
    // if true, the decommit has happened, but we haven't called Free yet
    // as there are still outstanding buffers
    BOOL m_bDecommitInProgress;

    //  Notification interface
    IMemAllocatorNotifyCallbackTemp *m_pNotify;

    BOOL m_fEnableReleaseCallback;

    // called to decommit the memory when the last buffer is freed
    // pure virtual - need to override this
    virtual void Free(void) PURE;

    // override to allocate the memory when commit called
    virtual HRESULT Alloc(void);

public:

    CBaseAllocator(
        __in_opt LPCTSTR , __inout_opt LPUNKNOWN, __inout HRESULT *,
        BOOL bEvent = TRUE, BOOL fEnableReleaseCallback = FALSE);
#ifdef UNICODE
    CBaseAllocator(
        __in_opt LPCSTR , __inout_opt LPUNKNOWN, __inout HRESULT *,
        BOOL bEvent = TRUE, BOOL fEnableReleaseCallback = FALSE);
#endif
    virtual ~CBaseAllocator();

    DECLARE_IUNKNOWN

    // override this to publicise our interfaces
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);

    STDMETHODIMP SetProperties(
		    __in ALLOCATOR_PROPERTIES* pRequest,
		    __out ALLOCATOR_PROPERTIES* pActual);

    // return the properties actually being used on this allocator
    STDMETHODIMP GetProperties(
		    __out ALLOCATOR_PROPERTIES* pProps);

    // override Commit to allocate memory. We handle the GetBuffer
    //state changes
    STDMETHODIMP Commit();

    // override this to handle the memory freeing. We handle any outstanding
    // GetBuffer calls
    STDMETHODIMP Decommit();

    // get container for a sample. Blocking, synchronous call to get the
    // next free buffer (as represented by an IMediaSample interface).
    // on return, the time etc properties will be invalid, but the buffer
    // pointer and size will be correct. The two time parameters are
    // optional and either may be NULL, they may alternatively be set to
    // the start and end times the sample will have attached to it
    // bPrevFramesSkipped is not used (used only by the video renderer's
    // allocator where it affects quality management in direct draw).

    STDMETHODIMP GetBuffer(__deref_out IMediaSample **ppBuffer,
                           __in_opt REFERENCE_TIME * pStartTime,
                           __in_opt REFERENCE_TIME * pEndTime,
                           DWORD dwFlags);

    // final release of a CMediaSample will call this
    STDMETHODIMP ReleaseBuffer(IMediaSample *pBuffer);
    // obsolete:: virtual void PutOnFreeList(CMediaSample * pSample);

    STDMETHODIMP SetNotify(IMemAllocatorNotifyCallbackTemp *pNotify);

    STDMETHODIMP GetFreeCount(__out LONG *plBuffersFree);

    // Notify that a sample is available
    void NotifySample();

    // Notify that we're waiting for a sample
    void SetWaiting() { m_lWaiting++; };
};


//=====================================================================
//=====================================================================
// Defines CMemAllocator
//
// this is an allocator based on CBaseAllocator that allocates sample
// buffers in main memory (from 'new'). You must call SetProperties
// before calling Commit.
//
// we don't free the memory when going into Decommit state. The simplest
// way to implement this without complicating CBaseAllocator is to
// have a Free() function, called to go into decommit state, that does
// nothing and a ReallyFree function called from our destructor that
// actually frees the memory.
//=====================================================================
//=====================================================================

//  Make me one from quartz.dll
STDAPI CreateMemoryAllocator(__deref_out IMemAllocator **ppAllocator);

class CMemAllocator : public CBaseAllocator
{

protected:

    LPBYTE m_pBuffer;   // combined memory for all buffers

    // override to free the memory when decommit completes
    // - we actually do nothing, and save the memory until deletion.
    void Free(void);

    // called from the destructor (and from Alloc if changing size/count) to
    // actually free up the memory
    void ReallyFree(void);

    // overriden to allocate the memory when commit called
    HRESULT Alloc(void);

public:
    /* This goes in the factory template table to create new instances */
    static CUnknown *CreateInstance(__inout_opt LPUNKNOWN, __inout HRESULT *);

    STDMETHODIMP SetProperties(
		    __in ALLOCATOR_PROPERTIES* pRequest,
		    __out ALLOCATOR_PROPERTIES* pActual);

    CMemAllocator(__in_opt LPCTSTR , __inout_opt LPUNKNOWN, __inout HRESULT *);
#ifdef UNICODE
    CMemAllocator(__in_opt LPCSTR , __inout_opt LPUNKNOWN, __inout HRESULT *);
#endif
    ~CMemAllocator();
};

// helper used by IAMovieSetup implementation
STDAPI
AMovieSetupRegisterFilter( const AMOVIESETUP_FILTER * const psetupdata
                         , IFilterMapper *                  pIFM
                         , BOOL                             bRegister  );


///////////////////////////////////////////////////////////////////////////
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////

#endif /* __FILTER__ */



//------------------------------------------------------------------------------
// File: Transfrm.h
//
// Desc: DirectShow base classes - defines classes from which simple 
//       transform codecs may be derived.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


// It assumes the codec has one input and one output stream, and has no
// interest in memory management, interface negotiation or anything else.
//
// derive your class from this, and supply Transform and the media type/format
// negotiation functions. Implement that class, compile and link and
// you're done.


#ifndef __TRANSFRM__
#define __TRANSFRM__

// ======================================================================
// This is the com object that represents a simple transform filter. It
// supports IBaseFilter, IMediaFilter and two pins through nested interfaces
// ======================================================================

class CTransformFilter;

// ==================================================
// Implements the input pin
// ==================================================

class CTransformInputPin : public CBaseInputPin
{
    friend class CTransformFilter;

protected:
    CTransformFilter *m_pTransformFilter;


public:

    CTransformInputPin(
        __in_opt LPCTSTR pObjectName,
        __inout CTransformFilter *pTransformFilter,
        __inout HRESULT * phr,
        __in_opt LPCWSTR pName);
#ifdef UNICODE
    CTransformInputPin(
        __in_opt LPCSTR pObjectName,
        __inout CTransformFilter *pTransformFilter,
        __inout HRESULT * phr,
        __in_opt LPCWSTR pName);
#endif

    STDMETHODIMP QueryId(__deref_out LPWSTR * Id)
    {
        return AMGetWideString(L"In", Id);
    }

    // Grab and release extra interfaces if required

    HRESULT CheckConnect(IPin *pPin);
    HRESULT BreakConnect();
    HRESULT CompleteConnect(IPin *pReceivePin);

    // check that we can support this output type
    HRESULT CheckMediaType(const CMediaType* mtIn);

    // set the connection media type
    HRESULT SetMediaType(const CMediaType* mt);

    // --- IMemInputPin -----

    // here's the next block of data from the stream.
    // AddRef it yourself if you need to hold it beyond the end
    // of this call.
    STDMETHODIMP Receive(IMediaSample * pSample);

    // provide EndOfStream that passes straight downstream
    // (there is no queued data)
    STDMETHODIMP EndOfStream(void);

    // passes it to CTransformFilter::BeginFlush
    STDMETHODIMP BeginFlush(void);

    // passes it to CTransformFilter::EndFlush
    STDMETHODIMP EndFlush(void);

    STDMETHODIMP NewSegment(
                        REFERENCE_TIME tStart,
                        REFERENCE_TIME tStop,
                        double dRate);

    // Check if it's OK to process samples
    virtual HRESULT CheckStreaming();

    // Media type
public:
    CMediaType& CurrentMediaType() { return m_mt; };

};

// ==================================================
// Implements the output pin
// ==================================================

class CTransformOutputPin : public CBaseOutputPin
{
    friend class CTransformFilter;

protected:
    CTransformFilter *m_pTransformFilter;

public:

    // implement IMediaPosition by passing upstream
    IUnknown * m_pPosition;

    CTransformOutputPin(
        __in_opt LPCTSTR pObjectName,
        __inout CTransformFilter *pTransformFilter,
        __inout HRESULT * phr,
        __in_opt LPCWSTR pName);
#ifdef UNICODE
    CTransformOutputPin(
        __in_opt LPCSTR pObjectName,
        __inout CTransformFilter *pTransformFilter,
        __inout HRESULT * phr,
        __in_opt LPCWSTR pName);
#endif
    ~CTransformOutputPin();

    // override to expose IMediaPosition
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);

    // --- CBaseOutputPin ------------

    STDMETHODIMP QueryId(__deref_out LPWSTR * Id)
    {
        return AMGetWideString(L"Out", Id);
    }

    // Grab and release extra interfaces if required

    HRESULT CheckConnect(IPin *pPin);
    HRESULT BreakConnect();
    HRESULT CompleteConnect(IPin *pReceivePin);

    // check that we can support this output type
    HRESULT CheckMediaType(const CMediaType* mtOut);

    // set the connection media type
    HRESULT SetMediaType(const CMediaType *pmt);

    // called from CBaseOutputPin during connection to ask for
    // the count and size of buffers we need.
    HRESULT DecideBufferSize(
                IMemAllocator * pAlloc,
                __inout ALLOCATOR_PROPERTIES *pProp);

    // returns the preferred formats for a pin
    HRESULT GetMediaType(int iPosition, __inout CMediaType *pMediaType);

    // inherited from IQualityControl via CBasePin
    STDMETHODIMP Notify(IBaseFilter * pSender, Quality q);

    // Media type
public:
    CMediaType& CurrentMediaType() { return m_mt; };
};


class AM_NOVTABLE CTransformFilter : public CBaseFilter
{

public:

    // map getpin/getpincount for base enum of pins to owner
    // override this to return more specialised pin objects

    virtual int GetPinCount();
    virtual CBasePin * GetPin(int n);
    STDMETHODIMP FindPin(LPCWSTR Id, __deref_out IPin **ppPin);

    // override state changes to allow derived transform filter
    // to control streaming start/stop
    STDMETHODIMP Stop();
    STDMETHODIMP Pause();

public:

    CTransformFilter(__in_opt LPCTSTR , __inout_opt LPUNKNOWN, REFCLSID clsid);
#ifdef UNICODE
    CTransformFilter(__in_opt LPCSTR , __inout_opt LPUNKNOWN, REFCLSID clsid);
#endif
    ~CTransformFilter();

    // =================================================================
    // ----- override these bits ---------------------------------------
    // =================================================================

    // These must be supplied in a derived class

    virtual HRESULT Transform(IMediaSample * pIn, IMediaSample *pOut);

    // check if you can support mtIn
    virtual HRESULT CheckInputType(const CMediaType* mtIn) PURE;

    // check if you can support the transform from this input to this output
    virtual HRESULT CheckTransform(const CMediaType* mtIn, const CMediaType* mtOut) PURE;

    // this goes in the factory template table to create new instances
    // static CCOMObject * CreateInstance(__inout_opt LPUNKNOWN, HRESULT *);

    // call the SetProperties function with appropriate arguments
    virtual HRESULT DecideBufferSize(
                        IMemAllocator * pAllocator,
                        __inout ALLOCATOR_PROPERTIES *pprop) PURE;

    // override to suggest OUTPUT pin media types
    virtual HRESULT GetMediaType(int iPosition, __inout CMediaType *pMediaType) PURE;



    // =================================================================
    // ----- Optional Override Methods           -----------------------
    // =================================================================

    // you can also override these if you want to know about streaming
    virtual HRESULT StartStreaming();
    virtual HRESULT StopStreaming();

    // override if you can do anything constructive with quality notifications
    virtual HRESULT AlterQuality(Quality q);

    // override this to know when the media type is actually set
    virtual HRESULT SetMediaType(PIN_DIRECTION direction,const CMediaType *pmt);

    // chance to grab extra interfaces on connection
    virtual HRESULT CheckConnect(PIN_DIRECTION dir,IPin *pPin);
    virtual HRESULT BreakConnect(PIN_DIRECTION dir);
    virtual HRESULT CompleteConnect(PIN_DIRECTION direction,IPin *pReceivePin);

    // chance to customize the transform process
    virtual HRESULT Receive(IMediaSample *pSample);

    // Standard setup for output sample
    HRESULT InitializeOutputSample(IMediaSample *pSample, __deref_out IMediaSample **ppOutSample);

    // if you override Receive, you may need to override these three too
    virtual HRESULT EndOfStream(void);
    virtual HRESULT BeginFlush(void);
    virtual HRESULT EndFlush(void);
    virtual HRESULT NewSegment(
                        REFERENCE_TIME tStart,
                        REFERENCE_TIME tStop,
                        double dRate);

#ifdef PERF
    // Override to register performance measurement with a less generic string
    // You should do this to avoid confusion with other filters
    virtual void RegisterPerfId()
         {m_idTransform = MSR_REGISTER(TEXT("Transform"));}
#endif // PERF


// implementation details

protected:

#ifdef PERF
    int m_idTransform;                 // performance measuring id
#endif
    BOOL m_bEOSDelivered;              // have we sent EndOfStream
    BOOL m_bSampleSkipped;             // Did we just skip a frame
    BOOL m_bQualityChanged;            // Have we degraded?

    // critical section protecting filter state.

    CCritSec m_csFilter;

    // critical section stopping state changes (ie Stop) while we're
    // processing a sample.
    //
    // This critical section is held when processing
    // events that occur on the receive thread - Receive() and EndOfStream().
    //
    // If you want to hold both m_csReceive and m_csFilter then grab
    // m_csFilter FIRST - like CTransformFilter::Stop() does.

    CCritSec m_csReceive;

    // these hold our input and output pins

    friend class CTransformInputPin;
    friend class CTransformOutputPin;
    CTransformInputPin *m_pInput;
    CTransformOutputPin *m_pOutput;
};

#endif /* __TRANSFRM__ */


//------------------------------------------------------------------------------
// File: TransIP.h
//
// Desc: DirectShow base classes - defines classes from which simple
//       Transform-In-Place filters may be derived.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


//
// The difference between this and Transfrm.h is that Transfrm copies the data.
//
// It assumes the filter has one input and one output stream, and has no
// interest in memory management, interface negotiation or anything else.
//
// Derive your class from this, and supply Transform and the media type/format
// negotiation functions. Implement that class, compile and link and
// you're done.


#ifndef __TRANSIP__
#define __TRANSIP__

// ======================================================================
// This is the com object that represents a simple transform filter. It
// supports IBaseFilter, IMediaFilter and two pins through nested interfaces
// ======================================================================

class CTransInPlaceFilter;

// Several of the pin functions call filter functions to do the work,
// so you can often use the pin classes unaltered, just overriding the
// functions in CTransInPlaceFilter.  If that's not enough and you want
// to derive your own pin class, override GetPin in the filter to supply
// your own pin classes to the filter.

// ==================================================
// Implements the input pin
// ==================================================

class CTransInPlaceInputPin : public CTransformInputPin
{

protected:
    CTransInPlaceFilter * const m_pTIPFilter;    // our filter
    BOOL                 m_bReadOnly;    // incoming stream is read only

public:

    CTransInPlaceInputPin(
        __in_opt LPCTSTR     pObjectName,
        __inout CTransInPlaceFilter *pFilter,
        __inout HRESULT             *phr,
        __in_opt LPCWSTR              pName);

    // --- IMemInputPin -----

    // Provide an enumerator for media types by getting one from downstream
    STDMETHODIMP EnumMediaTypes( __deref_out IEnumMediaTypes **ppEnum );

    // Say whether media type is acceptable.
    HRESULT CheckMediaType(const CMediaType* pmt);

    // Return our upstream allocator
    STDMETHODIMP GetAllocator(__deref_out IMemAllocator ** ppAllocator);

    // get told which allocator the upstream output pin is actually
    // going to use.
    STDMETHODIMP NotifyAllocator(IMemAllocator * pAllocator,
                                 BOOL bReadOnly);

    // Allow the filter to see what allocator we have
    // N.B. This does NOT AddRef
    __out IMemAllocator * PeekAllocator() const
        {  return m_pAllocator; }

    // Pass this on downstream if it ever gets called.
    STDMETHODIMP GetAllocatorRequirements(__out ALLOCATOR_PROPERTIES *pProps);

    HRESULT CompleteConnect(IPin *pReceivePin);

    inline const BOOL ReadOnly() { return m_bReadOnly ; }

};  // CTransInPlaceInputPin

// ==================================================
// Implements the output pin
// ==================================================

class CTransInPlaceOutputPin : public CTransformOutputPin
{

protected:
    // m_pFilter points to our CBaseFilter
    CTransInPlaceFilter * const m_pTIPFilter;

public:

    CTransInPlaceOutputPin(
        __in_opt LPCTSTR     pObjectName,
        __inout CTransInPlaceFilter *pFilter,
        __inout HRESULT             *phr,
        __in_opt LPCWSTR              pName);


    // --- CBaseOutputPin ------------

    // negotiate the allocator and its buffer size/count
    // Insists on using our own allocator.  (Actually the one upstream of us).
    // We don't override this - instead we just agree the default
    // then let the upstream filter decide for itself on reconnect
    // virtual HRESULT DecideAllocator(IMemInputPin * pPin, IMemAllocator ** pAlloc);

    // Provide a media type enumerator.  Get it from upstream.
    STDMETHODIMP EnumMediaTypes( __deref_out IEnumMediaTypes **ppEnum );

    // Say whether media type is acceptable.
    HRESULT CheckMediaType(const CMediaType* pmt);

    //  This just saves the allocator being used on the output pin
    //  Also called by input pin's GetAllocator()
    void SetAllocator(IMemAllocator * pAllocator);

    __out_opt IMemInputPin * ConnectedIMemInputPin()
        { return m_pInputPin; }

    // Allow the filter to see what allocator we have
    // N.B. This does NOT AddRef
    __out IMemAllocator * PeekAllocator() const
        {  return m_pAllocator; }

    HRESULT CompleteConnect(IPin *pReceivePin);

};  // CTransInPlaceOutputPin


class AM_NOVTABLE CTransInPlaceFilter : public CTransformFilter
{

public:

    // map getpin/getpincount for base enum of pins to owner
    // override this to return more specialised pin objects

    virtual CBasePin *GetPin(int n);

public:

    //  Set bModifiesData == false if your derived filter does
    //  not modify the data samples (for instance it's just copying
    //  them somewhere else or looking at the timestamps).

    CTransInPlaceFilter(__in_opt LPCTSTR, __inout_opt LPUNKNOWN, REFCLSID clsid, __inout HRESULT *,
                        bool bModifiesData = true);
#ifdef UNICODE
    CTransInPlaceFilter(__in_opt LPCSTR, __inout_opt LPUNKNOWN, REFCLSID clsid, __inout HRESULT *,
                        bool bModifiesData = true);
#endif
    // The following are defined to avoid undefined pure virtuals.
    // Even if they are never called, they will give linkage warnings/errors

    // We override EnumMediaTypes to bypass the transform class enumerator
    // which would otherwise call this.
    HRESULT GetMediaType(int iPosition, __inout CMediaType *pMediaType)
        {   DbgBreak("CTransInPlaceFilter::GetMediaType should never be called");
            return E_UNEXPECTED;
        }

    // This is called when we actually have to provide our own allocator.
    HRESULT DecideBufferSize(IMemAllocator*, __inout ALLOCATOR_PROPERTIES *);

    // The functions which call this in CTransform are overridden in this
    // class to call CheckInputType with the assumption that the type
    // does not change.  In Debug builds some calls will be made and
    // we just ensure that they do not assert.
    HRESULT CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut)
    {
        return S_OK;
    };


    // =================================================================
    // ----- You may want to override this -----------------------------
    // =================================================================

    HRESULT CompleteConnect(PIN_DIRECTION dir,IPin *pReceivePin);

    // chance to customize the transform process
    virtual HRESULT Receive(IMediaSample *pSample);

    // =================================================================
    // ----- You MUST override these -----------------------------------
    // =================================================================

    virtual HRESULT Transform(IMediaSample *pSample) PURE;

    // this goes in the factory template table to create new instances
    // static CCOMObject * CreateInstance(LPUNKNOWN, HRESULT *);


#ifdef PERF
    // Override to register performance measurement with a less generic string
    // You should do this to avoid confusion with other filters
    virtual void RegisterPerfId()
         {m_idTransInPlace = MSR_REGISTER(TEXT("TransInPlace"));}
#endif // PERF


// implementation details

protected:

    __out_opt IMediaSample * CTransInPlaceFilter::Copy(IMediaSample *pSource);

#ifdef PERF
    int m_idTransInPlace;                 // performance measuring id
#endif // PERF
    bool  m_bModifiesData;                // Does this filter change the data?

    // these hold our input and output pins

    friend class CTransInPlaceInputPin;
    friend class CTransInPlaceOutputPin;

    __out CTransInPlaceInputPin  *InputPin() const
    {
        return (CTransInPlaceInputPin *)m_pInput;
    };
    __out CTransInPlaceOutputPin *OutputPin() const
    {
        return (CTransInPlaceOutputPin *)m_pOutput;
    };

    //  Helper to see if the input and output types match
    BOOL TypesMatch()
    {
        return InputPin()->CurrentMediaType() ==
               OutputPin()->CurrentMediaType();
    }

    //  Are the input and output allocators different?
    BOOL UsingDifferentAllocators() const
    {
        return InputPin()->PeekAllocator() != OutputPin()->PeekAllocator();
    }
}; // CTransInPlaceFilter

#endif /* __TRANSIP__ */

//------------------------------------------------------------------------------
// File: OutputQ.h
//
// Desc: DirectShow base classes -  defines the COutputQueue class, which
//       makes a queue of samples and sends them to an output pin.  The 
//       class will optionally send the samples to the pin directly.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


typedef CGenericList<IMediaSample> CSampleList;

class COutputQueue : public CCritSec
{
public:
    //  Constructor
    COutputQueue(IPin      *pInputPin,          //  Pin to send stuff to
                 __inout HRESULT *phr,          //  'Return code'
                 BOOL       bAuto = TRUE,       //  Ask pin if blocks
                 BOOL       bQueue = TRUE,      //  Send through queue (ignored if
                                                //  bAuto set)
                 LONG       lBatchSize = 1,     //  Batch
                 BOOL       bBatchExact = FALSE,//  Batch exactly to BatchSize
                 LONG       lListSize =         //  Likely number in the list
                                DEFAULTCACHE,
                 DWORD      dwPriority =        //  Priority of thread to create
                                THREAD_PRIORITY_NORMAL,
                 bool       bFlushingOpt = false // flushing optimization
                );
    ~COutputQueue();

    // enter flush state - discard all data
    void BeginFlush();      // Begin flushing samples

    // re-enable receives (pass this downstream)
    void EndFlush();        // Complete flush of samples - downstream
                            // pin guaranteed not to block at this stage

    void EOS();             // Call this on End of stream

    void SendAnyway();      // Send batched samples anyway (if bBatchExact set)

    void NewSegment(
            REFERENCE_TIME tStart,
            REFERENCE_TIME tStop,
            double dRate);

    HRESULT Receive(IMediaSample *pSample);

    // do something with these media samples
    HRESULT ReceiveMultiple (
        __in_ecount(nSamples) IMediaSample **pSamples,
        long nSamples,
        __out long *nSamplesProcessed);

    void Reset();           // Reset m_hr ready for more data

    //  See if its idle or not
    BOOL IsIdle();

    // give the class an event to fire after everything removed from the queue
    void SetPopEvent(HANDLE hEvent);

protected:
    static DWORD WINAPI InitialThreadProc(__in LPVOID pv);
    DWORD ThreadProc();
    BOOL  IsQueued()
    {
        return m_List != NULL;
    };

    //  The critical section MUST be held when this is called
    void QueueSample(IMediaSample *pSample);

    BOOL IsSpecialSample(IMediaSample *pSample)
    {
        return (DWORD_PTR)pSample > (DWORD_PTR)(LONG_PTR)(-16);
    };

    //  Remove and Release() batched and queued samples
    void FreeSamples();

    //  Notify the thread there is something to do
    void NotifyThread();


protected:
    //  Queue 'messages'
    #define SEND_PACKET      ((IMediaSample *)(LONG_PTR)(-2))  // Send batch
    #define EOS_PACKET       ((IMediaSample *)(LONG_PTR)(-3))  // End of stream
    #define RESET_PACKET     ((IMediaSample *)(LONG_PTR)(-4))  // Reset m_hr
    #define NEW_SEGMENT      ((IMediaSample *)(LONG_PTR)(-5))  // send NewSegment

    // new segment packet is always followed by one of these
    struct NewSegmentPacket {
        REFERENCE_TIME tStart;
        REFERENCE_TIME tStop;
        double dRate;
    };

    // Remember input stuff
    IPin          * const m_pPin;
    IMemInputPin  *       m_pInputPin;
    BOOL            const m_bBatchExact;
    LONG            const m_lBatchSize;

    CSampleList   *       m_List;
    HANDLE                m_hSem;
    CAMEvent                m_evFlushComplete;
    HANDLE                m_hThread;
    __field_ecount_opt(m_lBatchSize) IMediaSample  **      m_ppSamples;
    __range(0, m_lBatchSize)         LONG                  m_nBatched;

    //  Wait optimization
    LONG                  m_lWaiting;
    //  Flush synchronization
    BOOL                  m_bFlushing;

    // flushing optimization. some downstream filters have trouble
    // with the queue's flushing optimization. other rely on it
    BOOL                  m_bFlushed;
    bool                  m_bFlushingOpt;

    //  Terminate now
    BOOL                  m_bTerminate;

    //  Send anyway flag for batching
    BOOL                  m_bSendAnyway;

    //  Deferred 'return code'
    HRESULT volatile         m_hr;

    // an event that can be fired after every deliver
    HANDLE m_hEventPop;
};

//------------------------------------------------------------------------------
// File: RenBase.h
//
// Desc: DirectShow base classes - defines a generic ActiveX base renderer
//       class.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __RENBASE__
#define __RENBASE__

// Forward class declarations

class CBaseRenderer;
class CBaseVideoRenderer;
class CRendererInputPin;

// This is our input pin class that channels calls to the renderer

class CRendererInputPin : public CBaseInputPin
{
protected:

    CBaseRenderer *m_pRenderer;

public:

    CRendererInputPin(__inout CBaseRenderer *pRenderer,
                      __inout HRESULT *phr,
                      __in_opt LPCWSTR Name);

    // Overriden from the base pin classes

    HRESULT BreakConnect();
    HRESULT CompleteConnect(IPin *pReceivePin);
    HRESULT SetMediaType(const CMediaType *pmt);
    HRESULT CheckMediaType(const CMediaType *pmt);
    HRESULT Active();
    HRESULT Inactive();

    // Add rendering behaviour to interface functions

    STDMETHODIMP QueryId(__deref_out LPWSTR *Id);
    STDMETHODIMP EndOfStream();
    STDMETHODIMP BeginFlush();
    STDMETHODIMP EndFlush();
    STDMETHODIMP Receive(IMediaSample *pMediaSample);

    // Helper
    IMemAllocator inline *Allocator() const
    {
        return m_pAllocator;
    }
};

// Main renderer class that handles synchronisation and state changes

class CBaseRenderer : public CBaseFilter
{
protected:

    friend class CRendererInputPin;

    friend void CALLBACK EndOfStreamTimer(UINT uID,      // Timer identifier
                                          UINT uMsg,     // Not currently used
                                          DWORD_PTR dwUser,  // User information
                                          DWORD_PTR dw1,     // Windows reserved
                                          DWORD_PTR dw2);    // Is also reserved

    CRendererPosPassThru *m_pPosition;  // Media seeking pass by object
    CAMEvent m_RenderEvent;             // Used to signal timer events
    CAMEvent m_ThreadSignal;            // Signalled to release worker thread
    CAMEvent m_evComplete;              // Signalled when state complete
    BOOL m_bAbort;                      // Stop us from rendering more data
    BOOL m_bStreaming;                  // Are we currently streaming
    DWORD_PTR m_dwAdvise;                   // Timer advise cookie
    IMediaSample *m_pMediaSample;       // Current image media sample
    BOOL m_bEOS;                        // Any more samples in the stream
    BOOL m_bEOSDelivered;               // Have we delivered an EC_COMPLETE
    CRendererInputPin *m_pInputPin;     // Our renderer input pin object
    CCritSec m_InterfaceLock;           // Critical section for interfaces
    CCritSec m_RendererLock;            // Controls access to internals
    IQualityControl * m_pQSink;         // QualityControl sink
    BOOL m_bRepaintStatus;              // Can we signal an EC_REPAINT
    //  Avoid some deadlocks by tracking filter during stop
    volatile BOOL  m_bInReceive;        // Inside Receive between PrepareReceive
                                        // And actually processing the sample
    REFERENCE_TIME m_SignalTime;        // Time when we signal EC_COMPLETE
    UINT m_EndOfStreamTimer;            // Used to signal end of stream
    CCritSec m_ObjectCreationLock;      // This lock protects the creation and
                                        // of m_pPosition and m_pInputPin.  It
                                        // ensures that two threads cannot create
                                        // either object simultaneously.

public:

    CBaseRenderer(REFCLSID RenderClass, // CLSID for this renderer
                  __in_opt LPCTSTR pName,         // Debug ONLY description
                  __inout_opt LPUNKNOWN pUnk,       // Aggregated owner object
                  __inout HRESULT *phr);        // General OLE return code

    ~CBaseRenderer();

    // Overriden to say what interfaces we support and where

    virtual HRESULT GetMediaPositionInterface(REFIID riid, __deref_out void **ppv);
    STDMETHODIMP NonDelegatingQueryInterface(REFIID, __deref_out void **);

    virtual HRESULT SourceThreadCanWait(BOOL bCanWait);

#ifdef DEBUG
    // Debug only dump of the renderer state
    void DisplayRendererState();
#endif
    virtual HRESULT WaitForRenderTime();
    virtual HRESULT CompleteStateChange(FILTER_STATE OldState);

    // Return internal information about this filter

    BOOL IsEndOfStream() { return m_bEOS; };
    BOOL IsEndOfStreamDelivered() { return m_bEOSDelivered; };
    BOOL IsStreaming() { return m_bStreaming; };
    void SetAbortSignal(BOOL bAbort) { m_bAbort = bAbort; };
    virtual void OnReceiveFirstSample(IMediaSample *pMediaSample) { };
    CAMEvent *GetRenderEvent() { return &m_RenderEvent; };

    // Permit access to the transition state

    void Ready() { m_evComplete.Set(); };
    void NotReady() { m_evComplete.Reset(); };
    BOOL CheckReady() { return m_evComplete.Check(); };

    virtual int GetPinCount();
    virtual CBasePin *GetPin(int n);
    FILTER_STATE GetRealState();
    void SendRepaint();
    void SendNotifyWindow(IPin *pPin,HWND hwnd);
    BOOL OnDisplayChange();
    void SetRepaintStatus(BOOL bRepaint);

    // Override the filter and pin interface functions

    STDMETHODIMP Stop();
    STDMETHODIMP Pause();
    STDMETHODIMP Run(REFERENCE_TIME StartTime);
    STDMETHODIMP GetState(DWORD dwMSecs, __out FILTER_STATE *State);
    STDMETHODIMP FindPin(LPCWSTR Id, __deref_out IPin **ppPin);

    // These are available for a quality management implementation

    virtual void OnRenderStart(IMediaSample *pMediaSample);
    virtual void OnRenderEnd(IMediaSample *pMediaSample);
    virtual HRESULT OnStartStreaming() { return NOERROR; };
    virtual HRESULT OnStopStreaming() { return NOERROR; };
    virtual void OnWaitStart() { };
    virtual void OnWaitEnd() { };
    virtual void PrepareRender() { };

#ifdef PERF
    REFERENCE_TIME m_trRenderStart; // Just before we started drawing
                                    // Set in OnRenderStart, Used in OnRenderEnd
    int m_idBaseStamp;              // MSR_id for frame time stamp
    int m_idBaseRenderTime;         // MSR_id for true wait time
    int m_idBaseAccuracy;           // MSR_id for time frame is late (int)
#endif

    // Quality management implementation for scheduling rendering

    virtual BOOL ScheduleSample(IMediaSample *pMediaSample);
    virtual HRESULT GetSampleTimes(IMediaSample *pMediaSample,
                                   __out REFERENCE_TIME *pStartTime,
                                   __out REFERENCE_TIME *pEndTime);

    virtual HRESULT ShouldDrawSampleNow(IMediaSample *pMediaSample,
                                        __out REFERENCE_TIME *ptrStart,
                                        __out REFERENCE_TIME *ptrEnd);

    // Lots of end of stream complexities

    void TimerCallback();
    void ResetEndOfStreamTimer();
    HRESULT NotifyEndOfStream();
    virtual HRESULT SendEndOfStream();
    virtual HRESULT ResetEndOfStream();
    virtual HRESULT EndOfStream();

    // Rendering is based around the clock

    void SignalTimerFired();
    virtual HRESULT CancelNotification();
    virtual HRESULT ClearPendingSample();

    // Called when the filter changes state

    virtual HRESULT Active();
    virtual HRESULT Inactive();
    virtual HRESULT StartStreaming();
    virtual HRESULT StopStreaming();
    virtual HRESULT BeginFlush();
    virtual HRESULT EndFlush();

    // Deal with connections and type changes

    virtual HRESULT BreakConnect();
    virtual HRESULT SetMediaType(const CMediaType *pmt);
    virtual HRESULT CompleteConnect(IPin *pReceivePin);

    // These look after the handling of data samples

    virtual HRESULT PrepareReceive(IMediaSample *pMediaSample);
    virtual HRESULT Receive(IMediaSample *pMediaSample);
    virtual BOOL HaveCurrentSample();
    virtual IMediaSample *GetCurrentSample();
    virtual HRESULT Render(IMediaSample *pMediaSample);

    // Derived classes MUST override these
    virtual HRESULT DoRenderSample(IMediaSample *pMediaSample) PURE;
    virtual HRESULT CheckMediaType(const CMediaType *) PURE;

    // Helper
    void WaitForReceiveToComplete();
};


// CBaseVideoRenderer is a renderer class (see its ancestor class) and
// it handles scheduling of media samples so that they are drawn at the
// correct time by the reference clock.  It implements a degradation
// strategy.  Possible degradation modes are:
//    Drop frames here (only useful if the drawing takes significant time)
//    Signal supplier (upstream) to drop some frame(s) - i.e. one-off skip.
//    Signal supplier to change the frame rate - i.e. ongoing skipping.
//    Or any combination of the above.
// In order to determine what's useful to try we need to know what's going
// on.  This is done by timing various operations (including the supplier).
// This timing is done by using timeGetTime as it is accurate enough and
// usually cheaper than calling the reference clock.  It also tells the
// truth if there is an audio break and the reference clock stops.
// We provide a number of public entry points (named OnXxxStart, OnXxxEnd)
// which the rest of the renderer calls at significant moments.  These do
// the timing.

// the number of frames that the sliding averages are averaged over.
// the rule is (1024*NewObservation + (AVGPERIOD-1) * PreviousAverage)/AVGPERIOD
#define AVGPERIOD 4
#define DO_MOVING_AVG(avg,obs) (avg = (1024*obs + (AVGPERIOD-1)*avg)/AVGPERIOD)
// Spot the bug in this macro - I can't. but it doesn't work!

class CBaseVideoRenderer : public CBaseRenderer,    // Base renderer class
                           public IQualProp,        // Property page guff
                           public IQualityControl   // Allow throttling
{
protected:

    // Hungarian:
    //     tFoo is the time Foo in mSec (beware m_tStart from filter.h)
    //     trBar is the time Bar by the reference clock

    //******************************************************************
    // State variables to control synchronisation
    //******************************************************************

    // Control of sending Quality messages.  We need to know whether
    // we are in trouble (e.g. frames being dropped) and where the time
    // is being spent.

    // When we drop a frame we play the next one early.
    // The frame after that is likely to wait before drawing and counting this
    // wait as spare time is unfair, so we count it as a zero wait.
    // We therefore need to know whether we are playing frames early or not.

    int m_nNormal;                  // The number of consecutive frames
                                    // drawn at their normal time (not early)
                                    // -1 means we just dropped a frame.

#ifdef PERF
    BOOL m_bDrawLateFrames;         // Don't drop any frames (debug and I'm
                                    // not keen on people using it!)
#endif

    BOOL m_bSupplierHandlingQuality;// The response to Quality messages says
                                    // our supplier is handling things.
                                    // We will allow things to go extra late
                                    // before dropping frames.  We will play
                                    // very early after he has dropped one.

    // Control of scheduling, frame dropping etc.
    // We need to know where the time is being spent so as to tell whether
    // we should be taking action here, signalling supplier or what.
    // The variables are initialised to a mode of NOT dropping frames.
    // They will tell the truth after a few frames.
    // We typically record a start time for an event, later we get the time
    // again and subtract to get the elapsed time, and we average this over
    // a few frames.  The average is used to tell what mode we are in.

    // Although these are reference times (64 bit) they are all DIFFERENCES
    // between times which are small.  An int will go up to 214 secs before
    // overflow.  Avoiding 64 bit multiplications and divisions seems
    // worth while.



    // Audio-video throttling.  If the user has turned up audio quality
    // very high (in principle it could be any other stream, not just audio)
    // then we can receive cries for help via the graph manager.  In this case
    // we put in a wait for some time after rendering each frame.
    int m_trThrottle;

    // The time taken to render (i.e. BitBlt) frames controls which component
    // needs to degrade.  If the blt is expensive, the renderer degrades.
    // If the blt is cheap it's done anyway and the supplier degrades.
    int m_trRenderAvg;              // Time frames are taking to blt
    int m_trRenderLast;             // Time for last frame blt
    int m_tRenderStart;             // Just before we started drawing (mSec)
                                    // derived from timeGetTime.

    // When frames are dropped we will play the next frame as early as we can.
    // If it was a false alarm and the machine is fast we slide gently back to
    // normal timing.  To do this, we record the offset showing just how early
    // we really are.  This will normally be negative meaning early or zero.
    int m_trEarliness;

    // Target provides slow long-term feedback to try to reduce the
    // average sync offset to zero.  Whenever a frame is actually rendered
    // early we add a msec or two, whenever late we take off a few.
    // We add or take off 1/32 of the error time.
    // Eventually we should be hovering around zero.  For a really bad case
    // where we were (say) 300mSec off, it might take 100 odd frames to
    // settle down.  The rate of change of this is intended to be slower
    // than any other mechanism in Quartz, thereby avoiding hunting.
    int m_trTarget;

    // The proportion of time spent waiting for the right moment to blt
    // controls whether we bother to drop a frame or whether we reckon that
    // we're doing well enough that we can stand a one-frame glitch.
    int m_trWaitAvg;                // Average of last few wait times
                                    // (actually we just average how early
                                    // we were).  Negative here means LATE.

    // The average inter-frame time.
    // This is used to calculate the proportion of the time used by the
    // three operations (supplying us, waiting, rendering)
    int m_trFrameAvg;               // Average inter-frame time
    int m_trDuration;               // duration of last frame.

#ifdef PERF
    // Performance logging identifiers
    int m_idTimeStamp;              // MSR_id for frame time stamp
    int m_idEarliness;              // MSR_id for earliness fudge
    int m_idTarget;                 // MSR_id for Target fudge
    int m_idWaitReal;               // MSR_id for true wait time
    int m_idWait;                   // MSR_id for wait time recorded
    int m_idFrameAccuracy;          // MSR_id for time frame is late (int)
    int m_idRenderAvg;              // MSR_id for Render time recorded (int)
    int m_idSchLateTime;            // MSR_id for lateness at scheduler
    int m_idQualityRate;            // MSR_id for Quality rate requested
    int m_idQualityTime;            // MSR_id for Quality time requested
    int m_idDecision;               // MSR_id for decision code
    int m_idDuration;               // MSR_id for duration of a frame
    int m_idThrottle;               // MSR_id for audio-video throttling
    //int m_idDebug;                  // MSR_id for trace style debugging
    //int m_idSendQuality;          // MSR_id for timing the notifications per se
#endif // PERF
    REFERENCE_TIME m_trRememberStampForPerf;  // original time stamp of frame
                                              // with no earliness fudges etc.
#ifdef PERF
    REFERENCE_TIME m_trRememberFrameForPerf;  // time when previous frame rendered

    // debug...
    int m_idFrameAvg;
    int m_idWaitAvg;
#endif

    // PROPERTY PAGE
    // This has edit fields that show the user what's happening
    // These member variables hold these counts.

    int m_cFramesDropped;           // cumulative frames dropped IN THE RENDERER
    int m_cFramesDrawn;             // Frames since streaming started seen BY THE
                                    // RENDERER (some may be dropped upstream)

    // Next two support average sync offset and standard deviation of sync offset.
    LONGLONG m_iTotAcc;                  // Sum of accuracies in mSec
    LONGLONG m_iSumSqAcc;           // Sum of squares of (accuracies in mSec)

    // Next two allow jitter calculation.  Jitter is std deviation of frame time.
    REFERENCE_TIME m_trLastDraw;    // Time of prev frame (for inter-frame times)
    LONGLONG m_iSumSqFrameTime;     // Sum of squares of (inter-frame time in mSec)
    LONGLONG m_iSumFrameTime;            // Sum of inter-frame times in mSec

    // To get performance statistics on frame rate, jitter etc, we need
    // to record the lateness and inter-frame time.  What we actually need are the
    // data above (sum, sum of squares and number of entries for each) but the data
    // is generated just ahead of time and only later do we discover whether the
    // frame was actually drawn or not.  So we have to hang on to the data
    int m_trLate;                   // hold onto frame lateness
    int m_trFrame;                  // hold onto inter-frame time

    int m_tStreamingStart;          // if streaming then time streaming started
                                    // else time of last streaming session
                                    // used for property page statistics
#ifdef PERF
    LONGLONG m_llTimeOffset;        // timeGetTime()*10000+m_llTimeOffset==ref time
#endif

public:


    CBaseVideoRenderer(REFCLSID RenderClass, // CLSID for this renderer
                       __in_opt LPCTSTR pName,         // Debug ONLY description
                       __inout_opt LPUNKNOWN pUnk,       // Aggregated owner object
                       __inout HRESULT *phr);        // General OLE return code

    ~CBaseVideoRenderer();

    // IQualityControl methods - Notify allows audio-video throttling

    STDMETHODIMP SetSink( IQualityControl * piqc);
    STDMETHODIMP Notify( IBaseFilter * pSelf, Quality q);

    // These provide a full video quality management implementation

    void OnRenderStart(IMediaSample *pMediaSample);
    void OnRenderEnd(IMediaSample *pMediaSample);
    void OnWaitStart();
    void OnWaitEnd();
    HRESULT OnStartStreaming();
    HRESULT OnStopStreaming();
    void ThrottleWait();

    // Handle the statistics gathering for our quality management

    void PreparePerformanceData(int trLate, int trFrame);
    virtual void RecordFrameLateness(int trLate, int trFrame);
    virtual void OnDirectRender(IMediaSample *pMediaSample);
    virtual HRESULT ResetStreamingTimes();
    BOOL ScheduleSample(IMediaSample *pMediaSample);
    HRESULT ShouldDrawSampleNow(IMediaSample *pMediaSample,
                                __inout REFERENCE_TIME *ptrStart,
                                __inout REFERENCE_TIME *ptrEnd);

    virtual HRESULT SendQuality(REFERENCE_TIME trLate, REFERENCE_TIME trRealStream);
    STDMETHODIMP JoinFilterGraph(__inout_opt IFilterGraph * pGraph, __in_opt LPCWSTR pName);

    //
    //  Do estimates for standard deviations for per-frame
    //  statistics
    //
    //  *piResult = (llSumSq - iTot * iTot / m_cFramesDrawn - 1) /
    //                            (m_cFramesDrawn - 2)
    //  or 0 if m_cFramesDrawn <= 3
    //
    HRESULT GetStdDev(
        int nSamples,
        __out int *piResult,
        LONGLONG llSumSq,
        LONGLONG iTot
    );
public:

    // IQualProp property page support

    STDMETHODIMP get_FramesDroppedInRenderer(__out int *cFramesDropped);
    STDMETHODIMP get_FramesDrawn(__out int *pcFramesDrawn);
    STDMETHODIMP get_AvgFrameRate(__out int *piAvgFrameRate);
    STDMETHODIMP get_Jitter(__out int *piJitter);
    STDMETHODIMP get_AvgSyncOffset(__out int *piAvg);
    STDMETHODIMP get_DevSyncOffset(__out int *piDev);

    // Implement an IUnknown interface and expose IQualProp

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid,__deref_out VOID **ppv);
};

#endif // __RENBASE__

//------------------------------------------------------------------------------
// File: WinUtil.h
//
// Desc: DirectShow base classes - defines generic handler classes.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


// Make sure that you call PrepareWindow to initialise the window after
// the object has been constructed. It is a separate method so that
// derived classes can override useful methods like MessageLoop. Also
// any derived class must call DoneWithWindow in its destructor. If it
// doesn't a message may be retrieved and call a derived class member
// function while a thread is executing the base class destructor code

#ifndef __WINUTIL__
#define __WINUTIL__

const int DEFWIDTH = 320;                    // Initial window width
const int DEFHEIGHT = 240;                   // Initial window height
const int CAPTION = 256;                     // Maximum length of caption
const int TIMELENGTH = 50;                   // Maximum length of times
const int PROFILESTR = 128;                  // Normal profile string
const WORD PALVERSION = 0x300;               // GDI palette version
const LONG PALETTE_VERSION = (LONG) 1;       // Initial palette version
const COLORREF VIDEO_COLOUR = 0;             // Defaults to black background
const HANDLE hMEMORY = (HANDLE) (-1);        // Says to open as memory file

#define WIDTH(x) ((*(x)).right - (*(x)).left)
#define HEIGHT(x) ((*(x)).bottom - (*(x)).top)
#define SHOWSTAGE TEXT("WM_SHOWSTAGE")
#define SHOWSTAGETOP TEXT("WM_SHOWSTAGETOP")
#define REALIZEPALETTE TEXT("WM_REALIZEPALETTE")

class AM_NOVTABLE CBaseWindow
{
protected:

    HINSTANCE m_hInstance;          // Global module instance handle
    HWND m_hwnd;                    // Handle for our window
    HDC m_hdc;                      // Device context for the window
    LONG m_Width;                   // Client window width
    LONG m_Height;                  // Client window height
    BOOL m_bActivated;              // Has the window been activated
    LPTSTR m_pClassName;            // Static string holding class name
    DWORD m_ClassStyles;            // Passed in to our constructor
    DWORD m_WindowStyles;           // Likewise the initial window styles
    DWORD m_WindowStylesEx;         // And the extended window styles
    UINT m_ShowStageMessage;        // Have the window shown with focus
    UINT m_ShowStageTop;            // Makes the window WS_EX_TOPMOST
    UINT m_RealizePalette;          // Makes us realize our new palette
    HDC m_MemoryDC;                 // Used for fast BitBlt operations
    HPALETTE m_hPalette;            // Handle to any palette we may have
    BYTE m_bNoRealize;              // Don't realize palette now
    BYTE m_bBackground;             // Should we realise in background
    BYTE m_bRealizing;              // already realizing the palette
    CCritSec m_WindowLock;          // Serialise window object access
    BOOL m_bDoGetDC;                // Should this window get a DC
    bool m_bDoPostToDestroy;        // Use PostMessage to destroy
    CCritSec m_PaletteLock;         // This lock protects m_hPalette.
                                    // It should be held anytime the
                                    // program use the value of m_hPalette.

    // Maps windows message procedure into C++ methods
    friend LRESULT CALLBACK WndProc(HWND hwnd,      // Window handle
                                    UINT uMsg,      // Message ID
                                    WPARAM wParam,  // First parameter
                                    LPARAM lParam); // Other parameter

    virtual LRESULT OnPaletteChange(HWND hwnd, UINT Message);

public:

    CBaseWindow(BOOL bDoGetDC = TRUE, bool bPostToDestroy = false);

#ifdef DEBUG
    virtual ~CBaseWindow();
#endif

    virtual HRESULT DoneWithWindow();
    virtual HRESULT PrepareWindow();
    virtual HRESULT InactivateWindow();
    virtual HRESULT ActivateWindow();
    virtual BOOL OnSize(LONG Width, LONG Height);
    virtual BOOL OnClose();
    virtual RECT GetDefaultRect();
    virtual HRESULT UninitialiseWindow();
    virtual HRESULT InitialiseWindow(HWND hwnd);

    HRESULT CompleteConnect();
    HRESULT DoCreateWindow();

    HRESULT PerformanceAlignWindow();
    HRESULT DoShowWindow(LONG ShowCmd);
    void PaintWindow(BOOL bErase);
    void DoSetWindowForeground(BOOL bFocus);
    virtual HRESULT SetPalette(HPALETTE hPalette);
    void SetRealize(BOOL bRealize)
    {
        m_bNoRealize = !bRealize;
    }

    //  Jump over to the window thread to set the current palette
    HRESULT SetPalette();
    void UnsetPalette(void);
    virtual HRESULT DoRealisePalette(BOOL bForceBackground = FALSE);

    void LockPaletteLock();
    void UnlockPaletteLock();

    virtual BOOL PossiblyEatMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	    { return FALSE; };

    // Access our window information

    bool WindowExists();
    LONG GetWindowWidth();
    LONG GetWindowHeight();
    HWND GetWindowHWND();
    HDC GetMemoryHDC();
    HDC GetWindowHDC();

    #ifdef DEBUG
    HPALETTE GetPalette();
    #endif // DEBUG

    // This is the window procedure the derived object should override

    virtual LRESULT OnReceiveMessage(HWND hwnd,          // Window handle
                                     UINT uMsg,          // Message ID
                                     WPARAM wParam,      // First parameter
                                     LPARAM lParam);     // Other parameter

    // Must be overriden to return class and window styles

    virtual LPTSTR GetClassWindowStyles(
                            __out DWORD *pClassStyles,          // Class styles
                            __out DWORD *pWindowStyles,         // Window styles
                            __out DWORD *pWindowStylesEx) PURE; // Extended styles
};


// This helper class is entirely subservient to the owning CBaseWindow object
// All this object does is to split out the actual drawing operation from the
// main object (because it was becoming too large). We have a number of entry
// points to set things like the draw device contexts, to implement the actual
// drawing and to set the destination rectangle in the client window. We have
// no critical section locking in this class because we are used exclusively
// by the owning window object which looks after serialising calls into us

// If you want to use this class make sure you call NotifyAllocator once the
// allocate has been agreed, also call NotifyMediaType with a pointer to a
// NON stack based CMediaType once that has been set (we keep a pointer to
// the original rather than taking a copy). When the palette changes call
// IncrementPaletteVersion (easiest thing to do is to also call this method
// in the SetMediaType method most filters implement). Finally before you
// start rendering anything call SetDrawContext so that we can get the HDCs
// for drawing from the CBaseWindow object we are given during construction

class CDrawImage
{
protected:

    CBaseWindow *m_pBaseWindow;     // Owning video window object
    CRefTime m_StartSample;         // Start time for the current sample
    CRefTime m_EndSample;           // And likewise it's end sample time
    HDC m_hdc;                      // Main window device context
    HDC m_MemoryDC;                 // Offscreen draw device context
    RECT m_TargetRect;              // Target destination rectangle
    RECT m_SourceRect;              // Source image rectangle
    BOOL m_bStretch;                // Do we have to stretch the images
    BOOL m_bUsingImageAllocator;    // Are the samples shared DIBSECTIONs
    CMediaType *m_pMediaType;       // Pointer to the current format
    int m_perfidRenderTime;         // Time taken to render an image
    LONG m_PaletteVersion;          // Current palette version cookie

    // Draw the video images in the window

    void SlowRender(IMediaSample *pMediaSample);
    void FastRender(IMediaSample *pMediaSample);
    void DisplaySampleTimes(IMediaSample *pSample);
    void UpdateColourTable(HDC hdc,__in BITMAPINFOHEADER *pbmi);
    void SetStretchMode();

public:

    // Used to control the image drawing

    CDrawImage(__inout CBaseWindow *pBaseWindow);
    BOOL DrawImage(IMediaSample *pMediaSample);
    BOOL DrawVideoImageHere(HDC hdc, IMediaSample *pMediaSample,
                            __in LPRECT lprcSrc, __in LPRECT lprcDst);
    void SetDrawContext();
    void SetTargetRect(__in RECT *pTargetRect);
    void SetSourceRect(__in RECT *pSourceRect);
    void GetTargetRect(__out RECT *pTargetRect);
    void GetSourceRect(__out RECT *pSourceRect);
    virtual RECT ScaleSourceRect(const RECT *pSource);

    // Handle updating palettes as they change

    LONG GetPaletteVersion();
    void ResetPaletteVersion();
    void IncrementPaletteVersion();

    // Tell us media types and allocator assignments

    void NotifyAllocator(BOOL bUsingImageAllocator);
    void NotifyMediaType(__in CMediaType *pMediaType);
    BOOL UsingImageAllocator();

    // Called when we are about to draw an image

    void NotifyStartDraw() {
        MSR_START(m_perfidRenderTime);
    };

    // Called when we complete an image rendering

    void NotifyEndDraw() {
        MSR_STOP(m_perfidRenderTime);
    };
};


// This is the structure used to keep information about each GDI DIB. All the
// samples we create from our allocator will have a DIBSECTION allocated to
// them. When we receive the sample we know we can BitBlt straight to an HDC

typedef struct tagDIBDATA {

    LONG        PaletteVersion;     // Current palette version in use
    DIBSECTION  DibSection;         // Details of DIB section allocated
    HBITMAP     hBitmap;            // Handle to bitmap for drawing
    HANDLE      hMapping;           // Handle to shared memory block
    BYTE        *pBase;             // Pointer to base memory address

} DIBDATA;


// This class inherits from CMediaSample and uses all of it's methods but it
// overrides the constructor to initialise itself with the DIBDATA structure
// When we come to render an IMediaSample we will know if we are using our own
// allocator, and if we are, we can cast the IMediaSample to a pointer to one
// of these are retrieve the DIB section information and hence the HBITMAP

class CImageSample : public CMediaSample
{
protected:

    DIBDATA m_DibData;      // Information about the DIBSECTION
    BOOL m_bInit;           // Is the DIB information setup

public:

    // Constructor

    CImageSample(__inout CBaseAllocator *pAllocator,
                 __in_opt LPCTSTR pName,
                 __inout HRESULT *phr,
                 __in_bcount(length) LPBYTE pBuffer,
                 LONG length);

    // Maintain the DIB/DirectDraw state

    void SetDIBData(__in DIBDATA *pDibData);
    __out DIBDATA *GetDIBData();
};


// This is an allocator based on the abstract CBaseAllocator base class that
// allocates sample buffers in shared memory. The number and size of these
// are determined when the output pin calls Prepare on us. The shared memory
// blocks are used in subsequent calls to GDI CreateDIBSection, once that
// has been done the output pin can fill the buffers with data which will
// then be handed to GDI through BitBlt calls and thereby remove one copy

class CImageAllocator : public CBaseAllocator
{
protected:

    CBaseFilter *m_pFilter;   // Delegate reference counts to
    CMediaType *m_pMediaType;           // Pointer to the current format

    // Used to create and delete samples

    HRESULT Alloc();
    void Free();

    // Manage the shared DIBSECTION and DCI/DirectDraw buffers

    HRESULT CreateDIB(LONG InSize,DIBDATA &DibData);
    STDMETHODIMP CheckSizes(__in ALLOCATOR_PROPERTIES *pRequest);
    virtual CImageSample *CreateImageSample(__in_bcount(Length) LPBYTE pData,LONG Length);

public:

    // Constructor and destructor

    CImageAllocator(__inout CBaseFilter *pFilter,__in_opt LPCTSTR pName,__inout HRESULT *phr);
#ifdef DEBUG
    ~CImageAllocator();
#endif

    STDMETHODIMP_(ULONG) NonDelegatingAddRef();
    STDMETHODIMP_(ULONG) NonDelegatingRelease();
    void NotifyMediaType(__in CMediaType *pMediaType);

    // Agree the number of buffers to be used and their size

    STDMETHODIMP SetProperties(
        __in ALLOCATOR_PROPERTIES *pRequest,
        __out ALLOCATOR_PROPERTIES *pActual);
};


// This class is a fairly specialised helper class for image renderers that
// have to create and manage palettes. The CBaseWindow class looks after
// realising palettes once they have been installed. This class can be used
// to create the palette handles from a media format (which must contain a
// VIDEOINFO structure in the format block). We try to make the palette an
// identity palette to maximise performance and also only change palettes
// if actually required to (we compare palette colours before updating).
// All the methods are virtual so that they can be overriden if so required

class CImagePalette
{
protected:

    CBaseWindow *m_pBaseWindow;             // Window to realise palette in
    CBaseFilter *m_pFilter;                 // Media filter to send events
    CDrawImage *m_pDrawImage;               // Object who will be drawing
    HPALETTE m_hPalette;                    // The palette handle we own

public:

    CImagePalette(__inout CBaseFilter *pBaseFilter,
                  __inout CBaseWindow *pBaseWindow,
                  __inout CDrawImage *pDrawImage);

#ifdef DEBUG
    virtual ~CImagePalette();
#endif

    static HPALETTE MakePalette(const VIDEOINFOHEADER *pVideoInfo, __in LPSTR szDevice);
    HRESULT RemovePalette();
    static HRESULT MakeIdentityPalette(__inout_ecount_full(iColours) PALETTEENTRY *pEntry,INT iColours, __in LPSTR szDevice);
    HRESULT CopyPalette(const CMediaType *pSrc,__out CMediaType *pDest);
    BOOL ShouldUpdate(const VIDEOINFOHEADER *pNewInfo,const VIDEOINFOHEADER *pOldInfo);
    HRESULT PreparePalette(const CMediaType *pmtNew,const CMediaType *pmtOld,__in LPSTR szDevice);

    BOOL DrawVideoImageHere(HDC hdc, IMediaSample *pMediaSample, __in LPRECT lprcSrc, __in LPRECT lprcDst)
    {
        return m_pDrawImage->DrawVideoImageHere(hdc, pMediaSample, lprcSrc,lprcDst);
    }
};


// Another helper class really for video based renderers. Most such renderers
// need to know what the display format is to some degree or another. This
// class initialises itself with the display format. The format can be asked
// for through GetDisplayFormat and various other accessor functions. If a
// filter detects a display format change (perhaps it gets a WM_DEVMODECHANGE
// message then it can call RefreshDisplayType to reset that format). Also
// many video renderers will want to check formats as they are proposed by
// source filters. This class provides methods to check formats and only
// accept those video formats that can be efficiently drawn using GDI calls

class CImageDisplay : public CCritSec
{
protected:

    // This holds the display format; biSize should not be too big, so we can
    // safely use the VIDEOINFO structure
    VIDEOINFO m_Display;

    static DWORD CountSetBits(const DWORD Field);
    static DWORD CountPrefixBits(const DWORD Field);
    static BOOL CheckBitFields(const VIDEOINFO *pInput);

public:

    // Constructor and destructor

    CImageDisplay();

    // Used to manage BITMAPINFOHEADERs and the display format

    const VIDEOINFO *GetDisplayFormat();
    HRESULT RefreshDisplayType(__in_opt LPSTR szDeviceName);
    static BOOL CheckHeaderValidity(const VIDEOINFO *pInput);
    static BOOL CheckPaletteHeader(const VIDEOINFO *pInput);
    BOOL IsPalettised();
    WORD GetDisplayDepth();

    // Provide simple video format type checking

    HRESULT CheckMediaType(const CMediaType *pmtIn);
    HRESULT CheckVideoType(const VIDEOINFO *pInput);
    HRESULT UpdateFormat(__inout VIDEOINFO *pVideoInfo);
    const DWORD *GetBitMasks(const VIDEOINFO *pVideoInfo);

    BOOL GetColourMask(__out DWORD *pMaskRed,
                       __out DWORD *pMaskGreen,
                       __out DWORD *pMaskBlue);
};

//  Convert a FORMAT_VideoInfo to FORMAT_VideoInfo2
STDAPI ConvertVideoInfoToVideoInfo2(__inout AM_MEDIA_TYPE *pmt);

//  Check a media type containing VIDEOINFOHEADER
STDAPI CheckVideoInfoType(const AM_MEDIA_TYPE *pmt);

//  Check a media type containing VIDEOINFOHEADER
STDAPI CheckVideoInfo2Type(const AM_MEDIA_TYPE *pmt);

#endif // __WINUTIL__

//------------------------------------------------------------------------------
// File: WinCtrl.h
//
// Desc: DirectShow base classes - defines classes for video control 
//       interfaces.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __WINCTRL__
#define __WINCTRL__

#define ABSOL(x) (x < 0 ? -x : x)
#define NEGAT(x) (x > 0 ? -x : x)

//  Helper
BOOL WINAPI PossiblyEatMessage(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class CBaseControlWindow : public CBaseVideoWindow, public CBaseWindow
{
protected:

    CBaseFilter *m_pFilter;            // Pointer to owning media filter
    CBasePin *m_pPin;                  // Controls media types for connection
    CCritSec *m_pInterfaceLock;        // Externally defined critical section
    COLORREF m_BorderColour;           // Current window border colour
    BOOL m_bAutoShow;                  // What happens when the state changes
    HWND m_hwndOwner;                  // Owner window that we optionally have
    HWND m_hwndDrain;                  // HWND to post any messages received
    BOOL m_bCursorHidden;              // Should we hide the window cursor

public:

    // Internal methods for other objects to get information out

    HRESULT DoSetWindowStyle(long Style,long WindowLong);
    HRESULT DoGetWindowStyle(__out long *pStyle,long WindowLong);
    BOOL IsAutoShowEnabled() { return m_bAutoShow; };
    COLORREF GetBorderColour() { return m_BorderColour; };
    HWND GetOwnerWindow() { return m_hwndOwner; };
    BOOL IsCursorHidden() { return m_bCursorHidden; };

    inline BOOL PossiblyEatMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        return ::PossiblyEatMessage(m_hwndDrain, uMsg, wParam, lParam);
    }

    // Derived classes must call this to set the pin the filter is using
    // We don't have the pin passed in to the constructor (as we do with
    // the CBaseFilter object) because filters typically create the
    // pins dynamically when requested in CBaseFilter::GetPin. This can
    // not be called from our constructor because is is a virtual method

    void SetControlWindowPin(CBasePin *pPin) {
        m_pPin = pPin;
    }

public:

    CBaseControlWindow(__inout CBaseFilter *pFilter,   // Owning media filter
                       __in CCritSec *pInterfaceLock,  // Locking object
                       __in_opt LPCTSTR pName,         // Object description
                       __inout_opt LPUNKNOWN pUnk,     // Normal COM ownership
                       __inout HRESULT *phr);          // OLE return code

    // These are the properties we support

    STDMETHODIMP put_Caption(__in BSTR strCaption);
    STDMETHODIMP get_Caption(__out BSTR *pstrCaption);
    STDMETHODIMP put_AutoShow(long AutoShow);
    STDMETHODIMP get_AutoShow(__out long *AutoShow);
    STDMETHODIMP put_WindowStyle(long WindowStyle);
    STDMETHODIMP get_WindowStyle(__out long *pWindowStyle);
    STDMETHODIMP put_WindowStyleEx(long WindowStyleEx);
    STDMETHODIMP get_WindowStyleEx(__out long *pWindowStyleEx);
    STDMETHODIMP put_WindowState(long WindowState);
    STDMETHODIMP get_WindowState(__out long *pWindowState);
    STDMETHODIMP put_BackgroundPalette(long BackgroundPalette);
    STDMETHODIMP get_BackgroundPalette(__out long *pBackgroundPalette);
    STDMETHODIMP put_Visible(long Visible);
    STDMETHODIMP get_Visible(__out long *pVisible);
    STDMETHODIMP put_Left(long Left);
    STDMETHODIMP get_Left(__out long *pLeft);
    STDMETHODIMP put_Width(long Width);
    STDMETHODIMP get_Width(__out long *pWidth);
    STDMETHODIMP put_Top(long Top);
    STDMETHODIMP get_Top(__out long *pTop);
    STDMETHODIMP put_Height(long Height);
    STDMETHODIMP get_Height(__out long *pHeight);
    STDMETHODIMP put_Owner(OAHWND Owner);
    STDMETHODIMP get_Owner(__out OAHWND *Owner);
    STDMETHODIMP put_MessageDrain(OAHWND Drain);
    STDMETHODIMP get_MessageDrain(__out OAHWND *Drain);
    STDMETHODIMP get_BorderColor(__out long *Color);
    STDMETHODIMP put_BorderColor(long Color);
    STDMETHODIMP get_FullScreenMode(__out long *FullScreenMode);
    STDMETHODIMP put_FullScreenMode(long FullScreenMode);

    // And these are the methods

    STDMETHODIMP SetWindowForeground(long Focus);
    STDMETHODIMP NotifyOwnerMessage(OAHWND hwnd,long uMsg,LONG_PTR wParam,LONG_PTR lParam);
    STDMETHODIMP GetMinIdealImageSize(__out long *pWidth,__out long *pHeight);
    STDMETHODIMP GetMaxIdealImageSize(__out long *pWidth,__out long *pHeight);
    STDMETHODIMP SetWindowPosition(long Left,long Top,long Width,long Height);
    STDMETHODIMP GetWindowPosition(__out long *pLeft,__out long *pTop,__out long *pWidth,__out long *pHeight);
    STDMETHODIMP GetRestorePosition(__out long *pLeft,__out long *pTop,__out long *pWidth,__out long *pHeight);
	STDMETHODIMP HideCursor(long HideCursor);
    STDMETHODIMP IsCursorHidden(__out long *CursorHidden);
};

// This class implements the IBasicVideo interface

class CBaseControlVideo : public CBaseBasicVideo
{
protected:

    CBaseFilter *m_pFilter;   // Pointer to owning media filter
    CBasePin *m_pPin;                   // Controls media types for connection
    CCritSec *m_pInterfaceLock;         // Externally defined critical section

public:

    // Derived classes must provide these for the implementation

    virtual HRESULT IsDefaultTargetRect() PURE;
    virtual HRESULT SetDefaultTargetRect() PURE;
    virtual HRESULT SetTargetRect(RECT *pTargetRect) PURE;
    virtual HRESULT GetTargetRect(RECT *pTargetRect) PURE;
    virtual HRESULT IsDefaultSourceRect() PURE;
    virtual HRESULT SetDefaultSourceRect() PURE;
    virtual HRESULT SetSourceRect(RECT *pSourceRect) PURE;
    virtual HRESULT GetSourceRect(RECT *pSourceRect) PURE;
    virtual HRESULT GetStaticImage(__inout long *pBufferSize,__out_bcount_part(*pBufferSize, *pBufferSize) long *pDIBImage) PURE;

    // Derived classes must override this to return a VIDEOINFO representing
    // the video format. We cannot call IPin ConnectionMediaType to get this
    // format because various filters dynamically change the type when using
    // DirectDraw such that the format shows the position of the logical
    // bitmap in a frame buffer surface, so the size might be returned as
    // 1024x768 pixels instead of 320x240 which is the real video dimensions

    __out virtual VIDEOINFOHEADER *GetVideoFormat() PURE;

    // Helper functions for creating memory renderings of a DIB image

    HRESULT GetImageSize(__in VIDEOINFOHEADER *pVideoInfo,
                         __out LONG *pBufferSize,
                         __in RECT *pSourceRect);

    HRESULT CopyImage(IMediaSample *pMediaSample,
                      __in VIDEOINFOHEADER *pVideoInfo,
                      __inout LONG *pBufferSize,
                      __out_bcount_part(*pBufferSize, *pBufferSize) BYTE *pVideoImage,
                      __in RECT *pSourceRect);

    // Override this if you want notifying when the rectangles change
    virtual HRESULT OnUpdateRectangles() { return NOERROR; };
    virtual HRESULT OnVideoSizeChange();

    // Derived classes must call this to set the pin the filter is using
    // We don't have the pin passed in to the constructor (as we do with
    // the CBaseFilter object) because filters typically create the
    // pins dynamically when requested in CBaseFilter::GetPin. This can
    // not be called from our constructor because is is a virtual method

    void SetControlVideoPin(__inout CBasePin *pPin) {
        m_pPin = pPin;
    }

    // Helper methods for checking rectangles
    virtual HRESULT CheckSourceRect(__in RECT *pSourceRect);
    virtual HRESULT CheckTargetRect(__in RECT *pTargetRect);

public:

    CBaseControlVideo(__inout CBaseFilter *pFilter,    // Owning media filter
                      __in CCritSec *pInterfaceLock,   // Serialise interface
                      __in_opt LPCTSTR pName,          // Object description
                      __inout_opt LPUNKNOWN pUnk,      // Normal COM ownership
                      __inout HRESULT *phr);           // OLE return code

    // These are the properties we support

    STDMETHODIMP get_AvgTimePerFrame(__out REFTIME *pAvgTimePerFrame);
    STDMETHODIMP get_BitRate(__out long *pBitRate);
    STDMETHODIMP get_BitErrorRate(__out long *pBitErrorRate);
    STDMETHODIMP get_VideoWidth(__out long *pVideoWidth);
    STDMETHODIMP get_VideoHeight(__out long *pVideoHeight);
    STDMETHODIMP put_SourceLeft(long SourceLeft);
    STDMETHODIMP get_SourceLeft(__out long *pSourceLeft);
    STDMETHODIMP put_SourceWidth(long SourceWidth);
    STDMETHODIMP get_SourceWidth(__out long *pSourceWidth);
    STDMETHODIMP put_SourceTop(long SourceTop);
    STDMETHODIMP get_SourceTop(__out long *pSourceTop);
    STDMETHODIMP put_SourceHeight(long SourceHeight);
    STDMETHODIMP get_SourceHeight(__out long *pSourceHeight);
    STDMETHODIMP put_DestinationLeft(long DestinationLeft);
    STDMETHODIMP get_DestinationLeft(__out long *pDestinationLeft);
    STDMETHODIMP put_DestinationWidth(long DestinationWidth);
    STDMETHODIMP get_DestinationWidth(__out long *pDestinationWidth);
    STDMETHODIMP put_DestinationTop(long DestinationTop);
    STDMETHODIMP get_DestinationTop(__out long *pDestinationTop);
    STDMETHODIMP put_DestinationHeight(long DestinationHeight);
    STDMETHODIMP get_DestinationHeight(__out long *pDestinationHeight);

    // And these are the methods

    STDMETHODIMP GetVideoSize(__out long *pWidth,__out long *pHeight);
    STDMETHODIMP SetSourcePosition(long Left,long Top,long Width,long Height);
    STDMETHODIMP GetSourcePosition(__out long *pLeft,__out long *pTop,__out long *pWidth,__out long *pHeight);
    STDMETHODIMP GetVideoPaletteEntries(long StartIndex,long Entries,__out long *pRetrieved,__out_ecount_part(Entries, *pRetrieved) long *pPalette);
    STDMETHODIMP SetDefaultSourcePosition();
    STDMETHODIMP IsUsingDefaultSource();
    STDMETHODIMP SetDestinationPosition(long Left,long Top,long Width,long Height);
    STDMETHODIMP GetDestinationPosition(__out long *pLeft,__out long *pTop,__out long *pWidth,__out long *pHeight);
    STDMETHODIMP SetDefaultDestinationPosition();
    STDMETHODIMP IsUsingDefaultDestination();
    STDMETHODIMP GetCurrentImage(__inout long *pBufferSize,__out_bcount_part(*pBufferSize, *pBufferSize) long *pVideoImage);
};

#endif // __WINCTRL__

//------------------------------------------------------------------------------
// File: VideoCtl.h
//
// Desc: DirectShow base classes.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __VIDEOCTL__
#define __VIDEOCTL__

// These help with property page implementations. The first can be used to
// load any string from a resource file. The buffer to load into is passed
// as an input parameter. The same buffer is the return value if the string
// was found otherwise it returns TEXT(""). The GetDialogSize is passed the
// resource ID of a dialog box and returns the size of it in screen pixels

#define STR_MAX_LENGTH 256
LPTSTR WINAPI StringFromResource(__out_ecount(STR_MAX_LENGTH) LPTSTR pBuffer, int iResourceID);

#ifdef UNICODE
#define WideStringFromResource StringFromResource
LPSTR WINAPI StringFromResource(__out_ecount(STR_MAX_LENGTH) LPSTR pBuffer, int iResourceID);
#else
LPWSTR WINAPI WideStringFromResource(__out_ecount(STR_MAX_LENGTH) LPWSTR pBuffer, int iResourceID);
#endif


BOOL WINAPI GetDialogSize(int iResourceID,     // Dialog box resource identifier
                          DLGPROC pDlgProc,    // Pointer to dialog procedure
                          LPARAM lParam,       // Any user data wanted in pDlgProc
                          __out SIZE *pResult);// Returns the size of dialog box

// Class that aggregates an IDirectDraw interface

class CAggDirectDraw : public IDirectDraw, public CUnknown
{
protected:

    LPDIRECTDRAW m_pDirectDraw;

public:

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid,__deref_out void **ppv);

    // Constructor and destructor

    CAggDirectDraw(__in_opt LPCTSTR pName,__inout_opt LPUNKNOWN pUnk) :
        CUnknown(pName,pUnk),
        m_pDirectDraw(NULL) { };

    virtual CAggDirectDraw::~CAggDirectDraw() { };

    // Set the object we should be aggregating
    void SetDirectDraw(__inout LPDIRECTDRAW pDirectDraw) {
        m_pDirectDraw = pDirectDraw;
    }

    // IDirectDraw methods

    STDMETHODIMP Compact();
    STDMETHODIMP CreateClipper(DWORD dwFlags,__deref_out LPDIRECTDRAWCLIPPER *lplpDDClipper,__inout_opt IUnknown *pUnkOuter);
    STDMETHODIMP CreatePalette(DWORD dwFlags,__in LPPALETTEENTRY lpColorTable,__deref_out LPDIRECTDRAWPALETTE *lplpDDPalette,__inout_opt IUnknown *pUnkOuter);
    STDMETHODIMP CreateSurface(__in LPDDSURFACEDESC lpDDSurfaceDesc,__deref_out LPDIRECTDRAWSURFACE *lplpDDSurface,__inout_opt IUnknown *pUnkOuter);
    STDMETHODIMP DuplicateSurface(__in LPDIRECTDRAWSURFACE lpDDSurface,__deref_out LPDIRECTDRAWSURFACE *lplpDupDDSurface);
    STDMETHODIMP EnumDisplayModes(DWORD dwSurfaceDescCount,__in LPDDSURFACEDESC lplpDDSurfaceDescList,__in LPVOID lpContext,__in LPDDENUMMODESCALLBACK lpEnumCallback);
    STDMETHODIMP EnumSurfaces(DWORD dwFlags,__in LPDDSURFACEDESC lpDDSD,__in LPVOID lpContext,__in LPDDENUMSURFACESCALLBACK lpEnumCallback);
    STDMETHODIMP FlipToGDISurface();
    STDMETHODIMP GetCaps(__out LPDDCAPS lpDDDriverCaps,__out LPDDCAPS lpDDHELCaps);
    STDMETHODIMP GetDisplayMode(__out LPDDSURFACEDESC lpDDSurfaceDesc);
    STDMETHODIMP GetFourCCCodes(__inout LPDWORD lpNumCodes,__out_ecount(*lpNumCodes) LPDWORD lpCodes);
    STDMETHODIMP GetGDISurface(__deref_out LPDIRECTDRAWSURFACE *lplpGDIDDSurface);
    STDMETHODIMP GetMonitorFrequency(__out LPDWORD lpdwFrequency);
    STDMETHODIMP GetScanLine(__out LPDWORD lpdwScanLine);
    STDMETHODIMP GetVerticalBlankStatus(__out LPBOOL lpblsInVB);
    STDMETHODIMP Initialize(__in GUID *lpGUID);
    STDMETHODIMP RestoreDisplayMode();
    STDMETHODIMP SetCooperativeLevel(HWND hWnd,DWORD dwFlags);
    STDMETHODIMP SetDisplayMode(DWORD dwWidth,DWORD dwHeight,DWORD dwBpp);
    STDMETHODIMP WaitForVerticalBlank(DWORD dwFlags,HANDLE hEvent);
};


// Class that aggregates an IDirectDrawSurface interface

class CAggDrawSurface : public IDirectDrawSurface, public CUnknown
{
protected:

    LPDIRECTDRAWSURFACE m_pDirectDrawSurface;

public:

    DECLARE_IUNKNOWN
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid,__deref_out void **ppv);

    // Constructor and destructor

    CAggDrawSurface(__in_opt LPCTSTR pName,__inout_opt LPUNKNOWN pUnk) :
        CUnknown(pName,pUnk),
        m_pDirectDrawSurface(NULL) { };

    virtual ~CAggDrawSurface() { };

    // Set the object we should be aggregating
    void SetDirectDrawSurface(__inout LPDIRECTDRAWSURFACE pDirectDrawSurface) {
        m_pDirectDrawSurface = pDirectDrawSurface;
    }

    // IDirectDrawSurface methods

    STDMETHODIMP AddAttachedSurface(__in LPDIRECTDRAWSURFACE lpDDSAttachedSurface);
    STDMETHODIMP AddOverlayDirtyRect(__in LPRECT lpRect);
    STDMETHODIMP Blt(__in LPRECT lpDestRect,__in LPDIRECTDRAWSURFACE lpDDSrcSurface,__in LPRECT lpSrcRect,DWORD dwFlags,__in LPDDBLTFX lpDDBltFx);
    STDMETHODIMP BltBatch(__in_ecount(dwCount) LPDDBLTBATCH lpDDBltBatch,DWORD dwCount,DWORD dwFlags);
    STDMETHODIMP BltFast(DWORD dwX,DWORD dwY,__in LPDIRECTDRAWSURFACE lpDDSrcSurface,__in LPRECT lpSrcRect,DWORD dwTrans);
    STDMETHODIMP DeleteAttachedSurface(DWORD dwFlags,__in LPDIRECTDRAWSURFACE lpDDSAttachedSurface);
    STDMETHODIMP EnumAttachedSurfaces(__in LPVOID lpContext,__in LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback);
    STDMETHODIMP EnumOverlayZOrders(DWORD dwFlags,__in LPVOID lpContext,__in LPDDENUMSURFACESCALLBACK lpfnCallback);
    STDMETHODIMP Flip(__in LPDIRECTDRAWSURFACE lpDDSurfaceTargetOverride,DWORD dwFlags);
    STDMETHODIMP GetAttachedSurface(__in LPDDSCAPS lpDDSCaps,__deref_out LPDIRECTDRAWSURFACE *lplpDDAttachedSurface);
    STDMETHODIMP GetBltStatus(DWORD dwFlags);
    STDMETHODIMP GetCaps(__out LPDDSCAPS lpDDSCaps);
    STDMETHODIMP GetClipper(__deref_out LPDIRECTDRAWCLIPPER *lplpDDClipper);
    STDMETHODIMP GetColorKey(DWORD dwFlags,__out LPDDCOLORKEY lpDDColorKey);
    STDMETHODIMP GetDC(__out HDC *lphDC);
    STDMETHODIMP GetFlipStatus(DWORD dwFlags);
    STDMETHODIMP GetOverlayPosition(__out LPLONG lpdwX,__out LPLONG lpdwY);
    STDMETHODIMP GetPalette(__deref_out LPDIRECTDRAWPALETTE *lplpDDPalette);
    STDMETHODIMP GetPixelFormat(__out LPDDPIXELFORMAT lpDDPixelFormat);
    STDMETHODIMP GetSurfaceDesc(__out LPDDSURFACEDESC lpDDSurfaceDesc);
    STDMETHODIMP Initialize(__in LPDIRECTDRAW lpDD,__in LPDDSURFACEDESC lpDDSurfaceDesc);
    STDMETHODIMP IsLost();
    STDMETHODIMP Lock(__in LPRECT lpDestRect,__inout LPDDSURFACEDESC lpDDSurfaceDesc,DWORD dwFlags,HANDLE hEvent);
    STDMETHODIMP ReleaseDC(HDC hDC);
    STDMETHODIMP Restore();
    STDMETHODIMP SetClipper(__in LPDIRECTDRAWCLIPPER lpDDClipper);
    STDMETHODIMP SetColorKey(DWORD dwFlags,__in LPDDCOLORKEY lpDDColorKey);
    STDMETHODIMP SetOverlayPosition(LONG dwX,LONG dwY);
    STDMETHODIMP SetPalette(__in LPDIRECTDRAWPALETTE lpDDPalette);
    STDMETHODIMP Unlock(__in LPVOID lpSurfaceData);
    STDMETHODIMP UpdateOverlay(__in LPRECT lpSrcRect,__in LPDIRECTDRAWSURFACE lpDDDestSurface,__in LPRECT lpDestRect,DWORD dwFlags,__in LPDDOVERLAYFX lpDDOverlayFX);
    STDMETHODIMP UpdateOverlayDisplay(DWORD dwFlags);
    STDMETHODIMP UpdateOverlayZOrder(DWORD dwFlags,__in LPDIRECTDRAWSURFACE lpDDSReference);
};


class CLoadDirectDraw
{
    LPDIRECTDRAW m_pDirectDraw;     // The DirectDraw driver instance
    HINSTANCE m_hDirectDraw;        // Handle to the loaded library

public:

    CLoadDirectDraw();
    ~CLoadDirectDraw();

    HRESULT LoadDirectDraw(__in LPSTR szDevice);
    void ReleaseDirectDraw();
    HRESULT IsDirectDrawLoaded();
    LPDIRECTDRAW GetDirectDraw();
    BOOL IsDirectDrawVersion1();
};

#endif // __VIDEOCTL__

//------------------------------------------------------------------------------
// File: PStream.h
//
// Desc: DirectShow base classes - defines a class for persistent properties
//       of filters.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __PSTREAM__
#define __PSTREAM__

// Base class for persistent properties of filters
// (i.e. filter properties in saved graphs)

// The simplest way to use this is:
// 1. Arrange for your filter to inherit this class
// 2. Implement in your class WriteToStream and ReadFromStream
//    These will override the "do nothing" functions here.
// 3. Change your NonDelegatingQueryInterface to handle IPersistStream
// 4. Implement SizeMax to return the number of bytes of data you save.
//    If you save UNICODE data, don't forget a char is 2 bytes.
// 5. Whenever your data changes, call SetDirty()
//
// At some point you may decide to alter, or extend the format of your data.
// At that point you will wish that you had a version number in all the old
// saved graphs, so that you can tell, when you read them, whether they
// represent the old or new form.  To assist you in this, this class
// writes and reads a version number.
// When it writes, it calls GetSoftwareVersion()  to enquire what version
// of the software we have at the moment.  (In effect this is a version number
// of the data layout in the file).  It writes this as the first thing in the data.
// If you want to change the version, implement (override) GetSoftwareVersion().
// It reads this from the file into mPS_dwFileVersion before calling ReadFromStream,
// so in ReadFromStream you can check mPS_dwFileVersion to see if you are reading
// an old version file.
// Normally you should accept files whose version is no newer than the software
// version that's reading them.


// CPersistStream
//
// Implements IPersistStream.
// See 'OLE Programmers Reference (Vol 1):Structured Storage Overview' for
// more implementation information.
class CPersistStream : public IPersistStream {
    private:

        // Internal state:

    protected:
        DWORD     mPS_dwFileVersion;         // version number of file (being read)
        BOOL      mPS_fDirty;

    public:

        // IPersistStream methods

        STDMETHODIMP IsDirty()
            {return (mPS_fDirty ? S_OK : S_FALSE);}  // note FALSE means clean
        STDMETHODIMP Load(LPSTREAM pStm);
        STDMETHODIMP Save(LPSTREAM pStm, BOOL fClearDirty);
        STDMETHODIMP GetSizeMax(__out ULARGE_INTEGER * pcbSize)
                         // Allow 24 bytes for version.
                         { pcbSize->QuadPart = 12*sizeof(WCHAR)+SizeMax(); return NOERROR; }

        // implementation

        CPersistStream(IUnknown *punk, __inout HRESULT *phr);
        ~CPersistStream();

        HRESULT SetDirty(BOOL fDirty)
            { mPS_fDirty = fDirty; return NOERROR;}


        // override to reveal IPersist & IPersistStream
        // STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void **ppv);

        // --- IPersist ---

        // You must override this to provide your own class id
        STDMETHODIMP GetClassID(__out CLSID *pClsid) PURE;

        // overrideable if you want
        // file version number.  Override it if you ever change format
        virtual DWORD GetSoftwareVersion(void) { return 0; }


        //=========================================================================
        // OVERRIDE THESE to read and write your data
        // OVERRIDE THESE to read and write your data
        // OVERRIDE THESE to read and write your data

        virtual int SizeMax() {return 0;}
        virtual HRESULT WriteToStream(IStream *pStream);
        virtual HRESULT ReadFromStream(IStream *pStream);
        //=========================================================================

    private:

};


// --- Useful helpers ---


// Writes an int to an IStream as UNICODE.
STDAPI WriteInt(IStream *pIStream, int n);

// inverse of WriteInt
STDAPI_(int) ReadInt(IStream *pIStream, __out HRESULT &hr);

#endif // __PSTREAM__
//------------------------------------------------------------------------------
// File: VTrans.h
//
// Desc: DirectShow base classes - defines a video transform class.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


// This class is derived from CTransformFilter, but is specialised to handle
// the requirements of video quality control by frame dropping.
// This is a non-in-place transform, (i.e. it copies the data) such as a decoder.

class CVideoTransformFilter : public CTransformFilter
{
  public:

    CVideoTransformFilter(__in_opt LPCTSTR, __inout_opt LPUNKNOWN, REFCLSID clsid);
    ~CVideoTransformFilter();
    HRESULT EndFlush();

    // =================================================================
    // ----- override these bits ---------------------------------------
    // =================================================================
    // The following methods are in CTransformFilter which is inherited.
    // They are mentioned here for completeness
    //
    // These MUST be supplied in a derived class
    //
    // NOTE:
    // virtual HRESULT Transform(IMediaSample * pIn, IMediaSample *pOut);
    // virtual HRESULT CheckInputType(const CMediaType* mtIn) PURE;
    // virtual HRESULT CheckTransform
    //     (const CMediaType* mtIn, const CMediaType* mtOut) PURE;
    // static CCOMObject * CreateInstance(LPUNKNOWN, HRESULT *);
    // virtual HRESULT DecideBufferSize
    //     (IMemAllocator * pAllocator, ALLOCATOR_PROPERTIES *pprop) PURE;
    // virtual HRESULT GetMediaType(int iPosition, CMediaType *pMediaType) PURE;
    //
    // These MAY also be overridden
    //
    // virtual HRESULT StopStreaming();
    // virtual HRESULT SetMediaType(PIN_DIRECTION direction,const CMediaType *pmt);
    // virtual HRESULT CheckConnect(PIN_DIRECTION dir,IPin *pPin);
    // virtual HRESULT BreakConnect(PIN_DIRECTION dir);
    // virtual HRESULT CompleteConnect(PIN_DIRECTION direction,IPin *pReceivePin);
    // virtual HRESULT EndOfStream(void);
    // virtual HRESULT BeginFlush(void);
    // virtual HRESULT EndFlush(void);
    // virtual HRESULT NewSegment
    //     (REFERENCE_TIME tStart,REFERENCE_TIME tStop,double dRate);
#ifdef PERF

    // If you override this - ensure that you register all these ids
    // as well as any of your own,
    virtual void RegisterPerfId() {
        m_idSkip        = MSR_REGISTER(TEXT("Video Transform Skip frame"));
        m_idFrameType   = MSR_REGISTER(TEXT("Video transform frame type"));
        m_idLate        = MSR_REGISTER(TEXT("Video Transform Lateness"));
        m_idTimeTillKey = MSR_REGISTER(TEXT("Video Transform Estd. time to next key"));
        CTransformFilter::RegisterPerfId();
    }
#endif

  protected:

    // =========== QUALITY MANAGEMENT IMPLEMENTATION ========================
    // Frames are assumed to come in three types:
    // Type 1: an AVI key frame or an MPEG I frame.
    //        This frame can be decoded with no history.
    //        Dropping this frame means that no further frame can be decoded
    //        until the next type 1 frame.
    //        Type 1 frames are sync points.
    // Type 2: an AVI non-key frame or an MPEG P frame.
    //        This frame cannot be decoded unless the previous type 1 frame was
    //        decoded and all type 2 frames since have been decoded.
    //        Dropping this frame means that no further frame can be decoded
    //        until the next type 1 frame.
    // Type 3: An MPEG B frame.
    //        This frame cannot be decoded unless the previous type 1 or 2 frame
    //        has been decoded AND the subsequent type 1 or 2 frame has also
    //        been decoded.  (This requires decoding the frames out of sequence).
    //        Dropping this frame affects no other frames.  This implementation
    //        does not allow for these.  All non-sync-point frames are treated
    //        as being type 2.
    //
    // The spacing of frames of type 1 in a file is not guaranteed.  There MUST
    // be a type 1 frame at (well, near) the start of the file in order to start
    // decoding at all.  After that there could be one every half second or so,
    // there could be one at the start of each scene (aka "cut", "shot") or
    // there could be no more at all.
    // If there is only a single type 1 frame then NO FRAMES CAN BE DROPPED
    // without losing all the rest of the movie.  There is no way to tell whether
    // this is the case, so we find that we are in the gambling business.
    // To try to improve the odds, we record the greatest interval between type 1s
    // that we have seen and we bet on things being no worse than this in the
    // future.

    // You can tell if it's a type 1 frame by calling IsSyncPoint().
    // there is no architected way to test for a type 3, so you should override
    // the quality management here if you have B-frames.

    int m_nKeyFramePeriod; // the largest observed interval between type 1 frames
                           // 1 means every frame is type 1, 2 means every other.

    int m_nFramesSinceKeyFrame; // Used to count frames since the last type 1.
                                // becomes the new m_nKeyFramePeriod if greater.

    BOOL m_bSkipping;           // we are skipping to the next type 1 frame

#ifdef PERF
    int m_idFrameType;          // MSR id Frame type.  1=Key, 2="non-key"
    int m_idSkip;               // MSR id skipping
    int m_idLate;               // MSR id lateness
    int m_idTimeTillKey;        // MSR id for guessed time till next key frame.
#endif

    virtual HRESULT StartStreaming();

    HRESULT AbortPlayback(HRESULT hr);	// if something bad happens

    HRESULT Receive(IMediaSample *pSample);

    HRESULT AlterQuality(Quality q);

    BOOL ShouldSkipFrame(IMediaSample * pIn);

    int m_itrLate;              // lateness from last Quality message
                                // (this overflows at 214 secs late).
    int m_tDecodeStart;         // timeGetTime when decode started.
    int m_itrAvgDecode;         // Average decode time in reference units.

    BOOL m_bNoSkip;             // debug - no skipping.

    // We send an EC_QUALITY_CHANGE notification to the app if we have to degrade.
    // We send one when we start degrading, not one for every frame, this means
    // we track whether we've sent one yet.
    BOOL m_bQualityChanged;

    // When non-zero, don't pass anything to renderer until next keyframe
    // If there are few keys, give up and eventually draw something
    int m_nWaitForKey;
};
//------------------------------------------------------------------------------
// File: AMExtra.h
//
// Desc: DirectShow base classes.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __AMEXTRA__
#define __AMEXTRA__

// Simple rendered input pin
//
// NOTE if your filter queues stuff before rendering then it may not be
// appropriate to use this class
//
// In that case queue the end of stream condition until the last sample
// is actually rendered and flush the condition appropriately

class CRenderedInputPin : public CBaseInputPin
{
public:

    CRenderedInputPin(__in_opt LPCTSTR pObjectName,
                      __in CBaseFilter *pFilter,
                      __in CCritSec *pLock,
                      __inout HRESULT *phr,
                      __in_opt LPCWSTR pName);
#ifdef UNICODE
    CRenderedInputPin(__in_opt LPCSTR pObjectName,
                      __in CBaseFilter *pFilter,
                      __in CCritSec *pLock,
                      __inout HRESULT *phr,
                      __in_opt LPCWSTR pName);
#endif
    
    // Override methods to track end of stream state
    STDMETHODIMP EndOfStream();
    STDMETHODIMP EndFlush();

    HRESULT Active();
    HRESULT Run(REFERENCE_TIME tStart);

protected:

    // Member variables to track state
    BOOL m_bAtEndOfStream;      // Set by EndOfStream
    BOOL m_bCompleteNotified;   // Set when we notify for EC_COMPLETE

private:
    void DoCompleteHandling();
};

#endif // __AMEXTRA__

//------------------------------------------------------------------------------
// File: CProp.h
//
// Desc: DirectShow base classes.
//
// Copyright (c) 1992-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __CPROP__
#define __CPROP__

// Base property page class. Filters typically expose custom properties by
// implementing special control interfaces, examples are IDirectDrawVideo
// and IQualProp on renderers. This allows property pages to be built that
// use the given interface. Applications such as the ActiveMovie OCX query
// filters for the property pages they support and expose them to the user
//
// This class provides all the framework for a property page. A property
// page is a COM object that supports IPropertyPage. We should be created
// with a resource ID for the dialog which we will load when required. We
// should also be given in the constructor a resource ID for a title string
// we will load from the DLLs STRINGTABLE. The property page titles must be
// stored in resource files so that they can be easily internationalised
//
// We have a number of virtual methods (not PURE) that may be overriden in
// derived classes to query for interfaces and so on. These functions have
// simple implementations here that just return NOERROR. Derived classes
// will almost definately have to override the message handler method called
// OnReceiveMessage. We have a static dialog procedure that calls the method
// so that derived classes don't have to fiddle around with the this pointer

class AM_NOVTABLE CBasePropertyPage : public IPropertyPage, public CUnknown
{
protected:

    LPPROPERTYPAGESITE m_pPageSite;       // Details for our property site
    HWND m_hwnd;                          // Window handle for the page
    HWND m_Dlg;                           // Actual dialog window handle
    BOOL m_bDirty;                        // Has anything been changed
    int m_TitleId;                        // Resource identifier for title
    int m_DialogId;                       // Dialog resource identifier

    static INT_PTR CALLBACK DialogProc(HWND hwnd,
                                       UINT uMsg,
                                       WPARAM wParam,
                                       LPARAM lParam);

private:
    BOOL m_bObjectSet ;                  // SetObject has been called or not.
public:

    CBasePropertyPage(__in_opt LPCTSTR pName,      // Debug only name
                      __inout_opt LPUNKNOWN pUnk, // COM Delegator
                      int DialogId,               // Resource ID
                      int TitleId);               // To get tital

#ifdef UNICODE
    CBasePropertyPage(__in_opt LPCSTR pName,
                      __inout_opt LPUNKNOWN pUnk,
                      int DialogId,  
                      int TitleId);
#endif
    virtual ~CBasePropertyPage() { };
    DECLARE_IUNKNOWN

    // Override these virtual methods

    virtual HRESULT OnConnect(IUnknown *pUnknown) { return NOERROR; };
    virtual HRESULT OnDisconnect() { return NOERROR; };
    virtual HRESULT OnActivate() { return NOERROR; };
    virtual HRESULT OnDeactivate() { return NOERROR; };
    virtual HRESULT OnApplyChanges() { return NOERROR; };
    virtual INT_PTR OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);

    // These implement an IPropertyPage interface

    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, __deref_out void **ppv);
    STDMETHODIMP_(ULONG) NonDelegatingRelease();
    STDMETHODIMP_(ULONG) NonDelegatingAddRef();
    STDMETHODIMP SetPageSite(__in_opt LPPROPERTYPAGESITE pPageSite);
    STDMETHODIMP Activate(HWND hwndParent, LPCRECT prect,BOOL fModal);
    STDMETHODIMP Deactivate(void);
    STDMETHODIMP GetPageInfo(__out LPPROPPAGEINFO pPageInfo);
    STDMETHODIMP SetObjects(ULONG cObjects, __in_ecount_opt(cObjects) LPUNKNOWN *ppUnk);
    STDMETHODIMP Show(UINT nCmdShow);
    STDMETHODIMP Move(LPCRECT prect);
    STDMETHODIMP IsPageDirty(void) { return m_bDirty ? S_OK : S_FALSE; }
    STDMETHODIMP Apply(void);
    STDMETHODIMP Help(LPCWSTR lpszHelpDir) { return E_NOTIMPL; }
    STDMETHODIMP TranslateAccelerator(__inout LPMSG lpMsg) { return E_NOTIMPL; }
};

#endif // __CPROP__

//------------------------------------------------------------------------------
// File: StrmCtl.h
//
// Desc: DirectShow base classes.
//
// Copyright (c) 1996-2001 Microsoft Corporation.  All rights reserved.
//------------------------------------------------------------------------------


#ifndef __strmctl_h__
#define __strmctl_h__

class CBaseStreamControl : public IAMStreamControl
{
public:
    // Used by the implementation
    enum StreamControlState
    { STREAM_FLOWING = 0x1000,
      STREAM_DISCARDING
    };

private:
    enum StreamControlState	m_StreamState;		// Current stream state
    enum StreamControlState	m_StreamStateOnStop;	// State after next stop
						// (i.e.Blocking or Discarding)

    REFERENCE_TIME	m_tStartTime;	    // MAX_TIME implies none
    REFERENCE_TIME	m_tStopTime;	    // MAX_TIME implies none
    DWORD		m_dwStartCookie;    // Cookie for notification to app
    DWORD		m_dwStopCookie;	    // Cookie for notification to app
    volatile BOOL       m_bIsFlushing;        // No optimization pls!
    volatile BOOL	m_bStopSendExtra;   // bSendExtra was set
    volatile BOOL	m_bStopExtraSent;   // the extra one was sent

    CCritSec		m_CritSec;	    // CritSec to guard above attributes

    // Event to fire when we can come
    // out of blocking, or to come out of waiting
    // to discard if we change our minds.
    //
    CAMEvent			m_StreamEvent;

    // All of these methods execute immediately.  Helpers for others.
    //
    void ExecuteStop();
    void ExecuteStart();
    void CancelStop();
    void CancelStart();

    // Some things we need to be told by our owning filter
    // Your pin must also expose IAMStreamControl when QI'd for it!
    //
    IReferenceClock *	m_pRefClock;	    // Need it to set advises
					    // Filter must tell us via
					    // SetSyncSource
    IMediaEventSink *   m_pSink;            // Event sink
					    // Filter must tell us after it
					    // creates it in JoinFilterGraph()
    FILTER_STATE	m_FilterState;	    // Just need it!
					    // Filter must tell us via
					    // NotifyFilterState
    REFERENCE_TIME	m_tRunStart;	    // Per the Run call to the filter

    // This guy will return one of the three StreamControlState's.  Here's what
    // the caller should do for each one:
    //
    // STREAM_FLOWING:		Proceed as usual (render or pass the sample on)
    // STREAM_DISCARDING:	Calculate the time 'til *pSampleStop and wait
    //				that long for the event handle
    //				(GetStreamEventHandle()).  If the wait
    //				expires, throw the sample away.  If the event
    //				fires, call me back - I've changed my mind.
    //
    enum StreamControlState CheckSampleTimes( __in const REFERENCE_TIME * pSampleStart,
					      __in const REFERENCE_TIME * pSampleStop );

public:
    // You don't have to tell us much when we're created, but there are other
    // obligations that must be met.  See SetSyncSource & NotifyFilterState
    // below.
    //
    CBaseStreamControl(__inout_opt HRESULT *phr = NULL);
    ~CBaseStreamControl();

    // If you want this class to work properly, there are thing you need to
    // (keep) telling it.  Filters with pins that use this class
    // should ensure that they pass through to this method any calls they
    // receive on their SetSyncSource.

    // We need a clock to see what time it is.  This is for the
    // "discard in a timely fashion" logic.  If we discard everything as
    // quick as possible, a whole 60 minute file could get discarded in the
    // first 10 seconds, and if somebody wants to turn streaming on at 30 
    // minutes into the file, and they make the call more than a few seconds
    // after the graph is run, it may be too late!
    // So we hold every sample until it's time has gone, then we discard it.
    // The filter should call this when it gets a SetSyncSource
    //
    void SetSyncSource( IReferenceClock * pRefClock )
    {
	CAutoLock lck(&m_CritSec);
	if (m_pRefClock) m_pRefClock->Release();
	m_pRefClock = pRefClock;
	if (m_pRefClock) m_pRefClock->AddRef();
    }

    // Set event sink for notifications
    // The filter should call this in its JoinFilterGraph after it creates the
    // IMediaEventSink
    //
    void SetFilterGraph( IMediaEventSink *pSink ) {
        m_pSink = pSink;
    }

    // Since we schedule in stream time, we need the tStart and must track the
    // state of our owning filter.
    // The app should call this ever state change
    //
    void NotifyFilterState( FILTER_STATE new_state, REFERENCE_TIME tStart = 0 );

    // Filter should call Flushing(TRUE) in BeginFlush,
    // and Flushing(FALSE) in EndFlush.
    //
    void Flushing( BOOL bInProgress );


    // The two main methods of IAMStreamControl

    // Class adds default values suitable for immediate
    // muting and unmuting of the stream.

    STDMETHODIMP StopAt( const REFERENCE_TIME * ptStop = NULL,
			 BOOL bSendExtra = FALSE,
			 DWORD dwCookie = 0 );
    STDMETHODIMP StartAt( const REFERENCE_TIME * ptStart = NULL,
		    	  DWORD dwCookie = 0 );
    STDMETHODIMP GetInfo( __out AM_STREAM_INFO *pInfo);

    // Helper function for pin's receive method.  Call this with
    // the sample and we'll tell you what to do with it.  We'll do a
    // WaitForSingleObject within this call if one is required.  This is
    // a "What should I do with this sample?" kind of call. We'll tell the
    // caller to either flow it or discard it.
    // If pSample is NULL we evaluate based on the current state
    // settings
    enum StreamControlState CheckStreamState( IMediaSample * pSample );

private:
    // These don't require locking, but we are relying on the fact that
    // m_StreamState can be retrieved with integrity, and is a snap shot that
    // may have just been, or may be just about to be, changed.
    HANDLE GetStreamEventHandle() const { return m_StreamEvent; }
    enum StreamControlState GetStreamState() const { return m_StreamState; }
    BOOL IsStreaming() const { return m_StreamState == STREAM_FLOWING; }
};

#endif



#else
    #ifdef DEBUG
    #pragma message("STREAMS.H included TWICE")
    #endif
#endif // __STREAMS__

