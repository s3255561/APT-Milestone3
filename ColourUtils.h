#ifndef ASSIGN2_COLOUR_UTILS_H
#define ASSIGN2_COLOUR_UTILS_H

#include <string>

enum Colour 
{
    RED = 'R',
    ORANGE = 'O',
    YELLOW = 'Y',
    GREEN = 'G',
    BLUE = 'B',
    PURPLE = 'P'
};

class ColourUtils 
{
    public:
        // ANSI color codes for different colours
        static const std::string RED_COLOUR;
        static const std::string ORANGE_COLOUR;
        static const std::string YELLOW_COLOUR;
        static const std::string GREEN_COLOUR;
        static const std::string BLUE_COLOUR;
        static const std::string PURPLE_COLOUR;
        static const std::string RESET_COLOUR;

        // Utility method to get the colour code based on the colour character
        static std::string getAnsiColor(Colour colour);

        // Utility method to convert a char to a Colour enum
        static Colour charToColour(char colourChar);

        // Utility method to convert a Colour enum to its string representation
        static std::string colourToString(Colour colour);
};

// Overloaded + operator to support concatenating Colour enum with std::string
std::string operator+(const std::string& str, Colour colour);
std::string operator+(Colour colour, const std::string& str);

#endif // ASSIGN2_COLOUR_UTILS_H
