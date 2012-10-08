#include "includes.h"

GameboardChunk::GameboardChunk() {
	//this->board_vertex_data = new UniqueDataVector< GLfloat >();
	//this->board_select_data = new UniqueDataVector< GLfloat >();

	this->board_vertex_data = NULL;
	this->board_select_data = NULL;
	
	this->vbo_hex_vert = 0;
	//this->vbo_hex_color = 0;
	this->vbo_hex_indicie = 0;

	this->vbo_sel_vert = 0;
	//this->vbo_sel_color = 0;
	this->vbo_sel_indicie = 0;

	glGenBuffers(1, &(this->vbo_hex_vert));
	//glGenBuffers(1, &(this->vbo_hex_color));
	glGenBuffers(1, &(this->vbo_hex_indicie));
	glGenBuffers(1, &(this->vbo_sel_vert));
	//glGenBuffers(1, &(this->vbo_sel_color));
	glGenBuffers(1, &(this->vbo_sel_indicie));

	this->regenerate = true;
}


GameboardChunk::~GameboardChunk() {
	std::cout << "deleting: " << this << std::endl;

	if(this->board_vertex_data) {
		delete this->board_vertex_data;
	}

	if(this->board_select_data) {
		delete this->board_select_data;
	}

	glDeleteBuffers(1, &(this->vbo_hex_vert));
	//glDeleteBuffers(1, &(this->vbo_hex_color));
	glDeleteBuffers(1, &(this->vbo_hex_indicie));

	glDeleteBuffers(1, &(this->vbo_sel_vert));
	//glDeleteBuffers(1, &(this->vbo_sel_color));
	glDeleteBuffers(1, &(this->vbo_sel_indicie));
}


void GameboardChunk::clear() {
	std::cout << "clear()..." << std::endl;
	if(this->board_vertex_data) {
		delete this->board_vertex_data;
	}

	if(this->board_select_data) {
		delete this->board_select_data;
	}
	this->board_vertex_data = new UniqueDataVector< GLfloat >();
	this->board_select_data = new UniqueDataVector< GLfloat >();
	this->regenerate = true;
}


void GameboardChunk::generate_render_data(Hexagon* curr_hex, double x, double y) {
	curr_hex->generate_vertex_data(x, y, this->board_vertex_data, this->board_select_data);
	curr_hex->parent_chunk = this;
}


void GameboardChunk::write_VBO_data() {
	std::cout << "VBO Start" << std::endl;
	std::cout << this->board_vertex_data->vector_size() << " | " << this->board_vertex_data->indicies_size() << " - " ;
	std::cout << this->board_select_data->vector_size() << " | " << this->board_select_data->indicies_size() << std::endl;

	GLsizeiptr hex_vert_size = sizeof(GLfloat) * this->board_vertex_data->vector_size();

	std::cout << "A: " << this->vbo_hex_vert << " | " << hex_vert_size << " | " << this->board_vertex_data->vector_data() << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_hex_vert);
	glBufferData(GL_ARRAY_BUFFER, hex_vert_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, hex_vert_size, this->board_vertex_data->vector_data());

	GLsizeiptr hex_indicie_size = sizeof(GLuint) * this->board_vertex_data->indicies_size();

	std::cout << "C: " << this->vbo_hex_indicie << " | " << hex_indicie_size << std::endl;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_hex_indicie);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, hex_indicie_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, hex_indicie_size, this->board_vertex_data->indicies_data());

	//-------------------------------------

	GLsizeiptr sel_vert_size = sizeof(GLfloat) * this->board_select_data->vector_size();

	std::cout << "D: " << this->vbo_sel_vert << " | " << sel_vert_size << std::endl;
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_sel_vert);
	glBufferData(GL_ARRAY_BUFFER, sel_vert_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sel_vert_size, this->board_select_data->vector_data());

	GLsizeiptr sel_indicie_size = sizeof(GLuint) * this->board_select_data->indicies_size();

	std::cout << "F: " << this->vbo_sel_indicie << " | " << sel_indicie_size << std::endl;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_sel_indicie);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sel_indicie_size, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sel_indicie_size, this->board_select_data->indicies_data());

	std::cout << "VBO End" << " | " << glGetError() << std::endl;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	std::cout << "VBO Stop: " << glGetError() << " | " << GL_NO_ERROR << std::endl << std::endl;

}


