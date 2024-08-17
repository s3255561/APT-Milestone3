#include "AIPlayer.h"
#include <iostream>
#include "Rules.h"
#include <random>
#include "Config.h"
#include "QwirkleUtils.h"

// Constructor
AIPlayer::AIPlayer(const std::string& name) : Player(name) {}

// Destructor
AIPlayer::~AIPlayer() {}

// Override makeMove method for AI logic
void AIPlayer::makeMove(GameBoard* gameBoard, TileBag* tileBag, std::vector<Player*>& players, bool &quit) {    

    bool isFirstMove = gameBoard->isEmpty();
    std::default_random_engine engine(Config::randSeed);

    Tile* bestTile = nullptr;
    int bestRow = -1;
    int bestCol = -1;
    int highestScore = 0;

    if (isFirstMove) {
        // Make a random first move
        std::uniform_int_distribution<int> rowDist(0, gameBoard->getRows() - 1);
        std::uniform_int_distribution<int> colDist(0, gameBoard->getCols() - 1);
        bestTile = getHand()->getHead()->getTile();
        bestRow = rowDist(engine);
        bestCol = colDist(engine);
        highestScore = Rules::calculateScore(gameBoard, bestRow, bestCol);
    } else {
        // Evaluate all tiles in AI's hand for the best move
        for (Node* node = getHand()->getHead(); node != nullptr; node = node->getNext()) {
            Tile* currentTile = node->getTile();
            
            // Evaluate all possible placements on the board
            for (int row = 0; row < gameBoard->getRows(); ++row) {
                for (int col = 0; col < gameBoard->getCols(); ++col) {
                    // Check if the position is empty
                    if (gameBoard->getTile(row, col) == nullptr) {  
                        if (Rules::validateMove(gameBoard, currentTile, row, col)) {
                            int score = Rules::calculateScore(gameBoard, row, col);

                            // Choose the move with the highest score
                            if (score > highestScore) {
                                highestScore = score;
                                bestTile = currentTile;
                                bestRow = row;
                                bestCol = col;
                            }
                        }
                    }
                }
            }
        }
    }

    if (bestTile != nullptr) {
        // Place the best tile found
        gameBoard->placeTile(bestRow, bestCol, bestTile);
        removeTileFromHand(bestTile);

        // Format the move output
        char rowChar = 'A' + bestRow;
        std::cout << "place " << bestTile->print() 
                  << " at " << rowChar << bestCol << std::endl;
        std::cout << std::endl;
        
        // Check and print QWIRKLE message
        if (highestScore >= 12) { 
            std::cout << "QWIRKLE!!!" << std::endl;
            std::cout << std::endl;
        }

        // Update the AI player's score
        setScore(getScore() + highestScore);

        // Draw a new tile
        Tile* newTile = tileBag->drawTile();
        if (newTile != nullptr) {
            addTileToHand(newTile);
        }
    } else {
        // No valid moves, so the AI must replace a tile
        Tile* tileToReplace = getHand()->getHead()->getTile();  

        // Remove the tile from hand
        Tile* removedTile = removeTileFromHand(tileToReplace);
        tileBag->addTile(removedTile);
        delete removedTile;

        // Draw a new tile
        Tile* newTile = tileBag->drawTile();
        if (newTile != nullptr) {
            addTileToHand(newTile);
        }
    }
}