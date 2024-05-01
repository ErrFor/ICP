#include <cmath>  // For cos() and sin()
#include <QGraphicsItem>
#include <QPainter>
#include "qgraphicsscene.h"
#include "robots.h"
#include "obstacle.h"

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
        if (item != this && dynamic_cast<Obstacle*>(item)) {
            qDebug() << "Obstacle detected: collision with an obstacle";
            return true;
        }
    }

    qDebug() << "No obstacles detected";
    return false;
}


