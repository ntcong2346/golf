// Renderer.h
#ifndef RENDERER_H
#define RENDERER_H

#include <SDL2/SDL.h>
#include "Hole.h"
#include "Obstacle.h"
#include "Reward.h"

class Renderer {
public:
    static void renderHole(const Hole& hole, SDL_Renderer* renderer);
    static void renderObstacle(const Obstacle& obstacle, SDL_Renderer* renderer);
    static void renderReward(const Reward& reward, SDL_Renderer* renderer);
};

#endif // RENDERER_H
