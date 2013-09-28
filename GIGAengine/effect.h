
#pragma once

class Effect {
	public:
		Effect() { }
		virtual ~Effect() { }
		Effect(const Effect&) { }
		const Effect& operator=(const Effect&) { }
		virtual void render(float t) = 0;
};
