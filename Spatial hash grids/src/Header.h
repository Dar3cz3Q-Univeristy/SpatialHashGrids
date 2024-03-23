#pragma once
#include "artifact/Artifact.h"
#include "jedi/Jedi.h"
#include "map/Map.h"

template<typename T>
void print(T arr) {
	int n = arr.size();
	for (int i = 0; i < n - 1; i++) {
		std::cout << arr.at(i);
	}
}

