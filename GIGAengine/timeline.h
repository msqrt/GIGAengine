
#pragma once

#include <vector>
#include "effect.h"

class TimeLine {
	public:
		void addEntry(float start, float end, Effect& effect);
		void render(float t, SyncMap& sync);

	private:
		
		struct TimeLineEntry {
				float start, end;
				Effect& effect;
		};

		std::vector<TimeLineEntry> entries;
};
