#include "Tile.h"
#include "ColourUtils.h"
#include <iostream>
#include <string>

// Constructor
Tile::Tile(Colour colour, Shape shape) : colour(colour), shape(shape) {}

// Getter for the colour of the Tile.
Colour Tile::getColour() const {
    return colour;
}

// Getter for the shape of the Tile.
Shape Tile::getShape() const {
    return shape;
}

// Overload the << operator for the Tile class
std::ostream& operator<<(std::ostream& os, const Tile& tile) {
    os << ColourUtils::colourToString(tile.getColour()) << tile.getShape();
    return os;
}

// Print the Tile as a string in the format of "ColourShape".
std::string Tile::print() const {
    return ColourUtils::getAnsiColor(colour) + ColourUtils::colourToString(colour) + std::to_string(shape) + ColourUtils::RESET_COLOUR;
}

// This function checks if two Tile objects are equal.
bool Tile::operator==(const Tile& other) const {
    return colour == other.getColour() && shape == other.getShape();
}

// This function checks if two Tile objects are not equal.
bool Tile::operator!=(const Tile& other) const {
    return !(*this == other);
}

// This function checks if the tile has a match by color or shape, is the same or doesn't match at all
std::string Tile::matchType(const Tile& other) const {
    if (colour == other.getColour() && shape == other.getShape()) {
        return "same";
    }
    if (colour == other.getColour()) {
        return "colour";
    }
    if (shape == other.getShape()) {
        return "shape";
    }
    return "no-match";
}

// Gets the ANSI color code for the tile's colour.
std::string Tile::getAnsiColor() const {
    return ColourUtils::getAnsiColor(colour);
}

// Converts the Tile to a colored string representation.
std::string Tile::toColouredString() const {
    return getAnsiColor() + ColourUtils::colourToString(colour) + std::to_string(getShape()) + ColourUtils::RESET_COLOUR;
}

