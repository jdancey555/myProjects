#ifndef ROUNDCONTROLLER_H
#define ROUNDCONTROLLER_H

#include <vector>
#include "cards.h"
#include "player.h"
#include "deck.h"

class RoundController {
	private:
		shared_ptr <Cards> played_card;
		shared_ptr<Player> curr_player;
		int turns;
		Deck deck;
		bool exit;
		vector <shared_ptr<Player>> players;
		vector <shared_ptr<Cards>> cards_on_table[4];
	public:
		RoundController(vector <shared_ptr<Player>>, Deck);
		~RoundController();
		RoundController();
		bool get_exit();
		void endRound();
		void takeTurn();
		vector <shared_ptr<Cards>>* getCardsOnTable();
		shared_ptr<Cards> getRecentCard();
		shared_ptr<Player> find7S(vector <shared_ptr<Player>>);
		void startRound();
		int getTotalPoints(shared_ptr<Player>);
};

#endif
