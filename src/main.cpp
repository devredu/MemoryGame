#include "raylib.h"
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

// Ustawienia
const int screenWidth = 1280;
const int screenHeight = 720;
const int cols = 4;
const int rows = 4;
const int cardSize = 115;
const int gapSize = 15;

enum class gameState {MENU, GAMEPLAY, SETTINGS};

enum class cardStatus {HIDDEN, REVEALED, GUESSED};

struct Card {
    int id;
    Rectangle rectangle;
    cardStatus status;

    bool isHovered() const {
        return CheckCollisionPointRec(GetMousePosition(), rectangle);
    }

    bool isClicked() const {
        return isHovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }

    void Draw() const {
        if (status == cardStatus::GUESSED) {
            return;
        }
        Color cardColor;

        if (status == cardStatus::REVEALED) {
            cardColor = GOLD;
        } else {
            if (isHovered()) {
                cardColor = GRAY;
            } else {
                cardColor = DARKGRAY;
            }
        }

        DrawRectangleRounded(rectangle, 0.25f, 4, cardColor);
        DrawRectangleRoundedLinesEx(rectangle, 0.25f, 4, 2.5f, BLACK);

        if (status == cardStatus::REVEALED) {
            int fontSize = rectangle.width * 0.3f;
            int textWidth = MeasureText(TextFormat("%d", id), fontSize);
            float textX = rectangle.x + (rectangle.width / 2.0f) - (textWidth / 2.0f);
            float textY = rectangle.y + (rectangle.height / 2.0f) - (fontSize / 2.0f);

            DrawText(TextFormat("%d", id), textX, textY, fontSize, BLACK);
        }
    }
};

struct Button {
    Rectangle rectangle;
    const char *text;

    bool isHovered() {
        return CheckCollisionPointRec(GetMousePosition(), rectangle);
    }
    bool isClicked() {
        return isHovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }
    void Draw() {
        Color buttonColor;

        if (isHovered()) {
            buttonColor = GRAY;
        } else {
            buttonColor = DARKGRAY;
        }

        DrawRectangleRounded(rectangle, 0.25f, 4, buttonColor);
        DrawRectangleRoundedLinesEx(rectangle, 0.25f, 4, 1.5f, BLACK);
        int fontSize = 20;
        int textWidth = MeasureText(text, fontSize);
        DrawText(text, rectangle.x + (rectangle.width - textWidth) / 2.0f, rectangle.y + (rectangle.height - fontSize) / 2.0f, fontSize, WHITE);
    }
};

struct gameStats {
    int movesCount = 0;
    float gameTime = 0.0f;
    int pairsLeft = 0;
    float revealTimer = 0.0f;

    void Reset(int totalPairs){
        movesCount = 0;
        gameTime = 0.0f;
        pairsLeft = totalPairs;
        revealTimer = 0.0f;
    }
};

vector<Card> createDeck(){
    vector<Card> deck;
    int totalCards = cols * rows;

    vector<int> cardIDs;
    for (int i = 0; i < totalCards; i++) {
        cardIDs.push_back((i / 2) + 1);
    }
    random_device rn;
    mt19937 g(rn());
    shuffle(cardIDs.begin(), cardIDs.end(), g);

    int IDIndex = 0;
    for (int i = 0; i < cols; i++) {
        for (int j = 0; j < rows; j++) {
            Card temp;
            temp.id = cardIDs[IDIndex];
            IDIndex++;

            temp.rectangle.x = ((screenWidth - ((cols * cardSize) + (cols * gapSize))) / 2) + i * (cardSize + gapSize);
            temp.rectangle.y = ((screenHeight - ((rows * cardSize) + (rows * gapSize))) / 2) + 20 + j * (cardSize + gapSize);
            temp.rectangle.width = cardSize;
            temp.rectangle.height = cardSize;
            temp.status = cardStatus::HIDDEN;
            deck.push_back(temp);
        }
    }
    return deck;
}

void drawMenu(Button &playButtonEasy, Button &playButtonMedium, Button &playButtonHard, Button &settingsButton, Button &exitButton) {
    int titleFontSize = 110;
    const char *title = "MEMORY GAME";
    int titleWidth = MeasureText(title, titleFontSize);
    DrawText(title, (screenWidth - titleWidth) / 2 + 3, 90 + 3, titleFontSize, LIGHTGRAY);
    DrawText(title, (screenWidth - titleWidth) / 2, 90, titleFontSize, BLACK);

    int subFontSize = 35;
    const char *subtitle = "Dopasuj w pary wszystkie karty!";
    int subWidth = MeasureText(subtitle, subFontSize);
    DrawText(subtitle, (1280 - subWidth) / 2, 200, subFontSize, DARKGRAY);

    int playFontSize = 25;
    const char *play = "Wybierz poziom trudnosci:";
    int playWidth = MeasureText(play, playFontSize);
    DrawText(play, (1280 - playWidth) / 2, 270, playFontSize, DARKGRAY);

    playButtonEasy.Draw();
    playButtonMedium.Draw();
    playButtonHard.Draw();
    settingsButton.Draw();
    exitButton.Draw();
}

void drawSettings(){
    DrawText("SETTINGS", 100, 100, 50, BLACK);
}

void drawGame(vector<Card> &deck, Button &backButton, gameStats &stats){
    for (int i = 0; i < deck.size(); i++) {
        deck[i].Draw();
    }
    backButton.Draw();

    int fontSize = 30;
    const char *timeText = TextFormat("Czas: %ds", (int)stats.gameTime);
    const char *movesText = TextFormat("Ruchy: %d", stats.movesCount);
    const char *pairsText = TextFormat("Pozostalo par: %d", stats.pairsLeft);

    int timeWidth = MeasureText(timeText, fontSize);
    int movesWidth = MeasureText(movesText, fontSize);
    int pairsWidth = MeasureText(pairsText, fontSize);

    int leftColumnCenter = 1280 / 4;
    int middleColumnCenter = 1280 / 2;
    int rightColumnCenter = (1280 / 4) * 3;

    DrawText(timeText, leftColumnCenter - (timeWidth / 2), 40, fontSize, DARKGRAY);
    DrawText(movesText, middleColumnCenter - (movesWidth / 2), 40, fontSize, DARKGRAY);
    DrawText(pairsText, rightColumnCenter - (pairsWidth / 2), 40, fontSize, DARKGRAY);
}

void checkForMouse(vector<Card> &deck, gameStats &stats) {
    int revealedCards = 0;
    int firstCard = -1;
    int secondCard = -1;

    for (int i = 0; i < deck.size(); i++) {
        if (deck[i].status == cardStatus::REVEALED) {
            revealedCards++;
            if (firstCard == -1) {
                firstCard = i;
            } else {
                secondCard = i;
            }
        }
    }
    if (revealedCards == 2) {
        if (stats.revealTimer <= 0.0f) {
            stats.revealTimer = 0.8f;
            stats.movesCount++;
        } else {
            stats.revealTimer -= GetFrameTime();
            if (stats.revealTimer <= 0.0f) {
                if (deck[firstCard].id == deck[secondCard].id) {
                    deck[firstCard].status = cardStatus::GUESSED;
                    deck[secondCard].status = cardStatus::GUESSED;
                    stats.pairsLeft--;
                } else {
                    deck[firstCard].status = cardStatus::HIDDEN;
                    deck[secondCard].status = cardStatus::HIDDEN;
                }
                stats.revealTimer = 0.0f;
            }
        }
        return;
    }
    for (int i = 0; i < deck.size(); i++) {
        if (deck[i].status == cardStatus::HIDDEN && deck[i].isClicked()) {
            deck[i].status = cardStatus::REVEALED;
            break;
        }
    }
}

int main(){
    InitWindow(screenWidth, screenHeight, "Memory Game");
    SetTargetFPS(60);

    Button playButtonEasy   = { { 1280 / 2.0f - 210, 330, 120, 120 }, "Easy" };
    Button playButtonMedium = { { 1280 / 2.0f - 60, 330, 120, 120 }, "Medium" };
    Button playButtonHard   = { { 1280 / 2.0f + 90, 330, 120, 120 }, "Hard" };
    Button settingsButton   = { { 1280 / 2.0f - 210, 470, 420, 80 }, "Ustawienia" };
    Button exitButton       = { { 1280 / 2.0f - 210, 570, 420, 80 }, "Wyjscie" };

    Button gameplayBackButton = { { 30, 645, 160, 45 }, "Wstecz" };
    Button settingsBackButton = { { 30, 645, 160, 45 }, "Wstecz" };

    Button victoryRestartButton = { { 1280 / 2.0f - 150, 720 / 2.0f + 10, 300, 50 }, "Zagraj ponownie" };
    Button victoryMenuButton = { { 1280 / 2.0f - 150, 720 / 2.0f + 75, 300, 50 }, "Menu glowne" };

    vector<Card> deck;
    gameStats stats;
    gameState currentGameState = gameState::MENU;
    bool keepRunning = true;

    while (keepRunning && !WindowShouldClose()) {
        switch (currentGameState) {
            case gameState::MENU:
                if (playButtonMedium.isClicked()) {
                    deck = createDeck();
                    stats.Reset(cols * rows / 2);
                    currentGameState = gameState::GAMEPLAY;
                } else if (settingsButton.isClicked()) {
                    currentGameState = gameState::SETTINGS;
                } else if (exitButton.isClicked()) {
                    keepRunning = false;
                }
                break;
            case gameState::GAMEPLAY:
                if (gameplayBackButton.isClicked()) {
                    currentGameState = gameState::MENU;
                }
                stats.gameTime += GetFrameTime();
                checkForMouse(deck, stats);
                break;
            case gameState::SETTINGS:
                // sprawdzenie przyciskow
                break;
        }
        BeginDrawing();
        ClearBackground(WHITE);
        switch (currentGameState) {
            case gameState::MENU:
                drawMenu(playButtonEasy, playButtonMedium, playButtonHard, settingsButton, exitButton);
                break;
            case gameState::GAMEPLAY:
                drawGame(deck, gameplayBackButton, stats);
                break;
            case gameState::SETTINGS:
                drawSettings();
                break;
        }
        EndDrawing();
    }
    CloseWindow();
    return 0;
}