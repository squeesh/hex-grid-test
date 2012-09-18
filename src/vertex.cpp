#include "includes.h"


Vertex::Vertex(double red, double green, double blue, double height) {
	this->color = new Color(red, green, blue);

	this->set_height(height);
}

void Vertex::set_color(double red, double green, double blue) {
	this->color->set_rgb(red, green, blue);
}

void Vertex::set_color(std::vector<double> rgb) {
	this->color->set_rgb(rgb);
}

std::vector<double> Vertex::get_color() {
	return this->color->get_rgb();
}

void Vertex::set_height(double height) {
	this->height = height;
}

double Vertex::get_height() {
	return this->height;
}
