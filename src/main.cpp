#include "game.h"

// Global game variables
RoundObstacles roundObstacles;
ObstacleSet leftObstacles = {{{0}}, 1};
ObstacleSet rightObstacles = {{{0}}, 1};
GameScore gameScore;
GameSettings settings;
Ball ball1;
Ball ball2;
Hole hole1;
Hole hole2;
Reward reward1;
Reward reward2;
int currentBall = 1;
int player1Strokes = 0;
int player2Strokes = 0;
GameState gameState = MENU;
bool isDragging = false;
bool isPoweringUp = false;
float currentPower = 0;
float aimAngle = 0;
int ballRadius = 10;

// Khai báo trước các hàm xử lý sự kiện
void handleMenuEvents(SDL_Event& e, SDL_Renderer* renderer, TTF_Font* font, 
                     SDL_Texture* settingsBgTexture, SDL_Texture* menuBgTexture,
                     SDL_Texture* playBrightTexture, SDL_Texture* menuBrightTexture,
                     SDL_Texture* exitBrightTexture, bool& quit);
                     
void handleGameEvents(SDL_Event& e, Mix_Chunk* swingSound, Mix_Chunk* bounceSound, float& powerIncrement);

int main(int argc, char* argv[]) {
    // Initialize SDL and create window
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!initSDL(&window, &renderer)) {
        std::cout << "Failed to initialize SDL!" << std::endl;
        return -1;
    }

    // Load media
    TTF_Font* font = nullptr;
    SDL_Texture* ballTexture = nullptr;
    SDL_Texture* bgTexture = nullptr;
    SDL_Texture* holeTexture = nullptr;
    SDL_Texture* pointTexture = nullptr;
    SDL_Texture* menuBgTexture = nullptr;
    SDL_Texture* darkTileTexture = nullptr;
    SDL_Texture* lightTileTexture = nullptr;
    SDL_Texture* powermeterTexture = nullptr;
    SDL_Texture* rewardTexture = nullptr;
    SDL_Texture* settingsBgTexture = nullptr;
    SDL_Texture* playBrightTexture = nullptr;
    SDL_Texture* menuBrightTexture = nullptr;
    SDL_Texture* exitBrightTexture = nullptr;
    SDL_Texture* gameOverTexture = nullptr;
    Mix_Chunk* swingSound = nullptr;
    Mix_Chunk* holeSound = nullptr;
    Mix_Chunk* bounceSound = nullptr;

    if (!loadMedia(renderer, &font, &ballTexture, &bgTexture, &holeTexture, &pointTexture,
                  &menuBgTexture, &darkTileTexture, &lightTileTexture, &powermeterTexture,
                  &rewardTexture, &settingsBgTexture, &playBrightTexture, &menuBrightTexture,
                  &exitBrightTexture, &gameOverTexture, &swingSound, &holeSound, &bounceSound)) {
        std::cout << "Failed to load media!" << std::endl;
        return -1;
    }

    // Initialize game objects
    initializeGame();
    initializeRoundObstacles();

    // Main game loop variables
    bool quit = false;
    SDL_Event e;
    float powerIncrement = 2.0f;

    // Main game loop
    while (!quit) {
        // Xử lý sự kiện trực tiếp trong vòng lặp chính
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            // Xử lý sự kiện dựa trên trạng thái game
            switch (gameState) {
                case MENU:
                    handleMenuEvents(e, renderer, font, settingsBgTexture, menuBgTexture,
                                    playBrightTexture, menuBrightTexture, exitBrightTexture, quit);
                    break;
                case PLAYING:
                    handleGameEvents(e, swingSound, bounceSound, powerIncrement);
                    break;
                case GAME_OVER:
                    // Game over events
                    if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                        gameState = MENU;
                        // Reset game state
                        gameScore.currentRound = 1;
                        gameScore.player1Score = 0;
                        gameScore.player2Score = 0;
                        gameScore.totalStrokes1 = 0;
                        gameScore.totalStrokes2 = 0;
                        gameScore.isGameOver = false;
                        player1Strokes = 0;
                        player2Strokes = 0;
                        
                        // Reset ball positions
                        ball1.x = SCREEN_WIDTH / 4;
                        ball1.y = SCREEN_HEIGHT - 50;
                        ball2.x = 3 * SCREEN_WIDTH / 4;
                        ball2.y = SCREEN_HEIGHT - 50;
                    }
                    break;
                default:
                    break;
            }
        }

        // Update game state
        if (gameState == PLAYING) {
            updateGame(bounceSound, holeSound);
            updatePower(powerIncrement);
            
            // Kiểm tra game over
            if (gameScore.isGameOver) {
                gameState = GAME_OVER;
            }
        }

        // Render
        if (gameState == MENU) {
            // Clear screen with black
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Render default background
            SDL_RenderCopy(renderer, menuBgTexture, NULL, NULL);

            // Get mouse position
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);

            // Define hit box rectangles
            SDL_Rect playHitBox = {291, 235, 210, 60};
            SDL_Rect menuHitBox = {291, 322, 210, 60};
            SDL_Rect exitHitBox = {291, 422, 210, 60};

            // Check hover states without drawing rectangles
            if (mouseX >= playHitBox.x && mouseX <= playHitBox.x + playHitBox.w &&
                mouseY >= playHitBox.y && mouseY <= playHitBox.y + playHitBox.h) {
                SDL_RenderCopy(renderer, playBrightTexture, NULL, NULL);
            }
            else if (mouseX >= menuHitBox.x && mouseX <= menuHitBox.x + menuHitBox.w &&
                    mouseY >= menuHitBox.y && mouseY <= menuHitBox.y + menuHitBox.h) {
                SDL_RenderCopy(renderer, menuBrightTexture, NULL, NULL);
            }
            else if (mouseX >= exitHitBox.x && mouseX <= exitHitBox.x + exitHitBox.w &&
                    mouseY >= exitHitBox.y && mouseY <= exitHitBox.y + exitHitBox.h) {
                SDL_RenderCopy(renderer, exitBrightTexture, NULL, NULL);
            }

            SDL_RenderPresent(renderer);
        } 
        else if (gameState == PLAYING) {
            renderGame(renderer, font, ballTexture, bgTexture, holeTexture, pointTexture,
                      darkTileTexture, lightTileTexture, rewardTexture);
        }
        else if (gameState == GAME_OVER) {
            renderGameOver(renderer, font, gameOverTexture);
        }

        // Small delay to control frame rate
        SDL_Delay(10);
    }

    // Clean up resources
    cleanup(window, renderer, font, ballTexture, bgTexture, holeTexture, pointTexture,
           menuBgTexture, darkTileTexture, lightTileTexture, powermeterTexture,
           rewardTexture, settingsBgTexture, playBrightTexture, menuBrightTexture,
           exitBrightTexture, gameOverTexture, swingSound, holeSound, bounceSound);

    return 0;
}

// Hàm xử lý sự kiện menu
void handleMenuEvents(SDL_Event& e, SDL_Renderer* renderer, TTF_Font* font, 
                     SDL_Texture* settingsBgTexture, SDL_Texture* menuBgTexture,
                     SDL_Texture* playBrightTexture, SDL_Texture* menuBrightTexture,
                     SDL_Texture* exitBrightTexture, bool& quit) {
    // Get mouse position
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Define hit box rectangles
    SDL_Rect playHitBox = {291, 235, 210, 60};
    SDL_Rect menuHitBox = {291, 322, 210, 60};
    SDL_Rect exitHitBox = {291, 422, 210, 60};

    // Handle clicks
    if (e.type == SDL_MOUSEBUTTONDOWN) {
        std::cout << "Mouse clicked at: " << mouseX << ", " << mouseY << std::endl;
        
        if (mouseX >= exitHitBox.x && mouseX <= exitHitBox.x + exitHitBox.w &&
            mouseY >= exitHitBox.y && mouseY <= exitHitBox.y + exitHitBox.h) {
            std::cout << "Exit button clicked!" << std::endl;
            quit = true;
        }
        else if (mouseX >= playHitBox.x && mouseX <= playHitBox.x + playHitBox.w &&
                mouseY >= playHitBox.y && mouseY <= playHitBox.y + playHitBox.h) {
            std::cout << "Play button clicked! Changing state to PLAYING" << std::endl;
            gameState = PLAYING;
            // Reset game state for new game
            initializeGame();
            gameState = PLAYING; // Đảm bảo trạng thái được đặt lại
        }
        else if (mouseX >= menuHitBox.x && mouseX <= menuHitBox.x + menuHitBox.w &&
                mouseY >= menuHitBox.y && mouseY <= menuHitBox.y + menuHitBox.h) {
            std::cout << "Settings button clicked!" << std::endl;
            // Show settings menu
            bool inSettings = true;
            while (inSettings && !quit) {
                while (SDL_PollEvent(&e) != 0) {
                    if (e.type == SDL_QUIT) {
                        quit = true;
                        inSettings = false;
                    }
                    if (e.type == SDL_MOUSEBUTTONDOWN) {
                        int settingsMouseX = e.button.x;
                        int settingsMouseY = e.button.y;
                        
                        // Check speed button clicks
                        for(int i = 0; i < 3; i++) {
                            if (settingsMouseY >= 250 + i*50 && settingsMouseY <= 280 + i*50 &&
                                settingsMouseX >= SCREEN_WIDTH/2 - 50 && settingsMouseX <= SCREEN_WIDTH/2 + 50) {
                                settings.speed = (BallSpeed)i;
                                break;
                            }
                        }
                        
                        // Check sound button click
                        if (settingsMouseY >= 400 && settingsMouseY <= 430 &&
                            settingsMouseX >= SCREEN_WIDTH/2 - 50 && settingsMouseX <= SCREEN_WIDTH/2 + 50) {
                            settings.soundEnabled = !settings.soundEnabled;
                            if (!settings.soundEnabled) {
                                Mix_Volume(-1, 0); // Mute all channels
                            } else {
                                Mix_Volume(-1, MIX_MAX_VOLUME); // Restore volume
                            }
                        }
                        
                        // Check back button click
                        if (settingsMouseY >= 450 && settingsMouseY <= 480 &&
                            settingsMouseX >= SCREEN_WIDTH/2 - 50 && settingsMouseX <= SCREEN_WIDTH/2 + 50) {
                            inSettings = false;
                        }
                    }
                }
                
                // Render settings menu
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderClear(renderer);
                renderSettingsMenu(renderer, font, settings, settingsBgTexture);
                SDL_RenderPresent(renderer);
            }
        }
    }
}

// Hàm xử lý sự kiện trong game
void handleGameEvents(SDL_Event& e, Mix_Chunk* swingSound, Mix_Chunk* bounceSound, float& powerIncrement) {
    Ball& activeBall = (currentBall == 1) ? ball1 : ball2;

    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
        // Check if mouse clicked on the ball
        int dx = e.button.x - activeBall.x;
        int dy = e.button.y - activeBall.y;
        if (sqrt(dx * dx + dy * dy) <= ballRadius) {
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
            
            float power = currentPower * settings.speedMultiplier[settings.speed];
            // More precise velocity calculations
            activeBall.speedX = roundf(cos(aimAngle) * power * 100) / 100.0f;
            activeBall.speedY = roundf(sin(aimAngle) * power * 100) / 100.0f;
            
            // Increment stroke counter for current player
            if (currentBall == 1) {
                player1Strokes++;
            } else {
                player2Strokes++;
            }

            if (settings.soundEnabled) {
                Mix_PlayChannel(-1, swingSound, 0); // Play swing sound
            }
        }
    }
    
    // Thêm phím tắt để quay lại menu (để test)
    if (e.type == SDL_KEYDOWN) {
        if (e.key.keysym.sym == SDLK_ESCAPE) {
            std::cout << "ESC pressed, returning to menu" << std::endl;
            gameState = MENU;
        }
    }
}
