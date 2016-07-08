/**
 * Autori: David Porizek, xporiz03
 * Modul: controller
 * Popis modulu: Tento modul implementuje reseni tahu a akci souvisejicich s hraci plochou. Slouzi zaroven jako rozhrani pro akce, ktere on sam neimplementuje.
 */

#ifndef CONTROLLER
#define CONTrOLLER

#include <vector>
#include <sstream>

#include "map.h"
#include "cardpack.h"
#include "log.h"

/**
 * @brief Struktura obsahujici BOOL hodnoty pohybu, zda se tento pohyb da uskutecnit nebo ne
 */
struct sMove{
        bool UP = 0;
        bool LEFT = 0;
        bool DOWN = 0;
        bool RIGHT = 0;
};

/**
 * @brief Enum vypisujici mozne pohyby
 */
enum tMove{ UP = 1, LEFT, DOWN, RIGHT };

/**
 * @brief Trida resici tahy hracu a jejich provadene akce na hraci plose
 */
class Controller{
private:
        bool gameEnd = 0;
        Logger* cLog;
        bool bUndo = 0;
public:
        Controller();
        bool ValidMove(tMove move, Player* pToMove, Map* pMap);
        sMove GetValidMoves(char type, int rotation);
        bool MakeMove(tMove move, Player* pToMove, Map* pMap);
        void EndTurn(Map* pMap);
        bool GameEnded();
        void EndGame();
        Logger* getLog();
        bool shiftBoard(int x, int y, Map* pMap);
        bool undoTurn(Map* pMap);
        void setUndo(bool undo);
        bool getUndo();
        void rotateField(Map* pMap);
};


#endif
