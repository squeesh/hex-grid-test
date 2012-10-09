#ifndef GAMEBOARD_H
#define GAMEBOARD_H


class Gameboard {
	private:
		RoundVector< RoundVector< Hexagon* >* >* hexagon_list;
		std::map< Hexagon*, GameboardChunk* >* chunk_map;

	public:
		std::map< Hexagon*, BoardObject* >* board_object_map;

		Gameboard();

		void push_back(Hexagon*);
		int count(Hexagon*);

		RoundVector< RoundVector< Hexagon* >* >* get_hexagon_list();

		GameboardChunk* get_chunk(Hexagon* base_hex);
		void render(int, int, int, int);

		void clear();

		void bind_obj_hex(BoardObject*, Hexagon*);
};


#endif
