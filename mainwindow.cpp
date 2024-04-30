/**
 * @file mainwindow.cpp
 * @brief Main file for handling output logic
 * @author xkinin00
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "obstacle.h"
#include "createobstacledialog.h"
#include "createRobotDialog.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QTimer>
#include <stdio.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // В конструкторе MainWindow, после инициализации ui
    connect(ui->createObstacle, &QPushButton::clicked, this, &MainWindow::on_createObstacleButton_clicked);
    connect(ui->deleteObstacle, &QPushButton::clicked, this, &MainWindow::on_deleteObstacleButton_clicked);
    connect(ui->createRobot, &QPushButton::clicked, this, &MainWindow::on_createRobotButton_clicked);

    QPushButton* buttons[2] = { ui->pushButton_4, ui->pushButton_5};

    buttons[0]->setStyleSheet("QPushButton { background-color: green; }");
    buttons[1]->setStyleSheet("QPushButton { background-color: red; }");

// Создаем сцену
    QGraphicsScene *scene = new QGraphicsScene(this);
    // Устанавливаем размер сцены (например, 800x600)
    scene->setSceneRect(0, 0, 1500, 600);
    scene->setBackgroundBrush(QBrush(Qt::gray));

    // Создаем виджет QGraphicsView и связываем его с сценой
    ui->graphicsView->setScene(scene);

    // Опционально: отключаем скроллбары, если хотите, чтобы вид всегда оставался на всю сцену
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    deletingMode = false;

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateRobots);
    timer->start(100); // Обновление каждые 100 мс
}

MainWindow::~MainWindow()
{
    // Деструктор
}

void MainWindow::on_createObstacleButton_clicked()
{
    CreateObstacleDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int x = dialog.getX();
        int y = dialog.getY();
        int width = dialog.getWidth();

        Obstacle *obstacle = new Obstacle(x, y, width);
        ui->graphicsView->scene()->addItem(obstacle);
    }
}

void MainWindow::on_deleteObstacleButton_clicked()
{
    // set deletingmode flag
    deletingMode = !deletingMode;

    // Меняем визуальное отображение для указания режима удаления
    if (deletingMode) {
        foreach (QGraphicsItem *item, ui->graphicsView->scene()->items()) {
            Obstacle *obstacle = dynamic_cast<Obstacle*>(item);
            if (obstacle) {
                obstacle->setPen(QPen(Qt::red));  // Подсвечиваем препятствия красным
            }
        }
    } else {
        foreach (QGraphicsItem *item, ui->graphicsView->scene()->items()) {
            Obstacle *obstacle = dynamic_cast<Obstacle*>(item);
            if (obstacle) {
                obstacle->setPen(QPen(Qt::black));  // Возвращаем обычный вид
            }
        }
    }
}

void MainWindow::on_createRobotButton_clicked() {
    CreateRobotDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int robotType = dialog.getRobotType();
        double x = dialog.getX();
        double y = dialog.getY();
        int orientation = dialog.getOrientation();
        printf("KAL %d\n", orientation);
        if (robotType == 0) {  // Autonomous
            double detectionRadius = dialog.getDetectionRadius();
            double avoidanceAngle = dialog.getAvoidanceAngle();
            AutonomousRobot *robotItem = new AutonomousRobot(x, y, orientation, detectionRadius, avoidanceAngle);
            robots.append(robotItem);
            ui->graphicsView->scene()->addItem(robotItem);
        } else {  // Remote Controlled
//            robotItem = new RemoteControlledRobot(x, y, orientation, speed);
        }


    }
}

void MainWindow::updateRobots() {
    for (Robot* robot : robots) {  // Перебор всех роботов в списке
        robot->update();
        ui->graphicsView->scene()->update();  // Обновление сцены, если требуется
    }
}


