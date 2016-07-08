/**
  Autori: Lukas Dibdak, xdibda00
  Modul: Map
  Popis modulu: Tento modul implementuje praci s mapu
  a jednotlivymi polickami mapy.
  */

#include "map.h"

#define FIELD_SIZE 38
#define FIELD_SPACE 4

#define CORNER_TOPLEFT 1
#define CORNER_TOPRIGHT 2
#define CORNER_BOTTOMLEFT 3
#define CORNER_BOTTOMRIGHT 4

#define EDGE_LEFT 1
#define EDGE_RIGHT 2
#define EDGE_TOP 3
#define EDGE_BOTTOM 4

/**
 * @brief konstruktor pro policko mapy.
 */
MapField::MapField() {
    size = 40;
}

/**
 * @brief sekundarni konstruktor pro policko mapy.
 * @param rotace policka mapy.
 */
MapField::MapField(int rot) {
    size = 40;
    xPos = 0;
    yPos = 0;
    rotation = rot;
}

/**
 * @brief rucni nastaveni rotace policka mapy.
 * @param velikost rotace.
 */
void MapField::setRotation(int rot) {
    rotation = rot;
}

/**
 * @brief automaticke nastaveni rohu.
 * @param typ rohu (levy/pravy horni/dolni - viz makro).
 */
void MapField::setCorner(int corner) {
    shape = 'L';
    switch (corner) {
    case 1:
        rotation = 3;
        break;
    case 2:
        rotation = 0;
        break;
    case 3:
        rotation = 2;
        break;
    case 4:
        rotation = 1;
        break;
    }
}

/**
 * @brief automaticke nastaveni okraje mapy.
 * @param typ okraje (levy/pravy horni/dolni - viz makro).
 */
void MapField::setBorder(int edge) {
    shape = 'T';
    switch (edge) {
    case 1:
        rotation = 3;
        break;
    case 2:
        rotation = 1;
        break;
    case 3:
        rotation = 0;
        break;
    case 4:
        rotation = 2;
        break;
    }
}

/**
 * @brief vraci typ tvaru policka.
 * @return T/I/L.
 */
char MapField::getShape() {
    return shape;
}

/**
 * @brief vraci velikost policka mapy.
 * @return velikost policka.
 */
int MapField::getSize() {
    return size;
}

/**
 * @brief vraci vodorovnou souradnici policka mapy.
 * @return souradnice.
 */
int MapField::getXPos() {
    return xPos;
}

/**
 * @brief vraci svislou souradnici policka mapy.
 * @return souradnice.
 */
int MapField::getYPos() {
    return yPos;
}

/**
 * @brief vraci rotaci na policku mapy.
 * @return rotace.
 */
int MapField::getRotation() {
    return rotation;
}

/**
 * @brief vraci poklad na danem policku mapy.
 * @return 1-12, 1-24 nebo 0.
 */
int MapField::getTreasure() {
    return treasure;
}

/**
 * @brief nastavi souradnice policka na mape.
 * @param vodorovna souradnice.
 * @param svisla souradnice.
 */
void MapField::setPosition(int x, int y) {
    xPos = x;
    yPos = y;
}

/**
 * @brief nastavi poklad pro policko mapy.
 * @param id pokladu.
 */
void MapField::setTreasure(int tr) {
    treasure = tr;
}

/**
 * @brief nastavi tvar policku mapy.
 * @param T/I/L.
 */
void MapField::setShape(int c) {
    switch (c) {
    case 1:
        shape = 'I';
        break;
    case 2:
        shape = 'L';
        break;
    case 3:
        shape = 'T';
        break;
    }
}

/**
 * @brief konstruktor mapy.
 */
Map::Map() {
    players = new PlayerList;
    cards = new CardPack;
    size = 7;
    lastX = -1;
    lastY = -1;
    wasShifted = 0;
}

/**
 * @brief vymaze mapu (vektor policek mapy).
 */
void Map::clearMap() {
    int sizeOfVector = fields.size();
    for (int i = 0; i < sizeOfVector; i++) {
        delete fields[i];
    }
    fields.clear();
}

/**
 * @brief posune mapu v radku nebo sloupci
 * @param vstupni vodorovna souradnice.
 * @param vstupni svisla souradnice.
 * @param undo param.
 * @return true(success) / false.
 */
bool Map::ShiftBoard(int x, int y, bool undo)
{
    MapField* tempFieldIn = fields[size * size];
    MapField* tempFieldOut;
    vector<Player *> tempPlayers;
    if (x == 0 || x == size - 1)
    {
        if (y % 2 == 1)
        {
            if (!undo)
            {
                if (!(lastX == size - x - 1 && lastY == y))
                {
                    lastX = x;
                    lastY = y;
                }
                else
                    return 0;
            }
            if (x == 0)
            {
                for (int i = 0; i < size; i++)
                {
                    if (players->GetPlayers(i, y) != tempPlayers)
                        tempPlayers = players->GetPlayers(i, y);
                    else
                        tempPlayers.clear();
                    for (unsigned int j = 0; j < tempPlayers.size(); j++)
                    {
                        if ((i + 1) == size)
                            tempPlayers[j]->setPos(0, y);
                        else
                            tempPlayers[j]->setPos(i + 1, y);
                    }
                    tempFieldOut = fields[i * size + y];
                    fields[i * size + y] = tempFieldIn;
                    GetField(i, y)->setPosition(i, y);
                    tempFieldIn = tempFieldOut;
                }
                fields[size * size] = tempFieldOut;
            }
            else
            {
                for (int i = size - 1; i >= 0; i--)
                {
                    if (players->GetPlayers(i, y) != tempPlayers)
                        tempPlayers = players->GetPlayers(i, y);
                    else
                        tempPlayers.clear();
                    for (unsigned int j = 0; j < tempPlayers.size(); j++)
                    {
                        if (i == 0)
                            tempPlayers[j]->setPos(size - 1, y);
                        else
                            tempPlayers[j]->setPos(i - 1, y);
                    }
                    tempFieldOut = fields[i * size + y];
                    fields[i * size + y] = tempFieldIn;
                    GetField(i, y)->setPosition(i, y);
                    tempFieldIn = tempFieldOut;
                }
                fields[size * size] = tempFieldOut;
            }
        }
        else
            return 0;
    }
    else if (y == 0 || y == size - 1)
        if (x % 2 == 1)
        {
            if (!undo)
            {
                if (!(lastX == x && lastY == size - y - 1))
                {
                    lastX = x;
                    lastY = y;
                }
                else
                    return 0;
            }
            if (y == 0)
            {
                for (int i = 0; i < size; i++)
                {
                    if (players->GetPlayers(x, i) != tempPlayers)
                        tempPlayers = players->GetPlayers(x, i);
                    else
                        tempPlayers.clear();
                    for (unsigned int j = 0; j < tempPlayers.size(); j++)
                    {
                        if ((i + 1) == size)
                            tempPlayers[j]->setPos(x, 0);
                        else
                            tempPlayers[j]->setPos(x, i + 1);
                    }
                    tempFieldOut = fields[x * size + i];
                    fields[x * size + i] = tempFieldIn;
                    GetField(x, i)->setPosition(x, i);
                    tempFieldIn = tempFieldOut;
                }
                fields[size * size] = tempFieldOut;
            }
            else
            {
                for (int i = size - 1; i >= 0; i--)
                {
                    if (players->GetPlayers(x, i) != tempPlayers)
                        tempPlayers = players->GetPlayers(x, i);
                    else
                        tempPlayers.clear();
                    for (unsigned int j = 0; j < tempPlayers.size(); j++)
                    {
                        if (i == 0)
                            tempPlayers[j]->setPos(x, size - 1);
                        else
                            tempPlayers[j]->setPos(x, i - 1);
                    }
                    tempFieldOut = fields[x * size + i];
                    fields[x * size + i] = tempFieldIn;
                    GetField(x, i)->setPosition(x, i);
                    tempFieldIn = tempFieldOut;
                }
                fields[size * size] = tempFieldOut;
            }
        }
        else
            return 0;
    else
        return 0;
    changeShiftedState(1);
    if (undo)
        changeShiftedState(0);
    return 1;
}

/**
 * @brief nastavi velikost setu karet
 * @param velikost balicku karet - 12 nebo 24.
 */
void Map::setCardsSet(int sc) {
    cards->setSize(sc);
}

/**
 * @brief vraci velikost balicku karet.
 * @return velikost.
 */
int Map::getCardsSet() {
    return cards->getSize();
}

/**
 * @brief nastavi pocet hracu.
 * @param pocet hracu - 2 nebo 3 nebo 4.
 */
void Map::setPlayers(int play) {
    players->setNumber(play);
}

/**
 * @brief vrati velikost mapy.
 * @return velikost.
 */
int Map::getSize() {
    return size;
}

/**
 * @brief pripravi balicek karet.
 */
void Map::prepareCards() {
    cards->clearCardPack();
    cards->AppendPack();
    cards->ShufflePack();
}

/**
 * @brief vrati pocet hracu.
 * @return pocet hracu.
 */
PlayerList* Map::getPlayers() {
    return players;
}

/**
 * @brief vrati vektor policek mapy.
 * @return vektor policek.
 */
vector<MapField *> Map::getFields() {
    return fields;
}

/**
 * @brief vrati vektor balicku karet.
 * @return vektor balicku.
 */
CardPack *Map::getCards() {
    return cards;
}

/**
 * @brief nastavi velikost mapy.
 * @param velikost mapy.
 */
void Map::setSize(int n) {
    size = n;
}

/**
 * @brief vrati policko na urcitem indexu na mape.
 * @param vodorovna souradnice.
 * @param svisla souradnice.
 * @return policko na mape.
 */
MapField* Map::GetField(int x, int y)
{
    return fields[x * size + y];
}

/**
 * @brief vytvori novou mapu na zaklade zadanych parametru hry.
 */
void Map::createMap() {
    vector<int> cardRandomNumbers;
    vector<int> figureRandomNumbers;
    vector<Card *> tempCards;
    int numberOfCards = cards->getSize();
    int randNumber;
    int currentItem = 0;
    bool isThere = false;
    srand(time(NULL));
    clearMap();
    prepareCards();
    tempCards = cards->getCardsVector();
    players->clearPlayers();
    for (int i = 0; i < players->getNumber(); i++) {
        while (1) {
            isThere = false;
            randNumber = (rand() % 4);
            for (auto i : figureRandomNumbers) {
                if (i == randNumber)
                    isThere = true;
            }
            if (!isThere)
                break;
        }
        figureRandomNumbers.push_back(randNumber);
        while (1) {
            isThere = false;
            randNumber = (rand() % 4);
            for (auto i : cardRandomNumbers) {
                if (i == randNumber)
                    isThere = true;
            }
            if (!isThere)
                break;
        }
        cardRandomNumbers.push_back(randNumber);

        switch (randNumber) {
        case 0:
            players->CreatePlayer(figureRandomNumbers.back(), 0, 0, &tempCards, numberOfCards, 0);
            break;
        case 1:
            players->CreatePlayer(figureRandomNumbers.back(), 0, size - 1, &tempCards, numberOfCards, 0);
            break;
        case 2:
            players->CreatePlayer(figureRandomNumbers.back(), size - 1, 0, &tempCards, numberOfCards, 0);
            break;
        case 3:
            players->CreatePlayer(figureRandomNumbers.back(), size - 1, size - 1, &tempCards, numberOfCards, 0);
            break;
        }
    }
    randNumber = 0;
    players->SetTurn(0);
    isThere = false;
    cardRandomNumbers.clear();
    for (int i = 0; i < cards->getSize(); i++) {
        while (1) {
            isThere = false;
            randNumber = rand() % ((size * size) - 1);
            if (randNumber == 0 || randNumber == (size - 1) || randNumber == ((size * size) - size))
                continue;
            for (auto i : cardRandomNumbers) {
                if (i == randNumber)
                    isThere = true;
            }
            if (!isThere)
                break;
        }
        cardRandomNumbers.push_back(randNumber);
    }
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fields.push_back(new MapField);
            isThere = false;
            for (auto stoneByStone : cardRandomNumbers) {
                if (i * size + j == stoneByStone) {
                    isThere = true;
                }
            }
            if (isThere){
                fields.back()->setTreasure(++currentItem);
            }
            else
                fields.back()->setTreasure(0);
            if (i == 0) {
                if (j == 0)
                    fields.back()->setCorner(CORNER_TOPLEFT);
                else if (j == size - 1)
                    fields.back()->setCorner(CORNER_BOTTOMLEFT);
                else if (!(j % 2))
                    fields.back()->setBorder(EDGE_LEFT);
                else {
                    fields.back()->setShape(rand() % 3 + 1);
                    fields.back()->setRotation(rand() % 4);
                }
            }
            else if (i == size - 1) {
                if (j == 0)
                    fields.back()->setCorner(CORNER_TOPRIGHT);
                else if (j == size - 1)
                    fields.back()->setCorner(CORNER_BOTTOMRIGHT);
                else if (!(j % 2))
                    fields.back()->setBorder(EDGE_RIGHT);
                else {
                    fields.back()->setShape(rand() % 3 + 1);
                    fields.back()->setRotation(rand() % 4);
                }
            }
            else if (!(i % 2) && !(j % 2) && i != 0 && j != 0 && i != size - 1 && j != size - 1) {
                fields.back()->setShape(3);
                fields.back()->setRotation(rand() % 4);
            }
            else {
                if (j == 0 && !(i % 2))
                    fields.back()->setBorder(EDGE_TOP);
                else if (j == size - 1 && !(i % 2))
                    fields.back()->setBorder(EDGE_BOTTOM);
                else {
                    fields.back()->setShape(rand() % 3 + 1);
                    fields.back()->setRotation(rand() % 4);
                }
            }
            fields.back()->setPosition(i, j);
        }
    }

    fields.push_back(new MapField);
    fields.back()->setShape(rand() % 3 + 1);
    fields.back()->setRotation(rand() % 4);
    fields.back()->setTreasure(0);
}

/**
 * @brief prepne hru na dalsiho hrace.
 */
void Map::NextTurn()
{
    players->NextTurn();
}

/**
 * @brief zjisti, zda byla v danem tahu jiz mapa posunuta.
 * @return true/false.
 */
bool Map::getShiftedState()
{
    return wasShifted;
}

/**
 * @brief nastavi flag posunuti mapy.
 * @param true/false.
 */
void Map::changeShiftedState(bool END)
{
    wasShifted = END;
}

/**
 * @brief vrati aktivniho hrace
 * @return aktivni hrac.
 */
Player* Map::GetActivePlayer()
{
    return players->GetActivePlayer();
}

void MapField::fieldCounterRotate()
{
    rotation -= 1;
    if (rotation < 0)
    {
        rotation = 3;
    }
}

/**
 * orotuje policko mapy o 90 stupnu.
 */
void MapField::fieldRotate()
{
    rotation += 1;
    if (rotation > 3)
    {
        rotation = 0;
    }
}

/**
 * @brief vrati posledni posunuti mapy ve svisle ose.
 * @return souradnice posunuti.
 */
signed int Map::getLastY()
{
    return lastY;
}

/**
 * @brief vrati posledni posunuti mapy ve vodorovne ose.
 * @return souradnice posunuti.
 */
signed int Map::getLastX()
{
    return lastX;
}

/**
 * @brief nastavi posledni posunuti mapy ve svisle ose.
 * @param svisly parametr.
 */
void Map::setLastY(int y)
{
    lastY = y;
}

/**
 * @brief nastavi posledni posunuti mapy ve vodorovne ose.
 * @param vodorovny parametr.
 */
void Map::setLastX(int x)
{
    lastX = x;
}

/**
 * @brief vrati, zda je mapa prazdna.
 * @return true/false.
 */
bool Map::isEmpty()
{
    if (fields.size() == 0)
        return 1;
    else
        return 0;
}

/**
 * @brief nastavi velikost policka.
 * @param velikost policka.
 */
void MapField::setSize(int fSize)
{
    size = fSize;
}

/**
 * @brief prida policka na konec vektoru mapy.
 * @param policko mapy.
 */
void Map::addField(MapField* tField)
{
    fields.push_back(tField);
}

/**
 * @brief nastavi tah hrace.
 * @param ktery hrac ma hrat.
 */
void Map::setTurn(int turn)
{
    players->SetTurn(turn);
}

/**
 * @brief vrati tah hrace na predchazejiciho hrace.
 */
void Map::PreviusTurn()
{
    players->PreviusTurn();
}
