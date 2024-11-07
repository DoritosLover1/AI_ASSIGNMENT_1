#include <iostream>
#include <queue>
#include <stack>
#include <unordered_set>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
#include "Timer.h"
#include "Output.h"

// Structure for storing puzzle configuration
struct Puzzle {
    short int arr[3][3];
};

struct PuzzleHistory {
    Puzzle history;
    short int RowLoc, ColLoc = 0;
    std::string path;

    std::string former() const {
        std::string result;
        for (short int i = 0; i < 3; i++)
            for (short int j = 0; j < 3; j++)
                result += std::to_string(history.arr[i][j]);
        return result;
    }
};

enum SearchType { BFSTYPE = 0, DFSTYPE = 1, DFSLTYPE = 2, IDDFSTYPE = 3 };

Output file("output.txt");
enum SearchType type;
std::vector<int> fringeSize;

Puzzle tarr = {{{0, 1, 2}, {3, 4, 5}, {6, 7, 8}}}; // Target state for comparison
long int limit = 2000;

short int dRow[] = { -1, 0, 1, 0 };
short int dCol[] = { 0, 1, 0, -1 };
std::string moveString = "URDL";
std::string duration;

// Function prototypes
bool isValidRowAndCol(int row, int col);
bool isCorrectSolution(const Puzzle& puzzle);
void printGrid(const Puzzle& puzzle, int& state);
bool isSolvableArr(const Puzzle& puzzle);
static void endOfAlgorithm(SearchType type, PuzzleHistory* current, std::vector<int>* fringeSize, int& state, std::string& duration);

// BFS function using Puzzle struct
void BFS(const Puzzle& puzzle, short int row, short int col) {
    Timer* time = new Timer();
    type = BFSTYPE;
    std::queue<PuzzleHistory> puzzleHistoryQueue;
    std::unordered_set<std::string> visitedSet;

    PuzzleHistory firstState = { puzzle, row, col, "" };
    puzzleHistoryQueue.push(firstState);
    visitedSet.insert(firstState.former());

    int iterationCount = 0;
    const int printInterval = 6000;

    while (!puzzleHistoryQueue.empty()) {
        PuzzleHistory current = puzzleHistoryQueue.front();
        puzzleHistoryQueue.pop();

        fringeSize.push_back(puzzleHistoryQueue.size());
        if (isCorrectSolution(current.history)) {
            duration = time->endTimer();
            endOfAlgorithm(type, &current, &fringeSize, iterationCount, duration);
            printGrid(current.history, iterationCount);
            delete time;
            return;
        }

        for (int i = 0; i < 4; i++) {
            int newRow = current.RowLoc + dRow[i];
            int newCol = current.ColLoc + dCol[i];

            if (isValidRowAndCol(newRow, newCol)) {
                PuzzleHistory nextCurrent = current;
                std::swap(nextCurrent.history.arr[current.RowLoc][current.ColLoc],
                          nextCurrent.history.arr[newRow][newCol]);

                nextCurrent.RowLoc = newRow;
                nextCurrent.ColLoc = newCol;
                nextCurrent.path += moveString[i];

                std::string formedString = nextCurrent.former();
                if (visitedSet.find(formedString) == visitedSet.end()) {
                    puzzleHistoryQueue.push(nextCurrent);
                    visitedSet.insert(formedString);
                }
            }
        }
        if (++iterationCount % printInterval == 0) {
            std::cout << "Stage: " << iterationCount << "\n";
            printGrid(current.history, iterationCount);
        }
    }
    std::cout << "No Solution Found via BFS. \n";
    delete time;
}
void DFS(const Puzzle& puzzle, short int row, short int col) {
    Timer* time = new Timer();
    type = DFSTYPE;
    std::stack<PuzzleHistory> puzzleHistoryStack;
    std::unordered_set<std::string> visitedSet;

    PuzzleHistory firstState = { puzzle, row, col, "" };
    puzzleHistoryStack.push(firstState);
    visitedSet.insert(firstState.former());

    int iterationCount = 0;
    const int printInterval = 6000;

    while (!puzzleHistoryStack.empty()) {
        PuzzleHistory current = puzzleHistoryStack.top();
        puzzleHistoryStack.pop();

        fringeSize.push_back(puzzleHistoryStack.size());
        if (isCorrectSolution(current.history)) {
            duration = time->endTimer();
            endOfAlgorithm(type, &current, &fringeSize, iterationCount, duration);
            delete time;
            return;
        }

        for (int i = 0; i < 4; i++) {
            int newRow = current.RowLoc + dRow[i];
            int newCol = current.ColLoc + dCol[i];

            if (isValidRowAndCol(newRow, newCol)) {
                PuzzleHistory nextCurrent = current;
                std::swap(nextCurrent.history.arr[current.RowLoc][current.ColLoc],
                    nextCurrent.history.arr[newRow][newCol]);

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
            printGrid(current.history, iterationCount);
        }
    }
    std::cout << "No Solution Found via DFS.\n";
    delete time;
}
void DFSL(const Puzzle& puzzle, short int row, short int col, long int limit) {
    Timer* time = new Timer();
    type = DFSLTYPE;
    std::stack<PuzzleHistory> puzzleHistoryStack;
    std::unordered_set<std::string> visitedSet;

    PuzzleHistory firstState = { puzzle, row, col, "" };
    puzzleHistoryStack.push(firstState);
    visitedSet.insert(firstState.former());

    int iterationCount = 0;
    const int printInterval = 6000;

    while (!puzzleHistoryStack.empty() && limit > 0) {
        PuzzleHistory current = puzzleHistoryStack.top();
        puzzleHistoryStack.pop();
        --limit;

        fringeSize.push_back(puzzleHistoryStack.size());
        if (isCorrectSolution(current.history)) {
            duration = time->endTimer();
            endOfAlgorithm(type, &current, &fringeSize, iterationCount, duration);
            delete time;
            return;
        }

        for (int i = 0; i < 4; i++) {
            int newRow = current.RowLoc + dRow[i];
            int newCol = current.ColLoc + dCol[i];

            if (isValidRowAndCol(newRow, newCol)) {
                PuzzleHistory nextCurrent = current;
                std::swap(nextCurrent.history.arr[current.RowLoc][current.ColLoc],
                    nextCurrent.history.arr[newRow][newCol]);

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
            printGrid(current.history, iterationCount);
        }
    }
    std::cout << "No Solution Found via DFSL.\n";
    delete time;
}
void IDDFS(const Puzzle& puzzle, short int row, short int col, long int depth_limit) {
    Timer* time = new Timer();
    type = IDDFSTYPE;
    std::queue<PuzzleHistory> puzzleHistoryQueue;
    std::unordered_set<std::string> visitedSet;

    PuzzleHistory firstState = { puzzle, row, col, "" };
    puzzleHistoryQueue.push(firstState);
    visitedSet.insert(firstState.former());

    int iterationCount = 0;
    const int printInterval = 6000;
    int depth = 0;

    while (!puzzleHistoryQueue.empty() && depth <= depth_limit) {
        PuzzleHistory current = puzzleHistoryQueue.front();
        puzzleHistoryQueue.pop();

        fringeSize.push_back(puzzleHistoryQueue.size());
        if (isCorrectSolution(current.history)) {
            duration = time->endTimer();
            endOfAlgorithm(type, &current, &fringeSize, iterationCount, duration);
            delete time;
            return;
        }

        for (int i = 0; i < 4; i++) {
            int newRow = current.RowLoc + dRow[i];
            int newCol = current.ColLoc + dCol[i];

            if (isValidRowAndCol(newRow, newCol)) {
                PuzzleHistory nextCurrent = current;
                std::swap(nextCurrent.history.arr[current.RowLoc][current.ColLoc],
                    nextCurrent.history.arr[newRow][newCol]);

                nextCurrent.RowLoc = newRow;
                nextCurrent.ColLoc = newCol;
                nextCurrent.path += moveString[i];

                if (visitedSet.find(nextCurrent.former()) == visitedSet.end()) {
                    puzzleHistoryQueue.push(nextCurrent);
                    visitedSet.insert(nextCurrent.former());
                }
            }
        }

        ++depth;
        if (++iterationCount % printInterval == 0) {
            std::cout << "Stage: " << iterationCount << "\n";
            printGrid(current.history, iterationCount);
        }
    }
    std::cout << "No Solution Found via IDDFS.\n";
    delete time;
}

int main() {
    std::vector<Puzzle> listOfArray = {
        {{{2, 1, 0}, {3, 5, 8}, {6, 4, 7}}},
        {{{1, 2, 0}, {3, 6, 5}, {4, 8, 7}}},
        {{{1, 0, 2}, {5, 6, 3}, {4, 8, 7}}},
        {{{6, 8, 2}, {3, 4, 7}, {0, 1, 5}}},
        {{{7, 0, 8}, {3, 2, 5}, {4, 1, 6}}},
        {{{3, 2, 4}, {1, 6, 7}, {0, 8, 5}}},
        {{{1, 6, 4}, {3, 7, 2}, {5, 8, 0}}},
        {{{4, 6, 8}, {0, 7, 2}, {5, 3, 1}}},
        {{{0, 6, 7}, {4, 8, 5}, {1, 3, 2}}},
        {{{4, 1, 8}, {0, 7, 5}, {2, 3, 6}}}
    };

    for (const Puzzle &puzzle : listOfArray) {
        short int zeroRowLoc = 0, zeroColLoc = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (puzzle.arr[i][j] == 0) {
                    zeroRowLoc = i;
                    zeroColLoc = j;
                }
            }
        }
        if (isSolvableArr(puzzle)) {
            std::cout << "Puzzle is solvable, running BFS:\n";
            BFS(puzzle, zeroRowLoc, zeroColLoc);
        } else {
            std::cout << "Puzzle is not solvable, skipping.\n";
        }
    }
    for (const Puzzle& puzzle : listOfArray) {
        short int zeroRowLoc = 0, zeroColLoc = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (puzzle.arr[i][j] == 0) {
                    zeroRowLoc = i;
                    zeroColLoc = j;
                }
            }
        }
        if (isSolvableArr(puzzle)) {
            std::cout << "Puzzle is solvable, running DFS:\n";
            DFS(puzzle, zeroRowLoc, zeroColLoc);
        }
        else {
            std::cout << "Puzzle is not solvable, skipping.\n";
        }
    }
    for (const Puzzle& puzzle : listOfArray) {
        short int zeroRowLoc = 0, zeroColLoc = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (puzzle.arr[i][j] == 0) {
                    zeroRowLoc = i;
                    zeroColLoc = j;
                }
            }
        }
        if (isSolvableArr(puzzle)) {
            std::cout << "Puzzle is solvable, running DFSL:\n";
            DFSL(puzzle, zeroRowLoc, zeroColLoc, 20);
        }
        else {
            std::cout << "Puzzle is not solvable, skipping.\n";
        }
    }
    for (const Puzzle& puzzle : listOfArray) {
        short int zeroRowLoc = 0, zeroColLoc = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (puzzle.arr[i][j] == 0) {
                    zeroRowLoc = i;
                    zeroColLoc = j;
                }
            }
        }
        if (isSolvableArr(puzzle)) {
            std::cout << "Puzzle is solvable, running IDDFS:\n";
            IDDFS(puzzle, zeroRowLoc, zeroColLoc, 1000);
        }
        else {
            std::cout << "Puzzle is not solvable, skipping.\n";
        }
    }
    return 0;
}

bool isValidRowAndCol(int row, int col) {
    return (row >= 0 && row < 3 && col >= 0 && col < 3);
}

bool isCorrectSolution(const Puzzle& puzzle) {
    for (short int i = 0; i < 3; i++)
        for (short int j = 0; j < 3; j++)
            if (puzzle.arr[i][j] != tarr.arr[i][j])
                return false;
    return true;
}

void printGrid(const Puzzle& puzzle, int& state) {
    std::string str;
    str += "State: " + std::to_string(state) + "\n";
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            str += std::to_string(puzzle.arr[i][j]) + " ";
            std::cout << puzzle.arr[i][j] << " ";
        }
        str += "\n";
        std::cout << std::endl;
    }
    str += "------------------\n";
    std::cout << "------------------" << std::endl;
    file.writeTheFile(str);
}

bool isSolvableArr(const Puzzle& puzzle) {
    std::vector<short int> set;
    int solvablility = 0;

    for (short int i = 0; i < 3; i++) {
        for (short int j = 0; j < 3; j++) {
            set.push_back(puzzle.arr[i][j]);
        }
    }
    for (short int i = 0; i < set.size(); i++) {
        for (short int j = i + 1; j < set.size(); j++) {
            if (set[i] > set[j] && set[j] != 0) ++solvablility;
        }
    }

    return (solvablility % 2 == 0);
}

static void endOfAlgorithm(SearchType type, PuzzleHistory* current, std::vector<int>* fringeSize, int& state, std::string& duration) {
    std::string str;

    switch (type) {
    case BFSTYPE:
        str += "BFS Solution Found:\n";
        std::cout << str << "\n";
        break;
    case DFSTYPE:
        str += "DFS Solution Found:\n";
        std::cout << str << "\n";
        break;
    case DFSLTYPE:
        str += "DFSL Solution Found:\n";
        std::cout << str << "\n";
        break;
    case IDDFSTYPE:
        str += "IDDFS Solution Found:\n";
        std::cout << str << "\n";
        break;
    default:
        str += "Unknown Solution Found:\n";
        std::cout << str << "\n";
        break;
    }

    int maxNumOfFringe = *std::max_element(fringeSize->begin(), fringeSize->end());
    str += "Max size of Fringe: " + std::to_string(maxNumOfFringe) + "\n";
    str += "Taken time: " + duration;

    std::cout << "Max size of Fringe: " << maxNumOfFringe << "\n";
    std::cout << "------------------" << std::endl;

    file.writeTheFile(str);
    fringeSize->clear();
}