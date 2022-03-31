#ifndef INC_1C_CHECKER_CHECKERS_H
#define INC_1C_CHECKER_CHECKERS_H

#include <iostream>
#include <utility>
#include <vector>
#include <queue>

enum Colors {
    EMPTY, WHITE, BLACK
};

enum Players {
    Human = -1, AI = 1
};

enum MinMax {
    MIN_VAL = -5, MAX_VAL = 5
};

class Cell {

public:
    Cell(int coord_x, int coord_y) : coords_(coord_x, coord_y) {}

    Cell(int coord_x, int coord_y, int state) : Cell(coord_x, coord_y) {
        state_ = state;
    }

    Cell &operator=(const Cell &cell);

    std::pair<int, int> GetCoords();

    int GetState() const;

    void SetState(int state);

    void MakeQueen();

    bool CheckQueen();

    int GetEstimation() const;

    void SetEstimation(int est);

private:
    bool is_queen_ = false;
    int state_ = EMPTY;
    int estimation_ = 0;
    std::pair<int, int> coords_;

};

class Desk {
public:
    Desk(size_t height, size_t width);

    Desk(Desk *desk);

    std::vector<std::vector<Cell>> GetAvaiableSteps(Cell &cell);

    void PrintDesk();

    size_t GetHeight();

    size_t GetWidth();

    Cell &GetCell(int coord_x, int coord_y);

    void SetCell(Cell &cell, int coord_x, int coord_y);

private:
    std::vector<std::vector<Cell>> desk_;

    bool InDesk(size_t coord_x, size_t coord_y);

    void GetStepsQueen(Cell &cell, int color, int enemy_color, std::vector<Cell> &steps, std::vector<Cell> &hit_cells);

    void GetStepsCheck(Cell &cell, int color, int enemy_color, std::vector<Cell> &steps, std::vector<Cell> &hit_cells);
};


class Player {
public:
    Player(int number_of_checkers, int colour) :
            number_of_checkers_(number_of_checkers), colour_(colour) {};

    void MakeStep(std::pair<int, int> from, std::pair<int, int> to, Desk *desk);

    void MakeStepWithKill(std::pair<int, int> from, std::pair<int, int> to, Desk *desk);

private:
    int number_of_checkers_;
    int colour_;
};

class HumanPlayer : public Player {
public:
    HumanPlayer(int number_of_checkers, int colour) :
            Player(number_of_checkers, colour) {}

private:
    std::vector<Cell> steps_;
    std::vector<Cell> hit_steps_;
    std::vector<Cell> checkers_;
};

class AIPlayer : public Player {
public:
    AIPlayer(int number_of_checkers, int colour) :
            Player(number_of_checkers, colour) {};

    std::vector<std::pair<int, int>> Predict(Desk *desk);

private:
    int AnalyzeCell(std::pair<int, int> from, Desk *desk);

    int Minimax(std::pair<int, int> from, int recursivw_lvl, Desk *desk, int player);

    const int max_recursion_depth_ = 5;
    std::vector<std::pair<Cell, std::vector<std::vector<Cell>>>> available_steps;
};

class Game {
public:
    Game(HumanPlayer humanPlayer, AIPlayer aiPlayer, Desk desk) :
            humanPlayer_(std::move(humanPlayer)),
            aiPlayer_(std::move(aiPlayer)),
            desk_(std::move(desk)) {};

    void MakeHumanStep(std::pair<int, int> from, std::pair<int, int> to);

    void MakeAiStep(std::pair<int, int> from, std::pair<int, int> to);

    void Start();

private:

    HumanPlayer humanPlayer_;
    AIPlayer aiPlayer_;
    Desk desk_;
};

#endif //INC_1C_CHECKER_CHECKERS_H
