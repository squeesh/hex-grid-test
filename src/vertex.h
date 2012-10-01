#ifndef VERTEX_H
#define VERTEX_H


class Vertex {
	private:
		std::vector< double >* color;
		double height;

	public:
		Vertex(double red, double green, double blue, double height);

		void set_color(double red, double green, double blue);
		void set_color(std::vector<double> rgb);
		std::vector<double>* get_color();

		void set_height(double height);
		double get_height();

};

#endif

