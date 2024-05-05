/**
 * @file createobstacledialog.cpp
 * @author Kininbayev Timur (xkinin00)
 * @brief File containing the create obstacle dialog class logic
 */
#include "createobstacledialog.h"
#include <QTimer>

/**
 * @brief constructor of the CreateObstacleDialog class
 * 
 * @param parent 
 */
CreateObstacleDialog::CreateObstacleDialog(QWidget *parent) : QDialog(parent)
{
    xInput = new QLineEdit(this);
    yInput = new QLineEdit(this);
    widthInput = new QLineEdit(this);
    createButton = new QPushButton("Create", this);

    QFormLayout *layout = new QFormLayout();
    layout->addRow("X coordinate:", xInput);
    layout->addRow("Y coordinate:", yInput);
    layout->addRow("Size:", widthInput);
    layout->addWidget(createButton);

    setLayout(layout);
    setWindowTitle("Create Obstacle");

    validationTimer = new QTimer(this);
    connect(validationTimer, &QTimer::timeout, this, &::CreateObstacleDialog::validateInputs);
    validationTimer->start(500);

    connect(createButton, &QPushButton::clicked, this, &CreateObstacleDialog::on_createButton_clicked);
    createButton->setEnabled(false);
}

/**
 * @brief validate the inputs of the dialog box
 * @details check if every input is entered and if the values are in the correct range
 */
void CreateObstacleDialog::validateInputs() {
    bool inputsValid = !xInput->text().isEmpty() && !yInput->text().isEmpty() &&
                       !widthInput->text().isEmpty();
    bool xValid = xInput->text().toInt() <= 1500 && xInput->text().toInt() >= 0;
    bool yValid = yInput->text().toInt() <= 600 && yInput->text().toInt() >= 0;
    bool widthValid = widthInput->text().toDouble() > 0;

    createButton->setEnabled(inputsValid && xValid && yValid && widthValid);
}

CreateObstacleDialog::~CreateObstacleDialog() {}

/**
 * @brief Create a Obstacle Dialog when create obstacle button is clicked
 * 
 */
void CreateObstacleDialog::on_createButton_clicked()
{
    accept(); // Close the dialog and return QDialog::Accepted
}

int CreateObstacleDialog::getX() const {
     return xInput->text().toInt(); 
}

int CreateObstacleDialog::getY() const {
    return yInput->text().toInt();
}

int CreateObstacleDialog::getWidth() const {
    return widthInput->text().toInt();
}
