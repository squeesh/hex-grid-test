#include "includes.h"


PyObject* py_call_func(PyObject *py_obj, char* func_name) {
    PyObject *py_func = PyObject_GetAttrString(py_obj, func_name);
    PyObject *output = PyObject_CallFunctionObjArgs(py_func, NULL);
    Py_XDECREF(py_func);
    return output;
}

PyObject* py_call_func(PyObject *py_obj, char* func_name, void* obj_arg_1) {
	PyObject *py_obj_addr_1 = PyInt_FromLong((long)obj_arg_1);

	PyObject *py_func = PyObject_GetAttrString(py_obj, func_name);
	PyObject *output = PyObject_CallFunctionObjArgs(py_func, py_obj_addr_1, NULL);
	Py_XDECREF(py_func);
	Py_XDECREF(py_obj_addr_1);

	return output;
}

PyObject* py_call_func(PyObject *py_obj, char* func_name, void* obj_arg_1, void* obj_arg_2) {
	PyObject *py_obj_addr_1 = PyInt_FromLong((long)obj_arg_1);
	PyObject *py_obj_addr_2 = PyInt_FromLong((long)obj_arg_2);

	PyObject *py_func = PyObject_GetAttrString(py_obj, func_name);
	PyObject *output = PyObject_CallFunctionObjArgs(py_func, py_obj_addr_1, py_obj_addr_2, NULL);
	Py_XDECREF(py_func);
	Py_XDECREF(py_obj_addr_1);
	Py_XDECREF(py_obj_addr_2);

	return output;
}

PyObject* py_call_func(PyObject *py_obj, char* func_name, void* obj_arg_1, void* obj_arg_2, void* obj_arg_3) {
    PyObject *py_obj_addr_1 = PyInt_FromLong((long)obj_arg_1);
    PyObject *py_obj_addr_2 = PyInt_FromLong((long)obj_arg_2);
    PyObject *py_obj_addr_3 = PyInt_FromLong((long)obj_arg_3);

    PyObject *py_func = PyObject_GetAttrString(py_obj, func_name);
    PyObject *output = PyObject_CallFunctionObjArgs(py_func, py_obj_addr_1, py_obj_addr_2, py_obj_addr_3, NULL);
    Py_XDECREF(py_func);
    Py_XDECREF(py_obj_addr_1);
    Py_XDECREF(py_obj_addr_2);
    Py_XDECREF(py_obj_addr_3);

    return output;
}

PyObject* py_call_func(PyObject *py_obj, char* func_name, PyObject* py_obj_1, PyObject* py_obj_2, PyObject* py_obj_3) {
    PyObject *py_func = PyObject_GetAttrString(py_obj, func_name);
    PyObject *output = PyObject_CallFunctionObjArgs(py_func, py_obj_1, py_obj_2, py_obj_3, NULL);
    Py_XDECREF(py_func);

    return output;
}

/***************/


