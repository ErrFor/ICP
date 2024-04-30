#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPushButton* buttons[2] = { ui->pushButton_4, ui->pushButton_5};

    buttons[0]->setStyleSheet("QPushButton { background-color: green; }");
    buttons[1]->setStyleSheet("QPushButton { background-color: red; }");
}

MainWindow::~MainWindow()
{
    // Деструктор
}
