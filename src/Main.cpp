#include <iostream>
#include "Game/Game.h"

int main(int argc, char* argv[]) {
    Game* game = new Game();
    game->Initialize();
    game->Run();
    game->Destroy();

    delete game;
    return 0;
}
