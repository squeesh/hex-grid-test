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
		Hexagon* debug_hex; // for debugging...

		bool print_flag;

		static const double COS_60;
		static const double SIN_60;

		std::map<int, std::map<const char*, int, cmp_str> > old_mouse_pos;

		static Controller* _get_controller();
		static Controller* get_controller();

		void init_board();
		void py_init_board();

		void push_hexagon(Hexagon *hex);
		//Hexagon* pop_hexagon();
		Hexagon* get_hexagon(int i, int j);

		void zoom_map(double);

		void set_rotation(double rotation);
		double get_rotation();

		void set_scroll(char direction);
		void clear_scroll(char direction);

		void init_gl(long width, long height);
		void resize(long width, long height);

		void tick();

		//void generate_render_data(Hexagon*, double, double, UniqueDataVector< GLfloat >*);
		//UniqueDataVector< GLfloat >* get_render_data(Hexagon*);

		void render();
		void render_for_select();

		void init_board_render_cache();
		void reset_board_render_cache();

		Hexagon* get_clicked_hex(double x, double y);

		std::set< Hexagon* >* get_neighbors_in_radius(Hexagon*, int);

		Hexagon* get_hex_by_name(long name);

		void mouse_left_click(int x, int y);
		void mouse_left_release(int x, int y);
		void mouse_right_click(int x, int y);
		void mouse_right_release(int x, int y);
		void mouse_scroll_up(int x, int y);
		void mouse_scroll_down(int x, int y);

		void mouse_left_drag(int x, int y);
		void mouse_right_drag(int x, int y);

		void key_down(unsigned char key, int x, int y);
		void key_up(unsigned char key, int x, int y);

		void set_selected_hex(Hexagon* curr_hex);
		Hexagon* get_selected_hex();

		//RoundVector< RoundVector< Hexagon* >* >* hexagon_list;
		//std::map< Hexagon*, std::vector< int >* >* hexagon_indicies;

		//std::map< Hexagon*, UniqueDataVector< GLfloat >* >* line_vertex_data;
		//std::map< Hexagon*, UniqueDataVector< GLfloat >* >* triangle_vertex_data;
		/*std::map< Hexagon*, GLuint >* vbo_ids;
		std::map< Hexagon*, GLuint >* vbo_colors;
		std::map< Hexagon*, GLuint >* vbo_indicies;*/

		Gameboard* gameboard;

		static void segfault_sigaction(int signal, siginfo_t *si, void *arg);
		static void set_segfault_hanlder();

		static struct sigaction sa;
};


#endif



