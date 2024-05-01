// В файле mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "robots.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT // Обязательно для использования сигналов и слотов

public:
    QTimer *timer;
    explicit MainWindow(QWidget *parent = nullptr);
    bool isDeletingModeActive() const { return deletingMode; }
    ~MainWindow();

private slots:
    void on_createObstacleButton_clicked();
    void on_deleteObstacleButton_clicked();
    void on_createRobotButton_clicked();
    void updateRobots();
    void startSimulation();
    void stopSimulation();

private:
    Ui::MainWindow *ui;
    bool deletingMode;
    QList<Robot*> robots;
};

#endif // MAINWINDOW_H
