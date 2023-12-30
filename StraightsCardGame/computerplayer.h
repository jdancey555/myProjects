#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H
#include "player.h"
#include "cards.h"
using namespace std;
class ComputerPlayer : public Player {
	public:
		ComputerPlayer();
		ComputerPlayer(vector <shared_ptr<Cards>>, int);
		ComputerPlayer(vector <shared_ptr<Cards>>, int, int, int);
		~ComputerPlayer();
		void play(vector <shared_ptr<Cards>>*);
		char getPlayerType();
};

#endif
