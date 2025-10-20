#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStack>
#include "allwidget.h"
#include <QVector>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>

typedef enum _MenuStatus{
    NONE = 0,
    HMI = 1,
    PROG = 2
} MenuStatus;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    AllWidget *m_allWidget; // 主窗口中所有的控件都组成了这个对象
    int tmpIndexSetParameter; //在哪个元件widget上右键选择设置参数的，把设好的参数传给这个元件
    QWidget *getUiCentralWidget();
    QMenuBar *getMenuBar();
    QPoint pointMenuBar;
    bool flagMouseLineDrawing = false;
    QPoint lastMousePos;
    QPoint posMouseCurrent;

    Qt::DropAction actionMoveOrCopy; //
    QPoint netStartPoint=QPoint(120,40);
    QPoint netEndPoint=QPoint(880,750);
    void paintAllLine(QList<structLine *> listStructLine);
    void paintAllLineNet(QList<structLineNet> listStructLineNet);
    QPoint getNearestPointOfNet(QPoint pp);//给出一个点，获取到距离这个点最近的网格格点，这样是为了控件只能整数格子移动
    int widthOfNet=10;
    QString tmpWidgetDelet;//临时传递删除元件的对象
    QString tmpLineDelet;//临时传递删除直线的对象
    QString tmpNodeDelet;//临时传递删除节点的对象


    QPoint tmpPosAddNode;//添加节点传过来的临时鼠标位置
    void _DFS(QList <structLine*> listStructLineTmp,int n);
    // 输入直线集合，开始搜索直线的序号，返回这条直线能联通的所有直线的序号集合
    QList<int> DFS(QList <structLine*> listStructLineTmp,int n);
    bool way(QList <structLine*> listStructLineTmp,int i,int j);//判断序号为i,j的直线是否联通
    bool visited[100]={false};//最大支持100条线
    void setComponentPatameterByOtherMainwindow(structWidget *tmp);
    QList<structLineNet> creatNet(QPoint p1,QPoint p2,int widt);
    QList<structLineNet> listLineNet;//背景网格
    bool flagTurnOnNet=true;
    //progmode tools bar
    QVector<QWidget*> progBarWidgets;
    //progs buttons
    QVector<QPushButton*> progListButtons;
    MenuStatus mMenuStatus;
    int curProgIdx;
    QSettings*  mProgSaveFile;

protected:
    void mousePressEvent(QMouseEvent *event);      // 鼠标按下事件
    void dragEnterEvent(QDragEnterEvent *event);   // 拖动进入事件
    void dragMoveEvent(QDragMoveEvent *event);     // 拖动事件
    void dropEvent(QDropEvent *event);             // 放下事件
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void contextMenuEvent(QContextMenuEvent *event);

    void paintEvent(QPaintEvent *event);
    void hideProgEditFrame();
    void showProgEditFrame();
    void clearProgEditItems();

private slots:
    void on_pushButton_clicked();

    void on_actionDelet_triggered();

    void on_actionRotateLeft90_triggered();

    void on_actionAddNode_triggered();

    void on_actionSetParameter_triggered();

    void on_actionDeletLine_triggered();

    void on_actionDeletNode_triggered();

    void on_actionEditNode_triggered();

    void on_pushButtonHindNet_clicked();

    void on_actionOpenWindow_triggered();

    void on_pushButton_add_prog_clicked();

    void on_pushButton_2_clicked();

    void pushButton_prog_id_clicked();

    void on_pushButton_save_clicked();

private:
    Ui::MainWindow *ui;
    QString getConnectItemsByPort(structPort* port);
    void getConnectPortsByLine(structLine* line,QPoint startPoint,QList<int>* result);
    void getConnectPortsByPoint(QPoint point,QList<int>* result);
    void getConnectLinesPortsByLine(structLine* line, QPoint startPoint, QList<int>* result);
};

#endif // MAINWINDOW_H
