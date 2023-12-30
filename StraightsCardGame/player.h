#ifndef PLAYER_H
#define PLAYER_H
#include <vector>
#include <memory>
#include "cards.h"
#include <string>
class Player {
	
	private:
		int player_num;
		vector <shared_ptr<Cards>> hand;
		int curr_points;
		int total_points;
		char player_type;
		vector <string> discards;
	public:
		Player();
		Player(vector <shared_ptr<Cards>>, int, int, int, char);
		~Player();
		shared_ptr<Cards> discard(string);
		vector <shared_ptr<Cards>> get_hand();
		void set_hand(vector <shared_ptr<Cards>>);
		bool isLegalPlay(vector <shared_ptr<Cards>>*, shared_ptr<Cards>);
		char getPlayerType();
		void setPlayerType(char);
		vector <string> get_discards();
		void add_discards(string);
		void set_discards(vector <string>);
		int getPlayerID();
		int getCurrPoints();
		void setCurrPoints(int);
		int getTotalPoints();
		void addTotalPoints(int);
};

#endif
