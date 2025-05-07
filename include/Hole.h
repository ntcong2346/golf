#ifndef HOLE_H
#define HOLE_H
#include <SDL2/SDL.h>

struct Hole {
    int x, y;
    int radius;
    SDL_Rect rect;
};

#endif