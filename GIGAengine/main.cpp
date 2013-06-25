
#include "resource.h"
#include "main.h"

#define screenw 1280
#define screenh 720

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show) {
//int main() {
	window win(screenw, screenh, 0, L"the pointillist chaos enhanced motion picture reproduction contraption by msqrt - enter to load, space to pause");
	shader simple(SIMPLE, SHADER), update(UPDATE, SHADER), spawn(SPAWN, SHADER), display(DISPLAY, SHADER), slider(SLIDER, SHADER);
	mesh quad(QUAD);

	video * clip = 0;
	
	texture particleDataOne  (2048, 1024, false, GL_NEAREST, GL_CLAMP_TO_BORDER),
			particleDataTwo  (2048, 1024, false, GL_NEAREST, GL_CLAMP_TO_BORDER),
			particleDataThree(2048, 1024, false, GL_NEAREST, GL_CLAMP_TO_BORDER),
			particleDataFour (2048, 1024, false, GL_NEAREST, GL_CLAMP_TO_BORDER);

	texture * pingDataOne = &particleDataOne  , * pingDataTwo = &particleDataTwo,
			* pongDataOne = &particleDataThree, * pongDataTwo = &particleDataFour;

	float * dat = new float[2048*1024*2];
	for(int i = 0; i<2048; i++)
	for(int j = 0; j<1024; j++) {
		dat[(i*1024+j)*2+0] = float(i)/2048.0f;
		dat[(i*1024+j)*2+1] = float(j)/1024.0f;
	}

	mesh particleMesh;
	particleMesh.data(0, 2048*1024, 2, dat);

	delete [] dat;
	
	target particleTarget, previousTarget;
	texture previousFrame(1,1,false,GL_LINEAR,GL_REPEAT);
	previousTarget.setTexture(GL_COLOR_ATTACHMENT0, &previousFrame, true);

	previousTarget.attach(1,1);
	glClear(GL_COLOR_BUFFER_BIT);
	previousFrame.bind(0);
	previousFrame.bind(1);

	DWORD start = GetTickCount();

	glDepthFunc(GL_LEQUAL);
	int rowCount = 0, k = 0;

	while(win.loop()) {

		if(win.keyHit[VK_RETURN]||k==0)
		{
			delete clip;
			
			wchar_t path[2048] = {0};
			OPENFILENAME fileName = {0};
			fileName.lStructSize = sizeof(OPENFILENAME);
			fileName.hInstance = GetModuleHandle(0);
			fileName.lpstrFilter = L"Video file\0*.avi;*.mp4;*.mpeg;*.mpg;*.mkv;*.mov;*.wmv\0Any file\0*.*\0";
			fileName.lpstrFile = path;
			fileName.nMaxFile = 2048;
			fileName.lpstrTitle = L"Open video";
			fileName.Flags = OFN_FILEMUSTEXIST|OFN_LONGNAMES;
			if(GetOpenFileName(&fileName))
				clip = new video(fileName.lpstrFile);
			else
				clip = 0;
			if(clip && clip->isValid()) {
				clip->play();
				while(!clip->hasNewFrame());
				clip->updateFrame();
				previousFrame.resize(clip->texture.width, clip->texture.height);
				previousTarget.setTexture(GL_COLOR_ATTACHMENT0, &clip->texture);
				glClear(GL_COLOR_BUFFER_BIT);
				previousTarget.attach(previousFrame.width, previousFrame.height);
				previousTarget.setTexture(GL_COLOR_ATTACHMENT0, &previousFrame);
				glClear(GL_COLOR_BUFFER_BIT);
			}
		}
		if(clip && clip->isValid()) {
			if(win.keyHit[VK_SPACE])
				clip->toggle();
			if(clip->hasNewFrame() && clip->playing()) {
				previousTarget.attach(previousFrame.width, previousFrame.height);
				simple.use();
				clip->texture.bind(0);
				glUniform1i(simple.getLoc("tex"),0);
				quad.draw(GL_TRIANGLES);
				clip->updateFrame();
				rowCount+=64;
				if(rowCount>=1024)
					rowCount = 0;
			}
			clip->texture.bind(0);
		}
		particleTarget.attach(2048, 1024);
		particleTarget.setTexture(GL_COLOR_ATTACHMENT0, pongDataOne);
		particleTarget.setTexture(GL_COLOR_ATTACHMENT1, pongDataTwo);
		
		float t = float(GetTickCount()-start)*.001f;

		update.use();
		pingDataOne->bind(2);
		pingDataTwo->bind(3);
		glUniform1i(update.getLoc("pos"), 2);
		glUniform1i(update.getLoc("col"), 3);
		glUniform1f(update.getLoc("t"), t);
		quad.draw(GL_TRIANGLES);

		spawn.use();

		glUniform1f(spawn.getLoc("row"), float(rowCount)/1024.0f*2.0f-1.0f);
		glUniform1f(spawn.getLoc("rowh"), 64.0f/1024.0f);
		glUniform1f(spawn.getLoc("t"), t);
		previousFrame.bind(1);
		glUniform1i(spawn.getLoc("tex1"), 0);
		glUniform1i(spawn.getLoc("tex2"), 1);

		quad.draw(GL_TRIANGLES);
		
		win.attach();
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		
		if(clip && clip->isValid()) {
			if(win.mousex>63 && win.mousex<1216 && win.mousey>647 && win.mousey<684 && win.mouseLeft)
				clip->seek(float(win.mousex-63)/float(1216-63)*clip->getDuration());
			slider.use();
			glUniform1f(slider.getLoc("t"), float(clip->getTime()/clip->getDuration()));
			glUniform1f(slider.getLoc("hover"), float(win.mousex>63 && win.mousex<1216 && win.mousey>647 && win.mousey<684));
			quad.draw(GL_TRIANGLES);
		}

		display.use();
		pongDataOne->bind(2);
		pongDataTwo->bind(3);
		glUniform1i(display.getLoc("pos"), 2);
		glUniform1i(display.getLoc("col"), 3);
		glUniform1f(display.getLoc("t"), t);
		glUniform1f(display.getLoc("aspect"), float(previousFrame.width)/float(previousFrame.height));

		glEnable(GL_DEPTH_TEST);
		particleMesh.draw(GL_POINTS);
		glDisable(GL_DEPTH_TEST);

		texture * temporaryTexture = pingDataOne;
		pingDataOne = pongDataOne;
		pongDataOne = temporaryTexture;

		temporaryTexture = pingDataTwo;
		pingDataTwo = pongDataTwo;
		pongDataTwo = temporaryTexture;

		k++;
	}

	delete clip;

	return 0;
}
