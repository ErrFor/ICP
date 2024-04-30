#include "obstacle.h"
#include "qbrush.h"
#include "qgraphicsscene.h"
#include "qgraphicsview.h"
#include "qpen.h"
#include <QGraphicsSceneMouseEvent>
#include "mainwindow.h"

Obstacle::Obstacle(qreal x, qreal y, qreal width, QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, width, width, parent)
{
    // Устанавливаем цвет заливки препятствия
    setBrush(QBrush(Qt::white));  // Можно изменить на другой цвет
    // Установка черной границы вокруг препятствия
    setPen(QPen(Qt::black));
}

void Obstacle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        auto scene = this->scene();
        if (!scene) return; // Проверка наличия сцены

        auto views = scene->views();
        if (views.isEmpty()) return; // Проверка наличия виджетов просмотра

        QGraphicsView* view = views.first(); // Берём первый виджет просмотра
        if (!view) return; // Проверка, что виджет просмотра существует

        MainWindow *mainWindow = dynamic_cast<MainWindow *>(view->window());
        if (!mainWindow) return; // Проверка, что приведение типов выполнено корректно

        if (mainWindow->isDeletingModeActive()) {
            scene->removeItem(this);
            delete this; // Удаление элемента
        }
    }
}
