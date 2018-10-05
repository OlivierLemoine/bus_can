#-------------------------------------------------
#
# Project created by QtCreator 2015-11-27T10:45:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QT_CANBUS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS        += -lpcan

#LIBS        += -LleRepertoireVersLaLibToto -lpcan
#INCLUDEPATH += /usr/local/
