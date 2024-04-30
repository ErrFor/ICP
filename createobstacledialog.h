#ifndef CREATEOBSTACLEDIALOG_H
#define CREATEOBSTACLEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>

class CreateObstacleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateObstacleDialog(QWidget *parent = nullptr);

    int getX() const;
    int getY() const;
    int getWidth() const;

private:
    QLineEdit *xInput;
    QLineEdit *yInput;
    QLineEdit *widthInput;
    QPushButton *createButton;

private slots:
    void on_createButton_clicked();
};

#endif // CREATEOBSTACLEDIALOG_H
