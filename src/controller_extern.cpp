#include "includes.h"


extern "C" {
	Controller* Controller_get_controller() {
		return Controller::_get_controller();
	}

	void Controller_push_hexagon(Hexagon *hex) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->push_hexagon(hex);
	}

	Hexagon* Controller_pop_hexagon() {
		Controller* curr_ctrl = Controller_get_controller();
		return curr_ctrl->pop_hexagon();
	}

	Hexagon* Controller_get_hexagon(int i, int j) {
		Controller* curr_ctrl = Controller_get_controller();
		return curr_ctrl->get_hexagon(i, j);
	}

	void Controller_set_zoom(double zoom) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->set_zoom(zoom);
	}

	double* Controller_get_zoom() {
		Controller* curr_ctrl = Controller_get_controller();
		double zoom = curr_ctrl->get_zoom();
		return &zoom;
	}

	void Controller_set_rotation(double rotation) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->set_rotation(rotation);
	}
	
	double* Controller_get_rotation() {
		Controller* curr_ctrl = Controller_get_controller();
		double rotation = curr_ctrl->get_rotation();
		return &rotation;
	}

	void Controller_set_scroll(char direction) {
        Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->set_scroll(direction);
	}

	void Controller_clear_scroll(char direction) {
        Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->clear_scroll(direction);
	}

	void Controller_set_view_range(double view_range) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->set_view_range(view_range);
	}

	double* Controller_get_view_range() {
        Controller* curr_ctrl = Controller_get_controller();
        double view_range = curr_ctrl->get_view_range();
        return &view_range;
    }


	void Controller_init_gl(long width, long height) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->init_gl(width, height);
	}

	void Controller_resize(long width, long height) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->resize(width, height);
	}

	void Controller_init_board() {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->init_board();
	}

	void Controller_tick() {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->tick();
	}

	void Controller_render() {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->render();
	}

	double* Controller_COS_60() {
		Controller* curr_ctrl = Controller_get_controller();
		double COS_60 = curr_ctrl->COS_60;
		return &COS_60;
	}

	double* Controller_SIN_60() {
		Controller* curr_ctrl = Controller_get_controller();
		double SIN_60 = curr_ctrl->SIN_60;
		return &SIN_60;
	}

	void Controller_mouse_left_click(double x, double y) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->mouse_left_click(x, y);
	}

	void Controller_mouse_scroll_up(double x, double y) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->mouse_scroll_up(x, y);
	}

	void Controller_mouse_scroll_down(double x, double y) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->mouse_scroll_down(x, y);
	}

	Hexagon* Controller_get_selected_hex() {
		Controller* curr_ctrl = Controller_get_controller();
		return curr_ctrl->get_selected_hex();
	}

	void Controller_set_MAX_PATHABLE_SLOPE(double slope) {
		Controller* curr_ctrl = Controller_get_controller();
		curr_ctrl->set_MAX_PATHABLE_SLOPE(slope);
	}

	double* Controller_get_MAX_PATHABLE_SLOPE() {
		Controller* curr_ctrl = Controller_get_controller();
		double slope = curr_ctrl->get_MAX_PATHABLE_SLOPE();
		return &slope;
	}

	/********************************************/

	Hexagon* Hexagon_new_hexagon(){
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

	double* Hexagon_get_last_x(Hexagon *curr_hex) {
		double last_x = curr_hex->last_x;
		return &last_x;
	}

	double* Hexagon_get_last_y(Hexagon *curr_hex) {
		double last_y = curr_hex->last_y;
		return &last_y;
	}

	void Hexagon_set_color(Hexagon *curr_hex, double red, double green, double blue) {
		curr_hex->set_color(red, green, blue);
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

	double* Hexagon_get_height(Hexagon *curr_hex) {
		double height = curr_hex->get_height();
		return &height;
	}

	double* Hexagon_get_slope(Hexagon *curr_hex) {
		double slope = curr_hex->get_slope();
		return &slope;
	}

	void Hexagon_set_neighbor(Hexagon* curr_hex, const char* position, Hexagon* neighbor_hex) {
		curr_hex->set_neighbor(position, neighbor_hex);
	}

	Hexagon* Hexagon_get_neighbor(Hexagon* curr_hex, const char* position) {
		return curr_hex->get_neighbor(position);
	}

	bool* Hexagon_is_pathable(Hexagon* curr_hex) {
		bool pathable = curr_hex->is_pathable();
		return &pathable;
	}
}
