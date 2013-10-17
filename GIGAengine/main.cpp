
#include "main.h"
#include "resource1.h"
#include <commctrl.h>

#include "timeline.h"
#include "effect.h"
#include "blobs.h"
#include "quad.h"
#include "sky.h"
#include "wall.h"
#include "nostatusEffect.h"
#include "vuoriEffect.h"
#include "insideEffect.h"
#include "cityEffect.h"
#include "blurbEffect.h"
#include "postprocess.h"
#include "shaderstorage.h"
#include "objloader.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "strmiids.lib")
#pragma comment(lib, "ComCtl32.lib")

#pragma comment(linker, "/nodefaultlib:msvcrt.lib")

#ifdef _DEBUG
#pragma comment(lib, "strmbasd.lib")
#else
#pragma comment(lib, "strmbase.lib")
#endif

#pragma comment(lib, "winmm.lib")

texture* cityTexture;
texture* koneTexture;
texture* mountainTexture;

bool runprogram = true, full = false;

int screenw = 0, screenh = 0;

INT_PTR CALLBACK launcherProc(HWND dlg, UINT msg, WPARAM w, LPARAM l)
{
	if(msg==WM_INITDIALOG)
	{
		ComboBox_AddString(GetDlgItem(dlg, IDC_COMBO1), L"1280x720  (welcome to the 80s)");
		ComboBox_AddString(GetDlgItem(dlg, IDC_COMBO1), L"1920x1080 (boring and typical)");
		ComboBox_AddString(GetDlgItem(dlg, IDC_COMBO1), L"3840x2160 (the PEISIK experience)");
		
		ComboBox_SetCurSel(GetDlgItem(dlg, IDC_COMBO1), 0);
		CheckDlgButton(dlg, IDC_CHECK1, BST_CHECKED);
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

	mesh quad(MESH_QUAD);

	song track(L"assets/noise01_7.mp3", 114.0);

	// a global texture used by some effects
	cityTexture = new texture(L"assets/walls.png");
	koneTexture = new texture(L"assets/kone.jpg");
	mountainTexture = new texture(L"assets/vuori.jpg");

	double dirx = .0, diry = .0, posx = .0, posy = .0, posz = 3.0;
	POINT pt = {win.width/2, win.height/2};

	float t = .0f;

	bool flymode = false;
	
	Blobs b;
	QuadEffect q;
	NostatusEffect nostatus;
	VuoriEffect vuoriefu;
	InsideEffect insideefu;
	CityEffect cityefu;
	BlurbEffect blurbefu;
	Sky sky;
	Wall wall;

	TimeLine timeline;
	CurveMap p1,p2,p3;
	p1["camx"](9.0f,.0f,.0f)(15.0f,20.0f,.0f)(55.0f,-12.0f,.0f)(80.0f,-3.0f,-1.0f);
	p1["camy"](9.0f,.0f,.0f)(20.0f,13.0f,.0f)(80.0f,5.0f,-1.0f);
	p1["camz"](9.0f,15.0f,1.0f)(50.0f,65.0f,1.0f)(80.0f,39.0f,-1.0f);
	p1["targetx"](9.0f,.0f,.0f)(71.0f,30.0f,1.0f)(80.0f,12.0f,-1.0f);
	p1["targety"](9.0f,.0f,.0f)(64.0f,3.0f,1.0f)(80.0f,30.0f,-1.0f);
	p1["targetz"](9.0f,-26.0f,.0f)(45.0f,-1.0f,1.0f)(80.0f,-15.0f,-1.0f);
	p1["aspect"](.0f,float(screenh)/float(screenw),.0f);
	p1["primx"](.0f,1.0f,.0f);
	p1["primy"](.0f,1.0f,.0f);
	p1["primz"](.0f,1.0f,.0f);
	p1["primh"](.0f,1.0f,.0f);
	p1["prims"](.0f,.4f,.0f);
	p1["primv"](.0f,20.0f,-6.0f)(10.0f,.4f,.0f);
	p1["secx"](.0f,1.0f,.0f);
	p1["secy"](.0f,-1.0f,.0f);
	p1["secz"](.0f,-1.0f,.0f);
	p1["sech"](.0f,170.0f,.0f);
	p1["secs"](.0f,.4f,.0f);
	p1["secv"](.0f,.15f,.0f);
	p1["fov"](.0f,4.0f,.0f)(80.0f,2.0f,.0f);

	CurveMap insideCurves;
	insideCurves["lamp"](0.0, 0.0f, 0.0f)(5.0f, 1.0f, 1.0f);
	insideCurves["speed"](1.0, 0.5, 1.0f);

	CurveMap cityp;
	cityp["speed"](0.0, 0.1, 1.0f);
	cityp["lamp"](0.0, 0.0, 1.0f);

	//p1["t"](0.0f,0.0f,1.0f)(40.0f,40.0f,1.0f);
	timeline.addEntry(0.0f, 80.0f, sky, p1);
	timeline.addEntry(0.0f, 80.0f, wall, p1);
	timeline.addEntry(80.0f, 144.0f, insideefu, cityp);
	timeline.addEntry(80.0f, 144.0f, cityefu, p3);
	p2["r"](0.0f,1.0f,0.0f)(10.0f,0.0f,0.0f)(15.0f,1.0f,0.0f);
	p2["g"](0.0f,0.0f,0.0f)(10.0f,1.0f,0.0f)(15.0f,0.0f,0.0f);
	p2["b"](0.0f,1.0f,0.0f)(10.0f,0.0f,0.0f)(15.0f,1.0f,0.0f);
	timeline.addEntry(144.0f, 183.0f, vuoriefu, p2);
	timeline.addEntry(183.0f, 248.0f, insideefu, insideCurves);
	timeline.addEntry(183.0f, 248.0f, blurbefu, insideCurves);
	timeline.addEntry(248.0f, 500.0f, nostatus, p2);

	track.seekBeats(.0);
	track.play();
	
	t = track.getTime();

	PostProcess post(screenw, screenh);
	post.bind();
	while(win.loop()) {
		glBeginQuery(GL_TIME_ELAPSED, query);

		if (win.keyHit[VK_F1]) {
			shaderstorage.reloadAll();
			post.bindUniforms();
		}

		if(win.keyDown[VK_LEFT])
			track.seekBeats(track.getBeats()-.5);
		if(win.keyDown[VK_RIGHT])
			track.seekBeats(track.getBeats()+.5);
		if(win.keyHit[0x31]) track.seekBeats(timeline.getBeginning(0));
		if(win.keyHit[0x32]) track.seekBeats(timeline.getBeginning(1));
		if(win.keyHit[0x33]) track.seekBeats(timeline.getBeginning(2));
		if(win.keyHit[0x34]) track.seekBeats(timeline.getBeginning(3));
		if(win.keyHit[0x35]) track.seekBeats(timeline.getBeginning(4));
		if(win.keyHit[0x36]) track.seekBeats(timeline.getBeginning(5));
		if(win.keyHit[0x37]) track.seekBeats(timeline.getBeginning(6));
		if(win.keyHit[0x38]) track.seekBeats(timeline.getBeginning(7));
		if(win.keyHit[0x39]) track.seekBeats(timeline.getBeginning(8));
		if(win.keyHit[0x40]) track.seekBeats(timeline.getBeginning(9));
		if(win.keyHit[0x30]) track.seekBeats(timeline.getBeginning(10));

		if (win.keyHit[VK_RETURN])
			track.toggle();

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

		timeline.render(t);

		post.render(t);

		#ifndef _RELEASE
		glEndQuery(GL_TIME_ELAPSED);
		glGetQueryObjectiv(query, GL_QUERY_RESULT, &res);

		
		if(!(loops%60)) {
			printf("frametime: %.2lfms\n", double(res)/1000000.0);
			printf("gl error: 0x%X\n", glGetError());
			printf("time: %f\n", t);
		}
		
		#endif
		loops++;
	}

	delete cityTexture;
	delete koneTexture;
	delete mountainTexture;

	return 0;
}
