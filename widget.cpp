#include "widget.h"
#include "ui_widget.h"
#include <QPainter>
#include <QPixmap>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->scene_ = new scene;
    this->ui->graphicsView->setScene(this->scene_);

    connect(ui->clickedUpdateMap_btn,SIGNAL(clicked()),this,SLOT(clickedUpdateMap_btn_slot()));
    connect(ui->closeUpdateMap_btn,SIGNAL(clicked()),this,SLOT(closeUpdateMap_btn_slot()));
    connect(ui->returnDefaultMap_btn,SIGNAL(clicked()),this,SLOT(returnDefaultMap_btn_slot()));
    connect(ui->setStart_btn,SIGNAL(clicked()),this,SLOT(setStart_btn_slot()));
    connect(ui->setEnd_btn,SIGNAL(clicked()),this,SLOT(setEnd_btn_slot()));
    connect(ui->startPlan_btn,SIGNAL(clicked()),this,SLOT(startPlan_btn_slot()));
    connect(this->scene_,SIGNAL(clickedStartCoord_sgl(QPoint)),this,SLOT(clickedStartCoord_slot(QPoint)));
    connect(this->scene_,SIGNAL(clickedEndCoord_sgl(QPoint)),this,SLOT(clickedEndCoord_slot(QPoint)));
    connect(this,SIGNAL(startPlanningSignal(QPoint,QPoint)),scene_->planner,SLOT(Astar(QPoint,QPoint))); //通过信号启动线程函数
    connect(this,SIGNAL(stopPlannningSignal()),this,SLOT(stopPlannningSlot()));
    connect(ui->showPath_btn,SIGNAL(clicked()),this,SLOT(showPath_btn_slot()));
    //connect(ui->stopThread_btn,SIGNAL(clicked()),this,SLOT(stopPlannning_btn_slot()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::clickedUpdateMap_btn_slot()
{
    //打开鼠标点击更新地图
    scene_->mouseUpdateFlag = 1;
}

void Widget::closeUpdateMap_btn_slot()
{
    //关闭鼠标点击更新地图
    scene_->mouseUpdateFlag = 0;
}

void Widget::returnDefaultMap_btn_slot()
{
    scene_->clear();
    scene_->planner->clearMap();
    scene_->planner->initObstacle();
    scene_->showMap();
}

void Widget::setStart_btn_slot()
{
    scene_->setStartFlag = 1;
    scene_->mouseUpdateFlag = 0;
}

void Widget::setEnd_btn_slot()
{
    scene_->setEndFlag = 1;
    scene_->mouseUpdateFlag = 0;
}

void Widget::startPlan_btn_slot()
{
    //scene_->Astar(scene_->startCoord,scene_->endCoord);
    if(ui->startPlan_btn->text() == "启动规划"){
        ui->startPlan_btn->setText("终止规划");
        scene_->thread->start();
        QPoint start,end;
        start.setX(ui->startX_ldt->text().toInt());
        start.setY(ui->startY_ldt->text().toInt());
        end.setX(ui->endX_ldt->text().toInt());
        end.setY(ui->endY_ldt->text().toInt());
        emit startPlanningSignal(start,end);
    }
    else if(ui->startPlan_btn->text() == "终止规划"){
        ui->startPlan_btn->setText("启动规划");
        //发送线程终止信号
        emit stopPlannningSignal();
    }
}

void Widget::stopPlannningSlot()
{
    scene_->thread->quit();
    scene_->thread->wait();
    qDebug()<<"线程安全退出";
}

void Widget::clickedStartCoord_slot(QPoint point)
{
    ui->startX_ldt->setText(QString::number(point.x()));
    ui->startY_ldt->setText(QString::number(point.y()));
}

void Widget::clickedEndCoord_slot(QPoint point)
{
    ui->endX_ldt->setText(QString::number(point.x()));
    ui->endY_ldt->setText(QString::number(point.y()));
}

void Widget::showPath_btn_slot()
{
    scene_->showPath();
}

