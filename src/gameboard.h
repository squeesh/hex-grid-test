#ifndef GAMEBOARD_H
#define GAMEBOARD_H

class GameboardRenderable: public Renderable {
	private:
		//std::map< Hexagon*, GameboardChunk* >* chunk_map;

	public:
		GameboardRenderable() : Renderable(false) {
			std::cout << "GameboardRenderable..." << std::endl;
			this->regist();
		}
		virtual void regist();
		//void render();
	
};


class Gameboard: public GameboardRenderable {
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

		void bind_obj_hex(BoardObject*, Hexagon*);
};


#endif
