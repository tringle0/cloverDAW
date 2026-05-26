#include "song.h"
#include <cmath>
#include "layer.h"
#include "note.h"

int Song::lengthBeats() {
	float maxBeat = 0;
	for (Layer* l : layers) {
		for (Note n : l->notes) {
			maxBeat = std::max(maxBeat, n.start + n.length);
		}
	}
	return (int)std::ceil(maxBeat);
}