#ifndef MEMORYGAME_USER_INTERFACE_H
#define MEMORYGAME_USER_INTERFACE_H

#include "raylib.h"
#include <vector>
#include "game_logic.h"
#include "common.h"

struct Button {
    Rectangle rectangle;
    const char *text;
    Color baseColor;
    Color hoverColor;
    int fontSize;
    bool isHovered();
    bool isClicked();
    void draw();
};

void drawTextWithShadow(Font &font, const char *text, float x, float y, int fontSize, float spacing, Color textColor, bool onCenter);
void drawMenu(Button &playButtonEasy, Button &playButtonMedium, Button &playButtonHard, Button &settingsButton, Button &exitButton);
void drawSettings(Button &backButton, Button &toggleMusicButton);
void drawGame(std::vector<Card> &deck, Button &backButton, GameStats &stats);
void drawVictory(Button &menuButton, GameStats &stats);

#endif //MEMORYGAME_USER_INTERFACE_H