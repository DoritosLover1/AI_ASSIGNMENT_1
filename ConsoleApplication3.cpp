#include <iostream>  
#include <queue>  
#include <cstring>
#include <string>
#include <unordered_set>
#include <stack>
#include "Timer.h"
#include "Output.h"

/*For selection about which search is that - just about select correct print -*/
enum SearchType {
    BFSTYPE = 0,
    DFSTYPE = 1,
    DFSLTYPE = 2,
    IDDFSTYPE = 3
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
long int limit = 2000;

/*x-axis and y-axis AI directions*/
short int dRow[] = { -1, 0, 1, 0 };
short int dCol[] = { 0, 1, 0, -1 };

/*For making moving of ai*/
std::string moveString = "URDL";
std::string duration;

/*Some implentations about corrections and printing*/
bool isValidRowAndCol(int row, int col);
bool isCorrectSolution(short int grid[3][3]);
void printGrid(short int grid[3][3], int& state);

static void endOfAlgorithm(SearchType type,PuzzleHistory* current, std::vector<int>* fringeSize, int& state, std::string& duration);

void BFS(short int grid[3][3], short int row, short int col) {
    Timer* time = new Timer();
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
            duration = time->endTimer();
            endOfAlgorithm(type, &current, &fringeSize, iterationCount, duration);
            printGrid(current.history, iterationCount);
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
            printGrid(current.history, iterationCount);
        }
    }
    std::cout << "No Solution Found via BFS. \n";
    delete time;
}
void DFS(short int grid[3][3], short int row, short int col) {
    Timer* time = new Timer();
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
            duration = time->endTimer();
            endOfAlgorithm(type, &current, &fringeSize, iterationCount, duration);
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
            printGrid(current.history, iterationCount);
        }
    }
    std::cout << "No Solution Found via DFS.\n";
    delete time;
}
void DFSL(short int grid[3][3], short int row, short int col, long int limit) {
    Timer* time = new Timer();
    type = DFSLTYPE;
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
            for (int i = 0; i < limit; --limit) {
                PuzzleHistory current = puzzleHistoryStack.top();
                puzzleHistoryStack.pop();
                fringeSize.push_back(puzzleHistoryStack.size());
                if (isCorrectSolution(current.history)) {
                    duration = time->endTimer();
                    endOfAlgorithm(type, &current, &fringeSize, iterationCount, duration);
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
        }
    }
    delete time;
    std::cout << "No Solution Found via DFSL.\n";
}
void IDDFS(short int grid[3][3], short int row, short int col, long int depth_limit) {
    Timer* time = new Timer();
    type = IDDFSTYPE;
    std::queue<PuzzleHistory> puzzleHistoryQueue;
    std::unordered_set<std::string> visitedSet;

    PuzzleHistory firstState = { {}, row, col, "" };
    std::memcpy(firstState.history, grid, sizeof(firstState.history));
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
        ++depth;
        if (++iterationCount % printInterval == 0) {
            std::cout << "Stage: " << iterationCount << "\n";
            printGrid(current.history, iterationCount);
        }
    }
    delete time;
    std::cout << "No Solution Found via IDDFS. \n";
    /*
    std::vector<int> fringeSize;

    for (int depth = 0; depth < depth_limit; ++depth) {
        std::unordered_set<std::string> visitedSet;
        PuzzleHistory firstState = { {}, row, col, "" };
        std::memcpy(firstState.history, grid, sizeof(firstState.history));

        if (DLS(firstState, 0, depth_limit, visitedSet)) {
            std::cout << "Solution Found via IDDFS. \n";
            return;
        }
    }
    std::cout << "No Solution Found via IDDFS. \n";
    */
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
    
    BFS(arr, zeroRowLoc, zeroColLoc);
    /*
    Timer* time1 = new Timer();
    DFS(arr, zeroRowLoc, zeroColLoc);
    time1->endTimer();
    
    Timer* time2 = new Timer();
    DFSL(arr, zeroRowLoc, zeroColLoc, 20);
    time2->endTimer();
    
    Timer* time3 = new Timer();
    IDDFS(arr, zeroRowLoc, zeroColLoc, 200);
    time3->endTimer();
    */
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
void printGrid(short int grid[3][3], int& state) {
    std::string str;
    str += "State: " + std::to_string(state) + "\n";
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