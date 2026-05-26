#pragma once
#include <vector>

class IEffect {
public:
	//returns new amp freq pair
	virtual std::pair<float, float> apply(float amp, float freq) { return { amp, freq }; };
	virtual void render() {};
};