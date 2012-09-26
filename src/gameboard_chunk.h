#ifndef GAMEBOARD_CHUNK_H
#define GAMEBOARD_CHUNK_H

class GameboardChunk {
	public:
		GLuint vbo_id;
		GLuint vbo_color;
		GLuint vbo_indicie;

		UniqueDataVector< GLfloat >* board_vertex_data;

		GameboardChunk();

		void generate_render_data(Hexagon*, double, double);

};

#endif

