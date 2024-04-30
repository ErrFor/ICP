#include <iostream>
#include <cmath>  // for basic math functions such as cos() and sin()
#include <QGraphicsItem>
#include <QPainter>
#include "qgraphicsscene.h"
#include "robots.h"

void AutonomousRobot::move(){
   switch (orientation) {
       case 0:
           positionY -= 1;  // Движение вверх уменьшает координату Y
           break;
       case 1:
           positionX += 1;  // Движение вправо увеличивает координату X
           break;
       case 2:
           positionY += 1;  // Движение вниз увеличивает координату Y
           break;
       case 3:
           positionX -= 1;  // Движение влево уменьшает координату X
           break;
       }
   setPos(positionX, positionY);
}

bool AutonomousRobot::detectObstacle() {
    QRectF searchArea(positionX - detectionRadius, positionY - detectionRadius,
                      detectionRadius, detectionRadius);
    QList<QGraphicsItem*> itemsFound = scene()->items(searchArea);
    ;
    for (QGraphicsItem* item : itemsFound) {
        printf("%s", item);
        if (item != this) {  // Проверяем, не является ли элемент самим роботом и является ли препятствием
            return true;
        }
    }
    return false;
}
