
#include "main.h"
#include "timeline.h"
#include "effect.h"

void TimeLine::addEntry(float start, float end, Effect& effect) {
	CurveMap p;
	addEntry(start, end, effect, p);
}

void TimeLine::addEntry(float start, float end, Effect& effect, CurveMap& p) {
	TimeLineEntry t = {start, end, effect, p};
	entries.push_back(t);
}

void TimeLine::render(float t) {
	for(auto e : entries)
	if(e.start<=t && e.end>=t) {
		ParameterMap p;
		for(auto i : e.parameters)
			p[i.first] = i.second.value(t-e.start);

		p["t"] = t - e.start;
		e.effect.render(p);
	}
}

// returns the #n unique effect start time
float TimeLine::getBeginning(int n) {
	int distinct_times = 0;

	if(n<(int)entries.size()) {
		if (n == 0) 
			return entries[n].start;

		for (int i=1;i<(int)entries.size();i++) {
			if (entries[i].start != entries[i-1].start) {
				distinct_times++; // increase distinct_times by one
			}

			if (distinct_times == n) {
				return entries[i].start;
			}

		}

		return entries[n].start;
	} else if(entries.size()>0) {
		return entries[entries.size()-1].start;
	} else {
		return .0f;
	}
}
