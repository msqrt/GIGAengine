
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
#include "towerEffect.h"
#include "creditsEffect.h"
#include "backparticles.h"
#include "postprocess.h"
#include "shaderstorage.h"
#include "objloader.h"
#include "laser.h"
#include "flashEffect.h"

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

int global_screenw = 0, global_screenh = 0;

INT_PTR CALLBACK launcherProc(HWND dlg, UINT msg, WPARAM w, LPARAM l)
{
	if(msg==WM_INITDIALOG)
	{
		ComboBox_AddString(GetDlgItem(dlg, IDC_COMBO1), L"1280x720  (we render large pixels)");
		ComboBox_AddString(GetDlgItem(dlg, IDC_COMBO1), L"1920x1080 (demo for the masses)");
		ComboBox_AddString(GetDlgItem(dlg, IDC_COMBO1), L"3840x2160 (the PEISIK experience)");
		
		ComboBox_SetCurSel(GetDlgItem(dlg, IDC_COMBO1), 1);
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

			swscanf_s(buff, L"%dx%d", &global_screenw, &global_screenh);

			delete [] buff;

			if(BST_CHECKED == IsDlgButtonChecked(dlg,IDC_CHECK1))
				full = true;

			EndDialog(dlg, 0);
			return 1;
		}
	}
	return 0;
}

window* win;

int main() {
	InitCommonControls();

	#ifdef _RELEASE
	DialogBox(GetModuleHandle(0), MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)launcherProc);
	if(!runprogram)
		ExitProcess(0);
	#else
	global_screenw = 1280 / 2;
	global_screenh = 720 / 2;
	#endif

	win = new window(global_screenw, global_screenh, full, L"PILOT/A");
	
	if(wglGetProcAddress("wglSwapIntervalEXT"))
		((PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT"))(1);

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

	#define randf() ((float)rand()/(float)RAND_MAX)
	srand(0);


	double dirx = .0, diry = .0, posx = .0, posy = .0, posz = 3.0;
	POINT pt = {win->width/2, win->height/2};

	float t = .0f;

	bool flymode = false;
	
	Blobs b;
	Dust dust;
	QuadEffect q;
	NostatusEffect nostatus;
	VuoriEffect vuoriefu;
	InsideEffect insideefu;
	CityEffect cityefu;
	BlurbEffect blurbefu;
	TowerEffect towerefu;
	CreditsEffect creditsefu(L"assets/credits.png");
	CreditsEffect logoefu(L"assets/logo.png");
	Sky sky;
	Wall wall;

	TimeLine timeline;

	LaserEffect spark(L"assets/spark.jpg");
	LaserEffect puu(L"assets/puu.jpg");

	FlashEffect flash;

	CurveMap p1,p2,p3;
	p1["camx"](2.0f,.0f,.0f)(15.0f,20.0f,.0f)(55.0f,-12.0f,.0f)(80.0f,-3.0f,-1.0f);
	p1["camy"](2.0f,.0f,.0f)(20.0f,13.0f,.0f)(80.0f,5.0f,-1.0f);
	p1["camz"](2.0f,15.0f,1.0f)(50.0f,20.0f,1.0f)(80.0f,200.0f,1.0f);
	p1["targetx"](2.0f,.0f,.0f)(71.0f,30.0f,1.0f)(80.0f,12.0f,-1.0f);
	p1["targety"](2.0f,.0f,.0f)(64.0f,3.0f,1.0f)(80.0f,30.0f,-1.0f);
	p1["targetz"](2.0f,-26.0f,.0f)(45.0f,-10.0f,1.0f)(80.0f,-2.0f,-1.0f);
	p1["primx"](.0f,1.0f,.0f);
	p1["primy"](.0f,1.0f,.0f);
	p1["primz"](.0f,1.0f,.0f);
	p1["primh"](.0f,1.0f,.0f);
	p1["prims"](.0f,.4f,.0f);
	p1["primv"](.0f,40.0f,-6.0f)(20.0f,.4f,.0f);
	p1["secx"](.0f,1.0f,.0f);
	p1["secy"](.0f,-1.0f,.0f);
	p1["secz"](.0f,-1.0f,.0f);
	p1["sech"](.0f,150.0f,.0f);
	p1["secs"](.0f,0.3f,.0f);
	p1["secv"](.0f,.3f,.0f);
	p1["aspect"](.0f,float(global_screenh)/float(global_screenw),.0f);
	p1["fov"](.0f,4.0f,.0f)(80.0f,8.0f,.0f);

	CurveMap insideCurves;
	insideCurves["lamp"](0.0, 0.0f, 0.0f)(5.0f, 1.0f, 1.0f);
	insideCurves["speed"](1.0, 0.5, 1.0f);

	CurveMap cityp;
	cityp["speed"](0.0, 0.1, 1.0f);
	cityp["lamp"](0.0, 0.0, 1.0f);

	CurveMap puuCurve;
	puuCurve["aspect"](.0f,float(global_screenh)/float(global_screenw),.0f);
	puuCurve["scale"](41.5f,1.0f,.0f)(41.7f,.99f,.0f)(41.9f,1.01f,.0f)(42.1f,1.0f,.0f);
	puuCurve["bright"](39.9f,1.0f,.0f)(40.0f,0.0f,.0f)(40.3f,2.0f,.0f)(40.4f,0.5f,.0f)(40.5f,1.5f,.0f)(40.8f,0.5f,.0f)(41.3f,0.1f,.0f)(41.5f, 1.0f, .0f)(60.0f, 1.0f, .0f)(64.0f,8.0f,.0f);
	puuCurve["ang"](.0f, 3.141592f, .0f);
	puuCurve["x"]()(.0f,.0,.0f);
	puuCurve["y"]()(.0f,-1.0f,.0f)(41.5f,.0,.04f)(64.0f,.2f,.0f);
	puuCurve["bright2"]()(41.3f,0.0f,.0f)(41.5f,1.0f,.0f);

	CurveMap puuFlash;
	puuFlash["aspect"](.0f,float(global_screenh)/float(global_screenw),.0f);
	puuFlash["scale"](38.3f,2.0f,.0f)(50.5f,7.0f,.0f);
	puuFlash["bright"](38.3f,0.0f,.0f)(41.5f,0.05f,.0f)(45.9f,0.0f,.0f);
	puuFlash["ang"](.0f, 3.141592f, .0f);
	puuFlash["x"]()(.0f,.0,.0f);
	puuFlash["y"]()(.0f,-1.0f,.0f)(41.5f,.0,.04f)(64.0f,.2f,.0f);
	puuFlash["bright2"]()(41.3f,0.0f,.0f)(41.5f,1.0f,.0f);

	CurveMap flashCurve;
	flashCurve["bright"](0.0f,1.0f,.0f)(0.1f,0.0f,.0f)(0.3f,2.0f,.0f)(0.4f,0.5f,.0f)(0.5f,1.5f,.0f)(0.8f,0.5f,.0f)(1.3f,0.0f,.0f);
	CurveMap flashCurve2;
	flashCurve["bright"](0.0f, 0.0f,.0f)(0.1f,2.0f,.0f)(0.3f,0.2f,.0f)(0.4f,0.5f,.0f)(0.5f,1.5f,.0f)(0.8f,0.5f,.0f)(1.3f,0.0f,.0f);

	CurveMap creducurvet;

	//p1["t"](0.0f,0.0f,1.0f)(40.0f,40.0f,1.0f);
	timeline.addEntry(0.0f, 80.0f, sky, p1);
	timeline.addEntry(0.0f, 80.0f, wall, p1);

	timeline.addEntry(80.0f, 144.0f, insideefu, cityp);
	timeline.addEntry(80.0f, 144.0f, puu, puuCurve);
	timeline.addEntry(80.0f, 144.0f, cityefu, p3);
	timeline.addEntry(80.0f, 144.0f, puu, puuFlash);
	timeline.addEntry(79.5f, 82.0f, flash, flashCurve);

	p2["r"](0.0f,1.0f,0.0f)(10.0f,0.0f,0.0f)(15.0f,1.0f,0.0f);
	p2["g"](0.0f,0.0f,0.0f)(10.0f,1.0f,0.0f)(15.0f,0.0f,0.0f);
	p2["b"](0.0f,1.0f,0.0f)(10.0f,0.0f,0.0f)(15.0f,1.0f,0.0f);
	timeline.addEntry(144.0f, 183.0f, vuoriefu, p2);
	timeline.addEntry(144.0f, 183.0f, nostatus, p2);

	timeline.addEntry(183.0f, 248.0f, insideefu, insideCurves);
	timeline.addEntry(183.0f, 248.0f, blurbefu, insideCurves);
	timeline.addEntry(183.0f + 16.0f, 183.0f + 16.0f + 12.0f, logoefu, insideCurves);
	timeline.addEntry(248.0f, 312.0f, insideefu, insideCurves);

		timeline.addEntry(247.0f, 252.0f, flash, flashCurve);	
	timeline.addEntry(248.0f, 312.0f, towerefu, p2);
	timeline.addEntry(312.0f, 500.0f, creditsefu, creducurvet);
	//timeline.addEntry(248.0f, 500.0f, dust, p1);
	//timeline.addEntry(248.0f, 248.0f, nostatus, p2);
	//timeline.addEntry(248.0f, 500.0f, dust, p1);
	
	////////////////////////////timeline.addEntry(248.0f, 500.0f, dust, p2);

	track.seekBeats(.0);
	track.play();
	
	t = track.getTime();

	PostProcess post(global_screenw, global_screenh);
	post.bind();
	while(win->loop()) {
		#ifndef _RELEASE
		glBeginQuery(GL_TIME_ELAPSED, query);
		#endif

		#ifdef _DEBUG
		if (win->keyHit[VK_F1] || win->keyHit[VK_F5]) {
			shaderstorage.reloadAll();
			post.bindUniforms();
		}

		if(win->keyDown[VK_LEFT])
			track.seekBeats(track.getBeats()-.5);
		if(win->keyDown[VK_RIGHT])
			track.seekBeats(track.getBeats()+.5);
		if(win->keyHit[0x31]) track.seekBeats(timeline.getBeginning(0));
		if(win->keyHit[0x32]) track.seekBeats(timeline.getBeginning(1));
		if(win->keyHit[0x33]) track.seekBeats(timeline.getBeginning(2));
		if(win->keyHit[0x34]) track.seekBeats(timeline.getBeginning(3));
		if(win->keyHit[0x35]) track.seekBeats(timeline.getBeginning(4));
		if(win->keyHit[0x36]) track.seekBeats(timeline.getBeginning(5));
		if(win->keyHit[0x37]) track.seekBeats(timeline.getBeginning(6));
		if(win->keyHit[0x38]) track.seekBeats(timeline.getBeginning(7));
		if(win->keyHit[0x39]) track.seekBeats(timeline.getBeginning(8));
		if(win->keyHit[0x40]) track.seekBeats(timeline.getBeginning(9));
		if(win->keyHit[0x30]) track.seekBeats(timeline.getBeginning(10));

		if (win->keyHit[VK_RETURN])
			track.toggle();

		if(win->keyHit[VK_SPACE]) {
			flymode = !flymode;
			if(flymode) {
				win->mousex = win->width/2;
				win->mousey = win->height/2;
				ShowCursor(0);
			}
			else
				ShowCursor(1);
		}
		#endif

		t = track.getBeats();

		timeline.render(t);

		post.render(t);

		#ifdef _DEBUG
		glEndQuery(GL_TIME_ELAPSED);
		glGetQueryObjectiv(query, GL_QUERY_RESULT, &res);

		if(!(loops%60)) {
			printf("frametime: %.2lfms\n", double(res)/1000000.0);
			printf("gl error: 0x%X\n", glGetError());
			printf("time: %f\n", t);
		}
		
		#endif

		loops++;

		#ifdef _RELEASE
		if (track.getTime() > track.getLength()-0.01) {
			break;
		}
		#endif
	}

	delete cityTexture;
	delete koneTexture;
	delete mountainTexture;

	return 0;
}
