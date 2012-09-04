#include <vector>


class Coordinate {
	private:
		double x;
		double y;
	
	public:
		Coordinate(double x, double y);

		void set(double x, double y);
		std::vector<double> get();

		void set_x(double x);
		void set_y(double y);
		
		double get_x();
		double get_y();
};

