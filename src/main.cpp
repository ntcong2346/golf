#include "C:\Users\Cong\golf\src\include\SDL2\SDL.h"
#include "C:\Users\Cong\golf\src\include\SDL2\SDL_image.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath> 
#undef main

using namespace std;
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Thêm struct để quản lý bóng
struct Ball {
    int x, y;
    float speedX, speedY;
    bool isActive;
    SDL_Rect rect;
    static constexpr float FRICTION = 0.985f;  // More precise friction
    static constexpr float BOUNCE = 0.75f;     // More consistent bounce
};

// Thêm struct để quản lý lỗ
struct Hole {
    int x, y;
    int radius;
    SDL_Rect rect;
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL không thể khởi tạo! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    // Khởi tạo SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cout << "SDL_image không thể khởi tạo! SDL_image Error: " << IMG_GetError() << endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Quả bóng kéo thả", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        cout << "Không thể tạo cửa sổ! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Không thể tạo renderer! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }

    int ballRadius = 15; // Bán kính quả bóng nhỏ hơn
    const int initialBallX = SCREEN_WIDTH / 2;
    const int initialBallY = SCREEN_HEIGHT / 2;

    // Thay thế các biến bóng cũ bằng 2 quả bóng mới với vị trí khởi đầu mới
    Ball ball1 = {
        SCREEN_WIDTH / 4,              // Giữa nửa màn hình bên trái
        SCREEN_HEIGHT - 50,            // Cách đáy 50px
        0, 0,                          // Initial speed
        true,                          // First ball is active
        {SCREEN_WIDTH / 4 - 15, SCREEN_HEIGHT - 65, 30, 30}  // Rect position matches ball position
    };

    Ball ball2 = {
        3 * SCREEN_WIDTH / 4,          // Giữa nửa màn hình bên phải
        SCREEN_HEIGHT - 50,            // Cách đáy 50px
        0, 0,                          // Initial speed
        false,                         // Second ball starts inactive
        {3 * SCREEN_WIDTH / 4 - 15, SCREEN_HEIGHT - 65, 30, 30}  // Rect position matches ball position
    };

    int currentBall = 1; // 1 for ball1, 2 for ball2

    // Power bar variables
    const int MAX_POWER = 100;
    float currentPower = 0;
    float powerIncrement = 2.0f;  // Slower power increment for better control
    bool isPoweringUp = false;
    float aimAngle = 0;
    const float POWER_MULTIPLIER = 0.12f;  // Consistent power multiplier
    const int POWER_BAR_LENGTH = 250;  // Make power bar taller
    const int POWER_BAR_WIDTH = 20;    // Make power bar wider
    const int POWER_BAR_MARGIN = 20;   // Margin from screen edge
    const int ARROW_OFFSET = 20;      // Distance from ball's edge to arrow
    const int POINT_WIDTH = 15;        // Reduced width for thinner appearance
    const int POINT_HEIGHT = 50;       // Adjusted height

    // Định nghĩa hố màu đỏ
    srand(static_cast<unsigned>(time(0)));
    int holeRadius = 20; // Bán kính hố

    // Thay thế khai báo hole bằng 2 lỗ
    const int HOLE_RADIUS = 20;
    Hole hole1 = {
        rand() % (SCREEN_WIDTH/2 - 2 * HOLE_RADIUS) + HOLE_RADIUS,  // Chỉ random trong nửa trái
        rand() % (SCREEN_HEIGHT - 2 * HOLE_RADIUS) + HOLE_RADIUS,
        HOLE_RADIUS,
        {0, 0, 40, 40}
    };

    Hole hole2 = {
        SCREEN_WIDTH/2 + rand() % (SCREEN_WIDTH/2 - 2 * HOLE_RADIUS) + HOLE_RADIUS,  // Chỉ random trong nửa phải
        rand() % (SCREEN_HEIGHT - 2 * HOLE_RADIUS) + HOLE_RADIUS,
        HOLE_RADIUS,
        {0, 0, 40, 40}
    };

    // Sửa đổi hàm tạo vị trí cho lỗ để chỉ cập nhật một lỗ
    auto updateHolePosition = [&](Hole& hole, bool leftSide) {
        if (leftSide) {
            hole.x = rand() % (SCREEN_WIDTH/2 - 2*HOLE_RADIUS) + HOLE_RADIUS;
        } else {
            hole.x = SCREEN_WIDTH/2 + rand() % (SCREEN_WIDTH/2 - 2*HOLE_RADIUS) + HOLE_RADIUS;
        }
        hole.y = rand() % (SCREEN_HEIGHT - 2*HOLE_RADIUS) + HOLE_RADIUS;
    };

    // Khởi tạo vị trí ban đầu cho cả hai lỗ
    updateHolePosition(hole1, true);
    updateHolePosition(hole2, false);

    bool isDragging = false;
    int startX = 0, startY = 0; // Vị trí bắt đầu kéo

    bool quit = false;
    SDL_Event e;

    // Load ảnh
    SDL_Surface* ballSurface = IMG_Load("assets/graphics/ball.png");
    SDL_Surface* bgSurface = IMG_Load("assets/graphics/bg.png");
    SDL_Surface* holeSurface = IMG_Load("assets/graphics/hole.png");
    SDL_Surface* pointSurface = IMG_Load("assets/graphics/point.png");

    if (!ballSurface || !bgSurface || !holeSurface || !pointSurface) {
        cout << "Không thể load ảnh! SDL_image Error: " << IMG_GetError() << endl;
        return -1;
    }

    // Tạo texture
    SDL_Texture* ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_Texture* holeTexture = SDL_CreateTextureFromSurface(renderer, holeSurface);
    SDL_Texture* pointTexture = SDL_CreateTextureFromSurface(renderer, pointSurface);

    // Giải phóng surfaces
    SDL_FreeSurface(ballSurface);
    SDL_FreeSurface(bgSurface);
    SDL_FreeSurface(holeSurface);
    SDL_FreeSurface(pointSurface);

    // Kích thước của ball và hole
    SDL_Rect ballRect = { SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 30, 30 }; // Điều chỉnh kích thước phù hợp
    SDL_Rect holeRect = { hole1.x, hole1.y, 40, 40 }; // Điều chỉnh kích thước phù hợp

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            Ball& activeBall = (currentBall == 1) ? ball1 : ball2;

            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
                Ball& activeBall = (currentBall == 1) ? ball1 : ball2;
                // Kiểm tra nếu chuột click vào quả bóng
                int dx = e.button.x - activeBall.x;
                int dy = e.button.y - activeBall.y;
                if (sqrt(dx * dx + dy * dy) <= ballRadius) {  // Removed isActive check
                    isDragging = true;
                    isPoweringUp = true;
                    currentPower = 0;
                }
            }

            if (e.type == SDL_MOUSEMOTION && isDragging) {
                // Calculate aim angle based on mouse position
                int dx = e.motion.x - activeBall.x;
                int dy = e.motion.y - activeBall.y;
                aimAngle = atan2(dy, dx);
            }

            if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                if (isDragging) {
                    isDragging = false;
                    isPoweringUp = false;
                    
                    float power = currentPower * POWER_MULTIPLIER;
                    // More precise velocity calculations
                    activeBall.speedX = roundf(cos(aimAngle) * power * 100) / 100.0f;
                    activeBall.speedY = roundf(sin(aimAngle) * power * 100) / 100.0f;
                    
                    // Switch balls immediately after shooting
                    if (currentBall == 1) {
                        currentBall = 2;
                        ball2.isActive = true;
                        ball1.isActive = false;
                    } else {
                        currentBall = 1;
                        ball1.isActive = true;
                        ball2.isActive = false;
                    }
                }
            }
        }

        // Update power while charging
        if (isPoweringUp) {
            currentPower += powerIncrement;
            if (currentPower >= MAX_POWER) {
                currentPower = MAX_POWER;
                powerIncrement = -powerIncrement;
            } else if (currentPower <= 0) {
                currentPower = 0;
                powerIncrement = -powerIncrement;
            }
        }

        // Update ball positions
        for (Ball* ball : {&ball1, &ball2}) {
            // Allow both balls to move based on their physics
            // Remove the isActive check here
            
            // Update position with higher precision
            ball->x = roundf(ball->x + ball->speedX);
            ball->y = roundf(ball->y + ball->speedY);

            // Apply consistent friction
            ball->speedX = roundf(ball->speedX * Ball::FRICTION * 1000) / 1000.0f;
            ball->speedY = roundf(ball->speedY * Ball::FRICTION * 1000) / 1000.0f;

            // More precise stop threshold
            if (fabs(ball->speedX) < 0.005f && fabs(ball->speedY) < 0.005f) {
                ball->speedX = 0;
                ball->speedY = 0;
            }

            // Bounce off walls with improved collision detection
            if (ball->x < ballRadius) {
                ball->x = ballRadius;
                ball->speedX = -ball->speedX * Ball::BOUNCE;
            } else if (ball->x > SCREEN_WIDTH - ballRadius) {
                ball->x = SCREEN_WIDTH - ballRadius;
                ball->speedX = -ball->speedX * Ball::BOUNCE;
            }

            // Add center line collision
            if (ball == &ball1 && ball->x > SCREEN_WIDTH/2 - ballRadius) {
                ball->x = SCREEN_WIDTH/2 - ballRadius;
                ball->speedX = -ball->speedX * Ball::BOUNCE;
            } else if (ball == &ball2 && ball->x < SCREEN_WIDTH/2 + ballRadius) {
                ball->x = SCREEN_WIDTH/2 + ballRadius;
                ball->speedX = -ball->speedX * Ball::BOUNCE;
            }

            if (ball->y < ballRadius) {
                ball->y = ballRadius;
                ball->speedY = -ball->speedY * Ball::BOUNCE;
            } else if (ball->y > SCREEN_HEIGHT - ballRadius) {
                ball->y = SCREEN_HEIGHT - ballRadius;
                ball->speedY = -ball->speedY * Ball::BOUNCE;
            }

            // Kiểm tra va chạm với lỗ tương ứng
            Hole& targetHole = (ball == &ball1) ? hole1 : hole2;
            int dx = ball->x - targetHole.x;
            int dy = ball->y - targetHole.y;
            
            if (sqrt(dx * dx + dy * dy) <= ballRadius + targetHole.radius) {
                // Reset ball position
                ball->x = (ball == &ball1) ? SCREEN_WIDTH/4 : 3*SCREEN_WIDTH/4;
                ball->y = SCREEN_HEIGHT - 50;
                ball->speedX = 0;
                ball->speedY = 0;

                // Chỉ cập nhật vị trí của lỗ tương ứng với quả bóng đã vào
                if (ball == &ball1) {
                    updateHolePosition(hole1, true);
                } else {
                    updateHolePosition(hole2, false);
                }
                
                // Chuyển lượt và kích hoạt bóng
                if (currentBall == 1) {
                    currentBall = 2;
                    ball2.isActive = true;
                    ball1.isActive = false;
                } else {
                    currentBall = 1;
                    ball1.isActive = true;
                    ball2.isActive = false;
                }
            }

            // Update ball rectangle
            ball->rect.x = ball->x - ball->rect.w/2;
            ball->rect.y = ball->y - ball->rect.h/2;
        }

        // Update hole rectangles
        hole1.rect.x = hole1.x - hole1.rect.w/2;
        hole1.rect.y = hole1.y - hole1.rect.h/2;
        hole2.rect.x = hole2.x - hole2.rect.w/2;
        hole2.rect.y = hole2.y - hole2.rect.h/2;

        // Render background
        SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
        
        // Vẽ đường phân chia giữa 2 bên
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT);
        
        // Render các lỗ
        SDL_RenderCopy(renderer, holeTexture, NULL, &hole1.rect);
        SDL_RenderCopy(renderer, holeTexture, NULL, &hole2.rect);
        
        // Render both balls
        SDL_RenderCopy(renderer, ballTexture, NULL, &ball1.rect);
        SDL_RenderCopy(renderer, ballTexture, NULL, &ball2.rect);

        // Draw power bar and aim arrow only for active ball
        if (isDragging) {
            Ball& activeBall = (currentBall == 1) ? ball1 : ball2;
            
            // Draw power bars based on current ball
            SDL_Rect powerBarBg;
            if (currentBall == 1) {
                // Left power bar for ball1
                powerBarBg = {
                    POWER_BAR_MARGIN,
                    SCREEN_HEIGHT - POWER_BAR_MARGIN - POWER_BAR_LENGTH,
                    POWER_BAR_WIDTH,
                    POWER_BAR_LENGTH
                };
            } else {
                // Right power bar for ball2
                powerBarBg = {
                    SCREEN_WIDTH - POWER_BAR_MARGIN - POWER_BAR_WIDTH,
                    SCREEN_HEIGHT - POWER_BAR_MARGIN - POWER_BAR_LENGTH,
                    POWER_BAR_WIDTH,
                    POWER_BAR_LENGTH
                };
            }

            // Draw power bar background
            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
            SDL_RenderFillRect(renderer, &powerBarBg);

            // Draw power bar fill
            int fillHeight = static_cast<int>((currentPower / MAX_POWER) * POWER_BAR_LENGTH);
            SDL_Rect powerBarFill = {
                powerBarBg.x,
                powerBarBg.y + (POWER_BAR_LENGTH - fillHeight),
                POWER_BAR_WIDTH,
                fillHeight
            };

            // Color gradient
            int r = currentPower > 50 ? 255 : (currentPower * 5.1f);
            int g = currentPower < 50 ? 255 : (255 - (currentPower - 50) * 5.1f);
            SDL_SetRenderDrawColor(renderer, r, g, 0, 255);
            SDL_RenderFillRect(renderer, &powerBarFill);

            // Draw power bar border
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderDrawRect(renderer, &powerBarBg);

            // Calculate point position to be just outside the ball
            int pointX = activeBall.x + cos(aimAngle) * (ballRadius + ARROW_OFFSET);
            int pointY = activeBall.y + sin(aimAngle) * (ballRadius + ARROW_OFFSET);

            SDL_Rect pointRect = {
                pointX - POINT_WIDTH/2,
                pointY - POINT_HEIGHT/2,
                POINT_WIDTH,
                POINT_HEIGHT
            };
            
            double angle = aimAngle * 180.0 / M_PI;
            SDL_RenderCopyEx(renderer, pointTexture, NULL, &pointRect, angle + 90, NULL, SDL_FLIP_NONE);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    // Giải phóng textures
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(holeTexture);
    SDL_DestroyTexture(pointTexture);

    // Giải phóng tài nguyên
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit(); // Thêm dòng này trước SDL_Quit()
    SDL_Quit();

    return 0;
}
