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

CoordinateVector::CoordinateVector() {

}

double* CoordinateVector::at(int index) {
	if(index < this->size()) {
		// fun with pointers...
		return (double*)((int)this->data.data() + (sizeof(double) * index * 3));
	} else {
		// throw whatever error at() throws... 
		// TODO: QUIT BEING LAZY
		double output = this->data.at(-1);

		return &output;
	}
}

void CoordinateVector::push_back(double x, double y, double z) {
	std::vector<double>* curr_coords = new std::vector<double>();
	curr_coords->push_back(x);
	curr_coords->push_back(y);
	curr_coords->push_back(z);

	int count = this->index_data.count(curr_coords);

	if(count == 0) {
		this->data.push_back(x);
		this->data.push_back(y);
		this->data.push_back(z);

		this->index_data[curr_coords] = (this->data.size() / 3) - 1;
	} else {
		delete curr_coords;
	}
}

/*void CoordinateVector::set(int index, double x, double y, double z) {
	this->data[index] = x;
	this->data[index+1] = y;
	this->data[index+2] = z;
}*/

int CoordinateVector::size() {
	return this->index_data.size();
}

int CoordinateVector::get_index(double x, double y, double z) {
	std::vector<double>* curr_coords = new std::vector<double>();
	curr_coords->push_back(x);
	curr_coords->push_back(y);
	curr_coords->push_back(z);

	//std::cout << "count: " << this->index_data.count(curr_coords) << std::endl;

	return this->index_data[curr_coords];
}

