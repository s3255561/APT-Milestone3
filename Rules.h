#ifndef ASSIGN2_RULES_H
#define ASSIGN2_RULES_H

#include "GameBoard.h"
#include "Player.h"
#include "TileBag.h"

class Rules {
public:
    // Validate a move
    static bool validateMove(GameBoard* board, Tile* tile, int x, int y);

    // Calculate the score of a move
    static int calculateScore(GameBoard* board, int x, int y);

    // Check if the game is over
    static bool isGameOver(const std::vector<Player*>& players, TileBag* tileBag, GameBoard* board);

private:
    // Helper functions for move validation and scoring
    static bool isValidPlacement(GameBoard* board, Tile* tile, int x, int y);
    
    // Check if tile placement is valid in rows
    static bool isRowInvalid(GameBoard* board, Tile* tile, int x, int y);

    // Check to confirm if a player has a vaid move available to them
    static bool playerHasValidMove(Player* player, GameBoard* board);
};

#endif // ASSIGN2_RULES_H
