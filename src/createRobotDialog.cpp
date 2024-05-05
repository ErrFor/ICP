/**
 * @file createRobotDialog.cpp
 * @author Kininbayev Timur (xkinin00)
 * @brief File containing the create robot dialog class logic
 */
#include "createRobotDialog.h"
#include <QTimer>

/**
 * @brief constructor of the CreateRobotDialog class
 * 
 * @param parent 
 */
CreateRobotDialog::CreateRobotDialog(QWidget *parent) : QDialog(parent) {
    setupForm();
    setupConnections();
}

/**
 * @brief destructor of the CreateRobotDialog class
 * 
 */
CreateRobotDialog::~CreateRobotDialog(){}

/**
 * @brief setup the form of the dialog box
 * 
 */
void CreateRobotDialog::setupForm() {
    robotTypeCombo = new QComboBox();
    robotTypeCombo->addItem("Autonomous");
    robotTypeCombo->addItem("Remote Controlled");

    orientationTypeCombo = new QComboBox();
    orientationTypeCombo->addItem("Top");
    orientationTypeCombo->addItem("Right");
    orientationTypeCombo->addItem("Bottom");
    orientationTypeCombo->addItem("Left");

    xInput = new QLineEdit();
    yInput = new QLineEdit();
    speedInput = new QLineEdit();
    detectionRadiusInput = new QLineEdit();
    avoidanceAngleInput = new QLineEdit();

    createButton = new QPushButton("Create");

    validationTimer = new QTimer(this);
    connect(validationTimer, &QTimer::timeout, this, &::CreateRobotDialog::validateInputs);
    validationTimer->start(500);

    QFormLayout *layout = new QFormLayout(this);
    layout->addRow("Type", robotTypeCombo);
    layout->addRow("X", xInput);
    layout->addRow("Y", yInput);
    layout->addRow("Speed", speedInput);
    layout->addRow("Orientation", orientationTypeCombo);
    layout->addRow("Detection Radius", detectionRadiusInput);
    layout->addRow("Avoidance Angle", avoidanceAngleInput);
    layout->addWidget(createButton);

    createButton->setEnabled(false);
    onRobotTypeChanged("Autonomous");  // Default to Autonomous
    setLayout(layout);
}

/**
 * @brief validate the inputs of the dialog box
 * @details check if every input is entered and if the values are in the correct range
 */
void CreateRobotDialog::validateInputs() {
    bool inputsValid = !xInput->text().isEmpty() && !yInput->text().isEmpty() &&
                       !speedInput->text().isEmpty();
    bool xValid = xInput->text().toInt() <= 1500 && xInput->text().toInt() >= 0;
    bool yValid = yInput->text().toInt() <= 600 && yInput->text().toInt() >= 0;
    bool radiusValid = detectionRadiusInput->text().toDouble() > 0;
    bool angleValid = avoidanceAngleInput->text().toDouble() > 0;
    if (getRobotType() == 1 && !angleValid){
        angleValid = 1;
    }
    createButton->setEnabled(inputsValid && xValid && yValid && radiusValid && angleValid);
}

void CreateRobotDialog::setupConnections() {
    connect(robotTypeCombo, &QComboBox::currentTextChanged, this, &CreateRobotDialog::onRobotTypeChanged);
    connect(createButton, &QPushButton::clicked, this, &QDialog::accept);
}

void CreateRobotDialog::onRobotTypeChanged(const QString &type) {
    bool isAutonomous = (type == "Autonomous");
    avoidanceAngleInput->setVisible(isAutonomous);
    orientationTypeCombo->setVisible(isAutonomous);
}

double CreateRobotDialog::getX() const {
    return xInput->text().toDouble();
}

double CreateRobotDialog::getY() const {
    return yInput->text().toDouble();
}

double CreateRobotDialog::getDetectionRadius() const {
    return detectionRadiusInput->text().toDouble();
}

double CreateRobotDialog::getAvoidanceAngle() const {
    return avoidanceAngleInput->text().toDouble();
}

int CreateRobotDialog::getRobotType() const {
    return robotTypeCombo->currentIndex();
}

int CreateRobotDialog::getOrientation() const {
    return orientationTypeCombo->currentIndex();
}

int CreateRobotDialog::getSpeed() const{
    return speedInput->text().toInt();
}
