#include "includes.h"

PyObject* get_py_attr(const char* attr_str) {
	PyObject *py_name = PyString_FromString("src.global_consts");
	PyObject *py_module = PyImport_Import(py_name);
	Py_XDECREF(py_name);

	PyObject *py_global_consts = PyObject_GetAttrString(py_module, "GlobalConsts");
	Py_XDECREF(py_module);

	PyObject *py_attr  = PyObject_GetAttrString(py_global_consts, attr_str);
	Py_XDECREF(py_global_consts);

	return py_attr;
}

long long_global_const_attr(const char* attr_str) {
	bool py_needed_init = false;

	if(!Py_IsInitialized() ) {
		Py_Initialize();
		py_needed_init = true;
	}

	PyObject *py_attr = get_py_attr(attr_str);

	long output = PyInt_AsLong(py_attr);
	Py_XDECREF(py_attr);

	if(py_needed_init) {
		Py_Finalize();
	}

	return output;
}


char char_global_const_attr(const char* attr_str) {
	bool py_needed_init = false;

	if(!Py_IsInitialized() ) {
		Py_Initialize();
		py_needed_init = true;
	}

	PyObject *py_attr = get_py_attr(attr_str);

	char output = PyString_AsString(py_attr)[0];
	Py_XDECREF(py_attr);

	if(py_needed_init) {
		Py_Finalize();
	}

	return output;
}


double double_global_const_attr(const char* attr_str) {
	bool py_needed_init = false;

	if(!Py_IsInitialized() ) {
		Py_Initialize();
		py_needed_init = true;
	}

	PyObject *py_attr = get_py_attr(attr_str);

	double output = PyFloat_AsDouble(py_attr);
	Py_XDECREF(py_attr);

	if(py_needed_init) {
		Py_Finalize();
	}

	return output;
}


const char GlobalConsts::LEFT  = char_global_const_attr("LEFT");
const char GlobalConsts::RIGHT = char_global_const_attr("RIGHT");
const char GlobalConsts::UP    = char_global_const_attr("UP");
const char GlobalConsts::DOWN  = char_global_const_attr("DOWN");

const int GlobalConsts::MOUSE_LEFT 	  = long_global_const_attr("MOUSE_LEFT");
const int GlobalConsts::MOUSE_MIDDLE 	  = long_global_const_attr("MOUSE_MIDDLE");
const int GlobalConsts::MOUSE_RIGHT 	  = long_global_const_attr("MOUSE_RIGHT");
const int GlobalConsts::MOUSE_SCROLL_UP   = long_global_const_attr("MOUSE_SCROLL_UP");
const int GlobalConsts::MOUSE_SCROLL_DOWN = long_global_const_attr("MOUSE_SCROLL_DOWN");

const int GlobalConsts::RENDER_LINES     = long_global_const_attr("RENDER_LINES");
const int GlobalConsts::RENDER_TRIANGLES = long_global_const_attr("RENDER_TRIANGLES");

const long GlobalConsts::BOARD_WIDTH  = long_global_const_attr("BOARD_WIDTH");
const long GlobalConsts::BOARD_HEIGHT = long_global_const_attr("BOARD_HEIGHT");

const double GlobalConsts::MIN_ZOOM = double_global_const_attr("MIN_ZOOM");
const double GlobalConsts::MAX_ZOOM = double_global_const_attr("MAX_ZOOM");

const double GlobalConsts::MAX_PATHABLE_SLOPE = double_global_const_attr("MAX_PATHABLE_SLOPE");


