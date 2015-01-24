
#include "main.h"

song::song(std::wstring path): playing(false) {
	IBasicAudio * audioControl;
	__int64 trackLength;
	IGraphBuilder * graph;

	CoInitialize(0);
	CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&graph);

	graph->QueryInterface(IID_IMediaControl, (void**)&mediaControl);
	graph->QueryInterface(IID_IMediaSeeking, (void**)&mediaSeeking);
	graph->QueryInterface(IID_IBasicAudio  , (void**)&audioControl);
	
	HRESULT hr = graph->RenderFile(path.c_str(), 0);

	if(hr==S_OK)
		printf("Song succesfully rendered!\n");
	else
		printf("mp3 rendering failed :(\n");

	audioControl->put_Volume(long(0));

	audioControl->Release();
	graph->Release();

	mediaSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
	mediaSeeking->GetDuration(&trackLength);
	length = (long double)(trackLength)/(long double)10000000.0;

	__int64 position = 0;
	mediaSeeking->SetPositions(&position, AM_SEEKING_AbsolutePositioning, &position, AM_SEEKING_NoPositioning);
}

song::~song() {
	mediaControl->Release();
	mediaSeeking->Release();
	CoUninitialize();
}

int song::play() {
	mediaControl->Run();
	playing = true;
	return 0;
}

int song::pause() {
	mediaControl->Stop();
	playing = false;
	return 0;
}

int song::toggle() {
	playing = !playing;
	if(playing)
		play();
	else
		pause();
	return 0;
}

bool song::ended() {
	__int64 position, duration;
	mediaSeeking->GetCurrentPosition(&position);
	mediaSeeking->GetDuration(&duration);
	return position >= duration;
}

int song::seek(long double position) {
	
	if(position>length)
		position = length;
	if(position<.0)
		position = .0;
	
	__int64 seekPosition = __int64(10000000.0*position);
	
	if(playing)
		mediaControl->Stop();
	mediaSeeking->SetPositions(&seekPosition, AM_SEEKING_AbsolutePositioning, &seekPosition, AM_SEEKING_NoPositioning);
	if(playing)
		mediaControl->Run();

	return 0;
}

long double song::getTime() {
	__int64 position;
	mediaSeeking->GetCurrentPosition(&position);
	return (long double)(position)/(long double)10000000.0;
}
