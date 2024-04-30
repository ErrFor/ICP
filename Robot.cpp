#include <iostream>
#include <cmath>  // for basic math functions such as cos() and sin()

class Robot {
protected:
    double positionX, positionY;  // X and Y coordinates of the robot's position
    double orientation;  // orientation of the robot in radians
    double speed;  // speed of the robot

public:
    Robot(double posX, double posY, double orient, double spd)
        : positionX(posX), positionY(posY), orientation(orient), speed(spd) {}

    virtual void move() {
        // Update position based on speed and orientation
        positionX += speed * cos(orientation);
        positionY += speed * sin(orientation);
    }

    void rotate(double angle) {
        // Update orientation by adding angle in radians
        orientation += angle;
    }

    virtual void update() = 0;  // Pure virtual function to enforce subclass implementation

    // Utility method for printing the state of the robot
    void printState() const {
        std::cout << "Position: (" << positionX << ", " << positionY << ")"
                  << " Orientation: " << orientation
                  << " Speed: " << speed << std::endl;
    }
};

class AutonomousRobot : public Robot {
private:
    double detectionRadius;  // Radius in which the robot detects obstacles
    double avoidanceAngle;  // Angle to turn for obstacle avoidance

public:
    AutonomousRobot(double posX, double posY, double orient, double spd, double detectRadius, double avoidAngle)
        : Robot(posX, posY, orient, spd), detectionRadius(detectRadius), avoidanceAngle(avoidAngle) {}

    void update() override {
        // Check for obstacles (simulated here as a function that returns a bool)
        if (detectObstacle()) {
            rotate(avoidanceAngle);  // Rotate to avoid the obstacle
        } else {
            move();  // Move forward if no obstacles
        }
    }

    bool detectObstacle() {
        // Placeholder for obstacle detection logic
        // This would interact with the environment to determine if any obstacles are within detectionRadius
        return false;  // Example: No obstacle detected
    }
};

class RemoteControlledRobot : public Robot {
public:
    enum Command { STOP, MOVE_FORWARD, ROTATE_LEFT, ROTATE_RIGHT };

private:
    Command currentCommand;

public:
    RemoteControlledRobot(double posX, double posY, double orient, double spd)
        : Robot(posX, posY, orient, spd), currentCommand(STOP) {}

    void setCommand(Command cmd) {
        currentCommand = cmd;
    }

    void update() override {
        switch (currentCommand) {
            case MOVE_FORWARD:
                move();
                break;
            case ROTATE_LEFT:
                rotate(-0.1);  // Rotate counter-clockwise
                break;
            case ROTATE_RIGHT:
                rotate(0.1);  // Rotate clockwise
                break;
            case STOP:
            default:
                // Do nothing if stopped
                break;
        }
    }
};


int main() {
    AutonomousRobot autoRobot(0, 0, 0, 1.0, 5.0, 0.785);  // Example parameters
    RemoteControlledRobot remoteRobot(10, 10, 0, 0.5);

    autoRobot.update();
    autoRobot.printState();

    remoteRobot.setCommand(RemoteControlledRobot::MOVE_FORWARD);
    remoteRobot.update();
    remoteRobot.printState();

    return 0;
}