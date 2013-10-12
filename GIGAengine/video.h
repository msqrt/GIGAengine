
#pragma once

#include "dshowbaseclass.h"
#include <comdef.h>

struct __declspec(uuid("{bf5129ce-01b3-4120-aae8-8d8e6e33d22a}")) peisikVideoSystem;

class textureGrabber : public CBaseVideoRenderer {
	public:
		textureGrabber(LPUNKNOWN lpUnknown, HRESULT *phResult);
		~textureGrabber();
		HRESULT CheckMediaType(const CMediaType *mediaType);
		HRESULT DoRenderSample(IMediaSample *pMediaSample);
		int setTexture(texture * texture);
		unsigned char * sampleData;
		int width, height, sampleSize, refreshed;
		texture * video;
};

class video {
	private:
		IMediaControl * mediaControl;
		IMediaSeeking * mediaSeeking;
		IBasicVideo * basicVideo;
		textureGrabber * grabber;
		long double length;
		bool rendered, isPlaying;

	public:
		texture texture;
		
		video(std::wstring path);
		~video();

		int isValid();
		int play();
		int pause();
		int toggle();
		int seek(long double position);
		long double getTime();
		long double getDuration();
		int hasNewFrame();
		int updateFrame(int slot = 0);
		int playing();
};
