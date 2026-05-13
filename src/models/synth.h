#pragma once
#include <vector>

enum WaveForm {
	triangle, sawtooth, sine, square, noise
};

class IEffect {
	virtual void apply();
};

struct Envelope {
public:
	double attack; //in ms
	double decay; //in ms
	double sustain; //in %
	double release; //in ms
};

class Synth {
	WaveForm waveform;
	Envelope envelope;

	double loudness;
	double detune;

	std::vector<IEffect> effects;
};

