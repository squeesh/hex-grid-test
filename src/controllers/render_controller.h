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

        GLdouble zoom;
        GLdouble view_range;
        GLdouble rotation;

        std::map<char, bool> scroll_map;

        std::vector< Renderable* >* renderables;

        RenderController(void);
    public:
        static std::string print_string;

        GLlong width;
        GLlong height;

        GLdouble x_offset;
        GLdouble y_offset;

        void init_gl(GLlong, GLlong);
        void resize(GLlong, GLlong);

        void tick();

        static RenderController* get_render_controller();
        void register_renderable(Renderable*);
        void render();
        void render_for_select();
        Hexagon* get_clicked_hex(GLdouble, GLdouble);

        void render_string(int, int, std::string);
        void render_string(int, int, std::string, std::vector< GLdouble >*);

        void set_scroll(char);
        void clear_scroll(char);

        void zoom_map(GLdouble);
        GLdouble get_zoom();

        void set_rotation(GLdouble);
        GLdouble get_rotation();
};


#endif

