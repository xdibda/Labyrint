/**
  * Autori: Lukas Dibdak, xdibda00 + David Porizek, xporiz03
  * Modul: CardPack
  * Popis modulu: Tento modul implementuje praci s kartami a balickem karet.
  */

#ifndef CARDPACK
#define CARDPACK

#include <vector>
#include <algorithm>

using namespace std;

/**
 * @brief trida popisujici jednotlivou kartu.
 */
class Card{
private:
    int treasure;
public:
    Card();
    Card(int type);
    int getTreasure();
};

/**
 * @brief trida popisujici cely balicek karet.
 */
class CardPack{
private:
    int size;
    vector<Card *>Cards;
public:
    int getSize();
    void ShufflePack();
    void AppendPack();
    void setSize(int s);
    void clearCardPack();
    Card GetCard();
    vector<Card *> getCardsVector();
};

#endif

