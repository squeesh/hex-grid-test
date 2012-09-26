#include "includes.h"

GameboardChunk::GameboardChunk() {
	board_vertex_data = new UniqueDataVector< GLfloat >();
}


void GameboardChunk::generate_render_data(Hexagon* curr_hex, double x, double y) {
	Vertex* curr_vert = NULL;
	std::vector< double >* curr_color;

	// Render hex guts
	curr_color = curr_hex->get_hex_color();

	for(int i = 1; i < 5; i++) {
		curr_vert = curr_hex->verticies[curr_hex->VERTEX_POSITIONS->at(0)];

		this->board_vertex_data->push_back(
			x + Hexagon::ROT_COORDS->at(0)->at(0), y + Hexagon::ROT_COORDS->at(0)->at(1), curr_vert->get_height(),
			curr_color->at(0), curr_color->at(1), curr_color->at(2)
		);

		curr_vert = curr_hex->verticies[curr_hex->VERTEX_POSITIONS->at(i)];

		this->board_vertex_data->push_back(
			x + Hexagon::ROT_COORDS->at(i)->at(0), y + Hexagon::ROT_COORDS->at(i)->at(1), curr_vert->get_height(),
			curr_color->at(0), curr_color->at(1), curr_color->at(2)
		);

		curr_vert = curr_hex->verticies[curr_hex->VERTEX_POSITIONS->at(i+1)];

		this->board_vertex_data->push_back(
			x + Hexagon::ROT_COORDS->at(i+1)->at(0), y + Hexagon::ROT_COORDS->at(i+1)->at(1), curr_vert->get_height(),
			curr_color->at(0), curr_color->at(1), curr_color->at(2)
		);
	}
}

