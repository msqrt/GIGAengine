
#pragma once

class Effect {
	public:
		Effect() { }
		virtual ~Effect() { }
		Effect(const Effect&) { }
		const Effect& operator=(const Effect&) { }
		virtual void render(SyncMap& sync) = 0;
};
