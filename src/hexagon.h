#ifndef HEXAGON_H
#define HEXAGON_H

class GameboardChunk;
class BoardObject;


class Hexagon {
	private:
		std::map<const char*, Hexagon*, cmp_str> neighbor_hexagons;

		std::vector< GLdouble >* select_color;
		std::vector< GLdouble >* hex_color;

		std::map< const char*, bool, cmp_str > improvements;

		void base_init();

		static GLdouble get_g_score(Hexagon*, Hexagon*);
		static GLdouble get_h_score(Hexagon*, Hexagon*);

	public:
		static const RoundVector<const char*>* VERTEX_POSITIONS;
		static const RoundVector<const char*>* NEIGHBOR_DIRECTION;

		static const RoundVector< RoundVector<GLdouble>* >* ROT_COORDS;

		int name;
		static int name_count;

		GameboardChunk* parent_chunk;

		std::map<const char*, Vertex*, cmp_str> verticies;

		GLdouble last_x;
		GLdouble last_y;

		Hexagon();

		void set_vertex(const char* position, Vertex* vertex);
		Vertex* get_vertex(const char* position);

		/*void set_border_color(GLdouble red, GLdouble green, GLdouble blue);
		void set_border_color(std::vector< GLdouble > rgb);*/

		void set_improvement(const char*, bool);
		bool get_improvement(const char*);

		void set_hex_color(GLdouble red, GLdouble green, GLdouble blue);
		void set_hex_color(std::vector< GLdouble > rgb);
		std::vector< GLdouble >* get_hex_color();

		void set_select();
		void clear_select();

		void set_select_color(GLdouble red, GLdouble green, GLdouble blue);
		void set_select_color(std::vector< GLdouble > rgb);
		void clear_select_color();
		std::vector< GLdouble >* get_select_color();

		void set_height(GLdouble height);
		void add_height(GLdouble height);
		GLdouble get_height();

		GLdouble get_slope();
		bool is_pathable();

		static std::vector< Hexagon* >* find_path(Hexagon*, Hexagon*);

		void set_neighbor(const char* position, Hexagon* neighbor_hex);
		Hexagon* get_neighbor(const char* position);

		void render_for_select(GLdouble, GLdouble);

		void generate_vertex_data(GLdouble, GLdouble, UniqueDataVector< GLdouble >*);
		void generate_select_data(GLdouble, GLdouble, UniqueDataVector< GLdouble >*);
		//void generate_object_data(GLdouble, GLdouble, UniqueDataVector< GLdouble >*);

		BoardObject* get_board_object();
};

#endif

