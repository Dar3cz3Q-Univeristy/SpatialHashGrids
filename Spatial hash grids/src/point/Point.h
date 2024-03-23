#pragma once

class Point
{
protected:
	int ID;
	double x;
	double y;
	std::string type;
public:
	Point() : ID(-1), x(-1), y(-1) {};
	Point(int id, double x, double y, std::string type) : ID(id), x(x), y(y), type(type) {};
	~Point() {};
	virtual void update_x(double) {};
	virtual void update_y(double) {};
	void set_x(double);
	void set_y(double);
	double get_x();
	double get_y();
	friend std::ostream& operator<<(std::ostream& os, const Point& p1);
	friend class Map;
};

