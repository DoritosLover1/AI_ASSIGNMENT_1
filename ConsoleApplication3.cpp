#include <iostream>  
#include <queue>  
#include <cstring>
#include <string>
#include <unordered_set>
#include <stack>
#include "Timer.h"
#include "Output.h"

//For selection about which search is that -just about select correct print-
enum SearchType {
    BFSTYPE = 0,
    DFSTYPE = 1
};

/*Kinda storage for avoiding reputation*/
struct PuzzleHistory {
    short int history[3][3];
    short int RowLoc, ColLoc = 0;
    std::string path;

    std::string former() const {
        std::string result;
        for (short int i = 0; i < 3; i++)
            for (short int j = 0; j < 3; j++)
                result += std::to_string(history[i][j]);
        return result;
    }
};

Output file("output.txt");

enum SearchType type;
std::vector<int> fringeSize;
short int tarr[3][3] = { {0, 1, 2}, {3, 4, 5}, {6, 7, 8} };
int limit = 20;

/*x-axis and y-axis AI directions*/
short int dRow[] = { -1, 0, 1, 0 };
short int dCol[] = { 0, 1, 0, -1 };

/*For making moving of ai*/
std::string moveString = "URDL";

/*Some implentations about corrections and printing*/
bool isValidRowAndCol(int row, int col);
bool isCorrectSolution(short int grid[3][3]);
void printGrid(short int grid[3][3]);
void endOfAlgorithm(SearchType type,PuzzleHistory* current, std::vector<int>* fringeSize);

void BFS(short int grid[3][3], short int row, short int col) {
    type = BFSTYPE;
    std::queue<PuzzleHistory> puzzleHistoryQueue;
    std::unordered_set<std::string> visitedSet;

    PuzzleHistory firstState = { {}, row, col, "" };
    std::memcpy(firstState.history, grid, sizeof(firstState.history));
    puzzleHistoryQueue.push(firstState);
    visitedSet.insert(firstState.former());

    int iterationCount = 0;
    const int printInterval = 6000;

    while (!puzzleHistoryQueue.empty()) {
        PuzzleHistory current = puzzleHistoryQueue.front();
        puzzleHistoryQueue.pop();

        fringeSize.push_back(puzzleHistoryQueue.size());
        if (isCorrectSolution(current.history)) {
            endOfAlgorithm(BFSTYPE, &current, &fringeSize);
            return;
        }

        for (int i = 0; i < 4; i++) {
            int newRow = current.RowLoc + dRow[i];
            int newCol = current.ColLoc + dCol[i];

            if (isValidRowAndCol(newRow, newCol)) {
                PuzzleHistory nextCurrent = current;
                std::swap(nextCurrent.history[current.RowLoc][current.ColLoc],
                    nextCurrent.history[newRow][newCol]);
                nextCurrent.RowLoc = newRow;
                nextCurrent.ColLoc = newCol;

                nextCurrent.path += moveString[i];

                if (visitedSet.find(nextCurrent.former()) == visitedSet.end()) {
                    puzzleHistoryQueue.push(nextCurrent);
                    visitedSet.insert(nextCurrent.former());
                }
            }
        }

        if (++iterationCount % printInterval == 0) {
            std::cout << "Stage: " << iterationCount << "\n";
            printGrid(current.history);
        }

    }
    std::cout << "No Solution Found via BFS. \n";
}

void DFS(short int grid[3][3], short int row, short int col) {
    type = DFSTYPE;
    std::stack<PuzzleHistory> puzzleHistoryStack;
    std::unordered_set<std::string> visitedSet;

    PuzzleHistory firstState = { {}, row, col, "" };
    std::memcpy(firstState.history, grid, sizeof(firstState.history));
    puzzleHistoryStack.push(firstState);
    visitedSet.insert(firstState.former());

    int iterationCount = 0;
    const int printInterval = 6000;

    while (!puzzleHistoryStack.empty()) {
        PuzzleHistory current = puzzleHistoryStack.top();
        puzzleHistoryStack.pop();

        fringeSize.push_back(puzzleHistoryStack.size());
        if (isCorrectSolution(current.history)) {
            endOfAlgorithm(DFSTYPE, &current, &fringeSize);
            return;
        }

        // Explore all possible moves in each direction
        for (int i = 0; i < 4; i++) {
            int newRow = current.RowLoc + dRow[i];
            int newCol = current.ColLoc + dCol[i];

            if (isValidRowAndCol(newRow, newCol)) {
                PuzzleHistory nextCurrent = current;
                std::swap(nextCurrent.history[current.RowLoc][current.ColLoc],
                    nextCurrent.history[newRow][newCol]);

                nextCurrent.RowLoc = newRow;
                nextCurrent.ColLoc = newCol;
                nextCurrent.path += moveString[i];

                if (visitedSet.find(nextCurrent.former()) == visitedSet.end()) {
                    puzzleHistoryStack.push(nextCurrent);
                    visitedSet.insert(nextCurrent.former());
                }
            }
        }

        if (++iterationCount % printInterval == 0) {
            std::cout << "Stage: " << iterationCount << "\n";
            printGrid(current.history);
        }
    }

    std::cout << "No Solution Found via DFS.\n";
}

// 04.11.2024 BURASI YAPILACAK.
void DFSL(short int grid[3][3], short int row, short int col, short int limit) {
    type = DFSTYPE;
    std::stack<PuzzleHistory> puzzleHistoryStack;
    std::unordered_set<std::string> visitedSet;

    PuzzleHistory firstState = { {}, row, col, "" };
    std::memcpy(firstState.history, grid, sizeof(firstState.history));
    puzzleHistoryStack.push(firstState);
    visitedSet.insert(firstState.former());

    int iterationCount = 0;
    const int printInterval = 6000;

    while (!puzzleHistoryStack.empty()) {
        if (limit == 0) {
            break;
        }
        else {
            for (int i = 0; i < limit; i++) {
                PuzzleHistory current = puzzleHistoryStack.top();
                puzzleHistoryStack.pop();
                fringeSize.push_back(puzzleHistoryStack.size());
                if (isCorrectSolution(current.history)) {
                    endOfAlgorithm(DFSTYPE, &current, &fringeSize);
                    return;
                }

                // Explore all possible moves in each direction
                for (int i = 0; i < 4; i++) {
                    int newRow = current.RowLoc + dRow[i];
                    int newCol = current.ColLoc + dCol[i];

                    if (isValidRowAndCol(newRow, newCol)) {
                        PuzzleHistory nextCurrent = current;
                        std::swap(nextCurrent.history[current.RowLoc][current.ColLoc],
                            nextCurrent.history[newRow][newCol]);

                        nextCurrent.RowLoc = newRow;
                        nextCurrent.ColLoc = newCol;
                        nextCurrent.path += moveString[i];

                        if (visitedSet.find(nextCurrent.former()) == visitedSet.end()) {
                            puzzleHistoryStack.push(nextCurrent);
                            visitedSet.insert(nextCurrent.former());
                        }
                    }
                }

                if (++iterationCount % printInterval == 0) {
                    std::cout << "Stage: " << iterationCount << "\n";
                    printGrid(current.history);
                }
            }
        }
    }

    std::cout << "No Solution Found via DFS.\n";
}

int main() {
    short int arr[3][3] = { {1, 2, 4}, {0, 5, 7}, {8, 3, 6} };
    short int zeroRowLoc, zeroColLoc = 0;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (arr[i][j] == 0) {
                zeroRowLoc = i;
                zeroColLoc = j;
            }
    /*
    Timer* time = new Timer();
    BFS(arr, zeroRowLoc, zeroColLoc);
    time->endTimer();
    */

    /*
    Timer* time1 = new Timer();
    DFS(arr, zeroRowLoc, zeroColLoc);
    time1->endTimer();
    */

    Timer* time2 = new Timer();
    DFSL(arr, zeroRowLoc, zeroColLoc, 20);
    time2->endTimer();

    return 0;
}

bool isValidRowAndCol(int row, int col) {
    return (row >= 0 && row < 3 && col >= 0 && col < 3);
}

bool isCorrectSolution(short int grid[3][3]) {
    for (short int i = 0; i < 3; i++)
        for (short int j = 0; j < 3; j++)
            if (grid[i][j] != tarr[i][j])
                return false;
    return true;
}

void printGrid(short int grid[3][3]) {
    std::string str;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            str += std::to_string(grid[i][j]) + " ";
            std::cout << grid[i][j] << " ";
        }
        str += "\n";
        std::cout << std::endl;
    }
    str += "------------------\n";
    std::cout << "------------------" << std::endl;
    file.writeTheFile(str);
}

void endOfAlgorithm(SearchType type,PuzzleHistory* current, std::vector<int>* fringeSize) {
    switch (type)
    {
    case 0:
        std::cout << "BFS Solution Found:\n";
        break;
    case 1:
        std::cout << "DFS Solution Found:\n";
        break;
    default:
        break;
    }
    printGrid(current->history);
    //std::cout << "Path to solution: " << current.path << std::endl;
    int maxNumofFringe = *std::max_element(fringeSize->begin(), fringeSize->end());
    std::cout << "Max size of Fringe: " << maxNumofFringe << "\n";
    fringeSize->clear();
}