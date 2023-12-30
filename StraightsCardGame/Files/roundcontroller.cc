#include "roundcontroller.h"
#include "player.h"
#include "cards.h"
#include "deck.h"
#include "humanplayer.h"
#include "computerplayer.h"
#include <string>
#include <sstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

RoundController::RoundController() {

}

RoundController::RoundController(vector <shared_ptr<Player>> players, Deck deck) {
	turns = 0;
	this->deck = deck;
	this->players = players;
	vector <shared_ptr<Cards>> clubs;
	vector <shared_ptr<Cards>> diamonds;
	vector <shared_ptr<Cards>> hearts;
	vector <shared_ptr<Cards>> spades;
	cards_on_table[0] = clubs;
	cards_on_table[1] = diamonds;
	cards_on_table[2] = hearts;
	cards_on_table[3] = spades;
	exit = false;
}

RoundController::~RoundController() {
	players.clear();
}

shared_ptr<Player> RoundController::find7S(vector <shared_ptr<Player>> players) {
	for (int i = 0; i < 4; ++i) {
		for (size_t j = 0; j < players[i]->get_hand().size(); ++j) {
			if (players[i]->get_hand()[j]->getcardname() == " 7S") {
				return players[i];
			}
		}
	}
	return players[0];
}

bool RoundController::get_exit() {
	return exit;
}

vector <shared_ptr<Cards>>* RoundController::getCardsOnTable() {
	return cards_on_table;
}

shared_ptr<Cards> RoundController::getRecentCard() {
	return played_card;
}

void RoundController::endRound() {
	vector <string> temp;
	if (exit == false) {
		for (int i = 0; i < 4; i++) {
			cout << "Player" << players[i]->getPlayerID() << "'s discards:";
			for (size_t j = 0; j < players[i]->get_discards().size(); j++) {
				cout << players[i]->get_discards()[j];
			}
			cout << endl;
			cout << "Player" << players[i]->getPlayerID() << "'s score: " << players[i]->getTotalPoints() << " + " << players[i]->getCurrPoints() << " = ";
			players[i]->addTotalPoints(players[i]->getCurrPoints());	
			cout << players[i]->getTotalPoints() << endl;
			players[i]->setCurrPoints(0);
			players[i]->set_discards(temp);	
		}
	}
}

void RoundController::takeTurn() {
	bool ragequit;
	bool bad_read;
	string temp_card;
        string command;
        string card;
	shared_ptr<HumanPlayer> H;
	shared_ptr<ComputerPlayer> C;
	if (curr_player->getPlayerType() == 'C') {
                C = make_shared<ComputerPlayer>(curr_player->get_hand(), curr_player->getPlayerID(), curr_player->getCurrPoints(), curr_player->getTotalPoints());;
		C->set_discards(curr_player->get_discards());
		C->play(cards_on_table);
		players[C->getPlayerID() - 1]->setCurrPoints(C->getCurrPoints());
                players[C->getPlayerID() - 1]->set_hand(C->get_hand());
		players[C->getPlayerID() - 1]->set_discards(C->get_discards());
	} else {
		ragequit = false;
		H = make_shared<HumanPlayer>(curr_player->get_hand(), curr_player->getPlayerID(), curr_player->getCurrPoints(), curr_player->getTotalPoints());
                H->set_discards(curr_player->get_discards());
		vector <shared_ptr<Cards>> legalplays;
		for (size_t i = 0; i < H->get_hand().size(); i++) {
			if (cards_on_table[3].size() == 0 && H->get_hand()[i]->getcardname() == " 7S") {
				legalplays.clear();
				legalplays.emplace_back(H->get_hand()[i]);
				break;
			}
			if (H->isLegalPlay(cards_on_table, H->get_hand()[i])) {
				legalplays.emplace_back(H->get_hand()[i]);

	
			}
		}
		cout << "Your hand:" << H->get_hand() << endl;
		cout << "Legal plays:" << legalplays << endl;
		while (true) {
			bad_read = false;
			cin >> command;
			if (command == "deck") {
				for (size_t i = 0; i < 4; i++) {
					for (int j = 0; j < 12; j++) {
						temp_card = deck.get_deck()[(i*13) + j]->getcardname();
						temp_card.erase(temp_card.begin());
						cout << temp_card << " ";
					}
					temp_card = deck.get_deck()[(i*13) + 12]->getcardname();
					temp_card.erase(temp_card.begin());
					cout << temp_card << endl;
				}
			} else if (command == "play") {
				cin >> card;
				card = " " + card;
				for (size_t i = 0; i < legalplays.size(); i++) {
					if (card == legalplays[i]->getcardname()) {
						break;
					} else if (i == legalplays.size() - 1) {
						bad_read = true;
					}
				}
				if (bad_read == true) {
					cout << "This is not a legal play." << endl;
					continue;
				}

			
				played_card = H->play(card);
               
				if (played_card->getsuit() == 'C') {
					if (played_card->getrank() <= 54 || played_card->getrank() == 65) {
						cards_on_table[0].insert(cards_on_table[0].begin(), make_shared<Cards>(played_card->getsuit(), played_card->getrank()));
					} else {
						cards_on_table[0].emplace_back(make_shared<Cards>(played_card->getsuit(), played_card->getrank()));
					}
				} else if (played_card->getsuit() == 'D') {
					if (played_card->getrank() <= 54 || played_card->getrank() == 65) {
						cards_on_table[1].insert(cards_on_table[1].begin(), make_shared<Cards>(played_card->getsuit(), played_card->getrank()));
					} else {
						cards_on_table[1].emplace_back(make_shared<Cards>(played_card->getsuit(), played_card->getrank()));
					}
				} else if (played_card->getsuit() == 'H') {
					if (played_card->getrank() <= 54 || played_card->getrank() == 65) {
						cards_on_table[2].insert(cards_on_table[2].begin(), make_shared<Cards>(played_card->getsuit(), played_card->getrank()));
					} else {
						cards_on_table[2].emplace_back(make_shared<Cards>(played_card->getsuit(), played_card->getrank()));
					}
				} else {
					if (played_card->getrank() <= 54 || played_card->getrank() == 65) {
						cards_on_table[3].insert(cards_on_table[3].begin(), make_shared<Cards>(played_card->getsuit(), played_card->getrank()));
					} else {
						cards_on_table[3].emplace_back(make_shared<Cards>(played_card->getsuit(), played_card->getrank()));
					}
				}
				cout << "Player" << H->getPlayerID() << " plays" << played_card->getcardname() << "." << endl;
				break;
			} else if (command == "discard") {
				cin >> card;
				card = " " + card;
				if (legalplays.size() > 0) {
					cout << "You have a legal play. You may not discard." << endl;
					bad_read = true;
					continue;
				}

				played_card = H->discard(card);
				players[H->getPlayerID() - 1]->add_discards(played_card->getcardname());
				break;
			} else if (command == "ragequit") {
				C = H->ragequit();
				ragequit = true;
				curr_player = C;
				cout << "Player" << curr_player->getPlayerID() << " ragequits. A computer will now take over." << endl;
				players[C->getPlayerID() - 1]->setCurrPoints(C->getCurrPoints());
				players[C->getPlayerID() - 1]->set_hand(C->get_hand());
				players[C->getPlayerID() - 1]->setPlayerType('C');
				takeTurn();
				break;
			} else if (command == "quit") {
				exit = true;
				break;				
			} else if (bad_read == false) {
				cout << "Invalid command. Try again" << endl;
			}
		}
		if (ragequit == false) {
			players[H->getPlayerID() - 1]->setCurrPoints(H->getCurrPoints());
			players[H->getPlayerID() - 1]->set_hand(H->get_hand());
		}
	}

}

void RoundController::startRound() {
	vector <shared_ptr<Cards>> decks[4];
	for (int j = 0; j < 4; j++) {
                for (int i = 0; i < 13; i++) {
                        decks[j].emplace_back(make_shared<Cards>(deck.get_deck()[(j*13)+i]->getsuit(), deck.get_deck()[(j*13)+i]->getrank()));
                }
		players[j]->set_hand(decks[j]);
        }
	curr_player = find7S(players);
	cout << "A new round begins. It's Player" << curr_player->getPlayerID() << "'s turn to play." << endl;

	while (turns <  52) {
		cout << "Cards on the table:" << endl;

		for (int i = 0; i < 4; i++) {
			
			if (i == 0) {
				cout << "Clubs:";
			} else if (i == 1) {
				cout << "Diamonds:";
			} else if (i == 2) {
				cout << "Hearts:";
			} else {
				cout << "Spades:";
			}

			for (size_t j = 0; j < cards_on_table[i].size(); j++) {
                                        cout << " " << cards_on_table[i][j]->getrank();
                                }
                                cout << endl;
		}
		
		takeTurn();
		if (exit == true) {
			break;
		}
		turns++;
		if (curr_player->getPlayerID() == 1) {
			curr_player = players[1];
		} else if (curr_player->getPlayerID() == 2) {
			curr_player = players[2];
		} else if (curr_player->getPlayerID() == 3) {
			curr_player = players[3];
		} else {
			curr_player = players[0];
		}
	}
	endRound();
}
