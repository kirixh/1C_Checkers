#include "checkers.cpp"

int main() {
    Desk desk(8, 8);
    HumanPlayer human(12, WHITE);
    AIPlayer aiPlayer(12, BLACK);
    Game game(human, aiPlayer, desk);
    game.Start();
    return 0;
}
