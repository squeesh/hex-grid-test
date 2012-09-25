#ifndef GAMEBOARD_H
#define GAMEBOARD_H

class GameboardChunk {
	//private:

};


class Gameboard : public std::map< Hexagon*, GameboardChunk* > {
	public:
		Gameboard();
		GameboardChunk* at(Hexagon*);
};


#endif
