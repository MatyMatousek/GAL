#-------------------------------------------------
#
# Project created by QtCreator 2014-11-30T01:41:43
#
#-------------------------------------------------

include(gal.config)

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GAL
TEMPLATE = app

win32 {
    CONFIG(debug, debug|release) {
        LIBS += $$OGDF_INSTALL_PATH_DEBUG/ogdf.lib
        DEFINES += OGDF_DEBUG
    } else {
        LIBS += $$OGDF_INSTALL_PATH_RELEASE/ogdf.lib
    }
    LIBS += Psapi.lib
}

!win32 {
    CONFIG(debug, debug|release) {
        LIBS += $$OGDF_INSTALL_PATH_DEBUG/libOGDF.a
        DEFINES += OGDF_DEBUG
    } else {
        LIBS += $$OGDF_INSTALL_PATH_RELEASE/libOGDF.a
    }
}

# add ogdf include path
INCLUDEPATH += $$OGDF_INCLUDE_PATH

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
