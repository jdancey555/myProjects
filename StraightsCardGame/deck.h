#ifndef DECK_H
#define DECK_H
#include <vector>
#include "cards.h"
#include <memory>
class Deck {
	
	private:
		vector <shared_ptr<Cards>> deck;
		unsigned seed;
	public:
		Deck(unsigned);
		Deck();
		~Deck();
		void shuffleDeck();
		vector <shared_ptr<Cards>> get_deck() const;
};

#endif
