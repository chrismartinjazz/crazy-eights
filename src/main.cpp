#include "game.h"

int main()
{
    Game game { Game::create() };
    game.game_loop();
    return 0;
}