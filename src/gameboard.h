#ifndef GAMEBOARD_H
#define GAMEBOARD_H


class Gameboard {
	private:
		RoundVector< RoundVector< Hexagon* >* >* hexagon_list;
		std::map< Hexagon*, GameboardChunk* >* chunk_map;

	public:
		std::vector< BoardObject* >* board_objects;

		Gameboard();

		void push_back(Hexagon*);
		int count(Hexagon*);

		RoundVector< RoundVector< Hexagon* >* >* get_hexagon_list();

		GameboardChunk* get_render_data(Hexagon* base_hex);
		void render(int, int, int, int);

		void clear();
};


#endif
