#ifndef BOARD_OBJECT_H
#define BOARD_OBJECT_H

class Controller;


class BoardObject: public Renderable  {
	private:
		bool selected;
		GLvoid* tex_data;
		GLuint tex_name;

	public:
		Hexagon* base_hex;
		std::vector< GLdouble >* color;
		std::vector< GLdouble >* selected_color;

		BoardObject(Hexagon*);

		void move_to_hex(Hexagon*);

		void set_selected(bool);
		bool get_selected();

		void set_tex_data(GLvoid*);
		GLvoid* get_tex_data();

		virtual void render(Controller*, int, int, int, int);
		void render();
};


#endif

