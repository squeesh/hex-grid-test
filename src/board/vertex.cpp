#include "../includes.h"


Vertex::Vertex( GLdouble height) {
    this->set_height(height);
}


/*Vertex::Vertex(GLdouble red, GLdouble green, GLdouble blue, GLdouble height) {
    this->color = new std::vector< GLdouble >();
    this->set_color(red, green, blue);

    this->set_height(height);
}

void Vertex::set_color(GLdouble red, GLdouble green, GLdouble blue) {
    this->color->clear();
    this->color->push_back(red);
    this->color->push_back(green);
    this->color->push_back(blue);
}

void Vertex::set_color(std::vector<GLdouble> rgb) {
    this->set_color(rgb[0], rgb[1], rgb[2]);
}

std::vector<GLdouble>* Vertex::get_color() {
    return this->color;
}*/

void Vertex::set_height(GLdouble height) {
    this->height = height;
}

GLdouble Vertex::get_height() {
    return this->height;
}
