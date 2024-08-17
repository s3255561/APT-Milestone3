#include "QwirkleUtils.h"
#include "Player.h"
#include "GameBoard.h"
#include <iostream>
#include "Rules.h"
#include "InputValidator.h"
#include "FileHandler.h"
#include "Config.h"
#include "AIPlayer.h"
#include "HumanPlayer.h"

namespace QwirkleUtils 
{
    // Display the current game state: player scores, board, and hand of the current player
    void displayGameState(std::vector<Player*>& players, GameBoard* gameBoard, Player* currentPlayer) 
    {
        for (const auto& player : players) 
        {
            std::cout << "Score for " << player->getName() << ": " << player->getScore() << std::endl;
        }    

        std::cout << std::endl;
        std::cout << gameBoard->displayBoard() << std::endl;
        std::cout << "Tiles in hand: " << currentPlayer->getHand()->toString() << std::endl;
        std::cout << "Your move " << currentPlayer->getName() << ": ";
    }

    // Handle input from the user and check for EOF (end of file)
    std::string handleInput(bool &quit)
    {
        std::string input;
        std::getline(std::cin, input);
        std::cout << std::endl;
        if (std::cin.eof()) {
            quit = true;
        }
        return input;
    }

    // Print the scores of all players
    void printScores(std::vector<Player*>& players) 
    {
        for (Player* player : players) 
        {
            std::cout << "Score for " << player->getName() << ": " << player->getScore() << std::endl;
        }
    }

    // Determine the winner by finding the player with the highest score
    Player* determineWinner(std::vector<Player*>& players) 
    {
        Player* winner = players[0];
        for (Player* player : players) 
        {
            if (player->getScore() > winner->getScore()) 
            {
                winner = player;
            }
        }
        return winner;
    }

    void gameLoop(std::vector<Player*>& players, TileBag* tileBag, GameBoard* gameBoard, Player* currentPlayer) 
    {
        bool quit = false;
        bool gameOver = false;
        size_t currentPlayerIndex = 0;

        // Find the index of the currentPlayer
        while (currentPlayerIndex < players.size() && players[currentPlayerIndex]->getName() != currentPlayer->getName()) 
        {
            ++currentPlayerIndex;
        }

        while (!quit && !gameOver) 
        {
            currentPlayer = players[currentPlayerIndex];

            QwirkleUtils::displayGameState(players, gameBoard, currentPlayer);

            // Execute the current player's turn and pass quit by reference
            currentPlayer->makeMove(gameBoard, tileBag, players, quit);

            // Check if the game is over after each turn
            gameOver = Rules::isGameOver(players, tileBag, gameBoard);
            if (gameOver) 
            {
                std::cout << gameBoard->displayBoard() << std::endl;
                Player* winner = determineWinner(players);
                std::cout << "\nGame over!\nThe winner is " << winner->getName() << " with a score of " << winner->getScore() << "!\n" << std::endl;
            }

            if (!quit && !gameOver) 
            {
                currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
            }
        }
    }

    // Load a previously saved game
    void loadGame(bool &quit) 
    {
        std::cout << "Enter the filename from which to load a game:" << std::endl;
        std::cout << "> ";
        std::string filename = QwirkleUtils::handleInput(quit);

        if (quit) 
        {
            return;
        }

        if (!quit && !InputValidator::isFileNameValid(filename)) 
        {
            std::cerr << "Error: Invalid file name or format." << std::endl;
            return;
        }

        FileHandler fileHandler;

        if (!quit && !fileHandler.fileExists(filename)) 
        {
            std::cerr << "Error: File does not exist." << std::endl;
            return;
        }

        // Create containers for loading the game state
        std::vector<Player*> players;
        TileBag* loadedTileBag = new TileBag();
        GameBoard* loadedBoard = new GameBoard();
        Player* currentPlayer = nullptr;

        // Load the game state
        bool gameLoaded = fileHandler.loadGame(filename, players, loadedTileBag, loadedBoard, currentPlayer);
        
        if (!gameLoaded) 
        {
            std::cerr << "Error: Invalid file format." << std::endl;
        } else 
        {
            std::cout << "Qwirkle game successfully loaded\n" << std::endl;
            gameLoop(players, loadedTileBag, loadedBoard, currentPlayer);
        }

        // Clean up loaded objects
        for (Player* player : players) 
        {
            delete player;
        }
        delete loadedTileBag;
        delete loadedBoard;

        // Ensure we do not double-delete currentPlayer if it points to a player already in the players vector
        bool playerExists = false;
        for (Player* player : players) 
        {
            if (player == currentPlayer) 
            {
                playerExists = true;
            }
        }

        // If the currentPlayer was not found in the players vector, delete it
        if (!playerExists) 
        {
            delete currentPlayer;
        }
    }

    // Start a new game by initialising players, the game board, and the tile bag
    void startNewGame(bool &quit)
    {
        // Default to 2 players if multiPlayerMode is false
        int numPlayers = Config::defaultNumPlayers; 

        if (Config::multiPlayerMode) {
            std::cout << "Enter the number of players (2-4):" << std::endl;
            std::cout << "> ";
            std::string input = QwirkleUtils::handleInput(quit);

            if (quit) 
            {
                return;
            }

            // Keep prompting the user until valid input is provided or they quit
            while (!quit) {
                try {
                    numPlayers = std::stoi(input);
                    if (numPlayers >= Config::minPlayers && numPlayers <= Config::maxPlayers) 
                    {
                        quit = true; 
                    } else 
                    {
                        std::cout << "Invalid number. Please enter a number between 2 and 4." << std::endl;
                        std::cout << "> ";
                        input = QwirkleUtils::handleInput(quit);                    
                    }
                }
                catch (const std::exception &) 
                {
                    std::cout << "Invalid input. Please enter a number between 2 and 4." << std::endl;
                    std::cout << "> ";
                    input = QwirkleUtils::handleInput(quit);
                }

            }
            // Reset quit flag if the user provided valid input
            quit = false; 
        }

        // Initialise players
        std::vector<Player*> players;
        for (int i = 0; i < numPlayers && !quit; ++i) 
        {
            bool playerAdded = false;

            if (Config::enableAI) {
                std::string isAIPlayer;

                // Loop until valid input is provided
                bool validInput = false;
                while (!validInput && !quit) {
                    std::cout << "Is player " << (i + 1) << " an AI player? (yes/no)" << std::endl;
                    std::cout << "> ";
                    isAIPlayer = QwirkleUtils::handleInput(quit);

                    if (quit) 
                    {
                        return;
                    }

                    if (isAIPlayer == "yes" || isAIPlayer == "no") 
                    {
                        validInput = true;
                    } else if (!quit) 
                    {
                        std::cout << "Invalid input. Please enter 'yes' or 'no'." << std::endl;
                    }
                }

                if (isAIPlayer == "yes" && !quit) {
                    std::cout << "Enter a name for AI player " << (i + 1) << " (uppercase characters only)" << std::endl;
                    std::cout << "> ";
                    std::string aiPlayerName = QwirkleUtils::handleInput(quit);

                    while (!InputValidator::isValidName(aiPlayerName) && !quit) 
                    {
                        std::cout << "Invalid name. Please enter uppercase letters only." << std::endl;
                        std::cout << "> ";
                        aiPlayerName = QwirkleUtils::handleInput(quit);
                    }

                    if (!quit) {
                        players.push_back(new AIPlayer(aiPlayerName));
                        playerAdded = true;
                    }
                } else 
                {
                    playerAdded = addHumanPlayer(players, i, quit);
                }
            } else {
                // Directly add human players if AI is not enabled
                playerAdded = addHumanPlayer(players, i, quit);
            }

            // If a player wasn't added due to quit or input issues, handle cleanup
            if (!playerAdded) {
                for (Player* player : players) 
                {
                    delete player;
                }
                return;
            }
        }

        // Proceed with game setup only if all players were successfully added
        GameBoard gameBoard(Config::boardRows, Config::boardCols);
        gameBoard.setColourDisplay(Config::colourDisplay);

        TileBag tileBag;
        tileBag.shuffle(Config::randSeed);

        std::cout << "Let's Play!" << std::endl;
        std::cout << std::endl;

        // Draw initial tiles for each player
        for (auto* player : players) 
        {
            player->drawQuantityTiles(&tileBag, Config::startingHandSize);
        }

        Player* startingPlayer = players[0];  
        gameLoop(players, &tileBag, &gameBoard, startingPlayer);
    }

    bool addHumanPlayer(std::vector<Player*>& players, int playerIndex, bool &quit) 
    {
        std::cout << "Enter a name for player " << (playerIndex + 1) << " (uppercase characters only)" << std::endl;
        std::cout << "> ";
        std::string playerName = QwirkleUtils::handleInput(quit);

        if (quit) 
        {
            return false;
        }

        while (!InputValidator::isValidName(playerName) && !quit) 
        {
            std::cout << "Invalid name. Please enter uppercase letters only." << std::endl;
            std::cout << "> ";
            playerName = QwirkleUtils::handleInput(quit);

            if (quit) 
            {
                return false;
            }
        }

        if (!quit) 
        {
            players.push_back(new HumanPlayer(playerName));
            return true;
        }

        return false;
    }
}