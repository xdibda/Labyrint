/**
  Autori: Lukas Dibdak, xdibda00 + David Porizek, xporiz03
  Modul: Players
  Popis modulu: Tento modul implementuje praci s hracem a
  seznamem hracu, kterym obsahuje vsechny hrace ve hre.
  */

#include "players.h"

/**
 * @brief vytvori hrace a prida ho do seznamu hracu.
 * @param barva figurky hrace.
 * @param vodorovna souradnice pozice hrace.
 * @param svisla souradnice pozice hrace.
 * @param karty hrace.
 * @param pocet karet ve hre.
 * @param skore hrace.
 */
void PlayerList::CreatePlayer(int col, int x, int y, vector<Card *> *tempCards, int numberOfCards, int score) {
    vector<Card *> playerCards;
    for (int i = 0; i < ((numberOfCards / number) - score); i++) {
        playerCards.push_back((*tempCards).back());
        (*tempCards).pop_back();
    }
    pList.push_back(new Player(col, x, y, playerCards, score));
}

/**
 * @brief vrati vektor vsech hracu ve hre.
 * @return vektor hracu.
 */
vector<Player *> PlayerList::getPList() {
    return pList;
}

/**
 * @brief odstrani vsechny hrace ve hre.
 */
void PlayerList::clearPlayers() {
    for (int i = 0; i < NumberOfPlayers(); i++) {
        delete pList[i];
    }
    pList.clear();
}

/**
 * @brief vrati pocet hracu ve hre.
 * @return pocet hracu.
 */
int PlayerList::NumberOfPlayers() {
    return pList.size();
}

/**
 * @brief nastavi tah hrace.
 * @param ktery hrac ma hrat.
 */
void PlayerList::SetTurn(int pTurn)
{
    iPlayersTurn = pTurn;
}

/**
 * @brief nastavi tah na dalsiho hrace.
 */
void PlayerList::NextTurn()
{
    iPlayersTurn += 1;
    if (iPlayersTurn >= number)
    {
        iPlayersTurn = 0;
    }
}

/**
 * @brief konstruktor pro seznam hracu.
 */
PlayerList::PlayerList() {
    iPlayersTurn = 0;
}

/**
 * @brief nastavi pocet hracu ve hre.
 * @param pocet hracu.
 */
void PlayerList::setNumber(int num) {
    number = num;
}

/**
 * @brief vrati pocet hracu ve hre.
 * @return pocet hracu.
 */
int PlayerList::getNumber() {
    return number;
}

/**
 * @brief konstruktor pro hrace.
 */
Player::Player() {
    iScore = 0;
    x = 0;
    y = 0;
    playerColor = 0;
}

/**
 * @brief sekundarni konstruktor pro hrace.
 * @param barva figurky.
 * @param vodorovna pozice hrace.
 * @param svisla pozice hrace.
 */
Player::Player(int color, int xpos, int ypos) {
    iScore = 0;
    x = xpos;
    y = ypos;
    playerColor = color;
}

/**
 * @brief ternalni konstruktor pro hrace.
 * @param barva figurky hrace.
 * @param vodorovna pozice hrace.
 * @param svisla pozice hrace.
 * @param vektor balicku karet hrace.
 * @param skore hrace.
 */
Player::Player(int color, int xpos, int ypos, vector<Card *> playerCards, int score) {
    iScore = score;
    x = xpos;
    y = ypos;
    playerColor = color;
    pCards = playerCards;
}

/**
 * @brief nastavi skore hrace.
 * @param skore hrace.
 */
void Player::SetScore(int score) {
    iScore = score;
}

/**
 * @brief prida skore k hraci.
 * @param skore hrace.
 */
void Player::AddScore(int score) {
    iScore += score;
}

/**
 * @brief vrati vodorovnou pozici hrace na mape.
 * @return vodorovna pozice.
 */
int Player::getXPos() {
    return x;
}

/**
 * @brief vrati svislou pozici hrace na mape.
 * @return svisla pozice.
 */
int Player::getYPos() {
    return y;
}

/**
 * @brief nastavi pozici hrace na mape.
 * @param vodorovna pozice.
 * @param svisla pozice.
 */
void Player::setPos(int newX, int newY)
{
    x = newX;
    y = newY;
}

/**
 * @brief prida hraci jeho vlastni balicek karet, ktere ma hledat.
 * @param vektor karet.
 */
void Player::appendCards(vector<Card *> cardList)
{
    pCards = cardList;
}

/**
 * @brief prepne na dalsi hracovu kartu
 */
void Player::getAnotherCard()
{
    if (pCards.size() > 0)
        pCards.pop_back();
}

/**
 * @brief vrati hracovu aktivni kartu.
 * @return hracova karta.
 */
Card* Player::getPlayersCard() {
    return pCards[pCards.size() - 1];
}

/**
 * @brief prida hracovi kartu.
 * @param karta.
 */
void Player::giveCard(Card* card)
{
    pCards.push_back(card);
}

/**
 * @brief vrati hrace na urcite pozici na mape.
 * @param vodorovna pozice policka na mape.
 * @param svisla pozice policka na mape.
 * @return vektor hracu, kteri se nachazeji na danem policku.
 */
vector<Player *> PlayerList::GetPlayers(int x, int y)
{
    vector<Player *>temp;
    for (int i = 0; i < number; i++)
    {
        if (pList[i]->getXPos() == x && pList[i]->getYPos() == y)
        {
            temp.push_back(pList[i]);
        }
    }
    return temp;
}

/**
 * @brief vrati hrace, ktery je prave na tahu.
 * @return hrac.
 */
Player* PlayerList::GetActivePlayer()
{
    return pList[iPlayersTurn];
}

/**
 * @brief vrati barvu figurky hrace.
 * @return barva figurky.
 */
int Player::getColor() {
    return playerColor;
}

/**
 * @brief vrati aktualni skore hrace.
 * @return skore.
 */
int Player::getScore()
{
    return iScore;
}

/**
 * @brief vrati tah hrace.
 * @return tah.
 */
int PlayerList::getPlayersTurn()
{
    return iPlayersTurn;
}

/**
 * @brief hraci vektor karet hrace.
 * @return vektor karet.
 */
vector<Card *> Player::getpCards()
{
    return pCards;
}

/**
 * @brief nastavi barvu figurky hrace.
 * @param barva figurky.
 */
void Player::setColor(int color)
{
    playerColor = color;
}

/**
 * @brief vrati tah hrace na predchazejiciho hrace.
 */
void PlayerList::PreviusTurn()
{
    iPlayersTurn -= 1;
    if (iPlayersTurn < 0)
    {
        iPlayersTurn = number - 1;
    }
}
