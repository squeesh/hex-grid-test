#ifndef BOARD_OBJECT_H
#define BOARD_OBJECT_H

class Controller;


class BoardObject: public Renderable  {
	private:
		bool selected;
		GLvoid* tex_data;

	public:
		Hexagon* base_hex;
		std::vector< GLfloat >* color;
		std::vector< GLfloat >* selected_color;

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

