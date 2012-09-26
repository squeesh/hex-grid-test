#include "includes.h"

GameboardChunk::GameboardChunk() {
	this->board_vertex_data = new UniqueDataVector< GLfloat >();
	this->board_select_data = new UniqueDataVector< GLfloat >();
}


GameboardChunk::~GameboardChunk() {
	delete this->board_vertex_data;
	delete this->board_select_data;

	glDeleteBuffers(1, &(this->vbo_hex_vert));
	glDeleteBuffers(1, &(this->vbo_hex_color));
	glDeleteBuffers(1, &(this->vbo_hex_indicie));

	glDeleteBuffers(1, &(this->vbo_sel_vert));
	glDeleteBuffers(1, &(this->vbo_sel_color));
	glDeleteBuffers(1, &(this->vbo_sel_indicie));
}


void GameboardChunk::generate_render_data(Hexagon* curr_hex, double x, double y) {
	Vertex* curr_vert = NULL;
	std::vector< double >* curr_color;

	// Generate verticies for hex guts
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

	curr_color = curr_hex->get_select_color();

        for(int k = 0; k < 6; k++) {
            curr_vert = curr_hex->verticies[curr_hex->VERTEX_POSITIONS->at(k)];

            if(k > 1) {
                if(curr_color) {
		    this->board_select_data->push_back(
			Hexagon::ROT_COORDS->at(0)->at(0) * 0.8 + x, Hexagon::ROT_COORDS->at(0)->at(1) * 0.8 + y, curr_hex->verticies[curr_hex->VERTEX_POSITIONS->at(0)]->get_height() + 0.1,
			curr_color->at(0), curr_color->at(1), curr_color->at(2)
		    );

		    this->board_select_data->push_back(
			Hexagon::ROT_COORDS->at(k-1)->at(0) * 0.8 + x, Hexagon::ROT_COORDS->at(k-1)->at(1) * 0.8 + y, curr_hex->verticies[curr_hex->VERTEX_POSITIONS->at(k-1)]->get_height() + 0.1,
			curr_color->at(0), curr_color->at(1), curr_color->at(2)
		    );

		    this->board_select_data->push_back(
			Hexagon::ROT_COORDS->at(k)->at(0) * 0.8 + x, Hexagon::ROT_COORDS->at(k)->at(1) * 0.8 + y, curr_hex->verticies[curr_hex->VERTEX_POSITIONS->at(k)]->get_height() + 0.1,
			curr_color->at(0), curr_color->at(1), curr_color->at(2)
		    );
                }
            }
        }

}


void GameboardChunk::write_VBO_data() {
	// generate a new VBO and get the associated ID
	glGenBuffersARB(1, &(this->vbo_hex_vert));
	// bind VBO in order to use
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, this->vbo_hex_vert);
	// upload data to VBO
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
		sizeof(this->board_vertex_data->data()) * this->board_vertex_data->indicies_size() * 3, this->board_vertex_data->data(), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	// generate a new VBO and get the associated ID
	glGenBuffersARB(1, &(this->vbo_hex_color));
	// bind VBO in order to use
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, this->vbo_hex_color);
	// upload data to VBO
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
		sizeof(this->board_vertex_data->color_data()) * this->board_vertex_data->indicies_size() * 3, this->board_vertex_data->color_data(), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	// generate a new VBO and get the associated ID
	glGenBuffersARB(1, &(this->vbo_hex_indicie));
	// bind VBO in order to use
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, this->vbo_hex_indicie);
	// upload data to VBO
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 
		sizeof(this->board_vertex_data->indicies_data()) * this->board_vertex_data->indicies_size(), this->board_vertex_data->indicies_data(), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

	//-------------------------------------

	// generate a new VBO and get the associated ID
	glGenBuffersARB(1, &(this->vbo_sel_vert));
	// bind VBO in order to use
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, this->vbo_sel_vert);
	// upload data to VBO
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
		sizeof(this->board_select_data->data()) * this->board_select_data->indicies_size() * 3, this->board_select_data->data(), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	// generate a new VBO and get the associated ID
	glGenBuffersARB(1, &(this->vbo_sel_color));
	// bind VBO in order to use
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, this->vbo_sel_color);
	// upload data to VBO
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
		sizeof(this->board_select_data->color_data()) * this->board_select_data->indicies_size() * 3, this->board_select_data->color_data(), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	// generate a new VBO and get the associated ID
	glGenBuffersARB(1, &(this->vbo_sel_indicie));
	// bind VBO in order to use
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, this->vbo_sel_indicie);
	// upload data to VBO
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 
		sizeof(this->board_select_data->indicies_data()) * this->board_select_data->indicies_size(), this->board_select_data->indicies_data(), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

}


