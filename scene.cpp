#include "scene.h"
scene::scene(QObject *parent) : QGraphicsScene(parent)
{
    this->setSceneRect(0,0,900,900);
    planner = new pathplan;
    thread = new QThread(this);
    planner->moveToThread(thread);
    showMap();

//    timer = new QTimer(this);
//    connect(timer,SIGNAL(timeout()),this,SLOT(showOpenlist_slot()));
//    timer->start(10);

}

/**
 * @brief scene::setObstacle 鼠标点击位置扩充21*21个障碍物点
 * @param point
 * @return
 */
QVector<int> scene::setObstacle(QPoint point)
{
    QVector<int> obstancleIndex;    //障碍物一维坐标
    int point_index_center = point.x() * 900 + point.y();        //一维地图21宫格中点的下标
    int point_index_start = point_index_center - 10 - 10 * 900;  //左上角初始点
    int point_index_end   = point_index_center + 10 + 10 * 900;  //右下角终止点
    if (point_index_start < 0 || point_index_end > 810000){
        qDebug()<< "超出地图界限，障碍物设置失败！";
    }
    int point_index;                                             //21宫格一维坐标值
    for (int i = 0; i < 21; i ++) {
       point_index = point_index_start + i * 900;
       for (int j = 0; j < 21;j ++) {
           point_index += 1;
           obstancleIndex.append(point_index);
       }
    }
    return obstancleIndex;
}

/**
 * @brief scene::showMap
 * 显示整张地图的障碍物
 */
void scene::showMap()
{
    for (int i = 0; i < 900; i++) {
        for (int j = 0; j < 900; j++) {
            if(planner->maze[i][j] == 1){
                QGraphicsRectItem* rectItem = new QGraphicsRectItem(0,0,1,1);
                rectItem->setBrush(QBrush(Qt::black));
                rectItem->setPos(i,j);
                this->addItem(rectItem);
            } // end of if
        } // end of for
    } // end of for

}

void scene::showPath()
{
    for(auto &p:planner->path){
        QGraphicsRectItem* rectItemPath = new QGraphicsRectItem(0,0,2,2);
        rectItemPath->setBrush(QBrush(Qt::blue));
        rectItemPath->setPos(p->x,p->y);
        this->addItem(rectItemPath);
    }
}


void scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (mouseUpdateFlag){
        QPoint mousePoint;
        mousePoint.setX(event->scenePos().x());
        mousePoint.setY(event->scenePos().y());
        showMap();
    }

    if (setStartFlag){

        //找到起点坐标
        mouseUpdateFlag = 0;
        setStartFlag = 0;
//        planner->startNode.x = event->scenePos().x();
//        planner->startNode.y = event->scenePos().y();

        //显示起点
        QGraphicsRectItem* rectItem = new QGraphicsRectItem(0,0,10,10);
        rectItem->setBrush(QBrush(Qt::red));
        rectItem->setPos(event->scenePos().x(),event->scenePos().y());
        this->addItem(rectItem);
        QPoint startPoint(event->scenePos().x(),event->scenePos().y());
        emit clickedStartCoord_sgl(startPoint);
    }

    if (setEndFlag){

        //找到终点坐标
        mouseUpdateFlag = 0;
        setEndFlag = 0;
//        planner->endNode.x = event->scenePos().x();
//        planner->endNode.y = event->scenePos().y();

        //显示起点
        QGraphicsRectItem* rectItem = new QGraphicsRectItem(0,0,10,10);
        rectItem->setBrush(QBrush(Qt::green));
        rectItem->setPos(event->scenePos().x(),event->scenePos().y());
        this->addItem(rectItem);
        QPoint endPoint(event->scenePos().x(),event->scenePos().y());
        emit clickedEndCoord_sgl(endPoint);
    }
}

void scene::showOpenlist_slot()
{
    for(auto &p:planner->path){

        QGraphicsRectItem* rectItem = new QGraphicsRectItem(0,0,10,10);
        rectItem->setBrush(QBrush(Qt::green));
        rectItem->setPos(p->x,p->y);
        this->addItem(rectItem);
    }

}
