#pragma once
#include "../point/Point.h"
#include "../artifact/Artifact.h"

class Jedi : public Point {
	std::priority_queue<Artifact> artifacts_in_radius;
	int found_artifacts_number;
public:
	Jedi() : found_artifacts_number(0) {};
	Jedi(int id) : Point(id, 0, 0, "Jedi"), found_artifacts_number(0) {};
	Jedi(int id, double x, double y) : Point(id, x, y, "Jedi"), found_artifacts_number(0) {};
	~Jedi() {}
	bool check_distance(Artifact, double);
	double calculate_distance(Artifact);
	double calculate_distance_faster(Artifact);
	void update_x(double);
	void update_y(double);
	void add_artifact(Artifact);
	Artifact dequeue_artifact();
	bool artifacts_in_queue() const;
};