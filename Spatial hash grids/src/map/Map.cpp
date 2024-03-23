#include "Map.h"
#include "pch.h"

Map::Map(std::string config_filename, int type) : current_step(0), H(0), D(0), N(0), A(nullptr), B(nullptr), NUMBER_OF_STEPS(0), JEDIS_POSITIONS(""), ARTIFACTS_POSITIONS(""), ARTIFACTS_STARTING_POSITIONS(""), JEDIS_OUTPUT_INSTRUCTIONS(""), is_jedis_initialized(false), is_artifacts_initialized(false), is_map_initialized(false), SEARCHING_TYPE(type)
{
	std::ifstream configFile(config_filename, std::ifstream::binary);

	if (!configFile) {
		std::cout << "Blad otwierania pliku konfiguracyjnego!" << std::endl;
		return;
	}

	Json::Value config;
	configFile >> config;

	configFile.close();

	this->H = config["map"]["h"].asDouble();
	this->D = config["map"]["d"].asDouble();
	this->N = config["map"]["n"].asInt();

	this->A = new Point(config["map"]["grid"]["A"]["id"].asInt(), config["map"]["grid"]["A"]["x"].asDouble(), config["map"]["grid"]["A"]["y"].asDouble(), config["map"]["grid"]["A"]["type"].asString());
	this->B = new Point(config["map"]["grid"]["B"]["id"].asInt(), config["map"]["grid"]["B"]["x"].asDouble(), config["map"]["grid"]["B"]["y"].asDouble(), config["map"]["grid"]["B"]["type"].asString());

	this->NUMBER_OF_STEPS = config["map"]["number_of_steps"].asInt();

	this->JEDIS_POSITIONS = config["file"]["jedi"]["jedi_input_folder"].asString() + "/" + config["file"]["jedi"]["jedi_filename_schema"].asString();

	this->ARTIFACTS_STARTING_POSITIONS = config["file"]["artifact"]["artifact_input_folder"].asString() + "/" + config["file"]["artifact"]["artifact_starting_position_filename"].asString();

	this->ARTIFACTS_POSITIONS = config["file"]["artifact"]["artifact_input_folder"].asString() + "/" + config["file"]["artifact"]["artifact_filename_schema"].asString();

	this->JEDIS_OUTPUT_INSTRUCTIONS = config["file"]["output"]["output_folder"].asString() + "/" + config["file"]["output"]["output_filename_schema"].asString();

	this->is_map_initialized = true;
};

int Map::initialize_jedis()
{
	if (!check_if_initialized(true)) {
		return -1;
	}

	for (int i = 0; i < 10; i++) {
		// Inicjowanie na podstawie danych z pliku
		int id = i + 1;
		Jedi temp(id);
		jedis_location[id] = temp;
	}

	this->is_jedis_initialized = true;

	return 0;
}

int Map::initialize_artifacts()
{
	if (!check_if_initialized(true)) {
		return -1;
	}

	switch (SEARCHING_TYPE) {
	case 0:
		initialize_artifacts_0();
		break;
	case 1:
		initialize_artifacts_1();
		break;
	default:
		return -1;
		break;
	}

	return 0;
}

int Map::update_jedis()
{
	std::string fileName = prepare_filename("Jedi");

	std::ifstream file;
	file.open(fileName);

	if (!file.good()) {
		std::cout << "Nie udalo sie otworzyc pliku: " << fileName << std::endl;
		return -1;
	}

	int id;
	double x, y;

	while (file >> id >> x >> y) {
		jedis_location[id].update_x(x);
		jedis_location[id].update_y(y);
	}

	file.close();

	return 0;
}

int Map::update_artifacts()
{
	switch (SEARCHING_TYPE) {
	case 0:
		update_artifacts_0();
		break;
	case 1:
		update_artifacts_1();
		break;
	default:
		return -1;
		break;
	}

	return 0;
}

bool Map::check_if_initialized(bool x) const
{
	if (!this->is_map_initialized) {
		std::cout << "Mapa nie zainicjalizowana. Mozliwy blad w pliku konfiguracyjnym" << std::endl;
		return false;
	}
	return true;
}

bool Map::check_if_initialized() const
{
	if (!this->is_map_initialized) {
		std::cout << "Mapa nie zainicjalizowana. Mozliwy blad w pliku konfiguracyjnym" << std::endl;
		return false;
	}

	if (!this->is_jedis_initialized) {
		std::cout << "Jedi nie zainicjalizowane. Uzyj funkcji 'initialize_jedis()'" << std::endl;
		return false;
	}

	if (!this->is_artifacts_initialized) {
		std::cout << "Artefakty nie zainicjalizowane. Uzyj funkcji 'initialize_artifacts()'" << std::endl;
		return false;
	}

	return true;
}

void Map::play()
{
	if (!check_if_initialized()) {
		return;
	}

	for (int i = 1; i <= this->NUMBER_OF_STEPS; i++) {

		next_step();

		//system("pause");
	}

	show_end_info();
}

void Map::next_step()
{
	this->current_step++;

	bool is_location_needed = check_given_jedis_locations();

	update_artifacts();

	if (is_location_needed) {
		update_jedis();
		find_artifacts_for_jedis(this->SEARCHING_TYPE);
		save_found_artifacts();
	}

	std::cout << "Krok " << this->current_step << " zostal wykonany. Czy przejsc do nastepnego?\n";
}

bool Map::check_given_jedis_locations()
{
	std::string fileName = prepare_filename("Jedi");
	std::ifstream file;
	file.open(fileName);
	return file.good();
}

std::string Map::prepare_filename(std::string type)
{
	std::string temp;
	if (type == "Jedi") {
		temp = this->JEDIS_POSITIONS;
	}
	else if (type == "Artifacts") {
		temp = this->ARTIFACTS_POSITIONS;
	}
	else {
		temp = this->JEDIS_OUTPUT_INSTRUCTIONS;
	}
	temp.replace(temp.find("$"), 1, std::to_string(this->current_step));
	return temp;
}

void Map::find_artifacts_for_jedis(int type)
{
	int elapsed_time = 0;
	unsigned int iterations = 0;
	std::string name;

	switch (type) {
	case 0:
		elapsed_time = algorithm_bruteforce(&iterations);
		name = "bruteforce";
		break;
	case 1:
		elapsed_time = algorithm_optimized(&iterations);
		name = "optimized";
		break;
	default:
		name = "ERROR";
		break;
	}

	std::string fileName = "logs/time.log";
	std::fstream file;

	file.open(fileName, std::ios::app);
	file << "algorithm: " << name << ", elapsed time: " << elapsed_time << " microseconds, loop iterations: " << iterations << ", current_step: " << this->current_step << std::endl;
	file.close();

	fileName = "logs/benchmark.log";
	file.open(fileName, std::ios::app);
	file << elapsed_time << std::endl;
	file.close();
}

int Map::save_found_artifacts()
{
	std::string fileName = prepare_filename("Instructions");

	std::ofstream file;
	file.open(fileName);

	if (!file.good()) {
		std::cout << "Nie udalo sie otworzyc pliku: " << fileName << std::endl;
		return -1;
	}

	file << "Ustawiony dystans: " << this->H << std::endl;
	file << "------------------------------------------------------------------------\n";

	for (auto& j : jedis_location) {
		file << j.second;
		file << "\nArtefakty w jego poblizu: \n\n";
		while (j.second.artifacts_in_queue()) {
			file << j.second.dequeue_artifact();
		}
		file << "------------------------------------------------------------------------\n";
	}

	file.close();

	return 0;
}

void Map::show_end_info() const
{
	std::cout << "\nDane zostaly zapisane w plikach tekstowych o schemacie: " << this->JEDIS_OUTPUT_INSTRUCTIONS << std::endl;
}