
#pragma once

#include <map>
#include "curve.h"

typedef std::map<std::string, Curve> CurveMap;
typedef std::map<std::string, float> ParameterMap;

class Effect;

class TimeLine {
	public:
		void addEntry(float start, float end, Effect& effect);
		void addEntry(float start, float end, Effect& effect, CurveMap& p);
		void render(float t);
		float getBeginning(int n);
	private:
		
		struct TimeLineEntry {
				float start, end;
				Effect& effect;
				CurveMap parameters;
		};

		std::vector<TimeLineEntry> entries;
};
