#ifndef CONTROLLER_H
#define CONTROLLER_H


class Controller {
	private:
		Controller(void);
		static Controller* curr_ctrl;
		static PyObject* py_pointer;

		std::set<Hexagon*>* get_neighbors_in_radius(Hexagon*, int, std::set<Hexagon*>*);

	public:
		PlayerInput* player_input;

		bool print_flag;

		static Controller* get_controller();
		static Controller* py_get_controller();

		void init_board();

		void tick();

		void push_hexagon(Hexagon*);
		Hexagon* get_hex_by_name(GLlong);
		Hexagon* get_hexagon(int, int);

		std::set< Hexagon* >* get_neighbors_in_radius(Hexagon*, int);

		void mouse_event(int, int, int, int);
		void keyboard_event(int, unsigned char, int, int);

		Hexagon* add_object_to_board(BoardObject*, Hexagon*) ;

		Gameboard* gameboard;
};


#endif



