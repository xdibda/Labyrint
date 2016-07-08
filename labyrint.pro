#-------------------------------------------------
#
# Project created by QtCreator 2015-04-20T14:28:11
#
#-------------------------------------------------

QT       += core gui \
            multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = labyrinth2015
TEMPLATE = app


SOURCES += \
    map.cpp \
    players.cpp \
    cardpack.cpp \
    controller.cpp \
    log.cpp \
    main_gui.cpp \
    game_gui.cpp \
    game_gui_button.cpp \
    game_gui_graph.cpp \
    game_gui_slots.cpp

HEADERS  += \
    map.h \
    players.h \
    cardpack.h \
    controller.h \
    log.h \
    game_gui.h \

QMAKE_CXXFLAGS += -std=c++0x

RESOURCES += \
    icplib.qrc
