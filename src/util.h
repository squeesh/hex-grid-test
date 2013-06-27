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


bool color_changed(std::vector< GLdouble >* color_a, std::vector< GLdouble >* color_b);
bool color_changed(GLdouble red_a, GLdouble green_a, GLdouble blue_a, GLdouble red_b, GLdouble green_b, GLdouble blue_b);


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
PyObject* py_call_func(PyObject *py_obj, char* func_name, PyObject* py_obj_1, PyObject* py_obj_2);
PyObject* py_call_func(PyObject *py_obj, char* func_name, PyObject* py_obj_1, PyObject* py_obj_2, PyObject* py_obj_3);

/***************/

template <typename T>
class UniqueDataVector {
	private:
		static const int VERTEX_STRIDE;
		static const int COLOR_STRIDE;

		static const int VERTEX_OFFSET;
		static const int COLOR_OFFSET;

		std::map< std::vector<T>*, GLuint, cmp_coord<T> > *index_data;
		std::vector< std::vector<T> *> *index_data_keys;

		std::vector<T> *vectors;
		std::vector< GLuint > *indicies;

		GLuint vbo_vert;
		GLuint vbo_indicie;

		GLvoid* tex_data;

	public:
		UniqueDataVector();
		~UniqueDataVector();

		T* at(int);
		GLuint push_back(T, T, T, T, T, T);
		GLuint push_back(T, T, T, std::vector<T>*);
		GLuint push_back(std::vector<T>*, std::vector<T>*);

		void set_tex_data(GLvoid*);

		int vector_size();
		int indicies_size();

		GLuint get_index(std::vector<T>*);
		T* vector_data();
		GLuint* indicies_data();
		void reserve(int);
		void reverse_indicies();
		void reverse_verticies();

		void write_VBO_data(GLenum gl_mode);
		void bind_for_draw(GLenum gl_mode);
};

template <typename T>
UniqueDataVector<T>::UniqueDataVector() {
	this->vectors = new std::vector<T>();
	this->index_data = new std::map< std::vector<T>*, GLuint, cmp_coord<T> >();
	this->index_data_keys = new std::vector< std::vector<T> *>(); // for easy garbage collection index_data
	this->indicies = new std::vector< GLuint >();

	this->vbo_vert = 0;
	this->vbo_indicie = 0;

	glGenBuffers(1, &(this->vbo_vert));
	glGenBuffers(1, &(this->vbo_indicie));
}

template <typename T>
UniqueDataVector<T>::~UniqueDataVector() {
	delete this->vectors;
	delete this->index_data;
	delete this->indicies;

	std::vector< std::vector<T> *> &curr_vec = *(this->index_data_keys);
	for(int i = 0; i < this->index_data_keys->size(); i++) {
	delete curr_vec[i];
	}
	delete this->index_data_keys;

	glDeleteBuffers(1, &(this->vbo_vert));
	glDeleteBuffers(1, &(this->vbo_indicie));
}

template <typename T>
T* UniqueDataVector<T>::at(int index) {
	if(index < this->size()) {
		// fun with pointers...
		return (T*)((GLlong)(this->data()) + (sizeof(T) * index * 3));
	} else {
		// throw whatever error at() throws...
		// TODO: QUIT BEING LAZY
		T output = this->vectors->at(-1);

		return &output;
	}
}

template <typename T>
GLuint UniqueDataVector<T>::push_back(T x, T y, T z, T r, T g, T b) {
	std::vector<T>* curr_coords = new std::vector<T>();
	curr_coords->push_back(x);
	curr_coords->push_back(y);
	curr_coords->push_back(z);

	curr_coords->push_back(r);
	curr_coords->push_back(g);
	curr_coords->push_back(b);

	GLuint index = 0;

	if(this->index_data->count(curr_coords) == 0) {
	    std::map< std::vector<T>*, GLuint, cmp_coord<T> > &curr_index_data = *(this->index_data);

		this->vectors->push_back(x);
		this->vectors->push_back(y);
		this->vectors->push_back(z);

		this->vectors->push_back(r);
		this->vectors->push_back(g);
		this->vectors->push_back(b);

		this->vectors->push_back(0);
		this->vectors->push_back(0);

		index = (GLuint)(this->vectors->size() / 8.0) - 1;
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

template <typename T>
GLuint UniqueDataVector<T>::push_back(T x, T y, T z, std::vector<T>* vec_rgb) {
	return this->push_back(x, y, z, vec_rgb->at(0), vec_rgb->at(1), vec_rgb->at(2));
}


template <typename T>
GLuint UniqueDataVector<T>::push_back(std::vector<T>* vec_xyz, std::vector<T>* vec_rgb) {
	return this->push_back(vec_xyz->at(0), vec_xyz->at(1), vec_xyz->at(2), vec_rgb->at(0), vec_rgb->at(1), vec_rgb->at(2));
}

/*void UniqueDataVector::set(int index, GLdouble x, GLdouble y, GLdouble z) {
	this->data[index] = x;
	this->data[index+1] = y;
	this->data[index+2] = z;
}*/

/*void UniqueDataVector::set(int index, GLdouble x, GLdouble y, GLdouble z) {
	this->data[index] = x;
	this->data[index+1] = y;
	this->data[index+2] = z;
}*/

template <typename T>
void UniqueDataVector<T>::set_tex_data(GLvoid* new_tex_data) {
	this->tex_data = new_tex_data;
}

template <typename T>
int UniqueDataVector<T>::vector_size() {
	return this->vectors->size();
}

template <typename T>
int UniqueDataVector<T>::indicies_size() {
	return this->indicies->size();
}

template <typename T>
GLuint UniqueDataVector<T>::get_index(std::vector<T>* curr_coords) {
    std::map< std::vector<T>*, GLuint, cmp_coord<T> > &curr_index_data = *(this->index_data);
    return curr_index_data[curr_coords];
}

template <typename T>
T* UniqueDataVector<T>::vector_data() {
    return this->vectors->data();
}

template <typename T>
GLuint* UniqueDataVector<T>::indicies_data() {
    return this->indicies->data();
}


template <typename T>
void UniqueDataVector<T>::reserve(int reserve) {
    this->vectors->reserve(reserve);
    //this->vectors->reserve(reserve / 3);
}


template <typename T>
void UniqueDataVector<T>::reverse_indicies() {
    std::reverse(this->indicies->begin(), this->indicies->end());
}


template <typename T>
void UniqueDataVector<T>::reverse_verticies() {
	std::reverse(this->vectors->begin(), this->vectors->end());
}

template <typename T>
void UniqueDataVector<T>::write_VBO_data(GLenum gl_mode) {
	GLsizeiptr vert_size = sizeof(T) * this->vector_size();

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vert);
	glBufferData(GL_ARRAY_BUFFER, vert_size, NULL, gl_mode);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vert_size, this->vector_data());

	GLsizeiptr indicie_size = sizeof(GLuint) * this->indicies_size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_indicie);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicie_size, NULL, gl_mode);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indicie_size, this->indicies_data());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

template <typename T>
void UniqueDataVector<T>::bind_for_draw(GLenum gl_mode) {
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_vert);

	// vertex VBO
	glVertexPointer(3, gl_mode, this->VERTEX_STRIDE, (void*)(this->VERTEX_OFFSET));
	glColorPointer(3, gl_mode, this->COLOR_STRIDE,  (void*)(this->COLOR_OFFSET));

	// bind indicie VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->vbo_indicie);
}

template <typename T>
const int UniqueDataVector<T>::VERTEX_STRIDE = sizeof(T) * 8;
template <typename T>
const int UniqueDataVector<T>::COLOR_STRIDE  = sizeof(T) * 8;

template <typename T>
const int UniqueDataVector<T>::VERTEX_OFFSET = sizeof(T) * 0;
template <typename T>
const int UniqueDataVector<T>::COLOR_OFFSET  = sizeof(T) * 3;

//----------------------------------------


class PythonException : public std::exception {};


template <typename T>
std::vector<T>* remove_from_vector(std::vector<T>* vec, T elem) {
	vec->erase(std::find(vec->begin(), vec->end(), elem));

	return vec;

	/*int pos = -1;
	int vec_size = vec->size();
	for(int i = 0; i < vec_size; i++) {
		if(vec->at(i) == elem) {
			pos = i;
			break;
		}
	}

	if(pos >= 0) {
		vec->erase(vec->begin()+pos);
	} else {
		T temp = vec->at(-1);
	}

	return vec;*/
}

template <typename T>
bool item_in_vector(std::vector<T>* vec, T elem) {
	//std::sort(vec->begin(), vec->end());
	//return std::binary_search(vec->begin(), vec->end(), elem);

	return std::find(vec->begin(), vec->end(), elem) != vec->end();

	/*int vec_size = vec->size();
	for(int i = 0; i < vec_size; i++) {
		if(vec->at(i) == elem) {
			return true;
		}
	}

	return false;*/
}


#endif
