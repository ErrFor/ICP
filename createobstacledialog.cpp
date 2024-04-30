#include "createobstacledialog.h"

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

void CreateObstacleDialog::on_createButton_clicked()
{
    accept(); // Закрывает диалог и возвращает QDialog::Accepted
}

int CreateObstacleDialog::getX() const { return xInput->text().toInt(); }
int CreateObstacleDialog::getY() const { return yInput->text().toInt(); }
int CreateObstacleDialog::getWidth() const { return widthInput->text().toInt(); }
