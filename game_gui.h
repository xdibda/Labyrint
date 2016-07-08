/**
  Autori: Lukas Dibdak, xdibda00
  Modul: GameGUI
  Popis modulu: Tento modul implementuje hlavni rozhrani pro
  GUI verzi hry - graficke vykresleni.
  */

#ifndef GAME
#define GAME

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsTextItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QWidget>
#include <QString>
#include <QImage>
#include <QVector>
#include <QCheckBox>
#include <QFont>
#include <QBrush>
#include <QSignalMapper>
#include <QKeyEvent>

#include "controller.h"

/** Generovani objektu do mapy. */
/**
 * @brief generateFigure - funkce vygeneruje figurku a vlozi ji do mapy na dane policko
 * @param scene - scena, na jakou se ma figurka vlozit
 * @param xpos - vodorovna souradnice policka
 * @param ypos - svisla souradnice policka
 * @param color - barva figurky
 */
void generateFigure(QGraphicsScene *scene, int xpos, int ypos, int color);
/**
 * @brief vygeneruje policko a sipky na mackani.
 * @param scena na kterou se ma generovat.
 * @param policko, ktere se ma vygenerovat.
 */
void generateField(QGraphicsScene *scene, MapField* field);

/** Grafika k figurkam. */
/**
 * @brief vraci grafiku k figurky.
 * @param barva figurky.
 * @return vraci adresu k figurce.
 */
QString getFigureReferenceBig(int color);
/**
 * @brief vraci grafiku figurky.
 * @param barva figurky.
 * @return adresa grafiky figurky.
 */
QString getFigureReference(int color);
/**
 * @brief vraci malou figurku v menu.
 * @param barva figurky.
 * @return vraci adresu zmensene figurky.
 */
QString getFigureReferenceMenu(int color);

/** Grafika k pokladum. */
/**
 * @brief vraci obrazek pokladu.
 * @param id pokladu.
 * @return adresa grafiky pokladu.
 */
QString getTreasureIcon(int treasureID);
/**
 * @brief vraci grafiku velkeho pokladu do menu.
 * @param id pokladu.
 * @return vraci grafiku pokladu do menu.
 */
QString getTreasureBig(int treasureID);

/** Grafika k cestam. */
/**
 * @brief vraci grafiku cesty
 * @param znak cesty - T, L, I.
 * @return vraci adresu grafiky.
 */
QString getPathReference(char c);

/**
 * @brief trida klikajicich tlacitek.
 */
class Button : public QWidget, public QGraphicsPixmapItem {
    Q_OBJECT
private:
    int ident;
    QString *noHover;
    QString *Hover;
    QString *click;
    QString *pom;
    int getID();
protected:
    QString *hover;
    QString *nohover;
private slots:
    void rotate(QGraphicsPixmapItem *missing_puzzle);
    void hoverClicked();
    void hoverUnclicked();
public:
    Button(QString name, QString hov, int x, unsigned int dx, int y, unsigned int dy, int offset, int rotation, QGraphicsScene *scene);
    Button(QString name, int x = 0, int y = 0, QGraphicsScene *scene = 0, QString hov = 0, QString clickS = 0, int identf = 0, QWidget *parent = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void setDefault();
signals:
    void clicked();
};

/**
 * @brief trida figurek.
 */
class Figure : public QWidget, public QGraphicsPixmapItem {
    Q_OBJECT
private:
    int color;
    QGraphicsPixmapItem *item;
public:
    int getColor();
    Figure(int c);
    QGraphicsPixmapItem *getItem();
};

/**
 * @brief trida klikajicich sipek.
 */
class Arrow : public Button, public MapField {
    Q_OBJECT
public:
    Arrow(QString name, QString hover, int rot, int x, unsigned int dx, int y, unsigned int dy, int offset, QGraphicsScene *scene) : Button(name, hover, x, dx, y, dy, offset, rot, scene) {};
signals:
    void arrowClicked(int x);
};

/**
 * @brief trida grafiky, ktera je potrebna pro vykresleni.
 */
class QtGraphics {
private:
    int offset;
    int figureX;
    int figureY;
    QVector<Arrow *> arrows;
public:
    QVector<Arrow *> *getArrows();
    void actualizeOffset();
    int getOffset();
};

/**
 * @brief bocni panel hrace.
 */
class SidePanel : public QGraphicsView {
public:
    void resetValues();
    void setValues();
    QGraphicsPixmapItem *playersIcon;
    QGraphicsPixmapItem *playersItem;
    QGraphicsTextItem *item;
};

/**
 * @brief hlavni trida zastupujici celou logiku hry.
 */
class Game : public QGraphicsView {
    Q_OBJECT // Pro praci se slotama;
private:
    bool gameStarted;
    SidePanel* panel; // bocni informacni panel;
    MapField* missingPiece;
    //Controller* mapControl;
    QGraphicsScene *scene;
    QGraphicsScene *authors;
    QGraphicsScene *newgame;
    QGraphicsScene *endgame;
    vector<Figure *> figuresVector;
public:
    Game();
    MapField* getMissingPiece();
    QString getFigureReferenceBig(int color);
    SidePanel *getPanel();
    QGraphicsScene *gameScene;
    void keyPressEvent(QKeyEvent *event);
    vector<Figure *> *getFiguresVector();
    void setMissingPiece(MapField* field);
    bool isGameStarted();
    void makeDesk();
    void displayMenu();
    void writeSideway();
    void drawGUI();
    void moveFigure(int color, MapField *indexedMapField);
    void drawPanel(int x, int y, int width, int height, QColor color, double opacity, QGraphicsScene *scene);
    void refreshMissingPiece();
    void gameEndedScreen();
public slots:
    void saveGame();
    void loadGame();
    void nextPlayer();
    void choose();
    void start();
    void ret_mm();
    void ret_game();
    void authors_view();
    void shiftB(int number);
    void rotate();
    void stepBack();
};

#endif // GAME
