#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H


class PlayerInput {
    private:
        PyObject* py_pointer;

    public:
        std::map<int, std::map<const char*, int, cmp_str> > old_mouse_pos;

        PlayerInput(PyObject*);

        void mouse_event(int, int, int, int);

        void mouse_click_event(int, int, int);
        void mouse_release_event(int, int, int);
        void mouse_drag_event(int, int, int);

        void keyboard_event(int, unsigned char, int, int);

        void call_py_mouse_func(char*, int, int);

        void key_down(unsigned char, int, int);
        void key_up(unsigned char, int, int);
};


#endif
