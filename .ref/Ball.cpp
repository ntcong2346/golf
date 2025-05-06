#include "Ball.h"
#include <cmath>

void updateBallPhysics(Ball& ball, int ballRadius, int screenWidth, int screenHeight, bool isLeftSide) {
    // Update position with higher precision
    ball.x = roundf(ball.x + ball.speedX);
    ball.y = roundf(ball.y + ball.speedY);

    // Apply consistent friction
    ball.speedX = roundf(ball.speedX * Ball::FRICTION * 1000) / 1000.0f;
    ball.speedY = roundf(ball.speedY * Ball::FRICTION * 1000) / 1000.0f;

    // More precise stop threshold
    if (fabs(ball.speedX) < 0.005f && fabs(ball.speedY) < 0.005f) {
        ball.speedX = 0;
        ball.speedY = 0;
    }

    // Bounce off walls with improved collision detection
    if (ball.x < ballRadius) {
        ball.x = ballRadius;
        ball.speedX = -ball.speedX * Ball::BOUNCE;
    } else if (ball.x > screenWidth - ballRadius) {
        ball.x = screenWidth - ballRadius;
        ball.speedX = -ball.speedX * Ball::BOUNCE;
    }

    // Center line collision
    if (isLeftSide && ball.x > screenWidth/2 - ballRadius) {
        ball.x = screenWidth/2 - ballRadius;
        ball.speedX = -ball.speedX * Ball::BOUNCE;
    } else if (!isLeftSide && ball.x < screenWidth/2 + ballRadius) {
        ball.x = screenWidth/2 + ballRadius;
        ball.speedX = -ball.speedX * Ball::BOUNCE;
    }

    if (ball.y < ballRadius) {
        ball.y = ballRadius;
        ball.speedY = -ball.speedY * Ball::BOUNCE;
    } else if (ball.y > screenHeight - ballRadius) {
        ball.y = screenHeight - ballRadius;
        ball.speedY = -ball.speedY * Ball::BOUNCE;
    }

    // Update ball rectangle
    ball.rect.x = ball.x - ball.rect.w/2;
    ball.rect.y = ball.y - ball.rect.h/2;
}

void checkBallInHole(Ball& ball, int holeX, int holeY, int ballRadius, int holeRadius) {
    int dx = ball.x - holeX;
    int dy = ball.y - holeY;
    
    if (sqrt(dx * dx + dy * dy) <= ballRadius + holeRadius && !ball.isAnimating) {
        ball.isAnimating = true;
        ball.animationTimer = Ball::ANIMATION_DURATION;
        ball.speedX = 0;
        ball.speedY = 0;
        ball.targetX = holeX;
        ball.targetY = holeY;
    }
    
    // Xử lý animation
    if (ball.isAnimating) {
        ball.animationTimer -= 0.016f;
        
        // Di chuyển bóng về tâm lỗ
        float dx = ball.targetX - ball.x;
        float dy = ball.targetY - ball.y;
        float dist = sqrt(dx * dx + dy * dy);
        
        if (dist > 1.0f) { // Nếu còn cách xa trung tâm
            float moveX = (dx / dist) * Ball::MOVE_SPEED;
            float moveY = (dy / dist) * Ball::MOVE_SPEED;
            ball.x += moveX;
            ball.y += moveY;
        } else {
            ball.x = ball.targetX;
            ball.y = ball.targetY;
        }
        
        // Cập nhật scale
        ball.scale = ball.animationTimer / Ball::ANIMATION_DURATION;
    }
}

void resetBall(Ball& ball, bool isLeftSide, int screenWidth, int screenHeight) {
    if (isLeftSide) {
        ball.x = screenWidth / 4;
        ball.y = screenHeight - 50;
    } else {
        ball.x = 3 * screenWidth / 4;
        ball.y = screenHeight - 50;
    }
    
    ball.speedX = 0;
    ball.speedY = 0;
    ball.isAnimating = false;
    ball.scale = 1.0f;
    ball.rect.x = ball.x - ball.rect.w/2;
    ball.rect.y = ball.y - ball.rect.h/2;
}