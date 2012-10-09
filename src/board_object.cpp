#include "includes.h"

BoardObject::BoardObject(Hexagon* base_hex) {
	this->base_hex = base_hex;
	this->color = new std::vector< double >();
	this->color->push_back(0);
	this->color->push_back(0);
	this->color->push_back(1);
}

