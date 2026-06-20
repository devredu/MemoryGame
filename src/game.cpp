#include "../include/game.h"
#include <random>
#include <algorithm>

void Stats::reset(int totalPairs) {
    movesCount = 0;
    gameTime = 0.0f;
    pairsLeft = totalPairs;
    revealTimer = 0.0f;
}

vector<Card> createDeck(GameConfig config){
    vector<int> cardIDs;
    int totalCards = config.cols * config.rows;
    int idIndex = 0;
    for (int i = 0; i < totalCards; i++) {
        cardIDs.push_back((i / 2) + 1);
    }
    random_device rn;
    mt19937 g(rn());
    shuffle(cardIDs.begin(), cardIDs.end(), g);

    vector<Card> deck;
    for (int i = 0; i < config.cols; i++) {
        for (int j = 0; j < config.rows; j++) {
            Card temp;
            temp.id = cardIDs[idIndex];
            idIndex++;
            temp.rectangle.x = ((screenWidth - ((config.cols * config.cardSize) + (config.cols * gapSize))) / 2.0f) + i * (config.cardSize + gapSize);
            temp.rectangle.y = ((screenHeight - ((config.rows * config.cardSize) + (config.rows * gapSize))) / 2.0f) + 20.0f + j * (config.cardSize + gapSize);
            temp.rectangle.width = config.cardSize;
            temp.rectangle.height = config.cardSize;
            temp.status = CardStatus::HIDDEN;
            deck.push_back(temp);
        }
    }
    return deck;
}

void updateGameplay(vector<Card> &deck, Stats &stats, Sound cardClickSound, Sound cardMatchSound) {
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
            stats.revealTimer = cardRevealDelay;
            stats.movesCount++;
        }
        stats.revealTimer -= GetFrameTime();
        if (stats.revealTimer <= 0.0f) {
                if (deck[firstCard].id == deck[secondCard].id) {
                    PlaySound(cardMatchSound);
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
            PlaySound(cardClickSound);
            break;
        }
    }
}