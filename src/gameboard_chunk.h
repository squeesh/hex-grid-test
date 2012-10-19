#ifndef GAMEBOARD_CHUNK_H
#define GAMEBOARD_CHUNK_H



class GameboardChunk: public Renderable {
	private:
		Hexagon* base_hex;

		/*GLuint vbo_terrain_vert;
		GLuint vbo_terrain_indicie;

		GLuint vbo_select_vert;
		GLuint vbo_select_indicie;*/

		UniqueDataVector< GLfloat >* board_terrain_data;
		UniqueDataVector< GLfloat >* board_select_data;
		UniqueDataVector< GLfloat >* board_object_data;

		void clear_terrain();
		void clear_select();
		void clear_object();

		void generate_chunk_data();
		void generate_render_data(Hexagon*, double, double);

		void write_VBO_data();

	public:
		bool regenerate_terrain;
		bool regenerate_select;
		bool regenerate_object;

		GameboardChunk(Hexagon*);
		~GameboardChunk();

		void verify_render_data();

		void render();
};

#endif

