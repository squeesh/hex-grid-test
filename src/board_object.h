#ifndef BOARD_OBJECT_H
#define BOARD_OBJECT_H

class BoardObject {
	public:
		Hexagon* base_hex;
		std::vector< double >* color;

		bool selected;

		BoardObject(Hexagon*);

		void render();
};

#endif

