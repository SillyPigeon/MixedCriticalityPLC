#ifndef WIRE_H
#define WIRE_H
#include <QPoint>
#include <QColor>
#include <QLabel>

typedef struct structWidget    // 控件节点结构体，输入输出算法节点全部
        // 统一使用这个节点了，用currWidgObjName来进行区分
{
    QString currWidgObjName;// 元件名字，就是元件底widget的对象名，下面的结构体也一样,这个不是显示的名字
    int state = 0;  // 代表这个元件的正常，左旋90度，180,270四种状态，取值为0,1,2,3
    QPoint point;   // 为这个元件底widget的坐标
    //得全部用字符串类型，因为写入txt时候，有些参数是空，如果用数字，空和0分不开

    QString name="";        // 元件名称
    QString value="";       // 元件值
    QString starValue="";   // 初值
    QString nOrder="";      // 阶次
    QString fz="";          // 幅值
    QString fre="";         // 频率
    QString cxw="";         // 初相位
    QString NADD="";        // N+控制
    QString NCUT="";        // N-控制
    QString k1="";          // k1变比系数
    QString G="";           // G回转器系数
    QString orderLeft="";   // 阶次（一次侧）
    QString orderRight="";  // 阶次（二次侧）
    QString hgxs="";        // 互感系数

}structWidget;

typedef struct structLine    // 直线结构体，内部成员会逐渐扩充
{
    QString currLineName;   // 给这条直线一个名字，方便以后各种使用
    QPoint pointStart;
    QPoint pointEnd;
    int lineWidth = 3;
    QColor lineColor = Qt::black;
    QString nodeNameLinked;//最后点击生成按钮会记录下这个值，最后哪个port连接这个line，就知道它连的哪个node

}structLine;

typedef struct structLineNet    // 网格直线结构体，窗口背景显示的网格，辅助连线
{
    QPoint pointStart;
    QPoint pointEnd;
}structLineNet;

typedef struct structLinePort    // 网格直线结构体，窗口背景显示的网格，辅助连线
{
    QPoint pointConnect;
    int    lineIdx;
}structLinePort;

typedef struct structPort    // 元件端口结构体,添加一个端口用的是liststructport.apped的形式添加的，而不需要计数变量
{
    QString portName;   // 给这个端口一个名字，方便以后各种使用
    QPoint posPort;
    QString nodeNameLinked;

}structPort;
typedef struct structNode
// 节点结构体，因为这是要创建widget的，所以跟component是一样过程，但是和line他们就不一样了
        // port也是要创建widget的，但是是跟component一起创建的，所以共用了component的计数变量numComponent
        // 所以创建node也得自己搞一个全局计数变量numNode
{
    QString currNodeObjName;   // 给这个节点一个名字，方便以后各种使用，是这个节点底widget的对象名，但是不是节点显示的名字
    QString name;//用户取的名字,也是显示的名字
    QPoint posNode;
    int indexLine=-1;//指示此节点在序号为几的直线上
}structNode;

class Wire
{
public:
    Wire();
};

#endif // WIRE_H
