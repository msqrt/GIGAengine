
#include "main.h"
#include <comdef.h>

#pragma comment(lib, "strmbase.lib")
#pragma comment(lib, "winmm.lib")

HRESULT textureGrabber::CheckMediaType(const CMediaType *mediaType) {
	width = 0;
	if(*mediaType->Type() != MEDIATYPE_Video || *mediaType->Subtype() != MEDIASUBTYPE_RGB24 || !mediaType->IsValid())
		return S_FALSE;
	
	if(((VIDEOINFOHEADER*)mediaType->pbFormat)->bmiHeader.biCompression!=BI_RGB||
	   ((VIDEOINFOHEADER*)mediaType->pbFormat)->bmiHeader.biBitCount!=24)
	   return S_FALSE;
	if(mediaType->formattype==FORMAT_VideoInfo) {
		width  = ((VIDEOINFOHEADER*)mediaType->pbFormat)->bmiHeader.biWidth;
		height = ((VIDEOINFOHEADER*)mediaType->pbFormat)->bmiHeader.biHeight;
	}
	if(mediaType->formattype==FORMAT_MPEG2Video) {
		width = ((MPEG1VIDEOINFO*)mediaType->pbFormat)->hdr.bmiHeader.biWidth;
		height = ((MPEG1VIDEOINFO*)mediaType->pbFormat)->hdr.bmiHeader.biHeight;
	}
	if(!width)
		return S_FALSE;
	printf("Video resolution: %dx%d\n",width,height);
	return S_OK;
}

HRESULT textureGrabber::DoRenderSample(IMediaSample * mediaSample) {
	if(sampleSize != mediaSample->GetActualDataLength()) {
		delete [] sampleData;
		sampleSize = mediaSample->GetActualDataLength();
		sampleData = new unsigned char[mediaSample->GetActualDataLength()];
	}
	unsigned char * temporaryData;
	mediaSample->GetPointer(&temporaryData);
	memcpy(sampleData, temporaryData, sampleSize);
	refreshed = 1;
	return S_OK;
}

textureGrabber::textureGrabber(LPUNKNOWN lpUnknown, HRESULT *phResult) : CBaseVideoRenderer(__uuidof(peisikVideoSystem), NAME("peisikVideoSystem"), lpUnknown, phResult) {
	refreshed = 0;
	sampleData = 0;
	sampleSize = 0;
	Ready();
}

textureGrabber::~textureGrabber() {
	delete [] sampleData;
}

int textureGrabber::setTexture(texture * texture) {
	video = texture;
	return 0;
}

video::video(std::wstring path): texture(1,1,false,GL_LINEAR,GL_REPEAT), rendered(false), isPlaying(false) {
	printf("Opening video \"%ls\"\n", path.c_str());
	IGraphBuilder * graph;
	IBaseFilter * base;
	__int64 clipLength;

	CoInitialize(0);
	CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&graph);
	
	HRESULT hr = S_OK;

	graph->QueryInterface(IID_IMediaControl, (void**)&mediaControl);
	graph->QueryInterface(IID_IMediaSeeking, (void**)&mediaSeeking);
	
	grabber = new textureGrabber(0, &hr);
	grabber->AddRef();
	grabber->QueryInterface(IID_IBaseFilter, (void**)&base);
	graph->AddFilter(base, L"peisikVideoSystem OpenGL texture renderer");

	hr = graph->RenderFile(path.c_str(), 0);

	grabber->setTexture(&texture);
	graph->Release();
	base->Release();

	if(SUCCEEDED(hr) && grabber->width)
	{
		printf("Video succesfully rendered\n");
		mediaSeeking->SetTimeFormat(&TIME_FORMAT_MEDIA_TIME);
		mediaSeeking->GetDuration(&clipLength);
		length = (long double)(clipLength)/(long double)10000000.0;

		__int64 position = 0;
		mediaSeeking->SetPositions(&position, AM_SEEKING_AbsolutePositioning, &position, AM_SEEKING_NoPositioning);

		rendered = true;
	}
	else
		printf("Couldn't find a working video graph\n");
}

video::~video() {
	mediaControl->Release();
	mediaSeeking->Release();
	grabber->Release();
	CoUninitialize();
}

int video::isValid() {
	return rendered;
}

int video::play() {
	if(!isValid())
		return 1;

	mediaControl->Run();
	isPlaying = true;
	return 0;
}

int video::pause() {
	if(!isValid())
		return 1;

	mediaControl->Stop();
	isPlaying = false;
	return 0;
}

int video::toggle() {
	if(!isValid())
		return 1;

	isPlaying = !isPlaying;
	if(isPlaying)
		play();
	else
		pause();
	return 0;
}

int video::seek(long double position) {
	if(!isValid())
		return 1;

	if(position>length)
		position = length;
	if(position<.0)
		position = .0;
	
	__int64 seekPosition = __int64(10000000.0*position);
	
	mediaControl->Stop();
	mediaSeeking->SetPositions(&seekPosition, AM_SEEKING_AbsolutePositioning, &seekPosition, AM_SEEKING_NoPositioning);
	if(isPlaying)
		mediaControl->Run();

	return 0;
}

long double video::getTime() {
	if(!isValid())
		return 1;

	__int64 position;
	mediaSeeking->GetCurrentPosition(&position);
	return (long double)(position)/(long double)10000000.0;
}

long double video::getDuration() {
	if(!isValid())
		return .0;
	else
		return length;
}

int video::hasNewFrame() {
	if(!isValid())
		return 0;
	else
		return grabber->refreshed;
}

int video::updateFrame(int slot) {
	if(!isValid())
		return 1;

	if(grabber->sampleSize) {
		if(grabber->width!=grabber->video->width||grabber->height!=grabber->video->height)
			grabber->video->resize(grabber->width, grabber->height);
		texture.bind(slot);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, texture.width, texture.height, 0, GL_BGR, GL_UNSIGNED_BYTE, grabber->sampleData);
	}
	grabber->refreshed = 0;
	return 0;
}

int video::playing() {
	if(!isValid())
		return 0;
	else
		return isPlaying;
}