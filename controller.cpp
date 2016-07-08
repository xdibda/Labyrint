/**
 * Autori: David Porizek, xporiz03
 * Modul: controller
 * Popis modulu: Tento modul implementuje reseni tahu a akci souvisejicich s hraci plochou. Slouzi zaroven jako rozhrani pro akce, ktere on sam neimplementuje.
 */

#include "controller.h"

/**
 * @brief Funkce resi na ktere strany se da z policka jit
 * @param Tvar kamene
 * @param Rotace kamene
 * @return Struktura boolu, ktera udava na ktere strany je mozne z policka jit pomoci TRUE
 */
sMove Controller::GetValidMoves(char type, int rotation)
{
        sMove validMove;
        validMove.UP = 0;
        validMove.LEFT = 0;
        validMove.DOWN = 0;
        validMove.RIGHT = 0;
        switch (type){
        case 'L':
                switch (rotation)
                {
                case 0:
                        validMove.LEFT = 1;
                        validMove.DOWN = 1;
                        break;
                case 1:
                        validMove.LEFT = 1;
                        validMove.UP = 1;
                        break;
                case 2:
                        validMove.UP = 1;
                        validMove.RIGHT = 1;
                        break;
                case 3:
                        validMove.RIGHT = 1;
                        validMove.DOWN = 1;
                        break;
                }
                break;
        case 'T':
                switch (rotation)
                {
                case 0:
                        validMove.LEFT = 1;
                        validMove.DOWN = 1;
                        validMove.RIGHT = 1;
                        break;
                case 1:
                        validMove.LEFT = 1;
                        validMove.UP = 1;
                        validMove.DOWN = 1;
                        break;
                case 2:
                        validMove.UP = 1;
                        validMove.RIGHT = 1;
                        validMove.LEFT = 1;
                        break;
                case 3:
                        validMove.UP = 1;
                        validMove.RIGHT = 1;
                        validMove.DOWN = 1;
                        break;
                }
                break;
        case 'I':
                switch (rotation)
                {
                case 0:
                        validMove.LEFT = 1;
                        validMove.RIGHT = 1;
                        break;
                case 1:
                        validMove.DOWN = 1;
                        validMove.UP = 1;
                        break;
                case 2:
                        validMove.LEFT = 1;
                        validMove.RIGHT = 1;
                        break;
                case 3:
                        validMove.DOWN = 1;
                        validMove.UP = 1;
                        break;
                }
                break;
        default:
                fprintf(stderr, "CLI ERROR: Unrecognized stone shape.\n");
                break;
        }
        return validMove;
}

/**
 * @brief Funkce resi zda je pozadovany tah validni a je mozne ho provest
 * @param Pohyb ktery hrac chce vykonat
 * @param Hrac ktery chce tahnout
 * @param Hraci plocha na ktere se ma tah uskutecnit
 * @return TRUE pokud se tento tah da uskutecnit
 */
bool Controller::ValidMove(tMove move, Player* pToMove, Map* pMap)
{
        MapField* field;
        sMove validMoveFrom;
        sMove validMoveTo;
        char c;
        int rot;
        int x = pToMove->getXPos();
        int y = pToMove->getYPos();
        field = pMap->GetField(x, y);
        int size = pMap->getSize();
        c = field->getShape();
        rot = field->getRotation();
        validMoveFrom = GetValidMoves(c, rot);
        if (!((y == 0 && move == UP) || (x == 0 && move == LEFT) || (x == size - 1 && move == RIGHT) || (y == size - 1 && move == DOWN)))
        {
                switch (move)
                {
                case UP:
                        field = pMap->GetField(x, y - 1);
                        break;
                case LEFT:
                        field = pMap->GetField(x - 1, y);
                        break;
                case DOWN:
                        field = pMap->GetField(x, y + 1);
                        break;
                case RIGHT:
                        field = pMap->GetField(x + 1, y);
                        break;
                }
                c = field->getShape();
                rot = field->getRotation();
                validMoveTo = GetValidMoves(c, rot);
                switch (move)
                {
                case UP:
                        if (validMoveFrom.UP & validMoveTo.DOWN)
                                return 1;
                        break;
                case LEFT:
                        if (validMoveFrom.LEFT & validMoveTo.RIGHT)
                                return 1;
                        break;
                case DOWN:
                        if (validMoveFrom.DOWN & validMoveTo.UP)
                                return 1;
                        break;
                case RIGHT:
                        if (validMoveFrom.RIGHT & validMoveTo.LEFT)
                                return 1;
                        break;
                }
        }
        return 0;
}

/**
 * @brief Funkce resi cely tah hrace a reaguje na nemoznost tah provest
 * @param Pohyb ktery hrac chce vykonat
 * @param Hrac ktery chce tahnout
 * @param Hraci plocha na ktere se ma tah uskutecnit
 * @return TRUE pokud se tento tah dal provest a byl proveden
 */
bool Controller::MakeMove(tMove move, Player* pToMove, Map* pMap)
{
        int x, y;
        MapField* newField;
        stringstream ss;
        string temp;
        //< Pokud se hrac dostal mimo tah na policko na kterem je poklad ktery sbira
        if (bUndo == 0 && (pToMove->getPlayersCard())->getTreasure() == pMap->GetField(pToMove->getXPos(), pToMove->getYPos())->getTreasure())
        {
                ss << pMap->GetField(pToMove->getXPos(), pToMove->getYPos())->getTreasure();
                ss >> temp;
                temp = "PICK:" + temp;
                if (!getUndo())
                        cLog->saveTurn(temp);
                ss.clear();
                temp.clear();
                pToMove->AddScore(1);
                pMap->GetField(pToMove->getXPos(), pToMove->getYPos())->setTreasure(0);
                pToMove->getAnotherCard();
                if (pToMove->getScore() == (pMap->getCardsSet() / (pMap->getPlayers())->getNumber()))
                {
                        EndGame();
                        return 1;
                }
                setUndo(1);
                EndTurn(pMap);
                return 1;
        }
        //< Pokud na policku na kterem prave je neni poklad ktery sbira
        if (ValidMove(move, pToMove, pMap))
        {
                ss << move;
                ss >> temp;
                temp = "MOVE:" + temp;
                if (!getUndo())
                {
                        cLog->incTurnNumber();
                        cLog->saveTurn(temp);
                }
                ss.clear();
                temp.clear();
                x = pToMove->getXPos();
                y = pToMove->getYPos();
                switch (move)
                {
                case UP:
                        pToMove->setPos(x, y - 1);
                        y -= 1;
                        break;
                case LEFT:
                        pToMove->setPos(x - 1, y);
                        x -= 1;
                        break;
                case DOWN:
                        pToMove->setPos(x, y + 1);
                        y += 1;
                        break;
                case RIGHT:
                        pToMove->setPos(x + 1, y);
                        x += 1;
                        break;
                }
                newField = pMap->GetField(x, y);
                if ((pToMove->getPlayersCard())->getTreasure() == newField->getTreasure())
                {
                        ss << newField->getTreasure();
                        ss >> temp;
                        temp = "PICK:" + temp;
                        if (!getUndo())
                                cLog->saveTurn(temp);
                        ss.clear();
                        temp.clear();
                        pToMove->AddScore(1);
                        newField->setTreasure(0);
                        pToMove->getAnotherCard();
                        if (pToMove->getScore() == (pMap->getCardsSet() / (pMap->getPlayers())->getNumber()))
                        {
                                EndGame();
                                return 1;
                        }
                        setUndo(1);
                        EndTurn(pMap);
                }
                return 1;
        }
        else
                return 0;
        return 0;
}

/**
 * @brief Funkce resi ukonceni tahu hrace
 * @param Hraci plocha na ktere hrac hraje
 */
void Controller::EndTurn(Map* pMap)
{
        if (!getUndo())
                cLog->saveTurn("END");
        else
                setUndo(0);
        cLog->incTurnNumber();
        pMap->changeShiftedState(0);
        pMap->NextTurn();
}

/**
 * @brief Funkce resi ukonceni hry v dusledku naplneni pozadovaneho poctu bodu
 */
void Controller::EndGame()
{
        gameEnd = 1;
}

/**
 * @brief Funkce pro zjednoduseni kontroly konce hry
 * @return TRUE pokud je hra u konce a mela by se zobrazit konecna obrazovka
 */
bool Controller::GameEnded()
{
        if (gameEnd)
                return 1;
        return 0;
}

/**
 * @brief Konstruktor Controlleru inicializuje Logger pro ukladani tahu
 */
Controller::Controller()
{
        cLog = new Logger;
}

/**
 * @brief Funkce vraci ukazatel na logger, aby se mohlo pracovat s jeho vektorem tahu
 */
Logger* Controller::getLog()
{
        return cLog;
}

/**
 * @brief Funkce resici vsunuti volneho pole do hraci plochy
 * @param X souradnice pro vlozeni kamene
 * @param Y souradnice pro vlozeni kamene
 * @param Hraci plocha na ktere se ma vlozeni provest
 * @return TRUE pokud se vlozeni dalo provest
 */
bool Controller::shiftBoard(int x, int y, Map* pMap)
{
        if (pMap->ShiftBoard(x, y, bUndo))
        {
                stringstream ss;
                ss << x << ":" << y << ":" << pMap->getLastX() << ":" << pMap->getLastY();
                string temp;
                ss >> temp;
                temp = "SHIFT:" + temp;
                if (!getUndo())
                {
                        cLog->incTurnNumber();
                        cLog->saveTurn(temp);
                }
                ss.clear();
                temp.clear();
                return 1;
        }
        else
                return 0;
}

/**
 * @brief Funkce resici navraceni tahu zpet
 * @param Hraci plocha na ktere se ma vraceni tahu provest
 * @return TRUE pokud jde tah jeste vracet, FALSE pokud jsme na zacatku hry
 */
bool Controller::undoTurn(Map* pMap)
{
        if (cLog->getTurnNumber() == 0)
                return 0;
        setUndo(1);
        cLog->decTurnNumber();
        Player* pToPlay = pMap->GetActivePlayer();
        string command;
        string sTemp;
        Card* tempCard;
        MapField* tempField;
        int iTemp;
        command = cLog->getTurn();
        if (command.find("MOVE") != string::npos)
        {
                //Case UP
                if (command.find("1") != string::npos)
                {
                        MakeMove(DOWN, pToPlay, pMap);
                }
                //Case LEFT
                else if (command.find("2") != string::npos)
                {
                        MakeMove(RIGHT, pToPlay, pMap);
                }
                //Case DOWN
                else if (command.find("3") != string::npos)
                {
                        MakeMove(UP, pToPlay, pMap);
                }
                //Case RIGHT
                else if (command.find("4") != string::npos)
                {
                        MakeMove(LEFT, pToPlay, pMap);
                }
        }
        if (command.find("SHIFT") != string::npos)
        {
                int x, y, lx, ly;
                int max = pMap->getSize() - 1;
                command = command.substr(command.find(":") + 1, string::npos);
                //X
                sTemp = command.substr(0, command.find(":"));
                command = command.substr(command.find(":") + 1, string::npos);
                x = stoi(sTemp);
                //Y
                sTemp = command.substr(0, command.find(":"));
                command = command.substr(command.find(":") + 1, string::npos);
                y = stoi(sTemp);
                //LASTX
                sTemp = command.substr(0, command.find(":"));
                command = command.substr(command.find(":") + 1, string::npos);
                lx = stoi(sTemp);
                //LASTY
                sTemp = command.substr(0, command.find(":"));
                command = command.substr(command.find(":") + 1, string::npos);
                ly = stoi(sTemp);
                pMap->setLastX(lx);
                pMap->setLastY(ly);
                if (x == 0 || x == max)
                {
                        shiftBoard(max - x, y, pMap);
                }
                else if (y == 0 || y == max)
                {
                        shiftBoard(x, max - y, pMap);
                }
        }
        if (command.find("END") != string::npos)
        {
                pMap->changeShiftedState(1);
                pMap->PreviusTurn();
        }
        if (command.find("PICK") != string::npos)
        {
                //Return turn
                pMap->changeShiftedState(1);
                pMap->PreviusTurn();
                pToPlay = pMap->GetActivePlayer();
                //Prepare
                command = command.substr(command.find(":") + 1, string::npos);
                iTemp = stoi(command);
                //Treasure type
                tempCard = new Card(iTemp);
                pToPlay->giveCard(tempCard);
                tempField = pMap->GetField(pToPlay->getXPos(), pToPlay->getYPos());
                tempField->setTreasure(iTemp);
                pToPlay->SetScore(pToPlay->getScore() - 1);
                undoTurn(pMap);
        }
        if (command.find("ROT") != string::npos)
        {
                pMap->GetField(pMap->getSize(), 0)->fieldCounterRotate();
        }
        setUndo(0);
        return 1;
}

/**
 * @brief Funkce meni stav undo flagy
 * @param Nastavi undo 1/0
 */
void Controller::setUndo(bool undo)
{
        bUndo = undo;
}

/**
 * @brief Funkce vraci stav undo flagy
 * @return Stav undo flagy 1/0
 */
bool Controller::getUndo()
{
        return bUndo;
}

/**
 * @brief Funkce spravujici rotaci volneho kamenu
 * @param Mapa na ktere chceme kamen orotovat
 */
void Controller::rotateField(Map* pMap)
{
        pMap->GetField(pMap->getSize(), 0)->fieldRotate();
        cLog->saveTurn("ROT");
        cLog->incTurnNumber();
}
