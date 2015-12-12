#-------------------------------------------------
#
# Project created by QtCreator 2015-11-09T19:21:30
#
#-------------------------------------------------

QT       += core gui

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = GitRelations
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    repositorywidget.cpp \
    paintwindow.cpp \
    node.cpp \
    graphicsviewzoom.cpp \
    treenode.cpp \
    contributer.cpp \
    macros.cpp \
    leafnode.cpp \
    forcelayout.cpp

HEADERS  += mainwindow.h \
    repositorywidget.h \
    paintwindow.h \
    node.h \
    graphicsviewzoom.h \
    treenode.h \
    contributer.h \
    macros.h \
    leafnode.h \
    forcelayout.h

FORMS    += mainwindow.ui \
    repositorywidget.ui \
    paintwindow.ui

unix:!macx: LIBS += -L$$PWD -lgitcpp -lgit2 -lcgraph -lgvc

INCLUDEPATH += $$PWD/../libgitcpp/include
DEPENDPATH += $$PWD/../libgitcpp/include
