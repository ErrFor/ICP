#include "createRobotDialog.h"

CreateRobotDialog::CreateRobotDialog(QWidget *parent) : QDialog(parent) {
    setupForm();
    setupConnections();
}

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

    QFormLayout *layout = new QFormLayout(this);
    layout->addRow("Type", robotTypeCombo);
    layout->addRow("X", xInput);
    layout->addRow("Y", yInput);
    layout->addRow("Speed", speedInput);
    layout->addRow("Orientation", orientationTypeCombo);
    layout->addRow("Detection Radius", detectionRadiusInput);
    layout->addRow("Avoidance Angle", avoidanceAngleInput);
    layout->addWidget(createButton);

    onRobotTypeChanged("Autonomous");  // Default to Autonomous
    setLayout(layout);
}

void CreateRobotDialog::setupConnections() {
    connect(robotTypeCombo, &QComboBox::currentTextChanged, this, &CreateRobotDialog::onRobotTypeChanged);
    connect(createButton, &QPushButton::clicked, this, &QDialog::accept);
}

void CreateRobotDialog::onRobotTypeChanged(const QString &type) {
    bool isAutonomous = (type == "Autonomous");
    detectionRadiusInput->setVisible(isAutonomous);
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
    return robotTypeCombo->currentIndex();  // Возвращает индекс выбранного типа робота
}

int CreateRobotDialog::getOrientation() const {
    return orientationTypeCombo->currentIndex();  // Возвращает индекс выбранного типа робота
}

int CreateRobotDialog::getSpeed() const{
    return speedInput->text().toInt();
}
