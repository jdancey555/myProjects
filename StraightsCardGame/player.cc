#include "player.h"
#include "cards.h"
#include <iostream>
#include <string>
using namespace std;

Player::Player() {}

Player::Player(vector <shared_ptr<Cards>> hand, int player_num, int curr_points, int total_points, char player_type)
	: player_num{player_num}, hand{hand}, curr_points{curr_points}, total_points{total_points}, player_type{player_type} {}

Player::~Player() {
	hand.clear();
}

vector <string> Player::get_discards() {
	return discards;
}

void Player::add_discards(string card) {
	discards.emplace_back(card);
}

void Player::set_discards(vector <string> cards) {
	discards.clear();
	discards = cards;
}

vector <shared_ptr<Cards>> Player::get_hand(){
	return hand;
}

void Player::set_hand(vector <shared_ptr<Cards>> hand) {
	this->hand = hand;
}

int Player::getPlayerID() {
	return player_num;
}

void Player::setPlayerType(char type) {
	player_type = type;
}

char Player::getPlayerType() {
	return player_type;
}

int Player::getCurrPoints() {
	return curr_points;
}

void Player::setCurrPoints(int points) {
	curr_points = points;
}

int Player::getTotalPoints() {
	return total_points;
}

void Player::addTotalPoints(int points) {
	total_points += points;
}

bool Player::isLegalPlay(vector <shared_ptr<Cards>> played_cards[4], shared_ptr<Cards> card) {
	for (int j = 0; j < 4; j++) {
			if (card->getrank() == '7' && j == 0) {
				return true;
		
			} else if (played_cards[j].size() > 0) {
			       	if (card->getsuit() == played_cards[j].back()->getsuit()) {
				
					if ((played_cards[j].back()->getrank() != 'J' && played_cards[j].back()->getrank() != 'K') &&
							(card->getrank() + 1 == played_cards[j].back()->getrank() || 
							 card->getrank() - 1 == played_cards[j].back()->getrank())) {
						return true;
					} else if (played_cards[j].back()->getrank() == '9' && card->getrank() == 'T') {
						return true; 
					}  else if (played_cards[j].back()->getrank() == 'T' && 
							(card->getrank() == 'J' || card->getrank() == '9')) {
						return true;
					} else if (played_cards[j].back()->getrank() == 'J' &&
							(card->getrank() == 'T' || card->getrank() == 'Q')) {
						return true;
					} else if (played_cards[j].back()->getrank() == 'Q' &&
							(card->getrank() == 'J' || card->getrank() == 'K')) {
						return true;
					} else if (played_cards[j].back()->getrank() == 'K' && card->getrank() == 'Q') {
						return true;
					}
				if ((card->getsuit() == played_cards[j][0]->getsuit()) && 
                                               (played_cards[j].back()->getcardname() != played_cards[j][0]->getcardname())) {
                                        if (card->getrank() + 1 == played_cards[j][0]->getrank()) {
                                                return true;
                                	} else if (played_cards[j][0]->getrank() == '2' && card->getrank() == 'A') {
                                                 return true;
                                        }
				}	
			}

		}
	}
	return false;
}

shared_ptr<Cards> Player::discard(string cardname) {
	shared_ptr <Cards> temp;
	vector <shared_ptr <Cards>> temp_hand;
	int index = 0;
	add_discards(cardname);
	cout << "Player" << getPlayerID() << " discards" << cardname << endl;
	for (size_t i = 0; i < hand.size(); i++) {
		if (hand[i]->getcardname() == cardname) {
			if (hand[i]->getrank() == 'K') {
				curr_points += 13;
			} else if (hand[i]->getrank() == 'Q') {
				curr_points += 12;
			} else if (hand[i]->getrank() == 'J') {
				curr_points += 11;
			} else if (hand[i]->getrank() == 'A') {
				curr_points += 1;
			} else if (hand[i]->getrank() == 'T') {
				curr_points += 10;
			} else {
				curr_points += (hand[i]->getrank() - 48);
			}
			temp = hand[i];
			temp_hand = get_hand();
			temp_hand.erase(temp_hand.begin() + index);
			set_hand(temp_hand);
			return temp;
		}
	index++;
	}
return temp;
}
