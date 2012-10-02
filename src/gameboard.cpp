#include "includes.h"



GameboardChunk* Gameboard::generate_chunk(Hexagon* base_hex) {
	double base_x = 0;
	double base_y = 0;

	std::cout << "Gameboard::generating: " << base_hex << std::endl;

	std::map< Hexagon*, GameboardChunk* > &chunk = *(this->chunk_map);
	GameboardChunk* output = chunk[base_hex];
	//GameboardChunk* output = new GameboardChunk();

	double x = base_x;
	double y = base_y;

	const char* dir_ary[] = {"SE", "NE"}; 

	Hexagon* curr_hex = base_hex;


	for(int i = 0; i < GlobalConsts::BOARD_CHUNK_SIZE; i++) {
	    //std::cout << "i: " << i << std::endl;

		const char* direction = dir_ary[i%2];
		std::vector< double >* x_y_diff = GlobalConsts::RENDER_TRAY_COORDS[direction];
		Hexagon* temp_hex = curr_hex;
		double temp_x = x;
		double temp_y = y;

		for(int j = 0; j < GlobalConsts::BOARD_CHUNK_SIZE; j++) {
		    //std::cout << "j: " << j << std::endl;
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

	//std::cout << "reverse..." << std::endl;

	/*GLfloat* temp_v = output->board_vertex_data->vector_data->data();

	std::cout << "size: " << output->board_vertex_data->vector_data->size() << std::endl;
	//std::cout << "cap:  " << output->board_vertex_data->indicies->capacity() << std::endl;

	for(int i = 0; i < output->board_vertex_data->vector_data->size(); i+=3) {
		std::cout << temp_v[i] << ", " << temp_v[i+1] << ", " << temp_v[i+2] << std::endl;
	}
	std::cout << std::endl << std::endl;
	std::cout << "------" << std::endl << std::endl;
	std::cout << "size: " << output->board_vertex_data->indicies->size() << std::endl;

	//output->board_vertex_data->reverse_indicies();
	GLuint* temp_i = output->board_vertex_data->indicies->data();

	for(int i = 0; i < output->board_vertex_data->indicies->size(); i+=3) {
		std::cout << temp_i[i] << ", " << temp_i[i+1] << ", " << temp_i[i+2] << std::endl;
	}
	std::cout << std::endl << std::endl;*/

	//output->board_vertex_data->reverse_indicies();
	output->board_vertex_data->reverse_indicies();
	output->board_select_data->reverse_indicies();
	std::map< Hexagon*, GameboardChunk* > &curr_chunk_map = *(this->chunk_map);
	curr_chunk_map[base_hex] = output;

	//std::cout << "writing..." << std::endl;
	//std::cout << "returning: " << output << std::endl;

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


void Gameboard::push_back(Hexagon* hex) {
	int total_size = 0;
	for(int i = 0; i < this->hexagon_list->size(); i++) {
		total_size += this->hexagon_list->at(i)->size();
	}

	int i = (int)(total_size % GlobalConsts::BOARD_WIDTH);

	this->hexagon_list->at(i)->push_back(hex);
}


GameboardChunk* Gameboard::get_render_data(Hexagon* base_hex) {
	GameboardChunk* output = NULL;
	std::map< Hexagon*, GameboardChunk* > &chunk = *(this->chunk_map);

	if(this->chunk_map->count(base_hex) == 0) {
		GameboardChunk* output = new GameboardChunk();
		chunk[base_hex] = output;
	} 

	if(chunk[base_hex]->regenerate) {
		chunk[base_hex]->clear();
		output = this->generate_chunk(base_hex);
		output->write_VBO_data();

		output->regenerate = false;	
	} else {
		output = chunk[base_hex];
	}

	return output;
}


RoundVector< RoundVector< Hexagon* >* >* Gameboard::get_hexagon_list() {
	// TODO: This should return a copy... so things do get added without being added to the other constructs
	return this->hexagon_list;
}

void Gameboard::clear() {
	// Possibly fixed a bug... It seems deleteing the chunks then regenerating them wasn't working as intended
	// due to concurrency on windows? Memory was flagged for deletion, but would be deleted when it wasn't safe...
	// so instead of deleteing... we now keep our assigned opengl memory locations and just rewrite to it...

	/*std::map< Hexagon*, GameboardChunk* > &curr_chunk_map = *(this->chunk_map);
	for(std::map< Hexagon*, GameboardChunk* >::iterator itr = curr_chunk_map.begin(); itr != curr_chunk_map.end(); itr++) {
		delete (*itr).second;
	}

	this->chunk_map->clear();*/

	std::cout << "no call this func!" << std::endl;
	
	std::map< Hexagon*, GameboardChunk* > &curr_chunk_map = *(this->chunk_map);
	for(std::map< Hexagon*, GameboardChunk* >::iterator itr = curr_chunk_map.begin(); itr != curr_chunk_map.end(); itr++) {
		(*itr).second->regenerate = true;
	}
}

