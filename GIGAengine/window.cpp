
#define declaregl
#include "main.h"

LRESULT CALLBACK wndproc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) {
	if(message==WM_DESTROY) {
		PostQuitMessage(0);
		return 0;
	}
	else
		return DefWindowProc(windowHandle, message, wParam, lParam);
}

window::window(int width, int height, bool fullScreen, std::wstring ptitle) : width (width), height(height), fullScreen(fullScreen), title(ptitle) {
	
	windowHandle = 0;
	deviceContext = 0;
	renderingContext = 0;

	WNDCLASSEX windowClass = {0};
	windowClass.cbSize = sizeof(windowClass);
	windowClass.hInstance = GetModuleHandle(0);
	windowClass.style = CS_OWNDC;
	windowClass.hIcon = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(129));
	windowClass.lpfnWndProc = wndproc;
	windowClass.lpszClassName = L"classy class";

	ZeroMemory(keyDown, 256*sizeof(bool));
	mouseLeft = mouseRight = false;

	RegisterClassEx(&windowClass);

	HWND temporaryWindow = CreateWindowEx(WS_EX_APPWINDOW, L"classy class", L"temporary", WS_SYSMENU|WS_BORDER|WS_MINIMIZEBOX, 0, 0, 0, 0, 0, 0, windowClass.hInstance, 0);
	HDC temporaryDeviceContext = GetDC(temporaryWindow);
	
	PIXELFORMATDESCRIPTOR pixelFormat = {0};

	pixelFormat.nSize = sizeof(pixelFormat);
	pixelFormat.nVersion = 1;
	pixelFormat.dwFlags = PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER;
	pixelFormat.cColorBits = 32;
	pixelFormat.cDepthBits = 24;
	
	SetPixelFormat(temporaryDeviceContext, ChoosePixelFormat(temporaryDeviceContext, &pixelFormat), &pixelFormat);
	
	HGLRC temporaryRenderingContext = wglCreateContext(temporaryDeviceContext);

	wglMakeCurrent(temporaryDeviceContext, temporaryRenderingContext);

	const int formatAttributes[] = 
	{
		WGL_DRAW_TO_WINDOW_ARB,    1,
		WGL_SUPPORT_OPENGL_ARB,    1,
		WGL_ACCELERATION_ARB, 0x2027,
		WGL_DOUBLE_BUFFER_ARB,     1,
		WGL_PIXEL_TYPE_ARB,   0x202B,
		WGL_COLOR_BITS_ARB,       32,
		WGL_DEPTH_BITS_ARB,       24,
		0
	};

	const int contextAttributes[] = 
	{
		WGL_CONTEXT_PROFILE_MASK_ARB, 1,
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		0
	};

	int format, formatcount;
	
	RECT windowArea = {0, 0, width, height};
	DWORD displayFlags = WS_POPUP;

	if(fullScreen) {
		DEVMODE dev = {0};
		dev.dmSize = sizeof(DEVMODE);
		dev.dmFields = DM_PELSWIDTH|DM_PELSHEIGHT|DM_BITSPERPEL;
		dev.dmPelsWidth = width;
		dev.dmPelsHeight = height;
		dev.dmBitsPerPel = 32;
		ChangeDisplaySettings(&dev, CDS_FULLSCREEN);
	}
	else {
		displayFlags = WS_SYSMENU|WS_CAPTION|WS_MINIMIZEBOX|WS_BORDER;
		AdjustWindowRect(&windowArea, displayFlags, 0);
		windowArea.right -= windowArea.left;
		windowArea.bottom -= windowArea.top;
		windowArea.left = (GetSystemMetrics(SM_CXSCREEN)-windowArea.right )/2;
		windowArea.top  = (GetSystemMetrics(SM_CYSCREEN)-windowArea.bottom)/2;
	}
	
	windowHandle = CreateWindowEx(WS_EX_APPWINDOW, L"classy class", title.c_str(), displayFlags, windowArea.left, windowArea.top, windowArea.right, windowArea.bottom, 0, 0, windowClass.hInstance, 0);
	deviceContext = GetDC(windowHandle);
	
	((PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB"))(deviceContext, formatAttributes, 0, 1, &format, (UINT*)&formatcount);

	SetPixelFormat(deviceContext, format, &pixelFormat);

	renderingContext = ((PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB"))(deviceContext, 0, contextAttributes);
	
	wglMakeCurrent(deviceContext, renderingContext);
	
	wglDeleteContext(temporaryRenderingContext);
	ReleaseDC(temporaryWindow, temporaryDeviceContext);
	DestroyWindow(temporaryWindow);
	MSG message;
	while(PeekMessage(&message, 0, 0, 0, PM_REMOVE));

	#include "glloading.h"

	printf("Vendor         : %s\nRenderer       : %s\nOpenGL version : %s\nGLSL version   : %s\n",glGetString(GL_VENDOR), glGetString(GL_RENDERER), glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	ShowWindow(windowHandle, SW_SHOW);
	glViewport(0, 0, width, height);
	glClearColor(.0f,.0f,.0f,.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	SwapBuffers(deviceContext);

	Gdiplus::GdiplusStartupInput gdiInput;
	ZeroMemory(&gdiInput, sizeof(gdiInput));
	gdiInput.GdiplusVersion = 1;

	Gdiplus::GdiplusStartup((ULONG_PTR*)&gdiToken, &gdiInput, 0);
	
	/*int charsize = 64;
	int fontsize = 16*charsize;

	Gdiplus::Font f(L"Segoe UI Light", float(charsize)*.7f, Gdiplus::FontStyleItalic, Gdiplus::UnitPixel, 0);
	Gdiplus::Bitmap canvas(fontsize, fontsize, PixelFormat32bppARGB);
	Gdiplus::Graphics gfx((Gdiplus::Image*)&canvas);
	gfx.Clear(Gdiplus::Color(0,0,0));
	Gdiplus::SolidBrush brush(Gdiplus::Color(255,255,255));
	Gdiplus::Rect r(0,0, fontsize, fontsize);

	for(int i = 0; i<16; i++)
	for(int j = 0; j<16; j++) {
		wchar_t k = i*16+j;
		Gdiplus::PointF point(float(charsize)*(float(i)+.2f), float(charsize)*(float(j)+.1f));
		gfx.DrawString(&k, 1, &f, point, &brush);
	}*/

	startTime = GetTickCount();
	frameStartTime = startTime+1000;
	frameCount = 0;
	defaultid = 0;
}

int _vsnprintf( wchar_t *buff, size_t sizeofbuff, size_t count, const wchar_t *format, va_list argptr) {
	return 0;
}

int window::loop() {
	frameCount++;
	DWORD curTime = GetTickCount();

	if(frameStartTime<curTime) {
		std::wstringstream newTitle;
		newTitle << title << L" - " << frameCount;
		SetWindowText(windowHandle, newTitle.str().c_str());

		frameStartTime = curTime+1000;
		frameCount = 0;
	}

	SwapBuffers(deviceContext);
	MSG message;
	ZeroMemory(keyHit, 256*sizeof(bool));
	while(PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
		TranslateMessage(&message);
		DispatchMessage(&message);
		if(message.message == WM_QUIT || message.message == WM_KEYDOWN && message.wParam == VK_ESCAPE)
			return 0;
		if(message.message==WM_KEYDOWN)
			keyHit[message.wParam] = keyDown[message.wParam] = true;
		if(message.message==WM_KEYUP)
			keyDown[message.wParam] = false;
		if(message.message==WM_LBUTTONDOWN)
			mouseLeft = true;
		if(message.message==WM_LBUTTONUP)
			mouseLeft = false;
		if(message.message==WM_RBUTTONDOWN)
			mouseRight = true;
		if(message.message==WM_RBUTTONUP)
			mouseRight = false;
		if(message.message==WM_MOUSEMOVE) {
			mousex = int(message.lParam)%65536;
			mousey = int(message.lParam)/65536;
		}
	}
	return 1;
}

window::~window() {
	Gdiplus::GdiplusShutdown(gdiToken);

	if(fullScreen)
		ChangeDisplaySettings(0, 0);
	wglMakeCurrent(0,0);
	wglDeleteContext(renderingContext);
	ReleaseDC(windowHandle, deviceContext);
	DestroyWindow(windowHandle);
	UnregisterClass(L"classy class", GetModuleHandle(0));
	return;
}

int window::attach(int id) {
	if(id<0)
		id = defaultid;
	glViewport(0, 0, width, height);
	glBindFramebuffer(GL_FRAMEBUFFER, id);
	return 0;
}

int window::setDefaultTarget(int id) {
	defaultid = id;
	return 0;
}

double window::time() {
	return double(GetTickCount()-startTime)*.001;
}