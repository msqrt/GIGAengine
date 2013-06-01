
#include "main.h"

song::song(std::wstring path): playing(false) {
	IPin * pin;
	IBaseFilter * base;
	IBasicAudio * audioControl;
	__int64 songDuration;
	IGraphBuilder * graph;

	CoInitialize(0);
	CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&graph);

	graph->QueryInterface(IID_IMediaControl, (void**)&mediaControl);
	graph->QueryInterface(IID_IMediaSeeking, (void**)&mediaSeeking);
	graph->QueryInterface(IID_IBasicAudio  , (void**)&audioControl);

	graph->AddSourceFilter(path.c_str(), L"mp3", &base);
	base->FindPin(L"Output", &pin);
	HRESULT hr = graph->Render(pin);

	if(hr==S_OK)
		printf("Song succesfully rendered!\n");
	else
		printf("mp3 rendering failed :(\n");

	audioControl->put_Volume(long(0));

	pin->Release();
	base->Release();
	audioControl->Release();
	graph->Release();

	mediaSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
	mediaSeeking->GetDuration(&songDuration);
	songLength = (long double)(songDuration)/(long double)10000000.0;

	__int64 position = 0;
	mediaSeeking->SetPositions(&position, AM_SEEKING_AbsolutePositioning, &position, AM_SEEKING_NoPositioning);
}

song::~song() {
	mediaControl->Release();
	mediaSeeking->Release();
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

int song::seek(long double position) {
	
	if(position>songLength)
		position = songLength;
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
