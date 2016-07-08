/**
  Autori: Lukas Dibdak, xdibda00
  Modul: GameGUI - Graphics
  Popis modulu: Tento modul implementuje hlavni rozhrani pro
  GUI verzi hry - graficke vykresleni.
  Upresneni specializace modulu: Tato cast se stara o pristup
  ke grafice a vykresluje sceny.
  */

#include "game_gui.h"

extern Map* fieldsMap;
extern QtGraphics* graphics;
extern Game* game;
extern QSignalMapper* mapper;
extern Controller *mapControl;

/**
 * @brief konecna scena hry.
 */
void Game::gameEndedScreen() {
    gameStarted = false;
    setScene(endgame);
    setBackgroundBrush(QBrush(QImage(":/bg/img/bg/endgame.jpg")));
    QGraphicsPixmapItem *winPlayerFigure = endgame->addPixmap(QPixmap(getFigureReferenceBig(fieldsMap->getPlayers()->GetActivePlayer()->getColor())));
    winPlayerFigure->setPos(420, 275);
    Button *mainMenuEnd = new Button(QString(":/buttons/img/buttons/mmbutton.png"), 300, 500, endgame, ":/buttons/img/buttons/mmbuttonhover.png");
    QObject::connect(mainMenuEnd, SIGNAL(clicked()), this, SLOT(ret_mm()));
}

/**
 * @brief vykresleni bocniho panelu
 * @param x souradnice.
 * @param y souradnice.
 * @param sirka.
 * @param vyska.
 * @param barva.
 * @param pruhlednost.
 * @param scena na kterou se ma vykreslit.
 */
void Game::drawPanel(int x, int y, int width, int height, QColor color, double opacity, QGraphicsScene *scene) {
    QGraphicsRectItem *panel = new QGraphicsRectItem;
    panel->setRect(x, y, width, height);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    panel->setBrush(brush);
    panel->setOpacity(opacity);
    scene->addItem(panel);
}

/**
 * @brief odalokovani a odmazani bocnich dynamickych udaju.
 */
void SidePanel::resetValues() {
   game->gameScene->removeItem(playersIcon);
   game->gameScene->removeItem(playersItem);
   game->gameScene->removeItem(item);
   delete item;
   delete playersIcon;
   delete playersItem;
}

/**
 * @brief natavi hodnoty hrace v bocnim panelu.
 */
void SidePanel::setValues() {
    playersIcon = new QGraphicsPixmapItem();
    playersIcon = game->gameScene->addPixmap(QPixmap(QString(getFigureReference(fieldsMap->getPlayers()->GetActivePlayer()->getColor()))));
    playersIcon->setPos(734, 32);
    playersItem = new QGraphicsPixmapItem();
    playersItem = game->gameScene->addPixmap(QPixmap(getTreasureBig(fieldsMap->getPlayers()->GetActivePlayer()->getPlayersCard()->getTreasure())));
    playersItem->setPos(725, 242);
    item = new QGraphicsTextItem(QString::number(fieldsMap->getPlayers()->GetActivePlayer()->getScore()) + " / " + QString::number(fieldsMap->getCards()->getSize() / fieldsMap->getPlayers()->getNumber()));
    item->setPos(720, 86);
    item->setFont(QFont("Times", 13));
    game->gameScene->addItem(item);
}

/**
 * @brief aktualizuje bocni policko na vkladani do mapy.
 */
void Game::refreshMissingPiece() {
    setMissingPiece(fieldsMap->getFields().back());
    QGraphicsPixmapItem *missing_puzzle = gameScene->addPixmap(QPixmap(getPathReference(fieldsMap->getFields().back()->getShape())));
    missing_puzzle->setTransformOriginPoint(19,19);
    missing_puzzle->setRotation(fieldsMap->getFields().back()->getRotation() * 90);
    missing_puzzle->setPos(723, 150);
    if(fieldsMap->getFields().back()->getTreasure() != 0) {
         QGraphicsPixmapItem *item = gameScene->addPixmap(QPixmap(QString(getTreasureIcon(fieldsMap->getFields().back()->getTreasure()))));
         item->setPos(733, 160);
    }
}

/**
 * vykresli vnitrek bocniho panelu.
 */
void Game::writeSideway() {
    drawPanel(600, 15, 189, 400, Qt::white, 0.5, gameScene);
    drawPanel(605, 20, 179, 55, Qt::white, 0.5, gameScene);
    drawPanel(605, 80, 179, 40, Qt::white, 0.5, gameScene);
    drawPanel(605, 125, 179, 85, Qt::white, 0.5, gameScene);
    drawPanel(605, 215, 179, 85, Qt::white, 0.5, gameScene);
    drawPanel(605, 305, 179, 105, Qt::white, 0.5, gameScene);
    QGraphicsPixmapItem *curplaying = gameScene->addPixmap(QPixmap(":/other/img/other/currentlyplaying.png"));
    curplaying->setPos(615, 22);
    QGraphicsPixmapItem *score = gameScene->addPixmap(QPixmap(":/other/img/other/score.png"));
    score->setPos(610, 81);
    QGraphicsPixmapItem *puzzle = gameScene->addPixmap(QPixmap(":/other/img/other/puzzle.png"));
    puzzle->setPos(610, 127);
    QGraphicsPixmapItem *item = gameScene->addPixmap(QPixmap(":/other/img/other/item.png"));
    item->setPos(610, 216);
    QGraphicsPixmapItem *allp = gameScene->addPixmap(QPixmap(":/other/img/other/allplayers.png"));
    allp->setPos(606, 308);
    if (fieldsMap->getPlayers()->getNumber() == 2) {
        QGraphicsPixmapItem *playersNumber = gameScene->addPixmap(QPixmap(":/other/img/other/players2.png"));
        playersNumber->setPos(650, 338);
    }
    else if (fieldsMap->getPlayers()->getNumber() == 3) {
        QGraphicsPixmapItem *playersNumber = gameScene->addPixmap(QPixmap(":/other/img/other/players3.png"));
        playersNumber->setPos(650, 338);
    }
    else if (fieldsMap->getPlayers()->getNumber() == 4) {
        QGraphicsPixmapItem *playersNumber = gameScene->addPixmap(QPixmap(":/other/img/other/players4.png"));
        playersNumber->setPos(650, 338);
    }
    QGraphicsPixmapItem *firstPlayerIcon = gameScene->addPixmap(QPixmap(getFigureReferenceMenu(fieldsMap->getPlayers()->getPList().at(0)->getColor())));
    firstPlayerIcon->setPos(645, 346);
    QGraphicsPixmapItem *secondPlayerIcon = gameScene->addPixmap(QPixmap(getFigureReferenceMenu(fieldsMap->getPlayers()->getPList().at(1)->getColor())));
    secondPlayerIcon->setPos(645, 361);
    if(fieldsMap->getPlayers()->getNumber() > 2) {
        QGraphicsPixmapItem *thirdPlayerIcon = gameScene->addPixmap(QPixmap(getFigureReferenceMenu(fieldsMap->getPlayers()->getPList().at(2)->getColor())));
        thirdPlayerIcon->setPos(645, 376);
    }
    if(fieldsMap->getPlayers()->getNumber() > 3) {
        QGraphicsPixmapItem *fourthPlayerIcon = gameScene->addPixmap(QPixmap(getFigureReferenceMenu(fieldsMap->getPlayers()->getPList().at(3)->getColor())));
        fourthPlayerIcon->setPos(645, 391);
    }
}

/**
 * @brief zobrazí hlavní menu hry.
 */
void Game::displayMenu() {
    setScene(scene);
    setBackgroundBrush(QBrush(QImage(":/bg/img/bg/bg.jpg")));
    if(isGameStarted()) {
        Button *ret_frame = new Button(":/buttons/img/buttons/retbutton.png", 250, 457, scene, ":/buttons/img/buttons/retbuttonhover.png");
        connect(ret_frame, SIGNAL(clicked()), this, SLOT(ret_game()));
        Button *sg_button = new Button(":/buttons/img/buttons/sgbutton.png", 250, 292, scene, ":/buttons/img/buttons/sgbuttonhover.png");
        connect(sg_button, SIGNAL(clicked()), this, SLOT(saveGame()));
    }
    else {
        Button *ret_frame = new Button(":/buttons/img/buttons/retbuttongray.png", 250, 457, scene, ":/buttons/img/buttons/retbuttongray.png");
        Button *sg_button = new Button(":/buttons/img/buttons/sgbuttongrey.png", 250, 292, scene, ":/buttons/img/buttons/sgbuttongrey.png");
    }
}

/**
 * @brief vykresli hlavni menu a autory - staticke obrazovky.
 */
void Game::drawGUI() {
    // authors;
    Button *back = new Button(":/buttons/img/buttons/backbutton.png", 250, 520, authors, ":/buttons/img/buttons/backbuttonhover.png");
    QGraphicsPixmapItem *creText = authors->addPixmap(QPixmap(":/bg/img/bg/creditstext.jpg"));
    creText->setPos(100,140);
    connect(back, SIGNAL(clicked()), this, SLOT(ret_mm()));
    // main menu;
    Button *ng_frame = new Button(":/buttons/img/buttons/ngbutton.png", 250, 237, scene, ":/buttons/img/buttons/ngbuttonhover.png");
    Button *lg_frame = new Button(":/buttons/img/buttons/lgbutton.png", 250, 347, scene, ":/buttons/img/buttons/lgbuttonhover.png");
    Button *authors_frame = new Button(":/buttons/img/buttons/crebutton.png", 250, 402, scene, ":/buttons/img/buttons/crebuttonhover.png");
    Button *quit_frame = new Button(":/buttons/img/buttons/qgbutton.png", 250, 512, scene, ":/buttons/img/buttons/qgbuttonhover.png");
    connect(ng_frame, SIGNAL(clicked()), this, SLOT(choose()));
    connect(quit_frame, SIGNAL(clicked()), this, SLOT(close()));
    connect(authors_frame, SIGNAL(clicked()), this, SLOT(authors_view()));
    connect(lg_frame, SIGNAL(clicked()), this, SLOT(loadGame()));
}

/** --- Funkce, ktere vraci grafiku. --- */
/**
 * @brief vraci grafiku k figurky.
 * @param barva figurky.
 * @return vraci adresu k figurce.
 */
QString Game::getFigureReferenceBig(int color) {
    switch (color) {
    case 0:
        return QString(":/figures/img/figures/figure_blue.png");
    case 1:
        return QString(":/figures/img/figures/figure_red.png");
    case 2:
        return QString(":/figures/img/figures/figure_yellow.png");
    case 3:
        return QString(":/figures/img/figures/figure_green.png");
    }
}

/**
 * @brief vraci grafiku cesty
 * @param znak cesty - T, L, I.
 * @return vraci adresu grafiky.
 */
QString getPathReference(char c) {
    QString path;
    switch (c) {
    case 'L':
        path = QString(":/path/img/path/l_shape35px.png");
        break;
    case 'T':
        path = QString(":/path/img/path/t_shape35px.png");
        break;
    case 'I':
        path = QString(":/path/img/path/i_shape35px.png");
        break;
    }
    return path;
}

/**
 * @brief vraci grafiku figurky.
 * @param barva figurky.
 * @return adresa grafiky figurky.
 */
QString getFigureReference(int color) {
    switch (color) {
    case 0:
        return QString(":/figures/img/figures/figure_blue_desk.png");
    case 1:
        return QString(":/figures/img/figures/figure_red_desk.png");
    case 2:
        return QString(":/figures/img/figures/figure_yellow_desk.png");
    case 3:
        return QString(":/figures/img/figures/figure_green_desk.png");
    }
}

/**
 * @brief vraci malou figurku v menu.
 * @param barva figurky.
 * @return vraci adresu zmensene figurky.
 */
QString getFigureReferenceMenu(int color) {
    switch (color) {
    case 0:
        return QString(":/figures/img/figures/figure_blue_menu.png");
    case 1:
        return QString(":/figures/img/figures/figure_red_menu.png");
    case 2:
        return QString(":/figures/img/figures/figure_yellow_menu.png");
    case 3:
        return QString(":/figures/img/figures/figure_green_menu.png");
    }
}

/**
 * @brief vraci obrazek pokladu.
 * @param id pokladu.
 * @return adresa grafiky pokladu.
 */
QString getTreasureIcon(int treasureID) {
    QString returnValue;
    switch (treasureID) {
    case 1:
        returnValue = ":/items/img/items/vag1min.png";
        break;
    case 2:
        returnValue = ":/items/img/items/vag2min.png";
        break;
    case 3:
        returnValue = ":/items/img/items/triangle1min.png";
        break;
    case 4:
        returnValue = ":/items/img/items/triangle3min.png";
        break;
    case 5:
        returnValue = ":/items/img/items/star2min.png";
        break;
    case 6:
        returnValue = ":/items/img/items/star3min.png";
        break;
    case 7:
        returnValue = ":/items/img/items/square3min.png";
        break;
    case 8:
        returnValue = ":/items/img/items/square4min.png";
        break;
    case 9:
        returnValue = ":/items/img/items/cross4min.png";
        break;
    case 10:
        returnValue = ":/items/img/items/cross1min.png";
        break;
    case 11:
        returnValue = ":/items/img/items/circle1min.png";
        break;
    case 12:
        returnValue = ":/items/img/items/circle2min.png";
        break;
    case 13:
        returnValue = ":/items/img/items/vag3min.png";
        break;
    case 14:
        returnValue = ":/items/img/items/vag4min.png";
        break;
    case 15:
        returnValue = ":/items/img/items/triangle2min.png";
        break;
    case 16:
        returnValue = ":/items/img/items/triangle4min.png";
        break;
    case 17:
        returnValue = ":/items/img/items/star1min.png";
        break;
    case 18:
        returnValue = ":/items/img/items/star4min.png";
        break;
    case 19:
        returnValue = ":/items/img/items/square1min.png";
        break;
    case 20:
        returnValue = ":/items/img/items/square2min.png";
        break;
    case 21:
        returnValue = ":/items/img/items/cross3min.png";
        break;
    case 22:
        returnValue = ":/items/img/items/cross2min.png";
        break;
    case 23:
        returnValue = ":/items/img/items/circle3min.png";
        break;
    case 24:
        returnValue = ":/items/img/items/circle4min.png";
        break;
    }
    return returnValue;
}

/**
 * @brief vraci grafiku velkeho pokladu do menu.
 * @param id pokladu.
 * @return vraci grafiku pokladu do menu.
 */
QString getTreasureBig(int treasureID) {
    QString returnValue;
    switch (treasureID) {
    case 1:
        returnValue = ":/items/img/items/vag1.png";
        break;
    case 2:
        returnValue = ":/items/img/items/vag2.png";
        break;
    case 3:
        returnValue = ":/items/img/items/triangle1.png";
        break;
    case 4:
        returnValue = ":/items/img/items/triangle3.png";
        break;
    case 5:
        returnValue = ":/items/img/items/star2.png";
        break;
    case 6:
        returnValue = ":/items/img/items/star3.png";
        break;
    case 7:
        returnValue = ":/items/img/items/square3.png";
        break;
    case 8:
        returnValue = ":/items/img/items/square4.png";
        break;
    case 9:
        returnValue = ":/items/img/items/cross4.png";
        break;
    case 10:
        returnValue = ":/items/img/items/cross1.png";
        break;
    case 11:
        returnValue = ":/items/img/items/circle1.png";
        break;
    case 12:
        returnValue = ":/items/img/items/circle2.png";
        break;
    case 13:
        returnValue = ":/items/img/items/vag3.png";
        break;
    case 14:
        returnValue = ":/items/img/items/vag4.png";
        break;
    case 15:
        returnValue = ":/items/img/items/triangle2.png";
        break;
    case 16:
        returnValue = ":/items/img/items/triangle4.png";
        break;
    case 17:
        returnValue = ":/items/img/items/star1.png";
        break;
    case 18:
        returnValue = ":/items/img/items/star4.png";
        break;
    case 19:
        returnValue = ":/items/img/items/square1.png";
        break;
    case 20:
        returnValue = ":/items/img/items/square2.png";
        break;
    case 21:
        returnValue = ":/items/img/items/cross3.png";
        break;
    case 22:
        returnValue = ":/items/img/items/cross2.png";
        break;
    case 23:
        returnValue = ":/items/img/items/circle3.png";
        break;
    case 24:
        returnValue = ":/items/img/items/circle4.png";
        break;
    }
    return returnValue;
}
