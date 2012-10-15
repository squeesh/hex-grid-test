#ifndef CONTROLLER_H
#define CONTROLLER_H


class Controller {
	private:
		Controller(void);
		static Controller* curr_ctrl;

		long width;
		long height;

		double x_offset;
		double y_offset;

		double zoom;
		double rotation;
	
		double view_range;

		std::map<char, bool> scroll_map;

		Hexagon* selected_hex;

		PyObject* controller_py;

		std::set<Hexagon*>* get_neighbors_in_radius(Hexagon*, int, std::set<Hexagon*>*);

	public:
		//Hexagon* debug_hex; // for debugging...

		std::string print_string;

		bool print_flag;

		static const double COS_60;
		static const double SIN_60;

		std::map<int, std::map<const char*, int, cmp_str> > old_mouse_pos;

		static Controller* get_controller();
		static Controller* py_get_controller();

		void init_board();

		void zoom_map(double);

		void set_rotation(double);
		double get_rotation();

		void set_scroll(char);
		void clear_scroll(char);

		void init_gl(long, long);
		void resize(long, long);

		void tick();

		void render_string(int, int, std::string);
		void render_string(int, int, std::string, std::vector< GLfloat >*);

		void render();
		void render_for_select();

		void push_hexagon(Hexagon*);
		void clear_selected_hex();
		void set_selected_hex(Hexagon*);
		Hexagon* get_selected_hex();
		Hexagon* get_hex_by_name(long);
		Hexagon* get_hexagon(int, int);
		Hexagon* get_clicked_hex(double, double);

		std::set< Hexagon* >* get_neighbors_in_radius(Hexagon*, int);

		void mouse_left_click(int, int);
		void mouse_left_release(int, int);
		void mouse_middle_click(int, int);
		void mouse_middle_release(int, int);
		void mouse_right_click(int, int);
		void mouse_right_release(int, int);
		void mouse_scroll_up(int, int);
		void mouse_scroll_down(int, int);

		void mouse_left_drag(int, int);
		void mouse_middle_drag(int, int);
		void mouse_right_drag(int, int);

		void key_down(unsigned char, int, int);
		void key_up(unsigned char, int, int);

		Hexagon* add_object_to_board(BoardObject*, Hexagon*) ;

		Gameboard* gameboard;
};


#endif



