/**
 * @file createobstacledialog.cpp
 * @author Kininbayev Timur (xkinin00)
 * @brief File containing the create obstacle dialog class logic
 */
#include "createobstacledialog.h"

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

    connect(createButton, &QPushButton::clicked, this, &CreateObstacleDialog::on_createButton_clicked);
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
