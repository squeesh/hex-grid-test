#ifndef UTIL_H
#define UTIL_H


struct cmp_str {
    bool operator()(char const *a, char const *b) {
        return strcmp(a, b) < 0;
    }
};


struct cmp_coord {
    bool operator()(std::vector<double> *vect_a, std::vector<double> *vect_b) const {
	// I have no idea why this works...
	std::vector<double> &temp_a = *vect_a;
	std::vector<double> &temp_b = *vect_b;

	return temp_a < temp_b;
    }
};


template <typename T>
class RoundVector : public std::vector<T> {
	public:
		T& operator[](std::vector<size_t>::size_type);
		const T& operator[](std::vector<size_t>::size_type) const;

		T& operator[](int);
		const T& operator[](int) const;

		T& at(std::vector<size_t>::size_type);
		const T& at(std::vector<size_t>::size_type) const;

		T& at(int index);
		const T& at(int) const;

		int size();
		int size() const;

		int get_index(int index);
		int get_index(int index) const;

		bool contains(T val, void* cmp_func);
};


template <typename T>
T& RoundVector<T>::operator[](std::vector<size_t>::size_type index) {
    std::vector<T>& vecRef = *this; // vector is not copied here
    return vecRef.at(this->get_index(index));
}

template <typename T>
const T& RoundVector<T>::operator[](std::vector<size_t>::size_type index) const {
    std::vector<T>& vecRef = *this; // vector is not copied here
    return vecRef.at(this->get_index(index));
}

template <typename T>
T& RoundVector<T>::operator[](int index) {
    std::vector<T>& vecRef = *this; // vector is not copied here
    return vecRef.at(this->get_index(index));
}

template <typename T>
const T& RoundVector<T>::operator[](int index) const {
    const std::vector<T>& vecRef = *this; // vector is not copied here
    return vecRef.at(this->get_index(index));
}

template <typename T>
T& RoundVector<T>::at(std::vector<size_t>::size_type index) {
	std::vector<T>& vecRef = *this; // vector is not copied here
	return vecRef.at(this->get_index(index));
}

template <typename T>
const T& RoundVector<T>::at(std::vector<size_t>::size_type index) const {
	std::vector<T>& vecRef = *this; // vector is not copied here
	return vecRef.at(this->get_index(index));
}

template <typename T>
T& RoundVector<T>::at(int index) {
	std::vector<T>& vecRef = *this; // vector is not copied here
	return vecRef.at(this->get_index(index));
}

template <typename T>
const T& RoundVector<T>::at(int index) const {
	const std::vector<T>& vecRef = *this; // vector is not copied here
	return vecRef.at(this->get_index(index));
}

template <typename T>
int RoundVector<T>::size() {
    std::vector<T>& vecRef = *this; // vector is not copied here
    return ((int)vecRef.size());
}

template <typename T>
int RoundVector<T>::size() const {
    const std::vector<T>& vecRef = *this; // vector is not copied here
    return ((int)vecRef.size());
}

template <typename T>
int RoundVector<T>::get_index(int index) {
    index = index % this->size();

    if(index < 0) {
        index = this->size() + index;
    }

    return index;
}

template <typename T>
int RoundVector<T>::get_index(int index) const {
    index = index % this->size();

    if(index < 0) {
        index = this->size() + index;
    }

    return index;
}

template <typename T>
bool RoundVector<T>::contains(T val, void* cmp_func) {
	for(int i = 0; i < this->size(); i++) {
		if(cmp_func(this->at(i), val)) {
			return true;
		}
	}

	return false;
}


PyObject* py_call_func(PyObject *py_obj, char* func_name);
PyObject* py_call_func(PyObject *py_obj, char* func_name, void* obj_arg_1);
PyObject* py_call_func(PyObject *py_obj, char* func_name, void* obj_arg_1, void* obj_arg_2);
PyObject* py_call_func(PyObject *py_obj, char* func_name, void* obj_arg_1, void* obj_arg_2, void* obj_arg_3);
PyObject* py_call_func(PyObject *py_obj, char* func_name, PyObject* py_obj_1, PyObject* py_obj_2, PyObject* py_obj_3);

/***************/

class CoordinateVector { 
	private:
		std::vector<double> data;
		std::map< std::vector<double>*, int, cmp_coord> index_data;
	public:
		CoordinateVector();

		double* at(int);
		int push_back(double, double, double);
		//void set(int, double, double, double);
		int size();
		int get_index(double, double, double);
		int get_index(std::vector<double>*);
};

#endif
