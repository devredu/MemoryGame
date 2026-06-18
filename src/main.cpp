#include "raylib.h"
#include <algorithm>
#include <random>
#include <vector>
using namespace std;

const int screenWidth = 1280;
const int screenHeight = 720;
const int gapSize = 15;
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

    void reset(int totalPairs){
        movesCount = 0;
        gameTime = 0.0f;
        pairsLeft = totalPairs;
        revealTimer = 0.0f;
    }
};

struct Card {
    int id;
    Rectangle rectangle;
    CardStatus status;

    bool isHovered() const {
        return CheckCollisionPointRec(GetMousePosition(), rectangle);
    }
    bool isClicked() const {
        return isHovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }
    void draw() const {
        if (status == CardStatus::GUESSED) {
            return;
        }
        Color cardColor;

        if (status == CardStatus::REVEALED) {
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

        if (status == CardStatus::REVEALED) {
            int fontSize = rectangle.width * 0.3f;
            int textWidth = MeasureText(TextFormat("%d", id), fontSize);
            float textX = rectangle.x + (rectangle.width - textWidth) / 2.0f;
            float textY = rectangle.y + (rectangle.height - fontSize) / 2.0f;
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
    void draw() {
        Color buttonColor;

        if (isHovered()) {
            buttonColor = GRAY;
        } else {
            buttonColor = DARKGRAY;
        }

        Rectangle shadowRectangle = {rectangle.x + 4, rectangle.y + 4, rectangle.width, rectangle.height};
        DrawRectangleRounded(shadowRectangle, 0.25f, 4, BLACK);
        DrawRectangleRounded(rectangle, 0.25f, 4, buttonColor);
        DrawRectangleRoundedLinesEx(rectangle, 0.25f, 4, 2.5f, BLACK);

        int fontSize = 20;
        int textWidth = MeasureText(text, fontSize);
        float textX = rectangle.x + (rectangle.width - textWidth) / 2.0f;
        float textY = rectangle.y + (rectangle.height - fontSize) / 2.0f;
        DrawText(text, textX, textY, fontSize, WHITE);
    }
};

vector<Card> createDeck(GameConfig config){
    vector<Card> deck;
    int totalCards = config.cols * config.rows;

    vector<int> cardIDs;
    for (int i = 0; i < totalCards; i++) {
        cardIDs.push_back((i / 2) + 1);
    }
    random_device rn;
    mt19937 g(rn());
    shuffle(cardIDs.begin(), cardIDs.end(), g);

    int IDIndex = 0;
    for (int i = 0; i < config.cols; i++) {
        for (int j = 0; j < config.rows; j++) {
            Card temp;
            temp.id = cardIDs[IDIndex];
            IDIndex++;

            temp.rectangle.x = ((screenWidth - ((config.cols * config.cardSize) + (config.cols * gapSize))) / 2.0f) + i * (config.cardSize + gapSize);
            temp.rectangle.y = ((screenHeight - ((config.rows * config.cardSize) + (config.rows * gapSize))) / 2.0f) + 20 + j * (config.cardSize + gapSize);
            temp.rectangle.width = config.cardSize;
            temp.rectangle.height = config.cardSize;
            temp.status = CardStatus::HIDDEN;
            deck.push_back(temp);
        }
    }
    return deck;
}

void drawMenu(Button &playButtonEasy, Button &playButtonMedium, Button &playButtonHard, Button &settingsButton, Button &exitButton) {
    int titleFontSize = 110;
    const char *title = "MEMORY GAME";
    int titleWidth = MeasureText(title, titleFontSize);
    float titleX = (screenWidth - titleWidth) / 2.0f;
    DrawText(title, titleX + 4, 90 + 4, titleFontSize, BLACK);
    DrawText(title, titleX, 90, titleFontSize, GOLD);

    int subFontSize = 37;
    const char *subtitle = "Dopasuj w pary wszystie karty!";
    int subWidth = MeasureText(subtitle, subFontSize);
    float subX = (screenWidth - subWidth) / 2.0f;
    DrawText(subtitle, subX, 200, subFontSize, BLACK);

    // int difficultyFontSize = 30;
    // const char *difficulty = "Zagraj!";
    // int difficultyWidth = MeasureText(difficulty, difficultyFontSize);
    // float difficultyX = (screenWidth - difficultyWidth) / 2.0f;
    // DrawText(difficulty, difficultyX, 300, difficultyFontSize, BLACK);

    playButtonEasy.draw();
    playButtonMedium.draw();
    playButtonHard.draw();
    settingsButton.draw();
    exitButton.draw();
}

void drawSettings(){
    DrawText("SETTINGS", 100, 100, 50, BLACK);
}

void drawGame(vector<Card> &deck, Button &backButton, GameStats &stats){
    for (int i = 0; i < deck.size(); i++) {
        deck[i].draw();
    }
    backButton.draw();

    int fontSize = 30;
    const char *timeText = TextFormat("Czas: %ds", (int)stats.gameTime);
    const char *movesText = TextFormat("Ruchy: %d", stats.movesCount);
    const char *pairsText = TextFormat("Pozostalo par: %d", stats.pairsLeft);
    int timeWidth = MeasureText(timeText, fontSize);
    int movesWidth = MeasureText(movesText, fontSize);
    int pairsWidth = MeasureText(pairsText, fontSize);
    int leftColumnCenter = screenWidth / 4;
    int middleColumnCenter = screenWidth / 2;
    int rightColumnCenter = (screenWidth / 4) * 3;
    DrawText(timeText, leftColumnCenter - (timeWidth / 2), 40, fontSize, BLACK);
    DrawText(movesText, middleColumnCenter - (movesWidth / 2), 40, fontSize, BLACK);
    DrawText(pairsText, rightColumnCenter - (pairsWidth / 2), 40, fontSize, BLACK);
}

void drawVictory(Button &menuButton, GameStats &stats) {
    int titleFontSize = 70;
    const char *title = "GRATULACJE!";
    int titleWidth = MeasureText(title, titleFontSize);
    float titleX = (screenWidth - titleWidth) / 2.0f;
    DrawText(title, titleX + 4, 220 + 4, titleFontSize, BLACK);
    DrawText(title, titleX, 220, titleFontSize, GOLD);

    int statsFontSize = 30;
    const char *timeText = TextFormat("Twoj czas: %ds", (int)stats.gameTime);
    const char *movesText = TextFormat("Wykonane ruchy: %d", stats.movesCount);
    int timeWidth = MeasureText(timeText, statsFontSize);
    int movesWidth = MeasureText(movesText, statsFontSize);
    float timeX = (screenWidth - timeWidth) / 2.0f;
    float movesX = (screenWidth - movesWidth) / 2.0f;
    DrawText(timeText, timeX, 320, statsFontSize, BLACK);
    DrawText(movesText, movesX, 370, statsFontSize, BLACK);

    menuButton.draw();
}

void updateGameplay(vector<Card> &deck, GameStats &stats) {
    int revealedCards = 0;
    int firstCard = -1;
    int secondCard = -1;

    for (int i = 0; i < deck.size(); i++) {
        if (deck[i].status == CardStatus::REVEALED) {
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
            stats.revealTimer = 0.65f;
            stats.movesCount++;
        }
        stats.revealTimer -= GetFrameTime();
        if (stats.revealTimer <= 0.0f) {
                if (deck[firstCard].id == deck[secondCard].id) {
                    deck[firstCard].status = CardStatus::GUESSED;
                    deck[secondCard].status = CardStatus::GUESSED;
                    stats.pairsLeft--;
                } else {
                    deck[firstCard].status = CardStatus::HIDDEN;
                    deck[secondCard].status = CardStatus::HIDDEN;
                }
                stats.revealTimer = 0.0f;
            }
        return;
    }
    for (int i = 0; i < deck.size(); i++) {
        if (deck[i].status == CardStatus::HIDDEN && deck[i].isClicked()) {
            deck[i].status = CardStatus::REVEALED;
            break;
        }
    }
}

int main(){
    InitWindow(screenWidth, screenHeight, "Memory Game");
    SetTargetFPS(60);
    Texture2D background = LoadTexture("../assets/textures/background.png");

    Button playButtonEasy = {{screenWidth / 2.0f - 210,370, 130, 80}, "LATWY"};
    Button playButtonMedium = {{screenWidth / 2.0f - 210 + 145, 370, 130, 80}, "SREDNI"};
    Button playButtonHard = {{screenWidth / 2.0f - 210 + 290, 370, 130, 80}, "TRUDNY"};
    Button settingsButton = {{screenWidth / 2.0f - 210, 470, 420, 80}, "USTAWIENIA"};
    Button exitButton = {{screenWidth / 2.0f - 210, 570, 420, 80}, "WYJSCIE"};
    Button gameplayBackButton = {{30, 645, 160, 45}, "WSTECZ"};
    Button settingsBackButton = {{30, 645, 160, 45}, "WSTECZ"};
    Button victoryMenuButton = {{screenWidth / 2.0f - 210, 450, 420, 80}, "MENU GLOWNE"};

    vector<Card> deck;
    GameStats stats;
    GameState currentGameState = GameState::VICTORY;
    bool keepRunning = true;

    while (keepRunning && !WindowShouldClose()) {
        switch (currentGameState) {

            case GameState::MENU:
                if (playButtonEasy.isClicked()) {
                    GameConfig config = {4, 4, 115};
                    deck = createDeck(config);
                    stats.reset(config.cols * config.rows / 2);
                    currentGameState = GameState::GAMEPLAY;
                }
                else if (playButtonMedium.isClicked()) {
                    GameConfig config = {6, 4, 100};
                    deck = createDeck(config);
                    stats.reset(config.cols * config.rows / 2);
                    currentGameState = GameState::GAMEPLAY;
                }
                else if (playButtonHard.isClicked()) {
                    GameConfig config = {6, 5, 95};
                    deck = createDeck(config);
                    stats.reset(config.cols * config.rows / 2);
                    currentGameState = GameState::GAMEPLAY;
                }
                else if (settingsButton.isClicked()) {
                    currentGameState = GameState::SETTINGS;
                }
                else if (exitButton.isClicked()) {
                    keepRunning = false;
                }
                break;

            case GameState::GAMEPLAY:
                if (gameplayBackButton.isClicked()) {
                    currentGameState = GameState::MENU;
                }
                stats.gameTime += GetFrameTime();
                updateGameplay(deck, stats);
                if (stats.pairsLeft == 0) {
                    currentGameState = GameState::VICTORY;
                }
                break;

            case GameState::SETTINGS:
                if (settingsBackButton.isClicked()) {
                    currentGameState = GameState::MENU;
                }
                break;

            case GameState::VICTORY:
                if (victoryMenuButton.isClicked()) {
                    currentGameState = GameState::MENU;
                }
                break;
        }
        BeginDrawing();
        DrawTexture(background, 0, 0, WHITE);
        switch (currentGameState) {

            case GameState::MENU:
                drawMenu(playButtonEasy, playButtonMedium, playButtonHard, settingsButton, exitButton);
                break;

            case GameState::GAMEPLAY:
                drawGame(deck, gameplayBackButton, stats);
                break;

            case GameState::SETTINGS:
                drawSettings();
                break;

            case GameState::VICTORY:
                drawVictory(victoryMenuButton, stats);
                break;
        }
        EndDrawing();
    }
    UnloadTexture(background);
    CloseWindow();
    return 0;
}