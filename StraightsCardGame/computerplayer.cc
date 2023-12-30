#include "computerplayer.h"
#include "player.h"
#include <iostream>

ComputerPlayer::ComputerPlayer() {};

ComputerPlayer::ComputerPlayer(vector <shared_ptr<Cards>> deck, int player_num): Player{deck, player_num, 0, 0, 'C'} {};

ComputerPlayer::ComputerPlayer(vector <shared_ptr<Cards>> hand, int player_num, int curr_points, int total_points) : Player{hand, player_num, curr_points, total_points, 'C'} {}

ComputerPlayer::~ComputerPlayer() {
	vector <shared_ptr<Cards>> temp = get_hand();
	temp.clear();
	set_hand(temp);
}

void ComputerPlayer::play(vector <shared_ptr<Cards>> *cards_on_table) {
	vector <shared_ptr<Cards>> legalplays;
	int index = 0;
	bool d_card = false;
	shared_ptr<Cards> played_card;
	for (size_t j = 0; j < get_hand().size(); j++) {
		if (cards_on_table[3].size() == 0 && get_hand()[j]->getcardname() == " 7S") {
			legalplays.clear();
			legalplays.emplace_back(make_shared<Cards>(get_hand()[j]->getsuit(), get_hand()[j]->getrank()));
			break;
		} else if (isLegalPlay(cards_on_table, get_hand()[j])) {
			legalplays.emplace_back(make_shared<Cards>(get_hand()[j]->getsuit(), get_hand()[j]->getrank()));
		}
		if (j == get_hand().size() - 1) {
			cout << "Your hand:" << get_hand() << endl;
        		cout << "Legal plays: " << legalplays << endl;
                       	if (legalplays.size() > 0) {
				break;
			}
		       	discard(get_hand()[0]->getcardname());
                	d_card = true;
		}
	}

	if (!d_card) {
		played_card = legalplays[0];
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
		for (size_t i = 0; i < get_hand().size(); i++) {
			if (get_hand()[i]->getcardname() == played_card->getcardname()) {
				cout << "Player" << getPlayerID() << " plays" << played_card->getcardname() << "." << endl;
				vector <shared_ptr<Cards>> temp = get_hand();
				temp.erase(temp.begin() + index);
				set_hand(temp);
				temp.clear();
				break;
			}
			index++;
		}
	}
	legalplays.clear();
}
char ComputerPlayer::getPlayerType() {
	return 'C';
}
