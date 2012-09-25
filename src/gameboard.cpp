#include "includes.h"

Gameboard::Gameboard() {
	std::cout << "yo!" << std::endl;
}

GameboardChunk* Gameboard::at(Hexagon* hex) {
	Gameboard &self = *this;
	return self[hex];
}


