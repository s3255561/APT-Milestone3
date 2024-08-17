#include <iostream>
#include "GameBoard.h"

// Constructor: Initialises a GameBoard with default size (26x26) and allocates memory for the board.
GameBoard::GameBoard() : rows(26), cols(26), board(rows, std::vector<Tile*>(cols, nullptr)) {
}

// Parameterised Constructor: Initialises a GameBoard with the specified number of rows and columns.
GameBoard::GameBoard(int rows, int cols) : rows(rows), cols(cols), board(rows, std::vector<Tile*>(cols, nullptr)) {
}

// Destructor: Cleans up the dynamically allocated tiles on the board.
GameBoard::~GameBoard() {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (board[row][col] != nullptr) {
                delete board[row][col];
            }
        }
    }
}

// Copy constructor: Creates a deep copy of another GameBoard.
GameBoard::GameBoard(const GameBoard& other)
    : rows(other.rows),
      cols(other.cols),
      board(rows, std::vector<Tile*>(cols, nullptr)) {
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      if (other.board[row][col] != nullptr) {
        board[row][col] = new Tile(*other.board[row][col]);
      }
    }
  }
}

// Copy assignment operator: Clears current resources and assigns values from another GameBoard.
GameBoard& GameBoard::operator=(const GameBoard& other) {
  if (this != &other) {
    // Clean up current resources.
    for (int row = 0; row < rows; ++row) {
      for (int col = 0; col < cols; ++col) {
        delete board[row][col];
      }
    }

    // Copy new values.
    rows = other.rows;
    cols = other.cols;
    board = std::vector<std::vector<Tile*>>(rows, std::vector<Tile*>(cols, nullptr));

    for (int row = 0; row < rows; ++row) {
      for (int col = 0; col < cols; ++col) {
        if (other.board[row][col] != nullptr) {
          board[row][col] = new Tile(*other.board[row][col]);
        }
      }
    }
  }
  return *this;
}

// Move constructor: Transfers ownership of another GameBoard's resources to this one.
GameBoard::GameBoard(GameBoard&& other)
    : rows(other.rows), cols(other.cols), board(std::move(other.board)) {
  other.rows = 0;
  other.cols = 0;
}

// Move assignment operator: Clears current resources and transfers ownership from another GameBoard.
GameBoard& GameBoard::operator=(GameBoard&& other) {
  if (this != &other) {
    // Clean up current resources.
    for (int row = 0; row < rows; ++row) {
      for (int col = 0; col < cols; ++col) {
        delete board[row][col];
      }
    }

    // Move new values.
    rows = other.rows;
    cols = other.cols;
    board = std::move(other.board);

    other.rows = 0;
    other.cols = 0;
  }
  return *this;
}

// Place a tile on the board
void GameBoard::placeTile(int row, int col, Tile* tile) {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        board[row][col] = tile;
    } else {
        std::cout << "Invalid position [" << row << "][" << col << "] for placing tile." << std::endl;
    }
}

// Get a tile from the board
Tile* GameBoard::getTile(int row, int col) const {
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        return board[row][col];
    }
    std::cout << "Invalid position [" << row << "][" << col << "] for accessing tile." << std::endl;
    return nullptr;
}

// Get the number of rows
int GameBoard::getRows() const {
    return rows;
}

// Get the number of columns
int GameBoard::getCols() const {
    return cols;
}

// Display the board as a string.
std::string GameBoard::displayBoard() const {
    std::string output;
    std::string dashes = "\n--";

    output += "   ";
    for (int col = 0; col < cols; ++col) {
        if (col < 9) {
            output += std::to_string(col) + "  ";
        } else {
            output += std::to_string(col) + " ";
        }
        dashes += "---";
    }
    output += dashes + "\n";

    for (int row = 0; row < rows; ++row) {
        output += std::string(1, 'A' + row) + "|";
        for (int col = 0; col < cols; ++col) {
            if (board[row][col] != nullptr) {
                if (colourDisplayEnabled) {
                    output += board[row][col]->toColoredString() + "|";
                } else {
                    output += ColourUtils::colourToString(board[row][col]->getColour()) + std::to_string(board[row][col]->getShape()) + "|";
                }
            } else {
                output += "  |";
            }
        }
        output += "\n";
    }

    return output;
}


// Check if the board is empty
bool GameBoard::isEmpty() const {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            if (board[row][col] != nullptr) {
                return false;
            }
        }
    }
    return true;
}

// Resize the board
void GameBoard::resize(int newRows, int newCols) {
    std::cout << "Resizing board to " << newRows << "x" << newCols << "." << std::endl;
    board.resize(newRows);
    for (auto& row : board) {
        row.resize(newCols, nullptr);
    }
    rows = newRows;
    cols = newCols;
}

// Enable display of coloured tiles 
void GameBoard::setColourDisplay(bool enabled) {
    colourDisplayEnabled = enabled;
}