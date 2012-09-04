#include "includes.h"


Color::Color(double red, double green, double blue){
	this->set_rgb(red, green, blue);
}

Color::Color(std::vector<double> rgb){
	this->set_rgb(rgb);
}


void Color::set_rgb(double red, double green, double blue){
	this->red = red;
	this->green = green;
	this->blue = blue;
}

void Color::set_rgb(std::vector<double> rgb){
	this->red = rgb[0];
	this->green = rgb[1];
	this->blue = rgb[2];
}

std::vector<double> Color::get_rgb(void) {
	std::vector<double> output = std::vector<double>();
	output.reserve(3);
	output.push_back(red);
	output.push_back(green);
	output.push_back(blue);

	return output;
}

