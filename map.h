/**
  * Autori: Lukas Dibdak, xdibda00 + David Porizek, xporiz03
  * Modul: Map
  * Popis modulu: Tento modul implementuje praci s mapu
  * a jednotlivymi polickami mapy.
  */

#ifndef MAP
#define MAP

#include <vector>
#include <cstdlib>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "players.h"

using namespace std;

/**
 * @brief Trida popisujici policko mapy.
 */
class MapField {
protected:
    int size;
    int treasure;
    int xPos;
    int yPos;
    int rotation;
    int shape;
public:
    MapField();
    MapField(int rot);
    int getSize();
    int getXPos();
    int getYPos();
    int getRotation();
    int getTreasure();
    char getShape();
    void setBorder(int edge);
    void setCorner(int corner);
    void setShape(int shape);
    void setRotation(int rot);
    void setPosition(int x, int y);
    void setTreasure(int tr);
    void fieldCounterRotate();
    void fieldRotate();
    void setSize(int fSize);
};

/**
 * @brief Trida popisujici mapu.
 */
class Map {
protected:
    int size;
    CardPack* cards;
    PlayerList* players;
    vector<MapField *> fields;
    signed int lastX;
    signed int lastY;
    bool wasShifted;
public:
    Map();
    int getSize();
    int getCardsSet();
    signed int getLastX();
    signed int getLastY();
    void clearMap();
    void createMap();
    void setSize(int n);
    void setCardsSet(int sc);
    void setPlayers(int play);
    void prepareCards();
    void addField(MapField* tField);
    void addPlayer(Player* tPlayer);
    void setTurn(int turn);
    void PreviusTurn();
    void setLastX(int x);
    void setLastY(int y);
    void changeShiftedState(bool END);
    void NextTurn();
    bool isEmpty();
    bool getShiftedState();
    bool ShiftBoard(int x, int y, bool undo);
    PlayerList* getPlayers();
    CardPack* getCards();
    vector<MapField *> getFields();
    MapField* GetField(int x, int y);
    Player* GetActivePlayer();
};

#endif
