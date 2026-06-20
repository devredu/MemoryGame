/**
 * @file widgets.h
 * @brief Deklaracje struktur obiektów (kart i przycisków) oraz stanów gry.
 * @details Plik zawiera deklaracje kart i przycisków do interfejsu użytkownika
 * oraz logiki gry Memory. Znajdują się tu struktury reprezentujące pojedynczą kartę (Card)
 * oraz uniwersalny przycisk (Button), wraz z enumami określającymi stany gry i kart.
 */

#ifndef MEMORYGAME_WIDGETS_H
#define MEMORYGAME_WIDGETS_H
#include "raylib.h"

/**
 * @brief Stany ekranów gry.
 */
enum struct GameStatus {
    MENU, ///< Ekran menu głównego.
    GAMEPLAY, ///< Ekran właściwej rozgrywki.
    SETTINGS, ///< Ekran ustawień muzyki.
    VICTORY ///< Ekran końca gry po wygranej.
};

/**
 * @brief Stany widoczności karty na planszy.
 */
enum struct CardStatus {
    HIDDEN, ///< Karta jest zakryta (gracz widzi rewers).
    REVEALED, ///< Karta jest odkryta (gracz widzi awers i numer).
    GUESSED ///< Karta została już odgadnięta w parze i jest niewidoczna.
};

/**
 * @brief Struktura reprezentująca pojedynczą kartę na planszy gry.
 */
struct Card {
    int id; ///< Identyfikator karty.
    Rectangle rectangle; ///< Pozycja (X, Y) oraz wymiary (szerokość, wysokość) karty na ekranie.
    CardStatus status; ///< Stan widoczności karty.
    /**
     * @brief Sprawdza, czy gracz najechał myszką na kartę.
     * @return true Jeśli kursor znajduje się nad kartą.
     * @return false Jeśli kursor nie znajduje się nad kartą.
     */
    bool isHovered();
    /**
     * @brief Sprawdza, czy karta została naciśnięta.
     * @return true Jeśli karta została naciśnięta.
     * @return false Jeśli karta nie została naciśnięta.
     */
    bool isClicked();
    /**
     * @brief Rysuje kartę na ekranie wraz z jej cieniem, obramowaniem i tekstem.
     */
    void draw();
};

/**
 * @brief Struktura reprezentująca przycisk interfejsu użytkownika.
 */
struct Button {
    Rectangle rectangle; ///< Pozycja (X, Y) oraz wymiary (szerokość, wysokość) przycisku.
    const char *text; ///< Napis wyświetlany na przycisku.
    Color baseColor; ///< Podstawowy kolor przycisku.
    Color hoverColor; ///< Kolor przycisku w momencie najechania myszą.
    int fontSize; ///< Rozmiar czcionki tekstu na przycisku.
    /**
     * @brief Sprawdza, czy gracz najechał myszką na przycisk.
     * @return true Jeśli kursor znajduje się nad przyciskiem.
     * @return false Jeśli kursor nie znajduje się nad przyciskiem.
     */
    bool isHovered();
    /**
     * @brief Sprawdza, czy przycisk został naciśnięty.
     * @return true Jeśli przycisk została naciśnięty.
     * @return false Jeśli przycisk nie został naciśnięty.
     */
    bool isClicked();
    /**
     * @brief Rysuje przycisk na ekranie wraz z cieniem, obramowaniem i tekstem.
     */
    void draw();
};

#endif //MEMORYGAME_WIDGETS_H