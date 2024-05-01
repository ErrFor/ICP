#include "qdebug.h"
#include "qgraphicsscene.h"
#include <iostream>
#include <cmath>  // for basic math functions such as cos() and sin()
#include <QGraphicsItem>
#include <QPainter>

class Robot : public QGraphicsItem {
protected:
    double positionX, positionY;
    int speed;
public:
    Robot(double posX, double posY, int speed)
        : positionX(posX), positionY(posY), speed(speed){

    }

    QRectF boundingRect() const override {
        return QRectF(-10, -10, 20, 20);  // Пример размеров
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setBrush(Qt::blue);  // Задаем цвет
        painter->drawEllipse(positionX, positionY, 20, 20);
    }



    virtual void update() = 0;
};

class AutonomousRobot : public Robot {
private:
    double detectionRadius;  // Radius in which the robot detects obstacles
    double avoidanceAngle;  // Angle to turn for obstacle avoidance
    int angle;
    int orientation;

public:
    AutonomousRobot(double posX, double posY, int orient, double detectRadius, double avoidAngle, int speed)
        : Robot(posX, posY, speed), detectionRadius(detectRadius), avoidanceAngle(avoidAngle) {
        angle = avoidAngle;
        switch (orient) {
            case 0: orientation = 270; break; // Наверх
            case 1: orientation = 0;   break; // Направо
            case 2: orientation = 90;  break; // Вниз
            case 3: orientation = 180; break; // Налево
            default: orientation = 0;  break; // Дефолтное значение
        }
    }
    void move();
    bool detectObstacle();

    void update() override {
        if (detectObstacle()) {
                orientation += avoidanceAngle; // Update orientation to avoid collision
                if (orientation >= 360) orientation -= 360; // Normalize orientation
            }

            // Calculate proposed movement
            double radAngle = orientation * M_PI / 180;
            double proposedX = positionX + speed * cos(radAngle);
            double proposedY = positionY + speed * sin(radAngle);

            // Check and handle boundaries
            QRectF sceneBounds = scene()->sceneRect();
            if (proposedX < sceneBounds.left() || proposedX > sceneBounds.right()) {
                orientation = 180 - orientation; // Reverse direction horizontally
                proposedX = positionX; // Stop horizontal movement
            }
            if (proposedY < sceneBounds.top() || proposedY > sceneBounds.bottom()) {
                orientation = -orientation; // Reverse direction vertically
                proposedY = positionY; // Stop vertical movement
            }

            // Normalize orientation
            if (orientation < 0) orientation += 360;
            if (orientation >= 360) orientation -= 360;

            // Update position
            positionX = proposedX;
            positionY = proposedY;
            setPos(positionX, positionY);

            qDebug() << "Robot moved to:" << positionX << "," << positionY;
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override{
        Q_UNUSED(option);
        Q_UNUSED(widget);

        // Basic robot visualization
        painter->setBrush(Qt::blue);
        painter->drawEllipse(boundingRect()); // Draw robot centered at its position

        // Variables for trapezoid field of vision
        double radOrientation = orientation * M_PI / 180;
        double robotRadius = 10;  // Radius of the robot
        double halfBaseWidth = robotRadius / 2;  // Half width at the robot
        double halfTopWidth = detectionRadius * tan(M_PI / 6); // Half width at the detection radius

        // Calculate trapezoid corners
        QPointF baseLeft(robotRadius, -halfBaseWidth);  // Start from the right edge of the robot
        QPointF baseRight(robotRadius, halfBaseWidth);
        QPointF topRight(detectionRadius + robotRadius, halfTopWidth);
        QPointF topLeft(detectionRadius + robotRadius, -halfTopWidth);

        // Rotate points around the robot's center at (0,0)
        baseLeft = rotatePoint(baseLeft, radOrientation);
        baseRight = rotatePoint(baseRight, radOrientation);
        topRight = rotatePoint(topRight, radOrientation);
        topLeft = rotatePoint(topLeft, radOrientation);

        // Creating the path for the trapezoid field of vision
        QPainterPath viewField;
        viewField.moveTo(baseLeft);
        viewField.lineTo(baseRight);
        viewField.lineTo(topRight);
        viewField.lineTo(topLeft);
        viewField.closeSubpath();

        // Setting semi-transparent red for the field of vision
        painter->setBrush(QColor(255, 0, 0, 100));
        painter->drawPath(viewField);
}

    QPointF rotatePoint(const QPointF& point, double angle) {
        return QPointF(cos(angle) * point.x() - sin(angle) * point.y(),
                       sin(angle) * point.x() + cos(angle) * point.y());
    }

    void rotate(double angle) {
        setRotation(angle);  // Преобразуем в градусы и устанавливаем
    }
};
