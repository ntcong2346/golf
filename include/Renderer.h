#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>
#include "GameLogic.h"
#include "ResourceManager.h"

class Renderer {
public:
    Renderer(int screenWidth, int screenHeight);
    void render(SDL_Renderer* renderer, const GameLogic& gameLogic, const ResourceManager& resourceManager);
    void renderSettingsMenu(SDL_Renderer* renderer, const GameSettings& settings, TTF_Font* font, SDL_Texture* settingsBgTexture);

private:
    int screenWidth, screenHeight;
};

#endif