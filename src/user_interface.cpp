#include "../include/user_interface.h"
using namespace std;

bool Button::isHovered() {
    return CheckCollisionPointRec(GetMousePosition(), rectangle);
}
bool Button::isClicked() {
    return isHovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
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
    DrawTextEx(mainFont, text, { textX + 2.0f, textY + 2.0f }, fontSize, spacing, COLOR_BLACK);
    DrawTextEx(mainFont, text, { textX, textY }, fontSize, spacing, COLOR_WHITE);
}

void drawTextWithShadow(Font &font, const char *text, float x, float y, int fontSize, float spacing, Color textColor, bool onCenter) {
    Vector2 textSize = MeasureTextEx(font, text, fontSize, spacing);
    float textX = 0.0f;
    if (onCenter) {
        textX = (screenWidth - textSize.x) / 2.0f;
    } else {
        textX = x - (textSize.x / 2.0f);
    }
    Vector2 shadowPos = {textX + 4.0f, y + 4.0f};
    DrawTextEx(font, text, shadowPos, fontSize, spacing, COLOR_BLACK);
    Vector2 mainPos = {textX, y};
    DrawTextEx(font, text, mainPos, fontSize, spacing, textColor);
}

void drawMenu(Button &playButtonEasy, Button &playButtonMedium, Button &playButtonHard, Button &settingsButton, Button &exitButton) {
    float screenCenter = screenWidth / 2.0f;
    drawTextWithShadow(mainFont, "MEMORY", screenCenter + 14.0f, 50, 150, 4.0f, COLOR_ORANGE, false);
    drawTextWithShadow(mainFont, "GAME", screenCenter + 14.0f, 150, 150, 4.0f, COLOR_WHITE, false);
    drawTextWithShadow(mainFont, "Dopasuj w pary wszystkie karty!", 0, 300, 25, 1.0f, COLOR_WHITE, true);

    playButtonEasy.draw();
    playButtonMedium.draw();
    playButtonHard.draw();
    settingsButton.draw();
    exitButton.draw();
}

void drawSettings(Button &backButton, Button &toggleMusicButton){
    float screenCenter = screenWidth / 2.0f;
    drawTextWithShadow(mainFont, "USTAWIENIA", screenCenter + 5.0f, 140, 50, 2.0f, COLOR_WHITE, false);
    toggleMusicButton.draw();
    backButton.draw();
}

void drawGame(vector<Card> &deck, Button &backButton, GameStats &stats){
    for (int i = 0; i < deck.size(); i++) {
        deck[i].draw();
    }
    backButton.draw();

    int fontSize = 30;
    float spacing = 1.0f;
    float textY = 50.0f;
    const char *timeText = TextFormat("CZAS: %d s", (int)stats.gameTime);
    const char *movesText = TextFormat("RUCHY: %d", stats.movesCount);
    const char *pairsText = TextFormat("POZOSTALO PAR: %d", stats.pairsLeft);
    int leftColumnCenter = screenWidth / 4;
    int middleColumnCenter = screenWidth / 2;
    int rightColumnCenter = (screenWidth / 4) * 3;
    drawTextWithShadow(mainFont, timeText,  leftColumnCenter,   textY, fontSize, spacing, COLOR_WHITE, false);
    drawTextWithShadow(mainFont, movesText, middleColumnCenter, textY, fontSize, spacing, COLOR_WHITE, false);
    drawTextWithShadow(mainFont, pairsText, rightColumnCenter,  textY, fontSize, spacing, COLOR_WHITE, false);
}

void drawVictory(Button &menuButton, GameStats &stats) {
    drawTextWithShadow(mainFont, "GRATULACJE!", 0, 170, 110, 3.0f, COLOR_ORANGE, true);

    const char *timeText = TextFormat("TWOJ CZAS: %d S", (int)stats.gameTime);
    const char *movesText = TextFormat("WYKONANE RUCHY: %d", stats.movesCount);

    drawTextWithShadow(mainFont, timeText, 0, 320, 30, 1.0f, COLOR_WHITE, true);
    drawTextWithShadow(mainFont, movesText, 0, 370, 30, 1.0f, COLOR_WHITE, true);

    menuButton.draw();
}