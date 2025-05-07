#ifndef REWARD_H
#define REWARD_H
#include <SDL2/SDL.h>

struct Reward {
    int x, y;
    bool active;
    SDL_Rect rect;
    static const int SIZE = 25;
};

#endif