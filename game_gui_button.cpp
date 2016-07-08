/**
  Autori: Lukas Dibdak, xdibda00
  Modul: GameGUI - Button
  Popis modulu: Tento modul implementuje hlavni rozhrani pro
  GUI verzi hry - graficke vykresleni.
  Upresneni specializace modulu: Tato cast se stara o logiku
  a grafiku klikaciho tlacitka, ktere je pouzito u sipek a
  klikacich tlacitek.
  */

#include "game_gui.h"
#include <vector>

extern Map* fieldsMap;
extern QtGraphics* graphics;

/**
 * @brief emituje se akce clicked() pokud bylo kliknuto na mysi.
 * @param akce kliknuti.
 */
void Button::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked();
}

/**
 * @brief pokud bylo najeto mysi na tlacitko.
 * @param akce prejeti.
 */
void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    setPixmap(QPixmap(*Hover));
}

/**
 * @brief pokud bylo z tlacitko sjeto pryc.
 * @param akce odjeti pryc.
 */
void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    setPixmap(QPixmap(*noHover));
}

/**
 * @brief nastavi defaultni hodnoty a grafiku k nim pro novou hru (vyberova obrazovka).
 */
void Button::setDefault() {
    setPixmap(QPixmap(QString(*click)));
    *noHover = *click;
    *Hover = *click;
    if(this->getID() == 2 or this->getID() == 3 or this->getID() == 4) {
        fieldsMap->setPlayers(this->getID());
    }
    else if(this->getID() == 12 or this->getID() == 24) {
        fieldsMap->setCardsSet(this->getID());
    }
    else {
        fieldsMap->setSize(this->getID());
        graphics->actualizeOffset();
    }
}

/**
 * @brief vraci typ tlacitka - jeho ID cislo.
 * @return toto cislo.
 */
int Button::getID() {
    return this->ident;
}

/**
 * @brief konstruktor pro klikaci tlacitko.
 * @param puvodni grafika tlacitka.
 * @param grafika po najeti na tlacitko.
 * @param vodorovna souradnice kam se ma vlozit.
 * @param offset ve vodorovne souradnici.
 * @param svisla souradnice, kam se ma vlozit.
 * @param offset ve svisle souradnici.
 * @param celkovy offset.
 * @param rotace tlacitka.
 * @param scena na jakou se vlozi.
 */
Button::Button(QString name, QString hov, int x, unsigned int dx, int y, unsigned int dy, int offset, int rotation, QGraphicsScene *scene) {
    setPos(300 + 40 * x - offset + dx, 300 + 40 * y - offset + dy);
    setPixmap(QPixmap(name));
    setTransformOriginPoint(5,19);
    setRotation(rotation * 90);
    setAcceptHoverEvents(true);
    scene->addItem(this);
    setFlag(QGraphicsItem::ItemIsFocusable);

    noHover = new QString(name);
    Hover = new QString(hov);
    pom = new QString();
}

/**
 * @brief sekundarni konstruktor tlacitka.
 * @param puvodni grafika tlacitka.
 * @param vodorovna souradnice tlacitka.
 * @param svisla souradnice tlacitka.
 * @param scena, kam se ma tlacitko vlozit.
 * @param grafika, pokud na tlacitko nekdo najede.
 * @param grafika, pokud na tlacitko nekdo klikne.
 * @param id tlacitka.
 * @param parent.
 */
Button::Button(QString name, int x, int y, QGraphicsScene *scene, QString hov, QString clickS, int identf, QWidget *parent) : QWidget(parent), QGraphicsPixmapItem() {
    setPos(x, y);
    setPixmap(QPixmap(name));
    setAcceptHoverEvents(true);
    scene->addItem(this);

    ident = identf;
    nohover = new QString(name);
    hover = new QString(hov);
    noHover = new QString(name);
    Hover = new QString(hov);
    click = new QString(clickS);
    pom = new QString();
}
