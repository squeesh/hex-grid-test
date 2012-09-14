#include "includes.h"


Controller* Controller::curr_ctrl = NULL;


const double Controller::COS_60 = std::cos(60.0 / 360.0 * 2.0 * M_PI);
const double Controller::SIN_60 = std::sin(60.0 / 360.0 * 2.0 * M_PI);


Controller::Controller(void) {
	this->x_offset = 0.0;
	this->y_offset = 0.0;

	this->zoom       = GlobalConsts::START_ZOOM;
	this->rotation   = GlobalConsts::START_ROTATION;
	this->view_range = GlobalConsts::START_VIEW_RANGE;

	this->hexagon_list = NULL;

	this->scroll_map[GlobalConsts::LEFT]    = false;
	this->scroll_map[GlobalConsts::RIGHT]   = false;
	this->scroll_map[GlobalConsts::UP]      = false;
	this->scroll_map[GlobalConsts::DOWN]    = false;

	this->selected_hex = NULL;

	for(int i = 0; i < 5; i++) {
		this->old_mouse_pos[i]["down"]	= 0;
		this->old_mouse_pos[i]["x"] 	= 0;
		this->old_mouse_pos[i]["y"] 	= 0;
	}

	this->hexagon_list = new RoundVector< RoundVector< Hexagon* >* >();
	this->hexagon_list->reserve(GlobalConsts::BOARD_WIDTH);
	this->hexagon_indicies = new std::map< Hexagon*, std::vector< int >* >();

	this->print_flag = false;

	/*TightlyPackedVector<float>* cv_test = new TightlyPackedVector<float>();
	cv_test->push_back(2, 3, 2);
	cv_test->push_back(5, 1, 4);
	cv_test->push_back(2, 3, 2);
	cv_test->push_back(7, 6, 2);
	cv_test->push_back(2, 6, 2);
	cv_test->push_back(5, 1, 4);
	cv_test->push_back(1, 2, 3);
	cv_test->push_back(2, 3, 2);

	std::cout << "index: " << cv_test->get_index(5, 1, 4) << std::endl;
	std::cout << "index: " << cv_test->get_index(2, 6, 2) << std::endl;

	float* data = cv_test->at(cv_test->get_index(5, 1, 4));
	std::cout << "cv: " << data[0] << " | " << data[1] << " | " << data[2] << std::endl;

	std::cout << "size: " << cv_test->size() << std::endl;

	exit(0);*/

}

Controller* Controller::_get_controller() {
	if(!Controller::curr_ctrl) {
		std::cout << "Starting up..." << std::endl;
		Controller::curr_ctrl = new Controller();
	}

	return Controller::curr_ctrl;
}

Controller* Controller::get_controller() {
	if(!Controller::curr_ctrl) {
		PyRun_SimpleString("import os, sys"); 
		PyRun_SimpleString("sys.path.append(os.getcwd())");
		PyRun_SimpleString("sys.dont_write_bytecode = True"); 
		PyObject *py_name = PyString_FromString("src.controller");
		PyObject *py_module = PyImport_Import(py_name);
		Py_XDECREF(py_name);

		PyObject *py_ctrl_cls = PyObject_GetAttrString(py_module, "Controller");
		Py_XDECREF(py_module);
		PyObject *py_ctrl_obj = py_call_func(py_ctrl_cls, "get_controller");
		Py_XDECREF(py_ctrl_cls);

		PyObject *py_ctrl_ptr = PyObject_GetAttrString(py_ctrl_obj, "_c_ctrl_obj");
		int ctrl_ptr = ((int)PyInt_AsLong(py_ctrl_ptr));
		Py_XDECREF(py_ctrl_ptr);

		Controller::curr_ctrl = (Controller*)ctrl_ptr;

		// capture our python controller pointer and save it for future use
		Controller::curr_ctrl->controller_py = py_ctrl_obj;
	}

	return Controller::curr_ctrl;
}

void Controller::push_hexagon(Hexagon *hex) {
	// assumption: the hex being passed in is not currently in hexagon_list
	int total_size = 0;
	for(int i = 0; i < this->hexagon_list->size(); i++) {
		total_size += this->hexagon_list->at(i)->size();
	}

	int i = (int)(total_size % GlobalConsts::BOARD_WIDTH);

	this->hexagon_list->at(i)->push_back(hex);

	int j = this->hexagon_list->at(i)->size()-1;

	std::map< Hexagon*, std::vector< int >* > &curr_indicies = *(this->hexagon_indicies);
	curr_indicies[hex] = new std::vector< int >();
	curr_indicies[hex]->push_back(i);
	curr_indicies[hex]->push_back(j);
}

Hexagon* Controller::pop_hexagon() {
	int total_size = 0;
	for(int i = 0; i < this->hexagon_list->size(); i++) {
		total_size += this->hexagon_list->at(i)->size();
	}

	int i = (int)(total_size % GlobalConsts::BOARD_WIDTH);

	Hexagon *last_hex = this->hexagon_list->at(i)->back();
	this->hexagon_list->at(i)->pop_back();
	return last_hex;
}

Hexagon* Controller::get_hexagon(int i, int j) {
    return this->hexagon_list->at(i)->at(j);
}

void Controller::zoom_map(double zoom_amount) {
	if(zoom_amount > 1) {
		if(zoom < GlobalConsts::MAX_ZOOM) {
			this->zoom *= zoom_amount;
			this->view_range *= zoom_amount;
		}
	} else {
		if(GlobalConsts::MIN_ZOOM < zoom) {
			this->zoom *= zoom_amount;
			this->view_range *= zoom_amount;
		}
	}
	
}

void Controller::set_rotation(double rotation) {
	this->rotation = rotation;
}

double Controller::get_rotation() {
	return this->rotation;
}

void Controller::set_scroll(char direction) {
	this->scroll_map[direction] = true;
}

void Controller::clear_scroll(char direction) {
    this->scroll_map[direction] = false;
}

void Controller::init_gl(long width, long height) {
	this->width = width;
	this->height = height;

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(1.0);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, this->width/((double) this->height), 0.1, 1000.0);
	glMatrixMode(GL_MODELVIEW);
}

void Controller::resize(long width, long height) {
        this->width = width;
	this->height = height;

        if(this->height == 0) {
            this->height = 1;
	}

        glViewport(0, 0, this->width, this->height);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0, this->width/((double) this->height), 0.1, 1000.0);
        glMatrixMode(GL_MODELVIEW);
}

void Controller::init_board() {
	for(int i = 0; i < GlobalConsts::BOARD_WIDTH; i++) {
	    RoundVector< Hexagon* >* curr_vect = new RoundVector< Hexagon* >();
		curr_vect->reserve(GlobalConsts::BOARD_HEIGHT);

		this->hexagon_list->push_back(curr_vect);
	}
}

void Controller::py_init_board() {
	py_call_func(this->controller_py, "init_board");

	Hexagon* curr_hex = hexagon_list->at(0)->at(0);
	//std::cout << "curr hex: " << curr_hex << std::endl;

	for(int i = 0; i < 6; i++) {
		//std::cout << "neigh hex: " << curr_hex->get_neighbor(Hexagon::NEIGHBOR_DIRECTION->at(i)) << std::endl;
	}

	//std::cout << std::endl;

	std::set< Hexagon* >* hex_set = this->get_neighbors_in_radius(curr_hex, 2);

	for(std::set< Hexagon* >::iterator itr = hex_set->begin(); itr != hex_set->end(); itr++) {
		//std::cout << "hex: " << *itr << std::endl;
	}
}


void Controller::tick() {
    if(this->scroll_map[GlobalConsts::LEFT]) {
        this->x_offset -= 0.5 * this->zoom;
    }

    if(this->scroll_map[GlobalConsts::RIGHT]) {
        this->x_offset += 0.5 * this->zoom;
    }

    if(this->scroll_map[GlobalConsts::UP]) {
        this->y_offset += 0.5 * this->zoom;
    }

    if(this->scroll_map[GlobalConsts::DOWN]) {
        this->y_offset -= 0.5 * this->zoom;
    }
}

void Controller::render() {
	this->render(GlobalConsts::RENDER_LINES);
}

std::vector<std::vector< GLfloat >*>* Controller::generate_render_data(Hexagon* curr_hex, double x, double y) {
	std::vector< GLfloat >* vertex_data = new std::vector< GLfloat >();
	std::vector< GLfloat >* color_data = new std::vector< GLfloat >();

	vertex_data->reserve(4*3*2);
	color_data->reserve(4*3*2);

	Vertex* curr_vert = NULL;
	std::vector<double> curr_color;

	for(int i = 0; i < 6; i++) {
		curr_vert = curr_hex->verticies[curr_hex->VERTEX_POSITIONS->at(i)];

		curr_color = curr_vert->get_color();

		color_data->push_back(curr_color[0]);
		color_data->push_back(curr_color[1]);
		color_data->push_back(curr_color[2]);

		vertex_data->push_back(x + Hexagon::ROT_COORDS->at(i)->at(0));
		vertex_data->push_back(y + Hexagon::ROT_COORDS->at(i)->at(1));
		vertex_data->push_back(curr_vert->get_height());

		curr_vert = curr_hex->verticies[curr_hex->VERTEX_POSITIONS->at(i+1)];

		curr_color = curr_vert->get_color();

		color_data->push_back(curr_color[0]);
		color_data->push_back(curr_color[1]);
		color_data->push_back(curr_color[2]);

		vertex_data->push_back(x + Hexagon::ROT_COORDS->at(i+1)->at(0));
		vertex_data->push_back(y + Hexagon::ROT_COORDS->at(i+1)->at(1));
		vertex_data->push_back(curr_vert->get_height());
	}

	std::vector<std::vector< GLfloat >*>* output = new std::vector<std::vector< GLfloat >*>();
	output->push_back(vertex_data);
	output->push_back(color_data);

	return output;
}

std::vector<std::vector< GLfloat >*>* Controller::get_render_data(Hexagon* curr_hex, double base_x, double base_y) {
	std::vector< GLfloat >* vertex_data = new std::vector< GLfloat >();
	std::vector< GLfloat >* color_data = new std::vector< GLfloat >();

	vertex_data->reserve(4*6*3*2);
	color_data->reserve(4*6*3*2);

	Vertex* curr_vert = NULL;
	std::vector<double> curr_color;

	double x = base_x;
        double y = base_y;

	std::vector<std::vector< GLfloat >*>* render_data = NULL;
	std::vector< GLfloat >* curr_vertex_data = NULL;
	std::vector< GLfloat >* curr_color_data = NULL;

	render_data = this->generate_render_data(curr_hex, x, y);
	curr_vertex_data = render_data->at(0);
	curr_color_data = render_data->at(1);

	for(int i = 0; i < curr_vertex_data->size(); i++) {
		vertex_data->push_back(curr_vertex_data->at(i));
		color_data->push_back(curr_color_data->at(i));
	}

	delete curr_vertex_data;
	delete curr_color_data;
	delete render_data;

	const char* dir_ary[] = {"N", "NE", "SE"}; 

	for(int i = 0; i < 3; i++) {
		const char* direction = dir_ary[i];
		std::vector< double >* x_y_diff = GlobalConsts::RENDER_TRAY_COORDS[direction];

		render_data = this->generate_render_data(curr_hex->get_neighbor(direction), x + x_y_diff->at(0), y + x_y_diff->at(1));
		curr_vertex_data = render_data->at(0);
		curr_color_data = render_data->at(1);

		for(int i = 0; i < curr_vertex_data->size(); i++) {
			vertex_data->push_back(curr_vertex_data->at(i));
			color_data->push_back(curr_color_data->at(i));
		}

		delete curr_vertex_data;
		delete curr_color_data;
		delete render_data;
	}

	std::vector<std::vector< GLfloat >*>* output = new std::vector<std::vector< GLfloat >*>();
	output->push_back(vertex_data);
	output->push_back(color_data);

	return output;
}

void Controller::render(int render_mode) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	double eye_x = this->x_offset * 1.5 * this->COS_60;
	double eye_y = this->y_offset * 1.0 * this->SIN_60;

	gluLookAt(
	    eye_x, eye_y, 15 * this->zoom,
	    eye_x, eye_y, 0,
		0, 1, 0
	);

	// set the rotation point... since our "origin" kinda changes... we need to go to it, rotate, then go back
	glTranslatef(0.0, this->y_offset * this->SIN_60, 0.0);
	glRotatef(this->rotation, 1.0, 0.0, 0.0);
	glTranslatef(0.0, -this->y_offset * this->SIN_60, 0.0);

	int neg_x_view = this->x_offset - this->view_range / 2.0;
	int pos_x_view = this->x_offset + this->view_range / 2.0;
	int neg_y_view = this->y_offset - this->view_range / 2.0;
	int pos_y_view = this->y_offset + this->view_range / 2.0;

	if(render_mode == GlobalConsts::RENDER_TRIANGLES) {
        for(int i = neg_x_view; i <= pos_x_view; i++) {
            for(int j = neg_y_view; j <= pos_y_view; j++) {
                Hexagon* curr_hex = this->hexagon_list->at(i)->at(j);
                glLoadName(curr_hex->name);

                double x = i * 1.5 * this->COS_60;
                double y = j * 1.0 * this->SIN_60;

                if(i % 2 != 0) {
                    y += 0.5 * this->SIN_60;
                }

                curr_hex->render_as_selected(x, y);
            }
        }

    } else {
        Vertex* curr_vert = NULL;
        int array_size = (pos_x_view+1 - neg_x_view) * (pos_y_view+1 - neg_y_view);

        RoundVector<GLfloat>* vertex_data = new RoundVector<GLfloat>();
        vertex_data->reserve(array_size * 6 * 2);

        RoundVector<GLfloat>* color_data = new RoundVector<GLfloat>();
        color_data->reserve(array_size * 6 * 2);

        RoundVector<GLfloat>* triangle_vertex_data = new RoundVector<GLfloat>();
        triangle_vertex_data->reserve(array_size * 3);

        RoundVector<GLfloat>* triangle_color_data = new RoundVector<GLfloat>();
        triangle_color_data->reserve(array_size);

        int tri_count = 0;

        for(int j = neg_y_view; j <= pos_y_view; j++) {
            for(int i = neg_x_view; i <= pos_x_view; i++) {
                Hexagon* curr_hex = this->hexagon_list->at(i)->at(j);
                glLoadName(curr_hex->name);

                double x = i * 1.5 * this->COS_60;
                double y = j * 1.0 * this->SIN_60;

                if(i % 2 != 0) {
                    y += 0.5 * this->SIN_60;
                }

                Vertex* curr_vert = NULL;
                std::vector<double> curr_color;

                for(int k = 0; k < 6; k++) {
                    curr_vert = curr_hex->verticies[curr_hex->VERTEX_POSITIONS->at(k)];

                    if(k > 1) {
                        if(curr_hex->select_color) {
                            curr_color = curr_hex->select_color->get_rgb();

                            triangle_color_data->push_back(curr_color[0]);
                            triangle_color_data->push_back(curr_color[1]);
                            triangle_color_data->push_back(curr_color[2]);

                            triangle_vertex_data->push_back(Hexagon::ROT_COORDS->at(0)->at(0) * 0.8 + x);
                            triangle_vertex_data->push_back(Hexagon::ROT_COORDS->at(0)->at(1) * 0.8 + y);
                            triangle_vertex_data->push_back(curr_hex->verticies[curr_hex->VERTEX_POSITIONS->at(0)]->get_height());

                            triangle_color_data->push_back(curr_color[0]);
                            triangle_color_data->push_back(curr_color[1]);
                            triangle_color_data->push_back(curr_color[2]);

                            triangle_vertex_data->push_back(Hexagon::ROT_COORDS->at(k-1)->at(0) * 0.8 + x);
                            triangle_vertex_data->push_back(Hexagon::ROT_COORDS->at(k-1)->at(1) * 0.8 + y);
                            triangle_vertex_data->push_back(curr_hex->verticies[curr_hex->VERTEX_POSITIONS->at(k-1)]->get_height());

                            triangle_color_data->push_back(curr_color[0]);
                            triangle_color_data->push_back(curr_color[1]);
                            triangle_color_data->push_back(curr_color[2]);

                            triangle_vertex_data->push_back(Hexagon::ROT_COORDS->at(k)->at(0) * 0.8 + x);
                            triangle_vertex_data->push_back(Hexagon::ROT_COORDS->at(k)->at(1) * 0.8 + y);
                            triangle_vertex_data->push_back(curr_hex->verticies[curr_hex->VERTEX_POSITIONS->at(k)]->get_height());

                            tri_count++;
                        }
                    }
                }

		if(i % 2 == 0 && j % 2 == 0) {
			std::vector<std::vector< GLfloat >*>* render_data = get_render_data(curr_hex, x, y);
			std::vector< GLfloat >* curr_vertex_data = render_data->at(0);
			std::vector< GLfloat >* curr_color_data = render_data->at(1);

			for(int k = 0; k < curr_vertex_data->size(); k++) {
				vertex_data->push_back(curr_vertex_data->at(k));
				color_data->push_back(curr_color_data->at(k));
			}

			delete curr_vertex_data;
			delete curr_color_data;
			delete render_data;
		}
            }
        }

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, vertex_data->data());
        glColorPointer(3, GL_FLOAT, 0, color_data->data());

        //glDrawArrays(GL_LINES, 0, array_size * 6 * 2);
	glDrawArrays(GL_LINES, 0, (int)(vertex_data->size() / 3.0));

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        //----------

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        glVertexPointer(3, GL_FLOAT, 0, triangle_vertex_data->data());
        glColorPointer(3, GL_FLOAT, 0, triangle_color_data->data());

        glDrawArrays(GL_TRIANGLES, 0, tri_count*3);

        glDisableClientState(GL_COLOR_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);

        delete vertex_data;
        delete color_data;
        delete triangle_vertex_data;
        delete triangle_color_data;
    }
}


Hexagon* Controller::get_clicked_hex(double x, double y) {
	GLuint buff[64] = {0};
	GLint hits = 0;
	GLint view[4];

    glSelectBuffer(64, buff);

    glGetIntegerv(GL_VIEWPORT,view);

    glRenderMode(GL_SELECT);

    //Clearing the name's stack
    //This stack contains all the info about the objects
    glInitNames();

    //Now fill the stack with one element (or glLoadName will generate an error)
    glPushName(0);

    glMatrixMode(GL_PROJECTION);

    glPushMatrix();
    glLoadIdentity();

    gluPickMatrix(x, y, 1.0, 1.0, view);
    gluPerspective(45.0, float(this->width)/float(this->height), 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);

    this->render(GlobalConsts::RENDER_TRIANGLES);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    //get number of objects drawed in that area
    //and return to render mode
    hits = glRenderMode(GL_RENDER);
    glMatrixMode(GL_MODELVIEW);

    Hexagon* clicked_hex = NULL;

	// TODO: buggy... needs to be fixed.
	for(int i = 0; i < hits; i++) {
		clicked_hex = this->get_hex_by_name(buff[i * 4 + 3]);
		if(clicked_hex) {
			break;
		}
	}

	return clicked_hex;
}

std::set<Hexagon*>* Controller::get_neighbors_in_radius(Hexagon* curr_hex, int radius) {
	std::set<Hexagon*>* output = new std::set<Hexagon*>();
	this->get_neighbors_in_radius(curr_hex, radius, output);

	return output;
}

std::set<Hexagon*>* Controller::get_neighbors_in_radius(Hexagon* curr_hex, int radius, std::set<Hexagon*>* curr_neighbors) {
	if(radius > 0) {
		Hexagon* neigh_hex = NULL;

		curr_neighbors->insert(curr_hex);

		for(int i = 0; i < 6; i++) {
			neigh_hex = curr_hex->get_neighbor(Hexagon::NEIGHBOR_DIRECTION->at(i));

			//if(!curr_neighbors->count(neigh_hex)) {
				this->get_neighbors_in_radius(neigh_hex, radius-1, curr_neighbors);
			//}
		}
	}

	return curr_neighbors;

}

void Controller::mouse_left_click(int x, int y) {
	Hexagon* curr_hex = this->get_clicked_hex(x, this->height-y);

	if(curr_hex && curr_hex->is_pathable()) {
		if(this->get_selected_hex()) {
			py_call_func(this->controller_py, "find_path", this->get_selected_hex(), curr_hex);
		}

		this->set_selected_hex(curr_hex);
	}

	this->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["down"] 	= 1;
	this->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["x"]	= x;
	this->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["y"] 	= y;
}

void Controller::mouse_left_release(int x, int y) {
	this->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["down"] 	= 0;
	this->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["x"]	= x;
	this->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["y"] 	= y;
}

void Controller::mouse_right_click(int x, int y) {
	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["down"] 	= 1;
	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["x"]	= x;
	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["y"] 	= y;
}

void Controller::mouse_right_release(int x, int y) {
	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["down"] 	= 0;
	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["x"]	= x;
	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["y"] 	= y;
}

void Controller::mouse_left_drag(int x, int y) {

}

void Controller::mouse_right_drag(int x, int y) {
	int x_diff = x - this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["x"];
	int y_diff = y - this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["y"];

	this->x_offset -= x_diff / 30.0 * this->zoom;
	this->y_offset += y_diff / 30.0 * this->zoom;

	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["x"] = x;
	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["y"] = y;
}

void Controller::mouse_scroll_up(int x, int y) {
	this->zoom_map(1 / 1.25);
}

void Controller::mouse_scroll_down(int x, int y) {
	this->zoom_map(1.25);
}

void Controller::key_down(unsigned char key, int x, int y) {
    char key_str[2] = {key, '\0'};
    PyObject* py_str = PyString_FromString(key_str);
    PyObject* py_x = PyInt_FromLong(x);
    PyObject* py_y = PyInt_FromLong(y);

    py_call_func(this->controller_py, "key_down", py_str, py_x, py_y);

    Py_XDECREF(py_str);
    Py_XDECREF(py_x);
    Py_XDECREF(py_y);
}

void Controller::key_up(unsigned char key, int x, int y) {
    char key_str[2] = {key, '\0'};
    PyObject* py_str = PyString_FromString(key_str);
    PyObject* py_x = PyInt_FromLong(x);
    PyObject* py_y = PyInt_FromLong(y);

    py_call_func(this->controller_py, "key_up", py_str, py_x, py_y);

    Py_XDECREF(py_str);
    Py_XDECREF(py_x);
    Py_XDECREF(py_y);
}

void Controller::set_selected_hex(Hexagon* curr_hex) {
    if(this->selected_hex) {
        this->selected_hex->clear_select_color();
    }
    this->selected_hex = curr_hex;
    this->selected_hex->set_select_color(1.0, 1.0, 0.0);
}

Hexagon* Controller::get_selected_hex() {
	return this->selected_hex;
}

Hexagon* Controller::get_hex_by_name(long name) {
	for(int i = 0; i < this->hexagon_list->size(); i++) {
		for(int j = 0; j < this->hexagon_list->at(i)->size(); j++) {
			Hexagon* curr_hex = this->hexagon_list->at(i)->at(j);

			if(curr_hex->name == name) {
				return curr_hex;
			}
		}
	}

	return NULL;
}


