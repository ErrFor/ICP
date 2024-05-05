QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simulation
TEMPLATE = app


FORMS += mainwindow.ui

SOURCES += main.cpp\
           mainwindow.cpp\
           createobstacledialog.cpp\
           createRobotDialog.cpp\
           obstacle.cpp\
           robots.cpp

HEADERS += mainwindow.h\
           obstacle.h\
           createobstacledialog.h\
           createRobotDialog.h\
           robots.h
