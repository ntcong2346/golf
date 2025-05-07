#ifndef GAME_SETTINGS_H
#define GAME_SETTINGS_H

enum BallSpeed {
    SLOW = 0,
    NORMAL = 1,
    SWIFT = 2
};

struct GameSettings {
    BallSpeed speed = NORMAL;
    bool soundEnabled = true;
    float speedMultiplier[3] = {0.08f, 0.12f, 0.18f};
};

#endif