#include "raylib.h"
#include <vector>
#include "../include/common.h"
#include "../include/game_logic.h"
#include "../include/user_interface.h"
using namespace std;

Font mainFont;

int main(){
    InitWindow(screenWidth, screenHeight, "Memory Game");
    InitAudioDevice();
    SetTargetFPS(60);
    Texture2D background = LoadTexture("assets/textures/background.png");
    mainFont = LoadFontEx("assets/fonts/upheavtt.ttf", 110, NULL, 0);
    SetTextureFilter(mainFont.texture, TEXTURE_FILTER_POINT);
    Sound cardMatchSound = LoadSound("assets/sounds/card_match_sound.mp3");
    Sound victorySound = LoadSound("assets/sounds/victory_sound.mp3");
    Sound cardClickSound = LoadSound("assets/sounds/card_click_sound.mp3");
    Sound buttonClickSound = LoadSound("assets/sounds/button_click_sound.mp3");
    Music backgroundMusic = LoadMusicStream("assets/sounds/music.mp3");
    PlayMusicStream(backgroundMusic);
    SetMusicVolume(backgroundMusic, 0.15f);
    bool isMuted = false;

    Button playButtonEasy = {{370,400, 160, 60}, "LATWY", COLOR_GREEN, COLOR_GREEN_HOVER, 25};
    Button playButtonMedium = {{560, 400, 160, 60}, "SREDNI", COLOR_ORANGE, COLOR_ORANGE_HOVER, 25};
    Button playButtonHard = {{750, 400, 160, 60}, "TRUDNY", COLOR_RED, COLOR_RED_HOVER, 25};
    Button settingsButton = {{430, 480, 420, 60}, "USTAWIENIA", COLOR_PURPLE, COLOR_PURPLE_HOVER, 30};
    Button exitButton = {{430, 560, 420, 60}, "WYJSCIE", COLOR_PURPLE, COLOR_PURPLE_HOVER, 30};
    Button toggleMusicButton = {{430, 240, 420, 60}, "MUZYKA: WLACZONA", COLOR_GREEN, COLOR_GREEN_HOVER, 25};
    Button backButton = {{30, 645, 160, 45}, "WSTECZ", COLOR_RED, COLOR_RED_HOVER, 22};
    Button victoryMenuButton = {{430, 455, 420, 80}, "MENU GLOWNE", COLOR_GREEN, COLOR_GREEN_HOVER, 30};

    vector<Card> deck;
    GameStats stats;
    GameState currentGameState = GameState::MENU;
    bool keepRunning = true;

    while (keepRunning && !WindowShouldClose()) {
        UpdateMusicStream(backgroundMusic);
        switch (currentGameState) {

            case GameState::MENU:
                if (playButtonEasy.isClicked()) {
                    PlaySound(buttonClickSound);
                    deck = createDeck(easyConfig);
                    stats.reset(easyConfig.cols * easyConfig.rows / 2);
                    currentGameState = GameState::GAMEPLAY;
                }
                else if (playButtonMedium.isClicked()) {
                    PlaySound(buttonClickSound);
                    deck = createDeck(mediumConfig);
                    stats.reset(mediumConfig.cols * mediumConfig.rows / 2);
                    currentGameState = GameState::GAMEPLAY;
                }
                else if (playButtonHard.isClicked()) {
                    PlaySound(buttonClickSound);
                    deck = createDeck(hardConfig);
                    stats.reset(hardConfig.cols * hardConfig.rows / 2);
                    currentGameState = GameState::GAMEPLAY;
                }
                else if (settingsButton.isClicked()) {
                    PlaySound(buttonClickSound);
                    currentGameState = GameState::SETTINGS;
                }
                else if (exitButton.isClicked()) {
                    keepRunning = false;
                }
                break;

            case GameState::GAMEPLAY:
                if (backButton.isClicked()) {
                    PlaySound(buttonClickSound);
                    currentGameState = GameState::MENU;
                }
                stats.gameTime += GetFrameTime();
                updateGameplay(deck, stats, cardClickSound, cardMatchSound);
                if (stats.pairsLeft == 0) {
                    PlaySound(victorySound);
                    currentGameState = GameState::VICTORY;
                }
                break;

            case GameState::SETTINGS:
                if (backButton.isClicked()) {
                    PlaySound(buttonClickSound);
                    currentGameState = GameState::MENU;
                }
                if (toggleMusicButton.isClicked()) {
                    PlaySound(buttonClickSound);
                    isMuted = !isMuted;
                }
                if (isMuted) {
                    PauseMusicStream(backgroundMusic);
                    toggleMusicButton.text = "MUZYKA: WYLACZONA";
                    toggleMusicButton.baseColor = COLOR_RED;
                    toggleMusicButton.hoverColor = COLOR_RED_HOVER;
                } else {
                    ResumeMusicStream(backgroundMusic);
                    toggleMusicButton.text = "MUZYKA: WLACZONA";
                    toggleMusicButton.baseColor = COLOR_GREEN;
                    toggleMusicButton.hoverColor = COLOR_GREEN_HOVER;
                }
                break;

            case GameState::VICTORY:
                if (victoryMenuButton.isClicked()) {
                    PlaySound(buttonClickSound);
                    currentGameState = GameState::MENU;
                }
                break;
        }
        BeginDrawing();
        DrawTexture(background, 0, 0, COLOR_LIGHT_GRAY);
        switch (currentGameState) {

            case GameState::MENU:
                drawMenu(playButtonEasy, playButtonMedium, playButtonHard, settingsButton, exitButton);
                break;

            case GameState::GAMEPLAY:
                drawGame(deck, backButton, stats);
                break;

            case GameState::SETTINGS:
                drawSettings(backButton, toggleMusicButton);
                break;

            case GameState::VICTORY:
                drawVictory(victoryMenuButton, stats);
                break;
        }
        EndDrawing();
    }
    UnloadTexture(background);
    UnloadFont(mainFont);
    UnloadSound(cardMatchSound);
    UnloadSound(victorySound);
    UnloadSound(cardClickSound);
    UnloadSound(buttonClickSound);
    UnloadMusicStream(backgroundMusic);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}