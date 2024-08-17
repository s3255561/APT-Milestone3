#ifndef ASSIGN2_QWIRKLEUTILS_H
#define ASSIGN2_QWIRKLEUTILS_H

#include <vector>
#include "Player.h"
#include "GameBoard.h"
#include "TileBag.h"

namespace QwirkleUtils 
{
    // Function declarations
    void gameLoop(std::vector<Player*>& players, TileBag* tileBag, GameBoard* gameBoard, Player* currentPlayer);
    void displayGameState(std::vector<Player*>& players, GameBoard* gameBoard, Player* currentPlayer);
    std::string handleInput(bool &quit);
    void printScores(std::vector<Player*>& players);
    Player* determineWinner(std::vector<Player*>& players);
    void loadGame(bool &quit);
    void startNewGame(bool &quit);
    bool addHumanPlayer(std::vector<Player*>& players, int playerIndex, bool &quit);

}

#endif // ASSIGN2_QWIRKLEUTILS_H