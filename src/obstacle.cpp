/**
 * @file obstacle.cpp
 * @author Kininbayev Timur (xkinin00)
 * @brief File containing the obstacle class logic
 */

#include "obstacle.h"
#include "qbrush.h"
#include "qgraphicsscene.h"
#include "qgraphicsview.h"
#include "qpen.h"
#include <QGraphicsSceneMouseEvent>
#include "mainwindow.h"

/**
 * @brief constructor of the Obstacle class
 * 
 * @param x x coordinate
 * @param y y coordinate
 * @param width size of the obstacle
 * @param parent parent object
 */
Obstacle::Obstacle(qreal x, qreal y, qreal width, QGraphicsItem *parent)
    : QGraphicsRectItem(x - width / 2, y - width / 2, width, width, parent)
{
    // set white color for the obstacle
    setBrush(QBrush(Qt::white)); 
    // set black color for the border
    setPen(QPen(Qt::black));
}

/**
 * @brief When the mouse is pressed on the obstacle, it is deleted (if deleted mode is active)
 * 
 * @param event 
 */
void Obstacle::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        auto scene = this->scene();
        if (!scene) return; // check if scene exists

        auto views = scene->views();
        if (views.isEmpty()) return; // check if views exist

        QGraphicsView* view = views.first(); // take the first view
        if (!view) return; // ensure that view exists

        MainWindow *mainWindow = dynamic_cast<MainWindow *>(view->window());
        if (!mainWindow) return; // check if mainWindow exists

        if (mainWindow->isDeletingModeActive()) {
            scene->removeItem(this);
            delete this; // delete the obstacle
        }
    }
}
