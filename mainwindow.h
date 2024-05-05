/**
 * @file mainwindow.h
 * @author Kininbayev Timur (xkinin00)
 * @author Yaroslav Slabik (xslabi01)
 * @brief File containing the main window class
 * 
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPointer>
#include "robots.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Main window class, contains all the slots and signals for the GUI
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT // necessary macro for Qt to recognize the class as a QObject

public:
    QTimer *timer;
    explicit MainWindow(QWidget *parent = nullptr);
    bool isDeletingModeActive() const { return deletingMode; }
    bool isRobotDeletingModeActive() const { return rDeletingMode; }
    ~MainWindow();
    void selectRobot(RemoteRobot* robot);
    QList<Robot*> autonomousRobots;
    QList<Robot*> remoteRobots;
    RemoteRobot* selectedRobot = nullptr;
    void loadSceneFromFile(const QString& filename);

private slots: // slots are functions that are called when a signal is emitted
    void createObstacle();
    void deleteObstacle();
    void createRobot();
    void deleteRobot();
    void updateRobots();
    void moveRobot();
    void rotateRobotLeft();
    void rotateRobotRight();
    void stopRobot();
    void startSimulation();
    void stopSimulation();
    void onLoadFileClicked();
    void processObject(const QString& type, const QString& attributes);
        QMap<QString, QString> parseAttributes(const QString& attributes);

private:
    Ui::MainWindow *ui;
    bool deletingMode;
    bool rDeletingMode;
};

#endif // MAINWINDOW_H
