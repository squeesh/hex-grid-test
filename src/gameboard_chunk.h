#ifndef GAMEBOARD_CHUNK_H
#define GAMEBOARD_CHUNK_H



class GameboardChunk {
	private:
		Hexagon* base_hex;

		void clear_terrain();
		void clear_select();

		void generate_chunk_data();
		void generate_render_data(Hexagon*, double, double);

		void write_VBO_data();

	public:
		GLuint vbo_terrain_vert;
		GLuint vbo_terrain_indicie;

		GLuint vbo_select_vert;
		GLuint vbo_select_indicie;

		UniqueDataVector< GLfloat >* board_terrain_data;
		UniqueDataVector< GLfloat >* board_select_data;

		bool regenerate_terrain;
		bool regenerate_select;

		GameboardChunk(Hexagon*);
		~GameboardChunk();

		void verify_render_data();
};

#endif

