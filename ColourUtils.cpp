#include "ColourUtils.h"
#include <stdexcept>

// ANSI colour codes definitions (example)
const std::string ColourUtils::RED_COLOUR = "\033[31m";
const std::string ColourUtils::ORANGE_COLOUR = "\033[33m";
const std::string ColourUtils::YELLOW_COLOUR = "\033[33m";
const std::string ColourUtils::GREEN_COLOUR = "\033[32m";
const std::string ColourUtils::BLUE_COLOUR = "\033[34m";
const std::string ColourUtils::PURPLE_COLOUR = "\033[35m";
const std::string ColourUtils::RESET_COLOUR = "\033[0m";

// Utility methods implementations...
std::string ColourUtils::getAnsiColour(Colour colour) 
{
    switch (colour) {
        case RED: return RED_COLOUR;
        case ORANGE: return ORANGE_COLOUR;
        case YELLOW: return YELLOW_COLOUR;
        case GREEN: return GREEN_COLOUR;
        case BLUE: return BLUE_COLOUR;
        case PURPLE: return PURPLE_COLOUR;
        default: return RESET_COLOUR;
    }
}

Colour ColourUtils::charToColour(char colourChar) 
{
    switch (colourChar) {
        case 'R': return RED;
        case 'O': return ORANGE;
        case 'Y': return YELLOW;
        case 'G': return GREEN;
        case 'B': return BLUE;
        case 'P': return PURPLE;
        default: throw std::invalid_argument("Invalid colour character");
    }
}

std::string ColourUtils::colourToString(Colour colour) 
{
    return std::string(1, static_cast<char>(colour));
}

// Overloaded + operator implementations
std::string operator+(const std::string& str, Colour colour) 
{
    return str + static_cast<char>(colour);
}

std::string operator+(Colour colour, const std::string& str) 
{
    return static_cast<char>(colour) + str;
}