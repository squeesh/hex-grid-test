#include "includes.h"


Controller* Controller::curr_ctrl = NULL;
PyObject* Controller::py_pointer = NULL;


Controller::Controller() {
	this->player_input = NULL;

	this->gameboard = new Gameboard();

	this->print_flag = false;
    this->kill_threads = false;

    this->ready = false;
    this->timer_thread = NULL;
}

Controller::~Controller() {
    std::cout << "closing..." << std::endl;

    curr_ctrl->kill_threads = true;
    curr_ctrl->timer_thread->join();

    delete curr_ctrl->timer_thread;
    delete this->player_input;
    delete this->gameboard;

    Py_XDECREF(this->py_pointer);

    Py_Finalize();
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

		try {
            PyObject *py_name = PyString_FromString("py.controller");
            if(PyErr_Occurred()) {
                throw PythonException();
            }
            PyObject *py_module = PyImport_Import(py_name);
            if(PyErr_Occurred()) {
                throw PythonException();
            }
            Py_XDECREF(py_name);

            PyObject *py_ctrl_cls = PyObject_GetAttrString(py_module, "Controller");
            if(PyErr_Occurred()) {
                throw PythonException();
            }
            Py_XDECREF(py_module);

            PyObject *py_ctrl_obj = py_call_func(py_ctrl_cls, "get_controller");
            Py_XDECREF(py_ctrl_cls);

            PyObject *py_ctrl_ptr = PyObject_GetAttrString(py_ctrl_obj, "_c_pointer");
            int ctrl_ptr = ((int)PyInt_AsLong(py_ctrl_ptr));
            Py_XDECREF(py_ctrl_ptr);

            Controller::curr_ctrl = (Controller*)ctrl_ptr;

            // capture our python controller pointer and save it for future use
            Controller::curr_ctrl->py_pointer = py_ctrl_obj;
		} catch(PythonException &e) {
		    PyErr_Print();
		    exit(0);
		}
	}

	return Controller::curr_ctrl;
}


void Controller::init_board() {
	py_call_func(this->py_pointer, "init_board");
    this->ready = true;
}


/*void Controller::link_segments() {
    //for i in range(GlobalConsts.BOARD_WIDTH):
    for(int i = 0; i < GlobalConsts::BOARD_WIDTH){
        offset = self.get_neighbor_offset(i)

        for j in range(GlobalConsts.BOARD_HEIGHT):
            neighbor_dict = {}
            curr_hex = self.get_hexagon(i, j)

            for pos, (i_offset, j_offset) in offset.iteritems():
                new_seg_i = i + i_offset
                new_seg_j = j + j_offset

                neighbor_hex = self.get_hexagon(new_seg_i, new_seg_j)

                assert neighbor_hex not in neighbor_dict.values()
                neighbor_dict[pos] = neighbor_hex

            curr_hex.set_neighbors(neighbor_dict)
    }

    for curr_hex in Hexagon.get_all_hexagons():
        curr_hex.link_verticies()
}*/


void Controller::tick() {
    MutexManager::get("controller_tick")->lock();
	RenderController* curr_rend_ctrl = RenderController::get_render_controller();
	curr_rend_ctrl->tick();

    py_call_func(this->py_pointer, "tick");
    MutexManager::get("controller_tick")->unlock();
}


void Controller::push_hexagon(Hexagon *hex) {
	this->gameboard->push_back(hex);
}


Hexagon* Controller::get_hex_by_name(GLlong name) {
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


void Controller::mouse_event(int event_type, int button, int x, int y) {
    MutexManager::get("controller_tick")->lock();
	RenderController* curr_rend_ctrl = RenderController::get_render_controller();
    this->player_input->mouse_event(event_type, button, x, curr_rend_ctrl->height - y);
    MutexManager::get("controller_tick")->unlock();
}

void Controller::keyboard_event(int event_type, unsigned char key, int x, int y) {
    MutexManager::get("controller_tick")->lock();
	RenderController* curr_rend_ctrl = RenderController::get_render_controller();
    this->player_input->keyboard_event(event_type, key, x, curr_rend_ctrl->height - y);
    MutexManager::get("controller_tick")->unlock();
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

