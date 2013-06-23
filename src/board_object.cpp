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


void BoardObject::move_to_hex(Hexagon* curr_hex) {
    Controller* curr_ctrl = Controller::get_controller();
    curr_ctrl->gameboard->bind_obj_hex(this, curr_hex);
}


void BoardObject::set_selected(bool selected) {
	// TODO: only regenerate if val changes
	this->selected = selected;
	//this->base_hex->parent_chunk->regenerate_object = true;
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

void BoardObject::render(Controller* curr_ctrl, int x_start, int x_stop, int y_start, int y_stop) {
    Hexagon* curr_hex = NULL;
    RoundVector< RoundVector< Hexagon* >* >* hex_list = curr_ctrl->gameboard->get_hexagon_list();

    // loops over entire board, checking to see if the current hex is base_hex.
    // There is a better way to do this...

    if(this->base_hex) {
        for(int j = y_start; j <= y_stop; j+=1) {
            for(int i = x_start; i <= x_stop; i+=1) {
                curr_hex = hex_list->at(i)->at(j);

                if(curr_hex == this->base_hex) {
                    double x = i * 1.5 * GlobalConsts::COS_60;
                    double y = j * 1.0 * GlobalConsts::SIN_60;

                    if(i % 2 != 0) {
                        y += 0.5 * GlobalConsts::SIN_60;
                    }

                    glPushMatrix();
                    glTranslatef(x, y, 0);

                    this->render();

                    glPopMatrix();
                }
            }
        }
    }
}

void BoardObject::render() {
    glDisable(GL_CULL_FACE);
    if(this->selected) {
        glColor3fv(this->selected_color->data());
    } else {
        glColor3fv(this->color->data());
    }

    glBegin(GL_QUADS);
        glVertex3f( 0.5,   0, 0.0); // The bottom right corner
        glVertex3f( 0.5,   0, 1.0); // The top right corner
        glVertex3f(-0.5,   0, 1.0); // The top left corner
        glVertex3f(-0.5,   0, 0.0); // The bottom left corner
    glEnd();
}

