#ifndef CONTROLLER_H
#define CONTROLLER_H


class Controller {
	private:
		Controller(void);
		static Controller* curr_ctrl;
		static PyObject* py_pointer;

		double rotation;
	
		double view_range;

		std::map<char, bool> scroll_map;

		//Hexagon* selected_hex;

		std::set<Hexagon*>* get_neighbors_in_radius(Hexagon*, int, std::set<Hexagon*>*);

	public:
		long width;
		long height;

		double x_offset;
		double y_offset;
		double zoom;

		PlayerInput* player_input;

		std::string print_string;

		bool print_flag;

		static const double COS_60;
		static const double SIN_60;

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
		/*void clear_selected_hex();
		void set_selected_hex(Hexagon*);
		Hexagon* get_selected_hex();*/
		Hexagon* get_hex_by_name(long);
		Hexagon* get_hexagon(int, int);
		Hexagon* get_clicked_hex(double, double);

		std::set< Hexagon* >* get_neighbors_in_radius(Hexagon*, int);

		void mouse_event(int, int, int, int);
		void keyboard_event(int, unsigned char, int, int);

		Hexagon* add_object_to_board(BoardObject*, Hexagon*) ;

		Gameboard* gameboard;
};


#endif



