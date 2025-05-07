#include "game.h"

bool initSDL(SDL_Window** window, SDL_Renderer** renderer) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL không thể khởi tạo! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "SDL_image không thể khởi tạo! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    // Create window
    *window = SDL_CreateWindow("Let's Golf", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        std::cout << "Không thể tạo cửa sổ! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Create renderer
    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        std::cout << "Không thể tạo renderer! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

bool loadMedia(SDL_Renderer* renderer, TTF_Font** font, 
               SDL_Texture** ballTexture, SDL_Texture** bgTexture, 
               SDL_Texture** holeTexture, SDL_Texture** pointTexture,
               SDL_Texture** menuBgTexture, SDL_Texture** darkTileTexture,
               SDL_Texture** lightTileTexture, SDL_Texture** powermeterTexture,
               SDL_Texture** rewardTexture, SDL_Texture** settingsBgTexture,
               SDL_Texture** playBrightTexture, SDL_Texture** menuBrightTexture,
               SDL_Texture** exitBrightTexture, SDL_Texture** gameOverTexture,
               Mix_Chunk** swingSound, Mix_Chunk** holeSound, Mix_Chunk** bounceSound) {
    // Load font
    *font = TTF_OpenFont("C:/Users/Cong/golf/assets/font/Arial-bold.ttf", 24);
    if (!*font) {
        std::cout << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return false;
    }

    // Load images
    SDL_Surface* ballSurface = IMG_Load("assets/graphics/ball.png");
    SDL_Surface* bgSurface = IMG_Load("assets/graphics/bg.png");
    SDL_Surface* holeSurface = IMG_Load("assets/graphics/hole.png");
    SDL_Surface* pointSurface = IMG_Load("assets/graphics/point.png");
    SDL_Surface* menuBgSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/bg1.png");
    SDL_Surface* darkTileSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/tile32_dark.png");
    SDL_Surface* lightTileSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/tile32_light.png");
    SDL_Surface* powermeterSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/powermeter.png");
    SDL_Surface* settingsBgSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/menu_settings.png");
    SDL_Surface* playBrightSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/playyy_bright.png");
    SDL_Surface* menuBrightSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/menu_bright.png");
    SDL_Surface* exitBrightSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/exit_bright.png");
    SDL_Surface* gameOverSurface = IMG_Load("C:/Users/Cong/golf/assets/graphics/gameoverbg.png");

    // Check if all surfaces loaded correctly
    if (!ballSurface || !bgSurface || !holeSurface || !pointSurface || !menuBgSurface || 
        !darkTileSurface || !lightTileSurface || !powermeterSurface || !settingsBgSurface || 
        !playBrightSurface || !menuBrightSurface || !exitBrightSurface || !gameOverSurface) {
        std::cout << "Không thể load ảnh! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Load reward image with transparency
    SDL_Surface* rewardSurface = IMG_Load("C:/Users/Cong/golf/assets/hearts/hearts.png");
    if (!rewardSurface) {
        std::cout << "Could not load heart image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    
    // Set color key to remove black background
    SDL_SetColorKey(rewardSurface, SDL_TRUE, SDL_MapRGB(rewardSurface->format, 0, 0, 0));
    
    SDL_Surface* scaledReward = SDL_CreateRGBSurface(0, Reward::SIZE, Reward::SIZE, 32, 
        0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    SDL_BlitScaled(rewardSurface, NULL, scaledReward, NULL);
    
    *rewardTexture = SDL_CreateTextureFromSurface(renderer, scaledReward);
    SDL_SetTextureBlendMode(*rewardTexture, SDL_BLENDMODE_BLEND);
    
    SDL_FreeSurface(rewardSurface);
    SDL_FreeSurface(scaledReward);

    // Create textures from surfaces
    *ballTexture = SDL_CreateTextureFromSurface(renderer, ballSurface);
    *bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    *holeTexture = SDL_CreateTextureFromSurface(renderer, holeSurface);
    *pointTexture = SDL_CreateTextureFromSurface(renderer, pointSurface);
    *menuBgTexture = SDL_CreateTextureFromSurface(renderer, menuBgSurface);
    *darkTileTexture = SDL_CreateTextureFromSurface(renderer, darkTileSurface);
    *lightTileTexture = SDL_CreateTextureFromSurface(renderer, lightTileSurface);
    *powermeterTexture = SDL_CreateTextureFromSurface(renderer, powermeterSurface);
    *settingsBgTexture = SDL_CreateTextureFromSurface(renderer, settingsBgSurface);
    *playBrightTexture = SDL_CreateTextureFromSurface(renderer, playBrightSurface);
    *menuBrightTexture = SDL_CreateTextureFromSurface(renderer, menuBrightSurface);
    *exitBrightTexture = SDL_CreateTextureFromSurface(renderer, exitBrightSurface);
    *gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);

    // Check if all textures created correctly
    if (!*ballTexture || !*bgTexture || !*holeTexture || !*pointTexture || !*menuBgTexture || 
        !*darkTileTexture || !*lightTileTexture || !*powermeterTexture || !*rewardTexture || 
        !*settingsBgTexture || !*playBrightTexture || !*menuBrightTexture || !*exitBrightTexture || 
        !*gameOverTexture) {
        std::cout << "Không thể tạo texture! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Free surfaces
    SDL_FreeSurface(ballSurface);
    SDL_FreeSurface(bgSurface);
    SDL_FreeSurface(holeSurface);
    SDL_FreeSurface(pointSurface);
    SDL_FreeSurface(menuBgSurface);
    SDL_FreeSurface(darkTileSurface);
    SDL_FreeSurface(lightTileSurface);
    SDL_FreeSurface(powermeterSurface);
    SDL_FreeSurface(settingsBgSurface);
    SDL_FreeSurface(playBrightSurface);
    SDL_FreeSurface(menuBrightSurface);
    SDL_FreeSurface(exitBrightSurface);
    SDL_FreeSurface(gameOverSurface);

    // Load sounds
    *swingSound = Mix_LoadWAV("C:/Users/Cong/golf/assets/audio/swing.mp3");
    *holeSound = Mix_LoadWAV("C:/Users/Cong/golf/assets/audio/hole.mp3");
    *bounceSound = Mix_LoadWAV("C:/Users/Cong/golf/assets/audio/charge.mp3");

    if (!*swingSound || !*holeSound || !*bounceSound) {
        std::cout << "Failed to load sounds! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

void initializeGame() {
    // Initialize ball1
    ball1 = Ball(
        SCREEN_WIDTH / 4,              // x position
        SCREEN_HEIGHT - 50,            // y position
        0, 0,                          // Initial speed
        true,                          // First ball is active
        SDL_Rect{SCREEN_WIDTH / 4 - 10, SCREEN_HEIGHT - 60, 20, 20}  // Rectangle
    );

    // Initialize ball2
    ball2 = Ball(
        3 * SCREEN_WIDTH / 4,          // x position
        SCREEN_HEIGHT - 50,            // y position
        0, 0,                          // Initial speed
        false,                         // Second ball starts inactive
        SDL_Rect{3 * SCREEN_WIDTH / 4 - 10, SCREEN_HEIGHT - 60, 20, 20}  // Rectangle
    );

    // Initialize holes
    hole1 = {
        50,                            // x position
        50,                            // y position
        15,                            // radius
        {35, 35, 30, 30}               // Rectangle
    };

    hole2 = {
        SCREEN_WIDTH/2 + 50,           // x position
        50,                            // y position
        15,                            // radius
        {SCREEN_WIDTH/2 + 35, 35, 30, 30}  // Rectangle
    };

    // Initialize first obstacles
    leftObstacles.obstacles[0] = {
        100,                           // x position
        100,                           // y position
        30,                            // width
        100,                           // height
        {100, 100, 30, 100}            // Rectangle
    };

    rightObstacles.obstacles[0] = {
        SCREEN_WIDTH/2 + 100,          // x position
        100,                           // y position
        30,                            // width
        100,                           // height
        {SCREEN_WIDTH/2 + 100, 100, 30, 100}  // Rectangle
    };

    // Initialize rewards as inactive initially
    reward1 = {
        rand() % (SCREEN_WIDTH/2 - 40) + 20,  // x position
        rand() % (SCREEN_HEIGHT - 40) + 20,   // y position
        false,                                // inactive
        {0, 0, 20, 20}                        // Rectangle
    };
    
    reward2 = {
        SCREEN_WIDTH/2 + rand() % (SCREEN_WIDTH/2 - 40) + 20,  // x position
        rand() % (SCREEN_HEIGHT - 40) + 20,                   // y position
        false,                                                // inactive
        {0, 0, 20, 20}                                        // Rectangle
    };

    // Initialize game state
    gameState = MENU;
    currentBall = 1;
    player1Strokes = 0;
    player2Strokes = 0;
    gameScore.currentRound = 1;
    gameScore.isGameOver = false;
}

void initializeRoundObstacles() {
    // Round 1: Simple vertical barriers
    roundObstacles.roundConfigs[0].count = 2;
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
    roundObstacles.roundConfigs[1].count = 3;
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
    roundObstacles.roundConfigs[2].count = 3;
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
    roundObstacles.roundConfigs[3].count = 4;
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
    roundObstacles.roundConfigs[4].count = 5;
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
}

void resetRewards() {
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
}

void cleanup(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font,
            SDL_Texture* ballTexture, SDL_Texture* bgTexture, 
            SDL_Texture* holeTexture, SDL_Texture* pointTexture,
            SDL_Texture* menuBgTexture, SDL_Texture* darkTileTexture,
            SDL_Texture* lightTileTexture, SDL_Texture* powermeterTexture,
            SDL_Texture* rewardTexture, SDL_Texture* settingsBgTexture,
            SDL_Texture* playBrightTexture, SDL_Texture* menuBrightTexture,
            SDL_Texture* exitBrightTexture, SDL_Texture* gameOverTexture,
            Mix_Chunk* swingSound, Mix_Chunk* holeSound, Mix_Chunk* bounceSound) {
    // Free textures
    SDL_DestroyTexture(ballTexture);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(holeTexture);
    SDL_DestroyTexture(pointTexture);
    SDL_DestroyTexture(menuBgTexture);
    SDL_DestroyTexture(rewardTexture);
    SDL_DestroyTexture(darkTileTexture);
    SDL_DestroyTexture(lightTileTexture);
    SDL_DestroyTexture(powermeterTexture);
    SDL_DestroyTexture(settingsBgTexture);
    SDL_DestroyTexture(playBrightTexture);
    SDL_DestroyTexture(menuBrightTexture);
    SDL_DestroyTexture(exitBrightTexture);
    SDL_DestroyTexture(gameOverTexture);

    // Free sounds
    Mix_FreeChunk(swingSound);
    Mix_FreeChunk(holeSound);
    Mix_FreeChunk(bounceSound);
    Mix_CloseAudio();

    // Free font
    TTF_CloseFont(font);
    TTF_Quit();

    // Free renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}
