
#ifndef WINDOW_H
#define WINDOW_H

class window {
	private:
		HWND windowHandle; HDC deviceContext; HGLRC renderingContext;
		int width, height, bitsPerPixel;
		unsigned long gdiToken;
		bool fullScreen;
		DWORD startTime; int fps, frameCount;
		char title[512];
	public:
		window(int width, int height, bool fullScreen, char * title);
	   ~window();
		bool keyDown[256], keyHit[256];
		int mousex, mousey, mouseLeft, mouseRight;
		//use like while(instance->loop()){/* main loop code */}
		int loop();
		int attach();
};

#endif
