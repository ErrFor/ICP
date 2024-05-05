/**
 * @file createobstacledialog.h
 * @author Kininbayev Timur (xkinin00)
 * @brief File containing the create obstacle dialog class
 * @version 0.1
 * @date 2024-05-02
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef CREATEOBSTACLEDIALOG_H
#define CREATEOBSTACLEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>

/**
 * @class CreateObstacleDialog
 * @brief Obstacle dialog window class for ui representation of the obstacle creation
 */
class CreateObstacleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateObstacleDialog(QWidget *parent = nullptr);
    virtual ~CreateObstacleDialog();

    int getX() const;
    int getY() const;
    int getWidth() const;

private:
    QLineEdit *xInput;
    QLineEdit *yInput;
    QLineEdit *widthInput;
    QPushButton *createButton;
    QTimer* validationTimer;

private slots:
    void on_createButton_clicked();
    void validateInputs();
};

#endif // CREATEOBSTACLEDIALOG_H
