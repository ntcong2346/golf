#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "structures.h"

enum GameState {
    MENU,
    PLAYING
};

class Game {
public:
    Game();
    ~Game();
    bool init();
    void handleInput();
    void update();
    void render();
    bool isRunning() const { return !quit; }
    bool init();
    void run();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;

    // Constants
    static constexpr int MAX_POWER = 100;
    static constexpr float POWER_MULTIPLIER = 0.12f;
    static constexpr int POWER_BAR_LENGTH = 250;
    static constexpr int POWER_BAR_WIDTH = 20;
    static constexpr int POWER_BAR_MARGIN = 20;
    static constexpr int ARROW_OFFSET = 20;
    static constexpr int POINT_WIDTH = 15;
    static constexpr int POINT_HEIGHT = 50;
    static constexpr int HOLE_RADIUS = 20;
    static constexpr int ballRadius = 15;

    // Game objects
    Ball ball1, ball2;
    GameScore gameScore;
    ObstacleSet leftObstacles, rightObstacles;
    RoundObstacles roundObstacles;
    Hole hole1, hole2;
    Reward reward1, reward2;

    // Game state
    bool quit;
    GameState gameState;
    int currentBall;
    bool isDragging;
    float currentPower;
    float aimAngle;
    bool isPoweringUp;
    float powerIncrement;
    int player1Strokes;
    int player2Strokes;

    // Textures
    SDL_Texture* ballTexture;
    SDL_Texture* bgTexture;
    SDL_Texture* holeTexture;
    SDL_Texture* pointTexture;
    SDL_Texture* menuBgTexture;
    SDL_Texture* darkTileTexture;
    SDL_Texture* lightTileTexture;
    SDL_Texture* playBrightTexture;
    SDL_Texture* menuBrightTexture;
    SDL_Texture* exitBrightTexture;
    SDL_Texture* rewardTexture;

    // Sound effects
    Mix_Chunk* swingSound;
    Mix_Chunk* holeSound;
    Mix_Chunk* bounceSound;

    // UI Elements
    SDL_Rect playHitBox;
    SDL_Rect menuHitBox;
    SDL_Rect exitHitBox;

    // Helper methods
    void loadTextures();
    void loadSounds();
    void updateBall(Ball* ball);
    void handleHoleCollision(Ball* ball);
    void handleObstacleCollision(Ball* ball, Obstacle& obs);
    void initializeRoundObstacles();
    void resetRound();
    void setupGame();
};
