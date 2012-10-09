#ifndef HEXAGON_H
#define HEXAGON_H

class GameboardChunk;


class Hexagon {
	private:

		std::map<const char*, Hexagon*, cmp_str> neighbor_hexagons;

		std::vector< double >* select_color;
		std::vector< double >* hex_color;

		void base_init();
	public:
		static const RoundVector<const char*>* VERTEX_POSITIONS;
		static const RoundVector<const char*>* NEIGHBOR_DIRECTION;

		static const RoundVector< RoundVector<double>* >* ROT_COORDS;

		int name;
		static int name_count;

		GameboardChunk* parent_chunk;

		std::map<const char*, Vertex*, cmp_str> verticies;

		double last_x;
		double last_y;	

		Hexagon();

		void set_vertex(const char* position, Vertex* vertex);
		Vertex* get_vertex(const char* position);

		void set_border_color(double red, double green, double blue);
		void set_border_color(std::vector< double > rgb);

		void set_hex_color(double red, double green, double blue);
		void set_hex_color(std::vector< double > rgb);
		std::vector< double >* get_hex_color();

		void set_select_color(double red, double green, double blue);
		void set_select_color(std::vector< double > rgb);
		void clear_select_color();
		std::vector< double >* get_select_color();

		void set_height(double height);
		void add_height(double height);
		double get_height();

		double get_slope();
		bool is_pathable();

		void set_neighbor(const char* position, Hexagon* neighbor_hex);
		Hexagon* get_neighbor(const char* position);

		void render_for_select(double, double);

		void generate_vertex_data(double, double, UniqueDataVector< GLfloat >*);
		void generate_select_data(double, double, UniqueDataVector< GLfloat >*);
		void generate_object_data(double, double, UniqueDataVector< GLfloat >*);
};

#endif

