/**
 * Autori: David Porizek, xporiz03
 * Modul: mainCLI
 * Popis modulu: Hlavni telo programu konzolove casti. Resi zpracovani parametru, vypisovani zprav o spatnem vstupu hry, resi volanim jadra aplikace.
 */

#include <iostream>
#include <string>

#include "game_cli.h"
#include "log.h"

using namespace std;

/**
 * @brief Telo cele CLI aplikace, spravuje parametry a vola potrebne funcke z jadra aplikace.
 * @param pocet parametru.
 * @param pole s parametry.
 * @return 1 pokud program skonci bez chyby, jinak 0.
 */
int main(int argc, char *argv[])
{
        Map* MyMap = new Map;
        MyMap->setPlayers(4);
        MyMap->setSize(7);
        MyMap->setCardsSet(12);
        //Parsovani parametru
        if (argc > 4)
        {
                std::cout << "Too many parameters";
        }
        else if (argc == 2 && (argv[1] == string("help") || argv[1] == string("-help") || argv[1] == string("--help")))
        {
                std::cout << "###############################################################################\n"
                        "ICP Projekt\n"
                        "Autor:David Porizek (CLI), Lukas Dibdak (Qt)\n"
                        "###############################################################################\n"
                        "Parametry CLI:\n"
                        "Spusteni: klient P C S\n"
                        "	help	Optional: Vypise napovedu\n"
                        "	P		Required: Nastavi pocet hracu:: 2 az 4\n"
                        "	C		Required: Nastavi pocet karet:: 12 nebo 24\n"
                        "	S		Optional: Nastavi rozmer:: pouze licha cisla od 5 do 15\n"
                        "###############################################################################\n";
                string command;
                while (command != string("exit"))
                {
                        std::cout << "Napiste exit pro ukonceni.\n";
                        cin >> command;
                }
                return 1;
        }
        else if (argc == 3)
        {
                if (stoi(argv[1]) >= 2 && stoi(argv[1]) <= 4)
                        MyMap->setPlayers(stoi(argv[1]));
                else
                        std::cout << "Spatne parametry, pouzijte './app help' pro zobrazeni napovedy.\n";
                if (stoi(argv[2]) == 12 || stoi(argv[2]) == 24)
                        MyMap->setCardsSet(stoi(argv[2]));
                else
                {
                        std::cout << "Spatne parametry, pouzijte './app help' pro zobrazeni napovedy.\n";
                        return 0;
                }
        }
        else if (argc == 4)
        {
                if (stoi(argv[1]) >= 2 && stoi(argv[1]) <= 4)
                        MyMap->setPlayers(stoi(argv[1]));
                else
                        std::cout << "Spatne parametry, pouzijte './app help' pro zobrazeni napovedy.\n";
                if (stoi(argv[2]) == 12 || stoi(argv[2]) == 24)
                        MyMap->setCardsSet(stoi(argv[2]));
                else
                        std::cout << "Spatne parametry, pouzijte './app help' pro zobrazeni napovedy.\n";

                if ((stoi(argv[3]) >= 5 && stoi(argv[3]) <= 11) && stoi(argv[3]) % 2 == 1)
                        MyMap->setSize(stoi(argv[3]));
                else
                {
                        std::cout << "Spatne parametry, pouzijte './app help' pro zobrazeni napovedy.\n";
                        return 0;
                }
        }
        else
        {
                std::cout << "Spatne parametry, pouzijte './app help' pro zobrazeni napovedy.\n";
                return 0;
        }
        //Incializace po proparsovani argumentu
        string command;
        MyMap->createMap();
        Controller control;
        PlayerList* pList;
        Logger* logging = control.getLog();
        gameCLI CLI;
        while (!(control.GameEnded()))
        {
                Player* pToPlay = MyMap->GetActivePlayer();
                CLI.DrawBoard(MyMap);
                std::cout << "\nTah cislo:" << logging->getTurnNumber();
                std::cout << "\nCLI Ovladani:\n";
                std::cout << "r     Rotace kamene navic o 90 stupnu doprava.\n";
                std::cout << "iX,Y  Vlozi kamen na pozici X - Sloupec, Y - Radek. Povinne na zacatku.\n";
                std::cout << "w     Pohyb s aktivnim hracem nahoru.\n";
                std::cout << "a     Pohyb s aktivnim hracem doleva.\n";
                std::cout << "s     Pohyb s aktivnim hracem dolu.\n";
                std::cout << "d     Pohyb s aktivnim hracem doprava.\n";
                std::cout << "u     Ulozi rozehranou hru tak jak je.\n";
                std::cout << "n     Nacte ulozenou hru tak jak byla predtim.\n";
                std::cout << "b     Vrati tah o jeden zpet.\n";
                std::cout << "end   Ukonci tah. Jinak se tah ukonci sam, pokud hrac vezme poklad ktery hleda.\n";
                std::cout << "exit  Ukonci hru.\n";
                std::cout << "<-    Znaci aktivniho hrace a vypisuje jaky poklad hleda.\n";

                while (!(control.GameEnded()))
                {
                        std::cout << "Prikaz: ";
                        cin >> command;
                        unsigned long x;
                        unsigned long y;
                        if (command.substr(0, 4) == "exit")
                        {
                                return 1;
                        }
                        else if (command.substr(0, 3) == "end")
                        {
                                control.EndTurn(MyMap);
                                break;
                        }
                        else if (command.substr(0, 1) == "n")
                        {
                                logging->loadGame(MyMap);
                                break;
                        }
                        else if (command.substr(0, 1) == "u")
                        {
                                logging->saveGame(*MyMap);
                        }
                        else if (command.substr(0, 1) == "b")
                        {
                                if (!control.undoTurn(MyMap))
                                {
                                        std::cout << "Jiz jste na zacatku hry.\n";
                                        continue;
                                }
                                break;
                        }
                        else if (command.substr(0, 1) == "r")
                        {
                                if (MyMap->getShiftedState())
                                {
                                        std::cout << "Rotovat kamen, ktery nebudete vkladat je zbytecne.\n";
                                        continue;
                                }
                                control.rotateField(MyMap);
                                break;
                        }
                        else if (command.substr(0, 1) == "i")
                        {
                                if (MyMap->getShiftedState())
                                {
                                        std::cout << "Nemuzete vkladat kamen vicekrat po sobe.\n";
                                        continue;
                                }
                                else
                                {
                                        command = command.substr(1, string::npos);
                                        if (command.find(',') == string::npos || command.find(' ') != string::npos)
                                        {
                                                std::cout << "Spatny format prikazu vlozeni: iR,S.\n";
                                                continue;
                                        }
                                        try
                                        {
                                                x = stoul(command.substr(0, command.find(',')), nullptr, 10);
                                                y = stoul(command.substr(command.find(',') + 1, string::npos), nullptr, 10);
                                                x -= 1;
                                                y -= 1;
                                        }
                                        catch (const invalid_argument& ia)
                                        {
                                                cerr << "Invalid argument: " << ia.what() << '\n';
                                        }
                                        catch (const out_of_range& oor)
                                        {
                                                cerr << "Out of Range error: " << oor.what() << '\n';
                                        }
                                        if (control.shiftBoard(x, y, MyMap))
                                        {
                                                break;
                                        }
                                        else
                                        {
                                                std::cout << "Wrong coordinations to insert.\n";
                                                continue;
                                        }

                                }
                        }
                        else
                        {
                                if (!MyMap->getShiftedState())
                                {
                                        std::cout << "Napred musite vlozit kamen: iR,S\n";
                                        continue;
                                }

                                else
                                {
                                        if (command.substr(0, 1) == "w")
                                        {
                                                if (!control.MakeMove(UP, pToPlay, MyMap))
                                                {
                                                        std::cout << "Sem nemuzes!\n";
                                                        continue;
                                                }
                                                else
                                                        break;
                                        }
                                        if (command.substr(0, 1) == "s")
                                        {
                                                if (!control.MakeMove(DOWN, pToPlay, MyMap))
                                                {
                                                        std::cout << "Sem nemuzes!\n";
                                                        continue;
                                                }
                                                else
                                                        break;

                                        }
                                        if (command.substr(0, 1) == "a")
                                        {
                                                if (!control.MakeMove(LEFT, pToPlay, MyMap))
                                                {
                                                        std::cout << "Sem nemuzes!\n";
                                                        continue;
                                                }
                                                else
                                                        break;
                                        }
                                        if (command.substr(0, 1) == "d")
                                        {
                                                if (!control.MakeMove(RIGHT, pToPlay, MyMap))
                                                {
                                                        std::cout << "Sem nemuzes!\n";
                                                        continue;
                                                }
                                                else
                                                        break;
                                        }
                                        if (MyMap->GetActivePlayer() != pToPlay)
                                        {
                                                break;
                                        }
                                }
                        }
                }
                CLI.ClearScreen();
        }

        std::cout << "HRA SKONCILA!\n\n\n";

        pList = MyMap->getPlayers();
        vector<Player*> players = pList->getPList();
        std::cout << "Skore hracu:\n";
        for (unsigned int i = 0; i < players.size(); i++)
        {
                std::cout << "Player " << players[i]->getColor() << ": ";
                std::cout << players[i]->getScore() << "/" << (MyMap->getCardsSet() / (MyMap->getPlayers())->getNumber());
                if (players[i] == pList->GetActivePlayer())
                {
                        std::cout << " <- " << "VITEZ";
                }
                std::cout << "\n";
        }
        command.clear();
        while (command != string("exit"))
        {
                std::cout << "Napiste exit pro ukonceni.\n";
                cin >> command;
        }
        return 1;
}
