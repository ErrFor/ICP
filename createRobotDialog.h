#ifndef CREATEROBOTDIALOG_H
#define CREATEROBOTDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>
#include <QLabel>

class CreateRobotDialog : public QDialog {
    Q_OBJECT

public:
    explicit CreateRobotDialog(QWidget *parent = nullptr);
    int getRobotType() const;
    int getOrientation() const;

    int getSpeed() const;
    double getX() const;
    double getY() const;
    double getDetectionRadius() const;  // Only for Autonomous
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

    void setupForm();
    void setupConnections();

private slots:
    void onRobotTypeChanged(const QString &type);
};

#endif // CREATEROBOTDIALOG_H
