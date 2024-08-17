# Advanced-Programming-A2
Advanced-Programming-A2

Save/Load Game File Format: An assumption has been made by them team and the file input/output validation has been scoped to include only files with the .txt extension. This has been done to ensure that the save file has a valid extension that ensures operational consistency across multiple platforms

## To Compile: 

 `make`<br>
 
OR<br>
 g++ -Wall -Wextra -std=c++14 -O -o qwirkle qwirkle.cpp Tile.cpp Node.cpp LinkedList.cpp TileBag.cpp Player.cpp FileHandler.cpp Rules.cpp InputValidator.cpp Student.cpp GameBoard.cpp Tests.cpp AIPlayer.cpp ColourUtils.cpp HumanPlayer.cpp Config.cpp QwirkleUtils.cpp
 
To clean up: `make clean`
 
Run unit tests: `./qwirkle.exe test`

Run all end-to-end tests: `./tests/run`

Or run individually:<br>
 `./tests/credits/test` Test credits menu<br>
 `./tests/qwirkle/test` Test scoring a QWIRKLE<br>
 `./tests/load-game/test` Test loading game<br>
 `./tests/new-game/test` Test create new game<br>
 `./tests/save-game/test` Test saving game<br>
 `./tests/game-end/test` Test game ends when tiles run out<br>
 `./tests/line-validation/test` Test tile placement is valid based on neighboring tiles<br>
 `./tests/ai/test` Test AI opponent implementation<br>
 `./tests/board-size/test` Test resizing of the gameboard<br>
 `./tests/colour/test` Test tile colour output on gameboard<br>
 `./tests/help/test` Test help command argument output<br>
 `./tests/multiplayer/test` Test implementation of 2-4 player feature<br>
 `./tests/starting-hand/test` Test player starting tile number feature<br>

## Command-Line Options

Usage: ./qwirkle.exe [OPTIONS]<br>

Options:<br>
  --e2etest         Set seed to 0 for end-to-end tests<br>
  --colour          Enable colour display<br>
  --mp              Enable multiplayer mode<br>
  --ai              Enable AI players and multiplayer mode<br>
  --board-size=RxC  Set the board size (rows x columns)<br>
  --hand-size=N     Set the starting hand size to N<br>
  --all             Enable AI, colour display, and multiplayer mode<br>
  --help            Display this help message<br>
