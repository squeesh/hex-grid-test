#include "../includes.h"


extern "C" {
	Controller* Controller_get_controller() {
		return Controller::get_controller();
	}

	void Controller_push_hexagon(Hexagon *hex) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->push_hexagon(hex);
	}

	Hexagon* Controller_get_hexagon(int i, int j) {
		Controller* curr_ctrl = Controller_get_controller();
		return curr_ctrl->get_hexagon(i, j);
	}

	void Controller_zoom_map(double zoom) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->zoom_map(zoom);
	}

	void Controller_set_rotation(double rotation) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->set_rotation(rotation);
	}
	
	double Controller_get_rotation() {
		Controller* curr_ctrl = Controller_get_controller();
		return curr_ctrl->get_rotation();
	}

	void Controller_add_x_offset(double x_offset) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->curr_rend_ctrl->x_offset += x_offset;
	}

	void Controller_add_y_offset(double y_offset) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->curr_rend_ctrl->y_offset += y_offset;
	}

	void Controller_set_scroll(char direction) {
        	Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->set_scroll(direction);
	}

	void Controller_clear_scroll(char direction) {
        	Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->clear_scroll(direction);
	}

	void Controller_init_gl(long width, long height) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->init_gl(width, height);
	}

	/*void Controller_resize(long width, long height) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->resize(width, height);
	}*/

	void Controller_tick() {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->tick();
	}

	/*void Controller_render() {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->render();
	}*/

	/*double Controller_COS_60() {
		Controller* curr_ctrl = Controller_get_controller();
		return curr_ctrl->COS_60;
	}

	double Controller_SIN_60() {
		Controller* curr_ctrl = Controller_get_controller();
		return curr_ctrl->SIN_60;
	}*/

	double Controller_get_width() {
        Controller* curr_ctrl = Controller_get_controller();
        return curr_ctrl->width;
    }

    double Controller_get_height() {
        Controller* curr_ctrl = Controller_get_controller();
        return curr_ctrl->height;
    }

    double Controller_get_zoom() {
        Controller* curr_ctrl = Controller_get_controller();
        return curr_ctrl->curr_rend_ctrl->zoom;
    }

	/*void Controller_clear_selected_hex() {
	    Controller* curr_ctrl = Controller_get_controller();
        curr_ctrl->clear_selected_hex();
	}

	Hexagon* Controller_get_selected_hex() {
		Controller* curr_ctrl = Controller_get_controller();
		return curr_ctrl->get_selected_hex();
	}

	void Controller_set_selected_hex(Hexagon* curr_hex) {
        Controller* curr_ctrl = Controller_get_controller();
        curr_ctrl->set_selected_hex(curr_hex);
    }*/

    Hexagon* Controller_get_clicked_hex(int x, int y) {
        Controller* curr_ctrl = Controller_get_controller();
        return curr_ctrl->get_clicked_hex(x, y);
    }

	void Controller_set_player_input(PlayerInput* curr_input) {
	    Controller* curr_ctrl = Controller_get_controller();
	    curr_ctrl->player_input = curr_input;
	}

	/********************************************/

	void Util_force_exit() {
	    exit(0);
	}

	/********************************************/

	Hexagon* Hexagon_new(){
		return new Hexagon();
	}

	void Hexagon_set_vertex(Hexagon *curr_hex, const char* position, Vertex* vertex) {
		curr_hex->set_vertex(position, vertex);
	}

	Vertex* Hexagon_get_vertex(Hexagon *curr_hex, const char* position) {
		return curr_hex->get_vertex(position);
	}

	void Hexagon_set_last_x(Hexagon *curr_hex, double last_x) {
		curr_hex->last_x = last_x;
	}

	void Hexagon_set_last_y(Hexagon *curr_hex, double last_y) {
		curr_hex->last_y = last_y;
	}

	double Hexagon_get_last_x(Hexagon *curr_hex) {
		return curr_hex->last_x;
	}

	double Hexagon_get_last_y(Hexagon *curr_hex) {
		return curr_hex->last_y;
	}

	/*void Hexagon_set_border_color(Hexagon *curr_hex, double red, double green, double blue) {
		curr_hex->set_border_color(red, green, blue);
	}*/

	void Hexagon_set_hex_color(Hexagon *curr_hex, double red, double green, double blue) {
		curr_hex->set_hex_color(red, green, blue);
	}

	void Hexagon_set_select_color(Hexagon *curr_hex, double red, double green, double blue) {
		curr_hex->set_select_color(red, green, blue);
	}

	void Hexagon_clear_select_color(Hexagon *curr_hex) {
		curr_hex->clear_select_color();
	}

	void Hexagon_set_height(Hexagon *curr_hex, double height) {
		curr_hex->set_height(height);
	}

	void Hexagon_add_height(Hexagon *curr_hex, double height) {
		curr_hex->add_height(height);
	}

	double Hexagon_get_height(Hexagon *curr_hex) {
		return curr_hex->get_height();
	}

	double Hexagon_get_slope(Hexagon *curr_hex) {
		return curr_hex->get_slope();
	}

	void Hexagon_set_neighbor(Hexagon* curr_hex, const char* position, Hexagon* neighbor_hex) {
		curr_hex->set_neighbor(position, neighbor_hex);
	}

	Hexagon* Hexagon_get_neighbor(Hexagon* curr_hex, const char* position) {
		return curr_hex->get_neighbor(position);
	}

	bool Hexagon_is_pathable(Hexagon* curr_hex) {
		return curr_hex->is_pathable();
	}

	BoardObject* Hexagon_get_board_object(Hexagon* curr_hex) {
	    curr_hex->get_board_object();
	}

	/********************************************/

	BoardObject* BoardObject_new(Hexagon* curr_hex) {
        return new BoardObject(curr_hex);
    }

	bool BoardObject_is_selected(BoardObject* curr_board_obj) {
	    return curr_board_obj->get_selected();
	}

    void BoardObject_set_selected(BoardObject* curr_board_obj, bool selected) {
        curr_board_obj->set_selected(selected);
    }

	Hexagon* BoardObject_get_base_hex(BoardObject* curr_board_obj) {
	    return curr_board_obj->base_hex;
	}

	void BoardObject_move_to_hex(BoardObject* curr_obj, Hexagon* curr_hex) {
        curr_obj->move_to_hex(curr_hex);
    }


	/********************************************/

	PlayerInput* PlayerInput_new(PyObject* py_pointer) {
	    return new PlayerInput(py_pointer);
	}
}
