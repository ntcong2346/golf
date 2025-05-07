#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <string>

// Screen dimensions
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Ball struct
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

// Hole struct
struct Hole {
    int x, y;
    int radius;
    SDL_Rect rect;
};

// Reward struct
struct Reward {
    int x, y;
    bool active;
    SDL_Rect rect;
    static const int SIZE = 25;
};

// Obstacle struct
struct Obstacle {
    int x = 0;
    int y = 0;
    int width = 0;
    int height = 0;
    SDL_Rect rect = {0, 0, 0, 0};

    Obstacle(int x = 0, int y = 0, int width = 0, int height = 0, SDL_Rect rect = {0, 0, 0, 0})
        : x(x), y(y), width(width), height(height), rect(rect) {}
};

// ObstacleSet struct
struct ObstacleSet {
    static const int MAX_OBSTACLES = 5;
    Obstacle obstacles[MAX_OBSTACLES];
    int count;
};

// RoundObstacles struct
struct RoundObstacles {
    static const int MAX_ROUNDS = 5;
    ObstacleSet roundConfigs[MAX_ROUNDS];
};

// GameScore struct
struct GameScore {
    int player1Score = 0;
    int player2Score = 0;
    int currentRound = 1;
    int maxRounds = 4;
    int totalStrokes1 = 0;
    int totalStrokes2 = 0;
    bool isGameOver = false;
};

// Game states
enum GameState {
    MENU,
    PLAYING,
    GAME_OVER,
    SETTINGS
};

// Ball speed settings
enum BallSpeed {
    SLOW = 0,
    NORMAL = 1,
    SWIFT = 2
};

// Game settings
struct GameSettings {
    BallSpeed speed = NORMAL;
    bool soundEnabled = true;
    float speedMultiplier[3] = {0.08f, 0.12f, 0.18f}; // For SLOW, NORMAL, SWIFT
};

// Global game variables
extern RoundObstacles roundObstacles;
extern ObstacleSet leftObstacles;
extern ObstacleSet rightObstacles;
extern GameScore gameScore;
extern GameSettings settings;
extern Ball ball1;
extern Ball ball2;
extern Hole hole1;
extern Hole hole2;
extern Reward reward1;
extern Reward reward2;
extern int currentBall;
extern int player1Strokes;
extern int player2Strokes;
extern GameState gameState;
extern bool isDragging;
extern bool isPoweringUp;
extern float currentPower;
extern float aimAngle;
extern int ballRadius;

// Power bar constants
const int MAX_POWER = 100;
const float POWER_MULTIPLIER = 0.12f;
const int POWER_BAR_LENGTH = 250;
const int POWER_BAR_WIDTH = 20;
const int POWER_BAR_MARGIN = 20;
const int ARROW_OFFSET = 20;
const int POINT_WIDTH = 15;
const int POINT_HEIGHT = 50;

// Function declarations
// Initialization functions
bool initSDL(SDL_Window** window, SDL_Renderer** renderer);
bool loadMedia(SDL_Renderer* renderer, TTF_Font** font, 
               SDL_Texture** ballTexture, SDL_Texture** bgTexture, 
               SDL_Texture** holeTexture, SDL_Texture** pointTexture,
               SDL_Texture** menuBgTexture, SDL_Texture** darkTileTexture,
               SDL_Texture** lightTileTexture, SDL_Texture** powermeterTexture,
               SDL_Texture** rewardTexture, SDL_Texture** settingsBgTexture,
               SDL_Texture** playBrightTexture, SDL_Texture** menuBrightTexture,
               SDL_Texture** exitBrightTexture, SDL_Texture** gameOverTexture,
               Mix_Chunk** swingSound, Mix_Chunk** holeSound, Mix_Chunk** bounceSound);
void initializeGame();
void initializeRoundObstacles();
void resetRewards();

// Event handling functions
void handleEvents(SDL_Event& e, SDL_Renderer* renderer, TTF_Font* font, 
                  SDL_Texture* settingsBgTexture, Mix_Chunk* swingSound, Mix_Chunk* bounceSound,
                  bool& quit, float& powerIncrement);
void handleMenuEvents(SDL_Event& e, SDL_Renderer* renderer, TTF_Font* font, 
                     SDL_Texture* settingsBgTexture, bool& quit);
void handleGameEvents(SDL_Event& e, Mix_Chunk* swingSound, Mix_Chunk* bounceSound, float& powerIncrement);
void handleSettingsEvents(SDL_Event& e, SDL_Renderer* renderer, TTF_Font* font, 
                         SDL_Texture* settingsBgTexture, bool& quit);
void handleGameOverEvents(SDL_Event& e, SDL_Renderer* renderer, TTF_Font* font, 
                         SDL_Texture* gameOverTexture, bool& quit);

// Update functions
void updateGame(Mix_Chunk* bounceSound = nullptr, Mix_Chunk* holeSound = nullptr);
void updateBall(Ball* ball, Mix_Chunk* bounceSound, Mix_Chunk* holeSound);
void updatePower(float& powerIncrement);
void updateObstacles();
void setupNextRound();
void checkGameOver(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* gameOverTexture);

// Render functions
void renderGame(SDL_Renderer* renderer, TTF_Font* font, 
               SDL_Texture* ballTexture, SDL_Texture* bgTexture, 
               SDL_Texture* holeTexture, SDL_Texture* pointTexture,
               SDL_Texture* darkTileTexture, SDL_Texture* lightTileTexture,
               SDL_Texture* rewardTexture);
void renderMenu(SDL_Renderer* renderer, SDL_Texture* menuBgTexture, 
               SDL_Texture* playBrightTexture, SDL_Texture* menuBrightTexture, 
               SDL_Texture* exitBrightTexture);
void renderSettingsMenu(SDL_Renderer* renderer, TTF_Font* font, 
                       GameSettings& settings, SDL_Texture* settingsBgTexture);
void renderGameOver(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* gameOverTexture);
void renderBalls(SDL_Renderer* renderer, SDL_Texture* ballTexture);
void renderObstacles(SDL_Renderer* renderer, SDL_Texture* darkTileTexture, SDL_Texture* lightTileTexture);
void renderPowerBar(SDL_Renderer* renderer);
void renderAimArrow(SDL_Renderer* renderer, SDL_Texture* pointTexture);
void renderScoreInfo(SDL_Renderer* renderer, TTF_Font* font);
void renderRewards(SDL_Renderer* renderer, SDL_Texture* rewardTexture);

// Cleanup function
void cleanup(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font,
            SDL_Texture* ballTexture, SDL_Texture* bgTexture, 
            SDL_Texture* holeTexture, SDL_Texture* pointTexture,
            SDL_Texture* menuBgTexture, SDL_Texture* darkTileTexture,
            SDL_Texture* lightTileTexture, SDL_Texture* powermeterTexture,
            SDL_Texture* rewardTexture, SDL_Texture* settingsBgTexture,
            SDL_Texture* playBrightTexture, SDL_Texture* menuBrightTexture,
            SDL_Texture* exitBrightTexture, SDL_Texture* gameOverTexture,
            Mix_Chunk* swingSound, Mix_Chunk* holeSound, Mix_Chunk* bounceSound);

#endif // GAME_H
