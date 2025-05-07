#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include "Ball.h"
#include "Hole.h"
#include "Reward.h"
#include "Obstacle.h"
#include "GameScore.h"
#include "GameSettings.h"
#include "GameState.h"

class GameLogic {
public:
    GameLogic(int screenWidth, int screenHeight);
    void update();
    GameState getGameState() const { return gameState; }
    // Thêm các getter/setter cần thiết

private:
    Ball ball1, ball2;
    Hole hole1, hole2;
    Reward reward1, reward2;
    ObstacleSet leftObstacles, rightObstacles;
    RoundObstacles roundObstacles;
    GameScore gameScore;
    GameSettings settings;
    GameState gameState;
    int currentBall;
    int player1Strokes, player2Strokes;
    bool isDragging, isPoweringUp;
    float currentPower, aimAngle;
    int screenWidth, screenHeight;
    // Thêm các biến khác như power bar, constants, v.v.
};

#endif