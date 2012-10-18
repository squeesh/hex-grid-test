#ifndef BOARD_OBJECT_H
#define BOARD_OBJECT_H

class BoardObject {
	private:
		bool selected;

	public:
		Hexagon* base_hex;
		std::vector< double >* color;

		BoardObject(Hexagon*);

		void render();
		void move_to_hex(Hexagon*);

		void set_selected(bool);
		bool get_selected();
};

#endif

