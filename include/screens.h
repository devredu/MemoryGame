/**
 * @file screens.h
 * @brief Deklaracje funkcji rysujących poszczególne ekrany gry Memory oraz tekst.
 * @details Plik odpowiada za część wizualną aplikacji.
 * Znajdują się tutaj deklaracje funkcji rysujących menu główne, ekran ustawień,
 * właściwą planszę rozgrywki oraz ekran podsumowania po wygranej. Dodatkowo jest też funkcja
 * odpowiadająca za rysowanie tekstu z efektem cienia.
 */
#ifndef MEMORYGAME_SCREENS_H
#define MEMORYGAME_SCREENS_H
#include "raylib.h"
#include "game.h"
#include "widgets.h"

extern Font mainFont;
/**
 * @brief Rysuje na ekranie tekst z efektem cienia.
 * @param font Referencja do załadowanej czcionki.
 * @param text Treść napisu do wyświetlenia.
 * @param x Współrzędna X napisu.
 * @param y Współrzędna Y napisu.
 * @param fontSize Rozmiar czcionki.
 * @param spacing Odstęp między literami.
 * @param textColor Kolor tekstu.
 * @param onCenter Zmienna określająca, czy tekst ma zostać automatycznie wyśrodkowany w osi X.
 */
void drawTextWithShadow(Font &font, const char *text, float x, float y, int fontSize, float spacing, Color textColor, bool onCenter);
/**
 * @brief Rysuje ekran menu głównego gry.
 * @details Wyświetla tytuł gry, podtytuł oraz zestaw przycisków do wyboru poziomu trudności, wejścia w ustawienia i wyjścia.
 * @param playButtonEasy Przycisk poziomu łatwego.
 * @param playButtonMedium Przycisk poziomu średniego.
 * @param playButtonHard Przycisk poziomu trudnego.
 * @param settingsButton Przycisk przejścia do ustawień.
 * @param exitButton Przycisk zamknięcia gry.
 */
void drawMenu(Button &playButtonEasy, Button &playButtonMedium, Button &playButtonHard, Button &settingsButton, Button &exitButton);
/**
 * @brief Rysuje ekran ustawień gry.
 * @details Wyświetla napis "Ustawienia" oraz przyciski odpowiedzialne za sterowanie muzyką i powrót.
 * @param backButton Przycisk powrotu do menu głównego.
 * @param toggleMusicButton Przycisk włączania/wyłączania muzyki w tle.
 */
void drawSettings(Button &backButton, Button &toggleMusicButton);
/**
 * @brief Rysuje ekran rozgrywki.
 * @details Iteruje po wektorze kart i wywołuje ich funkcje rysujące, renderuje przycisk powrotu oraz statystyki czasu, ruchów i pozostałych par.
 * @param deck Referencja do wektora kart tworzących planszę.
 * @param backButton Przycisk powrotu do menu głównego.
 * @param stats Referencja do struktury przechowującej bieżące statystyki gry.
 */
void drawGame(std::vector<Card> &deck, Button &backButton, Stats &stats);
/**
 * @brief Rysuje ekran końcowy po odgadnięciu wszystkich par.
 * @details Wyświetla napis z gratulacjami, końcowe statystyki gracza (czas i ruchy) oraz przycisk powrotu.
 * @param menuButton Przycisk powrotu do menu głównego.
 * @param stats Referencja do struktury ze statystykami ukończonej rozgrywki.
 */
void drawVictory(Button &menuButton, Stats &stats);

#endif //MEMORYGAME_SCREENS_H