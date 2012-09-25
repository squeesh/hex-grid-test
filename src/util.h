#ifndef UTIL_H
#define UTIL_H


struct cmp_str {
    bool operator()(char const *a, char const *b) {
        return strcmp(a, b) < 0;
    }
};


template <typename T>
struct cmp_coord {
    bool operator()(std::vector<T> *vect_a, std::vector<T> *vect_b) const {
	// I have no idea why this works...
	std::vector<T> &temp_a = *vect_a;
	std::vector<T> &temp_b = *vect_b;

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

template <typename T>
class UniqueDataVector { 
	private:
		std::vector<T> *vector_data;
		std::vector<T> *color;
		std::map< std::vector<T>*, int, cmp_coord<T> > *index_data;
		std::vector< std::vector<T> *> *index_data_keys;
		std::vector< int > *indicies;
	public:
		UniqueDataVector();
		~UniqueDataVector();

		T* at(int);
		int push_back(T, T, T, T, T, T);
		int size();
		int indicies_size();
		int get_index(T, T, T);
		int get_index(std::vector<T>*);
		T* data();
		int* indicies_data();
		T* color_data();
		void reserve(int);
		void reverse_indicies();
		void reverse_verticies();
};

template <typename T>
UniqueDataVector<T>::UniqueDataVector() {
    this->vector_data = new std::vector<T>();
    this->color = new std::vector<T>();
    this->index_data = new std::map< std::vector<T>*, int, cmp_coord<T> >();
    this->index_data_keys = new std::vector< std::vector<T> *>();
    this->indicies = new std::vector< int >();
}

template <typename T>
UniqueDataVector<T>::~UniqueDataVector() {
    delete this->vector_data;
    delete this->index_data;
    delete this->color;
    delete this->indicies;

    std::vector< std::vector<T> *> &curr_vec = *(this->index_data_keys);
    for(int i = 0; i < this->index_data_keys->size(); i++) {
        delete curr_vec[i];
    }
    delete this->index_data_keys;
}

template <typename T>
T* UniqueDataVector<T>::at(int index) {
	if(index < this->size()) {
		// fun with pointers...
		return (T*)((long)(this->data()) + (sizeof(T) * index * 3));
	} else {
		// throw whatever error at() throws... 
		// TODO: QUIT BEING LAZY
		T output = this->vector_data->at(-1);

		return &output;
	}
}

template <typename T>
int UniqueDataVector<T>::push_back(T x, T y, T z, T r, T g, T b) {
	std::vector<T>* curr_coords = new std::vector<T>();
	curr_coords->push_back(x);
	curr_coords->push_back(y);
	curr_coords->push_back(z);

	curr_coords->push_back(r);
	curr_coords->push_back(g);
	curr_coords->push_back(b);

	int index = 0;

	if(this->index_data->count(curr_coords) == 0) {
	    std::map< std::vector<T>*, int, cmp_coord<T> > &curr_index_data = *(this->index_data);

		this->vector_data->push_back(x);
		this->vector_data->push_back(y);
		this->vector_data->push_back(z);

		this->color->push_back(r);
		this->color->push_back(g);
		this->color->push_back(b);

		index = (int)(this->vector_data->size() / 3.0) - 1;
		this->indicies->push_back(index);
		curr_index_data[curr_coords] = index;
		this->index_data_keys->push_back(curr_coords);
	} else {	
		index = this->get_index(curr_coords);
		this->indicies->push_back(index);
		delete curr_coords;
	}

	return index;
}

/*void UniqueDataVector::set(int index, double x, double y, double z) {
	this->data[index] = x;
	this->data[index+1] = y;
	this->data[index+2] = z;
}*/

/*void UniqueDataVector::set(int index, double x, double y, double z) {
	this->data[index] = x;
	this->data[index+1] = y;
	this->data[index+2] = z;
}*/

template <typename T>
int UniqueDataVector<T>::size() {
	return this->index_data->size();
}

template <typename T>
int UniqueDataVector<T>::indicies_size() {
	return this->indicies->size();
}

template <typename T>
int UniqueDataVector<T>::get_index(T x, T y, T z) {
	std::vector<T>* curr_coords = new std::vector<T>();
	curr_coords->push_back(x);
	curr_coords->push_back(y);
	curr_coords->push_back(z);

	//std::cout << "count: " << this->index_data.count(curr_coords) << std::endl;

	int output = this->get_index(curr_coords);
	delete curr_coords;

	return output;
}

template <typename T>
int UniqueDataVector<T>::get_index(std::vector<T>* curr_coords) {
    std::map< std::vector<T>*, int, cmp_coord<T> > &curr_index_data = *(this->index_data);
    return curr_index_data[curr_coords];
}

template <typename T>
T* UniqueDataVector<T>::data() {
    return this->vector_data->data();
}

template <typename T>
int* UniqueDataVector<T>::indicies_data() {
    return this->indicies->data();
}

template <typename T>
T* UniqueDataVector<T>::color_data() {
    return this->color->data();
}

template <typename T>
void UniqueDataVector<T>::reserve(int reserve) {
    this->vector_data->reserve(reserve);
    this->vector_data->reserve(reserve / 3);
}


template <typename T>
void UniqueDataVector<T>::reverse_indicies() {
    std::reverse(this->indicies->begin(), this->indicies->end()); 
}


template <typename T>
void UniqueDataVector<T>::reverse_verticies() {
	std::reverse(this->vector_data->begin(), this->vector_data->end()); 
	std::reverse(this->color->begin(), this->color->end()); 
}


#endif
