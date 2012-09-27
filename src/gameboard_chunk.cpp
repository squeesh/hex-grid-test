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

	glGenBuffers(1, &(this->vbo_hex_vert));
	glGenBuffers(1, &(this->vbo_hex_color));
	glGenBuffers(1, &(this->vbo_hex_indicie));
	glGenBuffers(1, &(this->vbo_sel_vert));
	glGenBuffers(1, &(this->vbo_sel_color));
	glGenBuffers(1, &(this->vbo_sel_indicie));

	this->regenerate = true;
}


GameboardChunk::~GameboardChunk() {
    std::cout << "deleting: " << this << std::endl;

	delete this->board_vertex_data;
	delete this->board_select_data;

	glDeleteBuffers(1, &(this->vbo_hex_vert));
	glDeleteBuffers(1, &(this->vbo_hex_color));
	glDeleteBuffers(1, &(this->vbo_hex_indicie));

	glDeleteBuffers(1, &(this->vbo_sel_vert));
	glDeleteBuffers(1, &(this->vbo_sel_color));
	glDeleteBuffers(1, &(this->vbo_sel_indicie));
}


void GameboardChunk::clear() {
	delete this->board_vertex_data;
	delete this->board_select_data;

	this->board_vertex_data = new UniqueDataVector< GLfloat >();
	this->board_select_data = new UniqueDataVector< GLfloat >();
	this->regenerate = true;
}


void GameboardChunk::generate_render_data(Hexagon* curr_hex, double x, double y) {
	curr_hex->generate_vertex_data(x, y, this->board_vertex_data, this->board_select_data);
	curr_hex->parent_chunk = this;
}


void GameboardChunk::write_VBO_data() {
	// TODO: very messy looking... this can be cleaned further...

	// generate a new VBO and get the associated ID

	//glGenBuffers(1, &(this->vbo_hex_vert));
	//std::cout << "vbo_hex_vert:    " << this->vbo_hex_vert << " | " << this->board_vertex_data << " | " << this->board_vertex_data->indicies_size() * 3 << std::endl;

	// bind VBO in order to use
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_hex_vert);
	// upload data to VBO
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(this->board_vertex_data->data()) * this->board_vertex_data->indicies_size() * 3, this->board_vertex_data->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

		// generate a new VBO and get the associated ID
	//glGenBuffers(1, &(this->vbo_hex_color));
	//std::cout << "vbo_hex_color:   " << this->vbo_hex_color << " | " << this->board_vertex_data << " | " << this->board_vertex_data->indicies_size() * 3 << std::endl;

	// bind VBO in order to use
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_hex_color);
	// upload data to VBO
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(this->board_vertex_data->color_data()) * this->board_vertex_data->indicies_size() * 3, this->board_vertex_data->color_data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

		// generate a new VBO and get the associated ID
	//glGenBuffers(1, &(this->vbo_hex_indicie));
	//std::cout << "vbo_hex_indicie: " << this->vbo_hex_indicie << " | " << this->board_vertex_data << " | " << this->board_vertex_data->indicies_size() << std::endl;

	// bind VBO in order to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_hex_indicie);
	// upload data to VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(this->board_vertex_data->indicies_data()) * this->board_vertex_data->indicies_size(), this->board_vertex_data->indicies_data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//-------------------------------------

		// generate a new VBO and get the associated ID
	//glGenBuffers(1, &(this->vbo_sel_vert));
	//std::cout << "vbo_sel_vert:    " << this->vbo_sel_vert << " | " << this->board_select_data << " | " << this->board_select_data->indicies_size() * 3 << std::endl;

	// bind VBO in order to use
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_sel_vert);
	// upload data to VBO
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(this->board_select_data->data()) * this->board_select_data->indicies_size() * 3, this->board_select_data->data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

		// generate a new VBO and get the associated ID
	//glGenBuffers(1, &(this->vbo_sel_color));
	//std::cout << "vbo_sel_color:   " << this->vbo_sel_color << " | " << this->board_select_data << " | " << this->board_select_data->indicies_size() * 3 << std::endl;

	// bind VBO in order to use
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_sel_color);
	// upload data to VBO
	glBufferData(GL_ARRAY_BUFFER, 
		sizeof(this->board_select_data->color_data()) * this->board_select_data->indicies_size() * 3, this->board_select_data->color_data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

		// generate a new VBO and get the associated ID
	//glGenBuffers(1, &(this->vbo_sel_indicie));
	//std::cout << "vbo_sel_indicie: " << this->vbo_sel_indicie << " | " << this->board_select_data << " | " << this->board_select_data->indicies_size() * 3 << std::endl;

	// bind VBO in order to use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_sel_indicie);
	// upload data to VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
		sizeof(this->board_select_data->indicies_data()) * this->board_select_data->indicies_size(), this->board_select_data->indicies_data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}


