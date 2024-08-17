#ifndef ASSIGN2_PLAYER_H
#define ASSIGN2_PLAYER_H

#include <string>
#include <vector>
#include "LinkedList.h"
#include "TileBag.h"
#include "GameBoard.h"

class Player {
public:
    // Constructor
    Player(const std::string& name);

    // Destructor
    virtual ~Player(); 

    // Copy constructor
    Player(const Player& other);

    // Move constructor
    Player(Player&& other);

    // Copy assignment operator
    Player& operator=(const Player& other);

    // Move assignment operator
    Player& operator=(Player&& other);

    // Getter for player name
    std::string getName() const;

    // Setter for player name
    void setName(const std::string& newName);

    // Getter for player score
    int getScore() const;

    // Setter for player score
    void setScore(int newScore);

    // Add a tile to the player's hand
    void addTileToHand(Tile* tile);

    // Add quantity of tiles to player's hand
    void drawQuantityTiles(TileBag* tileBag, int quantity);

    // Remove a tile from the player's hand
    Tile* removeTileFromHand(Tile* tile);

    // Get the player's hand
    LinkedList* getHand();

    // Setter for player's hand (initialises hand with given tiles)
    void setHand(const std::vector<Tile*>& tiles);

    // Get the string representation of the player's hand
    std::string toString() const;

    // Confirms player has tile in player's hand
    bool containsTile(Tile* tile);

    // Virtual method for making a move
    virtual void makeMove(GameBoard* gameBoard, TileBag* tileBag, std::vector<Player*>& players, bool&quit) = 0;

private:
    std::string name;
    int score;
    LinkedList hand;

    // Helper function to clear resources
    void clear();
};

#endif // ASSIGN2_PLAYER_H