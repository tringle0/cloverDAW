#pragma once
#include <unordered_set>

class UniqueCounter {
private:
	const int startNumber = 0;
	const int maxValue = 1000000;
	std::unordered_set<int> counted;
public:
	int produceUnique() {
		for (int k = startNumber; k < maxValue; k++) {
			if (counted.count(k) == 0) {
				counted.insert(k);
				return k;
			}
			
		}
		return 0;
	}
	void remove(int value) {
		counted.erase(value);
	}
};