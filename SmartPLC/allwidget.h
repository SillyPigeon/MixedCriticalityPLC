#ifndef ALLWIDGET_H
#define ALLWIDGET_H

#include <QWidget>
#include "wire.h"

class MainWindow;

class AllWidget
{
public:
    AllWidget(MainWindow *mwindow);
    QWidget *addWidget(QString type, QWidget *widBack, QPoint position);
    QWidget *addLineWithName(QString name, QPoint pointStart, QPoint pointEnd);
    QWidget *addWidgetWithName(QString name, QString type, QWidget *widBack, QPoint position);
    bool isWidgetInnerOrOuter(QPoint posMouse);
    bool isWidgetRight(QPoint posMouse);   // 判断鼠标点击
    QList <structWidget*> listStructWidget; // 这个是每个元件的底下是Widget嘛，这个数组存了所有的
    // 元件的这些Widget，因为后面鼠标点击的是不是元件的空白处（底），时候会用于判断的

    QWidget *m_window;                                            // 所有控件依存的窗口,// here dont need,because have became addWidget function param
    MainWindow *m_mainwindow;
    int numComponent = 0;  // 窗口中总的元件数（包括左侧的），可以未连接
    int numNode=0;//窗口中节点数
    int cntAddNode=0;//防止添加的节点默认名字同名使用的
    QString arrayNameLeftComponent[50];   // 最大支持左边创建30个元件,仅仅存的是元件的表示名字，R,C等等，而不是对象名
    int numComponentExistLeft;  // 存在左边的元件数

    int getNumFromQString(QString str);
    QString getName(QString str);
    QString getTypeOfObjectName(QString str);//得到R_2_Port0的R
    int strToEveryDoubleNum(QString str, double *doubleNum);
    int getIdWidget(QString childName);
    bool isBlankOfWidget(QPoint posMouse);
    QPoint getOffset(QPoint pointMouse);
    bool isComponentExistLeft(QString childName);
    int getIndexFromListStructWidget(QString objName);// 根据对象名得到这个对象处于数组中的第几的序号
    bool isPortLabel(QPoint posMouse);
    bool isLabelLogoOrName(QPoint pointMouse);
    bool isLabelLogoOrName(QString childName);
    QString lastObjectName = "";
    bool isRoundOfPointP1(QPoint p1,QPoint pGoal,int d);//判断一个点pGoal是否在另外一个点p1附近的d的距离，是矩形的方式判断，不是圆形

    int lastLineClicked=-1;
    QList <structLine*> listStructLine; // 为了方便管理，直线(导线一个意思)也放在了这个类里面
    // 为什么这些结构体存的都是对象的地址呢，因为不同对象地址是唯一的，但是对象名却可以同名，使用会带来麻烦
    int lineWidth= 3;
    QList <structPort*> listStructPort;

    void selectdLineByObjName(QString objName);
    void selectLine(int whichLineSlected);
    int atWhichLine(QPoint pos);
    int atWhichPort(QPoint pos);
    bool isAtLine(QPoint pos);
    bool _isAtLine(structLine *linePara, int offset, QPoint pGoal);
    bool __isAtLine(QPoint p1, QPoint p2, int offset, QPoint pGoal);
    QList <QString> listSelectedElem;//总共选择的直线和元件，存的是他们的名字字符串
    int atWhichNode(QPoint posTmp);
    void deletNode(QString objName);
    structNode *findStructNode(QString objName);

    bool isHaveNodeOnThisLine(QString objName);
    int findLineByName(QString objName);
    structLine *findStructLine(QString objName);
    void deletLine(QString objName);
    void selectdWidgetByObjName(QString objName);
    void selectdWidget(int whichWidgetSlected);
    structWidget *findStructWidget(QString objName);
    void deletWidget(QString objName);
    QWidget *copyComponentRotate(structWidget *child);// 复制出一个完全同名的元件，但是是旋转了90度的
    QWidget *makeComponent0(QString type, QPoint pos, int n);
    QWidget *makeComponent90(QString type, QPoint pos, int n, structWidget *childTmp);
    QWidget *makeComponent180(QString type, QPoint pos, int n);
    QWidget *makeComponent270(QString type, QPoint pos, int n);
    structPort *findStructPort(QString objName);
    QWidget *addNode(QWidget *widBack, QPoint posN, QString nameNode);//在主窗口里添加一个节点，指定位置
    QList<structNode*> listStructNode;
    QList<int> findPortListByWidget(QString objNameWidget);
    // 给定一个元件底widget对象名，在listStructPort中找到它包含的端口的序号的集合,在移动控件的时候得把端口一起移走更新其端口坐标
    QString findPortLinkedNodeNameByComponentName(QString port,QString nameComponent);
    QPoint findPosOfWidgetOrLabel(QPoint posTmp);//根据鼠标点的位置，找到这个控件对象，然后得到其左上角坐标
    int cntAddComponent=0;//这个变量用于记录总共添加了多少次元件，因为考虑到删除元件后，再重新添加元件可能会有
    //与之前同名的可能，所以不能用元件数作为元件的objName设置，而应该用这个添加次数，才是一直递增的，不会同名
    int cntAddLine=0;//也是防止删除直线后出现同名现象
    QList<structLinePort*> getLinePortsAtLine(structLine* line);
};

#endif // ALLCOMPONENT_H
