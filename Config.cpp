#include "Config.h"
#include <string>
#include <ctime>
#include <iostream>
#include <stdexcept>

// Initialise the static variables
bool Config::colourDisplay = false;
bool Config::multiPlayerMode = false;
bool Config::enableAI = false;
int Config::randSeed = static_cast<unsigned int>(time(nullptr));
int Config::boardRows = Config::DEFAULT_BOARD_ROWS;
int Config::boardCols = Config::DEFAULT_BOARD_COLS;
int Config::startingHandSize = Config::DEFAULT_HAND_SIZE;
int Config::defaultNumPlayers = Config::DEFAULT_NUM_PLAYERS;
int Config::minPlayers = Config::MIN_PLAYERS;
int Config::maxPlayers = Config::MAX_PLAYERS;

void Config::displayHelp() 
{
    std::cout << "Usage: qwirkle [OPTIONS]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --e2etest         Set seed to 0 for end-to-end tests" << std::endl;
    std::cout << "  --colour          Enable colour display" << std::endl;
    std::cout << "  --mp              Enable multiplayer mode" << std::endl;
    std::cout << "  --ai              Enable AI players and multiplayer mode" << std::endl;
    std::cout << "  --board-size=RxC  Set the board size (rows x columns)" << std::endl;
    std::cout << "  --hand-size=N     Set the starting hand size to N" << std::endl;
    std::cout << "  --all             Enable AI, colour display, and multiplayer mode" << std::endl;
    std::cout << "  --help            Display this help message" << std::endl;
}

void Config::initialise(int argc, char** argv) 
{
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        try {
            if (arg == "--e2etest") {
                // Set seed to 0 for end-to-end tests
                randSeed = 0;  
            } else if (arg == "--colour") {
                colourDisplay = true;
            } else if (arg == "--mp") {
                multiPlayerMode = true;
            } else if (arg == "--ai") {
                enableAI = true;
                multiPlayerMode = true;
            } else if (arg == "--all") {
                enableAI = true;
                multiPlayerMode = true;
                colourDisplay = true;
            } else if (arg == "--help") {
                displayHelp();
                // Exit after displaying help
                exit(EXIT_SUCCESS);  
            } else if (arg.find("--board-size=") == 0) {
                // Start at index 13 to skip "--board-size="
                std::string size = arg.substr(13); 
                size_t xPos = size.find('x');
                if (xPos != std::string::npos) {
                    std::string rowPart = size.substr(0, xPos);
                    std::string colPart = size.substr(xPos + 1);

                    // Debug output to check substrings
                    std::cout << "Debug: rowPart = '" << rowPart << "', colPart = '" << colPart << "'" << std::endl;

                    // Convert to integers and handle potential errors
                    try {
                        boardRows = std::stoi(rowPart);
                    } catch (const std::invalid_argument&) {
                        throw std::invalid_argument("Invalid row size '" + rowPart + "' in --board-size. Must be an integer.");
                    }

                    try {
                        boardCols = std::stoi(colPart);
                    } catch (const std::invalid_argument&) {
                        throw std::invalid_argument("Invalid column size '" + colPart + "' in --board-size. Must be an integer.");
                    }

                    if (boardRows <= 0 || boardCols <= 0) {
                        throw std::invalid_argument("Board size must be positive integers.");
                    }
                } else {
                    throw std::invalid_argument("Invalid board size format. Use ROWSxCOLS.");
                }
            } else if (arg.find("--hand-size=") == 0) {
                // Get the section after "--hand-size="
                startingHandSize = std::stoi(arg.substr(12)); 
            } else {
                throw std::invalid_argument("Unknown argument '" + arg + "'. Use --help to see available options.");
            }
        } catch (const std::invalid_argument& e) {
            std::cerr << "Error: Invalid argument in '" << arg << "'. " << e.what() << std::endl;
            exit(EXIT_FAILURE);
        } catch (const std::out_of_range& e) {
            std::cerr << "Error: Argument out of range in '" << arg << "'. " << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}