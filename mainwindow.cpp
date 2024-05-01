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

    // slots connections
    connect(ui->createObstacle, &QPushButton::clicked, this, &MainWindow::on_createObstacleButton_clicked);
    connect(ui->deleteObstacle, &QPushButton::clicked, this, &MainWindow::on_deleteObstacleButton_clicked);
    connect(ui->createRobot, &QPushButton::clicked, this, &MainWindow::on_createRobotButton_clicked);
    connect(ui->deleteRobot, &QPushButton::clicked, this, &MainWindow::on_deleteRobotButton_clicked);
    connect(ui->deleteRobot, &QPushButton::clicked, this, &MainWindow::on_deleteRobotButton_clicked);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopSimulation);

    ui->startButton->setStyleSheet("QPushButton { background-color: green; }");
    ui->stopButton->setStyleSheet("QPushButton { background-color: red; }");

    // create scene
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1500, 600);
    scene->setBackgroundBrush(QBrush(Qt::gray));

    // create widget and link with scene
    ui->graphicsView->setScene(scene);

    // turn of scrollbars
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // Включение масштабирования, если необходимо
    ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    // Убедитесь, что масштабирование отключено или правильно настроено
    ui->graphicsView->setTransform(QTransform());

    deletingMode = false;

    timer = new QTimer(this);
    timer->setInterval(200); // Обновление каждые 200 мс
    connect(timer, &QTimer::timeout, this, &MainWindow::updateRobots);

    timer->stop();
}

MainWindow::~MainWindow()
{
    // Деструктор
}

void MainWindow::startSimulation() {
    if (!timer->isActive()) {
        timer->start();
    }
}

void MainWindow::stopSimulation() {
    if (timer->isActive()) {
        timer->stop();
    }
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
        int speed = dialog.getSpeed();
        if (robotType == 0) {  // Autonomous
            double detectionRadius = dialog.getDetectionRadius();
            double avoidanceAngle = dialog.getAvoidanceAngle();
            AutonomousRobot *robotItem = new AutonomousRobot(x, y, orientation, detectionRadius, avoidanceAngle, speed);
            robots.append(robotItem);
            ui->graphicsView->scene()->addItem(robotItem);
        } else {  // Remote Controlled
//            robotItem = new RemoteControlledRobot(x, y, orientation, speed);
        }


    }
    updateRobots();
}

void MainWindow::on_deleteRobotButton_clicked()
{
    // set deletingmode flag
    deletingMode = !deletingMode;

    // Меняем визуальное отображение для указания режима удаления
    if (deletingMode) {
        foreach (QGraphicsItem *item, ui->graphicsView->scene()->items()) {
            AutonomousRobot *robotItem = dynamic_cast<AutonomousRobot*>(item);
        }
    }
}

void MainWindow::updateRobots() {
    for (Robot* robot : robots) {  // Перебор всех роботов в списке
        robot->update();
        ui->graphicsView->scene()->update();  // Обновление сцены, если требуется
    }
}


