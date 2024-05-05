/**
 * @file createRobotDialog.h
 * @author Kininbayev Timur (xkinin00)
 * @brief File containing the create robot dialog class
 */
#ifndef CREATEROBOTDIALOG_H
#define CREATEROBOTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>
#include <QTimer>

/**
 * @class CreateRobotDialog
 * @brief Robot dialog window class for ui representation of the robot creation
 */
class CreateRobotDialog : public QDialog {
    Q_OBJECT

public:
    explicit CreateRobotDialog(QWidget *parent = nullptr);
    virtual ~CreateRobotDialog();
    int getRobotType() const;
    int getOrientation() const; // Only for Autonomous

    int getSpeed() const;
    double getX() const;
    double getY() const;
    double getDetectionRadius() const;
    double getAvoidanceAngle() const;   // Only for Autonomous

private:
    QComboBox *robotTypeCombo;
    QLineEdit *xInput;
    QLineEdit *yInput;
    QLineEdit *speedInput;
    QComboBox *orientationTypeCombo;
    QLineEdit *detectionRadiusInput;
    QLineEdit *avoidanceAngleInput;
    QPushButton *createButton;
    QTimer* validationTimer;
    void validateInputs();

    void setupForm();
    void setupConnections();

private slots:
    void onRobotTypeChanged(const QString &type);
};

#endif // CREATEROBOTDIALOG_H
