/**
 * @file robots.cpp
 * @author Yaroslav Slabik  (xslabi01)
 * @author Kininbayev Timur (xkinin00)
 * @brief File containing the robot class logic
 */
#include <cmath>  // For cos() and sin()
#include <QGraphicsItem>
#include <QPainter>
#include "mainwindow.h"
#include "qgraphicsscene.h"
#include "qgraphicsview.h"
#include "obstacle.h"
#include "qgraphicssceneevent.h"

/**
 * @brief delete the robot if the left mouse button is pressed and the deleting mode is active
 * 
 * @param event 
 */
void AutonomousRobot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        auto scene = this->scene();
        if (!scene) return;

        auto views = scene->views();
        if (views.isEmpty()) return;

        QGraphicsView* view = views.first(); // take the first view
        if (!view) return;

        MainWindow *mainWindow = dynamic_cast<MainWindow *>(view->window());
        if (!mainWindow) return; // check if mainWindow exists

        if (mainWindow->isDeletingModeActive()) {
            scene->removeItem(this);
            delete this; // delete the robot
        }
    }
}

/**
 * @brief select or delete the remote controlled robot if the left mouse button is pressed
 * 
 * @param event mouse event
 */
void RemoteRobot::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        auto scene = this->scene();
        if (!scene) return;

        auto views = scene->views();
        if (views.isEmpty()) return;

        QGraphicsView* view = views.first();
        if (!view) return;

        MainWindow *mainWindow = dynamic_cast<MainWindow *>(view->window());
        if (!mainWindow) return;

        if (mainWindow->isDeletingModeActive()) {
            scene->removeItem(this);
            delete this;
        } else {
            mainWindow->selectRobot(this);  // Теперь передаем this правильного типа
        }
    }
}

/**
 * @brief handle the autonomous robot movement
 * 
 */
void AutonomousRobot::move() {
    double radAngle = orientation * M_PI / 180;
    double dx = speed * cos(radAngle);
    double dy = speed * sin(radAngle);

    positionX += dx;
    positionY += dy;

    // Use QGraphicsItem's setPos to handle updating the scene position
    setPos(positionX, positionY);
}

/**
 * @brief detect obstacles in the robot's path
 * 
 * @return true when an obstacle is detected
 * @return false when no obstacles are detected
 */
bool AutonomousRobot::detectObstacle() {
    qDebug() << "Checking for obstacles for robot at position" << positionX << positionY;

    double radAngle = orientation * M_PI / 180;
    double dx = detectionRadius * cos(radAngle);
    double dy = detectionRadius * sin(radAngle);

    QRectF robotFuturePosition(positionX + dx, positionY + dy, 20, 20);
    QGraphicsScene* currentScene = scene();
    QRectF sceneRect = currentScene->sceneRect();

    if (!sceneRect.contains(robotFuturePosition)) {
        qDebug() << "Obstacle detected: out of scene bounds";
        return true;
    }

    // calculate the correct detection area
    QPainterPath detectionPath;
    QPointF frontCenter = QPointF(positionX + cos(radAngle) * 20, positionY + sin(radAngle) * 20); // 20 - robot's radius
    detectionPath.moveTo(frontCenter);
    detectionPath.addRect(QRectF(frontCenter.x(), frontCenter.y(), detectionRadius, 20));

    detectionPath.addRect(QRectF(positionX, positionY, detectionRadius, 20));
    QList<QGraphicsItem*> collidingItems = scene()->items(detectionPath);

    foreach(QGraphicsItem *item, collidingItems) {
        if (item != this && (dynamic_cast<Obstacle*>(item) || dynamic_cast<Robot*>(item))) {
            qDebug() << "Obstacle detected: collision with an obstacle";
            return true;
        }
    }

    qDebug() << "No obstacles detected";
    return false;
}

/**
 * @brief handling the remote robot movement
 * If obstacle is detected, the robot stops
 */
void RemoteRobot::moveForward() {
    if(detectObstacle()){
        isMoving = false;
        return;
    }

    double radAngle = orientation * M_PI / 180;
    double dx = speed * cos(radAngle);
    double dy = speed * sin(radAngle);

    positionX += dx;
    positionY += dy;

    isMoving = true;

    setPos(positionX, positionY);
}

/**
 * @brief rotate the robot to the right
 * @todo implement robot rotation not only by 90 degrees
 */
void RemoteRobot::rotateRight() {
    orientation = (orientation + 90) % 360;
}

/**
 * @brief rotate the robot to the left
 * @todo implement robot rotation not only by 90 degrees
 */
void RemoteRobot::rotateLeft() {
    orientation = (orientation - 90 + 360) % 360;
}

/**
 * @brief stop the robot
 * 
 */
void RemoteRobot::stop(){
    isMoving = false;
}

/**
 * @brief detect obstacles in the robot's path
 * 
 * @return true if an obstacle is detected
 * @return false if no obstacles are detected
 */
bool RemoteRobot::detectObstacle(){
    qDebug() << "Checking for obstacles for robot at position" << positionX << positionY;

    double radAngle = orientation * M_PI / 180;
    double dx = detectionRadius * cos(radAngle);
    double dy = detectionRadius * sin(radAngle);

    QRectF robotFuturePosition(positionX + dx, positionY + dy, 20, 20);
    QGraphicsScene* currentScene = scene();
    QRectF sceneRect = currentScene->sceneRect();

    if (!sceneRect.contains(robotFuturePosition)) {
        qDebug() << "Obstacle detected: out of scene bounds";
        return true;
    }

    // Рассчитаем правильную область обнаружения
    QPainterPath detectionPath;
    QPointF frontCenter = QPointF(positionX + cos(radAngle) * 20, positionY + sin(radAngle) * 20); // 20 - это предполагаемый "радиус" робота
    detectionPath.moveTo(frontCenter);
    detectionPath.addRect(QRectF(frontCenter.x(), frontCenter.y(), detectionRadius, 20)); // Эта область теперь корректно ориентирована

    detectionPath.addRect(QRectF(positionX, positionY, detectionRadius, 20));
    QList<QGraphicsItem*> collidingItems = scene()->items(detectionPath);

    foreach(QGraphicsItem *item, collidingItems) {
        if (item != this && (dynamic_cast<Obstacle*>(item) || dynamic_cast<Robot*>(item))) {
            qDebug() << "Obstacle detected: collision with an obstacle";
            return true;
        }
    }

    qDebug() << "No obstacles detected";
    return false;
}
