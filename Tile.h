#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H

#include <string>
#include "ColourUtils.h"

using Shape = int;

class Tile {
public:

    Tile(Colour colour, Shape shape);

    // Getter for the colour of the Tile.
    Colour getColour() const;

    // Getter for the shape of the Tile.
    Shape getShape() const;

    // Print the Tile as a string in the format of "ColourShape".
    std::string print() const;

    // Equality operator for Tile comparison
    bool operator==(const Tile& other) const;

    // Inequality operator for Tile comparison
    bool operator!=(const Tile& other) const;

    // Function to check if the tile has a match by colour or shape
    std::string matchType(const Tile& other) const;

    // Output the Tile with ANSI colour codes
    std::string toColouredString() const;

    // Function to overload the << operator
    friend std::ostream& operator<<(std::ostream& os, const Tile& tile);

    std::string getAnsiColour() const;

private:
    Colour colour;
    Shape shape;
};

#endif // ASSIGN2_TILE_H
