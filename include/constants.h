// constants.h
#pragma once

#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

enum GameState {
    MENU,
    PLAYING
};

// Power bar constants
const int MAX_POWER = 100;
const float POWER_MULTIPLIER = 0.12f;
const int POWER_BAR_LENGTH = 250;
const int POWER_BAR_WIDTH = 20;
const int POWER_BAR_MARGIN = 20;
const int ARROW_OFFSET = 20;
const int POINT_WIDTH = 15;
const int POINT_HEIGHT = 50;