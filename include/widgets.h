#ifndef MEMORYGAME_WIDGETS_H
#define MEMORYGAME_WIDGETS_H
#include "raylib.h"

enum struct GameStatus {MENU, GAMEPLAY, SETTINGS, VICTORY};
enum struct CardStatus {HIDDEN, REVEALED, GUESSED};

struct Card {
    int id;
    Rectangle rectangle;
    CardStatus status;
    bool isHovered();
    bool isClicked();
    void draw();
};

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

#endif //MEMORYGAME_WIDGETS_H