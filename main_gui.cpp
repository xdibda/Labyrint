/**
  Autori: Lukas Dibdak, xdibda00
  Modul: Soubor s main funkci pro spusteni GUI
  */

#include <QApplication>
#include "game_gui.h"

Game *game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    game = new Game();
    game->show();
    game->displayMenu();

    return a.exec();
}
