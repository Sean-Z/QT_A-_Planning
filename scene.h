#ifndef SCENE_H
#define SCENE_H
#include"pathplan.h"
#include <queue>
#include <vector>
#include <QObject>
#include <QDebug>
#include <QQueue>
#include <QStack>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>

//场景类
class scene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit scene(QObject *parent = nullptr);
    QVector<int> setObstacle(QPoint);       //设置地图的障碍物  返回obstancleIndex
    void showMap();                         //显示整张地图的障碍物
    void showPath();
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

signals:
    void clickedStartCoord_sgl(QPoint point); //鼠标单击
    void clickedEndCoord_sgl(QPoint point); //鼠标单击

public slots:
    void showOpenlist_slot();
public:
    bool mouseUpdateFlag = 0;       //鼠标更新地图标志位 0:关闭更新 1:打开更新
    bool setStartFlag = 0;
    bool setEndFlag = 0;

public:
    pathplan* planner;
    QThread* thread;
    QTimer* timer;

};

#endif // SCENE_H
