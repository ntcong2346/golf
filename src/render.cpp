#include "game.h"

void renderGame(SDL_Renderer* renderer, TTF_Font* font, 
               SDL_Texture* ballTexture, SDL_Texture* bgTexture, 
               SDL_Texture* holeTexture, SDL_Texture* pointTexture,
               SDL_Texture* darkTileTexture, SDL_Texture* lightTileTexture,
               SDL_Texture* rewardTexture) {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    // Render background
    SDL_RenderCopy(renderer, bgTexture, NULL, NULL);
    
    // Draw dividing line between two sides
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT);
    
    
    SDL_RenderDrawLine(renderer, SCREEN_WIDTH/2, 0, SCREEN_WIDTH/2, SCREEN_HEIGHT);
    
    // Render holes
    SDL_RenderCopy(renderer, holeTexture, NULL, &hole1.rect);
    SDL_RenderCopy(renderer, holeTexture, NULL, &hole2.rect);
    
    // Render obstacles
    renderObstacles(renderer, darkTileTexture, lightTileTexture);
    
    // Render rewards
    renderRewards(renderer, rewardTexture);
    
    // Render balls
    renderBalls(renderer, ballTexture);
    
    // Render power bar and aim arrow if dragging
    if (isDragging) {
        renderPowerBar(renderer);
        renderAimArrow(renderer, pointTexture);
    }
    
    // Render score information
    renderScoreInfo(renderer, font);
    
    // Present renderer
    SDL_RenderPresent(renderer);
}

void renderMenu(SDL_Renderer* renderer, SDL_Texture* menuBgTexture, 
               SDL_Texture* playBrightTexture, SDL_Texture* menuBrightTexture, 
               SDL_Texture* exitBrightTexture) {
    // Get mouse position
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    // Define hit box rectangles
    SDL_Rect playHitBox = {291, 235, 210, 60};
    SDL_Rect menuHitBox = {291, 322, 210, 60};
    SDL_Rect exitHitBox = {291, 422, 210, 60};

    // Default background
    SDL_RenderCopy(renderer, menuBgTexture, NULL, NULL);

    // Debug: Vẽ viền cho các hit box để kiểm tra
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &playHitBox);
    SDL_RenderDrawRect(renderer, &menuHitBox);
    SDL_RenderDrawRect(renderer, &exitHitBox);

    // Check hover states and render appropriate backgrounds
    if (mouseX >= playHitBox.x && mouseX <= playHitBox.x + playHitBox.w &&
        mouseY >= playHitBox.y && mouseY <= playHitBox.y + playHitBox.h) {
        SDL_RenderCopy(renderer, playBrightTexture, NULL, NULL);
        // Debug: In ra khi chuột hover trên nút Play
        std::cout << "Mouse hovering over Play button" << std::endl;
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

void renderSettingsMenu(SDL_Renderer* renderer, TTF_Font* font, 
                       GameSettings& settings, SDL_Texture* settingsBgTexture) {
    // Render background first
    SDL_RenderCopy(renderer, settingsBgTexture, NULL, NULL);
    
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Color selectedColor = {255, 255, 0, 255};
    
    // Speed settings
    const char* speedTexts[] = {"Speed: Slow", "Speed: Normal", "Speed: Swift"};
    for(int i = 0; i < 3; i++) {
        SDL_Surface* speedSurface = TTF_RenderText_Solid(font, speedTexts[i], 
            settings.speed == i ? selectedColor : textColor);
        SDL_Texture* speedTexture = SDL_CreateTextureFromSurface(renderer, speedSurface);
        SDL_Rect speedRect = {
            SCREEN_WIDTH/2 - speedSurface->w/2,
            250 + i*50,
            speedSurface->w,
            speedSurface->h
        };
        SDL_RenderCopy(renderer, speedTexture, NULL, &speedRect);
        SDL_FreeSurface(speedSurface);
        SDL_DestroyTexture(speedTexture);
    }
    
    // Sound settings
    const char* soundText = settings.soundEnabled ? "Sound: On" : "Sound: Off";
    SDL_Surface* soundSurface = TTF_RenderText_Solid(font, soundText, textColor);
    SDL_Texture* soundTexture = SDL_CreateTextureFromSurface(renderer, soundSurface);
    SDL_Rect soundRect = {
        SCREEN_WIDTH/2 - soundSurface->w/2,
        400,
        soundSurface->w,
        soundSurface->h
    };
    SDL_RenderCopy(renderer, soundTexture, NULL, &soundRect);
    
    // Back button
    SDL_Surface* backSurface = TTF_RenderText_Solid(font, "Back", textColor);
    SDL_Texture* backTexture = SDL_CreateTextureFromSurface(renderer, backSurface);
    SDL_Rect backRect = {
        SCREEN_WIDTH/2 - backSurface->w/2,
        450,
        backSurface->w,
        backSurface->h
    };
    SDL_RenderCopy(renderer, backTexture, NULL, &backRect);
    
    // Clean up
    SDL_FreeSurface(soundSurface);
    SDL_FreeSurface(backSurface);
    SDL_DestroyTexture(soundTexture);
    SDL_DestroyTexture(backTexture);
}

void renderBalls(SDL_Renderer* renderer, SDL_Texture* ballTexture) {
    // Render both balls with scale animation
    for (Ball* ball : {&ball1, &ball2}) {
        SDL_Rect scaledRect = ball->rect;
        if (ball->isAnimating) {
            // Calculate new size and position based on scale
            int newWidth = static_cast<int>(ball->rect.w * ball->scale);
            int newHeight = static_cast<int>(ball->rect.h * ball->scale);
            scaledRect.w = newWidth;
            scaledRect.h = newHeight;
            // Center rect with ball position
            scaledRect.x = ball->x - newWidth/2;
            scaledRect.y = ball->y - newHeight/2;
        }
        SDL_RenderCopy(renderer, ballTexture, NULL, &scaledRect);
    }
}

void renderObstacles(SDL_Renderer* renderer, SDL_Texture* darkTileTexture, SDL_Texture* lightTileTexture) {
    // Render left side obstacles
    for(int i = 0; i < leftObstacles.count; i++) {
        SDL_RenderCopy(renderer, darkTileTexture, NULL, &leftObstacles.obstacles[i].rect);
    }
    
    // Render right side obstacles
    for(int i = 0; i < rightObstacles.count; i++) {
        SDL_RenderCopy(renderer, lightTileTexture, NULL, &rightObstacles.obstacles[i].rect);
    }
}

void renderPowerBar(SDL_Renderer* renderer) {
    // Draw power bar based on current ball
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
}

void renderAimArrow(SDL_Renderer* renderer, SDL_Texture* pointTexture) {
    Ball& activeBall = (currentBall == 1) ? ball1 : ball2;
    
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

void renderScoreInfo(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Color textColor = {255, 255, 255, 255};
    
    // Player 1 strokes - left side
    char strokeText1[64];
    sprintf(strokeText1, "Strokes: %d   Total: %d", player1Strokes, gameScore.totalStrokes1);
    SDL_Surface* strokeSurface1 = TTF_RenderText_Solid(font, strokeText1, textColor);
    SDL_Texture* strokeTexture1 = SDL_CreateTextureFromSurface(renderer, strokeSurface1);
    SDL_Rect strokeRect1 = {20, 15, strokeSurface1->w, strokeSurface1->h};
    SDL_RenderCopy(renderer, strokeTexture1, NULL, &strokeRect1);

    // Player 2 strokes - right side
    char strokeText2[64];
    sprintf(strokeText2, "Strokes: %d   Total: %d", player2Strokes, gameScore.totalStrokes2);
    SDL_Surface* strokeSurface2 = TTF_RenderText_Solid(font, strokeText2, textColor);
    SDL_Texture* strokeTexture2 = SDL_CreateTextureFromSurface(renderer, strokeSurface2);
    SDL_Rect strokeRect2 = {
        SCREEN_WIDTH - strokeSurface2->w - 20,
        15,
        strokeSurface2->w,
        strokeSurface2->h
    };
    SDL_RenderCopy(renderer, strokeTexture2, NULL, &strokeRect2);

    // Round display - center
    char roundText[32];
    sprintf(roundText, "Round: %d/%d", gameScore.currentRound, gameScore.maxRounds);
    SDL_Surface* roundSurface = TTF_RenderText_Solid(font, roundText, textColor);
    SDL_Texture* roundTexture = SDL_CreateTextureFromSurface(renderer, roundSurface);
    SDL_Rect roundRect = {
        SCREEN_WIDTH/2 - roundSurface->w/2, 
        15,
        roundSurface->w, 
        roundSurface->h
    };
    SDL_RenderCopy(renderer, roundTexture, NULL, &roundRect);
    
    // Clean up
    SDL_FreeSurface(strokeSurface1);
    SDL_FreeSurface(strokeSurface2);
    SDL_FreeSurface(roundSurface);
    SDL_DestroyTexture(strokeTexture1);
    SDL_DestroyTexture(strokeTexture2);
    SDL_DestroyTexture(roundTexture);
}

void renderRewards(SDL_Renderer* renderer, SDL_Texture* rewardTexture) {
    // Render rewards if active
    if (reward1.active) {
        SDL_RenderCopy(renderer, rewardTexture, NULL, &reward1.rect);
    }
    if (reward2.active) {
        SDL_RenderCopy(renderer, rewardTexture, NULL, &reward2.rect);
    }
}

void renderGameOver(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* gameOverTexture) {
    // Determine winner
    std::string winnerText;
    if (gameScore.totalStrokes1 < gameScore.totalStrokes2) {
        winnerText = "Player 1 Wins!";
    } else if (gameScore.totalStrokes2 < gameScore.totalStrokes1) {
        winnerText = "Player 2 Wins!";
    } else {
        winnerText = "Draw! Let's play again!";
    }
    
    // Create winner texture
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* winnerSurface = TTF_RenderText_Solid(font, winnerText.c_str(), textColor);
    SDL_Texture* winnerTexture = SDL_CreateTextureFromSurface(renderer, winnerSurface);
    SDL_Rect winnerRect = {
        SCREEN_WIDTH/2 - winnerSurface->w/2,
        SCREEN_HEIGHT/2 - winnerSurface->h/2,
        winnerSurface->w,
        winnerSurface->h
    };
    
    // Display final scores
    char finalScoreText[128];
    sprintf(finalScoreText, "Final Scores - Player 1: %d strokes, Player 2: %d strokes",
            gameScore.totalStrokes1, gameScore.totalStrokes2);
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, finalScoreText, textColor);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);
    SDL_Rect scoreRect = {
        SCREEN_WIDTH/2 - scoreSurface->w/2,
        winnerRect.y + winnerRect.h + 20,
        scoreSurface->w,
        scoreSurface->h
    };
    
    // Add "Press SPACE to return to menu" text
    const char* promptText = "Press SPACE to return to menu";
    SDL_Surface* promptSurface = TTF_RenderText_Solid(font, promptText, textColor);
    SDL_Texture* promptTexture = SDL_CreateTextureFromSurface(renderer, promptSurface);
    SDL_Rect promptRect = {
        SCREEN_WIDTH/2 - promptSurface->w/2,
        scoreRect.y + scoreRect.h + 40,
        promptSurface->w,
        promptSurface->h
    };
    
    // Render game over screen
    SDL_RenderCopy(renderer, gameOverTexture, NULL, NULL);
    SDL_RenderCopy(renderer, winnerTexture, NULL, &winnerRect);
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
    SDL_RenderCopy(renderer, promptTexture, NULL, &promptRect);
    
    // Clean up
    SDL_DestroyTexture(winnerTexture);
    SDL_DestroyTexture(scoreTexture);
    SDL_DestroyTexture(promptTexture);
    SDL_FreeSurface(winnerSurface);
    SDL_FreeSurface(scoreSurface);
    SDL_FreeSurface(promptSurface);
}
