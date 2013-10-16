
#pragma once

#include <vector>

class Curve {
	public:
		float value(float);
		Curve & operator()(float, float, float);
		Curve & operator()();
	private:
		struct CurveEntry {
			float time, value, tangent;
		};
		std::vector<CurveEntry> values;
};
