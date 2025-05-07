#ifndef GAME_SCORE_H
#define GAME_SCORE_H

struct GameScore {
    int player1Score = 0;
    int player2Score = 0;
    int currentRound = 1;
    int maxRounds = 4;
    int totalStrokes1 = 0;
    int totalStrokes2 = 0;
    bool isGameOver = false;
};

#endif