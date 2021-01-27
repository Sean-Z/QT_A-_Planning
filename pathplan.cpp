#include "pathplan.h"

pathplan::pathplan(QObject *parent) : QObject(parent)
{
    initMap();
    initObstacle();
}

/**
 * @brief pathplan::initMap
 * 初始化整张地图为:900*900 赋值为0
 */

void pathplan::initMap()
{
    maze.resize(900, std::vector<int>(900,0));
}

/**
 * @brief pathplan::initObstacle 初始化障碍物坐标,更新到地图map中
 */
void pathplan::initObstacle()
{
    /*
     ************************
     *                      *
     *   ****      *        *
     *   *         *        *
     *   *         ****     *
     *                      *
     *      *      ****     *
     *      *         *     *
     *   ****         *     *
     *                      *
     ************************
       */
    QVector<QPoint> initObstaclePoints;
    for(int i = 100; i < 350; i ++){
        QPoint tmpPoint;
        tmpPoint.setX(i);
        tmpPoint.setY(100);
        initObstaclePoints.push_back(tmpPoint);
    }

    for (int j = 100; j < 350; j ++){
        QPoint tmpPoint;
        tmpPoint.setX(100);
        tmpPoint.setY(j);
        initObstaclePoints.push_back(tmpPoint);
    }

    for (int m = 350; m > 100; m --) {
        QPoint tmpPoint;
        tmpPoint.setX(m);
        tmpPoint.setY(750);
        initObstaclePoints.push_back(tmpPoint);
    }

    for (int n = 750; n > 500; n --){
        QPoint tmpPoint;
        tmpPoint.setX(350);
        tmpPoint.setY(n);
        initObstaclePoints.push_back(tmpPoint);
    }

    for (int p = 500; p < 750; p ++) {
        QPoint tmpPoint;
        tmpPoint.setX(p);
        tmpPoint.setY(350);
        initObstaclePoints.push_back(tmpPoint);
    }

    for (int q = 350; q > 100; q --) {
        QPoint tmpPoint;
        tmpPoint.setX(500);
        tmpPoint.setY(q);
        initObstaclePoints.push_back(tmpPoint);
    }

    for (int u = 750; u > 500; u --) {
        QPoint tmpPoint;
        tmpPoint.setX(u);
        tmpPoint.setY(500);
        initObstaclePoints.push_back(tmpPoint);
    }

    for (int v = 500; v < 750; v ++) {
        QPoint tmpPoint;
        tmpPoint.setX(750);
        tmpPoint.setY(v);
        initObstaclePoints.push_back(tmpPoint);
    }
    updateMap(initObstaclePoints);
}

std::list<node *> pathplan::GetPath(node &startPoint, node &endPoint, bool isIgnoreCorner)
{
    node *result=findPath(startPoint,endPoint,isIgnoreCorner);
    std::list<node *> path;
    //返回路径，如果没找到路径，返回空链表
    while(result)
    {
        path.push_front(result);
        result=result->parent;
    }
    return path;
}

node *pathplan::findPath(node &startPoint, node &endPoint, bool isIgnoreCorner)
{
    openList.push_back(new node(startPoint.x,startPoint.y)); //置入起点,拷贝开辟一个节点，内外隔离

    while(!openList.empty())
    {
        qDebug() << "openlist size: " << openList.size();
        auto curPoint=getLeastFpoint(); //找到F值最小的点
        openList.remove(curPoint); //从开启列表中删除
        closeList.push_back(curPoint); //放到关闭列表
        //1,找到当前周围八个格中可以通过的格子
        auto surroundPoints=getSurroundPoints(curPoint,isIgnoreCorner);
        for(auto &target:surroundPoints)
        {
            //2,对某一个格子，如果它不在开启列表中，加入到开启列表，设置当前格为其父节点，计算F G H
            if(!isInList(openList,target))
            {
                target->parent=curPoint;

                target->G=calcG(curPoint,target);
                target->H=calcH(target,&endPoint);
                target->F=calcF(target);

                openList.push_back(target);
            }
            //3，对某一个格子，它在开启列表中，计算G值, 如果比原来的大, 就什么都不做, 否则设置它的父节点为当前点,并更新G和F
            else
            {
                int tempG=calcG(curPoint,target);
                if(tempG<target->G)
                {
                    target->parent=curPoint;

                    target->G=tempG;
                    target->F=calcF(target);
                }
            }
            node *resPoint=isInList(openList,&endPoint);
            if(resPoint)
                return resPoint; //返回列表里的节点指针，不要用原来传入的endpoint指针，因为发生了深拷贝
        }
    }

    return NULL;


}

node *pathplan::getLeastFpoint()
{
    if(!openList.empty())
    {
        auto resPoint=openList.front();
        for(auto &point:openList)
            if(point->F<resPoint->F)
                resPoint=point;
        return resPoint;
    }
    return NULL;
}

node *pathplan::isInList(const std::list<node *> &list, const node *point) const
{
    //判断某个节点是否在列表中，这里不能比较指针，因为每次加入列表是新开辟的节点，只能比较坐标
    for(auto p:list)
        if(p->x==point->x&&p->y==point->y)
            return p;
    return NULL;
}

bool pathplan::isCanreach(const node *point, const node *target, bool isIgnoreCorner) const
{
    if(target->x<0||target->x>maze.size()-1
        ||target->y<0&&target->y>maze[0].size()-1
        ||maze[target->x][target->y]==1
        ||target->x==point->x&&target->y==point->y
        ||isInList(closeList,target)) //如果点与当前节点重合、超出地图、是障碍物、或者在关闭列表中，返回false
        return false;
    else
    {
        if(abs(point->x-target->x)+abs(point->y-target->y)==1) //非斜角可以
            return true;
        else
        {
            //斜对角要判断是否绊住
            if(maze[point->x][target->y]==0&&maze[target->x][point->y]==0)
                return true;
            else
                return isIgnoreCorner;
        }
    }
}

std::vector<node *> pathplan::getSurroundPoints(const node *point, bool isIgnoreCorner) const
{
    std::vector<node *> surroundPoints;

    for(int x=point->x-1;x<=point->x+1;x++)
        for(int y=point->y-1;y<=point->y+1;y++)
            if(isCanreach(point,new node(x,y),isIgnoreCorner))
                surroundPoints.push_back(new node(x,y));

    return surroundPoints;
}

int pathplan::calcG(node *temp_start, node *point)
{
    int extraG=(abs(point->x-temp_start->x)+abs(point->y-temp_start->y))==1?kCost1:kCost2;
    int parentG=point->parent==NULL?0:point->parent->G; //如果是初始节点，则其父节点是空
    return parentG+extraG;
}

int pathplan::calcH(node *point, node *end)
{
    //用简单的欧几里得距离计算H，这个H的计算是关键，还有很多算法，没深入研究^_^
    return sqrt((double)(end->x-point->x)*(double)(end->x-point->x)+(double)(end->y-point->y)*(double)(end->y-point->y))*kCost1;
}

int pathplan::calcF(node *point)
{
    return point->G+point->H;
}

void pathplan::updateMap(QVector<QPoint> obstanclePoints)
{
    for(QVector<QPoint>::iterator iter = obstanclePoints.begin();iter != obstanclePoints.end();iter++){
        auto x = unsigned(iter->x());
        auto y = unsigned(iter->y());
        maze[x][y] = 1;
    }
}

void pathplan::clearMap()
{
    planner->maze.resize(900,std::vector<int>(900,0));
}

void pathplan::Astar(QPoint start, QPoint end)
{
    node startNode = {start.x(),start.y()};
    node endNode = {end.x(),end.y()};
    //A*算法找寻路径
    path = GetPath(startNode,endNode,false);
    //打印
    for(auto &p:path)
        qDebug()<<'('<<p->x<<','<<p->y<<')'<<endl;
}
