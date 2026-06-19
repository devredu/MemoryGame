#ifndef MEMORYGAME_COMMON_H
#define MEMORYGAME_COMMON_H

#include "raylib.h"

const int screenWidth = 1280;
const int screenHeight = 720;
const int gapSize = 15;
const float cardRevealDelay = 0.65f;

enum struct GameState {MENU, GAMEPLAY, SETTINGS, VICTORY};
enum struct CardStatus {HIDDEN, REVEALED, GUESSED};

struct GameConfig {
    int cols;
    int rows;
    int cardSize;
};

struct GameStats {
    int movesCount = 0;
    float gameTime = 0.0f;
    int pairsLeft = 0;
    float revealTimer = 0.0f;

    void reset(int totalPairs) {
        movesCount = 0;
        gameTime = 0.0f;
        pairsLeft = totalPairs;
        revealTimer = 0.0f;
    }
};

const Color COLOR_BLACK = GetColor(0x000000FF);
const Color COLOR_GREEN = GetColor(0x4CAF50FF);
const Color COLOR_GREEN_HOVER = GetColor(0x66BB6AFF);
const Color COLOR_PURPLE = GetColor(0x4A148CFF);
const Color COLOR_PURPLE_HOVER = GetColor(0x6A1B9AFF);
const Color COLOR_PURPLE_DARK = GetColor(0x2A0854FF);
const Color COLOR_RED = GetColor(0xF44336FF);
const Color COLOR_RED_HOVER = GetColor(0xEF5350FF);
const Color COLOR_ORANGE = GetColor(0xFF9800FF);
const Color COLOR_ORANGE_HOVER = GetColor(0xFFB74DFF);
const Color COLOR_WHITE = GetColor(0xFFFFFFFF);
const Color COLOR_LIGHT_GRAY = GetColor(0xD3D3D3FF);

extern Font mainFont;

#endif //MEMORYGAME_COMMON_H