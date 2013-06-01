
#include "main.h"

#define screenw 1280
#define screenh 720

int main() {
	window win(screenw, screenh, 0, "GIGA");
	shader display("display.shader");
	mesh quad(QUAD);

	song track(L"assets/track.mp3");
	track.play();

	bool playing = false;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	while(win.loop()) {
		if(win.keyHit[VK_SPACE])
			track.toggle();

		if(win.keyHit[VK_LEFT])
			track.seek(track.getTime()-10.0);
		if(win.keyHit[VK_RIGHT])
			track.seek(track.getTime()+10.0);

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		display.use();
		quad.draw(GL_TRIANGLES);
	}
	
	return 0;
}
