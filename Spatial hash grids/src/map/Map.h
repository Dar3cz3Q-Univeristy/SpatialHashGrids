#pragma once
#include "../jedi/Jedi.h"
#include "../artifact/Artifact.h"
#include "../point/Point.h"
#include "json.h"

class Map
{
	// General
private:
	double H;
	double D;
	int SEARCHING_TYPE;
	int N;
	Point* A, * B;
	unsigned short int NUMBER_OF_STEPS;
	std::string JEDIS_POSITIONS;
	std::string ARTIFACTS_STARTING_POSITIONS;
	std::string ARTIFACTS_POSITIONS;
	std::string JEDIS_OUTPUT_INSTRUCTIONS;
	bool is_map_initialized;
	bool is_jedis_initialized;
	bool is_artifacts_initialized;
	unsigned short int current_step;
public:
	Map() = default;
	Map(std::string, int);
	~Map() {
		if (A) {
			delete A;
		}
		if (B) {
			delete B;
		}
		jedis_location.clear();
		artifacts_location.clear();
		artifacts_location_1.clear();
		artifacts_location_ptr.clear();
	};
	int initialize_jedis();
	int initialize_artifacts();
	void play();
	friend class Point;
	friend class Jedi;
	friend class Artifact;
private:
	std::map<int, Jedi> jedis_location;
	void show_end_info() const;
	void next_step();
	bool check_if_initialized(bool) const;
	bool check_if_initialized() const;
	bool check_given_jedis_locations();
	std::string prepare_filename(std::string);
	void find_artifacts_for_jedis(int);
	int save_found_artifacts();
	int update_jedis();
	int update_artifacts();
	// Bruteforce
private:
	std::map<int, Artifact> artifacts_location;
	void initialize_artifacts_0();
	void update_artifacts_0();
	int algorithm_bruteforce(unsigned int*);
	// Optimized
private:
	std::map<std::string, std::map<int, Artifact>> artifacts_location_1;
	std::map<int, Artifact*> artifacts_location_ptr;
	void generate_all_hashes(std::vector<std::string>*, double, double);
	template<typename T>
	std::string generate_hash(T x, T y) {
		return std::to_string((int)x) + "." + std::to_string((int)y);
	}
	void initialize_artifacts_1();
	void update_artifacts_1();
	int algorithm_optimized(unsigned int*);
	//Optional
private:
	void find_artifacts_for_jedi(unsigned int*, int);
};

