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
const int cardSize = 100;
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
        Color cardColor;

        if (status == cardStatus::GUESSED) {
            cardColor = GRAY;
        } else if (status == cardStatus::REVEALED) {
            cardColor = GREEN;
        } else {
            if (isHovered()) {
                cardColor = ORANGE;
            } else {
                cardColor = RED;
            }
        }

        DrawRectangleRounded(rectangle, 0.25f, 4, cardColor);
        DrawRectangleRoundedLinesEx(rectangle, 0.25f, 4, 3, BLACK);

        if (status == cardStatus::REVEALED || status == cardStatus::GUESSED) {
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
            temp.rectangle.y = ((screenHeight - ((rows * cardSize) + (rows * gapSize))) / 2) + j * (cardSize + gapSize);
            temp.rectangle.width = cardSize;
            temp.rectangle.height = cardSize;
            temp.status = cardStatus::HIDDEN;
            deck.push_back(temp);
        }
    }
    return deck;
}

void drawMenu(Button &playButton, Button &settingsButton, Button &exitButton) {
    int titleFontSize = 64;
    const char *title = "MEMORY GAME";
    int titleWidth = MeasureText(title, titleFontSize);
    DrawText(title, (screenWidth - titleWidth) / 2 + 3, 130 + 3, titleFontSize, LIGHTGRAY); // tutaj to zmienic z magic numbers na automat
    DrawText(title, (screenWidth - titleWidth) / 2, 130, titleFontSize, BLACK);

    int subFontSize = 20;
    const char *subtitle = "Zapamietaj pozycje i dopasuj w pary wszystkie karty!";
    int subWidth = MeasureText(subtitle, subFontSize);
    DrawText(subtitle, (1280 - subWidth) / 2, 210, subFontSize, DARKGRAY);

    playButton.Draw();
    settingsButton.Draw();
    exitButton.Draw();
}

void drawSettings(){
    DrawText("SETTINGS", 100, 100, 50, BLACK);
}

void drawGame(vector<Card> &deck){
    for (int i = 0; i < deck.size(); i++) {
        deck[i].Draw();
    }
}

void checkForMouse(vector<Card> &deck, float &revealTimer) {
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
        if (revealTimer <= 0.0f) {
            revealTimer = 0.8f;
        } else {
            revealTimer -= GetFrameTime();
            if (revealTimer <= 0.0f) {
                if (deck[firstCard].id == deck[secondCard].id) {
                    deck[firstCard].status = cardStatus::GUESSED;
                    deck[secondCard].status = cardStatus::GUESSED;
                } else {
                    deck[firstCard].status = cardStatus::HIDDEN;
                    deck[secondCard].status = cardStatus::HIDDEN;
                }
                revealTimer = 0.0f;
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

    Button playButton = { { 1280 / 2.0f - 150, 290, 300, 55 }, "Zagraj" };
    Button settingsButton = { { 1280 / 2.0f - 150, 370, 300, 55 }, "Ustawienia" };
    Button exitButton = { { 1280 / 2.0f - 150, 450, 300, 55 }, "Wyjscie" };

    Button gameplayBackButton = { { 30, 30, 140, 45 }, "<- Menu" };
    Button settingsBackButton = { { 30, 30, 140, 45 }, "<- Wstecz" };

    Button victoryRestartButton = { { 1280 / 2.0f - 150, 720 / 2.0f + 10, 300, 50 }, "Zagraj ponownie" };
    Button victoryMenuButton = { { 1280 / 2.0f - 150, 720 / 2.0f + 75, 300, 50 }, "Menu glowne" };

    vector<Card> deck;
    gameState currentGameState = gameState::MENU;
    float revealTimer = 0.0f;
    bool keepRunning = true;

    while (keepRunning && !WindowShouldClose()) {
        switch (currentGameState) {
            case gameState::MENU:
                if (playButton.isClicked()) {
                    deck = createDeck();
                    revealTimer = 0.0f;
                    currentGameState = gameState::GAMEPLAY;
                } else if (settingsButton.isClicked()) {
                    currentGameState = gameState::SETTINGS;
                } else if (exitButton.isClicked()) {
                    keepRunning = false;
                }
                break;
            case gameState::GAMEPLAY:
                checkForMouse(deck, revealTimer);
                break;
            case gameState::SETTINGS:
                // sprawdzenie przyciskow
                break;
        }
        BeginDrawing();
        ClearBackground(WHITE);
        switch (currentGameState) {
            case gameState::MENU:
                drawMenu(playButton, settingsButton, exitButton);
                break;
            case gameState::GAMEPLAY:
                drawGame(deck);
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