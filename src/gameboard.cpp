#include "includes.h"


Gameboard::Gameboard() {
	this->hexagon_list = new RoundVector< RoundVector< Hexagon* >* >();
	this->hexagon_list->reserve(GlobalConsts::BOARD_WIDTH);

	for(int i = 0; i < GlobalConsts::BOARD_WIDTH; i++) {
		RoundVector< Hexagon* >* curr_vect = new RoundVector< Hexagon* >();
		curr_vect->reserve(GlobalConsts::BOARD_HEIGHT);

		this->hexagon_list->push_back(curr_vect);
	}

	this->chunk_map = new std::map< Hexagon*, GameboardChunk* >();
	this->board_object_map = new std::map< Hexagon*,BoardObject* >();
}


void Gameboard::push_back(Hexagon* hex) {
	int total_size = 0;
	for(int i = 0; i < this->hexagon_list->size(); i++) {
		total_size += this->hexagon_list->at(i)->size();
	}

	int i = (int)(total_size % GlobalConsts::BOARD_WIDTH);

	this->hexagon_list->at(i)->push_back(hex);
}


GameboardChunk* Gameboard::get_chunk(Hexagon* base_hex) {
	GameboardChunk* output = NULL;
	std::map< Hexagon*, GameboardChunk* > &chunk = *(this->chunk_map);

	if(this->chunk_map->count(base_hex) == 0) {
		output = new GameboardChunk(base_hex);
		chunk[base_hex] = output;
	} else {
		output = chunk[base_hex];
	}

	output->verify_render_data();

	return output;
}


void Gameboard::render(int x_start, int x_stop, int y_start, int y_stop) {

	// draw game board
        for(int j = y_start; j <= y_stop; j+=GlobalConsts::BOARD_CHUNK_SIZE) {
            for(int i = x_start; i <= x_stop; i+=GlobalConsts::BOARD_CHUNK_SIZE) {
                Hexagon* curr_hex = this->hexagon_list->at(i)->at(j);
		GameboardChunk* curr_chunk = this->get_chunk(curr_hex);

                double x = i * 1.5 * Controller::COS_60;
                double y = j * 1.0 * Controller::SIN_60;

                if(i % 2 != 0) {
                    y += 0.5 * Controller::SIN_60;
                }

		glPushMatrix();
		glTranslatef(x, y, 0);

		curr_chunk->render();

		glPopMatrix();
            }
        }

	// render objects on the game board
	/*for(int i = 0; i < this->board_objects->size(); i++) {
		this->board_objects->at(i)->render();
	}*/
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
	
	/*std::map< Hexagon*, GameboardChunk* > &curr_chunk_map = *(this->chunk_map);
	for(std::map< Hexagon*, GameboardChunk* >::iterator itr = curr_chunk_map.begin(); itr != curr_chunk_map.end(); itr++) {
		(*itr).second->regenerate = true;
	}*/
}


void Gameboard::bind_obj_hex(BoardObject* curr_obj, Hexagon* curr_hex) {
	if(curr_obj->base_hex) {
		this->board_object_map->erase(curr_obj->base_hex);
	}

	std::map< Hexagon*, BoardObject* > &curr_board_object_map = *(this->board_object_map);
	curr_board_object_map[curr_hex] = curr_obj;
	curr_obj->base_hex = curr_hex;
}


