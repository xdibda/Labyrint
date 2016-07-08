/**
 * Autori: David Porizek, xporiz03
 * Modul: gameCLI
 * Popis modulu: Tento modul implementuje GUI CLI, resi vykresleni hraci plochy a dalsich potrebnych casti hry.
 */

#include <iostream>

#include "game_cli.h"
#include <stdlib.h>
#include <cstdlib>

/**
 * @brief Funkce resici cisteni obrazovky jak pro linux tak pro windows
 */
void gameCLI::ClearScreen()
{
#ifdef _WIN32
        std::system("CLS");
#else
        std::system("clear");
#endif
}

/**
 * @brief Funkce resici sort hracu podle jejich barev
 * @param Prvni hrac na porovnani.
 * @param Druhy hrac na porovnani.
 * @return TRUE pokud je barva prvniho vetsi nez druheho, naopak FALSE.
 */
bool cmd(Player * p1, Player * p2)
{
        return p1->getColor() < p2->getColor();
}

/**
 * @brief Funkce resici umisteni jednotlivych kamenu na hraci desce
 * @param Deska do ktere bude vykreslovat.
 */
void gameCLI::DrawBoard(Map* pMap)
{
        MapField* field;
        PlayerList* pList = pMap->getPlayers();
        vector<Player*> players;
        cout << "Hra Labyrint.\n" << "Projekt do ICP.\n" << "Autori: Lukas Dibdak, David Porizek\n\n\n";
        cout << "Plocha:\n";
        cout << " ";
        for (int x = 0; x < pMap->getSize(); x++)
        {
                cout << "    " << x + 1;
        }
        cout << "\n";
        for (int y = 0; y < pMap->getSize(); y++)
        {
                for (int i = 0; i < 4; i++)
                {
                        if (i == 1)
                        {
                                if (y + 1 < 10)
                                        cout << " ";
                                cout << y + 1 << " ";
                        }
                        else
                        {
                                cout << "   ";
                        }
                        for (int x = 0; x < pMap->getSize(); x++)
                        {
                                if (i == 3)
                                {
                                        players.clear();
                                        vector<Player*> players = pList->GetPlayers(x, y);

                                        sort(players.begin(), players.end(), cmd);

                                        int counter = 0;
                                        for (unsigned int j = 0; j < players.size(); j++)
                                        {
                                                if (players[0]->getColor() == 0 && j == 0)
                                                {
                                                        cout << "";
                                                        cout << players[0]->getColor();
                                                        counter = 1;
                                                }
                                                else if (players[0]->getColor() == 1 && j == 0)
                                                {
                                                        cout << " ";
                                                        cout << players[0]->getColor();
                                                        counter = 2;
                                                }
                                                else if (players[0]->getColor() == 2 && j == 0)
                                                {
                                                        cout << "  ";
                                                        cout << players[0]->getColor();
                                                        counter = 3;
                                                }
                                                else if (players[0]->getColor() == 3 && j == 0)
                                                {
                                                        cout << "   ";
                                                        cout << players[0]->getColor();
                                                        counter = 4;
                                                }
                                                else if (counter == players[j]->getColor())
                                                {
                                                        cout << players[j]->getColor();
                                                        counter++;
                                                }
                                                else
                                                {
                                                        cout << " ";
                                                        counter++;
                                                        j--;
                                                }
                                                if (players[players.size() - 1]->getColor() != 3 && j == players.size() - 1)
                                                {
                                                        if (players[players.size() - 1]->getColor() == 2)
                                                                cout << " ";
                                                        else if (players[players.size() - 1]->getColor() == 1)
                                                                cout << "  ";
                                                        else if (players[players.size() - 1]->getColor() == 0)
                                                                cout << "   ";
                                                }
                                        }
                                        if (0 == players.size())
                                        {
                                                cout << "    ";
                                        }
                                }
                                else
                                {
                                        field = pMap->GetField(x, y);
                                        PrintField(field->getShape(), field->getRotation(), i, field->getTreasure());
                                        cout << " ";
                                }
                                cout << " ";

                        }
                        cout << "\n";
                }
                cout << "\n";
        }
        //Vykresleni kamene navic
        cout << "Kamen k vlozeni: ";
        field = pMap->GetField(pMap->getSize(), 0);
        for (int i = 0; i < 3; i++)
        {
                PrintField(field->getShape(), field->getRotation(), i, field->getTreasure());
                if (i == 2)
                        cout << "\n";
                else
                        cout << "\n                 ";
        }
        //Vykresleni udaju o hracich
        players.clear();
        players = pList->getPList();
        cout << "Skore hracu:\n";
        for (unsigned int i = 0; i < players.size(); i++)
        {
                cout << "Player " << players[i]->getColor() << ": ";
                cout << players[i]->getScore() << "/" << (pMap->getCardsSet() / (pMap->getPlayers())->getNumber());
                if (players[i] == pList->GetActivePlayer())
                {
                        cout << " <- " << "Hleda poklad:" << (char)((players[i]->getPlayersCard())->getTreasure() + 64);
                }
                cout << "\n";
        }
        players.clear();
}

/**
 * @brief Funkce resici vykresleni jednotlivych radku policek
 * @param Tvar kamene, ktery chceme vykreslit
 * @param Rotace kamene, ktery chceme vykreslit
 * @param Cislo radku policka ktere chceme vykreslit (policko je 3x3)
 * @param Cislo pokladu, ktery ma byt na danem policku
 */
void gameCLI::PrintField(char shape, int rotation, int line, int treasure)
{
        switch (shape)
        {
        case 'L':
                switch (rotation)
                {
                case 0:
                        switch (line)
                        {
                        case 0:
                                cout << "XXX";
                                break;
                        case 1:
                                if (treasure == 0)
                                        cout << "  X";
                                else
                                        cout << " " << (char)(treasure+64) << "X";
                                break;
                        case 2:
                                cout << "X X";
                                break;
                        }
                        break;
                case 1:
                        switch (line)
                        {
                        case 0:
                                cout << "X X";
                                break;
                        case 1:
                                if (treasure == 0)
                                        cout << "  X";
                                else
                                        cout << " " << (char)(treasure + 64) << "X";
                                break;
                        case 2:
                                cout << "XXX";
                                break;
                        }
                        break;
                case 2:
                        switch (line)
                        {
                        case 0:
                                cout << "X X";
                                break;
                        case 1:
                                if (treasure == 0)
                                        cout << "X  ";
                                else
                                        cout << "X" << (char)(treasure + 64) << " ";
                                break;
                        case 2:
                                cout << "XXX";
                                break;
                        }
                        break;
                case 3:
                        switch (line)
                        {
                        case 0:
                                cout << "XXX";
                                break;
                        case 1:
                                if (treasure == 0)
                                        cout << "X  ";
                                else
                                        cout << "X" << (char)(treasure + 64) << " ";
                                break;
                        case 2:
                                cout << "X X";
                                break;
                        }
                        break;
                }
                break;
        case 'T':
                switch (rotation)
                {
                case 0:
                        switch (line)
                        {
                        case 0:
                                cout << "XXX";
                                break;
                        case 1:
                                if (treasure == 0)
                                        cout << "   ";
                                else
                                        cout << " " << (char)(treasure + 64) << " ";
                                break;
                        case 2:
                                cout << "X X";
                                break;
                        }
                        break;
                case 1:
                        switch (line)
                        {
                        case 0:
                                cout << "X X";
                                break;
                        case 1:
                                if (treasure == 0)
                                        cout << "  X";
                                else
                                        cout << " " << (char)(treasure + 64) << "X";
                                break;
                        case 2:
                                cout << "X X";
                                break;
                        }
                        break;
                case 2:
                        switch (line)
                        {
                        case 0:
                                cout << "X X";
                                break;
                        case 1:
                                if (treasure == 0)
                                        cout << "   ";
                                else
                                        cout << " " << (char)(treasure + 64) << " ";
                                break;
                        case 2:
                                cout << "XXX";
                                break;
                        }
                        break;
                case 3:
                        switch (line)
                        {
                        case 0:
                                cout << "X X";
                                break;
                        case 1:
                                if (treasure == 0)
                                        cout << "X  ";
                                else
                                        cout << "X" << (char)(treasure + 64) << " ";
                                break;
                        case 2:
                                cout << "X X";
                                break;
                        }
                        break;
                }
                break;
        case 'I':
                switch (rotation)
                {
                case 0:
                        switch (line)
                        {
                        case 0:
                                cout << "XXX";
                                break;
                        case 1:
                                if (treasure == 0)
                                        cout << "   ";
                                else
                                        cout << " " << (char)(treasure + 64) << " ";
                                break;
                        case 2:
                                cout << "XXX";
                                break;
                        }
                        break;
                case 1:
                        switch (line)
                        {
                        case 0:
                                cout << "X X";
                                break;
                        case 1:
                                if (treasure == 0)
                                        cout << "X X";
                                else
                                        cout << "X" << (char)(treasure + 64) << "X";
                                break;
                        case 2:
                                cout << "X X";
                                break;
                        }
                        break;
                case 2:
                        switch (line)
                        {
                        case 0:
                                cout << "XXX";
                                break;
                        case 1:
                                if (treasure == 0)
                                        cout << "   ";
                                else
                                        cout << " " << (char)(treasure + 64) << " ";
                                break;
                        case 2:
                                cout << "XXX";
                                break;
                        }
                        break;
                case 3:
                        switch (line)
                        {
                        case 0:
                                cout << "X X";
                                break;
                        case 1:
                                if (treasure == 0)
                                        cout << "X X";
                                else
                                        cout << "X" << (char)(treasure + 64) << "X";
                                break;
                        case 2:
                                cout << "X X";
                                break;
                        }
                        break;
                }
                break;
        }
}
