#include "../include/game_logic.h"
#include <random>
#include <algorithm>
using namespace std;

bool Card::isHovered() {
    return CheckCollisionPointRec(GetMousePosition(), rectangle);
}
bool Card::isClicked() {
    return isHovered() && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}
void Card::draw() {
    if (status == CardStatus::GUESSED) {
        return;
    }
    Color cardColor;
    Color borderCardColor;

    if (status == CardStatus::REVEALED) {
        cardColor = COLOR_ORANGE_HOVER;
        borderCardColor = COLOR_ORANGE;
    } else {
        if (isHovered()) {
            cardColor = COLOR_PURPLE;
            borderCardColor = COLOR_ORANGE;
        } else {
            cardColor = COLOR_PURPLE;
            borderCardColor = COLOR_PURPLE_DARK;
        }
    }

    Rectangle shadowRectangle = {rectangle.x + 4, rectangle.y + 4, rectangle.width + 4, rectangle.height + 4};
    DrawRectangleRounded(shadowRectangle, 0.25f, 4, COLOR_BLACK);
    DrawRectangleRounded(rectangle, 0.25f, 4, cardColor);
    DrawRectangleRoundedLinesEx(rectangle, 0.25f, 4, 4.0f, borderCardColor);

    if (status == CardStatus::REVEALED) {
        int fontSize = rectangle.width * 0.4f;
        int textWidth = MeasureText(TextFormat("%d", id), fontSize);
        float textX = rectangle.x + (rectangle.width - textWidth) / 2.0f;
        float textY = rectangle.y + (rectangle.height - fontSize) / 2.0f;
        DrawTextEx(mainFont, TextFormat("%d", id), {textX, textY}, fontSize, 1.0f, COLOR_BLACK);
    }
}

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

    int idIndex = 0;
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

void updateGameplay(vector<Card> &deck, GameStats &stats, Sound cardClickSound, Sound cardMatchSound) {
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