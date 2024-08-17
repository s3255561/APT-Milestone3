#ifndef ASSIGN2_HUMANPLAYER_H
#define ASSIGN2_HUMANPLAYER_H

#include "Player.h"

class HumanPlayer : public Player 
{
    public:
        // Constructor
        HumanPlayer(const std::string& name);

        // Destructor
        ~HumanPlayer() override;

        // Override makeMove method
        void makeMove(GameBoard* gameBoard, TileBag* tileBag, std::vector<Player*>& players, bool &quit) override;
};

#endif // ASSIGN2_HUMANPLAYER_H
