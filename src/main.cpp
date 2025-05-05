#include "C:\Users\Cong\golf\src\include\SDL2\SDL.h"
#include "C:\Users\Cong\golf\src\include\SDL2\SDL_image.h"
#include "C:\Users\Cong\golf\src\include\SDL2\SDL_ttf.h"
#include "C:\Users\Cong\golf\src\include\SDL2\SDL_mixer.h" // Thêm SDL_mixer
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
};

// Add new variables for game state
struct GameScore {
    int player1Score = 0;
    int player2Score = 0;
    int currentRound = 1;
    int maxRounds = 5;  // Changed to 5 rounds
    int totalStrokes1 = 0;  // Thêm biến đếm tổng số strokes của player 1
    int totalStrokes2 = 0;  // Thêm biến đếm tổng số strokes của player 2
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

RoundObstacles roundObstacles;

ObstacleSet leftObstacles = {{{0}}, 1};   // Start with 1 obstacle
ObstacleSet rightObstacles = {{{0}}, 1};  // Start with 1 obstacle

GameScore gameScore;

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

    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << endl;
        return -1;
    }

    // Khởi tạo SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << endl;
        return -1;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("C:/Users/Cong/golf/assets/font/font.ttf", 24);
    if (!font) {
        cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << endl;
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
        50,  // Fixed x position near left corner
        50,  // Fixed y position near top
        HOLE_RADIUS,
        {0, 0, 40, 40}
    };

    Hole hole2 = {
        SCREEN_WIDTH/2 + 50,  // Fixed x position near left of right half
        50,  // Fixed y position near top
        HOLE_RADIUS,
        {0, 0, 40, 40}
    };

    // Replace single obstacle initialization with arrays
    // Initialize first obstacles
    leftObstacles.obstacles[0] = {
        100,  // Near hole1
        100,
        30,   // Width
        100,  // Height
        {100, 100, 30, 100}
    };

    rightObstacles.obstacles[0] = {
        SCREEN_WIDTH/2 + 100,  // Near hole2
        100,
        30,    // Width
        100,   // Height
        {SCREEN_WIDTH/2 + 100, 100, 30, 100}
    };

    // Add function to update obstacles for new round
    auto initializeRoundObstacles = [&]() {
        // Round 1: Simple vertical barriers
        roundObstacles.roundConfigs[0].obstacles[0] = {
            SCREEN_WIDTH/8,     // Left side
            200,
            30,                 // Width
            150,               // Height
            {0, 0, 30, 150}    // Rect
        };
        roundObstacles.roundConfigs[0].obstacles[1] = {
            SCREEN_WIDTH/8 * 5, // Right side
            200,
            30,
            150,
            {0, 0, 30, 150}
        };

        // Round 2: L-shaped obstacles
        roundObstacles.roundConfigs[1].obstacles[0] = {
            SCREEN_WIDTH/8,
            150,
            30,                // Vertical part
            200,
            {0, 0, 30, 200}
        };
        roundObstacles.roundConfigs[1].obstacles[1] = {
            SCREEN_WIDTH/8,
            350,
            150,              // Horizontal part
            30,
            {0, 0, 150, 30}
        };
        roundObstacles.roundConfigs[1].obstacles[2] = {
            SCREEN_WIDTH/8 * 5,
            150,
            30,
            200,
            {0, 0, 30, 200}
        };

        // Round 3: Zigzag pattern
        roundObstacles.roundConfigs[2].obstacles[0] = {
            SCREEN_WIDTH/8,
            150,
            180,              // Long horizontal
            30,
            {0, 0, 180, 30}
        };
        roundObstacles.roundConfigs[2].obstacles[1] = {
            SCREEN_WIDTH/8 * 2,
            300,
            180,
            30,
            {0, 0, 180, 30}
        };
        roundObstacles.roundConfigs[2].obstacles[2] = {
            SCREEN_WIDTH/8 * 5,
            225,
            180,
            30,
            {0, 0, 180, 30}
        };

        // Round 4: Complex maze-like pattern
        roundObstacles.roundConfigs[3].obstacles[0] = {
            SCREEN_WIDTH/8,
            150,
            30,               // Vertical
            250,
            {0, 0, 30, 250}
        };
        roundObstacles.roundConfigs[3].obstacles[1] = {
            SCREEN_WIDTH/8,
            150,
            200,             // Horizontal top
            30,
            {0, 0, 200, 30}
        };
        roundObstacles.roundConfigs[3].obstacles[2] = {
            SCREEN_WIDTH/8 * 5,
            150,
            30,              // Vertical
            250,
            {0, 0, 30, 250}
        };
        roundObstacles.roundConfigs[3].obstacles[3] = {
            SCREEN_WIDTH/8 * 3,
            370,
            200,            // Horizontal bottom
            30,
            {0, 0, 200, 30}
        };

        // Round 5: Most complex pattern
        roundObstacles.roundConfigs[4].obstacles[0] = {
            SCREEN_WIDTH/8,
            120,
            30,             // Vertical left
            350,
            {0, 0, 30, 350}
        };
        roundObstacles.roundConfigs[4].obstacles[1] = {
            SCREEN_WIDTH/8 * 2,
            200,
            200,           // Horizontal middle
            30,
            {0, 0, 200, 30}
        };
        roundObstacles.roundConfigs[4].obstacles[2] = {
            SCREEN_WIDTH/8 * 5,
            120,
            30,            // Vertical right
            350,
            {0, 0, 30, 350}
        };
        roundObstacles.roundConfigs[4].obstacles[3] = {
            SCREEN_WIDTH/8,
            400,
            150,          // Bottom left
            30,
            {0, 0, 150, 30}
        };
        roundObstacles.roundConfigs[4].obstacles[4] = {
            SCREEN_WIDTH/8 * 4,
            300,
            150,         // Bottom right
            30,
            {0, 0, 150, 30}
        };
    };

    // Add reward variables and initialization
    Reward reward1 = {
        rand() % (SCREEN_WIDTH/2 - 40) + 20,  // Random x in left side
        rand() % (SCREEN_HEIGHT - 40) + 20,    // Random y
        true,
        {0, 0, 20, 20}
    };
    
    Reward reward2 = {
        SCREEN_WIDTH/2 + rand() % (SCREEN_WIDTH/2 - 40) + 20,  // Random x in right side
        rand() % (SCREEN_HEIGHT - 40) + 20,    // Random y
        true,
        {0, 0, 20, 20}
    };

    bool isDragging = false;
    int startX = 0, startY = 0; // Vị trí bắt đầu kéo

    bool quit = false;
    SDL_Event e;

    // Load ảnh
    SDL_Surface* ballSurface = IMG_Load("assets/graphics/ball.png");
    SDL_Surface* bgSurface = IMG_Load("assets/graphics/bg.png");
    SDL_Surface* holeSurface = IMG_Load("assets/graphics/hole.png");
    SDL_Surface* pointSurface = IMG_Load("assets/graphics/point.png");
    SDL_Surface* kawaiiBgSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/kawaii.png"); // Thêm load kawaii background
    SDL_Surface* startBtnSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/start.png");
    SDL_Surface* exitBtnSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/exit (1).png");
    SDL_Surface* darkTileSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/tile32_dark.png");
    SDL_Surface* lightTileSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/tile32_light.png");
    SDL_Surface* powermeter = IMG_Load("C:/Users/Cong/golf/assets/graphics/powermeter.png");
    if (!kawaiiBgSurface) {
        cout << "Không thể load ảnh kawaii! SDL_image Error: " << IMG_GetError() << endl;
        return -1;
    }

    if (!darkTileSurface || !lightTileSurface) {
        cout << "Không thể load ảnh tile! SDL_image Error: " << IMG_GetError() << endl;
        return -1;
    }

    if (!startBtnSurface || !exitBtnSurface) {
        cout << "Không thể load ảnh button! SDL_image Error: " << IMG_GetError() << endl;
        return -1;
    }

    // Update reward texture loading with transparency
    SDL_Surface* rewardSurface = IMG_Load("C:/Users/Cong/golf/assets/hearts/hearts.png");
    if (!rewardSurface) {
        cout << "Could not load heart image! SDL_image Error: " << IMG_GetError() << endl;
        return -1;
    }
    
    // Set color key to remove black background (RGB: 0,0,0)
    SDL_SetColorKey(rewardSurface, SDL_TRUE, SDL_MapRGB(rewardSurface->format, 0, 0, 0));
    
    SDL_Surface* scaledReward = SDL_CreateRGBSurface(0, Reward::SIZE, Reward::SIZE, 32, 
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);  // Surface with alpha channel
    SDL_BlitScaled(rewardSurface, NULL, scaledReward, NULL);
    
    SDL_Texture* rewardTexture = SDL_CreateTextureFromSurface(renderer, scaledReward);
    // Enable alpha blending for the texture
    SDL_SetTextureBlendMode(rewardTexture, SDL_BLENDMODE_BLEND);
    
    SDL_FreeSurface(rewardSurface);
    SDL_FreeSurface(scaledReward);

    if (!ballSurface || !bgSurface || !holeSurface || !pointSurface) {
        cout << "Không thể load ảnh! SDL_image Error: " << IMG_GetError() << endl;
        return -1;
    }

    // Tạo texture
    SDL_Texture* ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_Texture* holeTexture = SDL_CreateTextureFromSurface(renderer, holeSurface);
    SDL_Texture* pointTexture = SDL_CreateTextureFromSurface(renderer, pointSurface);
    SDL_Texture* kawaiiBgTexture = SDL_CreateTextureFromSurface(renderer, kawaiiBgSurface); // Thêm texture kawaii
    SDL_Texture* startBtnTexture = SDL_CreateTextureFromSurface(renderer, startBtnSurface);
    SDL_Texture* exitBtnTexture = SDL_CreateTextureFromSurface(renderer, exitBtnSurface);
    SDL_Texture* darkTileTexture = SDL_CreateTextureFromSurface(renderer, darkTileSurface);
    SDL_Texture* lightTileTexture = SDL_CreateTextureFromSurface(renderer, lightTileSurface);
    SDL_Texture* powermeterTexture = SDL_CreateTextureFromSurface(renderer, powermeter);
    if (!ballTexture || !bgTexture || !holeTexture || !pointTexture || !powermeterTexture) {
        cout << "Không thể tạo texture! SDL_Error: " << SDL_GetError() << endl;
        return -1;
    }
    // Load âm thanh
    Mix_Chunk* swingSound = Mix_LoadWAV("C:/Users/Cong/golf/assets/audio/swing.mp3");
    Mix_Chunk* holeSound = Mix_LoadWAV("C:/Users/Cong/golf/assets/audio/hole.mp3");
    Mix_Chunk* bounceSound = Mix_LoadWAV("C:/Users/Cong/golf/assets/audio/charge.mp3");

    if (!swingSound || !holeSound || !bounceSound) {
        cout << "Failed to load sounds! SDL_mixer Error: " << Mix_GetError() << endl;
        return -1;
    }

    // Giải phóng surfaces
    SDL_FreeSurface(ballSurface);
    SDL_FreeSurface(bgSurface);
    SDL_FreeSurface(holeSurface);
    SDL_FreeSurface(pointSurface);
    SDL_FreeSurface(kawaiiBgSurface); // Giải phóng kawaii surface
    SDL_FreeSurface(startBtnSurface);
    SDL_FreeSurface(exitBtnSurface);
    SDL_FreeSurface(darkTileSurface);
    SDL_FreeSurface(lightTileSurface);

    // Kích thước của ball và hole
    SDL_Rect ballRect = { SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 30, 30 }; // Điều chỉnh kích thước phù hợp
    SDL_Rect holeRect = { hole1.x, hole1.y, 40, 40 }; // Điều chỉnh kích thước phù hợp

    // Add stroke counters
    int player1Strokes = 0;
    int player2Strokes = 0;

    // Add after stroke counters
    int roundCounter = 1;  // Track game rounds

    // Add function to reset rewards (add before game loop)
    auto resetRewards = [&]() {
        reward1 = {
            rand() % (SCREEN_WIDTH/2 - 40) + 20,
            rand() % (SCREEN_HEIGHT - 40) + 20,
            true,
            {0, 0, 20, 20}
        };
        reward2 = {
            SCREEN_WIDTH/2 + rand() % (SCREEN_WIDTH/2 - 40) + 20,
            rand() % (SCREEN_HEIGHT - 40) + 20,
            true,
            {0, 0, 20, 20}
        };
    };

    // Initialize rewards as inactive initially
    reward1.active = false;
    reward2.active = false;

    // Add after SDL initialization and before game loop
    enum GameState {
        MENU,
        PLAYING
    };
    GameState gameState = MENU;

    // Menu buttons - Chỉnh sửa kích thước và vị trí
    SDL_Rect playButton = {
        SCREEN_WIDTH/2 - 125,  // Căn giữa ngang, thêm margin
        SCREEN_HEIGHT/2 - 100, // Đưa lên cao hơn
        250,                   // Tăng chiều rộng
        80                     // Tăng chiều cao
    };
    SDL_Rect exitButton = {
        SCREEN_WIDTH/2 - 125,  // Căn giữa ngang, giữ nguyên alignment với nút play
        SCREEN_HEIGHT/2 + 20,  // Giảm khoảng cách với nút play
        250,                   // Giữ kích thước giống nút play
        80                     // Giữ kích thước giống nút play
    };

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            // Handle menu events
            if (gameState == MENU) {
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX = e.button.x;
                    int mouseY = e.button.y;
                    
                    // Check play button
                    if (mouseX >= playButton.x && mouseX <= playButton.x + playButton.w &&
                        mouseY >= playButton.y && mouseY <= playButton.y + playButton.h) {
                        gameState = PLAYING;
                    }
                    
                    // Check exit button
                    if (mouseX >= exitButton.x && mouseX <= exitButton.x + exitButton.w &&
                        mouseY >= exitButton.y && mouseY <= exitButton.y + exitButton.h) {
                        quit = true;
                    }
                }
                continue;  // Skip game logic in menu
            }

            // Existing game event handling
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

            // Update mouse button up event handler
            if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
                if (isDragging) {
                    isDragging = false;
                    isPoweringUp = false;
                    
                    float power = currentPower * POWER_MULTIPLIER;
                    // More precise velocity calculations
                    activeBall.speedX = roundf(cos(aimAngle) * power * 100) / 100.0f;
                    activeBall.speedY = roundf(sin(aimAngle) * power * 100) / 100.0f;
                    
                    // Increment stroke counter for current player
                    if (currentBall == 1) {
                        player1Strokes++;
                    } else {
                        player2Strokes++;
                    }

                    Mix_PlayChannel(-1, swingSound, 0); // Phát âm thanh đánh bóng
                }
            }
        }

        // Render based on game state
        if (gameState == MENU) {
            // Render menu với kawaii background
            SDL_RenderCopy(renderer, kawaiiBgTexture, NULL, NULL); // Render kawaii background

            // Render button images instead of rectangles
            SDL_RenderCopy(renderer, startBtnTexture, NULL, &playButton);
            SDL_RenderCopy(renderer, exitBtnTexture, NULL, &exitButton);

            SDL_RenderPresent(renderer);
            continue;  // Skip game rendering in menu
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

            // Check reward collision
            if (ball == &ball1 && reward1.active) {
                int dx = ball->x - reward1.x;
                int dy = ball->y - reward1.y;
                if (sqrt(dx * dx + dy * dy) <= ballRadius + Reward::SIZE/2) {
                    reward1.active = false;
                    player1Strokes = max(0, player1Strokes - 2);  // Reduce strokes by 2
                }
            } else if (ball == &ball2 && reward2.active) {
                int dx = ball->x - reward2.x;
                int dy = ball->y - reward2.y;
                if (sqrt(dx * dx + dy * dy) <= ballRadius + Reward::SIZE/2) {
                    reward2.active = false;
                    player2Strokes = max(0, player2Strokes - 2);  // Reduce strokes by 2
                }
            }

            // Bounce off walls with improved collision detection
            if (ball->x < ballRadius) {
                ball->x = ballRadius;
                ball->speedX = -ball->speedX * Ball::BOUNCE;
                Mix_PlayChannel(-1, bounceSound, 0); // Thêm âm thanh nảy tường
            } else if (ball->x > SCREEN_WIDTH - ballRadius) {
                ball->x = SCREEN_WIDTH - ballRadius;
                ball->speedX = -ball->speedX * Ball::BOUNCE;
                Mix_PlayChannel(-1, bounceSound, 0); // Thêm âm thanh nảy tường
            }

            // Add center line collision
            if (ball == &ball1 && ball->x > SCREEN_WIDTH/2 - ballRadius) {
                ball->x = SCREEN_WIDTH/2 - ballRadius;
                ball->speedX = -ball->speedX * Ball::BOUNCE;
                Mix_PlayChannel(-1, bounceSound, 0); // Thêm âm thanh nảy tường giữa
            } else if (ball == &ball2 && ball->x < SCREEN_WIDTH/2 + ballRadius) {
                ball->x = SCREEN_WIDTH/2 + ballRadius;
                ball->speedX = -ball->speedX * Ball::BOUNCE;
                Mix_PlayChannel(-1, bounceSound, 0); // Thêm âm thanh nảy tường giữa
            }

            if (ball->y < ballRadius) {
                ball->y = ballRadius;
                ball->speedY = -ball->speedY * Ball::BOUNCE;
                Mix_PlayChannel(-1, bounceSound, 0); // Thêm âm thanh nảy tường
            } else if (ball->y > SCREEN_HEIGHT - ballRadius) {
                ball->y = SCREEN_HEIGHT - ballRadius;
                ball->speedY = -ball->speedY * Ball::BOUNCE;
                Mix_PlayChannel(-1, bounceSound, 0); // Thêm âm thanh nảy tường
            }

            // In the ball position update loop, trong phần xử lý va chạm với obstacle
            if (ball == &ball1) {
                for(int i = 0; i < leftObstacles.count; i++) {
                    Obstacle& obs = leftObstacles.obstacles[i];
                    if (ball->x + ballRadius > obs.x && 
                        ball->x - ballRadius < obs.x + obs.width &&
                        ball->y + ballRadius > obs.y && 
                        ball->y - ballRadius < obs.y + obs.height) {
                        
                        Mix_PlayChannel(-1, bounceSound, 0); // Phát âm thanh nảy

                        // Xác định hướng va chạm
                        float overlapLeft = (ball->x + ballRadius) - obs.x;
                        float overlapRight = (obs.x + obs.width) - (ball->x - ballRadius);
                        float overlapTop = (ball->y + ballRadius) - obs.y;
                        float overlapBottom = (obs.y + obs.height) - (ball->y - ballRadius);
                        
                        // Tìm overlap nhỏ nhất bằng cách so sánh từng cặp
                        float minOverlapX = overlapLeft < overlapRight ? overlapLeft : overlapRight;
                        float minOverlapY = overlapTop < overlapBottom ? overlapTop : overlapBottom;
                        
                        // So sánh overlap theo X và Y để xác định hướng va chạm
                        if (minOverlapX < minOverlapY) {
                            // Va chạm ngang
                            if (overlapLeft < overlapRight) {
                                ball->x = obs.x - ballRadius;
                                ball->speedX = -ball->speedX * Ball::OBSTACLE_BOUNCE;
                            } else {
                                ball->x = obs.x + obs.width + ballRadius;
                                ball->speedX = -ball->speedX * Ball::OBSTACLE_BOUNCE;
                            }
                            ball->speedY *= Ball::OBSTACLE_FRICTION;
                        } else {
                            // Va chạm dọc
                            if (overlapTop < overlapBottom) {
                                ball->y = obs.y - ballRadius;
                                ball->speedY = -ball->speedY * Ball::OBSTACLE_BOUNCE;
                            } else {
                                ball->y = obs.y + obs.height + ballRadius;
                                ball->speedY = -ball->speedY * Ball::OBSTACLE_BOUNCE;
                            }
                            ball->speedX *= Ball::OBSTACLE_FRICTION;
                        }
                        
                        // Thêm một chút ngẫu nhiên để tạo hiệu ứng tự nhiên hơn
                        float randomFactor = 0.98f + (rand() % 5) / 100.0f; // 0.98 - 1.02
                        ball->speedX *= randomFactor;
                        ball->speedY *= randomFactor;
                    }
                }
            }
            // Tương tự cho ball2
            else {
                for(int i = 0; i < rightObstacles.count; i++) {
                    Obstacle& obs = rightObstacles.obstacles[i];
                    if (ball->x + ballRadius > obs.x && 
                        ball->x - ballRadius < obs.x + obs.width &&
                        ball->y + ballRadius > obs.y && 
                        ball->y - ballRadius < obs.y + obs.height) {
                        
                        Mix_PlayChannel(-1, bounceSound, 0); // Phát âm thanh nảy

                        // Xác định hướng va chạm
                        float overlapLeft = (ball->x + ballRadius) - obs.x;
                        float overlapRight = (obs.x + obs.width) - (ball->x - ballRadius);
                        float overlapTop = (ball->y + ballRadius) - obs.y;
                        float overlapBottom = (obs.y + obs.height) - (ball->y - ballRadius);
                        
                        // Tìm overlap nhỏ nhất bằng cách so sánh từng cặp
                        float minOverlapX = overlapLeft < overlapRight ? overlapLeft : overlapRight;
                        float minOverlapY = overlapTop < overlapBottom ? overlapTop : overlapBottom;
                        
                        // So sánh overlap theo X và Y để xác định hướng va chạm
                        if (minOverlapX < minOverlapY) {
                            // Va chạm ngang
                            if (overlapLeft < overlapRight) {
                                ball->x = obs.x - ballRadius;
                                ball->speedX = -ball->speedX * Ball::OBSTACLE_BOUNCE;
                            } else {
                                ball->x = obs.x + obs.width + ballRadius;
                                ball->speedX = -ball->speedX * Ball::OBSTACLE_BOUNCE;
                            }
                            ball->speedY *= Ball::OBSTACLE_FRICTION;
                        } else {
                            // Va chạm dọc
                            if (overlapTop < overlapBottom) {
                                ball->y = obs.y - ballRadius;
                                ball->speedY = -ball->speedY * Ball::OBSTACLE_BOUNCE;
                            } else {
                                ball->y = obs.y + obs.height + ballRadius;
                                ball->speedY = -ball->speedY * Ball::OBSTACLE_BOUNCE;
                            }
                            ball->speedX *= Ball::OBSTACLE_FRICTION;
                        }
                        
                        // Thêm một chút ngẫu nhiên để tạo hiệu ứng tự nhiên hơn
                        float randomFactor = 0.98f + (rand() % 5) / 100.0f; // 0.98 - 1.02
                        ball->speedX *= randomFactor;
                        ball->speedY *= randomFactor;
                    }
                }
            }

            // Kiểm tra va chạm với lỗ tương ứng
            Hole& targetHole = (ball == &ball1) ? hole1 : hole2;
            int dx = ball->x - targetHole.x;
            int dy = ball->y - targetHole.y;
            
            if (sqrt(dx * dx + dy * dy) <= ballRadius + targetHole.radius) {
                if (!ball->isAnimating) {
                    Mix_PlayChannel(-1, holeSound, 0); // Phát âm thanh vào lỗ
                    ball->isAnimating = true;
                    ball->animationTimer = Ball::ANIMATION_DURATION;
                    ball->speedX = 0;
                    ball->speedY = 0;
                    // Lưu vị trí đích là tâm của lỗ
                    ball->targetX = targetHole.x;
                    ball->targetY = targetHole.y;
                }
                
                // Xử lý animation
                if (ball->isAnimating) {
                    ball->animationTimer -= 0.016f;
                    
                    // Di chuyển bóng về tâm lỗ
                    float dx = ball->targetX - ball->x;
                    float dy = ball->targetY - ball->y;
                    float dist = sqrt(dx * dx + dy * dy);
                    
                    if (dist > 1.0f) { // Nếu còn cách xa trung tâm
                        float moveX = (dx / dist) * Ball::MOVE_SPEED;
                        float moveY = (dy / dist) * Ball::MOVE_SPEED;
                        ball->x += moveX;
                        ball->y += moveY;
                    } else {
                        ball->x = ball->targetX;
                        ball->y = ball->targetY;
                    }
                    
                    // Cập nhật scale
                    ball->scale = ball->animationTimer / Ball::ANIMATION_DURATION;
                    
                    // Rest of the animation handling
                    if (ball->animationTimer <= 0) {
                        ball->isAnimating = false;
                        ball->scale = 1.0f;
                        
                        // Xử lý logic chuyển lượt/kết thúc màn như cũ
                        if (ball == &ball1) {
                            gameScore.totalStrokes1 += player1Strokes;
                            currentBall = 2;
                            ball2.isActive = true;
                            ball1.isActive = false;
                            ball1.x = SCREEN_WIDTH / 4;
                            ball1.y = SCREEN_HEIGHT - 50;
                        } else {
                            gameScore.totalStrokes2 += player2Strokes;  // Cộng dồn vào tổng
                            currentBall = 1;  // Switch back to player 1
                            ball1.isActive = true;
                            ball2.isActive = false;
                            
                            // End of round - both players have completed
                            gameScore.currentRound++;
                            
                            // Check if game is over
                            if (gameScore.currentRound > gameScore.maxRounds) {
                                gameScore.isGameOver = true;
                                gameState = MENU;  // Return to menu
                                // Reset game for next time
                                gameScore.currentRound = 1;
                                gameScore.player1Score = 0;
                                gameScore.player2Score = 0;
                                gameScore.totalStrokes1 = 0;  // Reset tổng strokes
                                gameScore.totalStrokes2 = 0;  // Reset tổng strokes
                            } else {
                                // Set up next round
                                // updateHolePosition(hole1, true);
                                // updateHolePosition(hole2, false);
                                // Reset ball positions
                                ball1.x = SCREEN_WIDTH / 4;
                                ball1.y = SCREEN_HEIGHT - 50;
                                ball2.x = 3 * SCREEN_WIDTH / 4;
                                ball2.y = SCREEN_HEIGHT - 50;
                                
                                // Update obstacles for new round
                                leftObstacles = roundObstacles.roundConfigs[gameScore.currentRound - 1];
                                // Mirror obstacles for right side
                                rightObstacles.count = leftObstacles.count;
                                for(int i = 0; i < leftObstacles.count; i++) {
                                    rightObstacles.obstacles[i] = leftObstacles.obstacles[i];
                                    rightObstacles.obstacles[i].x += SCREEN_WIDTH/2;  // Shift to right side
                                    rightObstacles.obstacles[i].rect.x = rightObstacles.obstacles[i].x;
                                }
                                
                                // Reset and activate rewards for new round
                                reward1 = {
                                    rand() % (SCREEN_WIDTH/2 - 40) + 20,
                                    rand() % (SCREEN_HEIGHT - 40) + 20,
                                    true,
                                    {0, 0, 20, 20}
                                };
                                reward2 = {
                                    SCREEN_WIDTH/2 + rand() % (SCREEN_WIDTH/2 - 40) + 20,
                                    rand() % (SCREEN_HEIGHT - 40) + 20,
                                    true,
                                    {0, 0, 20, 20}
                                };

                                // Make obstacles longer/bigger in round 2
                                if (gameScore.currentRound == 2) {
                                    for (int i = 0; i < leftObstacles.count; i++) {
                                        leftObstacles.obstacles[i].height = 150;
                                        leftObstacles.obstacles[i].rect.h = 150;
                                    }
                                    for (int i = 0; i < rightObstacles.count; i++) {
                                        rightObstacles.obstacles[i].height = 150;
                                        rightObstacles.obstacles[i].rect.h = 150;
                                    }
                                }

                                // Make diverse obstacles for both players
                                for (int i = 0; i < leftObstacles.count; i++) {
                                    bool isHorizontal = (rand() % 2) == 0;  // Random orientation
                                    int baseSize = 100 + (gameScore.currentRound * 20);  // Size increases with rounds
                                    
                                    // Set dimensions based on orientation
                                    int obsWidth = isHorizontal ? baseSize : 30;
                                    int obsHeight = isHorizontal ? 30 : baseSize;
                                    
                                    // Generate random position that works for both sides
                                    int obsX = rand() % (SCREEN_WIDTH/4 - obsWidth) + SCREEN_WIDTH/8;  // Center in left quarter
                                    int obsY = rand() % (SCREEN_HEIGHT - obsHeight - 100) + 50;
                                    
                                    // Set identical obstacles on both sides
                                    // Left side
                                    leftObstacles.obstacles[i] = {
                                        obsX,
                                        obsY,
                                        obsWidth,
                                        obsHeight,
                                        {obsX, obsY, obsWidth, obsHeight}
                                    };
                                    
                                    // Right side - same relative position
                                    rightObstacles.obstacles[i] = {
                                        obsX + SCREEN_WIDTH/2,  // Shift by half screen
                                        obsY,
                                        obsWidth,
                                        obsHeight,
                                        {obsX + SCREEN_WIDTH/2, obsY, obsWidth, obsHeight}
                                    };
                                }
                            }
                        }
                        
                        // Reset ball physics
                        ball->speedX = 0;
                        ball->speedY = 0;
                    }
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

        // Update obstacle rectangles
        for (int i = 0; i < leftObstacles.count; i++) {
            leftObstacles.obstacles[i].rect.x = leftObstacles.obstacles[i].x;
            leftObstacles.obstacles[i].rect.y = leftObstacles.obstacles[i].y;
        }
        for (int i = 0; i < rightObstacles.count; i++) {
            rightObstacles.obstacles[i].rect.x = rightObstacles.obstacles[i].x;
            rightObstacles.obstacles[i].rect.y = rightObstacles.obstacles[i].y;
        }

        // Render background
        SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
        
        // Vẽ đường phân chia giữa 2 bên
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT);
        
        // Render các lỗ
        SDL_RenderCopy(renderer, holeTexture, NULL, &hole1.rect);
        SDL_RenderCopy(renderer, holeTexture, NULL, &hole2.rect);
        
        // Render both balls với scale animation
        for (Ball* ball : {&ball1, &ball2}) {
            SDL_Rect scaledRect = ball->rect;
            if (ball->isAnimating) {
                // Tính toán kích thước và vị trí mới dựa trên scale
                int newWidth = static_cast<int>(ball->rect.w * ball->scale);
                int newHeight = static_cast<int>(ball->rect.h * ball->scale);
                scaledRect.w = newWidth;
                scaledRect.h = newHeight;
                // Căn giữa rect với vị trí bóng
                scaledRect.x = ball->x - newWidth/2;
                scaledRect.y = ball->y - newHeight/2;
            }
            SDL_RenderCopy(renderer, ballTexture, NULL, &scaledRect);
        }

        // Update the reward rendering section
        // Render rewards if active
        if (reward1.active) {
            reward1.rect = {
                reward1.x - Reward::SIZE/2,
                reward1.y - Reward::SIZE/2,
                Reward::SIZE,
                Reward::SIZE
            };
            SDL_RenderCopy(renderer, rewardTexture, NULL, &reward1.rect);
        }
        if (reward2.active) {
            reward2.rect = {
                reward2.x - Reward::SIZE/2,
                reward2.y - Reward::SIZE/2,
                Reward::SIZE,
                Reward::SIZE
            };
            SDL_RenderCopy(renderer, rewardTexture, NULL, &reward2.rect);
        }

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

        // Render stroke counts
        SDL_Color textColor = {255, 255, 255, 255};
        
        // Player 1 strokes
        char strokeText1[64];
        sprintf(strokeText1, "Round: %d Strokes: %d Total: %d", 
                gameScore.currentRound, player1Strokes, gameScore.totalStrokes1);
        SDL_Surface* strokeSurface1 = TTF_RenderText_Solid(font, strokeText1, textColor);
        SDL_Texture* strokeTexture1 = SDL_CreateTextureFromSurface(renderer, strokeSurface1);
        SDL_Rect strokeRect1 = {20, 20, strokeSurface1->w, strokeSurface1->h};
        SDL_RenderCopy(renderer, strokeTexture1, NULL, &strokeRect1);
        SDL_FreeSurface(strokeSurface1);
        SDL_DestroyTexture(strokeTexture1);

        // Player 2 strokes
        char strokeText2[64];
        sprintf(strokeText2, "Round: %d Strokes: %d Total: %d", 
                gameScore.currentRound, player2Strokes, gameScore.totalStrokes2);
        SDL_Surface* strokeSurface2 = TTF_RenderText_Solid(font, strokeText2, textColor);
        SDL_Texture* strokeTexture2 = SDL_CreateTextureFromSurface(renderer, strokeSurface2);
        SDL_Rect strokeRect2 = {
            SCREEN_WIDTH - strokeSurface2->w - 20, // 20 pixels margin from right edge
            20,
            strokeSurface2->w,
            strokeSurface2->h
        };
        SDL_RenderCopy(renderer, strokeTexture2, NULL, &strokeRect2);
        SDL_FreeSurface(strokeSurface2);
        SDL_DestroyTexture(strokeTexture2);

        // Update score display
        char scoreText1[64];
        sprintf(scoreText1, "Player 1: %d (%d)", gameScore.player1Score, player1Strokes);
        // ...existing score rendering code...

        char scoreText2[64];
        sprintf(scoreText2, "Player 2: %d (%d)", gameScore.player2Score, player2Strokes);
        // ...existing score rendering code...

        // Add round display
        char roundText[32];
        sprintf(roundText, "Round: %d/%d", gameScore.currentRound, gameScore.maxRounds);
        SDL_Surface* roundSurface = TTF_RenderText_Solid(font, roundText, textColor);
        SDL_Texture* roundTexture = SDL_CreateTextureFromSurface(renderer, roundSurface);
        SDL_Rect roundRect = {SCREEN_WIDTH/2 - roundSurface->w/2, 20, roundSurface->w, roundSurface->h};
        SDL_RenderCopy(renderer, roundTexture, NULL, &roundRect);
        SDL_FreeSurface(roundSurface);
        SDL_DestroyTexture(roundTexture);

        // Render obstacles with textures
        for(int i = 0; i < leftObstacles.count; i++) {
            SDL_RenderCopy(renderer, darkTileTexture, NULL, &leftObstacles.obstacles[i].rect);
        }
        for(int i = 0; i < rightObstacles.count; i++) {
            SDL_RenderCopy(renderer, lightTileTexture, NULL, &rightObstacles.obstacles[i].rect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    // Giải phóng textures
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(holeTexture);
    SDL_DestroyTexture(pointTexture);
    SDL_DestroyTexture(kawaiiBgTexture); // Giải phóng kawaii texture
    SDL_DestroyTexture(startBtnTexture);
    SDL_DestroyTexture(exitBtnTexture);
    SDL_DestroyTexture(rewardTexture);
    SDL_DestroyTexture(darkTileTexture);
    SDL_DestroyTexture(lightTileTexture);

    // Giải phóng âm thanh trước khi thoát
    Mix_FreeChunk(swingSound);
    Mix_FreeChunk(holeSound);
    Mix_FreeChunk(bounceSound);
    Mix_CloseAudio();

    // Giải phóng tài nguyên
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit(); // Thêm dòng này trước SDL_Quit()
    SDL_Quit();

    return 0;
}
