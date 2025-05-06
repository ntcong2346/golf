#include "game.h"
#include <SDL2/SDL.h>
#undef main

int main(int argc, char* argv[]) {
    // Create and initialize game
    Game game;
    if (!game.init()) {
        return -1;
    }

    // Main game loop
    while (game.isRunning()) {
        game.handleInput();
        game.update();
        game.render();
    }

    return 0;
}
