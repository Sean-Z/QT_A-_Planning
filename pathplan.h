#ifndef PATHPLAN_H
#define PATHPLAN_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QPoint>
#include <cmath>
const int kCost1=10; //直移一格消耗
const int kCost2=14; //斜移一格消耗
struct node
{
    int x,y; //点坐标，这里为了方便按照C++的数组来计算，x代表横排，y代表竖列
    int F,G,H; //F=G+H
    node *parent; //parent的坐标，这里没有用指针，从而简化代码
    node(int _x,int _y):x(_x),y(_y),F(0),G(0),H(0),parent(NULL)  //变量初始化
    {
    }
};
class pathplan : public QObject
{
    Q_OBJECT
public:
    explicit pathplan(QObject *parent = nullptr);
    //线程处理函数
    bool isStop = 0;
    bool map[810000];               //地图 0:free 1:obstacle
    std::vector<std::vector<int>> maze;
    pathplan *planner;
    std::list<node*> openList;  //开启列表
    std::list<node*> closeList; //关闭列表
    std::list<node*> path;
    int visit[810000];
    double directions[8][3] = {{ -1,  1, 1.4},{  0,  1,   1},
                               {  1,  1, 1.4},{ -1,  0,   1},
                               {  1,  0,   1},{ -1, -1, 1.4},
                               {  0, -1,   1},{  1, -1, 1.4}};
public:
    void initMap();
    void initObstacle();                    //初始化默认障碍物，更新到maze数组
    std::list<node *> GetPath(node &startPoint,node &endPoint,bool isIgnoreCorner);
    node* findPath(node &startPoint,node &endPoint,bool isIgnoreCorner);
    node* getLeastFpoint(); //从开启列表中返回F值最小的节点
    node* isInList(const std::list<node *> &list,const node *point) const; //判断开启/关闭列表中是否包含某点
    bool isCanreach(const node *point,const node *target,bool isIgnoreCorner) const; //判断某点是否可以用于下一步判断
    std::vector<node *> getSurroundPoints(const node *point,bool isIgnoreCorner) const;
    int calcG(node *temp_start,node *point);
    int calcH(node *point,node *end);
    int calcF(node *point);

    int  pointToIndex(QPoint);
    void updateMap(QVector<QPoint>);
    void updateMap(QVector<int>);
    void clearMap();

signals:

public slots:
    void Astar(QPoint start, QPoint end);

};

#endif // PATHPLAN_H
