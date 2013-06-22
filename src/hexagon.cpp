#include "includes.h"


int Hexagon::name_count = 0;

RoundVector< RoundVector<double>* >* get_rot_coords() {
	double rot_data[][2] = {
		{-0.5,   0.0},
		{-0.25,  0.433012701892},
		{ 0.25,  0.433012701892},
		{ 0.5,   0.0},
		{ 0.25, -0.433012701892},
		{-0.25, -0.433012701892}
	};

	RoundVector< RoundVector<double>* >* output = new RoundVector< RoundVector<double>* >();
	RoundVector<double>* row;
	for(int i = 0; i < 6; i++) {
		row = new RoundVector<double>();
		row->push_back(rot_data[i][0]);
		row->push_back(rot_data[i][1]);
		output->push_back(row);
	}

	return output;
}

const RoundVector< RoundVector<double>* >* Hexagon::ROT_COORDS = get_rot_coords();


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

/*void Hexagon::set_border_color(double red, double green, double blue) {
	for(int i = 0; i < 6; i++) {
		this->verticies[this->VERTEX_POSITIONS->at(i)]->set_color(red, green, blue);
	}
}

void Hexagon::set_border_color(std::vector<double> rgb) {
	for(int i = 0; i < 6; i++) {
		this->verticies[this->VERTEX_POSITIONS->at(i)]->set_color(rgb);
	}
}*/

void Hexagon::set_select_color(double red, double green, double blue) {
	bool changed = false;

	if(!this->select_color) {
		this->select_color = new std::vector< double >();
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

void Hexagon::set_select_color(std::vector<double> rgb) {
	if(!this->select_color) {
		this->select_color = new std::vector< double >();
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

std::vector<double>* Hexagon::get_select_color() {
	return this->select_color;
}

void Hexagon::set_hex_color(double red, double green, double blue) {
	if(!this->hex_color) {
		this->hex_color = new std::vector< double >();
	}

	this->hex_color->clear();
	this->hex_color->push_back(red);
	this->hex_color->push_back(green);
	this->hex_color->push_back(blue);

	/*if(this->parent_chunk) {
		this->parent_chunk->regenerate = true;
	}*/
}

void Hexagon::set_hex_color(std::vector<double> rgb) {
	if(!this->hex_color) {
		this->hex_color = new std::vector< double >();
	}

	this->set_hex_color(rgb[0], rgb[1], rgb[2]);
}

std::vector<double>* Hexagon::get_hex_color() {
	return this->hex_color;
}

void Hexagon::set_height(double height) {
	for(int i = 0; i < 6; i++) {
		this->verticies[this->VERTEX_POSITIONS->at(i)]->set_height(height);
	}
}

void Hexagon::add_height(double height) {
	for(int i = 0; i < 6; i++) {
		Vertex* curr_vert = this->verticies[this->VERTEX_POSITIONS->at(i)];
		curr_vert->set_height(curr_vert->get_height() + height);
	}
}

double Hexagon::get_height() {
	double output = 0;

	for(int i = 0; i < 6; i++) {
		output += this->verticies[this->VERTEX_POSITIONS->at(i)]->get_height();
	}

	return output / 6.0;
}

double Hexagon::get_slope() {
	double min_height = this->verticies[this->VERTEX_POSITIONS->at(0)]->get_height();
	double max_height = this->verticies[this->VERTEX_POSITIONS->at(0)]->get_height();

	for(int i = 1; i < 6; i++) {
		double curr_height = this->verticies[this->VERTEX_POSITIONS->at(i)]->get_height();
		if(curr_height < min_height) {
			min_height = curr_height;
		} else if (curr_height > max_height) {
			max_height = curr_height;
		}
	}

	return max_height - min_height;
}

bool Hexagon::is_pathable() {
	return this->get_slope() <= GlobalConsts::MAX_PATHABLE_SLOPE && this->get_board_object() == NULL;
}


void Hexagon::set_neighbor(const char* position, Hexagon* neighbor_hex) {
	this->neighbor_hexagons[position] = neighbor_hex;
}


Hexagon* Hexagon::get_neighbor(const char* position) {
	return this->neighbor_hexagons[position];
}


void Hexagon::render_for_select(double x, double y) {
	glBegin(GL_TRIANGLE_FAN);
		for(int i = 0; i < 6; i++) {
			Vertex* curr_vert = this->verticies[this->VERTEX_POSITIONS->at(i)];

			//glColor3df(curr_vert->get_color()->data());
			glColor3f(1, 1, 1);
			glVertex3f(x + ROT_COORDS->at(i)->at(0), y + ROT_COORDS->at(i)->at(1), curr_vert->get_height());
		}
	glEnd();
}


void Hexagon::generate_vertex_data(double x, double y, UniqueDataVector< GLfloat >* vertex_data) {
	Vertex* curr_vert = NULL;
	std::vector< double >* curr_color = this->get_hex_color();

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


void Hexagon::generate_select_data(double x, double y, UniqueDataVector< GLfloat >* select_data) {
	Vertex* curr_vert = NULL;
	std::vector< double >* curr_color = this->get_select_color();

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


void Hexagon::generate_object_data(double x, double y, UniqueDataVector< GLfloat >* object_data) {
	Controller* curr_ctrl = Controller::get_controller();

	std::map< Hexagon*, BoardObject* > &curr_board_object_map = *(curr_ctrl->gameboard->board_object_map);
    //std::vector< double > curr_rbg;
    //std::vector<int> v { 34,23 };

    BoardObject* curr_board_object = curr_board_object_map[this];

	if(curr_board_object) {
        std::vector< GLfloat > curr_xyz;
        std::vector< GLfloat >* curr_color = NULL;

	    if(curr_board_object->get_selected()) {
            curr_color = curr_board_object->selected_color;
	    } else {
            curr_color = curr_board_object->color;
	    }

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
}


BoardObject* Hexagon::get_board_object() {
    Controller* curr_ctrl = Controller::get_controller();
    std::map< Hexagon*, BoardObject* > &curr_board_object_map = *(curr_ctrl->gameboard->board_object_map);
    return curr_board_object_map[this];
}

