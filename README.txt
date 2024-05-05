ICP project 2023-2024

Simple robot simulation implementation using QT

Autors:
Kininbayev Timur(xkinin00) & Slabik Yaroslav(xslabi01)

Usage:
    "make" to build the proejct into build directory
    "make run" to execute project
    "make doxygen" to generate documentation into doc directory
    make clean deletes both build and doc directories

Simulation can be launched and stopped by using “Start” and “Stop” buttons.

Robots are created by the “Create Robot” button: users can create robots at any coordinates with any detection radius (px), speed (px / 100ms) and avoidance angle (deg).
Creation robots on coordinates that will collapse with obstacle or other robot is not allowed - dialog window with warning created. 

User can create a remote controlled robot: remote controlled robots are controlled by using “Move”, “Rotate right”, “Rotate left”, “Stop” buttons. 
First, user have to select robot (mouse click), after pressing move, robot will move at the selected destination (default right). 

Clicking on the "Delete Robot" button activates the delete mode, in which the selected robot (mouse click) is deleted.

Obstacles can be created by using ”Create Obstacle” button: users can create obstacles at any coordinates with any size (px).

Clicking on the "Delete Obstacle" button activates the delete mode, in which the selected obstacle (mouse click) is deleted.

User can clear whole map via "Clear" button - deletes every object in the scene, set selected robot to nullptr.

User can import a .txt format map by clicking on the "Import" button. (Also avialable using ./build/simulation example/test_file_number.txt)
Map format:

AutonomousRobot{
    positionX = 980
    positionY = 122
    orientation = 0
    detectionRadius = 50
    avoidanceAngle = 30
    speed = 7
}
Obstacle{
    positionX = 1266
    positionY = 141
    width = 11
}
RemoteRobot{
    positionX = 149
    positionY = 277
    speed = 5
    detectionRadius = 38
}

Implemted features:
    Whole logic of walls and objects detection both for remote and autonomous robots
    Proper autonomous and remote robots logic - movement, rotations, deletions, creations
    Remote robot selection to control selected robot.
    Spawn collision prevention - user can't create objects that will be on each over when created
    Wrong attributes creation prevention - user can't create objects outside the scene (1500x600) or with negative attributes
    Maps importing - user can import objects lists using gui or terminal
    Scene clear using "Clear" button or single objects deletions using "Delete robot" and "Delete obstacle" buttons

Not fully implemented parts:
    Robot's hitbox is little differ from visualisation (draws circles but hitbox is actually rectangle)
