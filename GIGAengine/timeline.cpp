
#include "main.h"
#include "timeline.h"

void TimeLine::addEntry(float start, float end, Effect& effect) {
	TimeLineEntry t = {start, end, effect};
	entries.push_back(t);
}

void TimeLine::render(float t, SyncMap& sync) {
	sync["globaltime"] = t;
	for(auto e : entries)
		if(e.start<=t && e.end>=t) {
			sync["time"] = t-e.start;
			e.effect.render(sync);
		}
}
