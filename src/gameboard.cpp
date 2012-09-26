#include "includes.h"



GameboardChunk* Gameboard::generate_chunk(Hexagon* base_hex) {
	double base_x = 0;
	double base_y = 0;

	std::cout << "Gameboard::generating: " << base_hex << std::endl;

	GameboardChunk* output = new GameboardChunk();

	double x = base_x;
	double y = base_y;

	const char* dir_ary[] = {"SE", "NE"}; 

	Hexagon* curr_hex = base_hex;

	for(int i = 0; i < GlobalConsts::BOARD_CHUNK_SIZE; i++) {
		const char* direction = dir_ary[i%2];
		std::vector< double >* x_y_diff = GlobalConsts::RENDER_TRAY_COORDS[direction];
		Hexagon* temp_hex = curr_hex;
		double temp_x = x;
		double temp_y = y;

		for(int j = 0; j < GlobalConsts::BOARD_CHUNK_SIZE; j++) {
			output->generate_render_data(temp_hex, temp_x, temp_y);

			temp_hex = temp_hex->get_neighbor("N");
			std::vector< double >* temp_x_y_diff = GlobalConsts::RENDER_TRAY_COORDS["N"];
			temp_x += temp_x_y_diff->at(0);
			temp_y += temp_x_y_diff->at(1);

		}

		curr_hex = curr_hex->get_neighbor(direction);
		x += x_y_diff->at(0);
		y += x_y_diff->at(1);
	}

	output->board_vertex_data->reverse_indicies();
	//output->reverse_verticies();
	std::map< Hexagon*, GameboardChunk* > &curr_chunk_map = *(this->chunk_map);
	curr_chunk_map[base_hex] = output;

	// TODO: This needs to be rewritten...
	// Really you should pass the function a hex, and it should return all the VBO pointers needed...

	//GLuint vec_vboId;
	// generate a new VBO and get the associated ID
	glGenBuffersARB(1, &(output->vbo_id));
	// bind VBO in order to use
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, output->vbo_id);
	// upload data to VBO
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(output->board_vertex_data->data()) * output->board_vertex_data->indicies_size() * 3, output->board_vertex_data->data(), GL_STATIC_DRAW_ARB);
	//std::map< Hexagon*, GLuint > &curr_vbo_ids = *(output->vbo_ids);
	//curr_vbo_ids[base_hex] = vec_vboId;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	//GLuint color_vboId;
	// generate a new VBO and get the associated ID
	glGenBuffersARB(1, &(output->vbo_color));
	// bind VBO in order to use
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, output->vbo_color);
	// upload data to VBO
	glBufferDataARB(GL_ARRAY_BUFFER_ARB, sizeof(output->board_vertex_data->color_data()) * output->board_vertex_data->indicies_size() * 3, output->board_vertex_data->color_data(), GL_STATIC_DRAW_ARB);
	//std::map< Hexagon*, GLuint > &curr_vbo_colors = *(output->vbo_colors);
	//curr_vbo_colors[base_hex] = color_vboId;
	glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);

	//GLuint ind_vboId;
	// generate a new VBO and get the associated ID
	glGenBuffersARB(1, &(output->vbo_indicie));
	// bind VBO in order to use
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, output->vbo_indicie);
	// upload data to VBO
	glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, sizeof(output->board_vertex_data->indicies_data()) * output->board_vertex_data->indicies_size(), output->board_vertex_data->indicies_data(), GL_STATIC_DRAW_ARB);
	//std::map< Hexagon*, GLuint > &curr_vbo_indicies = *(output->vbo_indicies);
	//curr_vbo_indicies[base_hex] = ind_vboId;
	glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);


	/*int* indicies = output->indicies_data();
	float* render_data =  output->data();

	for(int i = 0; i < output->indicies_size(); i ++) {
		std::cout << indicies[i] << " ";
		if(i % 2 != 0) {
			std::cout << "- ";
		}
	}
	std::cout << std::endl << std::endl;*/

	return output;
}


Gameboard::Gameboard() {
	this->hexagon_list = new RoundVector< RoundVector< Hexagon* >* >();
	this->hexagon_list->reserve(GlobalConsts::BOARD_WIDTH);

	for(int i = 0; i < GlobalConsts::BOARD_WIDTH; i++) {
		RoundVector< Hexagon* >* curr_vect = new RoundVector< Hexagon* >();
		curr_vect->reserve(GlobalConsts::BOARD_HEIGHT);

		this->hexagon_list->push_back(curr_vect);
	}

	chunk_map = new std::map< Hexagon*, GameboardChunk* >();
}


/*GameboardChunk* Gameboard::at(Hexagon* hex) {
	std::map< Hexagon*, GameboardChunk* > &chunk = *(this->chunk_map);
	return chunk[hex];
}*/


void Gameboard::push_back(Hexagon* hex) {
	int total_size = 0;
	for(int i = 0; i < this->hexagon_list->size(); i++) {
		total_size += this->hexagon_list->at(i)->size();
	}

	int i = (int)(total_size % GlobalConsts::BOARD_WIDTH);

	this->hexagon_list->at(i)->push_back(hex);

	//int j = this->hexagon_list->at(i)->size()-1;

	/*std::map< Hexagon*, std::vector< int >* > &curr_indicies = *(this->hexagon_indicies);
	curr_indicies[hex] = new std::vector< int >();
	curr_indicies[hex]->push_back(i);
	curr_indicies[hex]->push_back(j);*/

	//std::map< Hexagon*, GameboardChunk* > &chunk = *(this->chunk_map);
	//chunk[hex] = NULL;
}


GameboardChunk* Gameboard::get_render_data(Hexagon* base_hex) {
	/*std::vector< double >* coord_vect = new std::vector< double >();
	coord_vect->reserve(2);
	coord_vect->push_back(base_x);
	coord_vect->push_back(base_y);*/

	GameboardChunk* output = NULL;
	std::map< Hexagon*, GameboardChunk* > &chunk = *(this->chunk_map);

	if(this->chunk_map->count(base_hex) == 0) {
		output = this->generate_chunk(base_hex);

		chunk[base_hex] = output;
	} else {
		output = chunk[base_hex];
	}

	return output;
}


RoundVector< RoundVector< Hexagon* >* >* Gameboard::get_hexagon_list() {
	// TODO: This should return a copy... so things do get added without being added to the other constructs
	return this->hexagon_list;
}

