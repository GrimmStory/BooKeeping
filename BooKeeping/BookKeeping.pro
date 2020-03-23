#-------------------------------------------------
#
# Project created by QtCreator 2019-11-22T11:29:38
#
#-------------------------------------------------

QT       += core gui\
        core\
       gui\
       network\
       widgets\
       sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BookKeeping
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    logindialog.cpp \
    global.cpp \
    cvlink.cpp

HEADERS  += mainwindow.h \
    logindialog.h \
    global.h \
    cvlink.h

FORMS    += mainwindow.ui \
    logindialog.ui

RESOURCES += \
    bookkeeping.qrc
