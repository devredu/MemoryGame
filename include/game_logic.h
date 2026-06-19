#ifndef MEMORYGAME_GAME_LOGIC_H
#define MEMORYGAME_GAME_LOGIC_H

#include "common.h"
#include <vector>
using namespace std;

const GameConfig easyConfig = {4, 4, 115};
const GameConfig mediumConfig = {6, 4, 100};
const GameConfig hardConfig = {6, 5, 95};

struct Card {
    int id;
    Rectangle rectangle;
    CardStatus status;
    bool isHovered();
    bool isClicked();
    void draw();
};

vector<Card> createDeck(GameConfig config);
void updateGameplay(std::vector<Card> &deck, GameStats &stats, Sound cardClickSound, Sound cardMatchSound);

#endif //MEMORYGAME_GAME_LOGIC_H