// entities.h
#pragma once

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

struct Hole {
    int x, y;
    int radius;
    SDL_Rect rect;
};

struct Reward {
    int x, y;
    bool active;
    SDL_Rect rect;
    static const int SIZE = 25;
};

struct Obstacle {
    int x, y;
    int width, height;
    SDL_Rect rect;
};

struct GameScore {
    int player1Score = 0;
    int player2Score = 0;
    int currentRound = 1;
    int maxRounds = 5;
    int totalStrokes1 = 0;
    int totalStrokes2 = 0;
    bool isGameOver = false;
};

struct ObstacleSet {
    static const int MAX_OBSTACLES = 5;
    Obstacle obstacles[MAX_OBSTACLES];
    int count;
};

struct RoundObstacles {
    static const int MAX_ROUNDS = 5;
    ObstacleSet roundConfigs[MAX_ROUNDS] = {
        {{{0}}, 2},  // Round 1
        {{{0}}, 3},  // Round 2
        {{{0}}, 3},  // Round 3
        {{{0}}, 4},  // Round 4
        {{{0}}, 5}   // Round 5
    };
};