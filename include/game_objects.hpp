#ifndef GAME_OBJECTS_HPP
#define GAME_OBJECTS_HPP

#include <SDL2/SDL.h>
#include "../include/game_state.hpp" // Adjusted path to locate the header file

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int ballRadius = 15;
const int HOLE_RADIUS = 20;

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
    ObstacleSet roundConfigs[MAX_ROUNDS];
};

extern RoundObstacles roundObstacles;
extern ObstacleSet leftObstacles;
extern ObstacleSet rightObstacles;
extern GameScore gameScore;

void initializeRoundObstacles();
void resetRewards(Reward& reward1, Reward& reward2);
void updateBalls(Ball& ball1, Ball& ball2, Reward& reward1, Reward& reward2, Hole& hole1, Hole& hole2, int& currentBall, int& player1Strokes, int& player2Strokes, GameState& gameState);
void updateHoles(Hole& hole1, Hole& hole2);
void updateObstacles(ObstacleSet& leftObstacles, ObstacleSet& rightObstacles);

#endif