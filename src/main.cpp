#include "raylib.h"
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

struct Card {
    int id;
    Rectangle rectangle;
    Color color;
    int status; // 0 - nieodkryta 1 - odkryta 2 - zgadnieta
};

vector<Card> createDeck(int screenWidth, int screenHeight, int cols, int rows, int cardSize, int gapSize){
    vector<Card> deck;
    int totalCards = cols * rows;
    int cardCounter = 0;

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
            temp.status = 0;
            deck.push_back(temp);
        }
    }
    return deck;
}

void drawDeck(const vector<Card> &deck, int cardSize){
    for (int i = 0; i < deck.size(); i++) {
        int fontSize = deck[i].rectangle.width * 0.3f;
        int textWidth = MeasureText(TextFormat("%d", deck[i].id), fontSize);
        float textX = deck[i].rectangle.x + (deck[i].rectangle.width / 2) - (textWidth / 2);
        float textY = deck[i].rectangle.y + (deck[i].rectangle.height / 2) - (fontSize / 2);

        DrawRectangleRec(deck[i].rectangle, deck[i].color);
        if (deck[i].status == 1) {
            DrawText(TextFormat("%d", deck[i].id), textX, textY, fontSize, BLACK);
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

    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePos = GetMousePosition();
            for (int i = 0; i < deck.size(); i++) {
                if (CheckCollisionPointRec(mousePos, deck[i].rectangle)) {
                    deck[i].color = GREEN;
                    deck[i].status = 1;
                }
            }
        }



        BeginDrawing();
        ClearBackground(WHITE);
        drawDeck(deck, cardSize);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}