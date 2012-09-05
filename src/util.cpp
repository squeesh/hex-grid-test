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

TightlyPackedVector::TightlyPackedVector() {
    this->vector_data = new std::vector<double>();
    this->index_data = new std::map< std::vector<double>*, int, cmp_coord>();
    this->index_data_keys = new std::vector< std::vector<double> *>();
}

TightlyPackedVector::~TightlyPackedVector() {
    delete this->vector_data;
    delete this->index_data;

    std::vector< std::vector<double> *> &curr_vec = *(this->index_data_keys);
    for(int i = 0; i < this->index_data_keys->size(); i++) {
        delete curr_vec[i];
    }
    delete this->index_data_keys;
}

double* TightlyPackedVector::at(int index) {
	if(index < this->size()) {
		// fun with pointers...
		return (double*)((int)this->data() + (sizeof(double) * index * 3));
	} else {
		// throw whatever error at() throws... 
		// TODO: QUIT BEING LAZY
		double output = this->vector_data->at(-1);

		return &output;
	}
}

int TightlyPackedVector::push_back(double x, double y, double z) {
	std::vector<double>* curr_coords = new std::vector<double>();
	curr_coords->push_back(x);
	curr_coords->push_back(y);
	curr_coords->push_back(z);

	int index = 0;

	if(this->index_data->count(curr_coords) == 0) {
	    std::map< std::vector<double>*, int, cmp_coord> &curr_index_data = *(this->index_data);

		this->vector_data->push_back(x);
		this->vector_data->push_back(y);
		this->vector_data->push_back(z);

		index = this->size() - 1;
		curr_index_data[curr_coords] = index;
		this->index_data_keys->push_back(curr_coords);
	} else {	
		index = this->get_index(curr_coords);
		delete curr_coords;
	}

	return index;
}

/*void TightlyPackedVector::set(int index, double x, double y, double z) {
	this->data[index] = x;
	this->data[index+1] = y;
	this->data[index+2] = z;
}*/

int TightlyPackedVector::size() {
	return this->index_data->size();
}

int TightlyPackedVector::get_index(double x, double y, double z) {
	std::vector<double>* curr_coords = new std::vector<double>();
	curr_coords->push_back(x);
	curr_coords->push_back(y);
	curr_coords->push_back(z);

	//std::cout << "count: " << this->index_data.count(curr_coords) << std::endl;

	int output = this->get_index(curr_coords);
	delete curr_coords;

	return output;
}

int TightlyPackedVector::get_index(std::vector<double>* curr_coords) {
    std::map< std::vector<double>*, int, cmp_coord> &curr_index_data = *(this->index_data);
	return curr_index_data[curr_coords];
}

double* TightlyPackedVector::data() {
    return this->vector_data->data();
}

void TightlyPackedVector::reserve(int reserve) {
    this->vector_data->reserve(reserve);
    this->vector_data->reserve(reserve / 3);
}
