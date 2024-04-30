#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsRectItem>

class Obstacle : public QGraphicsRectItem
{
public:
    // Конструктор
    Obstacle(qreal x, qreal y, qreal width, QGraphicsItem *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;  // Обработчик события мыши
};

#endif // OBSTACLE_H
