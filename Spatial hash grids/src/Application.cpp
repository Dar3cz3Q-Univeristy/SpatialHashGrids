#include "pch.h"
#include "Header.h"

int main(int argc, char** argv) {
	// 0 - bruteforce O(n^2)
	// 1 - spatial hash grids
	int optimization_type = 1;

	std::string config_filename = "config/config.json";

	Map game(config_filename, optimization_type);

	game.initialize_jedis();
	game.initialize_artifacts();

	game.play();

	return 0;
}