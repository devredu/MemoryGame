#include "../include/widgets.h"
#include "../include/game.h"
#include "../include/screens.h"

bool Card::isHovered() {
    if (CheckCollisionPointRec(GetMousePosition(), rectangle)) {
        return true;
    } else {
        return false;
    }
}
bool Card::isClicked() {
    if (isHovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return true;
    } else {
        return false;
    }
}
void Card::draw() {
    if (status == CardStatus::GUESSED) {
        return;
    }
    Color cardColor;
    Color borderCardColor;

    if (status == CardStatus::REVEALED) {
        cardColor = COLOR_ORANGE_HOVER;
        borderCardColor = COLOR_ORANGE;
    } else {
        if (isHovered()) {
            cardColor = COLOR_PURPLE;
            borderCardColor = COLOR_ORANGE;
        } else {
            cardColor = COLOR_PURPLE;
            borderCardColor = COLOR_PURPLE_DARK;
        }
    }

    Rectangle shadowRectangle = {rectangle.x + 4, rectangle.y + 4, rectangle.width + 4, rectangle.height + 4};
    DrawRectangleRounded(shadowRectangle, 0.25f, 4, COLOR_BLACK);
    DrawRectangleRounded(rectangle, 0.25f, 4, cardColor);
    DrawRectangleRoundedLinesEx(rectangle, 0.25f, 4, 4.0f, borderCardColor);

    if (status == CardStatus::REVEALED) {
        int fontSize = rectangle.width * 0.4f;
        int textWidth = MeasureText(TextFormat("%d", id), fontSize);
        float textX = rectangle.x + (rectangle.width - textWidth) / 2.0f;
        float textY = rectangle.y + (rectangle.height - fontSize) / 2.0f;
        DrawTextEx(mainFont, TextFormat("%d", id), {textX, textY}, fontSize, 1.0f, COLOR_BLACK);
    }
}



bool Button::isHovered(){
    if (CheckCollisionPointRec(GetMousePosition(), rectangle)) {
        return true;
    } else {
        return false;
    }
}
bool Button::isClicked() {
    if (isHovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        return true;
    } else {
        return false;
    }
}
void Button::draw() {
    Color currentButtonColor;
    if (isHovered()) {
        currentButtonColor = hoverColor;
    } else {
        currentButtonColor = baseColor;
    }

    Rectangle shadowRectangle = {rectangle.x + 4, rectangle.y + 4, rectangle.width, rectangle.height};
    DrawRectangleRounded(shadowRectangle, 0.25f, 4, COLOR_BLACK);
    DrawRectangleRounded(rectangle, 0.25f, 4, currentButtonColor);
    DrawRectangleRoundedLinesEx(rectangle, 0.25f, 4, 3.0f, COLOR_BLACK);

    float spacing = 2.0f;
    Vector2 textSize = MeasureTextEx(mainFont, text, fontSize, spacing);
    float textX = rectangle.x + (rectangle.width - textSize.x) / 2.0f;
    float textY = rectangle.y + (rectangle.height - textSize.y) / 2.0f;
    DrawTextEx(mainFont, text, {textX + 2.0f, textY + 2.0f}, fontSize, spacing, COLOR_BLACK);
    DrawTextEx(mainFont, text, {textX, textY}, fontSize, spacing, COLOR_WHITE);
}