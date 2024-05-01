#include <cmath>  // For cos() and sin()
#include <QGraphicsItem>
#include <QPainter>
#include "mainwindow.h"
#include "qgraphicsscene.h"
#include "qgraphicsview.h"
#include "obstacle.h"
#include "qgraphicssceneevent.h"

void AutonomousRobot::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        auto scene = this->scene();
        if (!scene) return; // Проверка наличия сцены

        auto views = scene->views();
        if (views.isEmpty()) return; // Проверка наличия виджетов просмотра

        QGraphicsView* view = views.first(); // Берём первый виджет просмотра
        if (!view) return; // Проверка, что виджет просмотра существует

        MainWindow *mainWindow = dynamic_cast<MainWindow *>(view->window());
        if (!mainWindow) return; // Проверка, что приведение типов выполнено корректно

        if (mainWindow->isDeletingModeActive()) {
            scene->removeItem(this);
            delete this; // Удаление элемента
        }
    }
}

// В файле RemoteRobot.cpp или там, где у вас определен класс RemoteRobot
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


void AutonomousRobot::move() {
    double radAngle = orientation * M_PI / 180;
    double dx = speed * cos(radAngle);
    double dy = speed * sin(radAngle);

    positionX += dx;
    positionY += dy;

    // Use QGraphicsItem's setPos to handle updating the scene position
    setPos(positionX, positionY);
}

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

void RemoteRobot::rotateRight() {
        orientation = (orientation + 90) % 360;
}

void RemoteRobot::rotateLeft() {
        orientation = (orientation - 90 + 360) % 360;
}

void RemoteRobot::stop(){
    isMoving = false;
}

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
