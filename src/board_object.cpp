#include "includes.h"

BoardObject::BoardObject(Hexagon* base_hex) {
    Controller* curr_ctrl = Controller::get_controller();
    this->base_hex = curr_ctrl->add_object_to_board(this, base_hex);

	this->color = new std::vector< double >();
	this->color->push_back(0);
	this->color->push_back(0);
	this->color->push_back(1);

	this->selected = false;
}


void BoardObject::render() {

}

