#include "includes.h"


int Hexagon::name_count = 0;

RoundVector< RoundVector<GLdouble>* >* get_rot_coords() {
	GLdouble rot_data[][2] = {
		{-0.5,   0.0},
		{-0.25,  0.433012701892},
		{ 0.25,  0.433012701892},
		{ 0.5,   0.0},
		{ 0.25, -0.433012701892},
		{-0.25, -0.433012701892}
	};

	RoundVector< RoundVector<GLdouble>* >* output = new RoundVector< RoundVector<GLdouble>* >();
	RoundVector<GLdouble>* row;
	for(int i = 0; i < 6; i++) {
		row = new RoundVector<GLdouble>();
		row->push_back(rot_data[i][0]);
		row->push_back(rot_data[i][1]);
		output->push_back(row);
	}

	return output;
}

const RoundVector< RoundVector<GLdouble>* >* Hexagon::ROT_COORDS = get_rot_coords();


const RoundVector<const char*>* init_vert_pos() {
	RoundVector<const char*>* output = new RoundVector<const char*>();
	output->reserve(6);

	/* ORDER IS IMPORTANT HERE */
	output->push_back("W");
	output->push_back("NW");
	output->push_back("NE");
	output->push_back("E");
	output->push_back("SE");
	output->push_back("SW");

	return output;
}

const RoundVector<const char*>* Hexagon::VERTEX_POSITIONS = init_vert_pos();


const RoundVector<const char*>* init_neigh_dir() {
	RoundVector<const char*>* output = new RoundVector<const char*>();
	output->reserve(6);

	/* ORDER IS IMPORTANT HERE */
	output->push_back("N");
	output->push_back("NE");
	output->push_back("SE");
	output->push_back("S");
	output->push_back("SW");
	output->push_back("NW");

	return output;
}

const RoundVector<const char*>* Hexagon::NEIGHBOR_DIRECTION = init_neigh_dir();


void Hexagon::base_init() {
	this->select_color = NULL;

	for(int i = 0; i < 6; i++) {
		//this->set_vertex(this->VERTEX_POSITIONS->at(i), new Vertex(0, 0, 0, 0));
		this->set_vertex(this->VERTEX_POSITIONS->at(i), new Vertex(0));
	}

	this->name_count++;
	this->name = this->name_count;

	this->last_x = 0;
	this->last_y = 0;

	this->parent_chunk = NULL;

	this->hex_color = NULL;
	this->set_hex_color(0, 1, 0);

	this->improvements["road"] = false;
}

Hexagon::Hexagon() {
	this->base_init();
}

void Hexagon::set_vertex(const char* position, Vertex* vertex) {
	this->verticies[position] = vertex;
}

Vertex* Hexagon::get_vertex(const char* position) {
	return this->verticies[position];
}

/*void Hexagon::set_border_color(GLdouble red, GLdouble green, GLdouble blue) {
	for(int i = 0; i < 6; i++) {
		this->verticies[this->VERTEX_POSITIONS->at(i)]->set_color(red, green, blue);
	}
}

void Hexagon::set_border_color(std::vector<GLdouble> rgb) {
	for(int i = 0; i < 6; i++) {
		this->verticies[this->VERTEX_POSITIONS->at(i)]->set_color(rgb);
	}
}*/

void Hexagon::set_improvement(const char* key, bool value) {
	this->improvements[key] = value;
}

bool Hexagon::get_improvement(const char* key){
	return this->improvements[key];
}

void Hexagon::set_select_color(GLdouble red, GLdouble green, GLdouble blue) {
	bool changed = false;

	if(!this->select_color) {
		this->select_color = new std::vector< GLdouble >();
		changed = true;
	} else {
		// assume this will be 3 in length if we have a pointer...
		changed = color_changed(
			red, green, blue,
			this->select_color->at(0), this->select_color->at(1), this->select_color->at(2)
		);
	}

	this->select_color->clear();
	this->select_color->push_back(red);
	this->select_color->push_back(green);
	this->select_color->push_back(blue);

	if(this->parent_chunk && changed) {
		this->parent_chunk->regenerate_select = true;
	}
}

void Hexagon::set_select() {
    this->set_select_color(1.0, 1.0, 0.0);

    Controller* curr_ctrl = Controller::get_controller();
    std::map< Hexagon*, BoardObject* > &curr_board_object_map = *(curr_ctrl->gameboard->board_object_map);

    if(curr_board_object_map[this]) {
        curr_board_object_map[this]->set_selected(true);
    }
}

void Hexagon::clear_select() {
    this->clear_select_color();

    Controller* curr_ctrl = Controller::get_controller();
    std::map< Hexagon*, BoardObject* > &curr_board_object_map = *(curr_ctrl->gameboard->board_object_map);

    if(curr_board_object_map[this]) {
        curr_board_object_map[this]->set_selected(false);
    }
}

void Hexagon::set_select_color(std::vector<GLdouble> rgb) {
	if(!this->select_color) {
		this->select_color = new std::vector< GLdouble >();
	}

	this->set_select_color(rgb[0], rgb[1], rgb[2]);
}

void Hexagon::clear_select_color() {
	bool changed = false;

	if(this->select_color) {
		delete this->select_color;
		this->select_color = NULL;
		// assume pointer was 3 in length...
		changed = true;
	}

	if(this->parent_chunk && changed) {
		this->parent_chunk->regenerate_select = true;
	}
}

std::vector<GLdouble>* Hexagon::get_select_color() {
	return this->select_color;
}

void Hexagon::set_hex_color(GLdouble red, GLdouble green, GLdouble blue) {
	if(!this->hex_color) {
		this->hex_color = new std::vector< GLdouble >();
	}

	this->hex_color->clear();
	this->hex_color->push_back(red);
	this->hex_color->push_back(green);
	this->hex_color->push_back(blue);

	/*if(this->parent_chunk) {
		this->parent_chunk->regenerate = true;
	}*/
}

void Hexagon::set_hex_color(std::vector<GLdouble> rgb) {
	if(!this->hex_color) {
		this->hex_color = new std::vector< GLdouble >();
	}

	this->set_hex_color(rgb[0], rgb[1], rgb[2]);
}

std::vector<GLdouble>* Hexagon::get_hex_color() {
	return this->hex_color;
}

void Hexagon::set_height(GLdouble height) {
	for(int i = 0; i < 6; i++) {
		this->verticies[this->VERTEX_POSITIONS->at(i)]->set_height(height);
	}
}

void Hexagon::add_height(GLdouble height) {
	for(int i = 0; i < 6; i++) {
		Vertex* curr_vert = this->verticies[this->VERTEX_POSITIONS->at(i)];
		curr_vert->set_height(curr_vert->get_height() + height);
	}
}

GLdouble Hexagon::get_height() {
	GLdouble output = 0;

	for(int i = 0; i < 6; i++) {
		output += this->verticies[this->VERTEX_POSITIONS->at(i)]->get_height();
	}

	return output / 6.0;
}

GLdouble Hexagon::get_slope() {
	GLdouble min_height = this->verticies[this->VERTEX_POSITIONS->at(0)]->get_height();
	GLdouble max_height = this->verticies[this->VERTEX_POSITIONS->at(0)]->get_height();

	for(int i = 1; i < 6; i++) {
		GLdouble curr_height = this->verticies[this->VERTEX_POSITIONS->at(i)]->get_height();
		if(curr_height < min_height) {
			min_height = curr_height;
		} else if (curr_height > max_height) {
			max_height = curr_height;
		}
	}

	return max_height - min_height;
}

bool Hexagon::is_pathable() {
	bool output = this->get_slope() <= GlobalConsts::MAX_PATHABLE_SLOPE && this->get_board_object() == NULL;

    return output;
}

std::vector< Hexagon* >* Hexagon::find_path(Hexagon* start_hex, Hexagon* goal_hex) {
	/* A* path finding */
	std::vector< Hexagon* > open_list = {start_hex};
	std::vector< Hexagon* > closed_list;

	std::map< Hexagon*, Hexagon* > parent_map;

	std::map< Hexagon*, GLdouble > g_score;
	g_score[start_hex] = 0.0;

	std::map< Hexagon*, GLdouble > h_score;
	h_score[start_hex] = Hexagon::get_h_score(start_hex, goal_hex);

	std::map< Hexagon*, GLdouble > f_score;
	f_score[start_hex] = h_score[start_hex];

	Hexagon* curr_hex = start_hex;
    Hexagon* min_f_hex = NULL;
    std::vector<Hexagon*>* neighbors = NULL;

	while(open_list.size() > 0) {
        min_f_hex = open_list[0];

        int open_list_size = open_list.size();
        for(int i = 1; i < open_list_size; i++) {
            if(f_score[open_list[i]] < f_score[min_f_hex]) {
                min_f_hex = open_list[i];
            }
        }

        curr_hex = min_f_hex;

        if(curr_hex == goal_hex) {
        	return Hexagon::reconstruct_path(parent_map, goal_hex);
        }

        //if GlobalConsts.PATH_SHOW_SEARCH and curr_node.is_pathable():
        //    curr_node.set_select_color(1, 0, 0)

        remove_from_vector(&open_list, curr_hex);
        if(!item_in_vector(&closed_list, curr_hex)) {
        	closed_list.push_back(curr_hex);
        }

        neighbors = curr_hex->get_neighbors();
        int neighbors_size = neighbors->size();
        for(int i = 0; i < neighbors_size; i++) {
        	Hexagon* curr_neighbor = neighbors->at(i);

            if(!curr_neighbor->is_pathable() && !item_in_vector(&closed_list, curr_neighbor)) {
                closed_list.push_back(curr_neighbor);
            }

            if(item_in_vector(&closed_list, curr_neighbor)) {
                continue;
            }

            GLdouble tentative_g_score =  Hexagon::get_g_score(curr_hex, curr_neighbor) + g_score[curr_hex];

        	if(!item_in_vector(&open_list, curr_neighbor) || tentative_g_score < g_score[curr_neighbor]) {
        		parent_map[curr_neighbor] = curr_hex;
                if(!item_in_vector(&open_list, curr_neighbor)) {
        			open_list.push_back(curr_neighbor);
                }
    			g_score[curr_neighbor] = Hexagon::get_g_score(curr_hex, curr_neighbor) + g_score[curr_hex];
    			h_score[curr_neighbor] = Hexagon::get_h_score(curr_neighbor, goal_hex);
    			f_score[curr_neighbor] = g_score[curr_neighbor] + h_score[curr_neighbor];
        	}
        }

        delete neighbors;
	}

	return new std::vector< Hexagon* >();

}

GLdouble Hexagon::get_g_score(Hexagon* start_hex, Hexagon* end_hex) {
    if(end_hex->get_improvement("road")) {
        return 1;
    } else {
        return pow(end_hex->get_slope() + 1.1, 10);
    }
}

GLdouble Hexagon::get_h_score(Hexagon* start_hex, Hexagon* end_hex) {
	return (dist_between(start_hex, end_hex) * 1.0 / 0.866025) * 1.5; // one unit...
}

std::vector< Hexagon* >* Hexagon::reconstruct_path(std::map< Hexagon*, Hexagon* > parent_map, Hexagon* curr_hex) {
	Hexagon* parent_hex = parent_map[curr_hex];
	std::vector< Hexagon* >* output = NULL;

    if(parent_hex) {
        output = Hexagon::reconstruct_path(parent_map, parent_hex);
    } else {
    	output = new std::vector< Hexagon* >();
	}

	output->push_back(curr_hex);
	return output;
}

GLdouble Hexagon::dist_between(Hexagon* hex_a, Hexagon* hex_b) {
	GLdouble board_width  = GlobalConsts::BOARD_WIDTH  * 1.5 * GlobalConsts::COS_60;
	GLdouble board_height = GlobalConsts::BOARD_HEIGHT * 1.0 * GlobalConsts::SIN_60;

	GLdouble x_diff = hex_b->last_x - hex_a->last_x;
	GLdouble y_diff = hex_b->last_y - hex_a->last_y;

	GLdouble half_width  = board_width  / 2.0;
    GLdouble half_height = board_height / 2.0;

    if(x_diff > half_width) {
        x_diff -= board_width;
    } else if(x_diff < -half_width) {
        x_diff += board_width;
    }

    if(y_diff > half_height) {
        y_diff -= board_height;
    } else if(y_diff < -half_height) {
        y_diff += board_height;
    }

    return sqrt(pow(x_diff, 2.0) + pow(y_diff, 2.0));
}


void Hexagon::set_neighbor(const char* position, Hexagon* neighbor_hex) {
	this->neighbor_hexagons[position] = neighbor_hex;
}


Hexagon* Hexagon::get_neighbor(const char* position) {
	return this->neighbor_hexagons[position];
}

std::vector<Hexagon*>* Hexagon::get_neighbors() {
    std::vector<Hexagon*>* output = new std::vector<Hexagon*>();

    for(int i = 0; i < Hexagon::NEIGHBOR_DIRECTION->size(); i++) {
    	output->push_back(this->get_neighbor(Hexagon::NEIGHBOR_DIRECTION->at(i)));
    }

    return output;
}

void Hexagon::render_for_select(GLdouble x, GLdouble y) {
	glBegin(GL_TRIANGLE_FAN);
		for(int i = 0; i < 6; i++) {
			Vertex* curr_vert = this->verticies[this->VERTEX_POSITIONS->at(i)];

			//glColor3df(curr_vert->get_color()->data());
			glColor3f(1, 1, 1);
			glVertex3f(x + ROT_COORDS->at(i)->at(0), y + ROT_COORDS->at(i)->at(1), curr_vert->get_height());
		}
	glEnd();
}


void Hexagon::generate_vertex_data(GLdouble x, GLdouble y, UniqueDataVector< GLdouble >* vertex_data) {
	Vertex* curr_vert = NULL;
	std::vector< GLdouble >* curr_color = NULL;

	if(this->improvements["road"]) {
		curr_color = &GlobalConsts::COLOR_GREY;
	} else {
		curr_color = this->get_hex_color();
	}

	for(int i = 1; i < 5; i++) {
		curr_vert = this->verticies[this->VERTEX_POSITIONS->at(0)];

		vertex_data->push_back(
			x + Hexagon::ROT_COORDS->at(0)->at(0), y + Hexagon::ROT_COORDS->at(0)->at(1), curr_vert->get_height(),
			curr_color->at(0), curr_color->at(1), curr_color->at(2)
		);

		curr_vert = this->verticies[this->VERTEX_POSITIONS->at(i)];

		vertex_data->push_back(
			x + Hexagon::ROT_COORDS->at(i)->at(0), y + Hexagon::ROT_COORDS->at(i)->at(1), curr_vert->get_height(),
			curr_color->at(0), curr_color->at(1), curr_color->at(2)
		);

		curr_vert = this->verticies[this->VERTEX_POSITIONS->at(i+1)];

		vertex_data->push_back(
			x + Hexagon::ROT_COORDS->at(i+1)->at(0), y + Hexagon::ROT_COORDS->at(i+1)->at(1), curr_vert->get_height(),
			curr_color->at(0), curr_color->at(1), curr_color->at(2)
		);
	}
}


void Hexagon::generate_select_data(GLdouble x, GLdouble y, UniqueDataVector< GLdouble >* select_data) {
	Vertex* curr_vert = NULL;
	std::vector< GLdouble >* curr_color = this->get_select_color();

        for(int i = 2; i < 6; i++) {
		curr_vert = this->verticies[this->VERTEX_POSITIONS->at(i)];

        if(curr_color) {
		    select_data->push_back(
    			Hexagon::ROT_COORDS->at(0  )->at(0) + x, Hexagon::ROT_COORDS->at(0  )->at(1) + y, this->verticies[this->VERTEX_POSITIONS->at(0  )]->get_height() + 0.01,
    			curr_color->at(0), curr_color->at(1), curr_color->at(2)
		    );

		    select_data->push_back(
    			Hexagon::ROT_COORDS->at(i-1)->at(0) + x, Hexagon::ROT_COORDS->at(i-1)->at(1) + y, this->verticies[this->VERTEX_POSITIONS->at(i-1)]->get_height() + 0.01,
    			curr_color->at(0), curr_color->at(1), curr_color->at(2)
		    );

		    select_data->push_back(
    			Hexagon::ROT_COORDS->at(i  )->at(0) + x, Hexagon::ROT_COORDS->at(i  )->at(1) + y, this->verticies[this->VERTEX_POSITIONS->at(i  )]->get_height() + 0.01,
    			curr_color->at(0), curr_color->at(1), curr_color->at(2)
		    );
		}
	}
}


/*void Hexagon::generate_object_data(GLdouble x, GLdouble y, UniqueDataVector< GLdouble >* object_data) {
	Controller* curr_ctrl = Controller::get_controller();

	std::map< Hexagon*, BoardObject* > &curr_board_object_map = *(curr_ctrl->gameboard->board_object_map);
    //std::vector< GLdouble > curr_rbg;
    //std::vector<int> v { 34,23 };

    BoardObject* curr_board_object = curr_board_object_map[this];

	if(curr_board_object) {
        std::vector< GLdouble > curr_xyz;
        std::vector< GLdouble >* curr_color = NULL;

	    if(curr_board_object->get_selected()) {
            curr_color = curr_board_object->selected_color;
	    } else {
            curr_color = curr_board_object->color;
	    }

        object_data->set_tex_data(curr_board_object->get_tex_data());

        curr_xyz = {-0.5 + x, y, this->get_height()};
        object_data->push_back(&curr_xyz, curr_color);

        curr_xyz = {-0.5 + x, y, this->get_height() + 2.0};
        object_data->push_back(&curr_xyz, curr_color);

        curr_xyz = { 0.5 + x, y, this->get_height() + 2.0};
        object_data->push_back(&curr_xyz, curr_color);

        curr_xyz = { 0.5 + x, y, this->get_height() + 2.0};
        object_data->push_back(&curr_xyz, curr_color);

        curr_xyz = { 0.5 + x, y, this->get_height()};
        object_data->push_back(&curr_xyz, curr_color);

        curr_xyz = {-0.5 + x, y, this->get_height()};
        object_data->push_back(&curr_xyz, curr_color);
	}
}*/


BoardObject* Hexagon::get_board_object() {
    Controller* curr_ctrl = Controller::get_controller();
    std::map< Hexagon*, BoardObject* > &curr_board_object_map = *(curr_ctrl->gameboard->board_object_map);
    BoardObject* output = curr_board_object_map[this];

    return output;
}

