#include "includes.h"


GameboardBase::GameboardBase() {
	this->hexagon_list = new RoundVector< RoundVector< Hexagon* >* >();
	this->hexagon_list->reserve(GlobalConsts::BOARD_WIDTH);

	for(int i = 0; i < GlobalConsts::BOARD_WIDTH; i++) {
		RoundVector< Hexagon* >* curr_vect = new RoundVector< Hexagon* >();
		curr_vect->reserve(GlobalConsts::BOARD_HEIGHT);

		this->hexagon_list->push_back(curr_vect);
	}

	this->board_object_map = new std::map< Hexagon*,BoardObject* >();
}


void GameboardBase::push_back(Hexagon* hex) {
	int total_size = 0;
	for(int i = 0; i < this->hexagon_list->size(); i++) {
		total_size += this->hexagon_list->at(i)->size();
	}

	int i = (int)(total_size % GlobalConsts::BOARD_WIDTH);

	this->hexagon_list->at(i)->push_back(hex);
}


RoundVector< RoundVector< Hexagon* >* >* GameboardBase::get_hexagon_list() {
	// TODO: This should return a copy... so things do get added without being added to the other constructs
	return this->hexagon_list;
}


void GameboardBase::bind_obj_hex(BoardObject* curr_obj, Hexagon* curr_hex) {
	if(curr_obj->base_hex) {
		/*if(curr_obj->base_hex->parent_chunk) {
			curr_obj->base_hex->parent_chunk->regenerate_object = true;
		}*/
		this->board_object_map->erase(curr_obj->base_hex);
	}

	std::map< Hexagon*, BoardObject* > &curr_board_object_map = *(this->board_object_map);
	curr_board_object_map[curr_hex] = curr_obj;
	curr_obj->base_hex = curr_hex;
	/*if(curr_hex->parent_chunk) {
	    curr_hex->parent_chunk->regenerate_object = true;
	}*/
}

// -----------------------------------

Gameboard::Gameboard() {
	this->chunk_map = new std::map< Hexagon*, GameboardChunk* >();
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


void Gameboard::render(Controller* curr_ctrl, int x_start, int x_stop, int y_start, int y_stop) {
    // draw game board
    for(int j = y_start; j <= y_stop; j+=GlobalConsts::BOARD_CHUNK_SIZE) {
        for(int i = x_start; i <= x_stop; i+=GlobalConsts::BOARD_CHUNK_SIZE) {
            Hexagon* curr_hex = this->hexagon_list->at(i)->at(j);
            GameboardChunk* curr_chunk = this->get_chunk(curr_hex);

            double x = i * 1.5 * GlobalConsts::COS_60;
            double y = j * 1.0 * GlobalConsts::SIN_60;

            if(i % 2 != 0) {
                y += 0.5 * GlobalConsts::SIN_60;
            }

            glPushMatrix();
            glTranslatef(x, y, 0);

            curr_chunk->render();

            glPopMatrix();
        }
    }
}

//---------------------------------------

