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
	this->board_objects = new std::vector< BoardObject* >();
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
		output = new GameboardChunk(base_hex);
		chunk[base_hex] = output;
	} else {
		output = chunk[base_hex];
	}

	output->verify_render_data();

	return output;
}


void Gameboard::render(int x_start, int x_stop, int y_start, int y_stop) {
        for(int j = y_start; j <= y_stop; j+=GlobalConsts::BOARD_CHUNK_SIZE) {
            for(int i = x_start; i <= x_stop; i+=GlobalConsts::BOARD_CHUNK_SIZE) {
                Hexagon* curr_hex = this->hexagon_list->at(i)->at(j);
                //glLoadName(curr_hex->name);

                double x = i * 1.5 * Controller::COS_60;
                double y = j * 1.0 * Controller::SIN_60;

                if(i % 2 != 0) {
                    y += 0.5 * Controller::SIN_60;
                }

		//this->debug_hex = curr_hex;
		GameboardChunk* curr_chunk = this->get_render_data(curr_hex);

		glPushMatrix();
		glTranslatef(x, y, 0);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		// bind VBOs for vertex data
		glBindBuffer(GL_ARRAY_BUFFER, curr_chunk->vbo_terrain_vert);
		glVertexPointer(3, GL_FLOAT, curr_chunk->board_terrain_data->VERTEX_STRIDE, (void*)(curr_chunk->board_terrain_data->VERTEX_OFFSET));

		// color VBO
		//glBindBuffer(GL_ARRAY_BUFFER, curr_chunk->vbo_hex_color);
		glColorPointer( 3, GL_FLOAT, curr_chunk->board_terrain_data->COLOR_STRIDE,  (void*)(curr_chunk->board_terrain_data->COLOR_OFFSET));

		// bind indicie VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curr_chunk->vbo_terrain_indicie);

		// front facing polys
		//glColor3f(0, 1, 0);
		glPolygonMode(GL_FRONT, GL_FILL);
		glDrawElements(GL_TRIANGLES, curr_chunk->board_terrain_data->indicies_size(), GL_UNSIGNED_INT, 0);

		// turn off color array so that we can draw black lines
		glDisableClientState(GL_COLOR_ARRAY);

		// draw back facing black lines
		glCullFace(GL_FRONT);
		glPolygonMode(GL_BACK,  GL_LINE);
		glColor3f(0, 0, 0);
		glDrawElements(GL_TRIANGLES, curr_chunk->board_terrain_data->indicies_size(), GL_UNSIGNED_INT, 0);
		glCullFace(GL_BACK);

		glDisableClientState(GL_VERTEX_ARRAY);

		//----------------------------------------

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		// bind VBOs for vertex array and index array
		glBindBuffer(GL_ARRAY_BUFFER, curr_chunk->vbo_select_vert);
		glVertexPointer(3, GL_FLOAT, curr_chunk->board_select_data->VERTEX_STRIDE, (void*)(curr_chunk->board_select_data->VERTEX_OFFSET));

		// color VBO
		//glBindBuffer(GL_ARRAY_BUFFER, curr_chunk->vbo_sel_color);
		glColorPointer( 3, GL_FLOAT, curr_chunk->board_select_data->COLOR_STRIDE,  (void*)(curr_chunk->board_select_data->COLOR_OFFSET));

		// bind indicie VBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, curr_chunk->vbo_select_indicie);

		// front facing polys
		glPolygonMode(GL_FRONT, GL_FILL);
		glDrawElements(GL_TRIANGLES, curr_chunk->board_select_data->indicies_size(), GL_UNSIGNED_INT, 0);

		glDisableClientState(GL_COLOR_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);

		// bind with 0, so, switch back to normal pointer operation
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glPopMatrix();
            }
        }
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

