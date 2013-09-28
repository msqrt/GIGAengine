
#ifndef WINDOW_H
#define WINDOW_H

class window {
	private:
		HDC deviceContext; HGLRC renderingContext;
		unsigned long gdiToken;
		DWORD startTime, frameStartTime; int fps, frameCount;
		std::wstring title;
		int defaultid;
	public:
		HWND windowHandle; 
		bool fullScreen;
		window(int width, int height, bool fullScreen, std::wstring title);
	   ~window();
		bool keyDown[256], keyHit[256];
		int mousex, mousey, mouseLeft, mouseRight;
		int width, height, bitsPerPixel;
		//use like while(instance->loop()){/* main loop code */}
		int loop();
		int attach(int id = -1);
		int setDefaultTarget(int id);
		double time();
};

#endif
