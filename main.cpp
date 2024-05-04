/**
 * @file main.cpp
 * @author Yaroslav Slabik (xslabi01)
 * @brief File containing the main function 
 * 
 */
#include "mainwindow.h"

#include <QApplication>

/**
 * @brief entry point of the application
 * 
 * @param argc
 * @param argv 
 * @return int 
 */
int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    if (argc > 1) {  // check if filename entered
            QString filename = argv[1];  // take first arg as filename
            w.loadSceneFromFile(filename);
    }
    w.show();
    return a.exec();
}
