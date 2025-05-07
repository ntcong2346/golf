#ifndef RENDERING_HPP
#define RENDERING_HPP

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game_objects.hpp"

enum GameState {
    MENU,
    PLAYING
};

const int MAX_POWER = 100;
const float POWER_MULTIPLIER = 0.12f;
const int POWER_BAR_LENGTH = 250;
const int POWER_BAR_WIDTH = 20;
const int POWER_BAR_MARGIN = 20;
const int ARROW_OFFSET = 20;
const int POINT_WIDTH = 15;
const int POINT_HEIGHT = 50;

SDL_Texture* loadTexture(const char* path, SDL_Renderer* renderer);
SDL_Texture* loadRewardTexture(const char* path, SDL_Renderer* renderer);
void handleMenuEvents(SDL_Event& e, GameState& gameState, bool& quit, SDL_Rect& playHitBox, SDL_Rect& menuHitBox, SDL_Rect& exitHitBox);
void renderMenu(SDL_Renderer* renderer, SDL_Texture* menuBgTexture, SDL_Texture* playBrightTexture, SDL_Texture* menuBrightTexture, SDL_Texture* exitBrightTexture, SDL_Rect& playHitBox, SDL_Rect& menuHitBox, SDL_Rect& exitHitBox);
void renderGame(SDL_Renderer* renderer, SDL_Texture* bgTexture, SDL_Texture* holeTexture, SDL_Texture* ballTexture, SDL_Texture* rewardTexture, SDL_Texture* pointTexture, SDL_Texture* darkTileTexture, SDL_Texture* lightTileTexture, TTF_Font* font, Ball& ball1, Ball& ball2, Hole& hole1, Hole& hole2, Reward& reward1, Reward& reward2, int currentBall, bool isDragging, float aimAngle, float currentPower, int player1Strokes, int player2Strokes);

#endif