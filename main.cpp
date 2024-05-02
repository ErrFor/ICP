/**
 * @file main.cpp
 * @author Kininbayev Timur (xkinin00)
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
    w.show();
    return a.exec();
}
