#ifndef GAMEBOARD_H
#define GAMEBOARD_H


class Gameboard {
	private:
		RoundVector< RoundVector< Hexagon* >* >* hexagon_list;
		std::map< Hexagon*, GameboardChunk* >* chunk_map;

		GameboardChunk* generate_chunk(Hexagon*);

	public:
		Gameboard();

		void push_back(Hexagon*);
		int count(Hexagon*);

		RoundVector< RoundVector< Hexagon* >* >* get_hexagon_list();

		GameboardChunk* get_render_data(Hexagon* base_hex);

		void clear();
};


#endif
