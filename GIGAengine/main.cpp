
#include "main.h"
#include "resource1.h"
#include <commctrl.h>

#include "timeline.h"
#include "effect.h"
#include "blobs.h"
#include "quad.h"
#include "postprocess.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "ComCtl32.lib")
#pragma comment(lib, "msvcrt.lib")

#pragma comment(linker, "/nodefaultlib:libcmt.lib")

#ifdef _DEBUG
#pragma comment(lib, "strmbasd.lib")
#else
#pragma comment(lib, "strmbase.lib")
#endif

#pragma comment(lib, "winmm.lib")

bool runprogram = true, full = false;

int screenw = 0, screenh = 0;

INT_PTR CALLBACK launcherProc(HWND dlg, UINT msg, WPARAM w, LPARAM l)
{
	if(msg==WM_INITDIALOG)
	{
		ComboBox_AddString(GetDlgItem(dlg, IDC_COMBO1), L"1280x720");
		ComboBox_AddString(GetDlgItem(dlg, IDC_COMBO1), L"1920x1080");
		ComboBox_AddString(GetDlgItem(dlg, IDC_COMBO1), L"3840x2160");
		
		ComboBox_SetCurSel(GetDlgItem(dlg, IDC_COMBO1), 0);
	}
	if(msg==WM_COMMAND)
	{

		if(HIWORD(w)==BN_CLICKED && LOWORD(w)==IDCANCEL)
		{
			runprogram=false;
			EndDialog(dlg,0);
			return 1;
		}

		if(HIWORD(w)==BN_CLICKED && LOWORD(w)==IDOK) // launch pressed
		{
			int len = 1+ComboBox_GetTextLength(GetDlgItem(dlg, IDC_COMBO1));
			wchar_t * buff = new wchar_t[len];
			ComboBox_GetText(GetDlgItem(dlg, IDC_COMBO1), buff, len);

			swscanf_s(buff, L"%dx%d", &screenw, &screenh);

			delete [] buff;

			if(BST_CHECKED == IsDlgButtonChecked(dlg,IDC_CHECK1))
				full = true;

			EndDialog(dlg, 0);
			return 1;
		}
	}
	return 0;
}


//int CALLBACK main(HINSTANCE self, HINSTANCE prev, LPSTR cmdline, int show) {
int main() {

	InitCommonControls();

	//DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)launcherProc);
	
	if(!runprogram)
		ExitProcess(0);

	screenw = 1280;
	screenh = 720;

	window win(screenw, screenh, full, L"ALTDEMO");
	
	ShowCursor(0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	unsigned int loops = 0;
	GLuint query; GLint res;
	glGenQueries(1, &query);

	mesh quad(QUAD);

	song track(L"assets/noise01_7.mp3", 114.0);

	double dirx = .0, diry = .0, posx = .0, posy = .0, posz = 3.0;
	POINT pt = {win.width/2, win.height/2};

	float t = .0f;

	bool flymode = false;
	
	Blobs b;
	QuadEffect q;

	TimeLine T;
	CurveMap p1,p2,p3;
	p1["r"](0.0f,1.0f,0.0f)(5.0f,0.0f,0.0f)(10.0f,1.0f,0.0f);
	p1["g"](0.0f,1.0f,0.0f)(5.0f,0.0f,0.0f)(10.0f,1.0f,0.0f);
	p1["b"](0.0f,1.0f,0.0f)(5.0f,0.0f,0.0f)(10.0f,1.0f,0.0f);
	p1["t"](0.0f,0.0f,1.0f)(20.0f,20.0f,1.0f);
	T.addEntry(0.0f, 20.0f, b, p1);
	p3["t"](0.0f,0.0f,1.0f)(20.0f,20.0f,1.0f);
	T.addEntry(20.0f, 40.0f, q, p3);
	p2["r"](0.0f,1.0f,0.0f)(10.0f,0.0f,0.0f)(15.0f,1.0f,0.0f);
	p2["g"](0.0f,0.0f,0.0f)(10.0f,1.0f,0.0f)(15.0f,0.0f,0.0f);
	p2["b"](0.0f,1.0f,0.0f)(10.0f,0.0f,0.0f)(15.0f,1.0f,0.0f);
	p2["t"](0.0f,0.0f,1.0f)(20.0f,20.0f,1.0f);
	T.addEntry(40.0f, 60.0f, b, p2);

	track.seekBeats(.0);
	track.play();
	
	t = track.getTime();

	PostProcess p(screenw, screenh);
	p.bind();

	while(win.loop()) {
		glBeginQuery(GL_TIME_ELAPSED, query);

		if(win.keyDown[VK_LEFT])
			track.seekBeats(track.getBeats()-.5);
		if(win.keyDown[VK_RIGHT])
			track.seekBeats(track.getBeats()+.5);
		if(win.keyHit[0x31]) track.seekBeats(T.getBeginning(0));
		if(win.keyHit[0x32]) track.seekBeats(T.getBeginning(1));
		if(win.keyHit[0x33]) track.seekBeats(T.getBeginning(2));
		if(win.keyHit[0x34]) track.seekBeats(T.getBeginning(3));
		if(win.keyHit[0x35]) track.seekBeats(T.getBeginning(4));
		if(win.keyHit[0x36]) track.seekBeats(T.getBeginning(5));
		if(win.keyHit[0x37]) track.seekBeats(T.getBeginning(6));
		if(win.keyHit[0x38]) track.seekBeats(T.getBeginning(7));
		if(win.keyHit[0x39]) track.seekBeats(T.getBeginning(8));
		if(win.keyHit[0x40]) track.seekBeats(T.getBeginning(9));
		if(win.keyHit[0x30]) track.seekBeats(T.getBeginning(10));
		
		if(win.keyHit[VK_SPACE]) {
			flymode = !flymode;
			if(flymode) {
				win.mousex = win.width/2;
				win.mousey = win.height/2;
				ShowCursor(0);
			}
			else
				ShowCursor(1);
		}


		t = track.getBeats();

		T.render(t);

		p.render(t);
		
		glEndQuery(GL_TIME_ELAPSED);
		glGetQueryObjectiv(query, GL_QUERY_RESULT, &res);
		if(!(loops%60)) {
			printf("frametime: %.2lfms\n", double(res)/1000000.0);
			printf("gl error: 0x%X\n", glGetError());
			printf("time: %f\n", t);
		}
		loops++;
	}

	return 0;
}
