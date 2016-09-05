#-------------------------------------------------
#
# Project created by QtCreator 2016-09-05T09:56:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = plotClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mynetwork.cpp

HEADERS  += mainwindow.h \
    mynetwork.h

FORMS    += mainwindow.ui

QT += network
