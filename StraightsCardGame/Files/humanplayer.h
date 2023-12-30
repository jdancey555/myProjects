#ifndef _HUMANPLAYER_H_
#define _HUMANPLAYER_H_
#include <string>
#include <vector>
#include "player.h"
#include "computerplayer.h"

class HumanPlayer : public Player {
	public:
		HumanPlayer(vector <shared_ptr<Cards>>, int);
		HumanPlayer();
		HumanPlayer( vector <shared_ptr<Cards>>, int, int, int);
		~HumanPlayer();
		shared_ptr<Cards> play(string);
		char getPlayerType();
		shared_ptr<ComputerPlayer> ragequit();
};

#endif
