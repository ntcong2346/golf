#pragma once
#include <SDL2/SDL.h>  // Sửa đường dẫn include
#include "structures.h"

class Renderer {
public:
    Renderer(SDL_Renderer* renderer);
    void renderBall(const Ball& ball);
    void renderHole(const Hole& hole);
    void renderUI(const GameScore& score);
    void renderWinScreen(bool isPlayer1Winner);
    
private:
    SDL_Renderer* renderer;
    // Thêm các texture và asset cần thiết
};
