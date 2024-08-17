#include "HumanPlayer.h"
#include <iostream>
#include <sstream>
#include "Rules.h"
#include "FileHandler.h"
#include "InputValidator.h"
#include "QwirkleUtils.h"

// Constructor
HumanPlayer::HumanPlayer(const std::string& name) : Player(name) {}

// Destructor
HumanPlayer::~HumanPlayer() {}

// Override makeMove method for human interaction
void HumanPlayer::makeMove(GameBoard* gameBoard, TileBag* tileBag, std::vector<Player*>& players, bool &quit) 
{
    bool turnComplete = false;

    while (!turnComplete && !quit) {

        std::string playerMove = QwirkleUtils::handleInput(quit);

        if (quit || playerMove == "quit") {
            quit = true;
        } else if (playerMove == "save") {
            std::cout << "Enter filename to save: ";
            std::string filename = QwirkleUtils::handleInput(quit);
            FileHandler fileHandler;
            fileHandler.saveGame(filename, players, tileBag, gameBoard, this);
            std::cout << "Game saved to " << filename << std::endl;
            QwirkleUtils::displayGameState(players, gameBoard, this);

        } else if (playerMove.substr(0, 7) == "replace") {
            std::string tileToReplace = playerMove.substr(8);
            if (tileToReplace.size() == 2) {
                char colourChar = tileToReplace[0];
                int shape = tileToReplace[1] - '0';
                Colour colour = ColourUtils::charToColour(colourChar);
                Tile* tile = new Tile(colour, shape);

                Tile* removedTile = removeTileFromHand(tile);
                if (removedTile != nullptr) {
                    tileBag->addTile(tile);

                    Tile* newTile = tileBag->drawTile();
                    if (newTile != nullptr) {
                        addTileToHand(newTile);
                        std::cout << newTile->print() << " tile drawn and added to your hand." << std::endl;
                    } else {
                        std::cout << "No tiles left to draw from the tile bag." << std::endl;
                    }
                    turnComplete = true;
                } else {
                    std::cout << "You don't have that tile in your hand." << std::endl;
                }
            } else {
                std::cout << "Invalid tile format. Use <colour><shape>." << std::endl;
            }
        } else if (playerMove == "pass") {
            std::cout << getName() << " has passed their turn." << std::endl;
            turnComplete = true;
        } else if (playerMove.substr(0, 5) == "place") {
            std::stringstream stringstream(playerMove);
            std::string extractedWord;
            std::vector<std::string> moveBreakdown;

            while (stringstream >> extractedWord) {
                moveBreakdown.push_back(extractedWord);
            }

            if (moveBreakdown.size() == 4 && moveBreakdown[2] == "at") {
                char tileColourChar = moveBreakdown[1][0];
                int tileShape = moveBreakdown[1][1] - '0';
                char rowChar = moveBreakdown[3][0];
                int col = std::stoi(moveBreakdown[3].substr(1));

                int row = rowChar - 'A';

                Colour tileColour = ColourUtils::charToColour(tileColourChar);
                Tile* tile = new Tile(tileColour, tileShape);
                if (containsTile(tile)) {
                    if (Rules::validateMove(gameBoard, tile, row, col)) {
                        gameBoard->placeTile(row, col, tile);

                        Tile* removedTile = removeTileFromHand(tile);
                        if (removedTile != nullptr) {
                            delete removedTile;

                            Tile* newTile = tileBag->drawTile();
                            if (newTile != nullptr) {
                                addTileToHand(newTile);
                            }

                            int score = Rules::calculateScore(gameBoard, row, col);
                            setScore(getScore() + score);

                            if (score >= 12) {
                                std::cout << "QWIRKLE!!!" << std::endl;
                                std::cout << std::endl;
                            }
                            turnComplete = true;
                        } else {
                            std::cout << "Error: Failed to remove tile from hand." << std::endl;
                        }
                    } else {
                        std::cout << "Invalid move. Try again." << std::endl;
                        delete tile;
                    }
                } else {
                    std::cout << "You don't have that tile in your hand." << std::endl;
                    delete tile;
                }
            } else {
                std::cout << "Invalid move format. Use 'place <tile> at <position>'." << std::endl;
            }
        } else {
            std::cout << "Invalid command. Try again." << std::endl;
        }
    }
}