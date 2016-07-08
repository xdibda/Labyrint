/**
  Autori: Lukas Dibdak, xdibda00 + David Porizek, xporiz03
  Modul: Players
  Popis modulu: Tento modul implementuje praci s hracem a
  seznamem hracu, kterym obsahuje vsechny hrace ve hre.
  */

#ifndef PLAYER
#define PLAYER

#include <vector>
#include <iostream>
#include "cardpack.h"

using namespace std;

/**
 * @brief Trida hrac.
 */
class Player{
private:
    int iScore;
    int x;
    int y;
    int playerColor;
    vector<Card *> pCards;
public:
    Player();
    Player(int id, int x, int y);
    Player(int id, int x, int y, vector<Card *> playerCards, int score);
    int getScore();
    int getXPos();
    int getYPos();
    int getColor();
    void SetScore(int score);
    void AddScore(int score);
    void appendCards(vector<Card *> cardList);
    Card* getPlayersCard();
    void getAnotherCard();
    void setPos(int x, int y);
    vector<Card *> getpCards();
    void setColor(int color);
    void giveCard(Card* card);
};

/**
 * @brief Trida seznam hracu.
 */
class PlayerList{
private:
    int number;
    int iPlayersTurn;
    vector<Player *> pList;
public:
    PlayerList();
    int NumberOfPlayers();
    int getNumber();
    void setNumber(int num);
    void clearPlayers();
    void CreatePlayer(int num, int xpos, int ypos, vector<Card *> *tempCards, int numberOfCards, int score);
    vector<Player *> getPList();
    void SetTurn(int pTurn);
    void NextTurn();
    Player* GetActivePlayer();
    vector<Player *> GetPlayers(int x, int y);
    int getPlayersTurn();
    void PreviusTurn();
};

#endif
