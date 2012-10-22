#ifndef RENDER_CONTROLLER_H
#define RENDER_CONTROLLER_H


class Renderable {
	public:
		Renderable();
		virtual void render(int x_start, int x_stop, int y_start, int y_stop) {}
};


class RenderController {
	private:
		static RenderController* curr_rend_ctrl;

		std::vector< Renderable* >* renderables;

		RenderController(void);
	public:
		static std::string print_string;

		double x_offset;
		double y_offset;
		double zoom;
		double view_range;
		double rotation;


		static RenderController* get_render_controller();
		void register_renderable(Renderable*);
		void render();
};


#endif

