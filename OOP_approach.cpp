#include <bits/stdc++.h>
using namespace std;

class Board {
private:
    int Side; // side length of the board
    int Mines; // number of mines on the board
    char realBoard[25][25];
    char myBoard[25][25];
    int mineLocations[99][2];

public:
    Board(int side, int mines) : Side(side), Mines(mines) {
        initialiseBoards();
        placeMines();
    }

    int getSide() const { return Side; } // Getter for Side
    int getMines() const { return Mines; } // Getter for Mines
    char getRealCell(int row, int col) const { return realBoard[row][col]; }
    char getMyCell(int row, int col) const { return myBoard[row][col]; }
    void setMyCell(int row, int col, char value) { myBoard[row][col] = value; }
    void setRealCell(int row, int col, char value) { realBoard[row][col] = value; }

    void initialiseBoards() {
        for (int i = 0; i < Side; i++) {
            for (int j = 0; j < Side; j++) {
                realBoard[i][j] = myBoard[i][j] = '-';
            }
        }
    }

    void placeMines() {
        bool mark[625]; // 25 * 25 = 625 max cells
        memset(mark, false, sizeof(mark));

        for (int i = 0; i < Mines; ) {
            int random = rand() % (Side * Side);
            int x = random / Side;
            int y = random % Side;

            if (!mark[random]) {
                mineLocations[i][0] = x;
                mineLocations[i][1] = y;
                realBoard[x][y] = '*';
                mark[random] = true;
                i++;
            }
        }
    }

    void printBoard(bool showMines = false) const {
        printf(" ");
        for (int i = 0; i < Side; i++)
            printf("%d ", i);
        printf("\n\n");

        for (int i = 0; i < Side; i++) {
            printf("%d ", i);
            for (int j = 0; j < Side; j++) {
                if (showMines)
                    printf("%c ", realBoard[i][j]);
                else
                    printf("%c ", myBoard[i][j]);
            }
            printf("\n");
        }
    }

    bool isCellValid(int row, int col) const {
        return (row >= 0) && (row < Side) && (col >= 0) && (col < Side);
    }

    bool isMine(int row, int col) const {
        return realBoard[row][col] == '*';
    }

    int countAdjacentMines(int row, int col) const {
        int count = 0;
        int directions[8][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}, {-1, 1}, {-1, -1}, {1, 1}, {1, -1}};
        for (auto dir : directions) {
            int newRow = row + dir[0];
            int newCol = col + dir[1];
            if (isCellValid(newRow, newCol) && isMine(newRow, newCol))
                count++;
        }
        return count;
    }

    int* getMineLocation(int index) {
        return mineLocations[index];
    }
};

class Game {
private:
    Board *board;
    int movesLeft;
    bool gameOver;

public:
    Game(Board *b) : board(b), gameOver(false), movesLeft(b->getSide() * b->getSide() - b->getMines()) {}

    void play() {
        int currentMoveIndex = 0;
        while (!gameOver) {
            printf("Current Board Status:\n");
            board->printBoard();

            int x, y;
            makeMove(&x, &y);

            if (currentMoveIndex == 0 && board->isMine(x, y)) {
                replaceMine(x, y);
            }

            currentMoveIndex++;
            gameOver = playMove(x, y);

            if (!gameOver && movesLeft == 0) {
                printf("You won!\n");
                gameOver = true;
            }
        }
    }

    void makeMove(int *x, int *y) const {
        printf("Enter your move (row, col): ");
        scanf("%d %d", x, y);
    }

    void replaceMine(int row, int col) {
        for (int i = 0; i < board->getSide(); i++) {
            for (int j = 0; j < board->getSide(); j++) {
                if (!board->isMine(i, j)) {
                    board->setRealCell(i, j, '*');
                    board->setRealCell(row, col, '-');
                    return;
                }
            }
        }
    }

    bool playMove(int row, int col) {
        if (board->getMyCell(row, col) != '-') return false;

        if (board->isMine(row, col)) {
            board->setMyCell(row, col, '*');
            for (int i = 0; i < board->getMines(); i++) {
                int *loc = board->getMineLocation(i);
                board->setMyCell(loc[0], loc[1], '*');
            }
            board->printBoard(true);
            printf("You lost!\n");
            return true;
        }

        int mineCount = board->countAdjacentMines(row, col);
        movesLeft--;
        board->setMyCell(row, col, mineCount + '0');

        if (mineCount == 0) {
            int directions[8][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}, {-1, 1}, {-1, -1}, {1, 1}, {1, -1}};
            for (auto dir : directions) {
                int newRow = row + dir[0];
                int newCol = col + dir[1];
                if (board->isCellValid(newRow, newCol) && !board->isMine(newRow, newCol)) {
                    playMove(newRow, newCol);
                }
            }
        }

        return false;
    }
};

int main() {
    int level;
    printf("Enter the Difficulty Level\n");
    printf("Press 0 for BEGINNER (9x9 cells and 10 mines)\n");
    printf("Press 1 for INTERMEDIATE (16x16 cells and 40 mines)\n");
    printf("Press 2 for ADVANCED (24x24 cells and 99 mines)\n");
    scanf("%d", &level);

    int side, mines;
    if (level == 0) {
        side = 9;
        mines = 10;
    } else if (level == 1) {
        side = 16;
        mines = 40;
    } else {
        side = 24;
        mines = 99;
    }

    Board b(side, mines);
    Game g(&b);
    g.play();

    return 0;
}
