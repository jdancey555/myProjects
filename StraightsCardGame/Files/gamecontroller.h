#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H
#include "player.h"
#include "deck.h"
#include "roundcontroller.h"

class GameController {
	private:
		vector <shared_ptr<Player>> players;
		Deck deck;
		bool game_over;
		bool quit;
	public:
		GameController(vector <shared_ptr<Player>>, Deck&);
		GameController();
		~GameController();
		void reset();
		void playGame();
		void endGame();
		void playRound();
		void declareWinner(shared_ptr<Player>);

};

#endif
