#include "includes.h"

PlayerInput::PlayerInput(PyObject* py_pointer) {
    this->py_pointer = py_pointer;

    for(int i = 0; i < 5; i++) {
        this->old_mouse_pos[i]["state"] = GlobalConsts::MOUSE_UP;
        this->old_mouse_pos[i]["x"]     = 0;
        this->old_mouse_pos[i]["y"]     = 0;
    }
}

void PlayerInput::mouse_event(int event_type, int button, int x, int y) {
    // Mouse down events
    if(event_type == GlobalConsts::MOUSE_DOWN) {
        this->mouse_click_event(button, x, y);

    // Mouse up events
    } else if(event_type == GlobalConsts::MOUSE_UP) {
        this->mouse_release_event(button, x, y);

    // Mouse drag events
    } else if(event_type == GlobalConsts::MOUSE_DRAG) {
        // Glut handles mouse drag events oddly... this will probably have to change
        // if we switch to SDL or something else besides glut...
        if(this->old_mouse_pos[GlobalConsts::MOUSE_LEFT]["state"] == GlobalConsts::MOUSE_DOWN) {
            this->mouse_drag_event(GlobalConsts::MOUSE_LEFT, x, y);
        } else if(this->old_mouse_pos[GlobalConsts::MOUSE_MIDDLE]["state"] == GlobalConsts::MOUSE_DOWN) {
            this->mouse_drag_event(GlobalConsts::MOUSE_MIDDLE, x, y);
        } else if(this->old_mouse_pos[GlobalConsts::MOUSE_RIGHT]["state"] == GlobalConsts::MOUSE_DOWN) {
            this->mouse_drag_event(GlobalConsts::MOUSE_RIGHT, x, y);
        }
    }

    // If we have a click oriented event, record the change in button state
    if(event_type == GlobalConsts::MOUSE_DOWN || event_type == GlobalConsts::MOUSE_UP) {
        this->old_mouse_pos[button]["state"]  = event_type;
        this->old_mouse_pos[button]["x"]      = x;
        this->old_mouse_pos[button]["y"]      = y;
    }
}

void PlayerInput::mouse_click_event(int button, int x, int y) {
    if(button == GlobalConsts::MOUSE_LEFT) {
        this->call_py_mouse_func("mouse_left_click", x, y);
    } else if(button == GlobalConsts::MOUSE_MIDDLE) {
        this->call_py_mouse_func("mouse_middle_click", x, y);
    } else if(button == GlobalConsts::MOUSE_RIGHT) {
        this->call_py_mouse_func("mouse_right_click", x, y);
    } else if(button == GlobalConsts::MOUSE_SCROLL_UP) {
        this->call_py_mouse_func("mouse_scroll_up", x, y);
    } else if(button == GlobalConsts::MOUSE_SCROLL_DOWN) {
        this->call_py_mouse_func("mouse_scroll_down", x, y);
    }
}

void PlayerInput::mouse_release_event(int button, int x, int y) {
    if(button == GlobalConsts::MOUSE_LEFT) {
       this->call_py_mouse_func("mouse_left_release", x, y);
    } else if(button == GlobalConsts::MOUSE_MIDDLE) {
       this->call_py_mouse_func("mouse_middle_release", x, y);
    } else if(button == GlobalConsts::MOUSE_RIGHT) {
       this->call_py_mouse_func("mouse_right_release", x, y);
    }
}

void PlayerInput::mouse_drag_event(int button, int x, int y) {
    Controller* curr_ctrl = Controller::get_controller();
    int x_diff = x - this->old_mouse_pos[button]["x"];
    int y_diff = y - this->old_mouse_pos[button]["y"];

    if(button == GlobalConsts::MOUSE_LEFT) {
        this->call_py_mouse_func("mouse_left_drag", x_diff, y_diff);
    } else if(button == GlobalConsts::MOUSE_MIDDLE) {
        this->call_py_mouse_func("mouse_middle_drag", x_diff, y_diff);
    } else if(button == GlobalConsts::MOUSE_RIGHT) {
        this->call_py_mouse_func("mouse_right_drag", x_diff, y_diff);
    }

    this->old_mouse_pos[button]["x"]      = x;
    this->old_mouse_pos[button]["y"]      = y;
}

void PlayerInput::keyboard_event(int event_type, unsigned char key, int x, int y) {
    if(event_type == GlobalConsts::KEY_DOWN) {
        this->key_down(key, x, y);
    } else if(event_type == GlobalConsts::KEY_UP) {
        this->key_up(key, x, y);
    }
}

void PlayerInput::call_py_mouse_func(char* func_name, int x, int y){
    PyObject* py_x = PyInt_FromLong(x);
    PyObject* py_y = PyInt_FromLong(y);

    py_call_func(this->py_pointer, func_name, py_x, py_y);

    Py_XDECREF(py_x);
    Py_XDECREF(py_y);
}

void PlayerInput::key_down(unsigned char key, int x, int y) {
    char key_str[2] = {(char) key, '\0'};
    PyObject* py_str = PyString_FromString(key_str);
    PyObject* py_x = PyInt_FromLong(x);
    PyObject* py_y = PyInt_FromLong(y);

    py_call_func(this->py_pointer, "key_down", py_str, py_x, py_y);

    Py_XDECREF(py_str);
    Py_XDECREF(py_x);
    Py_XDECREF(py_y);
}

void PlayerInput::key_up(unsigned char key, int x, int y) {
    char key_str[2] = {(char) key, '\0'};
    PyObject* py_str = PyString_FromString(key_str);
    PyObject* py_x = PyInt_FromLong(x);
    PyObject* py_y = PyInt_FromLong(y);

    py_call_func(this->py_pointer, "key_up", py_str, py_x, py_y);

    Py_XDECREF(py_str);
    Py_XDECREF(py_x);
    Py_XDECREF(py_y);
}
