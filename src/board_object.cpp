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

    glGenTextures(1, &(this->tex_name));
    glBindTexture(GL_TEXTURE_2D, this->tex_name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->tex_data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
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
                    GLdouble x = i * 1.5 * GlobalConsts::COS_60;
                    GLdouble y = j * 1.0 * GlobalConsts::SIN_60;

                    if(i % 2 != 0) {
                        y += 0.5 * GlobalConsts::SIN_60;
                    }

                    glPushMatrix();
                    glTranslatef(x, y, curr_hex->get_height());

                    this->render();

                    glPopMatrix();
                }
            }
        }
    }
}

void BoardObject::render() {
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, this->tex_name);

    /*if(this->selected) {
        glColor3fv(this->selected_color->data());
    } else {
        glColor3fv(this->color->data());
    }*/

    glColor3f(1, 1, 1);

    glBegin(GL_QUADS);
        glTexCoord2d(1.0, 1.0); glVertex3f( 0.5, 0, 0.0); // The bottom right corner
        glTexCoord2d(1.0, 0.0); glVertex3f( 0.5, 0, 1.0); // The top right corner
        glTexCoord2d(0.0, 0.0); glVertex3f(-0.5, 0, 1.0); // The top left corner
        glTexCoord2d(0.0, 1.0); glVertex3f(-0.5, 0, 0.0); // The bottom left corner
    glEnd();

    glDisable( GL_TEXTURE_2D );
}

