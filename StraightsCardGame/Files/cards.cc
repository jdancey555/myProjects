#include "cards.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

Cards::Cards() {
}

Cards::Cards(char suit, char rank) 
	: suit{suit}, rank{rank} {
	card_name = " ";
	card_name.push_back(rank);
	card_name.push_back(suit);
}

Cards::~Cards() {
}

char Cards::getsuit() {
	return this->suit;
}

char Cards::getrank() {
	return this->rank;
}

string Cards::getcardname() const {
	return card_name;
}

ostream & operator<<(ostream & out, const vector <shared_ptr<Cards>> & deck) {
	for (size_t i = 0; i < deck.size(); i++) {
		out << deck[i]->getcardname();
	}
return out;
}
