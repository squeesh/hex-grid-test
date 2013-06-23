#ifndef RENDER_CONTROLLER_H
#define RENDER_CONTROLLER_H

class Hexagon;
class Controller;


class Renderable {
	public:
		Renderable();
		virtual void render(Controller* curr_ctrl, int x_start, int x_stop, int y_start, int y_stop) {}
};


class RenderController {
	private:
		static RenderController* curr_rend_ctrl;

		double zoom;
		double view_range;
		double rotation;

		std::map<char, bool> scroll_map;

		std::vector< Renderable* >* renderables;

		RenderController(void);
	public:
		static std::string print_string;

		long width;
		long height;

		double x_offset;
		double y_offset;

		void init_gl(long, long);
		void resize(long, long);

		void tick();

		static RenderController* get_render_controller();
		void register_renderable(Renderable*);
		void render();
		void render_for_select();
		Hexagon* get_clicked_hex(double, double);

		void render_string(int, int, std::string);
		void render_string(int, int, std::string, std::vector< GLfloat >*);

		void set_scroll(char);
		void clear_scroll(char);

		void zoom_map(double);
		double get_zoom();

		void set_rotation(double);
		double get_rotation();
};


#endif

