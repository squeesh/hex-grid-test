#ifndef VERTEX_H
#define VERTEX_H


class Vertex {
    private:
        //std::vector< GLdouble >* color;
        GLdouble height;

    public:
        //Vertex(GLdouble red, GLdouble green, GLdouble blue, GLdouble height);
        Vertex(GLdouble height);

        //void set_color(GLdouble red, GLdouble green, GLdouble blue);
        //void set_color(std::vector<GLdouble> rgb);
        //std::vector<GLdouble>* get_color();

        void set_height(GLdouble height);
        GLdouble get_height();

};

#endif

