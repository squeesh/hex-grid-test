#ifndef GAMEBOARD_CHUNK_H
#define GAMEBOARD_CHUNK_H



class GameboardChunk {
	public:
		class GenerationException : public std::exception {};

		GLuint vbo_hex_vert;
		GLuint vbo_hex_color;
		GLuint vbo_hex_indicie;

		GLuint vbo_sel_vert;
		GLuint vbo_sel_color;
		GLuint vbo_sel_indicie;

		UniqueDataVector< GLfloat >* board_vertex_data;
		UniqueDataVector< GLfloat >* board_select_data;

		bool regenerate;

		GameboardChunk();
		~GameboardChunk();

		void clear();
		void generate_render_data(Hexagon*, double, double);

		void write_VBO_data();

};

#endif

