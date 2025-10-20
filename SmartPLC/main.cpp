#include "mainwindow.h"
#include <QApplication>

//MainWindow *aGlobal;//用于把主窗口对象a给传出来，称为全局对象，别的类就可以使用里面的函数了，可以使用多窗口通信
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
//    aGlobal=&w;
    return a.exec();
}
