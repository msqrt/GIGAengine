
#pragma once

class song {
	private:
		long double length;
		double BPM;
		bool playing;
		IMediaControl * mediaControl;
		IMediaSeeking * mediaSeeking;
	public:
		song(std::wstring path, double BPM);
		~song();
		int play();
		int pause();
		int toggle();
		int seekTime(long double position);
		int seekBeats(long double position);
		long double getTime();
		long double getBeats();
		long double getLength();
};
