#pragma once
#include "../point/Point.h"

class Artifact : public Point
{
public:
	Artifact() = default;
	Artifact(int id, double x, double y) : Point(id, x, y, "Artefakt") {};
	~Artifact() {};
	void update_x(double, Point*, Point*, double);
	void update_y(double, Point*, Point*, double);
	std::string get_hash();

	friend class Jedi;
	friend class Map;

	friend bool operator<(const Artifact&, const Artifact&);
};



