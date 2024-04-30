#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "obstacle.h"
#include "createobstacledialog.h"
#include <QGraphicsScene>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // В конструкторе MainWindow, после инициализации ui
    connect(ui->createObstacle, &QPushButton::clicked, this, &MainWindow::on_createObstacleButton_clicked);
    connect(ui->deleteObstacle, &QPushButton::clicked, this, &MainWindow::on_deleteObstacleButton_clicked);

    QPushButton* buttons[2] = { ui->pushButton_4, ui->pushButton_5};

    buttons[0]->setStyleSheet("QPushButton { background-color: green; }");
    buttons[1]->setStyleSheet("QPushButton { background-color: red; }");

// Создаем сцену
    QGraphicsScene *scene = new QGraphicsScene(this);
    // Устанавливаем размер сцены (например, 800x600)
//    scene->setSceneRect(0, 200, 1311, 741);
    scene->setBackgroundBrush(QBrush(Qt::gray));

    // Создаем виджет QGraphicsView и связываем его с сценой
    ui->graphicsView->setScene(scene);

    // Опционально: отключаем скроллбары, если хотите, чтобы вид всегда оставался на всю сцену
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    deletingMode = false;
}

MainWindow::~MainWindow()
{
    // Деструктор
}

void MainWindow::on_createObstacleButton_clicked()
{
    CreateObstacleDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        int x = dialog.getX();
        int y = dialog.getY();
        int width = dialog.getWidth();
//        int height = dialog.getHeight();

        Obstacle *obstacle = new Obstacle(x, y, width);
        ui->graphicsView->scene()->addItem(obstacle);
    }
}

void MainWindow::on_deleteObstacleButton_clicked()
{
    // Устанавливаем флаг режима удаления
    deletingMode = !deletingMode;  // deletingMode должен быть членом класса MainWindow

    // Меняем визуальное отображение для указания режима удаления
    if (deletingMode) {
        foreach (QGraphicsItem *item, ui->graphicsView->scene()->items()) {
            Obstacle *obstacle = dynamic_cast<Obstacle*>(item);
            if (obstacle) {
                obstacle->setPen(QPen(Qt::red));  // Подсвечиваем препятствия красным
            }
        }
    } else {
        foreach (QGraphicsItem *item, ui->graphicsView->scene()->items()) {
            Obstacle *obstacle = dynamic_cast<Obstacle*>(item);
            if (obstacle) {
                obstacle->setPen(QPen(Qt::black));  // Возвращаем обычный вид
            }
        }
    }
}
