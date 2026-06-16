#include "raylib.h"
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

enum class gameState {MENU, GAMEPLAY, SETTINGS};

enum class cardStatus {HIDDEN, REVEALED, GUESSED};

class Card {
public:
    int id;
    Rectangle rectangle;
    Color color;
    cardStatus status;
};

vector<Card> createDeck(int screenWidth, int screenHeight, int cols, int rows, int cardSize, int gapSize){
    vector<Card> deck;
    int totalCards = cols * rows;

    vector<int> cardIDs;
    for (int i = 0; i < totalCards; i++) {
        cardIDs.push_back((i / 2) + 1);
    }
    random_device rn;
    mt19937 g(rn());
    shuffle(cardIDs.begin(), cardIDs.end(), g);
    // wymieszanie id kart

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
            temp.color = RED;
            temp.status = cardStatus::HIDDEN;
            deck.push_back(temp);
        }
    }
    return deck;
}

void drawMenu(){
    DrawText("MENU", 100, 100, 50, BLACK);
}

void drawSettings(){
    DrawText("SETTINGS", 100, 100, 50, BLACK);
}

void drawDeck(const vector<Card> &deck){
    for (int i = 0; i < deck.size(); i++) {
        int fontSize = deck[i].rectangle.width * 0.3f;
        int textWidth = MeasureText(TextFormat("%d", deck[i].id), fontSize);
        float textX = deck[i].rectangle.x + (deck[i].rectangle.width / 2) - (textWidth / 2);
        float textY = deck[i].rectangle.y + (deck[i].rectangle.height / 2) - (fontSize / 2);

        DrawRectangleRec(deck[i].rectangle, deck[i].color);
        if (deck[i].status == cardStatus::REVEALED || deck[i].status == cardStatus::GUESSED) {
            DrawText(TextFormat("%d", deck[i].id), textX, textY, fontSize, BLACK);
        }
    }
}

void checkForMouse(vector<Card> &deck){
    int revealedCars = 0;
    for (int i = 0; i < deck.size(); i++) {
        if (deck[i].status == cardStatus::REVEALED) {
            revealedCars++;
        }
    }

    if (revealedCars == 2) {
        WaitTime(0.7);
        int firstCard = -1;
        int secondCard = -1;
        for (int i = 0; i < deck.size(); i++) {
            if (deck[i].status == cardStatus::REVEALED) {
                if (firstCard == -1) {
                    firstCard = i;
                } else {
                    secondCard = i;
                }
            }
        }

        if (deck[firstCard].id == deck[secondCard].id) {
            deck[firstCard].status = cardStatus::GUESSED;
            deck[firstCard].color = LIGHTGRAY;
            deck[secondCard].status = cardStatus::GUESSED;
            deck[secondCard].color = LIGHTGRAY;
        } else {
            deck[firstCard].status = cardStatus::HIDDEN;
            deck[firstCard].color = RED;;
            deck[secondCard].status = cardStatus::HIDDEN;
            deck[secondCard].color = RED;
        }
        return;
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        for (int i = 0; i < deck.size(); i++) {
            if (CheckCollisionPointRec(mousePos, deck[i].rectangle)) {
                if (deck[i].status == cardStatus::HIDDEN) {
                    deck[i].color = GREEN;
                    deck[i].status = cardStatus::REVEALED;
                }
            }
        }
    }
}


int main(){
    const int screenWidth = 1280;
    const int screenHeight = 720;
    const int cols = 4;
    const int rows = 4;
    const int cardSize = 100;
    const int gapSize = 15;

    InitWindow(screenWidth, screenHeight, "Memory Game");
    SetTargetFPS(60);

    vector<Card> deck = createDeck(screenWidth, screenHeight, cols, rows, cardSize, gapSize);
    gameState currentGameState = gameState::MENU;

    while (!WindowShouldClose()) {
        switch (currentGameState) {
            case gameState::MENU:
                // sprawdzenie przyciskow
                break;
            case gameState::GAMEPLAY:
                checkForMouse(deck);
                break;
            case gameState::SETTINGS:
                // sprawdzenie przyciskow
                break;
        }
        BeginDrawing();
        ClearBackground(WHITE);
        switch (currentGameState) {
            case gameState::MENU:
                drawMenu();
                break;
            case gameState::GAMEPLAY:
                drawDeck(deck);
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