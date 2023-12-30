#include "deck.h"
#include "cards.h"
using namespace std;
#include <iostream>
#include <string>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <random>
#include <memory>
#include <iostream>
Deck::Deck() {
};

Deck::Deck(unsigned seed) {
	this->seed = seed;
	char suit[4] = {'C', 'D', 'H', 'S'};
	char rank[13] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 13; j++) {
			deck.emplace_back(make_shared<Cards>(suit[i], rank[j]));
		}
	}
}

Deck::~Deck() {
	deck.clear();
}

void Deck::shuffleDeck() {
	default_random_engine rng{seed};
	shuffle(deck.begin(), deck.end(), rng );
}

vector <shared_ptr<Cards>> Deck::get_deck() const {
	return deck;
}
