#pragma once

#include <string>
#include "Timer.h"
#include "Output.h"
#include <queue>
#include <unordered_set>
enum SearchType { BFSTYPE = 0, DFSTYPE = 1, DFSLTYPE = 2, IDDFSTYPE = 3 };

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
class Agent
{
public:
	Agent() {

	}
	~Agent() {

	}

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
};

