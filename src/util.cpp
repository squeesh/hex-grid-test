#include "includes.h"


PyObject* py_call_func(PyObject *py_obj, char* func_name) {
    try {
        PyObject *py_func = PyObject_GetAttrString(py_obj, func_name);
        if(PyErr_Occurred()) {
            throw PythonException();
        }
        PyObject *output = PyObject_CallFunctionObjArgs(py_func, NULL);
        if(PyErr_Occurred()) {
            throw PythonException();
        }
        Py_XDECREF(py_func);

        return output;

    } catch(PythonException &e) {
        PyErr_Print();
        exit(0);
    }
}

PyObject* py_call_func(PyObject *py_obj, char* func_name, void* obj_arg_1) {
    try {
        PyObject *py_obj_addr_1 = PyInt_FromLong((GLlong)obj_arg_1);

        PyObject *py_func = PyObject_GetAttrString(py_obj, func_name);
        if(PyErr_Occurred()) {
            throw PythonException();
        }
        PyObject *output = PyObject_CallFunctionObjArgs(py_func, py_obj_addr_1, NULL);
        if(PyErr_Occurred()) {
            throw PythonException();
        }
        Py_XDECREF(py_func);
        Py_XDECREF(py_obj_addr_1);

        return output;

    } catch(PythonException &e) {
        PyErr_Print();
        exit(0);
    }
}

PyObject* py_call_func(PyObject *py_obj, char* func_name, void* obj_arg_1, void* obj_arg_2) {
    try {
        PyObject *py_obj_addr_1 = PyInt_FromLong((GLlong)obj_arg_1);
        PyObject *py_obj_addr_2 = PyInt_FromLong((GLlong)obj_arg_2);

        PyObject *py_func = PyObject_GetAttrString(py_obj, func_name);
        if(PyErr_Occurred()) {
            throw PythonException();
        }
        PyObject *output = PyObject_CallFunctionObjArgs(py_func, py_obj_addr_1, py_obj_addr_2, NULL);
        if(PyErr_Occurred()) {
            throw PythonException();
        }
        Py_XDECREF(py_func);
        Py_XDECREF(py_obj_addr_1);
        Py_XDECREF(py_obj_addr_2);

        return output;

    } catch(PythonException &e) {
        PyErr_Print();
        exit(0);
    }
}

PyObject* py_call_func(PyObject *py_obj, char* func_name, void* obj_arg_1, void* obj_arg_2, void* obj_arg_3) {
    try {
        PyObject *py_obj_addr_1 = PyInt_FromLong((GLlong)obj_arg_1);
        PyObject *py_obj_addr_2 = PyInt_FromLong((GLlong)obj_arg_2);
        PyObject *py_obj_addr_3 = PyInt_FromLong((GLlong)obj_arg_3);

        PyObject *py_func = PyObject_GetAttrString(py_obj, func_name);
        if(PyErr_Occurred()) {
            throw PythonException();
        }
        PyObject *output = PyObject_CallFunctionObjArgs(py_func, py_obj_addr_1, py_obj_addr_2, py_obj_addr_3, NULL);
        if(PyErr_Occurred()) {
            throw PythonException();
        }
        Py_XDECREF(py_func);
        Py_XDECREF(py_obj_addr_1);
        Py_XDECREF(py_obj_addr_2);
        Py_XDECREF(py_obj_addr_3);

        return output;

    } catch(PythonException &e) {
        PyErr_Print();
        exit(0);
    }
}

PyObject* py_call_func(PyObject *py_obj, char* func_name, PyObject* py_obj_1, PyObject* py_obj_2) {
    try {
        PyObject *py_func = PyObject_GetAttrString(py_obj, func_name);
        if(PyErr_Occurred()) {
            throw PythonException();
        }
        PyObject *output = PyObject_CallFunctionObjArgs(py_func, py_obj_1, py_obj_2, NULL);
        if(PyErr_Occurred()) {
            throw PythonException();
        }
        Py_XDECREF(py_func);

        return output;

    } catch(PythonException &e) {
        PyErr_Print();
        exit(0);
    }
}

PyObject* py_call_func(PyObject *py_obj, char* func_name, PyObject* py_obj_1, PyObject* py_obj_2, PyObject* py_obj_3) {
    try {
        PyObject *py_func = PyObject_GetAttrString(py_obj, func_name);
        if(PyErr_Occurred()) {
            throw PythonException();
        }
        PyObject *output = PyObject_CallFunctionObjArgs(py_func, py_obj_1, py_obj_2, py_obj_3, NULL);
        if(PyErr_Occurred()) {
            throw PythonException();
        }
        Py_XDECREF(py_func);

        return output;

    } catch(PythonException &e) {
        PyErr_Print();
        exit(0);
    }
}

/***************/


bool color_changed(std::vector< GLdouble >* color_a, std::vector< GLdouble >* color_b) {
	if(color_a->size() != color_b->size()) {
		return true;
	}

	if(color_a->size() == 0 && color_b->size() == 0) {
		return false;
	}

	return (color_a->at(0) != color_b->at(0)) || (color_a->at(1) != color_b->at(1)) || (color_a->at(2) != color_b->at(2));
}


bool color_changed(GLdouble red_a, GLdouble green_a, GLdouble blue_a, GLdouble red_b, GLdouble green_b, GLdouble blue_b) {
	return (red_a != red_b) || (green_a != green_b) || (blue_a != blue_b);
}

