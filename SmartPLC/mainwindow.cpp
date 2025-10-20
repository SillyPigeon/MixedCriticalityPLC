#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "allwidget.h"
#include "wire.h"
#include "mainwindowsetcomponentparameter.h"
#include "DrawPlot/mainwindowdraw.h"

#include <stdlib.h>
#include <QLabel>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QPainter>
#include <QMimeData>
#include <QDrag>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QDir>
#include <QLineEdit>
#include <QInputDialog>
#include <QProcess>
#include <QLineEdit>
#include <QFileDialog>
#include <QDateTime>
#include <QStack>
#include <QScrollArea>

#define INVALID_PROG_ID         0xFF
#define FRAME_PROG_ITEMS        3
#define POWER_LINE_X            150
#define POWER_LINE_Y_START      50
#define POWER_LINE_Y_END        740
#define POWER_LINE_PORT_INDEX   1001

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->hmiWidget->hide();
    listLineNet=creatNet(netStartPoint,netEndPoint,widthOfNet);
    //    QColor color = QColor(Qt::white);
    //    QPalette p = this->palette();
    //    p.setColor(QPalette::Window,color);
    //    this->setPalette(p);// 设置背景色为白色，用样式表设置方式的话，我画直线就显示不出来

    m_allWidget = new AllWidget(this);
    setMouseTracking(true);
    ui->centralwidget->setMouseTracking(true);  // 注：由于主窗体表面有个centralWidget，所以得这样设置两个鼠标跟踪
    setAcceptDrops(true);                      // 设置窗口部件可以接收拖入

    pointMenuBar = QPoint(0,ui->menubar->height());  // 因为经过反复测试发现菜单栏会占据一个高度，但是绘图没有体现出来

    //init widiget
    int cntTmp=0;
    mMenuStatus = NONE;
    curProgIdx = INVALID_PROG_ID;
    progBarWidgets.reserve(10);
    progListButtons.reserve(10);

    //Delete at first
    QFile file("progSave.ini");
    if (file.exists()){
        file.remove();
    }
    mProgSaveFile = new QSettings("progSave.ini", QSettings::IniFormat);
    mProgSaveFile->setValue("/con/editor", "smartPLC");
    delete mProgSaveFile;
    mProgSaveFile = nullptr;

    QWidget* tempWdgP;
    tempWdgP = m_allWidget->addWidget("R",ui->centralwidget,QPoint(890,40));
    m_allWidget->arrayNameLeftComponent[cntTmp++] = "R";
    this->findChild<QLabel *>(tempWdgP->objectName()+"_name")->setText("线圈");
    progBarWidgets.push_back(tempWdgP);

    tempWdgP = m_allWidget->addWidget("C",ui->centralwidget,QPoint(890,120));
    m_allWidget->arrayNameLeftComponent[cntTmp++] = "C";
    this->findChild<QLabel *>(tempWdgP->objectName()+"_name")->setText("常开开关");
    progBarWidgets.push_back(tempWdgP);

    tempWdgP = m_allWidget->addWidget("FC",ui->centralwidget,QPoint(890,200));
    m_allWidget->arrayNameLeftComponent[cntTmp++] = "FC";
    this->findChild<QLabel *>(tempWdgP->objectName()+"_name")->setText("常闭开关");
    progBarWidgets.push_back(tempWdgP);

    m_allWidget->addLineWithName(QString("line_power"),
                                 QPoint(POWER_LINE_X, POWER_LINE_Y_START), QPoint(POWER_LINE_X, POWER_LINE_Y_END));
    update();

    m_allWidget->numComponentExistLeft = m_allWidget->numComponent;
    QString widNameTmp=m_allWidget->listStructWidget.at(0)->currWidgObjName;
    QWidget *childTmp = this->findChild<QWidget *>(widNameTmp);
}
QWidget *MainWindow::getUiCentralWidget()
{
    return ui->centralwidget;
}
QMenuBar *MainWindow::getMenuBar()
{
    return ui->menubar;
}

void MainWindow::mousePressEvent(QMouseEvent *event)   // 鼠标按下事件,注：左键点击了连线标签，不能马上进行update()
{                                                      // 操作，因为这时候直线绘制的终点坐标还没有确定，系统默认为0,0
    if(event->button() == Qt::LeftButton)              // 所以会出现一瞬间绘制到原点的直线的情况
    {
        QPoint posMouse = event->pos();
        QWidget *child = static_cast<QWidget*>(childAt(posMouse));
        // 第二步：自定义MIME类型
        QByteArray itemData;                                     // 创建字节数组
        QDataStream dataStream(&itemData, QIODevice::WriteOnly); // 创建数据流
        // 将图片信息，位置信息输入到字节数组中
        // 说明移动的时候点击的不是元件的空白处（widget），而是标签了，但是我们传过去坐标一定得是
        // 元件空白处（widget）的坐标才行，因为后面就是用这个坐标来移动元件的，不然就是出现坐标偏差问题（我是调试了好久才发现这个问题的！！！)
        QWidget *childTmp;//这个变量的目的是不想影响child的值
        if(m_allWidget->isLabelLogoOrName(child->objectName()) == true)
        {
            QLabel *lab = this->findChild<QLabel *>(child->objectName());
            childTmp = lab->parentWidget();    // change to label's parent widget,because need move widget
        }
        else
        {
            childTmp = this->findChild<QWidget *>(child->objectName());
        }// 现在childTmp已经是元件空白处（widget）的指针了，而child还是原来的指针
        QPoint positionWidgetComponent = childTmp->pos();//////////这个地方有问题！！！
        QPoint offset = m_allWidget->getOffset(posMouse);
        // 控件的左上角位置，offset：鼠标和左上角距离差
        QString childName = child->objectName();
        dataStream << positionWidgetComponent<<offset<<childName;  // 注：这里只能首先输入坐标数据，因为后面也是按照顺序拿的数据
        //传过去的东西如下：拖动元件的widget坐标，鼠标位置和元件的widget坐标差，拖动时候点击的对象名（可能是标签的对象名）
        // 还有个getIdWidget(childName)
        /***************************************************************************/
        /* 单击了控件内部任何位置 */
        /***************************************************************************/
        if(m_allWidget->isWidgetInnerOrOuter(posMouse) == true)    //
        {
            /***************************************************************************/
            /* 左侧的控件 */
            /***************************************************************************/
            if(m_allWidget->isWidgetRight(posMouse) == false)  //
            {

                actionMoveOrCopy = Qt::CopyAction;
                dataStream<<m_allWidget->getIdWidget(childName);   // if
                // 第三步：将数据放入QMimeData中
                QMimeData *mimeData = new QMimeData;  // 创建QMimeData用来存放要移动的数据
                // 将字节数组放入QMimeData中，这里的MIME类型是我们自己定义的
                mimeData->setData("myimage/png", itemData);

                // 第四步：将QMimeData数据放入QDrag中
                QDrag *drag = new QDrag(this);      // 创建QDrag，它用来移动数据
                drag->setMimeData(mimeData);
                drag->setHotSpot(posMouse - child->pos()); // 拖动时鼠标指针的位置不变
                // 第六步：执行拖放操作   这里我把它改为复制操作了
                if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::MoveAction)== Qt::MoveAction)  // 设置拖放可以是移动和复制操作，默认是复制操作
                {
                    // child->close();         // 如果是移动操作，那么拖放完成后关闭原标签
                }
                else
                {
                    child->show();            // 如果是复制操作，那么拖放完成后显示标签
                }

            }
            /***************************************************************************/
            /* 右侧控件 */
            /***************************************************************************/
            else if(m_allWidget->isWidgetRight(posMouse) == true) //
            {
                /***************************************************************************/
                /* 点击的是控件上面的端口标签位置 */
                /***************************************************************************/
                if(m_allWidget->isWidgetInnerOrOuter(posMouse) == true&&m_allWidget->isPortLabel(posMouse) == true)
                {
                    //之前没有连端口且直线,现在连了端口
                    if((m_allWidget->lastObjectName=="")&&(m_allWidget->lastLineClicked==-1))
                    {
                        m_allWidget->lastObjectName = childName;
                        QPoint posTmp1 = child->parentWidget()->pos();
                        QPoint posTmp2 = child->pos()+QPoint(child->width()/2,child->height()/2);//线的起点是端口的中心位置
                        QPoint posTmp = posTmp1+posTmp2;
                        qDebug()<<"点击的端口名称"<<child->objectName()<<endl;
                        lastMousePos = posTmp;
                        posMouseCurrent = lastMousePos; // attention: here must set posMouseCurrent = lastMousePos,if not, will a line exist
                        flagMouseLineDrawing = true;    // 鼠标动态画线开始
                    }
                    qDebug()<<"m_allWidget->lastObjectName:"<<m_allWidget->lastObjectName<<endl;
                    //之前连了端口或者直线,现在连了端口
                    if((m_allWidget->lastObjectName!= child->objectName())||(m_allWidget->lastLineClicked!=-1))    // 线连接成功，需要绘制直线了
                    {
                        QWidget *wid2;
                        QLabel *label2 = this->findChild<QLabel *>(child->objectName());
                        QString label2Name=label2->objectName();
                        qDebug()<<"label2Name:"<<label2Name<<endl;

                        wid2 = label2->parentWidget();
                        QPoint pointEndTmp = label2->pos()+QPoint(label2->width()/2,label2->height()/2)+wid2->pos();

                        flagMouseLineDrawing = false;   // 鼠标动态画线结束
                        qDebug()<<pointEndTmp<<endl;


                        structLine*tmpLine = new structLine;
                        tmpLine->currLineName = "line_"
                                + QString::number(m_allWidget->cntAddLine);
                        tmpLine->pointStart = lastMousePos;
                        tmpLine->pointEnd = pointEndTmp ;
                        // 直线的起点和终点一定得刚好在端口的左上角，这样最后的搜索才准确
                        m_allWidget->cntAddLine++;
                        m_allWidget->listStructLine.append(tmpLine);  // 记录直线

                        update();
                        // 等到算法连接成功后，才更新界面连线
                        m_allWidget->lastObjectName = "";
                        m_allWidget->lastLineClicked=-1;
                    }
                }
                /***************************************************************************/
                /* 点击的是控件的空位置（不是任何标签和编辑框的位置） */
                /***************************************************************************/
                else if(m_allWidget->isBlankOfWidget(posMouse) ==true || m_allWidget->isLabelLogoOrName(posMouse) == true)      //
                {
                    qDebug()<<"单击了控件"<<endl;
                    //说明鼠标left键在控件位置,select this widget
                    // child here should is widget not label,so we should judge it whether switch to widget

                    if(m_allWidget->isLabelLogoOrName(child->objectName()) == true)
                    {
                        QLabel *labelLogo = this->findChild<QLabel *>(child->objectName());
                        child = labelLogo->parentWidget();    // change to label's parent widget,because need move widget
                    }
                    else
                    {
                        child = this->findChild<QWidget *>(child->objectName());
                    }

                    QList<int> portListTmp=m_allWidget->findPortListByWidget(child->objectName());
                    for (int j = 0; j < portListTmp.size(); ++j) {
                        //                        qDebug()<<"此端口坐标"<<
                        int indexTmp=portListTmp.at(j);
                        if(m_allWidget->atWhichLine(m_allWidget->listStructPort.at(indexTmp)->posPort)>=0)
                        {
                            qDebug()<<"单击了控件,但是此控件有线连着，所有不能移动，本函数结束"<<endl;
                            QMessageBox::warning(this,"警告","元件连着导线，所以不能拖动",QMessageBox::Abort);

                            return;// 这样写法还是不错的
                        }
                    }//单击了控件,发现此元件的端口中有个是有线连着，所有不能移动此元件，本函数结束，停止继续往下运行


                    int w = -1;
                    for (int i = 0; i < m_allWidget->listStructWidget.size(); ++i) {
                        if(m_allWidget->listStructWidget.at(i)->currWidgObjName == child->objectName())
                        {
                            w=i;
                            break;
                        }
                    }
                    /**********************************************************************/

                    actionMoveOrCopy = Qt::MoveAction;
                    dataStream<<m_allWidget->getIdWidget(childName);   // if
                    // 第三步：将数据放入QMimeData中
                    QMimeData *mimeData = new QMimeData;  // 创建QMimeData用来存放要移动的数据
                    // 将字节数组放入QMimeData中，这里的MIME类型是我们自己定义的
                    mimeData->setData("myimage/png", itemData);

                    // 第四步：将QMimeData数据放入QDrag中
                    QDrag *drag = new QDrag(this);      // 创建QDrag，它用来移动数据
                    drag->setMimeData(mimeData);
                    drag->setHotSpot(posMouse - child->pos()); // 拖动时鼠标指针的位置不变
                    // 第六步：执行拖放操作   这里我把它改为复制操作了
                    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::MoveAction)== Qt::MoveAction)  // 设置拖放可以是移动和复制操作，默认是复制操作
                    {
                        // child->close();         // 如果是移动操作，那么拖放完成后关闭原标签
                    }
                    else
                    {
                        child->show();            // 如果是复制操作，那么拖放完成后显示标签
                    }

                }
            }
        }
        /***************************************************************************/
        /* 控件外部 */
        /***************************************************************************/
        else if(m_allWidget->isWidgetInnerOrOuter(posMouse) == false)   //
        {
            qDebug()<<"单击了空白"<<endl;

            /***************************************************************************/
            /* 单击在直线上 */
            /***************************************************************************/
            if(m_allWidget->isAtLine(posMouse) == true)   //
            {
                //前面没有连接端口和直线,现在点了直线
                if((m_allWidget->lastLineClicked==-1)&&(m_allWidget->lastObjectName==""))//点击在直线上，重新开始新的一条直线的条件是，之前没有点击端口且直线
                {
                    m_allWidget->lastLineClicked = m_allWidget->atWhichLine(posMouse);
                    lastMousePos = getNearestPointOfNet(posMouse);
                    posMouseCurrent = lastMousePos; // attention: here must set posMouseCurrent = lastMousePos,if not, will a line exist
                    flagMouseLineDrawing = true;    // 鼠标动态画线开始
                }
                //前面连了端口或者直线，现在点了直线
                else if((m_allWidget->atWhichLine(posMouse)!=m_allWidget->lastLineClicked)||(m_allWidget->lastObjectName!=""))
                {
                    flagMouseLineDrawing = false;   // 鼠标动态画线结束
                    structLine*tmpLine = new structLine;
                    tmpLine->currLineName = "line_" + QString::number(m_allWidget->cntAddLine);
                    tmpLine->pointStart = lastMousePos;
                    tmpLine->pointEnd = getNearestPointOfNet(posMouse) ;
                    // 直线的起点和终点一定得刚好在端口的左上角，这样最后的搜索才准确
                    m_allWidget->cntAddLine++;
                    m_allWidget->listStructLine.append(tmpLine);  // 记录直线
                    update();
                    // 等到算法连接成功后，才更新界面连线
                    m_allWidget->lastLineClicked=-1;
                    m_allWidget->lastObjectName="";
                }
            }
            /***************************************************************************/
            /* 单击在直线外 */
            /***************************************************************************/
            else if(m_allWidget->isAtLine(posMouse) == false)  //
            {
                if(m_allWidget->lastObjectName!= ""||m_allWidget->lastLineClicked!=-1)    // 说明端口或者直线上正在引出了线，需要绘制直线了
                {
                    if(m_allWidget->isRoundOfPointP1(posMouseCurrent,posMouse,5))//说明此时鼠标点了出来的线的头头，这样可以让这条线存在,即添加直线
                    {
                        structLine*tmpLine = new structLine;
                        tmpLine->currLineName = "line_" + QString::number(m_allWidget->cntAddLine);
                        tmpLine->pointStart = lastMousePos;
                        tmpLine->pointEnd = posMouseCurrent ;
                        // 直线的起点和终点一定得刚好在端口的左上角，这样最后的搜索才准确
                        m_allWidget->cntAddLine++;
                        m_allWidget->listStructLine.append(tmpLine);  // 记录直线
                        // 等到算法连接成功后，才更新界面连线
                    }

                    flagMouseLineDrawing =false;
                    m_allWidget->lastObjectName= "";
                    m_allWidget->lastLineClicked=-1;
                    update();
                }


            }
        }
        /*----------------------------@ 这是一个分隔符 @-----------------------------*/
        /***************************************************************************/
    }
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    // 如果有我们定义的MIME类型数据，则进行移动操作
    if (event->mimeData()->hasFormat("myimage/png")) {
        event->setDropAction(actionMoveOrCopy);
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("myimage/png")) {
        event->setDropAction(actionMoveOrCopy);
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("myimage/png")) {

        QByteArray itemData = event->mimeData()->data("myimage/png");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);
        QPoint offset,positionLastOfWidgetComponent;//这是被拖到元件原来的底widget的坐标
        QString childName;
        QWidget *child;
        QLabel *labelLogo;
        int i = -1;
        // 使用数据流将字节数组中的数据读入到QPixmap和QPoint变量中
        dataStream >> positionLastOfWidgetComponent>>offset>>childName>>i;
        qDebug()<<"childName:"<<childName<<endl;
        // child here should is widget not label,so we should judge it whether switch to widget
        if(m_allWidget->isLabelLogoOrName(childName) == true)
        {
            labelLogo = this->findChild<QLabel *>(childName);
            child = labelLogo->parentWidget();    // change to label's parent widget,because need move widget
        }
        else
        {
            child = this->findChild<QWidget *>(childName);
        }
        QPoint positionCurrent = event->pos() - offset;
        positionCurrent=getNearestPointOfNet(positionCurrent);//只能停靠在网格上
        if(positionCurrent == positionLastOfWidgetComponent)
        {
            qDebug()<<"鼠标上次位置和这次位置一样，不能拖动，返回"<<endl;
            return;
        }
        QString operat = childName;
        if(operat.contains("_"))
        {
            int index = operat.indexOf("_");
            operat.truncate(index);        // 由于operate是add_8这样的，这里需要开始调用toolbox了，需要去掉_8
        }
        if(m_allWidget->isComponentExistLeft(childName)) // childName已经是去掉后缀id的名字了
        {
            if(i>=m_allWidget->numComponentExistLeft)   // 如果是移动的话，这里的i是被移动的控件的id，会大于左边候选控件的个数的
            {
                child->move(positionCurrent.x(),positionCurrent.y());   // 先移动控件，再移动对应链接的直线
                m_allWidget->listStructWidget.at(m_allWidget->getIndexFromListStructWidget(child->objectName()))->point=QPoint(positionCurrent.x(),positionCurrent.y());
                QList<int> portListTmp=m_allWidget->findPortListByWidget(child->objectName());
                for (int j = 0; j < portListTmp.size(); ++j) {
                    m_allWidget->listStructPort.at(portListTmp.at(j))->posPort=m_allWidget->listStructPort.at(portListTmp.at(j))->posPort+positionCurrent-positionLastOfWidgetComponent;
                }//更新完毕在listStructPort中此元件的所有端口的坐标
                child->show();
                update();

            }
            else//是拖动也就是复制左边的元件的话
            {
                m_allWidget->addWidget(operat,ui->centralwidget,positionCurrent);
                update();
            }

        }
        qDebug()<<actionMoveOrCopy<<endl;
        event->setDropAction(actionMoveOrCopy);
        event->accept();
    }

    else
    {
        event->ignore();
    }

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if((m_allWidget->lastObjectName != "")||(m_allWidget->lastLineClicked!=-1))
    {
        if(abs(event->pos().x()-lastMousePos.x())>=abs(event->pos().y()-lastMousePos.y()))//鼠标在在右侧
        {
            posMouseCurrent=getNearestPointOfNet(QPoint(event->pos().x(),lastMousePos.y()));
        }
        else
        {
            posMouseCurrent=getNearestPointOfNet(QPoint(lastMousePos.x(),event->pos().y()));
        }
        qDebug()<<"posMouseCurrent"<<posMouseCurrent<<"event->pos()"<<event->pos()<<endl;
        update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{

}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint posMouse = event->pos() ;    // 鼠标当前坐标
    QMenu *menuLine = new QMenu(this);

    if(m_allWidget->isBlankOfWidget(posMouse) ==true || m_allWidget->isLabelLogoOrName(posMouse) == true)      //
    {
        QWidget *child = static_cast<QWidget*>(childAt(event->pos()));
        // child here should is widget not label,so we should judge it whether switch to widget
        if(m_allWidget->isLabelLogoOrName(child->objectName()) == true)
        {
            QLabel *labelLogo = this->findChild<QLabel *>(child->objectName());
            child = labelLogo->parentWidget();    // change to label's parent widget,because need move widget
        }
        else
        {
            child = this->findChild<QWidget *>(child->objectName());
        }
        int w = -1;
        for (int i = 0; i < m_allWidget->listStructWidget.size(); ++i) {
            if(m_allWidget->listStructWidget.at(i)->currWidgObjName == child->objectName())
            {
                w=i;
                break;
            }
        }
        tmpWidgetDelet=child->objectName();
        menuLine->addAction(ui->actionDelet);//删除元件的
        //说明元件还未旋转过，而且不是示波器元件（示波器不支持旋转，太丑了），则可以进行旋转
        if((m_allWidget->listStructWidget.at(w)->state==0) && (!m_allWidget->listStructWidget.at(w)->name.contains("Scope")))
            menuLine->addAction(ui->actionRotateLeft90);

        tmpIndexSetParameter=w;
        menuLine->addAction(ui->actionSetParameter);
        if(m_allWidget->listStructWidget.at(w)->name.contains("Scope"))
            menuLine->addAction(ui->actionOpenWindow);

        menuLine->move(cursor().pos());
        menuLine->show();
    }
    else if(m_allWidget->atWhichNode(posMouse)>=0)
    {
        tmpNodeDelet=m_allWidget->listStructNode.at(m_allWidget->atWhichNode(posMouse))->currNodeObjName;
        menuLine->addAction(ui->actionEditNode);
        menuLine->addAction(ui->actionDeletNode);//删除节点的
        menuLine->move(cursor().pos());
        menuLine->show();
    }
    else if(m_allWidget->isAtLine(posMouse))//右键在直线上，说明想添加节点
    {
        tmpPosAddNode=posMouse;
        tmpLineDelet=m_allWidget->listStructLine.at(m_allWidget->atWhichLine(posMouse))->currLineName;
        menuLine->addAction(ui->actionDeletLine);//删除直线的
        menuLine->addAction(ui->actionAddNode);
        menuLine->move(cursor().pos());
        menuLine->show();
    }

}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if(flagTurnOnNet)
        paintAllLineNet(listLineNet);
    //paint action
    paintAllLine(m_allWidget->listStructLine);

    if(flagMouseLineDrawing == true)
    {
        if((m_allWidget->lastObjectName != "")||(m_allWidget->lastLineClicked!=-1))
        {
            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing,true);
            QPen pen = painter.pen();
            pen.setWidth(m_allWidget->lineWidth);
            painter.setPen(pen);
            painter.drawLine(lastMousePos,posMouseCurrent);    // 后面为什么不用加上菜单栏高度呢，因为这个是用实时鼠标坐标，是正确的
        }
    }
}
void MainWindow::paintAllLine(QList <structLine*> listStructLine)
{
    QPainter painterPara(this);
    painterPara.setRenderHint(QPainter::Antialiasing,true);
    QPen pen = painterPara.pen();
    pen.setWidth(m_allWidget->lineWidth);

    for (int i = 0; i < listStructLine.size(); ++i) {
        pen.setColor(listStructLine.at(i)->lineColor);
        painterPara.setPen(pen);
        painterPara.drawLine(listStructLine.at(i)->pointStart,listStructLine.at(i)->pointEnd);
    }
}

void MainWindow::paintAllLineNet(QList<structLineNet> listStructLineNet)
{
    QPainter painterPara(this);
    painterPara.setRenderHint(QPainter::Antialiasing,true);
    QPen pen = painterPara.pen();
    pen.setWidth(2);

    for (int i = 0; i < listStructLineNet.size(); ++i) {
        pen.setColor(QColor(220,220,220));
        painterPara.setPen(pen);
        painterPara.drawLine(listStructLineNet.at(i).pointStart,listStructLineNet.at(i).pointEnd);
    }
}

QPoint MainWindow::getNearestPointOfNet(QPoint pp)
{
    QPoint res;
    int n1=(pp.y()-netStartPoint.y())/widthOfNet;
    int v=netStartPoint.y()+n1*widthOfNet;//得到鼠标y坐标的上面一条线的那个y
    int c=pp.y()-v;
    if(c<=(widthOfNet/2))
        res.setY(v);//如果鼠标的y更加靠近上面一条直线的y，那就选这条直线的y坐标作为停靠坐标
    else
        res.setY(netStartPoint.y()+(n1+1)*widthOfNet);//如果鼠标的y更加靠近下面一条直线的y，那就选这条直线的y坐标作为停靠坐标
    //现在设置x停靠
    n1=(pp.x()-netStartPoint.x())/widthOfNet;
    v=netStartPoint.x()+n1*widthOfNet;//得到鼠标y坐标的上面一条线的那个y
    c=pp.x()-v;
    if(c<=(widthOfNet/2))
        res.setX(v);//如果鼠标的y更加靠近上面一条直线的y，那就选这条直线的y坐标作为停靠坐标
    else
        res.setX(netStartPoint.x()+(n1+1)*widthOfNet);//如果鼠标的y更加靠近下面一条直线的y，那就选这条直线的y坐标作为停靠坐标
    return res;
}

void MainWindow::_DFS(QList<structLine *> listStructLineTmp, int n)
{
    visited[n]=true;
    for (int i = 0; i < listStructLineTmp.size(); ++i) {
        if((visited[i]==false)&&way(listStructLineTmp,n,i))//必须是未访问过的，不然会一直循环
            _DFS(listStructLineTmp,i);
    }
}

QList<int> MainWindow::DFS(QList<structLine *> listStructLineTmp, int n)
{
    for (int i = 0; i < listStructLineTmp.size(); ++i) {
        visited[i]=false;//得清空这个全局数组才行，不然后面的节点的遍历会受影响
    }
    _DFS(listStructLineTmp,n);
    QList<int> lines;
    for (int i = 0; i < listStructLineTmp.size(); ++i) {
        if(visited[i]==true)
            lines.append(i);
    }
    return lines;
}

bool MainWindow::way(QList<structLine *> listStructLineTmp, int i, int j)
{
    if(i==j)
        return false;
    else if(m_allWidget->atWhichLine(listStructLineTmp.at(i)->pointStart)==j)
        return true;
    else if(m_allWidget->atWhichLine(listStructLineTmp.at(i)->pointEnd)==j)
        return true;
    else if(m_allWidget->atWhichLine(listStructLineTmp.at(j)->pointStart)==i)
        return true;
    else if(m_allWidget->atWhichLine(listStructLineTmp.at(j)->pointEnd)==i)
        return true;
    else
        return false;
}

void MainWindow::setComponentPatameterByOtherMainwindow(structWidget *tmp)
{
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->name=tmp->name;
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->value=tmp->value;
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->starValue=tmp->starValue;
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->nOrder=tmp->nOrder;
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->fz=tmp->fz;
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->fre=tmp->fre;
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->cxw=tmp->cxw;
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->NADD=tmp->NADD;
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->NCUT=tmp->NCUT;
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->k1=tmp->k1;
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->G=tmp->G;
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->orderLeft=tmp->orderLeft;
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->orderRight=tmp->orderRight;
    m_allWidget->listStructWidget.at(tmpIndexSetParameter)->hgxs=tmp->hgxs;





    delete tmp;//记得释放掉不用的对象，否则内存泄漏，好像我前面的list的使用不知道是不是也得释放，不然造成内存不足
    //根据备选元件的对象名找到此widget的地址，然后找到它包含的子控件中的name标签即可
    QWidget *wid = findChild<QWidget *>(m_allWidget->listStructWidget.at(tmpIndexSetParameter)->currWidgObjName);
    QList<QLabel *> lab = wid->findChildren<QLabel*>();
    for (int i = 0; i < lab.size(); ++i) {
        if(lab.at(i)->objectName().contains("name"))//说明找到了元件中的name标签，更新他的显示了
            lab.at(i)->setText(m_allWidget->listStructWidget.at(tmpIndexSetParameter)->name);
    }
}

QList<structLineNet> MainWindow::creatNet(QPoint p1, QPoint p2, int widt)
{
    QList<structLineNet> res;
    int n1=(p2.x()-p1.x())/widt+1;//有多少条竖线
    int n2=(p2.y()-p1.y())/widt+1;//有多少横线
    structLineNet net;
    for (int i = 0; i < n1; ++i) {//先画竖线
        net.pointStart=QPoint(p1.x()+i*widt,p1.y());
        net.pointEnd=QPoint(p1.x()+i*widt,p2.y());
        res.append(net);
    }
    for (int i = 0; i < n2; ++i) {//画横线
        net.pointStart=QPoint(p1.x(),p1.y()+i*widt);
        net.pointEnd=QPoint(p2.x(),p1.y()+i*widt);
        res.append(net);
    }
    return res;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getConnectLinesPortsByLine(structLine* line, QPoint startPoint, QList<int>* result){
    QList<structLinePort*> linePorts = m_allWidget->getLinePortsAtLine(line);
    qDebug()<<"[DEBUG]check in: "<< line->currLineName << endl;
    foreach(structLinePort* linePort, linePorts){
        if((linePort->pointConnect == startPoint) || (linePort->pointConnect == startPoint)){
            qDebug()<<"[DEBUG]no check the start Point: "<< startPoint << endl;
            continue;
        }
        qDebug()<<"[DEBUG]connect in: "<< m_allWidget->listStructLine.at(linePort->lineIdx)->currLineName << endl;
        getConnectPortsByLine(m_allWidget->listStructLine.at(linePort->lineIdx), linePort->pointConnect, result);
    }
}

void MainWindow::getConnectPortsByPoint(QPoint point,QList<int>* result){
    int findIdx = m_allWidget->atWhichPort(point);
    if(findIdx == -1){
        //check if connect line;
        int lineIndex = m_allWidget->atWhichLine(point);
        // qDebug()<<"[DEBUG]find the index: "<< lineIndex << endl;
        // qDebug()<<"[DEBUG]find the LINE : "<< m_allWidget->listStructLine.at(lineIndex)->currLineName << endl;
        structLine* line = m_allWidget->listStructLine.at(lineIndex);
        if((point == line->pointStart) || (point == line->pointEnd)){
            qDebug()<<"[ERROR] Line point did not connect the line, error! "<< endl;
            return;
        }
        if(lineIndex == 0){
            //connect to power Line;
            if(result->indexOf(POWER_LINE_PORT_INDEX) < 0){
                result->append(POWER_LINE_PORT_INDEX);
            }
        }else if(lineIndex > 0){
            getConnectPortsByLine(m_allWidget->listStructLine.at(lineIndex), point, result);
        }else{
            //found nothing connect
            //qDebug()<<"[WARNING] Line point did not connect anything "<< endl;
        }
    }else{
        if(result->indexOf(findIdx) < 0){
            result->append(findIdx);
        }
    }
}

void MainWindow::getConnectPortsByLine(structLine* line,QPoint startPoint,QList<int>* result){
    if(result == NULL){
        return;
    }
    //check pointStart
    if(line->pointStart != startPoint){
        getConnectPortsByPoint(line->pointStart, result);
    }
    //check pointEnd
    if(line->pointEnd != startPoint){
        getConnectPortsByPoint(line->pointEnd, result);
    }
    //check lines
    getConnectLinesPortsByLine(line, startPoint, result);
}

QString MainWindow::getConnectItemsByPort(structPort* port){
    QString tmpList = "";
    QList<int>  cnPortList;
    int lineIndex = m_allWidget->atWhichLine(port->posPort);
    if(lineIndex <= 0){
        qDebug()<<"[ERROR]not found line connect in port: "<< port->portName << endl;
        return tmpList;
    }
    structLine* connectLine = m_allWidget->listStructLine.at(lineIndex);
    getConnectPortsByLine(connectLine, port->posPort, &cnPortList);
    qSort(cnPortList.begin(), cnPortList.end(), [](int& a, int& b){return a>b;});
    for (int i = 0; i < cnPortList.size(); ++i) {
        int portIdx = cnPortList.at(i);
        if(portIdx == POWER_LINE_PORT_INDEX){
            //qDebug()<<"[DEBUG][getConnectItemsByPort]find the connect line power " << endl;
            if(i < cnPortList.size() - 1){
                tmpList = tmpList + "power-";
            }else{
                tmpList = tmpList + "power";
            }
        }else{
            //qDebug()<<"[DEBUG][getConnectItemsByPort]find the connectPort: "<< m_allWidget->listStructPort.at(i)->portName << endl;
            QString portName = m_allWidget->listStructPort.at(portIdx)->portName;
            QStringList nameList = portName.split("_");
            structWidget* unit = m_allWidget->findStructWidget(nameList[0] + "_" + nameList[1]);
            if(i < cnPortList.size() - 1){
                tmpList = tmpList + unit->name + "-";
            }else{
                tmpList = tmpList + unit->name;
            }
        }
    }
    return tmpList;
}

//计算生成PLC的指令
void MainWindow::on_pushButton_clicked()
{
    //qDebug() << "port size : " << m_allWidget->listStructPort.size() << endl;

    QList<structWidget*> plcList;
    foreach(structWidget* i, m_allWidget->listStructWidget){
        if(("R_0" == i->name) || ("C_1" == i->name) || ("FC_2" == i->name))
            continue;
        plcList.append(i);
    }

    qSort(plcList.begin(), plcList.end(),
    [](const structWidget* u1, const structWidget* u2){
        //qDebug()<<"[DEBUG]compared a:b "<< u1->name << ":" << u2->name << endl;
        if(u1->point.y() < u2->point.y()){
            return true;
        }
        if(u1->point.y() == u2->point.y()){
            return u1->point.x() < u2->point.x();
        }
        return false;
    });

    // [[command]:[data]] cmd:unit_start
    // [[command]:[data]] unit:indix_unitName_unitType_connectIO
    // [[command]:[data]] inPort:unitName1_unitName2_...
    // [[command]:[data]] outPort:unitName1_unitName2_...
    // [[command]:[data]] cmd:unit_end
    QStringList transCommand;
    foreach(structWidget* i, plcList){
        QList<int> portList=m_allWidget->findPortListByWidget(i->currWidgObjName);
        transCommand.append("cmd:unit_start");
        //1-unitCommand
        QString unitCommand = "unit:" + QString::number(plcList.indexOf(i)) + "-"
                              + i->name + "-" + i->currWidgObjName.split("_")[0] + "-"
                              + i->value;
        transCommand.append(unitCommand);
        //2-inPortCommand
        qDebug()<<"[DEBUG]compute inPortCommand start: " << endl;
        structPort* inPort = m_allWidget->listStructPort.at(portList.at(0));
        QString inPortCommand = getConnectItemsByPort(inPort);
        transCommand.append("inPort:" + inPortCommand);
        //3-outPortCommand
        qDebug()<<"[DEBUG]compute outPortCommand start: " << endl;
        structPort* outPort = m_allWidget->listStructPort.at(portList.at(1));
        QString outPortCommand = getConnectItemsByPort(outPort);
        transCommand.append("outPort:" + outPortCommand);
        //
        transCommand.append("cmd:unit_end");
        qDebug()<<"[DEBUG]compute cmd: " << endl;
        foreach(QString cmd, transCommand){
            qDebug()<< cmd << endl;
        }
    }

}

void MainWindow::on_actionDelet_triggered()
{
    //tmpWidgetDelet这是临时传递右键选中的元件底widget对象名
    QList<int> portListTmp=m_allWidget->findPortListByWidget(tmpWidgetDelet);
    for (int j = 0; j < portListTmp.size(); ++j) {
        int indexTmp=portListTmp.at(j);
        if(m_allWidget->atWhichLine(m_allWidget->listStructPort.at(indexTmp)->posPort)>=0)
        {
            qDebug()<<"右键单击了控件,但是此控件有线连着，所以不能删除，本函数结束"<<endl;
            QMessageBox::warning(this,"警告","元件连着导线，不能删除该元件，得先删除连着的导线",QMessageBox::Abort);
            return;// 本函数结束，这样写法还是不错的
        }
    }
    m_allWidget->deletWidget(tmpWidgetDelet);
    update();
}

void MainWindow::on_actionRotateLeft90_triggered()
{
    //tmpWidgetDelet这是临时传递右键选中的元件底widget对象名
    QList<int> portListTmp=m_allWidget->findPortListByWidget(tmpWidgetDelet);
    for (int j = 0; j < portListTmp.size(); ++j) {
        int indexTmp=portListTmp.at(j);
        if(m_allWidget->atWhichLine(m_allWidget->listStructPort.at(indexTmp)->posPort)>=0)
        {
            qDebug()<<"右键单击了控件,但是此控件有线连着，所有不能旋转，本函数结束"<<endl;
            QMessageBox::warning(this,"警告","元件连着导线，不能旋转",QMessageBox::Abort);
            return;// 本函数结束，这样写法还是不错的
        }
    }
    structWidget *tt=m_allWidget->findStructWidget(tmpWidgetDelet);
    if(tt->state==1)//如果这个元件已经是旋转了90度，就不允许再旋转了，因为我没做其它角度的旋转，函数结束
        return;
    structWidget *tmp = new structWidget;
    tmp->currWidgObjName = tt->currWidgObjName;
    tmp->state=tt->state;
    tmp->point=tt->point;

    tmp->name=tt->name;
    tmp->value=tt->value;
    tmp->starValue=tt->starValue;
    tmp->nOrder=tt->nOrder;
    tmp->fz=tt->fz;
    tmp->fre=tt->fre;
    tmp->cxw=tt->cxw;
    tmp->NADD=tt->NADD;
    tmp->NCUT=tt->NCUT;
    tmp->k1=tt->k1;
    tmp->G=tt->G;
    tmp->orderLeft=tt->orderLeft;
    tmp->orderRight=tt->orderRight;
    tmp->hgxs=tt->hgxs;

    //元件复制完成
    m_allWidget->deletWidget(tmpWidgetDelet);
    m_allWidget->copyComponentRotate(tmp);
}

void MainWindow::on_actionAddNode_triggered()
{
    bool ok;
    QString str=QInputDialog::getText(this,"set node name","node name",QLineEdit::Normal,QString::number(m_allWidget->cntAddNode),&ok);
    if(ok)
        m_allWidget->addNode(ui->centralwidget,tmpPosAddNode,str);
}

void MainWindow::on_actionSetParameter_triggered()
{
    MainWindowSetComponentParameter *mmc = new MainWindowSetComponentParameter(m_allWidget->listStructWidget.at(tmpIndexSetParameter),this);
    mmc->show();

}

void MainWindow::on_actionDeletLine_triggered()
{
    if(m_allWidget->isHaveNodeOnThisLine(tmpLineDelet))
    {
        QMessageBox::warning(this,"警告","导线上有节点，不能删除，只能先删除节点",QMessageBox::Abort);
        return;
    }
    m_allWidget->deletLine(tmpLineDelet);
    update();
}

void MainWindow::on_actionDeletNode_triggered()
{
    m_allWidget->deletNode(tmpNodeDelet);
    update();
}

void MainWindow::on_actionEditNode_triggered()
{
    structNode *tmp=m_allWidget->findStructNode(tmpNodeDelet);
    bool ok;
    QString str=QInputDialog::getText(this,"set node name","node name",QLineEdit::Normal,tmp->name,&ok);
    if(ok)
        tmp->name=str;
    else
        return;
    QWidget *wid = findChild<QWidget *>(tmpNodeDelet);
    QList<QLabel *> lab = wid->findChildren<QLabel*>();
    for (int i = 0; i < lab.size(); ++i) {
        if(lab.at(i)->objectName().contains("content"))//说明找到了元件中的content标签，更新他的显示了
            lab.at(i)->setText(tmp->name);
    }

}

void MainWindow::on_pushButtonHindNet_clicked()
{
    if(ui->pushButtonHindNet->text()=="隐藏网格")
    {
        ui->pushButtonHindNet->setText("显示网格");
        flagTurnOnNet=false;
    }
    else
    {
        ui->pushButtonHindNet->setText("隐藏网格");
        flagTurnOnNet=true;
    }
    update();
}

void MainWindow::on_actionOpenWindow_triggered()
{
    MainWindowDraw* scope = new MainWindowDraw(this);
    scope->setWindowFlags(scope->windowFlags()|Qt::Dialog);
    scope->show();
}


void MainWindow::on_pushButton_add_prog_clicked()
{
    //add new btn
    QPushButton * btn = new QPushButton;
    QPushButton * addbtn = ui->pushButton_add_prog;
    QString curId = QString::number(progListButtons.count());
    btn->setParent(this);
    btn->setObjectName(curId);
    btn->setText("主程序_" + curId);
    btn->setFont(QFont("Ubuntu", 9));
    btn->resize(addbtn->size());
    btn->move(addbtn->x(), addbtn->y());
    addbtn->move(addbtn->x(), addbtn->y() + addbtn->height());
    connect(btn, SIGNAL(clicked()), this, SLOT(pushButton_prog_id_clicked()));
    btn->show();
    curProgIdx = progListButtons.count();
    progListButtons.push_back(btn);
    //clear elements
    clearProgEditItems();

    //show EditFrame
    if(mMenuStatus == HMI){
        ui->hmiWidget->hide();
        showProgEditFrame();
    }
    mMenuStatus = PROG;
}


void MainWindow::on_pushButton_2_clicked()
{
    //hide old items
    if(mMenuStatus != HMI){
        clearProgEditItems();
        hideProgEditFrame();
        ui->hmiWidget->show();
    }
    //set status
    mMenuStatus = HMI;
}

void MainWindow::hideProgEditFrame(){
    flagTurnOnNet=false;
    m_allWidget->deletLine("line_power");
    update();
    for (int i = 0; i < progBarWidgets.size(); ++i) {
        QWidget* wgp = progBarWidgets.at(i);
        wgp->hide();
    }
    ui->lineToolBar->hide();
    ui->pushButtonHindNet->hide();
    ui->pushButton_save->hide();
}

void MainWindow::showProgEditFrame(){
    flagTurnOnNet=true;
    m_allWidget->addLineWithName(QString("line_power"),
                                 QPoint(POWER_LINE_X, POWER_LINE_Y_START), QPoint(POWER_LINE_X, POWER_LINE_Y_END));
    update();
    for (int i = 0; i < progBarWidgets.size(); ++i) {
        QWidget* wgp = progBarWidgets.at(i);
        wgp->show();
    }
    ui->lineToolBar->show();
    ui->pushButtonHindNet->show();
    ui->pushButton_save->show();
}

void MainWindow::clearProgEditItems(){
    foreach(structWidget* i, m_allWidget->listStructWidget){
        if(("R_0" == i->currWidgObjName) || ("C_1" == i->currWidgObjName) || ("FC_2" == i->currWidgObjName))
            continue;
        //QWidget *wid = this->findChild<QWidget *>(i->name);
        m_allWidget->deletWidget(i->currWidgObjName);
        //qDebug()<<"ob获取名字: "<<wid->objectName()<<endl;
    }
    foreach(structLine* i, m_allWidget->listStructLine){
        if("line_power" == i->currLineName)
            continue;
        qDebug()<<"line获取名字: "<<i->currLineName<<endl;
        m_allWidget->deletLine(i->currLineName);
        update();
    }
}

void MainWindow::pushButton_prog_id_clicked(){
    QPushButton* btn = qobject_cast<QPushButton* >(sender());
    // qDebug()<<"objectName()=="<<btn->objectName();
    // clear old items
    if(m_allWidget->listStructWidget.length() > FRAME_PROG_ITEMS){
        qDebug() << "INFO::clear at first " << endl;
        clearProgEditItems();
    }
    //read ini start
    mProgSaveFile = new QSettings("progSave.ini", QSettings::IniFormat);
    mProgSaveFile->beginGroup("/" + btn->objectName());
    QStringList data = mProgSaveFile->allKeys();
    if(data.length() <= 0){
        qDebug() << "WARNING::No found group: " << "/" + btn->objectName() << endl;
        mProgSaveFile->endGroup();
        delete mProgSaveFile;
        mProgSaveFile = nullptr;
        return;
    }
    //recall items
    foreach(QString key, data){
        QString objPositon = mProgSaveFile->value(key).toString();
        QStringList  typeXY = objPositon.split("-");
        qDebug()<<"object =" << key << ":" << objPositon;

        if(key.split("_")[0] == "line"){
            qDebug()<<"object start position =" << typeXY[0] <<":" << typeXY[1] << endl;
            qDebug()<<"object end position =" << typeXY[2] <<":" << typeXY[3] << endl;
            m_allWidget->addLineWithName(key, QPoint(typeXY[0].toInt(),typeXY[1].toInt()), QPoint(typeXY[2].toInt(),typeXY[3].toInt()));
        }else{
            qDebug()<<"object position =" << typeXY[1] <<":" << typeXY[2] << endl;
            qDebug()<<"object type =" << typeXY[0] << endl;
            m_allWidget->addWidgetWithName(key, typeXY[0], ui->centralwidget, QPoint(typeXY[1].toInt(),typeXY[2].toInt()));
        }
    }
    update();
    mProgSaveFile->endGroup();
    delete mProgSaveFile;
    mProgSaveFile = nullptr;
    //read ini end;
}


void MainWindow::on_pushButton_save_clicked()
{
    if(curProgIdx == INVALID_PROG_ID){
        qDebug()<<"No valid prog id" << endl;
        return;
    }
    //write ini start
    mProgSaveFile = new QSettings("progSave.ini", QSettings::IniFormat);

    //check if exist
    mProgSaveFile->beginGroup("/" + QString::number(curProgIdx));
    QStringList data = mProgSaveFile->allKeys();
    //clear first
    if(data.length() > 0){
        qDebug() << "remove group: " << "/" + QString::number(curProgIdx) << endl;
        mProgSaveFile->remove("/" + QString::number(curProgIdx));
    }
    //save start
    foreach(structWidget* i, m_allWidget->listStructWidget){
        if(("R_0" == i->name) || ("C_1" == i->name) || ("FC_2" == i->name))
            continue;
        QString point_xy = QString::number(i->point.x()) + "-" + QString::number(i->point.y());
        QString objName = i->currWidgObjName;
        mProgSaveFile->setValue(i->name, objName + "-" + point_xy);
        //qDebug()<<"widget获取名字: "<< i->name <<endl;
    }
    foreach(structLine* i, m_allWidget->listStructLine){
        if("line_0" == i->currLineName)
            continue;
        QString point_xy = QString::number(i->pointStart.x()) + "-" + QString::number(i->pointStart.y()) \
                   + "-" + QString::number(i->pointEnd.x())   + "-" + QString::number(i->pointEnd.y());
        mProgSaveFile->setValue(i->currLineName, point_xy);
        //qDebug()<<"line获取名字: "<<i->currLineName<<endl;
    }
    qDebug()<<"curProgIdx = "<< curProgIdx << " save success." << endl;
    mProgSaveFile->endGroup();
    delete mProgSaveFile;
    mProgSaveFile = nullptr;
    //write ini end
}

