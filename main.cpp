#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();
}

//1-5   c a b b a
//6-10  a d a b b
//11-15 c a b d c
//16-20 b a b b d

//21-25 c d d b d
//26-30 d a c a d
//31-35 c d d c a
//36-40 a a b c a
//41-45 A E D B F
