#include "game.h"

class GraphicsManager {
public:
    static void render(Game& game) {
        // Clear screen
        SDL_SetRenderDrawColor(game.renderer, 135, 206, 235, 255);
        SDL_RenderClear(game.renderer);

        if (game.gameState == MENU) {
            renderMenu(game);
        } else {
            renderGame(game);
        }

        SDL_RenderPresent(game.renderer);
    }

private:
    static void renderMenu(Game& game) {
        // Copy menu rendering from main.cpp
    }

    static void renderGame(Game& game) {
        // Copy game rendering from main.cpp
        // Including background, balls, obstacles, UI, etc.
    }

    static void renderBall(Game& game, const Ball& ball);
    static void renderPowerBar(Game& game);
    static void renderObstacles(Game& game);
    static void renderHoles(Game& game);
    static void renderUI(Game& game);
};