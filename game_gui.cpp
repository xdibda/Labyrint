/**
  Autori: Lukas Dibdak, xdibda00
  Modul: GameGUI
  Popis modulu: Tento modul implementuje hlavni rozhrani pro
  GUI verzi hry - graficke vykresleni.
  Upresneni specializace modulu: Tato cast se stara o logiku
  vykresleni mapy - sipky, samotna mapa, offset pro posunuti
  na stred.
  */

#include "game_gui.h"
#include <QApplication>

extern Game *game;

Map *fieldsMap;
QtGraphics *graphics;
QSignalMapper* mapper;
Controller *mapControl;

/**
 * @brief konstruktor pro hlavní třídu vykreslování a logiky hry.
 */
Game::Game() {
    gameStarted = false;
    scene = new QGraphicsScene();
    gameScene = new QGraphicsScene();
    authors = new QGraphicsScene();
    newgame = new QGraphicsScene();
    endgame = new QGraphicsScene();
    missingPiece = new MapField();
    panel = new SidePanel();

    scene->setSceneRect(0,0,800,600);
    gameScene->setSceneRect(0,0,800,600);
    authors->setSceneRect(0,0,800,600);
    newgame->setSceneRect(0,0,800,600);
    endgame->setSceneRect(0,0,800,600);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(800, 600);
    drawGUI();
}

/**
 * @brief vrati policko, ktere se ma zasunout do mapy.
 * @return vraci policko.
 */
MapField *Game::getMissingPiece() {
    return missingPiece;
}

/**
 * @brief nastavuje policko, ktere se bude zasunovat do mapy.
 * @param policko, ktere se ma nastavit.
 */
void Game::setMissingPiece(MapField* field) {
    missingPiece = field;
}

/**
 * @brief kontroluje, zda jiz byla zapocata hra.
 * @return true - hra byla zahajena / false - zadna hra neni aktivni.
 */
bool Game::isGameStarted() {
    return gameStarted;
}

/**
 * @brief konstruktor pro figurku.
 * @param c - barva figurky.
 */
Figure::Figure(int c) {
    color = c;
    item = new QGraphicsPixmapItem(QString(getFigureReference(c)));
    setFlag(QGraphicsItem::ItemIsFocusable);
}

/**
 * @brief vraci barvu figurky.
 * @return barva figurky.
 */
int Figure::getColor() {
    return color;
}

/**
 * @brief vraci odkaz na grafiku figurky.
 * @return grafika figurky.
 */
QGraphicsPixmapItem *Figure::getItem() {
    return item;
}

/**
 * @brief vraci bocni informacni panel.
 * @return vraci panel.
 */
SidePanel *Game::getPanel() {
    return panel;
}

/**
 * @brief generateFigure - funkce vygeneruje figurku a vlozi ji do mapy na dane policko
 * @param scene - scena, na jakou se ma figurka vlozit
 * @param xpos - vodorovna souradnice policka
 * @param ypos - svisla souradnice policka
 * @param color - barva figurky
 */
void generateFigure(QGraphicsScene *scene, int xpos, int ypos, int color) {
    game->getFiguresVector()->push_back(new Figure(color));
    MapField* indexedField = fieldsMap->getFields().at(xpos * fieldsMap->getSize() + ypos);
    game->getFiguresVector()->back()->getItem()->setPos(300 - graphics->getOffset() + xpos * 40 + 12, 300 - graphics->getOffset() + ypos * 40 + 2);
    scene->addItem(game->getFiguresVector()->back()->getItem());
}

/**
 * @brief pohyb figurky po mape.
 * @param akce pohybu.
 */
void Game::keyPressEvent(QKeyEvent *event) {
    if(game->isGameStarted()) {
        if(fieldsMap->getShiftedState()) {
            switch (event->key()) {
            case Qt::Key_Left:
                mapControl->MakeMove(LEFT, fieldsMap->getPlayers()->GetActivePlayer(), fieldsMap);
                break;
            case Qt::Key_Right:
                mapControl->MakeMove(RIGHT, fieldsMap->getPlayers()->GetActivePlayer(), fieldsMap);
                break;
            case Qt::Key_Up:
                mapControl->MakeMove(UP, fieldsMap->getPlayers()->GetActivePlayer(), fieldsMap);
                break;
            case Qt::Key_Down:
                mapControl->MakeMove(DOWN, fieldsMap->getPlayers()->GetActivePlayer(), fieldsMap);
                break;
            }
        }
        if(mapControl->GameEnded()) {
            gameEndedScreen();
        }
        panel->resetValues();
	if(!mapControl->GameEnded()) {
            panel->setValues();
	}
	else {
	    delete mapControl;
	}
        makeDesk();
    }
}

/**
 * @brief vraci vektor figurek.
 * @return vraci figurky.
 */
vector<Figure *> *Game::getFiguresVector() {
    return &figuresVector;
}

/**
 * @brief vykresli mapu + poklady + figurky.
 */
void Game::makeDesk() {
   refreshMissingPiece();
   mapper = new QSignalMapper(game);
   QObject::connect(mapper, SIGNAL(mapped(int)), this, SLOT(shiftB(int)));
   for(auto &descr : fieldsMap->getFields()) {
       if (descr != fieldsMap->getFields().back())
           generateField(gameScene, descr);
   }
   for(auto &pldescr: fieldsMap->getPlayers()->getPList()) {
       generateFigure(gameScene, pldescr->getXPos(), pldescr->getYPos(), pldescr->getColor());
   }
   fieldsMap->getFields().clear();
}

/**
 * @brief vygeneruje policko a sipky na mackani.
 * @param scena na kterou se ma generovat.
 * @param policko, ktere se ma vygenerovat.
 */
void generateField(QGraphicsScene *scene, MapField* field) {
    QGraphicsPixmapItem *path = scene->addPixmap(QPixmap(QString(getPathReference(field->getShape()))));
    if(field->getXPos() == 0 && field->getYPos() % 2) {
        graphics->getArrows()->push_back(new Arrow(":/other/img/other/arrownohover.png", ":/other/img/other/arrowhover.png", 0, field->getXPos(), - 11, field->getYPos(), 0, graphics->getOffset(), game->gameScene));
        mapper->setMapping(graphics->getArrows()->back(), field->getXPos() * fieldsMap->getSize() + field->getYPos());
        QObject::connect(graphics->getArrows()->back(), SIGNAL(clicked()), mapper, SLOT(map()));
    }
    else if (field->getXPos() == fieldsMap->getSize() - 1 && field->getYPos() % 2) {
        graphics->getArrows()->push_back(new Arrow(":/other/img/other/arrownohover.png", ":/other/img/other/arrowhover.png", 2, field->getXPos(), field->getSize() - 1, field->getYPos(), 0, graphics->getOffset(), game->gameScene));
        mapper->setMapping(graphics->getArrows()->back(), field->getXPos() * fieldsMap->getSize() + field->getYPos());
        QObject::connect(graphics->getArrows()->back(), SIGNAL(clicked()), mapper, SLOT(map()));
    }
    else if (field->getXPos() % 2 && field->getYPos() == 0) {
        graphics->getArrows()->push_back(new Arrow(":/other/img/other/arrownohover.png", ":/other/img/other/arrowhover.png", 1, field->getXPos(), field->getSize() - 26, field->getYPos(), -25, graphics->getOffset(), game->gameScene));
        mapper->setMapping(graphics->getArrows()->back(), field->getXPos() * fieldsMap->getSize() + field->getYPos());
        QObject::connect(graphics->getArrows()->back(), SIGNAL(clicked()), mapper, SLOT(map()));
    }
    else if (field->getXPos() % 2 && field->getYPos() == fieldsMap->getSize() - 1) {
        graphics->getArrows()->push_back(new Arrow(":/other/img/other/arrownohover.png", ":/other/img/other/arrowhover.png", 3, field->getXPos(), field->getSize() - 26, field->getYPos(), 25, graphics->getOffset(), game->gameScene));
        mapper->setMapping(graphics->getArrows()->back(), field->getXPos() * fieldsMap->getSize() + field->getYPos());
        QObject::connect(graphics->getArrows()->back(), SIGNAL(clicked()), mapper, SLOT(map()));
    }
    path->setTransformOriginPoint(19,19);
    path->setRotation(field->getRotation() * 90);
    path->setPos(300 + field->getXPos() * field->getSize() - graphics->getOffset(), 300 + field->getYPos() * field->getSize() - graphics->getOffset());
    if(field->getTreasure() != 0) {
        QGraphicsPixmapItem *item = scene->addPixmap(QPixmap(QString(getTreasureIcon(field->getTreasure()))));
        if (field->getTreasure() == 7 or field->getTreasure() == 8 or field->getTreasure() == 11 or field->getTreasure() == 12 or field->getTreasure() == 19 or field->getTreasure() == 20 or field->getTreasure() == 23 or field->getTreasure() == 24) {
            item->setPos(311 + field->getXPos() * field->getSize() - graphics->getOffset(), 311 + field->getYPos() * field->getSize() - graphics->getOffset());
        }
        else if (field->getTreasure() == 9 or field->getTreasure() == 10 or field->getTreasure() == 21 or field->getTreasure() == 22) {
            item->setPos(309 + field->getXPos() * field->getSize() - graphics->getOffset(), 310 + field->getYPos() * field->getSize() - graphics->getOffset());
        }
        else {
            item->setPos(309 + field->getXPos() * field->getSize() - graphics->getOffset(), 308 + field->getYPos() * field->getSize() - graphics->getOffset());
        }
    }
}

/**
 * @brief vraci vektor sipek, na ktere se klika pro posunuti mapy.
 * @return vrati vektor sipek.
 */
QVector<Arrow *> *QtGraphics::getArrows() {
    return &arrows;
}

/**
 * @brief aktualizuje offset pro posunuti od stredu - mapa je na stredu pro 5x5 i 7x7.
 */
void QtGraphics::actualizeOffset() {
    offset = (fieldsMap->getSize() * 38 + (fieldsMap->getSize() - 1) * 4 ) / 2;
}

/**
 * @brief vrati offset posunuti, aby mapa byla na stredu pro vsechny velikosti.
 * @return vraci offset.
 */
int QtGraphics::getOffset() {
    return offset;
}
