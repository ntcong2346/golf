// game.h
#pragma once

#include "constants.h"
#include "structs.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

class Game {
public:
    Game();
    ~Game();
    bool init();
    void run();
    void resetBall(Ball& ball);

private:
    void resetRound();
    void cleanup();
    void handleEvents();
    void update(float dt);
    void render();
    void initializeRoundObstacles();
    void resetRewards();
    void resetGame();

    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;

    // Game objects
    Ball ball1, ball2;
    Hole hole1, hole2;
    Reward reward1, reward2;
    ObstacleSet leftObstacles, rightObstacles;
    RoundObstacles roundObstacles;
    GameScore gameScore;

    // Game state
    GameState gameState;
    int currentBall;
    bool isDragging;
    bool isPoweringUp;
    float currentPower;
    float powerIncrement;
    float aimAngle;

    // Textures
    SDL_Texture* ballTexture;
    SDL_Texture* bgTexture;
    SDL_Texture* holeTexture;
    SDL_Texture* pointTexture;
    SDL_Texture* menuBgTexture;
    SDL_Texture* darkTileTexture;
    SDL_Texture* lightTileTexture;
    SDL_Texture* powermeterTexture;
    SDL_Texture* playBrightTexture;
    SDL_Texture* menuBrightTexture;
    SDL_Texture* exitBrightTexture;
    SDL_Texture* rewardTexture;

    // Sounds
    Mix_Chunk* swingSound;
    Mix_Chunk* holeSound;
    Mix_Chunk* bounceSound;

    // UI elements
    SDL_Rect playHitBox;
    SDL_Rect menuHitBox;
    SDL_Rect exitHitBox;
    SDL_Rect hoverButton;

    // Stroke counters
    int player1Strokes;
    int player2Strokes;
};