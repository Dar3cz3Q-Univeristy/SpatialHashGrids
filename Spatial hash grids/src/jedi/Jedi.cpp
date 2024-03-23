#include "Jedi.h"
#include "pch.h"

void Jedi::update_x(double x) {
	this->x = x;
};

void Jedi::update_y(double y) {
	this->y = y;
}

bool Jedi::check_distance(Artifact a, double H) {
	if (abs(x - a.x) > H) {
		return true;
	}

	if (abs(y - a.y) > H) {
		return true;
	}

	return false;
}

double Jedi::calculate_distance(Artifact a) {
	return sqrt(pow(x - a.x, 2) + pow(y - a.y, 2));
}

double Jedi::calculate_distance_faster(Artifact a) {
	return (pow(x - a.x, 2) + pow(y - a.y, 2));
}

void Jedi::add_artifact(Artifact a) {
	artifacts_in_radius.push(a);
	found_artifacts_number++;
}

Artifact Jedi::dequeue_artifact() {
	Artifact temp = artifacts_in_radius.top();
	artifacts_in_radius.pop();
	found_artifacts_number--;
	return temp;
}

bool Jedi::artifacts_in_queue() const {
	return found_artifacts_number > 0;
}