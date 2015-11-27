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
    paintwindow.cpp

HEADERS  += mainwindow.h \
    repositorywidget.h \
    paintwindow.h

FORMS    += mainwindow.ui \
    repositorywidget.ui \
    paintwindow.ui

unix:!macx: LIBS += -L$$PWD -lgitcpp

INCLUDEPATH += $$PWD/../libgitcpp/include
DEPENDPATH += $$PWD/../libgitcpp/include
