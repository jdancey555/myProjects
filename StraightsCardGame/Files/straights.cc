#include <iostream>
#include <random>
#include <vector>
#include <sstream>
#include "player.h"
#include "deck.h"
#include "cards.h"
#include "gamecontroller.h"
#include "computerplayer.h"
#include "humanplayer.h"
using namespace std;

int main(int argc, char** argv) {
	char x;
	vector <shared_ptr <Player>> arr;
	unsigned seed = 0;

if ( argc > 1 ) {
                try {
                        seed = std::stoi( std::string{ argv[1] } );
                } catch( std::invalid_argument & e ) {
                        std::cerr << e.what() << std::endl;
                        return 1;
                } catch( std::out_of_range & e ) {
                        std::cerr << e.what() << std::endl;
                        return -1;
                }
} 
	Deck deck = Deck(seed);
	vector <shared_ptr<Cards>> decks[4];

	for (int i = 0; i < 4; i++) {
		cout << "Is Player " << i + 1 << " a human (h) or a computer (c)?" << endl;
		
		cin >> x;
		if (x == 'h') {
			arr.emplace_back(make_shared<HumanPlayer>(decks[i], i+1));
		} else if (x == 'c') {
			arr.emplace_back(make_shared<ComputerPlayer>(decks[i], i+1));
		} else {
			cout << "Invalid input. Try again" << endl;
			i--;
		}
	}
	GameController game = GameController(arr, deck);
	game.playGame();
}
