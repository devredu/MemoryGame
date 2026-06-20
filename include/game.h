/**
 * @file game.h
 * @brief Deklaracje głównych ustawień i logiki gry Memory.
 * @details Główny plik konfiguracyjny projektu. Znajdują się tutaj deklaracje kolorów, stałe odpowiadające
 * m.in. za rozmiar okna, struktura przechowująca konfigurację dla różnych poziomów trudności, struktura ze statystykami
 * potrzebnymi do działania aplikacji oraz deklaracje funkcji odpowiedzialnych za tworzenie talii kart i aktualizację logiki rozgrywki.
 */

#ifndef MEMORYGAME_GAME_H
#define MEMORYGAME_GAME_H

#include "raylib.h"
#include <vector>
#include "widgets.h"

using namespace std;

/**
 * @name Definicje kolorów
 * @{
 */
const Color COLOR_BLACK        = GetColor(0x000000FF); ///< Kolor czarny.
const Color COLOR_GREEN        = GetColor(0x4CAF50FF); ///< Kolor zielony.
const Color COLOR_GREEN_HOVER  = GetColor(0x66BB6AFF); ///< Kolor zielony.
const Color COLOR_PURPLE       = GetColor(0x4A148CFF); ///< Kolor fioletowy.
const Color COLOR_PURPLE_HOVER = GetColor(0x6A1B9AFF); ///< Jasny kolor fioletowy.
const Color COLOR_PURPLE_DARK  = GetColor(0x2A0854FF); ///< Ciemny kolor fioletowy.
const Color COLOR_RED          = GetColor(0xF44336FF); ///< Kolor czerwony.
const Color COLOR_RED_HOVER    = GetColor(0xEF5350FF); ///< Jasny kolor czerwony.
const Color COLOR_ORANGE       = GetColor(0xFF9800FF); ///< Kolor pomarańczowy.
const Color COLOR_ORANGE_HOVER = GetColor(0xFFB74DFF); ///< Jasny kolor pomarańczowy.
const Color COLOR_WHITE        = GetColor(0xFFFFFFFF); ///< Kolor biały.
const Color COLOR_LIGHT_GRAY   = GetColor(0xD3D3D3FF); ///< Kolor jasnoszary.
/** @} */

/**
 * @name Stałe konfiguracyjne okna i mechaniki gry
 * @{
 */
const int screenWidth = 1280; ///< Szerokość okna aplikacji w pikselach.
const int screenHeight = 720; ///< Wysokość okna aplikacji w pikselach.
const int gapSize = 15; ///< Odstęp między kartami na planszy.
const float cardRevealDelay = 0.65f; ///< Czas, przez który dwie odkryte karty są widoczne.
/** @} */

/**
 * @brief Struktura przechowująca konfigurację wymiarów planszy dla danego poziomu trudności.
 */
struct GameConfig {
    int cols; ///< Liczba kolumn w siatce kart.
    int rows; ///< Liczba wierszy w siatce kart.
    int cardSize; ///< Rozmiar boku pojedynczej karty.
};

/**
 * @name Predefiniowane poziomy trudności gry
 * @{
 */
const GameConfig easyConfig   = {4, 4, 115}; ///< Poziom Łatwy: plansza 4x4, rozmiar karty 115px (8 par).
const GameConfig mediumConfig = {6, 4, 100}; ///< Poziom Średni: plansza 6x4, rozmiar karty 100px (12 par).
const GameConfig hardConfig   = {6, 5, 95}; ///< Poziom Trudny: plansza 6x5, rozmiar karty 95px (15 par).
/** @} */

/**
 * @brief Struktura przechowująca bieżące statystyki i liczniki aktywnej rozgrywki.
 */
struct Stats {
    int movesCount = 0; ///< Liczba wykonanych przez gracza ruchów.
    float gameTime = 0.0f; ///< Czas trwania obecnej rozgrywki w sekundach.
    int pairsLeft = 0; ///< Liczba par pozostałych do odgadnięcia na planszy.
    float revealTimer = 0.0f; ///< Licznik czasu, przez który dwie karty pozostają odkryte.

    /**
     * @brief Resetuje statystyki i przygotowuje strukturę do nowej gry.
     * @param totalPairs Całkowita liczba par na planszy.
     */
    void reset(int totalPairs);
};

/**
 * @brief Tworzy, rozmieszcza na ekranie i losowo tasuje talię kart na planszy.
 * Funkcja generuje pary kart na podstawie podanej konfiguracji,
 * miesza je algorytmem losowym oraz oblicza współrzędne X i Y dla każdej karty tak,
 * aby cała plansza była idealnie wyśrodkowana na ekranie gry.
 * @param config Struktura konfiguracji określająca wymiary tworzonej siatki kart.
 * @return vector<Card> Wektor przygotowanych, potasowanych i rozstawionych na ekranie kart.
 */
vector<Card> createDeck(GameConfig config);

/**
 * @brief Odpowiada za aktualizację logiki gry w każdej klatce.
 * Obsługuje kliknięcia w karty, zlicza ruchy gracza, odlicza czas do zakrycia
 * kart oraz weryfikuje, czy dwie odkryte karty są parą.
 * Dodatkowo odtwarza odpowiednie efekty dzwiękowe przy interakcji.
 * @param deck Referencja do wektora kart, przechowującego wszystkie karty.
 * @param stats Referencja do struktury statystyk bieżącej gry.
 * @param cardClickSound Dźwięk odtwarzany przy kliknięciu i odkrywaniu pojedynczej karty.
 * @param cardMatchSound Dźwięk odtwarzany w momencie poprawnego dopasowania pary kart.
 */
void updateGameplay(std::vector<Card> &deck, Stats &stats, Sound cardClickSound, Sound cardMatchSound);

#endif //MEMORYGAME_GAME_H