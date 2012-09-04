#include "coordinate.h"


Coordinate::Coordinate(double x, double y) {
	this->set(x, y);
}

void Coordinate::set(double x, double y) {
	this->x = x;
	this->y = y;
}
std::vector<double> Coordinate::get() {
	std::vector<double> output;
	output.reserve(2);
	output.push_back(x);
	output.push_back(y);
	return output;
}

void Coordinate::set_x(double x) {
	this->x = x;
}

void Coordinate::set_y(double y) {
	this->y = y;
}

double Coordinate::get_x() {
	return this->x;
}

double Coordinate::get_y() {
	return this->y;
}
