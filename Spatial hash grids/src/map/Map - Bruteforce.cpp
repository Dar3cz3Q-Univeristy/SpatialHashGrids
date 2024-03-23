#include "Map.h"
#include "pch.h"

void Map::initialize_artifacts_0()
{
	std::ifstream file;
	file.open(this->ARTIFACTS_STARTING_POSITIONS);

	if (!file.good()) {
		std::cout << "Nie udalo sie otworzyc pliku: " << this->ARTIFACTS_STARTING_POSITIONS << std::endl;
		return;
	}

	int id;
	double x, y;

	while (file >> id >> x >> y) {
		Artifact temp(id, x, y);
		this->artifacts_location[id] = temp;
	}

	file.close();

	this->is_artifacts_initialized = true;
}

void Map::update_artifacts_0()
{
	std::string fileName = prepare_filename("Artifacts");

	std::ifstream file;
	file.open(fileName);

	if (!file.good()) {
		std::cout << "Nie udalo sie otworzyc pliku: " << fileName << std::endl;
		return;
	}

	int id;
	double x, y;

	while (file >> id >> x >> y) {
		this->artifacts_location[id].update_x(x, A, B, D);
		this->artifacts_location[id].update_y(y, A, B, D);
	}

	file.close();
}

int Map::algorithm_bruteforce(unsigned int* iterations) {
	unsigned int counter = 0;
	auto start = std::chrono::high_resolution_clock::now();

	for (auto& jedi : jedis_location) {
		for (auto& artifact : artifacts_location) {
			double distance = jedi.second.calculate_distance(artifact.second);
			if (distance <= H) {
				jedi.second.add_artifact(artifact.second);
			}
			counter++;
		}
	}

	auto stop = std::chrono::high_resolution_clock::now();
	unsigned int duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	*iterations = counter;

	return duration;
}