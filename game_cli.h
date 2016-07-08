/**
 * Autori: David Porizek, xporiz03
 * Modul: gameCLI
 * Popis modulu: Tento modul implementuje GUI CLI, resi vykresleni hraci plochy a dalsich potrebnych casti hry.
 */

#ifndef GAMECLI
#define GAMECLI

#include "map.h"
#include "players.h"
#include "controller.h"

/**
 * @brief Funkce resici sort hracu podle jejich barev
 * @param prvni hrac.
 * @param druhy hrac.
 * @return TRUE pokud je barva prvniho vetsi nez druheho, naopak FALSE.
 */
bool cmd(Player * p1, Player * p2);

/**
 * @brief Trida resici generovani GUI na CLI vystup.
 */
class gameCLI{
public:
        void ClearScreen();
        void DrawBoard(Map* pMap);
        void PrintField(char shape, int rotation, int line, int treasure);
};

#endif
