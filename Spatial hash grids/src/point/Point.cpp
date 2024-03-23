#include "Point.h"
#include "pch.h"

void Point::set_x(double x) {
	this->x = x;
}

void Point::set_y(double y) {
	this->y = y;
}

std::ostream& operator<<(std::ostream& os, const Point& p1) {
	os << p1.type << ", ID: " << p1.ID << std::setprecision(16) << ", x: " << p1.x << ", y: " << p1.y << std::endl;
	return os;
}

double Point::get_x() {
	return x;
}

double Point::get_y() {
	return y;
}
