#include "includes.h"

#include <GL/freeglut.h>


Controller* Controller::curr_ctrl = NULL;


const double Controller::COS_60 = std::cos(60.0 / 360.0 * 2.0 * M_PI);
const double Controller::SIN_60 = std::sin(60.0 / 360.0 * 2.0 * M_PI);


Controller::Controller(void) {
    this->controller_py = NULL;

    this->width = 0;
    this->height = 0;

	this->x_offset = 0.0;
	this->y_offset = 0.0;

	this->zoom       = GlobalConsts::START_ZOOM;
	this->rotation   = GlobalConsts::START_ROTATION;
	this->view_range = GlobalConsts::START_VIEW_RANGE;

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

	this->gameboard = new Gameboard();

	this->print_flag = false;

	this->print_string = std::string();
}

Controller* Controller::get_controller() {
	if(!Controller::curr_ctrl) {
		std::cout << "Starting up..." << std::endl;
		Controller::curr_ctrl = new Controller();
	}

	return Controller::curr_ctrl;
}

Controller* Controller::py_get_controller() {
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
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glShadeModel(GL_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_BLEND );

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK,  GL_LINE);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, this->width/((double) this->height),-1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);

	//glEnable(GL_LINE_SMOOTH);
	glLineWidth(5);


	//glewExperimental = GL_TRUE; 
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));

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
	py_call_func(this->controller_py, "init_board");

	//this->gameboard->bind_obj_hex(new BoardObject(NULL), this->get_hexagon(0, 0));
	//this->gameboard->bind_obj_hex(new BoardObject(NULL), this->get_hexagon(3, 7));
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


void Controller::render_string(int x, int y, std::string curr_string) {
	std::vector< GLfloat >* default_color = new std::vector< GLfloat >();
	default_color->push_back(0);
	default_color->push_back(0);
	default_color->push_back(0);

	this->render_string(x, y, curr_string, default_color);

	delete default_color;
}


void Controller::render_string(int x, int y, std::string curr_string, std::vector< GLfloat >* color) {
	glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
			glLoadIdentity();
			glOrtho(0, this->width, 0, this->height, -1.0f, 1.0f);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
				glLoadIdentity();
				glColor3fv(color->data());
				glRasterPos2i(x, this->height - y);
				glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, (const unsigned char*)curr_string.data());
				glMatrixMode(GL_PROJECTION);
			glPopMatrix();
			glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	glPopMatrix();
}

void Controller::render() {
	// TODO: Do something about duplicated code...

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::vector< GLfloat >* color = new std::vector< GLfloat >();
	color->push_back(1);
	color->push_back(1);
	color->push_back(0);
	this->render_string(2, 20, this->print_string, color);
	delete color;

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

	int neg_x_view = this->x_offset - this->view_range / 2.0 - GlobalConsts::BOARD_CHUNK_SIZE;
	int pos_x_view = this->x_offset + this->view_range / 2.0;
	int neg_y_view = this->y_offset - this->view_range / 2.0 - GlobalConsts::BOARD_CHUNK_SIZE;
	int pos_y_view = this->y_offset + this->view_range / 2.0;

	if(neg_y_view < 0) {
		neg_y_view += (-neg_y_view) % GlobalConsts::BOARD_CHUNK_SIZE;
	} else {
		neg_y_view -=  neg_y_view % GlobalConsts::BOARD_CHUNK_SIZE;
	}

	if(pos_y_view < 0) {
		pos_y_view += (-pos_y_view) % GlobalConsts::BOARD_CHUNK_SIZE;
	} else {
		pos_y_view -=  pos_y_view % GlobalConsts::BOARD_CHUNK_SIZE;
	}

	int temp = neg_x_view;

	if(neg_x_view < 0) {
		neg_x_view += (-neg_x_view) % GlobalConsts::BOARD_CHUNK_SIZE;
	} else {
		neg_x_view -=  neg_x_view % GlobalConsts::BOARD_CHUNK_SIZE;
	}

	if(pos_x_view < 0) {
		pos_x_view += (-pos_x_view) % GlobalConsts::BOARD_CHUNK_SIZE;
	} else {
		pos_x_view -=  pos_x_view % GlobalConsts::BOARD_CHUNK_SIZE;
	}


	this->gameboard->render(neg_x_view, pos_x_view, neg_y_view, pos_y_view);
}


void Controller::render_for_select() {
	// TODO: Do something about duplicated code...
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

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        for(int i = neg_x_view; i <= pos_x_view; i++) {
            for(int j = neg_y_view; j <= pos_y_view; j++) {
                Hexagon* curr_hex = this->gameboard->get_hexagon_list()->at(i)->at(j);
                glLoadName(curr_hex->name);

                double x = i * 1.5 * this->COS_60;
                double y = j * 1.0 * this->SIN_60;

                if(i % 2 != 0) {
                    y += 0.5 * this->SIN_60;
                }

                curr_hex->render_for_select(x, y);
		
            }
        }
	glEnable(GL_CULL_FACE);
}


void Controller::push_hexagon(Hexagon *hex) {
	this->gameboard->push_back(hex);
}


void Controller::set_selected_hex(Hexagon* curr_hex) {
	if(this->selected_hex) {
		this->selected_hex->clear_select();
	}
	this->selected_hex = curr_hex;

	if(this->selected_hex) {
	    this->selected_hex->set_select();
	}
}


void Controller::clear_selected_hex() {
    this->set_selected_hex(NULL);
}

Hexagon* Controller::get_selected_hex() {
	return this->selected_hex;
}


Hexagon* Controller::get_hex_by_name(long name) {
	for(int i = 0; i < this->gameboard->get_hexagon_list()->size(); i++) {
		for(int j = 0; j < this->gameboard->get_hexagon_list()->at(i)->size(); j++) {
			Hexagon* curr_hex = this->gameboard->get_hexagon_list()->at(i)->at(j);

			if(curr_hex->name == name) {
				return curr_hex;
			}
		}
	}

	return NULL;
}


Hexagon* Controller::get_hexagon(int i, int j) {
    return this->gameboard->get_hexagon_list()->at(i)->at(j);
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

	this->render_for_select();

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
	    std::map< Hexagon*, BoardObject* > &curr_board_object_map = *(this->gameboard->board_object_map);
	    if(curr_board_object_map[curr_hex]) {
	        this->set_selected_hex(curr_hex);
	    }
	}

	this->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["down"] 	= 1;
	this->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["x"]      = x;
	this->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["y"] 	    = y;
}

void Controller::mouse_left_release(int x, int y) {
	this->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["down"] 	= 0;
	this->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["x"]	    = x;
	this->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["y"] 	    = y;
}

void Controller::mouse_middle_click(int x, int y) {
    this->old_mouse_pos[GlobalConsts::MOUSE_MIDDLE]["down"] = 1;
    this->old_mouse_pos[GlobalConsts::MOUSE_MIDDLE]["x"]    = x;
    this->old_mouse_pos[GlobalConsts::MOUSE_MIDDLE]["y"]    = y;
}

void Controller::mouse_middle_release(int x, int y) {
    this->old_mouse_pos[GlobalConsts::MOUSE_MIDDLE]["down"] = 0;
    this->old_mouse_pos[GlobalConsts::MOUSE_MIDDLE]["x"]    = x;
    this->old_mouse_pos[GlobalConsts::MOUSE_MIDDLE]["y"]    = y;
}

void Controller::mouse_right_click(int x, int y) {
    Hexagon* curr_hex = this->get_clicked_hex(x, this->height-y);

    if(curr_hex && curr_hex->is_pathable()) {
        if(this->get_selected_hex()) {
            py_call_func(this->controller_py, "find_path", this->get_selected_hex(), curr_hex);
        }
    }

	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["down"] 	= 1;
	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["x"]	    = x;
	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["y"]     = y;
}

void Controller::mouse_right_release(int x, int y) {
	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["down"] 	= 0;
	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["x"]	    = x;
	this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["y"] 	= y;
}

void Controller::mouse_left_drag(int x, int y) {
}

void Controller::mouse_middle_drag(int x, int y) {
    int x_diff = x - this->old_mouse_pos[GlobalConsts::MOUSE_MIDDLE]["x"];
    int y_diff = y - this->old_mouse_pos[GlobalConsts::MOUSE_MIDDLE]["y"];

    this->x_offset -= x_diff / 30.0 * this->zoom;
    this->y_offset += y_diff / 30.0 * this->zoom;

    this->old_mouse_pos[GlobalConsts::MOUSE_MIDDLE]["x"] = x;
    this->old_mouse_pos[GlobalConsts::MOUSE_MIDDLE]["y"] = y;
}

void Controller::mouse_right_drag(int x, int y) {
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

Hexagon* Controller::add_object_to_board(BoardObject* curr_obj, Hexagon* curr_hex) {
    // TODO: actually implement logic below...
    // accepts a BoardObject and Hexagon.
    // The hexagon is the target, that the BoardObject would like to be placed on
    // if this space is occupied, the controller is free to assign it to a differnt Hexagon
    // The value returned in the Hexagon that it was actually assigned to.

    this->gameboard->bind_obj_hex(curr_obj, curr_hex);
    return curr_hex;
}

