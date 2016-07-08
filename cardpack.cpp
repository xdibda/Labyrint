/**
  Autori: Lukas Dibdak, xdibda00 + David Porizek, xporiz03
  Modul: CardPack
  Popis modulu: Tento modul implementuje praci s kartami a balickem karet.
  */

#include "cardpack.h"

/**
 * @brief konstruktor karty.
 */
Card::Card() {
    treasure = 0;
}

/**
 * @brief sekundarni konstruktor karty.
 * @param typ karty.
 */
Card::Card(int type) {
    treasure = type;
}

/**
 * @brief vraci poklad na karte.
 * @return 1-12 nebo 1-24 pokud na karte poklad je, pokud neni vraci 0.
 */
int Card::getTreasure() {
    return treasure;
}

/**
 * @brief prida do prazdneho balicku sadu karet.
 */
void CardPack::AppendPack() {
    for (int i = 1; i < size + 1; i++) {
        Cards.push_back(new Card(i));
    }
}

/**
 * @brief nastavi velikost balicku karet.
 * @param velikost balicku karet.
 */
void CardPack::setSize(int s) {
    size = s;
}

/**
 * @brief vycisti balicek karet tak, aby byl prazdny.
 */
void CardPack::clearCardPack() {
    int CardPackSize = Cards.size();
    for (int i = 0; i < CardPackSize; i++) {
        delete Cards.back();
    }
    Cards.clear();
}

/**
 * @brief vrati velikost balicku karet.
 * @return velikost balicku karet - 12 nebo 24.
 */
int CardPack::getSize() {
    return size;
}

/**
 * @brief vrati kartu na vrcholu balicku.
 * @return karta.
 */
Card CardPack::GetCard() {
    Card temp = *Cards.back();
    Cards.pop_back();
    return temp;
}

/**
 * @brief vraci vektor karet.
 * @return vektor karet.
 */
vector<Card *> CardPack::getCardsVector() {
    return Cards;
}

/**
 * @brief zamicha balicek karet.
 */
void CardPack::ShufflePack() {
    random_shuffle(Cards.begin(), Cards.end());
}
