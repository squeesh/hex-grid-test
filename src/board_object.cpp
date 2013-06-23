#include "includes.h"

BoardObject::BoardObject(Hexagon* base_hex) {
    Controller* curr_ctrl = Controller::get_controller();
    this->base_hex = NULL;
    this->base_hex = curr_ctrl->add_object_to_board(this, base_hex);

	this->color = new std::vector< GLfloat >();
	this->color->push_back(0);
	this->color->push_back(0);
	this->color->push_back(1);

    this->selected_color = new std::vector< GLfloat >();
    this->selected_color->push_back(1);
    this->selected_color->push_back(0);
    this->selected_color->push_back(1);

	this->selected = false;
    this->tex_data = NULL;
}


void BoardObject::render() {

}


void BoardObject::move_to_hex(Hexagon* curr_hex) {
    Controller* curr_ctrl = Controller::get_controller();
    curr_ctrl->gameboard->bind_obj_hex(this, curr_hex);
}


void BoardObject::set_selected(bool selected) {
	// TODO: only regenerate if val changes
	this->selected = selected;
	this->base_hex->parent_chunk->regenerate_object = true;
}

bool BoardObject::get_selected() {
	return this->selected;
}

void BoardObject::set_tex_data(GLvoid* new_tex_data) {
    this->tex_data = new_tex_data;
}

GLvoid* BoardObject::get_tex_data() {
    return this->tex_data;
}

