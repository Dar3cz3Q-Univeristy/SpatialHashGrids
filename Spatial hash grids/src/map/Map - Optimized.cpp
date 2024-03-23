#include "Map.h"
#include "pch.h"

void Map::initialize_artifacts_1()
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
		std::string hash = generate_hash(x, y);
		artifacts_location_1[hash][id] = temp;
		artifacts_location_ptr[id] = &artifacts_location_1[hash][id];
	}

	file.close();

	this->is_artifacts_initialized = true;
}

void Map::update_artifacts_1()
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
		std::string last_hash = artifacts_location_ptr[id]->get_hash();
		artifacts_location_ptr[id]->update_x(x, A, B, D);
		artifacts_location_ptr[id]->update_y(y, A, B, D);
		std::string new_hash = artifacts_location_ptr[id]->get_hash();
		if (last_hash != new_hash) {
			artifacts_location_1[new_hash].insert(std::make_pair(id, *artifacts_location_ptr[id]));
			artifacts_location_1[last_hash].erase(id);
			artifacts_location_ptr[id] = &artifacts_location_1[new_hash][id];
		}
	}

	file.close();
}

int Map::algorithm_optimized(unsigned int* iterations) {
	auto start = std::chrono::high_resolution_clock::now();
	unsigned int counter = 0;

	double length = pow(H, 2);

	for (auto& jedi : jedis_location) {
		double x = jedi.second.get_x();
		double y = jedi.second.get_y();
		std::vector<std::string> hashes_to_check;
		generate_all_hashes(&hashes_to_check, x, y);
		for (auto& hash : hashes_to_check) {
			for (auto& artifact : artifacts_location_1[hash]) {
				if (jedi.second.check_distance(artifact.second, H)) continue;
				double distance = jedi.second.calculate_distance_faster(artifact.second);
				if (distance <= length) {
					jedi.second.add_artifact(artifact.second);
				}
				counter++;
			}
		}
	}

	auto stop = std::chrono::high_resolution_clock::now();
	unsigned int duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	*iterations = counter;

	return duration;
}

void Map::find_artifacts_for_jedi(unsigned int* counter, int k) {
	int half_of_jedi = jedis_location.size() / 2;
	double length = pow(H, 2);
	Jedi* jedi = nullptr;
	for (int i = k; i < half_of_jedi + k; i++) {
		jedi = &jedis_location.at(i + 1);
		double x = jedi->get_x();
		double y = jedi->get_y();
		std::vector<std::string> hashes_to_check;
		generate_all_hashes(&hashes_to_check, x, y);
		for (auto& hash : hashes_to_check) {
			for (auto& artifact : artifacts_location_1[hash]) {
				if (jedi->check_distance(artifact.second, H)) continue;
				double distance = jedi->calculate_distance_faster(artifact.second);
				if (distance <= length) {
					jedi->add_artifact(artifact.second);
				}
				(*counter)++;
			}
		}
	}
}

void Map::generate_all_hashes(std::vector<std::string>* hashes, double x, double y) {
	int start_x = static_cast<int>(x - H);
	int start_y = static_cast<int>(y - H);
	int end_x = static_cast<int>(x + H);
	int end_y = static_cast<int>(y + H);
	for (int i = start_x; i <= end_x; i++) {
		for (int j = start_y; j <= end_y; j++) {
			if (i >= A->get_x() && i < B->get_x() && j >= A->get_y() && j < B->get_y()) {
				hashes->push_back(generate_hash(i, j));
			}
		}
	}
}