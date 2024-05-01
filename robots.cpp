#include <cmath>  // For cos() and sin()
#include <QGraphicsItem>
#include <QPainter>
#include "qgraphicsscene.h"
#include "robots.h"

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
    double radAngle = orientation * M_PI / 180;
    double dx = detectionRadius * cos(radAngle);
    double dy = detectionRadius * sin(radAngle);

    // Check for scene boundaries
    QGraphicsScene* currentScene = scene();
    QRectF sceneRect = currentScene->sceneRect();
    QRectF robotFuturePosition(positionX + dx, positionY + dy, 20, 20); // Assuming robot size is 20x20

    if (!sceneRect.contains(robotFuturePosition)) {
        return true; // Robot is about to move out of the scene bounds
    }

    // Existing obstacle detection logic remains the same
    QList<QGraphicsItem*> itemsFound = currentScene->items(mapToScene(robotFuturePosition).boundingRect(), Qt::IntersectsItemShape);
    for (QGraphicsItem* item : itemsFound) {
        if (item != this && item->collidesWithItem(this)) {
            return true;
        }
    }
    return false;
}

