/**
 * @file createobstacledialog.cpp
 * @author Kininbayev Timur (xkinin00)
 * @brief File containing the create obstacle dialog class logic
 */
#include "createobstacledialog.h"
#include <QTimer>

/**
 * @brief Create a Obstacle Dialog:: Create Obstacle Dialog object
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

void CreateObstacleDialog::validateInputs() {
    bool inputsValid = !xInput->text().isEmpty() && !yInput->text().isEmpty() &&
                       !widthInput->text().isEmpty();
    bool xValid = xInput->text().toInt() <= 1500 && xInput->text().toInt() >= 0;
    bool yValid = yInput->text().toInt() <= 600 && yInput->text().toInt() >= 0;
    bool widthValid = widthInput->text().toDouble() > 0;

    createButton->setEnabled(inputsValid && xValid && yValid && widthValid);
}

CreateObstacleDialog::~CreateObstacleDialog() {
    // Destructor code here
}

/**
 * @brief Create a Obstacle Dialog::on create Button clicked object
 * 
 */
void CreateObstacleDialog::on_createButton_clicked()
{
    accept(); // Close the dialog and return QDialog::Accepted
}

/**
 * @brief xInput getter
 * 
 * @return int 
 */
int CreateObstacleDialog::getX() const {
     return xInput->text().toInt(); 
}

/**
 * @brief yInput getter
 * 
 * @return int 
 */
int CreateObstacleDialog::getY() const {
    return yInput->text().toInt();
}

/**
 * @brief widthInput getter
 * 
 * @return int 
 */
int CreateObstacleDialog::getWidth() const {
    return widthInput->text().toInt();
}
