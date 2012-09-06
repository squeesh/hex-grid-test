#include "includes.h"


int Hexagon::name_count = 0;
RoundVector< RoundVector<double>* >* Hexagon::rendered_verticies = NULL;

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
		this->set_vertex(this->VERTEX_POSITIONS->at(i), new Vertex(0, 1, 0, 0));
	}

	this->name_count++;
	this->name = this->name_count;

	this->last_x = 0;
	this->last_y = 0;
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

void Hexagon::set_color(double red, double green, double blue) {
	for(int i = 0; i < 6; i++) {
		this->verticies[this->VERTEX_POSITIONS->at(i)]->set_color(red, green, blue);
	}
}

void Hexagon::set_color(std::vector<double> rgb) {
	for(int i = 0; i < 6; i++) {
		this->verticies[this->VERTEX_POSITIONS->at(i)]->set_color(rgb);
	}
}

void Hexagon::set_select_color(double red, double green, double blue) {
	if(!this->select_color) {
		this->select_color = new Color(red, green, blue);
	} else {
		this->select_color->set_rgb(red, green, blue);
	}
}

void Hexagon::set_select_color(std::vector<double> rgb) {
	if(!this->select_color) {
		this->select_color = new Color(rgb);
	} else {
		this->select_color->set_rgb(rgb);
	}
}

void Hexagon::clear_select_color() {
    delete this->select_color;
    this->select_color = NULL;
}

std::vector<double> Hexagon::get_select_color() {
    return this->select_color->get_rgb();
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
	return this->get_slope() <= GlobalConsts::MAX_PATHABLE_SLOPE;
}

void Hexagon::set_neighbor(const char* position, Hexagon* neighbor_hex) {
	this->neighbor_hexagons[position] = neighbor_hex;
}

Hexagon* Hexagon::get_neighbor(const char* position) {
	return this->neighbor_hexagons[position];
}

void Hexagon::render(double x, double y) {
	this->last_x = x;
	this->last_y = y;

	this->render_border(x, y);
	if(this->select_color) {
		this->render_select(x, y);
	}
}

void Hexagon::render_as_selected(double x, double y) {
	this->render_triangles(x, y);
}

void Hexagon::render_border(double x, double y) {
	glBegin(GL_LINE_LOOP);
		for(int i = 0; i < 6; i++) {
			Vertex* curr_vert = this->verticies[this->VERTEX_POSITIONS->at(i)];

			glColor3dv(curr_vert->get_color().data());
			glVertex3f(x + ROT_COORDS->at(i)->at(0), y + ROT_COORDS->at(i)->at(1), curr_vert->get_height());
		}
	glEnd();
}

void Hexagon::render_triangles(double x, double y) {
	glBegin(GL_TRIANGLE_FAN);
		for(int i = 0; i < 6; i++) {
			Vertex* curr_vert = this->verticies[this->VERTEX_POSITIONS->at(i)];

			glColor3dv(curr_vert->get_color().data());
			glVertex3f(x + ROT_COORDS->at(i)->at(0), y + ROT_COORDS->at(i)->at(1), curr_vert->get_height());
		}
	glEnd();
}

void Hexagon::render_select(double x, double y) {
	/*// not 100% correct... but works
	glPushMatrix();	
		glTranslatef(x, y, 0);
		glScalef(0.4, 0.4, 1);
		glTranslatef(-1, 0, 0);
		glRotatef(-30, 0.0, 0.0, 1.0);
		for(int i = 0; i < 6; i++) {
			glBegin(GL_TRIANGLES);
				glColor3dv(this->select_color->get_rgb().data());
				glVertex3f(0, 0, this->verticies[this->VERTEX_POSITIONS->at(i)]->get_height());
				glVertex3f(0, 1, this->verticies[this->VERTEX_POSITIONS->at(i+1)]->get_height());
				glVertex3f(1, 0.5, this->get_height());
			glEnd();
			glTranslatef(0, 1, 0);
			glRotatef(-60, 0.0, 0.0, 1.0);
		}
	glPopMatrix();*/

	glBegin(GL_TRIANGLE_FAN);
		for(int i = 0; i < 6; i++) {
			Vertex* curr_vert = this->verticies[this->VERTEX_POSITIONS->at(i)];

			glColor3dv(this->select_color->get_rgb().data());
			glVertex3f(x + ROT_COORDS->at(i)->at(0) * 0.8, y + ROT_COORDS->at(i)->at(1) * 0.8, curr_vert->get_height());
		}
	glEnd();
}


