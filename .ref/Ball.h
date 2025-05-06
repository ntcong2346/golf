#pragma once
#include <SDL2/SDL.h>

// Struct quản lý bóng
struct Ball {
    int x, y;
    float speedX, speedY;
    bool isActive;
    SDL_Rect rect;
    
    // Physics constants
    static constexpr float FRICTION = 0.985f;  // Ma sát
    static constexpr float BOUNCE = 0.75f;     // Hệ số nảy
    static constexpr float OBSTACLE_BOUNCE = 0.85f;  // Hệ số nảy với chướng ngại vật
    static constexpr float OBSTACLE_FRICTION = 0.95f; // Ma sát với chướng ngại vật
    
    // Animation properties
    float scale = 1.0f;             // Tỷ lệ scale của bóng
    bool isAnimating = false;       // Trạng thái đang animate
    float animationTimer = 0.0f;    // Timer cho animation
    static constexpr float ANIMATION_DURATION = 1.5f; // Thời gian animation (giây)
    float targetX = 0;    // Vị trí đích khi vào lỗ
    float targetY = 0;    // Vị trí đích khi vào lỗ
    static constexpr float MOVE_SPEED = 2.0f;  // Tốc độ di chuyển vào lỗ
};

// Ball handling functions
void updateBallPhysics(Ball& ball, int ballRadius, int screenWidth, int screenHeight, bool isLeftSide);
void checkBallInHole(Ball& ball, int holeX, int holeY, int ballRadius, int holeRadius);
void resetBall(Ball& ball, bool isLeftSide, int screenWidth, int screenHeight);