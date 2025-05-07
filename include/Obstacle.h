#ifndef OBSTACLE_H
#define OBSTACLE_H
#include <SDL2/SDL.h>

struct Obstacle {
    int x, y;
    int width, height;
    SDL_Rect rect;
    Obstacle(int x = 0, int y = 0, int width = 0, int height = 0, SDL_Rect rect = {0, 0, 0, 0})
        : x(x), y(y), width(width), height(height), rect(rect) {}
};

struct ObstacleSet {
    static const int MAX_OBSTACLES = 5;
    Obstacle obstacles[MAX_OBSTACLES];
    int count;
};

struct RoundObstacles {
    static const int MAX_ROUNDS = 5;
    ObstacleSet roundConfigs[MAX_ROUNDS];
};

#endif