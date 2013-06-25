
#ifndef SONG_H
#define SONG_H

class song {
	private:
		long double length;
		bool playing;
		IMediaControl * mediaControl;
		IMediaSeeking * mediaSeeking;
	public:
		song(std::wstring path);
		~song();
		int play();
		int pause();
		int toggle();
		int seek(long double position);
		long double getTime();
};

#endif