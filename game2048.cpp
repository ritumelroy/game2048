#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

class Game2048 {
public:
    Game2048() : grid(4, std::vector<int>(4, 0)), score(0) {
        std::srand(std::time(0));
        addRandomTile();
        addRandomTile();
    }

    void play() {
        char move;
        while (true) {
            printGrid();
            std::cout << "Score: " << score << std::endl;
            std::cout << "Enter move (WASD): ";
            std::cin >> move;

            if (!makeMove(move)) {
                std::cout << "Invalid move. Try again." << std::endl;
                continue;
            }

            addRandomTile();
            if (isGameOver()) {
                printGrid();
                std::cout << "Game Over! Final score: " << score << std::endl;
                break;
            }
        }
    }

private:
    std::vector<std::vector<int>> grid;
    int score;

    void addRandomTile() {
        std::vector<std::pair<int, int>> emptyTiles;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (grid[i][j] == 0) {
                    emptyTiles.emplace_back(i, j);
                }
            }
        }

        if (!emptyTiles.empty()) {
            auto [x, y] = emptyTiles[std::rand() % emptyTiles.size()];
            grid[x][y] = (std::rand() % 10 < 9) ? 2 : 4;
        }
    }

    bool makeMove(char move) {
        switch (std::tolower(move)) {
            case 'w': return moveUp();
            case 's': return moveDown();
            case 'a': return moveLeft();
            case 'd': return moveRight();
            default: return false;
        }
    }

    bool moveUp() {
        return slideAndCombine([this](int i, int j) { return std::make_pair(i, j); });
    }

    bool moveDown() {
        return slideAndCombine([this](int i, int j) { return std::make_pair(3 - i, j); });
    }

    bool moveLeft() {
        return slideAndCombine([this](int i, int j) { return std::make_pair(j, i); });
    }

    bool moveRight() {
        return slideAndCombine([this](int i, int j) { return std::make_pair(j, 3 - i); });
    }

template<typename CoordFunc>
bool slideAndCombine(CoordFunc coord) {
    bool moved = false;
    for (int x = 0; x < 4; ++x) {
        std::vector<int> line;
        for (int y = 0; y < 4; ++y) {
            auto [i, j] = coord(x, y);
            if (i >= 0 && i < 4 && j >= 0 && j < 4 && grid[i][j] != 0) {
                line.push_back(grid[i][j]);
            }
        }

        // Combine adjacent tiles
        for (int k = 0; k < static_cast<int>(line.size()) - 1; ++k) {
            if (line[k] == line[k + 1]) {
                line[k] *= 2;
                score += line[k];
                line[k + 1] = 0; // Mark the next element for removal
                moved = true;
            }
        }

        line.erase(std::remove(line.begin(), line.end(), 0), line.end());

        for (int y = 0; y < 4; ++y) {
            auto [i, j] = coord(x, y);
            if (i >= 0 && i < 4 && j >= 0 && j < 4) {
                int newValue = y < static_cast<int>(line.size()) ? line[y] : 0;
                if (grid[i][j] != newValue) {
                    grid[i][j] = newValue;
                    moved = true;
                }
            }
        }
    }
    return moved;
}

    bool isGameOver() const {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (grid[i][j] == 0) return false;
                if (i > 0 && grid[i][j] == grid[i - 1][j]) return false;
                if (i < 3 && grid[i][j] == grid[i + 1][j]) return false;
                if (j > 0 && grid[i][j] == grid[i][j - 1]) return false;
                if (j < 3 && grid[i][j] == grid[i][j + 1]) return false;
            }
        }
        return true;
    }

    void printGrid() const {
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                std::cout << std::setw(5) << cell;
            }
            std::cout << std::endl;
        }
    }
};

int main() {
    Game2048 game;
    game.play();
    return 0;
}

