
#include "main.h"
#include "resource1.h"
#include <commctrl.h>

#include "effect.h"
#include "blobs.h"

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

INT_PTR CALLBACK startupproc(HWND dlg, UINT msg, WPARAM w, LPARAM l)
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

	//DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)startupproc);
	
	if(!runprogram)
		ExitProcess(0);

	screenw = 1280;
	screenh = 720;

	window win(screenw, screenh, full, L"ALTDEMO");
	//window win(1920, 1080, 1, L"windy window");
	
	ShowCursor(0);

	((PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT"))(1);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	unsigned int loops = 0;
	GLuint query; GLint res;
	glGenQueries(1, &query);

	mesh quad(QUAD);

	song track(L"track.mp3");

	double dirx = .0, diry = .0, posx = .0, posy = .0, posz = 3.0;
	POINT pt = {win.width/2, win.height/2};

	float t = .0f;

	bool flymode = false;

	track.seek(.0);
	track.play();
	t = track.getTime();

	Blobs b;

	while(win.loop() && t<200.0f) {
		glBeginQuery(GL_TIME_ELAPSED, query);

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

		b.render(t);
		
		//while(t>track.getTime());
		//while(t<track.getTime())
		while(t<track.getTime())
			t+=1.0f/60.0f;
		//t = win.time();
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
