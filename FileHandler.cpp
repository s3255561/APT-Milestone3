#include "FileHandler.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "Config.h"
#include "HumanPlayer.h"
#include "AIPlayer.h"
#include "ColourUtils.h"

/*
 * Helper function to convert LinkedList to std::vector
 * This function iterates through the linked list, extracts each tile,
 * and adds it to a std::vector.
 */
std::vector<Tile*> FileHandler::linkedListToVector(LinkedList* list) {
    std::vector<Tile*> vec;
    Node* current = list->getHead();
    while (current) {
        vec.push_back(current->getTile()); 
        current = current->getNext();
    }
    return vec; 
}

/*
 * Helper function to convert std::vector to LinkedList
 * This function iterates through the std::vector,
 * and adds each tile to the linked list.
 */
void FileHandler::vectorToLinkedList(const std::vector<Tile*>& vec, LinkedList* list) {
    list->clear();
    for (Tile* tile : vec) {
        list->addBack(tile); 
    }
}

/*
 * Save the game state to a file
 * This function serializes the players, board, tile bag,
 * and current player, then writes them to a file.
 */
void FileHandler::saveGame(const std::string& filename, const std::vector<Player*>& players, TileBag* tileBag, GameBoard* board, Player* currentPlayer) {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {

        // Save the configuration settings
        outFile << Config::colourDisplay << "\n";
        outFile << Config::multiPlayerMode << "\n";
        outFile << Config::enableAI << "\n";
        outFile << Config::randSeed << "\n";
        outFile << Config::boardRows << "\n";
        outFile << Config::boardCols << "\n";
        outFile << Config::startingHandSize << "\n";
        outFile << Config::defaultNumPlayers << "\n";
        outFile << Config::minPlayers << "\n";
        outFile << Config::maxPlayers << "\n";

        // Save the number of players
        outFile << players.size() << "\n";

        // Save each player
        for (Player* player : players) {
            std::string playerData = serialisePlayer(player);
            outFile << playerData << std::endl;
        }

        // Save the board
        std::string boardData = serialiseBoard(board);
        outFile << boardData << std::endl;

        // Save the tile bag
        std::string tileBagData = serialiseTileBag(tileBag);
        outFile << tileBagData << std::endl;

        // Save the current player
        std::string currentPlayerData = serialiseCurrentPlayer(currentPlayer);
        outFile << currentPlayerData;

        outFile.close();
        std::cout << "Game successfully saved" << std::endl;
    } else {
        std::cerr << "Error: Unable to open file for writing" << std::endl;
    }
}

/*
 * Load the game state from a file
 * This function reads the serialized players, board, tile bag,
 * and current player from a file and deserializes them.
 * THE BELOW INCLUDES PRINT OUTS TO CHECK FILE IS READING
 * CORRECTLY AND MAY BE DELETED AND THE ABOVE FUNCTION USED
 * INSTEAD.
 */
bool FileHandler::loadGame(const std::string& filename, std::vector<Player*>& players, TileBag* tileBag, GameBoard*& board, Player*& currentPlayer) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Error: Unable to open file for reading" << std::endl;
        return false;
    }

    std::string line;

    // Load the configuration settings
    try {
        std::getline(inFile, line); Config::colourDisplay = (line == "1");
        std::getline(inFile, line); Config::multiPlayerMode = (line == "1");
        std::getline(inFile, line); Config::enableAI = (line == "1");
        std::getline(inFile, line); Config::randSeed = std::stoi(line);
        std::getline(inFile, line); Config::boardRows = std::stoi(line);
        std::getline(inFile, line); Config::boardCols = std::stoi(line);
        std::getline(inFile, line); Config::startingHandSize = std::stoi(line);
        std::getline(inFile, line); Config::defaultNumPlayers = std::stoi(line);
        std::getline(inFile, line); Config::minPlayers = std::stoi(line);
        std::getline(inFile, line); Config::maxPlayers = std::stoi(line);
    } catch (const std::exception& e) {
        std::cerr << "Error: Invalid configuration data - " << e.what() << std::endl;
        return false;
    }

    // Read the number of players
    std::getline(inFile, line);
    int numPlayers = std::stoi(line);

    // Load each player
    for (int i = 0; i < numPlayers; ++i) {
        std::string nameData, scoreData, handData;
        if (!std::getline(inFile, nameData) || !std::getline(inFile, scoreData) || !std::getline(inFile, handData)) {
            std::cerr << "Error: Incomplete player data at player " << i + 1 << std::endl;
            return false;
        }

        Player* player = nullptr;
        if (nameData.find("AI_") == 0) {
            // Remove "AI_" prefix
            player = new AIPlayer(nameData.substr(3));  
        } else if (nameData.find("HUMAN_") == 0) {
            // Remove "HUMAN_" prefix
            player = new HumanPlayer(nameData.substr(6));  
        } else {
            std::cerr << "Error: Unknown player type for player " << nameData << std::endl;
            return false;
        }

        std::string combinedData = nameData + "\n" + scoreData + "\n" + handData;
        deserialisePlayer(player, combinedData);
        players.push_back(player);
    }

    // Read and deserialise the board
    std::string boardData;
    if (!std::getline(inFile, boardData)) {
        std::cerr << "Error: Board data missing" << std::endl;
        return false;
    }

    std::string boardTiles;
    if (!std::getline(inFile, boardTiles)) {
        std::cerr << "Error: Board tiles data missing" << std::endl;
        return false;
    }
    boardData += "\n" + boardTiles;

    GameBoard* newBoard = deserialiseBoard(boardData);
    if (!newBoard) {
        std::cerr << "Error: Invalid board data" << std::endl;
        return false;
    }

    newBoard->setColourDisplay(Config::colourDisplay);
    delete board;
    board = newBoard;

    // Read and deserialise Tile Bag
    std::string tileBagData;
    if (!std::getline(inFile, tileBagData)) {
        std::cerr << "Error: Tile bag data missing" << std::endl;
        return false;
    }
    deserialiseTileBag(tileBag, tileBagData);

    // Read and deserialise Current Player
    std::string currentPlayerData;
    if (!std::getline(inFile, currentPlayerData)) {
        std::cerr << "Error: Current player data missing" << std::endl;
        return false;
    }

    currentPlayer = nullptr;
    for (Player* player : players) {
        if (player->getName() == currentPlayerData) {
            currentPlayer = player;
            break;
        }
    }

    if (currentPlayer == nullptr) {
        std::cerr << "Error: Failed to set current player" << std::endl;
        return false;
    }

    inFile.close();
    return true;
}

/*
 * Check if a file exists
 * This function tries to open a file and returns true if successful.
 */
bool FileHandler::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    return file.good(); 
}

/*
 * Method to read file content into a string.
 */
std::string FileHandler::readFileContent(const std::string& filename) const {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        std::cerr << "Error: Unable to open file for reading" << std::endl;
        return "";
    }

    std::string fileContent;
    std::string line;
    while (std::getline(inFile, line)) {
        fileContent += line + "\n";
    }

    inFile.close();

    if (!fileContent.empty() && fileContent[fileContent.length() - 1] == '\n') {
        fileContent.erase(fileContent.length() - 1);
    }

    return fileContent;
}

/*
 * Serialize a Player object to a string
 * This function converts the player's name, score, and hand
 * to a string representation.
 */
std::string FileHandler::serialisePlayer(Player* player) {
    std::string result;

    // Add a prefix to indicate if the player is an AI or Human
    if (dynamic_cast<AIPlayer*>(player) != nullptr) {
        result += "AI_";
    } else if (dynamic_cast<HumanPlayer*>(player) != nullptr) {
        result += "HUMAN_";
    }

    result += player->getName() + "\n";
    result += std::to_string(player->getScore()) + "\n";

    // Serialise player's hand
    LinkedList* hand = player->getHand(); 
    Node* current = hand->getHead();
    while (current) {
        Tile* tile = current->getTile();
        result += tile->getColour() + std::to_string(tile->getShape());
        if (current->getNext()) {
            result += ",";
        }
        current = current->getNext();
    }

    return result;
}

/*
 * Serialize a TileBag object to a string
 * This function converts the tiles in the bag to a string representation.
 */
std::string FileHandler::serialiseTileBag(TileBag* tileBag) {
    std::string result;
    LinkedList* tiles = tileBag->getTiles();
    Node* current = tiles->getHead();
    while (current) {
        Tile* tile = current->getTile();
        result += tile->getColour() + std::to_string(tile->getShape());
        if (current->getNext()) {
            result += ",";
        }
        current = current->getNext();
    }

    return result;
}

/*
 * Serialize a GameBoard object to a string
 * This function converts the board size and tiles
 * to a string representation.
 */
std::string FileHandler::serialiseBoard(GameBoard* board) {
    std::string result;
    result += std::to_string(board->getRows()) + "," + std::to_string(board->getCols()) + "\n";

    bool firstTile = true;
    for (int row = 0; row < board->getRows(); ++row) {
        for (int col = 0; col < board->getCols(); ++col) {
            Tile* tile = board->getTile(row, col);
            if (tile) {
                if (!firstTile) {
                    result += ",";
                }
                result += tile->getColour() + std::to_string(tile->getShape()) + "@" + std::string(1, 'A' + row) + std::to_string(col);
                firstTile = false;
            }
        }
    }

    return result;
}

/*
 * Serialize the current player to a string
 * This function returns the name of the current player.
 */
std::string FileHandler::serialiseCurrentPlayer(Player* currentPlayer) {
    return currentPlayer->getName();
}

/*
 * Deserialize a Player object from a string
 * This function extracts the player's name, score, and hand
 * from a string representation.
 */
void FileHandler::deserialisePlayer(Player* player, const std::string& data) {
    size_t pos = 0;
    size_t nextPos = data.find('\n', pos);

    // Parse the player's name and remove any prefixes
    std::string name = data.substr(pos, nextPos - pos);
    if (name.find("AI_") == 0) {
        // Remove "AI_" prefix
        name = name.substr(3);  
    } else if (name.find("HUMAN_") == 0) {
        // Remove "HUMAN_" prefix
        name = name.substr(6);  
    }
    player->setName(name);
        
    pos = nextPos + 1;
    if (pos >= data.size()) {
        std::cerr << "Error: Missing player score and hand data" << std::endl;
        return;
    }

    // Parse the player's score
    nextPos = data.find('\n', pos);
    if (nextPos == std::string::npos) {
        std::cerr << "Error: Missing newline after player score" << std::endl;
        return;
    }
    std::string scoreStr = data.substr(pos, nextPos - pos);
    int score = std::stoi(scoreStr);
    player->setScore(score);

    pos = nextPos + 1;
    if (pos >= data.size()) {
        return;
    }

    // Parse the player's hand
    std::string handData = data.substr(pos);
    std::vector<Tile*> hand;
    size_t start = 0;
    size_t end = handData.find(',', start);
    
    while (end != std::string::npos) {
        std::string tileData = handData.substr(start, end - start);
        Colour colour = ColourUtils::charToColour(tileData[0]);
        Shape shape = std::stoi(tileData.substr(1));
        hand.push_back(new Tile(colour, shape));

        start = end + 1;
        end = handData.find(',', start);
    }

    if (start < handData.length()) {
        std::string tileData = handData.substr(start);
        Colour colour = ColourUtils::charToColour(tileData[0]);
        Shape shape = std::stoi(tileData.substr(1));
        hand.push_back(new Tile(colour, shape));
    }

    // Convert vector to linked list and set it as player's hand
    vectorToLinkedList(hand, player->getHand());
}

/*
 * Deserialize a TileBag object from a string
 * This function extracts the tiles from a string representation
 * and adds them to the tile bag.
 */
void FileHandler::deserialiseTileBag(TileBag* tileBag, const std::string& data) {
    std::vector<Tile*> tiles;
    size_t start = 0;
    size_t end = data.find(',', start);
    while (end != std::string::npos) {
        std::string tileEntry = data.substr(start, end - start);
        Colour colour = ColourUtils::charToColour(tileEntry[0]);
        Shape shape = std::stoi(tileEntry.substr(1));
        tiles.push_back(new Tile(colour, shape));
        start = end + 1;
        end = data.find(',', start);
    }
    if (start < data.length()) {
        std::string tileEntry = data.substr(start);
        Colour colour = ColourUtils::charToColour(tileEntry[0]);
        Shape shape = std::stoi(tileEntry.substr(1));
        tiles.push_back(new Tile(colour, shape));
    }
    vectorToLinkedList(tiles, tileBag->getTiles());
}

/*
 * Deserialize a GameBoard object from a string
 * This function extracts the board size and tiles
 * from a string representation and sets up the board.
 */
GameBoard* FileHandler::deserialiseBoard(const std::string& data) {
    size_t pos = 0;
    size_t nextPos = data.find('\n', pos);
    std::string sizeData = data.substr(pos, nextPos - pos);
    size_t commaPos = sizeData.find(',');
    if (commaPos == std::string::npos) {
        std::cerr << "Error: Invalid board size format" << std::endl;
        return nullptr;
    }

    // Extract number of rows
    int rows = std::stoi(sizeData.substr(0, commaPos)); 
    // Extract number of columns
    int cols = std::stoi(sizeData.substr(commaPos + 1));

    GameBoard* board = new GameBoard(rows, cols);

    pos = nextPos + 1;
    std::string boardData = data.substr(pos);
    size_t start = 0;
    size_t end = boardData.find(',', start);
    while (end != std::string::npos) {
        std::string tileData = boardData.substr(start, end - start);
        Colour colour = ColourUtils::charToColour(tileData[0]);
        size_t atPos = tileData.find('@');
        if (atPos == std::string::npos) {
            std::cerr << "Error: Invalid tile data format - " << tileData << std::endl;
            delete board;
            return nullptr;
        }

        Shape shape = std::stoi(tileData.substr(1, atPos - 1));
        std::string position = tileData.substr(atPos + 1);

        if (position.length() < 2) {
            std::cerr << "Error: Invalid tile position format - " << position << std::endl;
            delete board;
            return nullptr;
        }

        int row = position[0] - 'A';
        int col = std::stoi(position.substr(1));
        board->placeTile(row, col, new Tile(colour, shape));

        start = end + 1;
        end = boardData.find(',', start);
    }

    if (start < boardData.length()) {
        std::string tileData = boardData.substr(start);

        Colour colour = ColourUtils::charToColour(tileData[0]);
        size_t atPos = tileData.find('@');
        if (atPos == std::string::npos) {
            std::cerr << "Error: Invalid tile data format - " << tileData << std::endl;
            delete board;
            return nullptr;
        }

        Shape shape = std::stoi(tileData.substr(1, atPos - 1));
        std::string position = tileData.substr(atPos + 1);

        if (position.length() < 2) {
            std::cerr << "Error: Invalid tile position format - " << position << std::endl;
            delete board;
            return nullptr;
        }

        int row = position[0] - 'A';
        int col = std::stoi(position.substr(1));
        board->placeTile(row, col, new Tile(colour, shape));
    }

    return board;
}

/*
 * Deserialize the current player from a string
 * This function sets the current player's name
 * from a string representation.
 */
void FileHandler::deserialiseCurrentPlayer(Player* currentPlayer, const std::string& data) {
    if (currentPlayer == nullptr) {
        std::cerr << "Error: currentPlayer is null during deserialisation." << std::endl;
        return;
    }
    currentPlayer->setName(data);
}