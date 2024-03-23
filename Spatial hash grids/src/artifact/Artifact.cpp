#include "Artifact.h"
#include "pch.h"

void Artifact::update_x(double x, Point* A, Point* B, double D) {
	if (abs(x) > D) return;
	double temp = this->x + x;
	if (temp > B->get_x() || temp < A->get_x()) return;
	this->x = temp;
};

void Artifact::update_y(double y, Point* A, Point* B, double D) {
	if (abs(y) > D) return;
	double temp = this->y + y;
	if (temp > B->get_y() || temp < A->get_y()) return;
	this->y = temp;
}

std::string Artifact::get_hash() {
	return std::to_string((int)x) + "." + std::to_string((int)y);
}

bool operator<(const Artifact& a1, const Artifact& a2) {
	return a1.ID > a2.ID;
}