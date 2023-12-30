#include "humanplayer.h"
#include "computerplayer.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

HumanPlayer::HumanPlayer(){};

HumanPlayer::HumanPlayer(vector <shared_ptr<Cards>> hand, int player_num) :
	Player{hand, player_num, 0, 0, 'H'} {}

HumanPlayer::HumanPlayer(vector <shared_ptr<Cards>> hand, int player_num, int curr_points, int total_points) :
	Player{hand, player_num, curr_points, total_points, 'H'} {}

HumanPlayer::~HumanPlayer() {
	vector <shared_ptr<Cards>> temp = get_hand();
	temp.clear();
	set_hand(temp);
}

shared_ptr<Cards> HumanPlayer::play(string card) {
	int index = 0;
	shared_ptr<Cards> temp;
	vector <shared_ptr <Cards>> temp_hand;
	for (size_t i = 0; i < get_hand().size(); i++) {
		if (get_hand()[i]->getcardname() == card) {
			temp = get_hand()[i];
			temp_hand = get_hand();
			temp_hand.erase(temp_hand.begin() + index);
			set_hand(temp_hand);
			return temp;
		}
	index++;
	}
return temp;
}

char HumanPlayer::getPlayerType() {
	return 'H';
}

shared_ptr<ComputerPlayer> HumanPlayer::ragequit() {
	shared_ptr<ComputerPlayer> new_player = make_shared<ComputerPlayer>(get_hand(), getPlayerID(),  getCurrPoints(), getTotalPoints());
	return new_player;
}
