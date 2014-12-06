#-------------------------------------------------
#
# Project created by QtCreator 2014-11-30T01:41:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GAL
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    edge.cpp \
    node.cpp \
    graphscene.cpp \
    textvalue.cpp

HEADERS  += mainwindow.h \
    edge.h \
    node.h \
    graphscene.h \
    textvalue.h

FORMS    += mainwindow.ui

RESOURCES += \
    MyResources.qrc
