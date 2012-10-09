#include "includes.h"

GameboardChunk::GameboardChunk(Hexagon* base_hex) {
	this->base_hex = base_hex;

	this->board_terrain_data = NULL;
	this->board_select_data = NULL;
	
	/*this->vbo_terrain_vert = 0;
	this->vbo_terrain_indicie = 0;

	this->vbo_select_vert = 0;
	this->vbo_select_indicie = 0;*/

	/*glGenBuffers(1, &(this->vbo_terrain_vert));
	glGenBuffers(1, &(this->vbo_terrain_indicie));

	glGenBuffers(1, &(this->vbo_select_vert));
	glGenBuffers(1, &(this->vbo_select_indicie));*/

	this->regenerate_terrain = true;
	this->regenerate_select = true;
}


GameboardChunk::~GameboardChunk() {
	std::cout << "deleting: " << this << std::endl;

	if(this->board_terrain_data) {
		delete this->board_terrain_data;
	}

	if(this->board_select_data) {
		delete this->board_select_data;
	}

	/*glDeleteBuffers(1, &(this->vbo_terrain_vert));
	glDeleteBuffers(1, &(this->vbo_terrain_indicie));

	glDeleteBuffers(1, &(this->vbo_select_vert));
	glDeleteBuffers(1, &(this->vbo_select_indicie));*/
}


void GameboardChunk::clear_terrain() {
	if(this->board_terrain_data) {
		delete this->board_terrain_data;
	}

	this->board_terrain_data = new UniqueDataVector< GLfloat >();
	this->regenerate_terrain = true;
}


void GameboardChunk::clear_select() {
	if(this->board_select_data) {
		delete this->board_select_data;
	}

	this->board_select_data = new UniqueDataVector< GLfloat >();
	this->regenerate_select = true;
}

void GameboardChunk::generate_render_data(Hexagon* curr_hex, double x, double y) {
	// TODO: I don't like this logic being here.. this should be moved out...
	if(this->regenerate_terrain) {
		curr_hex->generate_vertex_data(x, y, this->board_terrain_data);
	} 

	if(this->regenerate_select) {
		curr_hex->generate_select_data(x, y, this->board_select_data);
	} 

	curr_hex->parent_chunk = this;
}


void GameboardChunk::generate_chunk_data() {
	double base_x = 0;
	double base_y = 0;

	std::cout << "Gameboard::generating: " << this->base_hex << std::endl;

	double x = base_x;
	double y = base_y;

	const char* dir_ary[] = {"SE", "NE"}; 

	Hexagon* curr_hex = this->base_hex;

	for(int i = 0; i < GlobalConsts::BOARD_CHUNK_SIZE; i++) {
		const char* direction = dir_ary[i%2];
		std::vector< double >* x_y_diff = GlobalConsts::RENDER_TRAY_COORDS[direction];
		Hexagon* temp_hex = curr_hex;
		double temp_x = x;
		double temp_y = y;

		for(int j = 0; j < GlobalConsts::BOARD_CHUNK_SIZE; j++) {
			this->generate_render_data(temp_hex, temp_x, temp_y);

			temp_hex = temp_hex->get_neighbor("N");
			std::vector< double >* temp_x_y_diff = GlobalConsts::RENDER_TRAY_COORDS["N"];
			temp_x += temp_x_y_diff->at(0);
			temp_y += temp_x_y_diff->at(1);
		}

		curr_hex = curr_hex->get_neighbor(direction);
		x += x_y_diff->at(0);
		y += x_y_diff->at(1);
	}

	if(this->regenerate_terrain) {
		this->board_terrain_data->reverse_indicies();
	}

	if(this->regenerate_select) {
		this->board_select_data->reverse_indicies();
	}
}


void GameboardChunk::verify_render_data() {
	if(this->regenerate_terrain || this->regenerate_select) {
		if(this->regenerate_terrain) {
			this->clear_terrain();
		}

		if(this->regenerate_select) {
			this->clear_select();
		}

		this->generate_chunk_data();
		this->write_VBO_data();

		this->regenerate_terrain = false;
		this->regenerate_select = false;
	}
}

void GameboardChunk::write_VBO_data() {
	this->board_terrain_data->write_VBO_data(GL_STATIC_DRAW);
	this->board_select_data->write_VBO_data(GL_STATIC_DRAW);
}

void GameboardChunk::render() {
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	this->board_terrain_data->bind_for_draw(GL_FLOAT);

	glPolygonMode(GL_FRONT, GL_FILL);
	glDrawElements(GL_TRIANGLES, this->board_terrain_data->indicies_size(), GL_UNSIGNED_INT, 0);

	// turn off color array so that we can draw black lines
	glDisableClientState(GL_COLOR_ARRAY);

	// draw back facing black lines
	glCullFace(GL_FRONT);
	glPolygonMode(GL_BACK,  GL_LINE);
	glColor3f(0, 0, 0);
	glDrawElements(GL_TRIANGLES, this->board_terrain_data->indicies_size(), GL_UNSIGNED_INT, 0);
	glCullFace(GL_BACK);

	glDisableClientState(GL_VERTEX_ARRAY);

	//----------------------------------------

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	this->board_select_data->bind_for_draw(GL_FLOAT);

	// front facing polys
	glPolygonMode(GL_FRONT, GL_FILL);
	glDrawElements(GL_TRIANGLES, this->board_select_data->indicies_size(), GL_UNSIGNED_INT, 0);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	// bind with 0, so, switch back to normal pointer operation
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


