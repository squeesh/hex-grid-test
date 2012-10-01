#include "includes.h"


Vertex::Vertex(double red, double green, double blue, double height) {
	this->color = new std::vector< double >();
	this->set_color(red, green, blue);

	this->set_height(height);
}

void Vertex::set_color(double red, double green, double blue) {
	this->color->clear();
	this->color->push_back(red);
	this->color->push_back(green);
	this->color->push_back(blue);
}

void Vertex::set_color(std::vector<double> rgb) {
	this->set_color(rgb[0], rgb[1], rgb[2]);
}

std::vector<double>* Vertex::get_color() {
	return this->color;
}

void Vertex::set_height(double height) {
	this->height = height;
}

double Vertex::get_height() {
	return this->height;
}
