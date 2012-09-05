#ifndef CONTROLLER_H
#define CONTROLLER_H


class Controller {
	private:
		Controller(void);
		static Controller* curr_ctrl;

		static const int RENDER_LINES;
		static const int RENDER_TRIANGLES;

		double MIN_VIEW_RANGE;
		double MAX_VIEW_RANGE;

		double MIN_ZOOM;
		double MAX_ZOOM;

		double MAX_PATHABLE_SLOPE;

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

	public:
		static const double COS_60;
		static const double SIN_60;

		std::map<int, std::map<const char*, int, cmp_str> > old_mouse_pos;

		static Controller* _get_controller();
		static Controller* get_controller();

		void set_MAX_PATHABLE_SLOPE(double slope);
		double get_MAX_PATHABLE_SLOPE();

		void init_board();
		void py_init_board();

		void push_hexagon(Hexagon *hex);
		Hexagon* pop_hexagon();
		Hexagon* get_hexagon(int i, int j);

		void set_zoom(double zoom);
		double get_zoom();

		void set_rotation(double rotation);
		double get_rotation();

		void set_scroll(char direction);
		void clear_scroll(char direction);

		double get_view_range();
		void set_view_range(double view_range);

		void init_gl(long width, long height);
		void resize(long width, long height);

		

		void tick();

		void render();
		void render(int render_mode);

		Hexagon* get_clicked_hex(double x, double y);

		std::vector<Hexagon*>* get_neighbor_in_radius(Hexagon*, int);

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

		RoundVector< RoundVector< Hexagon* >* >* hexagon_list;
};

#endif



