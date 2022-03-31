#include "checkers.h"

std::pair<int, int> Cell::GetCoords() {
    return coords_;
}

int Cell::GetState() const {
    return state_;
}

void Cell::SetState(int state) {
    state_ = state;
}

void Cell::MakeQueen() {
    is_queen_ = true;
}

bool Cell::CheckQueen() {
    return is_queen_;
}

void Cell::SetEstimation(int est) {
    estimation_ = est;
}

int Cell::GetEstimation() const {
    return estimation_;
}

Cell &Cell::operator=(const Cell &cell) = default;

Desk::Desk(size_t height, size_t width) {
    int checkers_white[3] = {0, 1, 2};
    int checkers_black[3] = {5, 6, 7};
    desk_.resize(height);
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            desk_[i].push_back(Cell(i, j, EMPTY));
        }
    }
    for (auto line: checkers_white) {
        for (int i = line % 2; i < width; i += 2) {
            desk_[line][i].SetState(WHITE);
        }
    }
    for (auto line: checkers_black) {
        for (int i = line % 2; i < width; i += 2) {
            desk_[line][i].SetState(BLACK);
        }
    }
}

void Desk::PrintDesk() {
    for (int i = 0; i < desk_.size(); ++i) {
        for (int j = 0; j < desk_[0].size(); ++j) {
            std::cout << desk_[i][j].GetState() << ' ';
        }
        std::cout << '\n';
    }
}

bool Desk::InDesk(size_t coord_x, size_t coord_y) {
    return (0 <= coord_x && coord_x < GetHeight()
            && 0 <= coord_y && coord_y < GetWidth());
}

std::vector<std::vector<Cell>> Desk::GetAvaiableSteps(Cell &cell) {
    std::vector<Cell> steps;
    std::vector<Cell> hit_cells;
    std::pair<int, int> position = cell.GetCoords();
    if (cell.GetState() == WHITE) {
        if (cell.CheckQueen()) {
            GetStepsQueen(cell, WHITE, BLACK, steps, hit_cells);
        } else {
            GetStepsCheck(cell, WHITE, BLACK, steps, hit_cells);
        }
    } else {
        if (cell.CheckQueen()) {
            GetStepsQueen(cell, BLACK, WHITE, steps, hit_cells);
        } else {
            GetStepsCheck(cell, BLACK, WHITE, steps, hit_cells);
        }
    }
    std::vector<std::vector<Cell>> result;
    result.push_back(steps);
    result.push_back(hit_cells);
    return result;
}

size_t Desk::GetHeight() {
    return desk_.size();
}

size_t Desk::GetWidth() {
    return desk_[0].size();
}

void
Desk::GetStepsQueen(Cell &cell, int color, int enemy_color, std::vector<Cell> &steps, std::vector<Cell> &hit_cells) {
    int positive_diag_enemy = 0;
    int negative_diag_enemy = 0;
    bool end_positive_diag = false;
    bool end_negative_diag = false;
    std::pair<int, int> position = cell.GetCoords();
    for (int i = 1; i < GetHeight(); ++i) {
        if (color == BLACK) { i = -i; }
        if (!end_positive_diag && InDesk(position.first + i, position.second + i)) {
            if (desk_[position.first + i][position.second + i].GetState() == EMPTY) {
                if (positive_diag_enemy > 1) {
                    end_positive_diag = true;
                }
                if (positive_diag_enemy == 1) {
                    hit_cells.push_back(desk_[position.first + i][position.second + i]);
                } else {
                    steps.push_back(desk_[position.first + i][position.second + i]);
                }
            } else if (desk_[position.first + i][position.second + i].GetState() == enemy_color) {
                ++positive_diag_enemy;
            }
        }
        if (!end_negative_diag && InDesk(position.first + i, position.second - i)) {
            if (desk_[position.first + i][position.second - i].GetState() == EMPTY) {
                if (negative_diag_enemy > 1) {
                    end_negative_diag = true;
                }
                if (negative_diag_enemy == 1) {
                    hit_cells.push_back(desk_[position.first + i][position.second - i]);
                } else {
                    steps.push_back(desk_[position.first + i][position.second - i]);
                }
            } else if (desk_[position.first + i][position.second - i].GetState() == enemy_color) {
                ++negative_diag_enemy;
            }
        }
    }
}

void Desk::GetStepsCheck(Cell &cell, int color, int enemy_color,
                         std::vector<Cell> &steps, std::vector<Cell> &hit_cells) {
    std::pair<int, int> position = cell.GetCoords();
    bool positive_diag_enemy = false;
    bool negative_diag_enemy = false;
    for (int i = 1; i < 4; ++i) {
        if (color == BLACK) { i = -i; }
        if (InDesk(position.first + i, position.second + i)) {
            if (desk_[position.first + i][position.second + i].GetState() == EMPTY) {
                if (positive_diag_enemy) {
                    hit_cells.push_back(desk_[position.first + i][position.second + i]);
                } else if (i % 2 == 1) {
                    steps.push_back(desk_[position.first + i][position.second + i]);
                }
            } else if (desk_[position.first + i][position.second + i].GetState() == enemy_color) {
                positive_diag_enemy = true;
            }
        }
        if (InDesk(position.first + i, position.second - i)) {
            if (desk_[position.first + i][position.second - i].GetState() == EMPTY) {
                if (negative_diag_enemy) {
                    hit_cells.push_back(desk_[position.first + i][position.second - i]);
                } else if (i % 2 == 1) {
                    steps.push_back(desk_[position.first + i][position.second - i]);
                }
            } else if (desk_[position.first + i][position.second - i].GetState() == enemy_color) {
                negative_diag_enemy = true;
            }
        }
    }
}

void Desk::SetCell(Cell &cell, int coord_x, int coord_y) {
    desk_[coord_x][coord_y].SetState(cell.GetState());
}

Cell &Desk::GetCell(int coord_x, int coord_y) {
    return desk_[coord_x][coord_y];
}

Desk::Desk(Desk *desk) {
    desk_ = desk->desk_;
}

void Player::MakeStep(std::pair<int, int> from, std::pair<int, int> to, Desk *desk) {
    desk->SetCell(desk->GetCell(from.first, from.second), to.first, to.second);
    Cell empty_cell = Cell(from.first, from.second);
    desk->SetCell(empty_cell, from.first, from.second);
}

void Player::MakeStepWithKill(std::pair<int, int> from, std::pair<int, int> to, Desk *desk) {
    desk->SetCell(desk->GetCell(from.first, from.second), to.first, to.second);
    Cell empty_cell = Cell(from.first, from.second);
    Cell empty_middle_cell = Cell((from.first + to.first) / 2, (from.second + to.second) / 2);
    desk->SetCell(empty_cell, from.first, from.second);
    desk->SetCell(empty_middle_cell, (from.first + to.first) / 2, (from.second + to.second) / 2);
}


void Game::MakeHumanStep(std::pair<int, int> from, std::pair<int, int> to) {
    ///Добавить проверку
    std::vector<std::vector<Cell>> available_steps = desk_.GetAvaiableSteps(desk_.GetCell(from.first, from.second));
    if (!available_steps[1].empty()) {
        for (auto &i: available_steps[1]) {
            if (i.GetCoords() == to) {
                humanPlayer_.MakeStepWithKill(from, to, &desk_);
                break;
            }
        }
        std::cout << "Неправильный ход, бить обязательно";
    } else {
        for (auto &i: available_steps[0]) {
            if (i.GetCoords() == to) {
                humanPlayer_.MakeStep(from, to, &desk_);
                break;
            }
        }
        std::cout << "Неправильный ход";
    }

}

void Game::Start() {
    int from_x, from_y, to_x, to_y;
    while (true) {
        std::cout << "\n*************************************************\n";
        desk_.PrintDesk();
        std::cout << "\nХод игрока: ";
        std::cin >> from_x >> from_y >> to_x >> to_y;
        if (from_x == -1) {
            break;
        }
        MakeHumanStep({from_x, from_y}, {to_x, to_y});
        desk_.PrintDesk();
        MakeAiStep({from_x, from_y}, {to_x, to_y});

    }
}

void Game::MakeAiStep(std::pair<int, int> from, std::pair<int, int> to) {}


int AIPlayer::AnalyzeCell(std::pair<int, int> from, Desk *desk) {
    std::queue<std::pair<int, int>> queue;
    Desk tmp_desk = Desk(desk);
    queue.push(from);
    std::pair<int, int> current;
    int min_estimation = std::numeric_limits<int32_t>::max();
    while (!queue.empty()) {
        current = queue.front();
        queue.pop();
        std::vector<std::vector<Cell>> available_steps_ = desk->GetAvaiableSteps(
                desk->GetCell(current.first, current.second));
        for (auto cell: available_steps_[1]) {
            MakeStepWithKill(current, cell.GetCoords(), &tmp_desk);
            Cell new_cell = tmp_desk.GetCell(cell.GetCoords().first, cell.GetCoords().second);
            new_cell.SetEstimation(new_cell.GetEstimation() + 2);
            if (new_cell.GetEstimation() < min_estimation) {
                min_estimation = new_cell.GetEstimation();
            }
        }
        for (auto cell: available_steps_[0]) {
            MakeStep(current, cell.GetCoords(), &tmp_desk);
            Cell new_cell = tmp_desk.GetCell(cell.GetCoords().first, cell.GetCoords().second);
            new_cell.SetEstimation(new_cell.GetEstimation() + 1);
            if (new_cell.GetEstimation() > min_estimation) {
                min_estimation = new_cell.GetEstimation();
            }
        }
    }
    return min_estimation;
}

int AIPlayer::Minimax(std::pair<int, int> from, int recursion_lvl, Desk *desk, int player) {
    Desk tmp_desk = desk;
    if (recursion_lvl == 0) {
        tmp_desk = Desk(desk);
    }
    if (recursion_lvl >= max_recursion_depth_) {
        return AnalyzeCell(from, desk);
    }
    std::pair<Cell, std::pair<int, int>> best_move{Cell(-1, -1), {}};
    int minmax = player == Human ? MIN_VAL : MAX_VAL;
    int test_minmax1;
    int test_minmax2;
    for (auto &checker: available_steps) {
        for (auto &correct_step: checker.second[0]) {
            test_minmax1 = Minimax(checker.first.GetCoords(), recursion_lvl + 1, &tmp_desk, -player);
            if ((test_minmax1 > minmax && player == Human) || (test_minmax1 <= minmax && player == AI) ||
                (best_move.first.GetCoords().first == -1)) {
                minmax = test_minmax1;
            }
        }
        for (auto &correct_step: checker.second[1]) {
            test_minmax2 = Minimax(checker.first.GetCoords(), recursion_lvl + 1, &tmp_desk, -player);
            if ((test_minmax2 > minmax && player == Human) || (test_minmax2 <= minmax && player == AI) ||
                (best_move.first.GetCoords().first == -1)) {
                minmax = test_minmax2;
            }
        }

    }
    return minmax;
}


