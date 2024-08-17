#ifndef ASSIGN2_AIPLAYER_H
#define ASSIGN2_AIPLAYER_H

#include "Player.h"

class AIPlayer : public Player {
public:
    // Constructor
    AIPlayer(const std::string& name);

    // Destructor
    ~AIPlayer() override;

    // Override makeMove method
    void makeMove(GameBoard* gameBoard, TileBag* tileBag, std::vector<Player*>& players, bool &quit) override;
};

#endif // ASSIGN2_AIPLAYER_H