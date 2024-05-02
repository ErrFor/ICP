/**
 * @file createRobotDialog.cpp
 * @author Kininbayev Timur (xkinin00)
 * @brief File containing the create robot dialog class logic
 */
#include "createRobotDialog.h"

/**
 * @brief Create a Robot Dialog:: Create Robot Dialog object
 * 
 * @param parent 
 */
CreateRobotDialog::CreateRobotDialog(QWidget *parent) : QDialog(parent) {
    setupForm();
    setupConnections();
}

/**
 * @brief Create a Robot Dialog::setup Form object
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

/**
 * @brief Create a Robot Dialog::setup Connections object
 * 
 */
void CreateRobotDialog::setupConnections() {
    connect(robotTypeCombo, &QComboBox::currentTextChanged, this, &CreateRobotDialog::onRobotTypeChanged);
    connect(createButton, &QPushButton::clicked, this, &QDialog::accept);
}

/**
 * @brief Create a Robot Dialog::on Robot Type Changed object
 * 
 * @param type 
 */
void CreateRobotDialog::onRobotTypeChanged(const QString &type) {
    bool isAutonomous = (type == "Autonomous");
    avoidanceAngleInput->setVisible(isAutonomous);
    orientationTypeCombo->setVisible(isAutonomous);
}

/**
 * @brief x coordinate getter
 * 
 * @return double
 */
double CreateRobotDialog::getX() const {
    return xInput->text().toDouble();
}

/**
 * @brief y coordinate getter
 * 
 * @return double 
 */
double CreateRobotDialog::getY() const {
    return yInput->text().toDouble();
}

/**
 * @brief detection radius getter
 * 
 * @return double 
 */
double CreateRobotDialog::getDetectionRadius() const {
    return detectionRadiusInput->text().toDouble();
}

/**
 * @brief avoidance angle getter
 * 
 * @return double 
 */
double CreateRobotDialog::getAvoidanceAngle() const {
    return avoidanceAngleInput->text().toDouble();
}

/**
 * @brief robot type getter
 * 
 * @return int 
 */
int CreateRobotDialog::getRobotType() const {
    return robotTypeCombo->currentIndex();  // Возвращает индекс выбранного типа робота
}

/**
 * @brief orientation getter
 * 
 * @return int 
 */
int CreateRobotDialog::getOrientation() const {
    return orientationTypeCombo->currentIndex();  // Возвращает индекс выбранного типа робота
}

/**
 * @brief speed getter
 * 
 * @return int 
 */
int CreateRobotDialog::getSpeed() const{
    return speedInput->text().toInt();
}
