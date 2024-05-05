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

        if (mainWindow->isRobotDeletingModeActive()) {
            scene->removeItem(this);
            mainWindow->autonomousRobots.removeOne(this);
            delete this; // delete the robot
            scene->update(); // update the scene
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

        if (mainWindow->isRobotDeletingModeActive()) {
            scene->removeItem(this);
            if (mainWindow->selectedRobot == this) {
                mainWindow->selectRobot(nullptr);  // Clear the selected robot if it is the one being deleted
            }
            mainWindow->remoteRobots.removeOne(this);  // Remove from the list
            delete this;  // Delete the object
            scene->update();  // Update the view
            return;  // Exit to avoid further processing since the object is deleted
        }

        if (mainWindow->selectedRobot){
            mainWindow->selectedRobot->setColor(Qt::magenta);
        }
        setColor(Qt::yellow);
        setSelected(true);
        mainWindow->selectRobot(this);
        QGraphicsItem::update();
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

    double radOrientation = orientation * M_PI / 180;
    double robotRadius = 20;  // Radius of the robot
    double halfTopWidth = detectionRadius * tan(M_PI / 6); // Half width at the detection radius
    double halfBaseWidth = halfTopWidth / 4;  // Half width at the robot
    if (halfBaseWidth > robotRadius / 3) {
        halfBaseWidth = robotRadius / 3;
    }

    // Calculate trapezoid corners relative to robot center
    QPointF baseLeft(robotRadius, -halfBaseWidth);
    QPointF baseRight(robotRadius, halfBaseWidth);
    QPointF topRight(detectionRadius + robotRadius, halfTopWidth);
    QPointF topLeft(detectionRadius + robotRadius, -halfTopWidth);

    // Rotate points around the robot's center at (0,0)
    baseLeft = rotatePoint(baseLeft, radOrientation);
    baseRight = rotatePoint(baseRight, radOrientation);
    topRight = rotatePoint(topRight, radOrientation);
    topLeft = rotatePoint(topLeft, radOrientation);

    QPainterPath detectionPath;
    detectionPath.moveTo(baseLeft + QPointF(positionX, positionY));
    detectionPath.lineTo(baseRight + QPointF(positionX, positionY));
    detectionPath.lineTo(topRight + QPointF(positionX, positionY));
    detectionPath.lineTo(topLeft + QPointF(positionX, positionY));
    detectionPath.closeSubpath();

    QGraphicsScene* currentScene = scene();
    QRectF sceneRect = currentScene->sceneRect();

    if (!sceneRect.contains(detectionPath.boundingRect())) {
        qDebug() << "Obstacle detected: out of scene bounds";
        return true;
    }

    QList<QGraphicsItem*> collidingItems = currentScene->items(detectionPath);
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
    rotationDirection = NoRotation;

    setPos(positionX, positionY);
}

/**
 * @brief rotate the robot to the right
 */
void RemoteRobot::rotateRight() {
    stop();
    rotationDirection = RotateRight;
    orientation = (orientation + 1) % 360;
    updatePosition();
}

/**
 * @brief rotate the robot to the left
 */
void RemoteRobot::rotateLeft() {
    stop();
    rotationDirection = RotateLeft;
    orientation = (orientation - 1 + 360) % 360;
    updatePosition();
}

/**
 * @brief stop the robot
 * 
 */
void RemoteRobot::stop(){
    isMoving = false;
    rotationDirection = NoRotation;
    updatePosition();
}

void RemoteRobot::updatePosition() {
    setPos(positionX, positionY);
}

Robot::RotationDirection RemoteRobot::getRotationDirection()  {
    return rotationDirection;
}

/**
 * @brief detect obstacles in the robot's path
 * 
 * @return true if an obstacle is detected
 * @return false if no obstacles are detected
 */
bool RemoteRobot::detectObstacle() {
    qDebug() << "Checking for obstacles for robot at position" << positionX << positionY;

    double radOrientation = orientation * M_PI / 180;
    double robotRadius = 20;  // Radius of the robot
    double halfTopWidth = detectionRadius * tan(M_PI / 6); // Half width at the detection radius
    double halfBaseWidth = halfTopWidth / 4;  // Half width at the robot
    if (halfBaseWidth > robotRadius / 3) {
        halfBaseWidth = robotRadius / 3;
    }

    // Calculate trapezoid corners relative to robot center
    QPointF baseLeft(robotRadius, -halfBaseWidth);
    QPointF baseRight(robotRadius, halfBaseWidth);
    QPointF topRight(detectionRadius + robotRadius, halfTopWidth);
    QPointF topLeft(detectionRadius + robotRadius, -halfTopWidth);

    // Rotate points around the robot's center at (0,0)
    baseLeft = rotatePoint(baseLeft, radOrientation);
    baseRight = rotatePoint(baseRight, radOrientation);
    topRight = rotatePoint(topRight, radOrientation);
    topLeft = rotatePoint(topLeft, radOrientation);

    QPainterPath detectionPath;
    detectionPath.moveTo(baseLeft + QPointF(positionX, positionY));
    detectionPath.lineTo(baseRight + QPointF(positionX, positionY));
    detectionPath.lineTo(topRight + QPointF(positionX, positionY));
    detectionPath.lineTo(topLeft + QPointF(positionX, positionY));
    detectionPath.closeSubpath();

    QGraphicsScene* currentScene = scene();
    QRectF sceneRect = currentScene->sceneRect();

    if (!sceneRect.contains(detectionPath.boundingRect())) {
        qDebug() << "Obstacle detected: out of scene bounds";
        return true;
    }

    QList<QGraphicsItem*> collidingItems = currentScene->items(detectionPath);
    foreach(QGraphicsItem *item, collidingItems) {
        if (item != this && (dynamic_cast<Obstacle*>(item) || dynamic_cast<Robot*>(item))) {
            qDebug() << "Obstacle detected: collision with an obstacle";
            return true;
        }
    }

    qDebug() << "No obstacles detected";
    return false;
}