#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath> 
#include "GameState.h"  // Add GameState include
#undef main

using namespace std;
// Constants    
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int MAX_POWER = 100;
const float POWER_MULTIPLIER = 0.12f;
const int POWER_BAR_LENGTH = 250;
const int POWER_BAR_WIDTH = 20;
const int POWER_BAR_MARGIN = 20;
const int ARROW_OFFSET = 20;
const int POINT_WIDTH = 15;
const int POINT_HEIGHT = 50;
const int HOLE_RADIUS = 20;

struct Ball {
    int x, y;
    float speedX, speedY;
    bool isActive;
    SDL_Rect rect;
    static constexpr float FRICTION = 0.985f;  // More precise friction
    static constexpr float BOUNCE = 0.75f;     // More consistent bounce
    static constexpr float OBSTACLE_BOUNCE = 0.85f;  // Hệ số nảy với chướng ngại vật
    static constexpr float OBSTACLE_FRICTION = 0.95f; // Ma sát với chướng ngại vật
    float scale = 1.0f;             // Tỷ lệ scale của bóng
    bool isAnimating = false;       // Trạng thái đang animate
    float animationTimer = 0.0f;    // Timer cho animation
    static constexpr float ANIMATION_DURATION = 1.5f; // Thời gian animation (giây)
    float targetX = 0;    // Vị trí đích khi vào lỗ
    float targetY = 0;    // Vị trí đích khi vào lỗ
    static constexpr float MOVE_SPEED = 2.0f;  // Tốc độ di chuyển vào lỗ
};

// Thêm struct để quản lý lỗ
struct Hole {
    int x, y;
    int radius;
    SDL_Rect rect;
};

// Update Reward struct size for hearts image
struct Reward {
    int x, y;
    bool active;
    SDL_Rect rect;
    static const int SIZE = 25;  // Adjusted size for heart image
};

// Add obstacle struct
struct Obstacle {
    int x, y;
    int width, height;
    SDL_Rect rect;

    // Constructor to initialize an Obstacle
    Obstacle(int x = 0, int y = 0, int width = 0, int height = 0, SDL_Rect rect = {0, 0, 0, 0})
        : x(x), y(y), width(width), height(height), rect(rect) {}
};

// Modify the GameScore struct
struct GameScore {
    int player1Score = 0;
    int player2Score = 0;
    int currentRound = 1;  // Thay đổi từ 0 thành 1
    int maxRounds = 4;     // Giữ nguyên là 4
    int totalStrokes1 = 0;
    int totalStrokes2 = 0;
    bool isGameOver = false;
};

// Add array of obstacles for each player (maximum 5 per side)
struct ObstacleSet {
    static const int MAX_OBSTACLES = 5;
    Obstacle obstacles[MAX_OBSTACLES];
    int count;
};

// Add after obstacle struct definitions
struct RoundObstacles {
    static const int MAX_ROUNDS = 5;
    ObstacleSet roundConfigs[MAX_ROUNDS] = {
        {{{0}}, 2},  // Round 1: 2 obstacles
        {{{0}}, 3},  // Round 2: 3 obstacles
        {{{0}}, 3},  // Round 3: 3 obstacles
        {{{0}}, 4},  // Round 4: 4 obstacles
        {{{0}}, 5}   // Round 5: 5 obstacles
    };
};
// Add after GameState enum
enum BallSpeed {
    SLOW = 0,
    NORMAL = 1,
    SWIFT = 2
};

// Add new variables for game settings
struct GameSettings {
    BallSpeed speed = NORMAL;
    bool soundEnabled = true;
    float speedMultiplier[3] = {0.08f, 0.12f, 0.18f}; // For SLOW, NORMAL, SWIFT
};

void renderSettingsMenu(SDL_Renderer* renderer, TTF_Font* font, GameSettings& settings, SDL_Texture* settingsBgTexture);

void initializeGame(SDL_Renderer* renderer, TTF_Font* font, GameState& gameState, Ball& ball1, Ball& ball2, 
                   Hole& hole1, Hole& hole2, Reward& reward1, Reward& reward2, 
                   ObstacleSet& leftObstacles, ObstacleSet& rightObstacles, 
                   GameScore& gameScore, RoundObstacles& roundObstacles, 
                   int& currentBall, int& player1Strokes, int& player2Strokes);

void handleEvents(SDL_Event& e, bool& quit, GameState& gameState, Ball& ball1, Ball& ball2, 
                 int& currentBall, bool& isDragging, bool& isPoweringUp, float& currentPower, 
                 float& aimAngle, int& player1Strokes, int& player2Strokes, Mix_Chunk* swingSound, int ballRadius);

void updateGame(Ball& ball1, Ball& ball2, Hole& hole1, Hole& hole2, 
               Reward& reward1, Reward& reward2, ObstacleSet& leftObstacles, 
               ObstacleSet& rightObstacles, GameScore& gameScore, 
               RoundObstacles& roundObstacles, int& currentBall, bool& isPoweringUp, 
               float& currentPower, float& powerIncrement, int& player1Strokes, 
               int& player2Strokes, GameState& gameState, Mix_Chunk* holeSound, 
               Mix_Chunk* bounceSound, int ballRadius);

void renderGame(SDL_Renderer* renderer, GameState& gameState, Ball& ball1, Ball& ball2, 
               Hole& hole1, Hole& hole2, Reward& reward1, Reward& reward2, 
               ObstacleSet& leftObstacles, ObstacleSet& rightObstacles, 
               GameScore& gameScore, bool isDragging, float currentPower, float aimAngle, 
               int currentBall, int player1Strokes, int player2Strokes, 
               SDL_Texture* bgTexture, SDL_Texture* ballTexture, SDL_Texture* holeTexture, 
               SDL_Texture* pointTexture, SDL_Texture* menuBgTexture, SDL_Texture* rewardTexture,
               SDL_Texture* darkTileTexture, SDL_Texture* lightTileTexture, 
               SDL_Texture* playBrightTexture, SDL_Texture* menuBrightTexture, 
               SDL_Texture* exitBrightTexture, TTF_Font* font, int ballRadius);

float currentPower = 0;
float powerIncrement = 2.0f;  // Slower power increment for better control
bool isPoweringUp = false;
float aimAngle = 0;

