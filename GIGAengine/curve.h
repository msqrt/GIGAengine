
#pragma once

#include <vector>

class Curve {
	public:
		float value(float);
		Curve & operator()(float, float, float);
	private:
		struct CurveEntry {
			float time, value, tangent;
		};
		std::vector<CurveEntry> values;
};
