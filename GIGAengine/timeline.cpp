
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
		e.effect.render(p);
	}
}

float TimeLine::getBeginning(int n) {
	if(n<(int)entries.size())
		return entries[n].start;
	else if(entries.size()>0)
		return entries[entries.size()-1].start;
	else
		return .0f;
}
