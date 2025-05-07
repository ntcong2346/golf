#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <string>

class ResourceManager {
public:
    ResourceManager();
    bool loadResources(SDL_Renderer* renderer);
    void freeResources();
    SDL_Texture* getBallTexture() const { return ballTexture; }
    SDL_Texture* getBgTexture() const { return bgTexture; }
    SDL_Texture* getHoleTexture() const { return holeTexture; }
    SDL_Texture* getPointTexture() const { return pointTexture; }
    SDL_Texture* getMenuBgTexture() const { return menuBgTexture; }
    SDL_Texture* getDarkTileTexture() const { return darkTileTexture; }
    SDL_Texture* getLightTileTexture() const { return lightTileTexture; }
    SDL_Texture* getRewardTexture() const { return rewardTexture; }
    SDL_Texture* getPlayBrightTexture() const { return playBrightTexture; }
    SDL_Texture* getMenuBrightTexture() const { return menuBrightTexture; }
    SDL_Texture* getExitBrightTexture() const { return exitBrightTexture; }
    SDL_Texture* getSettingsBgTexture() const { return settingsBgTexture; }
    SDL_Texture* getGameOverTexture() const { return gameOverTexture; }
    Mix_Chunk* getSwingSound() const { return swingSound; }
    Mix_Chunk* getHoleSound() const { return holeSound; }
    Mix_Chunk* getBounceSound() const { return bounceSound; }
    TTF_Font* getFont() const { return font; }

private:
    SDL_Texture* ballTexture;
    SDL_Texture* bgTexture;
    SDL_Texture* holeTexture;
    SDL_Texture* pointTexture;
    SDL_Texture* menuBgTexture;
    SDL_Texture* darkTileTexture;
    SDL_Texture* lightTileTexture;
    SDL_Texture* rewardTexture;
    SDL_Texture* playBrightTexture;
    SDL_Texture* menuBrightTexture;
    SDL_Texture* exitBrightTexture;
    SDL_Texture* settingsBgTexture;
    SDL_Texture* gameOverTexture;
    Mix_Chunk* swingSound;
    Mix_Chunk* holeSound;
    Mix_Chunk* bounceSound;
    TTF_Font* font;
};

#endif