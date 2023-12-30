#include "gamecontroller.h"
#include "roundcontroller.h"
#include <iostream>


GameController::GameController(vector <shared_ptr<Player>> players, Deck &deck) 
	:deck{deck}, game_over{false} {
	this->players = players;
	quit = false;
	}

GameController::~GameController() {
}

void GameController::playRound() {
	deck.shuffleDeck();
	RoundController round = RoundController(players, deck);
	round.startRound();
	if (round.get_exit() == true) {
		game_over = true;
		quit = true;
	}	
}

void GameController::playGame() {
	int winner_index;
	while (!game_over) {
		playRound();
		for (int i = 0; i < 4; i++) {
			if (players[i]->getTotalPoints() >= 80) {
				game_over = true;
				break;
			}
		}
	}
	if (!quit) {
		for (int i = 0; i < 4; i++) {
			if (i == 0) {
				winner_index = 0;
			} else {
				if (players[i]->getTotalPoints() < players[winner_index]->getTotalPoints()) {
					winner_index = i;
				}
			}
		}
		declareWinner(players[winner_index]);
	}
}

void GameController::declareWinner(shared_ptr <Player> player) {
	cout << "Player" << player->getPlayerID() << " wins!" << endl;
}
