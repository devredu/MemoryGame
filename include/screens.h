#ifndef MEMORYGAME_SCREENS_H
#define MEMORYGAME_SCREENS_H
#include "raylib.h"
#include "game.h"
#include "widgets.h"

extern Font mainFont;

void drawTextWithShadow(Font &font, const char *text, float x, float y, int fontSize, float spacing, Color textColor, bool onCenter);
void drawMenu(Button &playButtonEasy, Button &playButtonMedium, Button &playButtonHard, Button &settingsButton, Button &exitButton);
void drawSettings(Button &backButton, Button &toggleMusicButton);
void drawGame(std::vector<Card> &deck, Button &backButton, Stats &stats);
void drawVictory(Button &menuButton, Stats &stats);

#endif //MEMORYGAME_SCREENS_H