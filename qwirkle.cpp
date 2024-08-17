#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Config.h"
#include "FileHandler.h"
#include "StudentInfo.h"
#include "QwirkleUtils.h"

#define EXIT_SUCCESS 0

// Function prototypes
void displayWelcomeMessage();
void displayMainMenu();
void showCredits();
void handleMenuChoice(int choice, bool &quit);

// Application entry point
int main(int argc, char **argv)
{
    bool quit = false;

    // Initialise configuration settings based on command-line arguments
    Config::initialise(argc, argv);

    displayWelcomeMessage();

    while (!quit) {
        displayMainMenu();
        std::string input = QwirkleUtils::handleInput(quit);

        if (!quit) {
            try {
                int choice = std::stoi(input);
                handleMenuChoice(choice, quit);
            }
            catch (const std::invalid_argument &) {
                std::cout << "Invalid input. Please enter a number between 1 and 4." << std::endl;
            }
            catch (const std::out_of_range &) {
                std::cout << "Invalid input. Number is out of range. Please enter a number between 1 and 4." << std::endl;
            }
        }

        if (quit) {
            std::cout << "Goodbye!" << std::endl;
        }
    }

    return EXIT_SUCCESS;
}

// Display the welcome message to the user
void displayWelcomeMessage()
{
    std::cout << "Welcome to Qwirkle!" << std::endl;
    std::cout << "-------------------" << std::endl;
}

// Display the main menu options to the user
void displayMainMenu()
{
    std::cout << "Menu" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Credits" << std::endl;
    std::cout << "4. Quit" << std::endl;
    std::cout << "> ";
}

// Display the credits for the game
void showCredits()
{
    std::cout << "---------------------------------------" << std::endl;
    for (size_t i = 0; i < numStudents; ++i) {
        students[i].display();
    }
    std::cout << "---------------------------------------" << std::endl;
}

void handleMenuChoice(int choice, bool &quit)
{
    if (choice == 1) {
        QwirkleUtils::startNewGame(quit);
    } else if (choice == 2) {
        QwirkleUtils::loadGame(quit);
    } else if (choice == 3) {
        showCredits();
    } else if (choice == 4) {
        quit = true;
    } else {
        std::cout << "Invalid choice. Please try again." << std::endl;
    }
}