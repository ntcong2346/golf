#ifndef BALL_H
#define BALL_H
#include <SDL2/SDL.h>

struct Ball {
    int x, y;
    float speedX, speedY;
    bool isActive;
    SDL_Rect rect;
    static constexpr float FRICTION = 0.985f;
    static constexpr float BOUNCE = 0.75f;
    static constexpr float OBSTACLE_BOUNCE = 0.85f;
    static constexpr float OBSTACLE_FRICTION = 0.95f;
    float scale = 1.0f;
    bool isAnimating = false;
    float animationTimer = 0.0f;
    static constexpr float ANIMATION_DURATION = 1.5f;
    float targetX = 0;
    float targetY = 0;
    static constexpr float MOVE_SPEED = 2.0f;
};

#endif