#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H
#include <SDL2/SDL.h>
#include "GameLogic.h"
#include "ResourceManager.h"

class EventHandler {
public:
    bool handleEvents(SDL_Event& e, GameLogic& gameLogic, const ResourceManager& resourceManager, SDL_Renderer* renderer);
};

#endif