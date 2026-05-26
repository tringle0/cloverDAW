#pragma once
#include <vector>

enum WaveForm {
	triangle, sawtooth, sine, square, noise
};

struct Envelope {
public:
	float attack = 0; //in ms
	float decay = 0; //in ms
	float sustain = 1; //in %
	float release = 0; //in ms
};

class Synth {
public:
	WaveForm waveform = sawtooth;
	Envelope envelope;
};

