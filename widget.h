#ifndef WIDGET_H
#define WIDGET_H
#include <QWidget>
#include "pathplan.h"
#include <scene.h>
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
public:
    QPoint startPoint_;
    QPoint endPoint_;
signals:
    void startPlanningSignal(QPoint,QPoint);//启动子线程的信号
    void stopPlannningSignal();
public slots:
    void clickedUpdateMap_btn_slot();
    void closeUpdateMap_btn_slot();
    void returnDefaultMap_btn_slot();
    void setStart_btn_slot();
    void setEnd_btn_slot();
    void startPlan_btn_slot();
    void stopPlannningSlot();
    void clickedStartCoord_slot(QPoint point);
    void clickedEndCoord_slot(QPoint point);
    void showPath_btn_slot();
private:
    Ui::Widget *ui;
    scene *scene_;
};
#endif // WIDGET_H
