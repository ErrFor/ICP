/**
 * @file mainwindow.cpp
 * @brief Main file for handling output logic
 * @author Kininbayev Timur (xkinin00)
 * @author Yaroslav Slabik (xslabi01)
 */

#include "mainwindow.h"
#include "obstacle.h"
#include "createobstacledialog.h"
#include "createRobotDialog.h"
#include "ui_mainwindow.h"
#include <QGraphicsScene>
#include <QDebug>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <stdio.h>
#include <QGraphicsDropShadowEffect>

/**
 * @brief Construct a new Main Window:: Main Window object
 *
 * @param parent
 */
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
    connect(ui->moveRobotButton, &QPushButton::clicked, this, &MainWindow::moveRobot);
    connect(ui->rotateRightButton, &QPushButton::clicked, this, &MainWindow::rotateRobotRight);
    connect(ui->rotateLeftButton, &QPushButton::clicked, this, &MainWindow::rotateRobotLeft);
    connect(ui->stopRobotButton, &QPushButton::clicked, this, &MainWindow::stopRobot);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopSimulation);

    ui->startButton->setStyleSheet("QPushButton { background-color: green; }");
    ui->stopButton->setStyleSheet("QPushButton { background-color: red; }");

    // create scene
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 1500, 600);
    scene->setBackgroundBrush(QBrush(QColor(51,51,51,200)));

    // create widget and link with scene
    ui->graphicsView->setScene(scene);

    // turn of scrollbars
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // ensure that the view stays centered on resize
    ui->graphicsView->setResizeAnchor(QGraphicsView::AnchorViewCenter);
    ui->graphicsView->setTransform(QTransform());

    // set deleting mode to false
    deletingMode = false;
    rDeletingMode = false;

    timer = new QTimer(this);
    timer->setInterval(10); // update every 10 ms
    connect(timer, &QTimer::timeout, this, &MainWindow::updateRobots);

    timer->stop();
}

MainWindow::~MainWindow()
{
    // Деструктор
}

/**
 * @brief Starts or continues the simulation
 *
 */
void MainWindow::startSimulation() {
    if (!timer->isActive()) {
        timer->start();
    }
}

/**
 * @brief Stops the simulation
 *
 */
void MainWindow::stopSimulation() {
    if (timer->isActive()) {
        timer->stop();
    }
}

/**
 * @brief Create obstacle
 *
 */
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

/**
 * @brief Delete obstacle
 *
 */
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

/**
 * @brief Create robot
 * create robot dialog where user can set robot type, position, orientation, speed and detection radius
 * @todo change update logic so robots won't change their position when new robot is created
 */
void MainWindow::on_createRobotButton_clicked() {
    CreateRobotDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int robotType = dialog.getRobotType();
        int orientation = dialog.getOrientation();
        int speed = dialog.getSpeed();
        double detectionRadius = dialog.getDetectionRadius();
        double x = dialog.getX();
        double y = dialog.getY();

        if (robotType == 0) {  // Autonomous
            double avoidanceAngle = dialog.getAvoidanceAngle();
            AutonomousRobot *robotItem = new AutonomousRobot(x, y, orientation, detectionRadius, avoidanceAngle, speed);
            autonomousRobots.append(robotItem);
            ui->graphicsView->scene()->addItem(robotItem);
        } else {  // Remote Controlled
            RemoteRobot *remoteRobotItem = new RemoteRobot(x, y, speed, detectionRadius);
            remoteRobots.append(remoteRobotItem);
            ui->graphicsView->scene()->addItem(remoteRobotItem);
        }
    }
    updateRobots();
}

/**
 * @brief Delete robot
 * @todo implement robot deletion
 */
void MainWindow::on_deleteRobotButton_clicked()
{
    // set deletingmode flag
    rDeletingMode = !rDeletingMode;
    if (rDeletingMode) {
        ui->deleteRobot->setStyleSheet("QPushButton { background-color: red; }");
    } else {
        ui->deleteRobot->setStyleSheet("QPushButton { backgorund-color: white; }");
    }
}

/**
 * @brief select remote controlled robot
 *
 * @param robot pointer to remote controlled robot
 */
void MainWindow::selectRobot(RemoteRobot* robot) {
    selectedRobot = robot;  // save selected robot
}

/**
 * @brief move remote controlled robot to it's actual destination
 *
 */
void MainWindow::moveRobot() {
    if (selectedRobot && selectedRobot->scene() && timer->isActive()) {  // Check if selectedRobot is still in the scene
        selectedRobot->moveForward();
    }
}

/**
 * @brief rotate remote controlled robot to the right
 *
 */
void MainWindow::rotateRobotRight() {
    if (selectedRobot && timer->isActive()) {
        selectedRobot->rotateRight();
    }
}

/**
 * @brief rotate remote controlled robot to the left
 *
 */
void MainWindow::rotateRobotLeft() {
    if (selectedRobot && timer->isActive()) {
        selectedRobot->rotateLeft();
    }
}

/**
 * @brief stop remote controlled robot
 *
 */
void MainWindow::stopRobot() {
    if (selectedRobot) {
        selectedRobot->stop();
    }
}

/**
 * @brief Update robots positions
 * Updates all robots in the scene
 */
void MainWindow::updateRobots() {
    ui->graphicsView->scene()->update();
    for (Robot* robot : autonomousRobots) {  // go through all autonomous robots
        robot->update();
    }
    for (Robot* robot: remoteRobots) { // go through all remote controlled robots
        RemoteRobot* remoteRobot = dynamic_cast<RemoteRobot*>(robot);
        if (remoteRobot->getRotationDirection() == RemoteRobot::RotateRight) {
            rotateRobotRight();
        } else if (remoteRobot->getRotationDirection() == RemoteRobot::RotateLeft) {
            rotateRobotLeft();   // Обработка поворота влево
        }
        if(remoteRobot->isMoving){
            remoteRobot->update();
        }
    }
}

void MainWindow::loadSceneFromFile(const QString& filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open file for reading:" << filename;
        return;
    }

    QTextStream in(&file);
    QString buffer;
    QString currentType;
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith("#")) {
            continue; // Пропускаем пустые строки и комментарии
        }

        if (line.endsWith("{")) {
            // Начало нового объекта
            currentType = line.left(line.length() - 1).trimmed();
            buffer.clear();
        } else if (line.startsWith("}")) {
            // Конец описания объекта
            if (!currentType.isEmpty()) {
                processObject(currentType, buffer);
                currentType.clear();
            }
        } else {
            buffer += line + "\n";
        }
    }
}

void MainWindow::processObject(const QString& type, const QString& attributes) {
    QMap<QString, QString> params = parseAttributes(attributes);
    qDebug() << "params: " << params;
    int x = params.value("positionX").toInt();
    qDebug() << "X: " << x;
    int y = params.value("positionY").toInt();
    int speed = params.value("speed").toInt();
    int orientation = params.value("orientation").toInt();
    int detectionRadius = params.value("detectionRadius").toInt();
    int avoidanceAngle = params.value("avoidanceAngle").toInt();
    int size = params.value("width").toInt();

    if (type == "AutonomousRobot") {
        AutonomousRobot *robotItem = new AutonomousRobot(x, y, orientation, detectionRadius, avoidanceAngle, speed);
        autonomousRobots.append(robotItem);
        ui->graphicsView->scene()->addItem(robotItem);
    } else if (type == "RemoteRobot") {
        RemoteRobot *remoteRobotItem = new RemoteRobot(x, y, speed, detectionRadius);
        remoteRobots.append(remoteRobotItem);
        ui->graphicsView->scene()->addItem(remoteRobotItem);
    } else if (type == "Obstacle"){
        Obstacle *obstacle = new Obstacle(x, y, size);
        ui->graphicsView->scene()->addItem(obstacle);
    }
    else {
        qDebug() << "Unknown object type:" << type;
    }
}

QMap<QString, QString> MainWindow::parseAttributes(const QString& attributes) {
    QMap<QString, QString> params;
    QStringList lines = attributes.split("\n", Qt::SkipEmptyParts);
    for (const QString& line : lines) {
        QStringList parts = line.split("=");
        if (parts.size() == 2) {
            QString key = parts[0].trimmed();
            QString value = parts[1].trimmed();
            params[key] = value;
        }
    }
    return params;
}



