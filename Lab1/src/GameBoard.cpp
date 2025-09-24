#include "../include/GameBoard.h"
#include <algorithm>
#include <random>
#include <chrono>

GameBoard::GameBoard() {
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    generator = std::mt19937(seed);
    reset();
}

void GameBoard::reset() {
    // Initialize the board to be 4x4 with all values set to 0
    board = Board(4, std::vector<int>(4, 0));
    std::uniform_int_distribution<int> dist(0, 3);  // [0, 3] uniform distribution
    std::uniform_int_distribution<int> value_dist(0, 1);

    // Add two initial tiles
    for (int i = 0; i < 2; ++i) {
        int x = dist(generator);
        int y = dist(generator);
        while (board[x][y] != 0) { // Ensure we don't place a tile on an occupied cell
            x = dist(generator);
            y = dist(generator);
        }
        board[x][y] = (value_dist(generator) + 1) * 2; // Place either a 2 or a 4
    }
}

int GameBoard::move(Direction direction) {
    // Slide and merge, anyother?
    // TODO 1

    // Add a new tile after every move
    // You can refer to the reset() function to see how to add a new tile
    // TODO 2

}

void GameBoard::slide(Direction direction) {
    switch (direction) {
        case Direction::UP:
            for (int col = 0; col < 4; ++col) {
                int target = 3;
                for (int row = 3; row >= 0; --row) {
                    if (board[row][col] != 0) {
                        std::swap(board[target][col], board[row][col]);
                        if (target != row) board[row][col] = 0;
                        --target;
                    }
                }
            }
            break;
        case Direction::DOWN:
            // TODO
            break;
        case Direction::LEFT:
            // TODO
            break;
        case Direction::RIGHT:
            // TODO
            break;
    }
}

int GameBoard::merge(Direction direction) {
    int moveScore = 0;
    switch (direction) {
        case Direction::UP:
            for (int col = 0; col < 4; ++col) {
                for (int row = 3; row > 0; --row) {
                    if (board[row][col] != 0 && board[row][col] == board[row - 1][col]) {
                        board[row][col] *= 2;
                        moveScore += board[row][col];
                        board[row - 1][col] = 0;
                    }
                }
            }
            break;
        case Direction::DOWN:
            // TODO
            break;
        case Direction::LEFT:
            // TODO
            break;
        case Direction::RIGHT:
            // TODO
            break;
    }

    return moveScore;
}

bool GameBoard::isGameOver() const {
    // Check if there are any empty tiles
    // TODO

    // Check if any adjacent tiles can merge
    // TODO
}

bool GameBoard::hasWinningTile() const {
    // Check if there is a tile with the value 2048
    // TODO
}


const Board &GameBoard::getBoard() const {
    return board;
}