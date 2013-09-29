
#include <algorithm>
#include "curve.h"

#define H1(x) (2.0f*(x)*(x)*(x)-3.0f*(x)*(x)+1.0f)
#define H2(x) ((x)*(x)*(x)-2.0f*(x)*(x)+(x))
#define H3(x) (-2.0f*(x)*(x)*(x)+3.0f*(x)*(x))
#define H4(x) ((x)*(x)*(x)-(x)*(x))

float Curve::value(float t) {
	for(int i = 0; i<(int)values.size(); i++) {
		if(values[i].time>t)
			return values[i].value;
		if(i == values.size()-1)
			return values[i].value;
		if(values[i+1].time>t) {
			float normt = float(t-values[i].time)/float(values[i+1].time-values[i].time);
			return H1(normt)*values[i  ].value +
				   H2(normt)*values[i  ].tangent*float(values[i+1].time-values[i].time) +
				   H3(normt)*values[i+1].value +
				   H4(normt)*values[i+1].tangent*float(values[i+1].time-values[i].time);
		}
	}
	return .0f;
}

Curve & Curve::operator()(float time, float value, float tangent) {
	CurveEntry c = {time, value, tangent};
	values.push_back(c);
	std::sort(values.begin(), values.end(), [](CurveEntry a, CurveEntry b) -> bool {return a.time < b.time;});
	return *this;
}
