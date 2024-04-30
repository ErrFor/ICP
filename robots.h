#include <iostream>
#include <cmath>  // for basic math functions such as cos() and sin()
#include <QGraphicsItem>
#include <QPainter>

class Robot : public QGraphicsItem {
protected:
    double positionX, positionY;

public:
    Robot(double posX, double posY, double orient)
        : positionX(posX), positionY(posY) {}

    QRectF boundingRect() const override {
        return QRectF(positionX, positionY, 20, 20);  // Пример размеров
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override {
        painter->setBrush(Qt::blue);  // Задаем цвет
        painter->drawEllipse(positionX, positionY, 20, 20);  // Рисуем круг
    }



    virtual void update() = 0;
};

class AutonomousRobot : public Robot {
private:
    double detectionRadius;  // Radius in which the robot detects obstacles
    double avoidanceAngle;  // Angle to turn for obstacle avoidance
    int orientation;
    int angle;

public:
    AutonomousRobot(double posX, double posY, double orient, double detectRadius, double avoidAngle)
        : Robot(posX, posY, orient), detectionRadius(detectRadius), avoidanceAngle(avoidAngle) {
        orientation = orient;
        angle = avoidAngle;
    }
    void move();
    bool detectObstacle();

    void update() override {
        // Check for obstacles (simulated here as a function that returns a bool)
        if (detectObstacle()) {
            rotate(avoidanceAngle);  // Rotate to avoid the obstacle
        } else {
            move();  // Move forward if no obstacles
        }
    }

    void rotate(double angle) {
        setRotation(angle);  // Преобразуем в градусы и устанавливаем
    }
};
