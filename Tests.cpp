#include <iostream>
#include <fstream>
#include "TileBag.h"
#include "TileCodes.h"
#include "FileHandler.h"
#include "HumanPlayer.h"

class Tests
{
public:
    // Entry point to run all the tests
    static void run()
    {
        std::cout << "Running tests\n" << std::endl;

        tileBagConstructorOverloadTest();
        tileBagShuffleTest();
        readFileContentTest();
        saveGameTest();
    }
    
    static void tileBagConstructorOverloadTest()
    {
        std::cout << "#tileBagConstructorOverloadTest" << std::endl;
        
        // given
        std::vector<Tile*> tileVector = std::vector<Tile*>({
            new Tile(ColourUtils::charToColour('R'), CIRCLE), 
            new Tile(ColourUtils::charToColour('R'), STAR_4)
    });

        
        // when
        TileBag *tilebag = new TileBag(tileVector);
        
        // then
        std::string tileString = tilebag->getTiles()->toString();
        std::cout << "Tilebag instantiated with tiles: " << tileString << std::endl;

        assert_equality("R1, R2", tileString);
    }
    
    // Test to check the shuffle functionality of the TileBag
    static void tileBagShuffleTest()
    {
        std::cout << "#tileShuffleTest" << std::endl;
        
        // given
        TileBag *tilebag = new TileBag();        
        LinkedList *tiles = tilebag->getTiles();
        std::string tileString = tiles->toString();
        
        // Use the current time as a random seed
        int randSeed = (unsigned int)time(NULL);
        
        // when
        tilebag->shuffle(randSeed);
        std::string shuffledTileString = tiles->toString();
        
        // then
        std::cout << "Original: " << tileString << std::endl;
        std::cout << "Shuffled: " << shuffledTileString << std::endl;

        assert_inequality(tileString, shuffledTileString);
    }
    
    // Test to check if a file's content can be read correctly
    static void readFileContentTest()
    {
        std::cout << "#readFileContentTest" << std::endl;
        
        // given
        FileHandler fileHandler;
        std::string savedGame = "ABBY\n3\nR1,G2\nBILL\n1\nB3,Y4\n12,12\nR1@A0,G2@B1,B3@C2,Y4@D3\nR1,G2,B3,Y4\nABBY";
        
        // when
        std::string fileContent = fileHandler.readFileContent("tests/stubs/load-game-test-stub.txt");
        
        // then
        std::cout << "File content: " << fileContent << std::endl;

        assert_equality(savedGame, fileContent);
    }
    
    // Test to check if the game state is saved correctly to a file
    static void saveGameTest() 
    {
        std::cout << "#saveGameTest" << std::endl;
        
        // Given
        HumanPlayer player1("ALICE");
        HumanPlayer player2("BOB");

        Tile* tile1 = new Tile(ColourUtils::charToColour('R'), 1);
        Tile* tile2 = new Tile(ColourUtils::charToColour('G'), 2);
        Tile* tile3 = new Tile(ColourUtils::charToColour('B'), 3);
        Tile* tile4 = new Tile(ColourUtils::charToColour('Y'), 4);

        player1.addTileToHand(tile1);
        player2.addTileToHand(tile2);

        GameBoard board(6, 6);
        board.placeTile(3, 3, tile3);

        std::vector<Tile*> tiles = { tile4 };
        TileBag* tileBag = new TileBag(tiles);

        std::vector<Player*> players = { &player1, &player2 };

        HumanPlayer currentPlayer("ALICE");
        
        // When
        std::string filename = "./tests/stubs/save-game-test-stub.txt";
        FileHandler fileHandler;
        fileHandler.saveGame(filename, players, tileBag, &board, &currentPlayer);

        // Then
        std::string fileContent = fileHandler.readFileContent(filename);
        std::string expectedSavedGame = 
            "ALICE\n0\nR1\n"
            "BOB\n0\nG2\n"
            "6,6\nB3@D3\n"
            "Y4\nALICE";

        assert_equality(expectedSavedGame, fileContent);

        // Clean up
        delete tile1;
        delete tile2;
        delete tile3;
        delete tile4;
        delete tileBag;
    }



    // Utility function to assert that two strings are equal
    static void assert_equality(std::string expected, std::string actual)
    {
        if (expected != actual) {
            // If they are not equal, print a failure message and throw an exception
            std::cout << "\033[91m" << "Failed \n" << "\033[0m" << std::endl;
            throw std::runtime_error("Expected: \n" + expected + "\nReceived: \n" + actual);
        } else {
            // If they are equal, print a success message
            std::cout << "\033[32m" << "Passed \n" << "\033[0m" << std::endl;
        }
    }
    
    // Utility function to assert that two strings are not equal
    static void assert_inequality(std::string expected, std::string actual)
    {
        if (expected == actual) {
            // If they are equal, print a failure message and throw an exception
            std::cout << "\033[91m" << "Failed \n" << "\033[0m" << std::endl;
            throw std::runtime_error("Expected: \n" + expected + "\nNot to equal: \n" + actual);
        } else {
            // If they are not equal, print a success message
            std::cout << "\033[32m" << "Passed \n" << "\033[0m" << std::endl;
        }
    }
};
