#ifndef ASSIGN2_CONFIG_H
#define ASSIGN2_CONFIG_H

class Config 
{
    public:
        // Static variables that represent the configuration settings
        static bool colourDisplay;
        static bool multiPlayerMode;
        static bool enableAI;
        static int randSeed;
        static int boardRows;
        static int boardCols;
        static int startingHandSize;
        static int defaultNumPlayers;
        static int minPlayers;
        static int maxPlayers;

        // Function to initialise config based on command-line arguments
        static void initialise(int argc, char** argv);

    private:

        static constexpr int DEFAULT_BOARD_ROWS = 26;
        static constexpr int DEFAULT_BOARD_COLS = 26;
        static constexpr int DEFAULT_HAND_SIZE = 6;
        static constexpr int DEFAULT_NUM_PLAYERS = 2;
        static constexpr int MIN_PLAYERS = 2;
        static constexpr int MAX_PLAYERS = 4;

        static void displayHelp();
};

#endif // ASSIGN2_CONFIG_H