#include "game.h"

void updateGame(Mix_Chunk* bounceSound, Mix_Chunk* holeSound) {
    // Update ball positions
    updateBall(&ball1, bounceSound, holeSound);
    updateBall(&ball2, bounceSound, holeSound);
    
    // Update obstacle rectangles
    for (int i = 0; i < leftObstacles.count; i++) {
        // Kiểm tra và điều chỉnh vị trí của chướng ngại vật bên trái
        if (leftObstacles.obstacles[i].x + leftObstacles.obstacles[i].width > SCREEN_WIDTH/2) {
            leftObstacles.obstacles[i].width = SCREEN_WIDTH/2 - leftObstacles.obstacles[i].x;
        }
        leftObstacles.obstacles[i].rect.x = leftObstacles.obstacles[i].x;
        leftObstacles.obstacles[i].rect.y = leftObstacles.obstacles[i].y;
        leftObstacles.obstacles[i].rect.w = leftObstacles.obstacles[i].width;
        leftObstacles.obstacles[i].rect.h = leftObstacles.obstacles[i].height;
    }
    
    for (int i = 0; i < rightObstacles.count; i++) {
        // Kiểm tra và điều chỉnh vị trí của chướng ngại vật bên phải
        if (rightObstacles.obstacles[i].x < SCREEN_WIDTH/2) {
            int adjustment = SCREEN_WIDTH/2 - rightObstacles.obstacles[i].x;
            rightObstacles.obstacles[i].x = SCREEN_WIDTH/2;
            rightObstacles.obstacles[i].width -= adjustment;
        }
        rightObstacles.obstacles[i].rect.x = rightObstacles.obstacles[i].x;
        rightObstacles.obstacles[i].rect.y = rightObstacles.obstacles[i].y;
        rightObstacles.obstacles[i].rect.w = rightObstacles.obstacles[i].width;
        rightObstacles.obstacles[i].rect.h = rightObstacles.obstacles[i].height;
    }
    
    // Update hole rectangles
    hole1.rect.x = hole1.x - hole1.rect.w/2;
    hole1.rect.y = hole1.y - hole1.rect.h/2;
    hole2.rect.x = hole2.x - hole2.rect.w/2;
    hole2.rect.y = hole2.y - hole2.rect.h/2;
    
    // Update reward rectangles
    if (reward1.active) {
        reward1.rect = {
            reward1.x - Reward::SIZE/2,
            reward1.y - Reward::SIZE/2,
            Reward::SIZE,
            Reward::SIZE
        };
    }
    if (reward2.active) {
        reward2.rect = {
            reward2.x - Reward::SIZE/2,
            reward2.y - Reward::SIZE/2,
            Reward::SIZE,
            Reward::SIZE
        };
    }
}

void updateBall(Ball* ball, Mix_Chunk* bounceSound, Mix_Chunk* holeSound) {
    // Skip animation if ball is animating
    if (ball->isAnimating) {
        ball->animationTimer -= 0.016f;
        
        // Move ball toward hole center
        float dx = ball->targetX - ball->x;
        float dy = ball->targetY - ball->y;
        float dist = sqrt(dx * dx + dy * dy);
        
        if (dist > 1.0f) {
            float moveX = (dx / dist) * Ball::MOVE_SPEED;
            float moveY = (dy / dist) * Ball::MOVE_SPEED;
            ball->x += moveX;
            ball->y += moveY;
        } else {
            ball->x = ball->targetX;
            ball->y = ball->targetY;
        }
        
        // Update scale
        ball->scale = ball->animationTimer / Ball::ANIMATION_DURATION;
        
        // Check if animation is complete
        if (ball->animationTimer <= 0) {
            ball->isAnimating = false;
            ball->scale = 1.0f;
            
            // Handle player turn switching and round completion
            if (ball == &ball1) {
                gameScore.totalStrokes1 += player1Strokes;
                currentBall = 2;
                ball2.isActive = true;
                ball1.isActive = false;
                ball1.x = SCREEN_WIDTH / 4;
                ball1.y = SCREEN_HEIGHT - 50;
                player1Strokes = 0; // Reset strokes for next round
            } else {
                gameScore.totalStrokes2 += player2Strokes;
                currentBall = 1;
                ball1.isActive = true;
                ball2.isActive = false;
                player2Strokes = 0; // Reset strokes for next round
                
                // End of round - both players have completed
                gameScore.currentRound++;
                
                // Check if game is over
                if (gameScore.currentRound > gameScore.maxRounds) {
                    gameScore.isGameOver = true;
                    gameState = GAME_OVER;
                    
                    // Đảm bảo màn hình game over được hiển thị
                    std::cout << "Game over! Round: " << gameScore.currentRound 
                              << ", Max rounds: " << gameScore.maxRounds << std::endl;
                    std::cout << "Player 1 total strokes: " << gameScore.totalStrokes1 << std::endl;
                    std::cout << "Player 2 total strokes: " << gameScore.totalStrokes2 << std::endl;
                } else {
                    setupNextRound();
                }
            }
            
            // Reset ball physics
            ball->speedX = 0;
            ball->speedY = 0;
        }
        
        // Update ball rectangle
        ball->rect.x = ball->x - ball->rect.w/2;
        ball->rect.y = ball->y - ball->rect.h/2;
        return;
    }
    
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
            player1Strokes = std::max(0, player1Strokes - 2);  // Reduce strokes by 2
        }
    } else if (ball == &ball2 && reward2.active) {
        int dx = ball->x - reward2.x;
        int dy = ball->y - reward2.y;
        if (sqrt(dx * dx + dy * dy) <= ballRadius + Reward::SIZE/2) {
            reward2.active = false;
            player2Strokes = std::max(0, player2Strokes - 2);  // Reduce strokes by 2
        }
    }

    // Bounce off walls with improved collision detection
    if (ball->x < ballRadius) {
        ball->x = ballRadius;
        ball->speedX = -ball->speedX * Ball::BOUNCE;
        if (settings.soundEnabled && bounceSound) {
            Mix_PlayChannel(-1, bounceSound, 0);
        }
    } else if (ball->x > SCREEN_WIDTH - ballRadius) {
        ball->x = SCREEN_WIDTH - ballRadius;
        ball->speedX = -ball->speedX * Ball::BOUNCE;
        if (settings.soundEnabled && bounceSound) {
            Mix_PlayChannel(-1, bounceSound, 0);
        }
    }

    // Add center line collision
    if (ball == &ball1 && ball->x > SCREEN_WIDTH/2 - ballRadius) {
        ball->x = SCREEN_WIDTH/2 - ballRadius;
        ball->speedX = -ball->speedX * Ball::BOUNCE;
        if (settings.soundEnabled && bounceSound) {
            Mix_PlayChannel(-1, bounceSound, 0);
        }
    } else if (ball == &ball2 && ball->x < SCREEN_WIDTH/2 + ballRadius) {
        ball->x = SCREEN_WIDTH/2 + ballRadius;
        ball->speedX = -ball->speedX * Ball::BOUNCE;
        if (settings.soundEnabled && bounceSound) {
            Mix_PlayChannel(-1, bounceSound, 0);
        }
    }

    if (ball->y < ballRadius) {
        ball->y = ballRadius;
        ball->speedY = -ball->speedY * Ball::BOUNCE;
        if (settings.soundEnabled && bounceSound) {
            Mix_PlayChannel(-1, bounceSound, 0);
        }
    } else if (ball->y > SCREEN_HEIGHT - ballRadius) {
        ball->y = SCREEN_HEIGHT - ballRadius;
        ball->speedY = -ball->speedY * Ball::BOUNCE;
        if (settings.soundEnabled && bounceSound) {
            Mix_PlayChannel(-1, bounceSound, 0);
        }
    }

    // Handle obstacle collisions
    ObstacleSet& obstacles = (ball == &ball1) ? leftObstacles : rightObstacles;
    for(int i = 0; i < obstacles.count; i++) {
        Obstacle& obs = obstacles.obstacles[i];
        if (ball->x + ballRadius > obs.x && 
            ball->x - ballRadius < obs.x + obs.width &&
            ball->y + ballRadius > obs.y && 
            ball->y - ballRadius < obs.y + obs.height) {
            
            if (settings.soundEnabled && bounceSound) {
                Mix_PlayChannel(-1, bounceSound, 0);
            }

            // Determine collision direction
            float overlapLeft = (ball->x + ballRadius) - obs.x;
            float overlapRight = (obs.x + obs.width) - (ball->x - ballRadius);
            float overlapTop = (ball->y + ballRadius) - obs.y;
            float overlapBottom = (obs.y + obs.height) - (ball->y - ballRadius);
            
            // Find minimum overlap
            float minOverlapX = overlapLeft < overlapRight ? overlapLeft : overlapRight;
            float minOverlapY = overlapTop < overlapBottom ? overlapTop : overlapBottom;
            
            // Resolve collision based on minimum overlap
            if (minOverlapX < minOverlapY) {
                // Horizontal collision
                if (overlapLeft < overlapRight) {
                    ball->x = obs.x - ballRadius;
                    ball->speedX = -ball->speedX * Ball::OBSTACLE_BOUNCE;
                } else {
                    ball->x = obs.x + obs.width + ballRadius;
                    ball->speedX = -ball->speedX * Ball::OBSTACLE_BOUNCE;
                }
                ball->speedY *= Ball::OBSTACLE_FRICTION;
            } else {
                // Vertical collision
                if (overlapTop < overlapBottom) {
                    ball->y = obs.y - ballRadius;
                    ball->speedY = -ball->speedY * Ball::OBSTACLE_BOUNCE;
                } else {
                    ball->y = obs.y + obs.height + ballRadius;
                    ball->speedY = -ball->speedY * Ball::OBSTACLE_BOUNCE;
                }
                ball->speedX *= Ball::OBSTACLE_FRICTION;
            }
            
            // Add slight randomness for natural effect
            float randomFactor = 0.98f + (rand() % 5) / 100.0f; // 0.98 - 1.02
            ball->speedX *= randomFactor;
            ball->speedY *= randomFactor;
        }
    }

    // Check hole collision
    Hole& targetHole = (ball == &ball1) ? hole1 : hole2;
    int dx = ball->x - targetHole.x;
    int dy = ball->y - targetHole.y;
    
    if (sqrt(dx * dx + dy * dy) <= ballRadius + targetHole.radius) {
        if (!ball->isAnimating) {
            if (settings.soundEnabled && holeSound) {
                Mix_PlayChannel(-1, holeSound, 0);
            }
            ball->isAnimating = true;
            ball->animationTimer = Ball::ANIMATION_DURATION;
            ball->speedX = 0;
            ball->speedY = 0;
            ball->targetX = targetHole.x;
            ball->targetY = targetHole.y;
        }
    }

    // Update ball rectangle
    ball->rect.x = ball->x - ball->rect.w/2;
    ball->rect.y = ball->y - ball->rect.h/2;
}

void updatePower(float& powerIncrement) {
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
}

void setupNextRound() {
    // Reset ball positions
    ball1.x = SCREEN_WIDTH / 4;
    ball1.y = SCREEN_HEIGHT - 50;
    ball2.x = 3 * SCREEN_WIDTH / 4;
    ball2.y = SCREEN_HEIGHT - 50;
    
    // Reset strokes for new round
    player1Strokes = 0;
    player2Strokes = 0;
    
    // Update obstacles for new round
    leftObstacles = roundObstacles.roundConfigs[gameScore.currentRound - 1];
    
    // Mirror obstacles for right side
    rightObstacles.count = leftObstacles.count;
    for(int i = 0; i < leftObstacles.count; i++) {
        // Đảm bảo chướng ngại vật không vượt qua vạch giữa màn hình
        // Kiểm tra và điều chỉnh vị trí của chướng ngại vật bên trái
        if (leftObstacles.obstacles[i].x + leftObstacles.obstacles[i].width > SCREEN_WIDTH/2) {
            leftObstacles.obstacles[i].width = SCREEN_WIDTH/2 - leftObstacles.obstacles[i].x;
            leftObstacles.obstacles[i].rect.w = leftObstacles.obstacles[i].width;
        }
        
        // Tạo chướng ngại vật bên phải bằng cách phản chiếu từ bên trái
        rightObstacles.obstacles[i] = leftObstacles.obstacles[i];
        
        // Đặt vị trí cho chướng ngại vật bên phải
        rightObstacles.obstacles[i].x = SCREEN_WIDTH - leftObstacles.obstacles[i].x - leftObstacles.obstacles[i].width;
        
        // Đảm bảo chướng ngại vật bên phải không vượt qua vạch giữa
        if (rightObstacles.obstacles[i].x < SCREEN_WIDTH/2) {
            int adjustment = SCREEN_WIDTH/2 - rightObstacles.obstacles[i].x;
            rightObstacles.obstacles[i].x += adjustment;
            rightObstacles.obstacles[i].width -= adjustment;
        }
        
        rightObstacles.obstacles[i].rect.x = rightObstacles.obstacles[i].x;
        rightObstacles.obstacles[i].rect.w = rightObstacles.obstacles[i].width;
    }
    
    // Reset and activate rewards for new round
    resetRewards();
}

void checkGameOver(SDL_Renderer* renderer, TTF_Font* font, SDL_Texture* gameOverTexture) {
    if (gameScore.isGameOver) {
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
        
        // Game over render loop
        bool gameOverScreen = true;
        SDL_Event e;
        bool quit = false;
        
        while (gameOverScreen && !quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                }
                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_SPACE) {
                    gameOverScreen = false;
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
                }
            }
            
            // Render game over screen
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, gameOverTexture, NULL, NULL);
            SDL_RenderCopy(renderer, winnerTexture, NULL, &winnerRect);
            SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
            SDL_RenderCopy(renderer, promptTexture, NULL, &promptRect);
            SDL_RenderPresent(renderer);
        }
        
        // Clean up textures
        SDL_DestroyTexture(winnerTexture);
        SDL_DestroyTexture(scoreTexture);
        SDL_DestroyTexture(promptTexture);
        SDL_FreeSurface(winnerSurface);
        SDL_FreeSurface(scoreSurface);
        SDL_FreeSurface(promptSurface);
        
        // Reset ball positions for new game
        ball1.x = SCREEN_WIDTH / 4;
        ball1.y = SCREEN_HEIGHT - 50;
        ball2.x = 3 * SCREEN_WIDTH / 4;
        ball2.y = SCREEN_HEIGHT - 50;
    }
}
