#ifndef GAMEBOARD_H
#define GAMEBOARD_H

class GameboardBase {
	protected:
		RoundVector< RoundVector< Hexagon* >* >* hexagon_list;

	public:
		std::map< Hexagon*, BoardObject* >* board_object_map;

		GameboardBase();

		void push_back(Hexagon*);
		RoundVector< RoundVector< Hexagon* >* >* get_hexagon_list();
		void bind_obj_hex(BoardObject*, Hexagon*);
};

class Gameboard: public GameboardBase, public Renderable {
	private:
		std::map< Hexagon*, GameboardChunk* >* chunk_map;

	public:
		Gameboard();

		GameboardChunk* get_chunk(Hexagon*);
		virtual void render(int, int, int, int);
};


#endif
