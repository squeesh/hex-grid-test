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
	output->board_select_data->reverse_indicies();
	std::map< Hexagon*, GameboardChunk* > &curr_chunk_map = *(this->chunk_map);
	curr_chunk_map[base_hex] = output;

	output->write_VBO_data();

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

void Gameboard::clear() {
	std::map< Hexagon*, GameboardChunk* > &curr_chunk_map = *(this->chunk_map);
	for(std::map< Hexagon*, GameboardChunk* >::iterator itr = curr_chunk_map.begin(); itr != curr_chunk_map.end(); itr++) {
		delete (*itr).second;
	}

	this->chunk_map->clear();
}

