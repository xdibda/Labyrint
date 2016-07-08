/**
 * Autori: David Porizek, xporiz03
 * Modul: log
 * Popis modulu: Tento modul implementuje ukladani a nacitani her.
 */

#ifndef LOG
#define LOG

#include <iostream>

#include "map.h"
#include "cardpack.h"
#include "players.h"

/**
 * @brief Trida resici ukladani a nacitani her a take stack tahu, ktere byly provedeny
 */
class Logger{
private:
        int iTurnNumber = 0;
        vector<string>moves;
public:
        int getTurnNumber();
        void incTurnNumber();
        void decTurnNumber();
        void setTurnNumber(int turn);
        void saveGame(Map pMap);
        void loadGame(Map* sMap);
        void saveTurn(string turn);
        string getTurn();
        void clearMoves();
};

#endif
