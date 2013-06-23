#ifndef BOARD_OBJECT_H
#define BOARD_OBJECT_H

class BoardObject {
	private:
		bool selected;
		GLvoid* tex_data;

	public:
		Hexagon* base_hex;
		std::vector< GLfloat >* color;
		std::vector< GLfloat >* selected_color;

		BoardObject(Hexagon*);

		void render();
		void move_to_hex(Hexagon*);

		void set_selected(bool);
		bool get_selected();

		void set_tex_data(GLvoid*);
		GLvoid* get_tex_data();
};

#endif

