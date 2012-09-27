#include "includes.h"

GameboardChunk::GameboardChunk() {
	this->board_vertex_data = new UniqueDataVector< GLfloat >();
	this->board_select_data = new UniqueDataVector< GLfloat >();

	this->vbo_hex_vert = 0;
	this->vbo_hex_color = 0;
	this->vbo_hex_indicie = 0;

	this->vbo_sel_vert = 0;
	this->vbo_sel_color = 0;
	this->vbo_sel_indicie = 0;

	glGenBuffersARB(1, &(this->vbo_hex_vert));
	glGenBuffersARB(1, &(this->vbo_hex_color));
	glGenBuffersARB(1, &(this->vbo_hex_indicie));
	glGenBuffersARB(1, &(this->vbo_sel_vert));
	glGenBuffersARB(1, &(this->vbo_sel_color));
	glGenBuffersARB(1, &(this->vbo_sel_indicie));

	this->regenerate = true;
}


GameboardChunk::~GameboardChunk() {
    std::cout << "deleting: " << this << std::endl;

	delete this->board_vertex_data;
	delete this->board_select_data;

	glDeleteBuffersARB(1, &(this->vbo_hex_vert));
	glDeleteBuffersARB(1, &(this->vbo_hex_color));
	glDeleteBuffersARB(1, &(this->vbo_hex_indicie));

	glDeleteBuffersARB(1, &(this->vbo_sel_vert));
	glDeleteBuffersARB(1, &(this->vbo_sel_color));
	glDeleteBuffersARB(1, &(this->vbo_sel_indicie));
}


void GameboardChunk::clear() {
	delete this->board_vertex_data;
	delete this->board_select_data;

	this->board_vertex_data = new UniqueDataVector< GLfloat >();
	this->board_select_data = new UniqueDataVector< GLfloat >();
	this->regenerate = true;
}


void GameboardChunk::generate_render_data(Hexagon* curr_hex, float x, float y) {
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
	// TODO: very messy looking... this can be cleaned further...

	// generate a new VBO and get the associated ID

	//glGenBuffersARB(1, &(this->vbo_hex_vert));
	std::cout << "vbo_hex_vert:    " << this->vbo_hex_vert << " | " << this->board_vertex_data << " | " << this->board_vertex_data->indicies_size() * 3 << std::endl;

	// bind VBO in order to use
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, this->vbo_hex_vert);
	// upload data to VBO
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
		sizeof(this->board_vertex_data->data()) * this->board_vertex_data->indicies_size() * 3, this->board_vertex_data->data(), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		// generate a new VBO and get the associated ID
	//glGenBuffersARB(1, &(this->vbo_hex_color));
	std::cout << "vbo_hex_color:   " << this->vbo_hex_color << " | " << this->board_vertex_data << " | " << this->board_vertex_data->indicies_size() * 3 << std::endl;

	// bind VBO in order to use
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, this->vbo_hex_color);
	// upload data to VBO
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
		sizeof(this->board_vertex_data->color_data()) * this->board_vertex_data->indicies_size() * 3, this->board_vertex_data->color_data(), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		// generate a new VBO and get the associated ID
	//glGenBuffersARB(1, &(this->vbo_hex_indicie));
	std::cout << "vbo_hex_indicie: " << this->vbo_hex_indicie << " | " << this->board_vertex_data << " | " << this->board_vertex_data->indicies_size() << std::endl;

	// bind VBO in order to use
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, this->vbo_hex_indicie);
	// upload data to VBO
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 
		sizeof(this->board_vertex_data->indicies_data()) * this->board_vertex_data->indicies_size(), this->board_vertex_data->indicies_data(), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

	//-------------------------------------

		// generate a new VBO and get the associated ID
	//glGenBuffersARB(1, &(this->vbo_sel_vert));
	std::cout << "vbo_sel_vert:    " << this->vbo_sel_vert << " | " << this->board_select_data << " | " << this->board_select_data->indicies_size() * 3 << std::endl;

	// bind VBO in order to use
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, this->vbo_sel_vert);
	// upload data to VBO
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
		sizeof(this->board_select_data->data()) * this->board_select_data->indicies_size() * 3, this->board_select_data->data(), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		// generate a new VBO and get the associated ID
	//glGenBuffersARB(1, &(this->vbo_sel_color));
	std::cout << "vbo_sel_color:   " << this->vbo_sel_color << " | " << this->board_select_data << " | " << this->board_select_data->indicies_size() * 3 << std::endl;

	// bind VBO in order to use
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, this->vbo_sel_color);
	// upload data to VBO
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, 
		sizeof(this->board_select_data->color_data()) * this->board_select_data->indicies_size() * 3, this->board_select_data->color_data(), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

		// generate a new VBO and get the associated ID
	//glGenBuffersARB(1, &(this->vbo_sel_indicie));
	std::cout << "vbo_sel_indicie: " << this->vbo_sel_indicie << " | " << this->board_select_data << " | " << this->board_select_data->indicies_size() * 3 << std::endl;

	// bind VBO in order to use
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, this->vbo_sel_indicie);
	// upload data to VBO
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 
		sizeof(this->board_select_data->indicies_data()) * this->board_select_data->indicies_size(), this->board_select_data->indicies_data(), GL_STATIC_DRAW_ARB);
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

}


