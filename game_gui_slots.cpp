/**
  Autori: Lukas Dibdak, xdibda00
  Modul: GameGUI - Slots
  Popis modulu: Tento modul implementuje hlavni rozhrani pro
  GUI verzi hry - graficke vykresleni.
  Upresneni specializace modulu: Tato cast se stara o logiku
  slotu, na ktere se odkazuje pri stisknuti klikacich tlacitek.
  */

#include "game_gui.h"

extern Map* fieldsMap;
extern QtGraphics* graphics;
extern Game* game;
extern Controller *mapControl;

/**
 * @brief slouzi k rotaci policka v informacnim panelu vpravo.
 * @param toto policko.
 */
void Button::rotate(QGraphicsPixmapItem *missing_puzzle) {
    missing_puzzle->setRotation(90);
}

/**
 * @brief slouzi k odkliknuti tlacitko, pokud bylo kliknuto jine.
 */
void Button::hoverUnclicked() {
   setPixmap(QPixmap(QString(*nohover)));
   *noHover = *nohover;
   *Hover = *hover;
}

/**
 * @brief slouzi ke kliknuti tlacitka.
 */
void Button::hoverClicked() {
    setPixmap(QPixmap(QString(*click)));
    if(this->getID() == 2 or this->getID() == 3 or this->getID() == 4) {
        fieldsMap->setPlayers(this->getID());
    }
    else if(this->getID() == 12 or this->getID() == 24) {
        if (fieldsMap->getSize() != 5) {
            fieldsMap->setCardsSet(this->getID());
        }
        else {
            fieldsMap->setCardsSet(12);
        }
    }
    else {
        fieldsMap->setSize(this->getID());
        graphics->actualizeOffset();
    }
    *noHover = *click;
    *Hover = *click;
}

/**
 * @brief rotace policko, ktere se ma vlozit - je ulozeno v informacnim panelu.
 */
void Game::rotate() {
    mapControl->rotateField(fieldsMap);
    refreshMissingPiece();
}

/**
 * @brief o jeden krok zpet - undo.
 */
void Game::stepBack() {
    mapControl->undoTurn(fieldsMap);
    makeDesk();
}

/**
 * @brief vrati se do menu.
 */
void Game::ret_mm() {
    setScene(scene);
    displayMenu();
}

/**
 * @brief hrac se vraci do hry - prepne se na scenu hry.
 */
void Game::ret_game() {
    setScene(gameScene);
    setBackgroundBrush(QBrush(QImage(":/bg/img/bg/gamebg.jpg")));
}

/**
 * @brief prepne na scenu autoru.
 */
void Game::authors_view() {
    setScene(authors);
    setBackgroundBrush(QBrush(QImage(":/bg/img/bg/creditsbg.jpg")));
}

/**
 * @brief slot pro ulozeni hry.
 */
void Game::saveGame() {
    if (isGameStarted()) {
        mapControl->getLog()->saveGame(*fieldsMap);
    }
}

/**
 * @brief slouzi k posunuti mapy - vlozeni kamene do mapy.
 * @param cislo pozice kam se vklada - x* size + y.
 */
void Game::shiftB(int number) {
    if(fieldsMap->getShiftedState() != 1) {
        int x = 0, y = 0;
        for(int i = 0; i <= fieldsMap->getSize(); i++) {
           if (number < i * fieldsMap->getSize()) {
               x = i - 1;
               y = number - x * fieldsMap->getSize();
               break;
           }
         }
        if(mapControl->shiftBoard(x, y, fieldsMap))
            fieldsMap->changeShiftedState(1);
        makeDesk();
    }
}

/**
 * @brief slot pro nacteni hry.
 */
void Game::loadGame() {
    delete fieldsMap;
    fieldsMap = new Map();
    delete mapControl;
    mapControl = new Controller();
    delete graphics;
    graphics = new QtGraphics();

    setScene(gameScene);
    gameScene->clear();
    setBackgroundBrush(QBrush(QImage(":/bg/img/bg/gamebg.jpg")));

    mapControl->getLog()->loadGame(fieldsMap);
    graphics->actualizeOffset();
    gameStarted = true;
    makeDesk();

    writeSideway();
    panel->setValues();
    refreshMissingPiece();

    Button *endturn = new Button(QString(":/buttons/img/buttons/endturnbutton.png"), 600, 425, gameScene, ":/buttons/img/buttons/endturnbuttonhover.png");
    Button *stepback = new Button(QString(":/buttons/img/buttons/sbbutton.png"), 600, 480, gameScene, ":/buttons/img/buttons/sbbuttonhover.png");
    Button *mainmenu = new Button(QString(":/buttons/img/buttons/mmbutton.png"), 600, 535, gameScene, ":/buttons/img/buttons/mmbuttonhover.png");
    Button *rotate = new Button(":/other/img/other/rotate.png", 702, 190, gameScene, ":/other/img/other/rotatehover.png", ":/other/img/other/rotate.png", 12);
    connect(mainmenu, SIGNAL(clicked()), this, SLOT(ret_mm()));
    connect(rotate, SIGNAL(clicked()), game, SLOT(rotate()));
    connect(endturn, SIGNAL(clicked()), game, SLOT(nextPlayer()));
}

/**
 * @brief prepne na dalsiho hrace po ukonceni tahu.
 */
void Game::nextPlayer() {
    mapControl->EndTurn(fieldsMap);
    panel->resetValues();
    panel->setValues();
}

/**
 * @brief spusti novou hru.
 */
void Game::start() {
    fieldsMap->setLastX(-1);
    fieldsMap->setLastY(-1);
    mapControl = new Controller();
    setScene(gameScene);
    gameScene->clear();
    setBackgroundBrush(QBrush(QImage(":/bg/img/bg/gamebg.jpg")));
    fieldsMap->changeShiftedState(false);
    fieldsMap->createMap();
    writeSideway();
    makeDesk();
    Button *endturn = new Button(QString(":/buttons/img/buttons/endturnbutton.png"), 600, 425, gameScene, ":/buttons/img/buttons/endturnbuttonhover.png");
    Button *stepback = new Button(QString(":/buttons/img/buttons/sbbutton.png"), 600, 480, gameScene, ":/buttons/img/buttons/sbbuttonhover.png");
    Button *mainmenu = new Button(QString(":/buttons/img/buttons/mmbutton.png"), 600, 535, gameScene, ":/buttons/img/buttons/mmbuttonhover.png");
    Button *rotate = new Button(":/other/img/other/rotate.png", 702, 190, gameScene, ":/other/img/other/rotatehover.png", ":/other/img/other/rotate.png", 12);
    connect(mainmenu, SIGNAL(clicked()), this, SLOT(ret_mm()));
    connect(stepback, SIGNAL(clicked()), this, SLOT(stepBack()));
    connect(rotate, SIGNAL(clicked()), game, SLOT(rotate()));
    connect(endturn, SIGNAL(clicked()), game, SLOT(nextPlayer()));
    gameStarted = true;
    panel->setValues();
    fieldsMap->getPlayers()->getPList().clear();
}

/**
 * @brief prepne na scenu vyberu parametru pro novou hru.
 */
void Game::choose() {
    delete fieldsMap;
    fieldsMap = new Map();
    delete graphics;
    graphics = new QtGraphics();

    newgame->clear();
    setScene(newgame);
    setBackgroundBrush(QBrush(QImage(":/bg/img/bg/bgchoose.jpg")));

    Button *littleset = new Button(":/buttons/img/buttons/12.png", 70, 380, newgame, ":/buttons/img/buttons/12hover.png", ":/buttons/img/buttons/12clicked.png", 12);
    Button *largeset = new Button(":/buttons/img/buttons/24.png", 225, 380, newgame, ":/buttons/img/buttons/24hover.png", ":/buttons/img/buttons/24clicked.png", 24);
    Button *twoplayers = new Button(":/buttons/img/buttons/2.png", 70, 70, newgame, ":/buttons/img/buttons/2hover.png", ":/buttons/img/buttons/2clicked.png", 2);
    Button *threeplayers = new Button(":/buttons/img/buttons/3.png", 225, 70, newgame, ":/buttons/img/buttons/3hover.png", ":/buttons/img/buttons/3clicked.png", 3);
    Button *fourplayers = new Button(":/buttons/img/buttons/4.png", 380, 70, newgame, ":/buttons/img/buttons/4hover.png", ":/buttons/img/buttons/4clicked.png", 4);
    Button *sizefive = new Button(":/buttons/img/buttons/55.png", 70, 220, newgame, ":/buttons/img/buttons/55hover.png", ":/buttons/img/buttons/55clicked.png", 5);
    Button *sizeseven = new Button(":/buttons/img/buttons/77.png", 225, 220, newgame, ":/buttons/img/buttons/77hover.png", ":/buttons/img/buttons/77clicked.png", 7);
    Button *sizenine = new Button(":/buttons/img/buttons/99.png", 380, 220, newgame, ":/buttons/img/buttons/99hover.png", ":/buttons/img/buttons/99clicked.png", 9);
    Button *sizeeleven = new Button(":/buttons/img/buttons/1111.png", 535, 220, newgame, ":/buttons/img/buttons/1111hover.png", ":/buttons/img/buttons/1111clicked.png", 11);
    Button *ok = new Button(":/buttons/img/buttons/okbutton.png", 204, 510, newgame, ":/buttons/img/buttons/okbuttonhover.png");
    Button *cancel = new Button(":/buttons/img/buttons/cancelbutton.png", 400, 510, newgame, ":/buttons/img/buttons/cancelbuttonhover.png");
    littleset->setDefault();
    twoplayers->setDefault();
    sizeseven->setDefault();
    connect(ok, SIGNAL(clicked()), this, SLOT(start()));
    connect(cancel, SIGNAL(clicked()), this, SLOT(ret_mm()));
    connect(littleset, SIGNAL(clicked()), littleset, SLOT(hoverClicked()));
    connect(littleset, SIGNAL(clicked()), largeset, SLOT(hoverUnclicked()));
    connect(largeset, SIGNAL(clicked()), largeset, SLOT(hoverClicked()));
    connect(largeset, SIGNAL(clicked()), littleset, SLOT(hoverUnclicked()));
    connect(twoplayers, SIGNAL(clicked()), twoplayers, SLOT(hoverClicked()));
    connect(twoplayers, SIGNAL(clicked()), threeplayers, SLOT(hoverUnclicked()));
    connect(twoplayers, SIGNAL(clicked()), fourplayers, SLOT(hoverUnclicked()));
    connect(threeplayers, SIGNAL(clicked()), twoplayers, SLOT(hoverUnclicked()));
    connect(threeplayers, SIGNAL(clicked()), threeplayers, SLOT(hoverClicked()));
    connect(threeplayers, SIGNAL(clicked()), fourplayers, SLOT(hoverUnclicked()));
    connect(fourplayers, SIGNAL(clicked()), twoplayers, SLOT(hoverUnclicked()));
    connect(fourplayers, SIGNAL(clicked()), threeplayers, SLOT(hoverUnclicked()));
    connect(fourplayers, SIGNAL(clicked()), fourplayers, SLOT(hoverClicked()));
    connect(sizefive, SIGNAL(clicked()), sizefive, SLOT(hoverClicked()));
    connect(sizefive, SIGNAL(clicked()), sizeseven, SLOT(hoverUnclicked()));
    connect(sizefive, SIGNAL(clicked()), sizenine, SLOT(hoverUnclicked()));
    connect(sizefive, SIGNAL(clicked()), sizeeleven, SLOT(hoverUnclicked()));
    connect(sizeseven, SIGNAL(clicked()), sizefive, SLOT(hoverUnclicked()));
    connect(sizeseven, SIGNAL(clicked()), sizeseven, SLOT(hoverClicked()));
    connect(sizeseven, SIGNAL(clicked()), sizenine, SLOT(hoverUnclicked()));
    connect(sizeseven, SIGNAL(clicked()), sizeeleven, SLOT(hoverUnclicked()));
    connect(sizenine, SIGNAL(clicked()), sizefive, SLOT(hoverUnclicked()));
    connect(sizenine, SIGNAL(clicked()), sizeseven, SLOT(hoverUnclicked()));
    connect(sizenine, SIGNAL(clicked()), sizenine, SLOT(hoverClicked()));
    connect(sizenine, SIGNAL(clicked()), sizeeleven, SLOT(hoverUnclicked()));
    connect(sizeeleven, SIGNAL(clicked()), sizefive, SLOT(hoverUnclicked()));
    connect(sizeeleven, SIGNAL(clicked()), sizeseven, SLOT(hoverUnclicked()));
    connect(sizeeleven, SIGNAL(clicked()), sizenine, SLOT(hoverUnclicked()));
    connect(sizeeleven, SIGNAL(clicked()), sizeeleven, SLOT(hoverClicked()));
}
