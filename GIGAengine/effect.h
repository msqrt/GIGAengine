
#pragma once
#include "timeline.h"

class Effect {
	public:
		Effect() { }
		virtual ~Effect() { }
		Effect(const Effect&) { }
		const Effect& operator=(const Effect&) { }
		virtual void render(ParameterMap& param) = 0;
};
