#ifndef COLOR_H
#define COLOR_H


class Color {
	private:
		double red;
		double green;
		double blue;
	
	public:
		Color(double red, double green, double blue);
		Color(std::vector<double> rgb);

		void set_rgb(double red, double green, double blue);
		void set_rgb(std::vector<double> rgb);
		
		std::vector<double> get_rgb(void);
};

#endif
