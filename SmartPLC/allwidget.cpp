#include "allwidget.h"
#include "math.h"
#include <QLineEdit>
#include <QLabel>
#include <QApplication>
#include <QWidget>
#include <mainwindow.h>
#include <QDebug>
#include <QMessageBox>
#include <QFile>
#include <QDir>

AllWidget::AllWidget(MainWindow *mwindow)
{
    m_mainwindow = mwindow;
    m_window = m_mainwindow->getUiCentralWidget();
}

QWidget *AllWidget::copyComponentRotate(structWidget * childTmp)// child是元件底下那个widget，用来做复制
{
    QString objName=childTmp->currWidgObjName;

    QWidget *widgt;
    int state=childTmp->state;
    if(state==0)// 说明这个元件是正常摆放，现在需要给他旋转90度
    {

        QString type=getTypeOfObjectName(objName);
        widgt=makeComponent90(type,childTmp->point,getNumFromQString(objName),childTmp);

    }
    else if(state==1)
    {

    }
    else if(state==2)
    {

    }
    else if(state==3)
    {

    }
    return widgt;
}
QWidget *AllWidget::makeComponent0(QString type,QPoint pos,int n)// 是用于复制出指定命名的元件
{
    QWidget *widgetTmp;

    widgetTmp = new QWidget(m_window);  // 主窗口的中间框widget作为背景，跟add那儿是一样的
    widgetTmp->setGeometry(QRect(pos.x(), pos.y(), 100, 70));
    widgetTmp->setObjectName(type+"_"+QString::number(n));//左侧元件是C0
    widgetTmp->setStyleSheet("background-color: rgb(173, 173, 173);");
    widgetTmp->setMouseTracking(true);

    QLabel *labelLogo = new QLabel(widgetTmp);
    labelLogo->setObjectName(type+"_"+QString::number(n)+"_logo");
    labelLogo->setGeometry(QRect(20, 40, 60, 25));
    labelLogo->setText("电阻");
    labelLogo->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));

    QLabel *labelName = new QLabel(widgetTmp);
    labelName->setObjectName(type+"_"+QString::number(n)+"_name");
    labelName->setGeometry(QRect(30, 0, 35, 35));
    labelName->setText(type+"_"+QString::number(n));
    labelName->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));

    QLabel *labelPort0 = new QLabel(widgetTmp);
    labelPort0->setObjectName(type+"_"+QString::number(n)+"_port0");
    labelPort0->setGeometry(QRect(0, 30, 15, 15));
    labelPort0->setText("-");
    labelPort0->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));

    QLabel *labelPort1 = new QLabel(widgetTmp);
    labelPort1->setObjectName(type+"_"+QString::number(n)+"_port1");
    labelPort1->setGeometry(QRect(85, 30, 15, 15));
    labelPort1->setText("+");
    labelPort1->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));
    widgetTmp->show();
    widgetTmp->setAttribute(Qt::WA_DeleteOnClose);// 这两句代码只能最后加，不然拖动过去不显示port和name标签

}
QWidget *AllWidget::makeComponent90(QString type, QPoint pos, int n,structWidget * childTmp)// 是用于复制出指定命名的元件
{
    QWidget *widgetTmp;
    if(type == "R"||type == "C"||type == "FC"||type == "L"||type == "FL"||type == "E"||type == "I"||type=="CS"||type=="VS") // 代表是两端口的元件，电阻，电容
    {
        widgetTmp = new QWidget(m_window);  // 主窗口的中间框widget作为背景，跟add那儿是一样的
        widgetTmp->setGeometry(QRect(pos.x(), pos.y(), 80, 120));
        widgetTmp->setObjectName(type+"_"+QString::number(n));//左侧元件是C0
//        widgetTmp->setStyleSheet("background-color: rgb(173, 173, 173);");
        widgetTmp->setStyleSheet("background:transparent");
        widgetTmp->setMouseTracking(true);

        QLabel *labelLogo = new QLabel(widgetTmp);
        labelLogo->setObjectName(type+"_"+QString::number(n)+"_logo");
        labelLogo->setGeometry(QRect(40, 0, 40, 120));

        QPixmap pixmapR(":/new/prefix1/"+type+"90.png");
        labelLogo->setPixmap(pixmapR);

        //    labelLogo->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));

        QLabel *labelName = new QLabel(widgetTmp);
        labelName->setObjectName(type+"_"+QString::number(n)+"_name");
        labelName->setGeometry(QRect(0, 40, 40, 40));
        labelName->setText(childTmp->name);//标签显示的内容
        //    labelName->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));

        QLabel *labelPort0 = new QLabel(widgetTmp);
        labelPort0->setObjectName(type+"_"+QString::number(n)+"_port0");
        labelPort0->setGeometry(QRect(50, 100, 20, 20));
        //    labelPort0->setText("-");
        //    labelPort0->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));
        // 添加端口0对象（结构体）
        structPort *port0=new structPort;
        port0->portName=labelPort0->objectName();
        port0->posPort=widgetTmp->pos()+labelPort0->pos()+QPoint(labelPort0->width()/2,labelPort0->height()/2);
        listStructPort.append(port0);

        QLabel *labelPort1 = new QLabel(widgetTmp);
        labelPort1->setObjectName(type+"_"+QString::number(n)+"_port1");
        labelPort1->setGeometry(QRect(50, 0, 20, 20));
        //    labelPort1->setText("+");
        //    labelPort1->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));
        // 添加端口1对象（结构体）
        structPort *port1=new structPort;
        port1->portName=labelPort1->objectName();
        port1->posPort=widgetTmp->pos()+labelPort1->pos()+QPoint(labelPort1->width()/2,labelPort1->height()/2);
        listStructPort.append(port1);

        widgetTmp->show();
        widgetTmp->setAttribute(Qt::WA_DeleteOnClose);// 这两句代码只能最后加，不然拖动过去不显示port和name标签
    }
    else if(type == "T"||type == "K")//四个端口的元件，比如变压器
    {
        widgetTmp = new QWidget(m_window);
        widgetTmp->setGeometry(QRect(pos.x(), pos.y(), 80, 120));
        widgetTmp->setObjectName(type+"_"+QString::number(cntAddComponent));
//        widgetTmp->setStyleSheet("background-color: rgb(173, 173, 173);");
        widgetTmp->setStyleSheet("background:transparent");
        widgetTmp->setMouseTracking(true);

        QLabel *labelLogo = new QLabel(widgetTmp);
        labelLogo->setObjectName(type+"_"+QString::number(cntAddComponent)+"_logo");
        labelLogo->setGeometry(QRect(40, 0, 40, 120));

        QPixmap pixmapR(":/new/prefix1/"+type+"90.png");
        labelLogo->setPixmap(pixmapR);


        //        labelLogo->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));

        QLabel *labelName = new QLabel(widgetTmp);
        labelName->setObjectName(type+"_"+QString::number(cntAddComponent)+"_name");
        labelName->setGeometry(QRect(0, 40, 40, 40));
        labelName->setText(childTmp->name);
        //        labelName->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));

        QLabel *labelPort0 = new QLabel(widgetTmp);
        labelPort0->setObjectName(type+"_"+QString::number(cntAddComponent)+"_port0");
        labelPort0->setGeometry(QRect(40, 100, 20, 20));
        //        labelPort0->setText("-");
        //        labelPort0->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));
        // 添加端口0对象（结构体）
        structPort *port0=new structPort;
        port0->portName=labelPort0->objectName();
        port0->posPort=widgetTmp->pos()+labelPort0->pos()+QPoint(labelPort0->width()/2,labelPort0->height()/2);
        listStructPort.append(port0);

        QLabel *labelPort1 = new QLabel(widgetTmp);
        labelPort1->setObjectName(type+"_"+QString::number(cntAddComponent)+"_port1");
        labelPort1->setGeometry(QRect(60, 100, 20, 20));
        //        labelPort1->setText("+");
        //        labelPort1->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));
        // 添加端口1对象（结构体）
        structPort *port1=new structPort;
        port1->portName=labelPort1->objectName();
        port1->posPort=widgetTmp->pos()+labelPort1->pos()+QPoint(labelPort1->width()/2,labelPort1->height()/2);
        listStructPort.append(port1);

        QLabel *labelPort2 = new QLabel(widgetTmp);
        labelPort2->setObjectName(type+"_"+QString::number(cntAddComponent)+"_port2");
        labelPort2->setGeometry(QRect(60, 0, 20, 20));
        //        labelPort0->setText("-");
        //        labelPort0->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));
        // 添加端口2对象（结构体）
        structPort *port2=new structPort;
        port2->portName=labelPort2->objectName();
        port2->posPort=widgetTmp->pos()+labelPort2->pos()+QPoint(labelPort2->width()/2,labelPort2->height()/2);
        listStructPort.append(port2);

        QLabel *labelPort3 = new QLabel(widgetTmp);
        labelPort3->setObjectName(type+"_"+QString::number(cntAddComponent)+"_port1");
        labelPort3->setGeometry(QRect(40, 0, 20, 20));
        //        labelPort1->setText("+");
        //        labelPort1->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));
        // 添加端口3对象（结构体）
        structPort *port3=new structPort;
        port3->portName=labelPort3->objectName();
        port3->posPort=widgetTmp->pos()+labelPort3->pos()+QPoint(labelPort3->width()/2,labelPort3->height()/2);
        listStructPort.append(port3);

        widgetTmp->show();
        widgetTmp->setAttribute(Qt::WA_DeleteOnClose);
    }
    else
        return 0;


    structWidget *tmp = new structWidget;
    tmp->currWidgObjName = widgetTmp->objectName();
    tmp->state=1;
    tmp->point = widgetTmp->pos();

    tmp->name=childTmp->name;
    tmp->value=childTmp->value;
    tmp->starValue=childTmp->starValue;
    tmp->nOrder=childTmp->nOrder;
    tmp->fz=childTmp->fz;
    tmp->fre=childTmp->fre;
    tmp->cxw=childTmp->cxw;
    tmp->NADD=childTmp->NADD;
    tmp->NCUT=childTmp->NCUT;
    tmp->k1=childTmp->k1;
    tmp->G=childTmp->G;
    tmp->orderLeft=childTmp->orderLeft;
    tmp->orderRight=childTmp->orderRight;
    tmp->hgxs=childTmp->hgxs;
    //元件复制完成

    listStructWidget.append(tmp);

    return widgetTmp;
}
QWidget *AllWidget::makeComponent180(QString type, QPoint pos, int n)// 是用于复制出指定命名的元件
{

}
QWidget *AllWidget::makeComponent270(QString type, QPoint pos, int n)// 是用于复制出指定命名的元件
{

}
QWidget *AllWidget::addNode(QWidget *widBack, QPoint posN, QString nameNode)
{
    QWidget *widgetTmp;
    widgetTmp = new QWidget(widBack);
    widgetTmp->setGeometry(QRect(posN.x()-7, posN.y()-7, 15, 15));//使得节点中心在鼠标位置
    widgetTmp->setObjectName("node_"+QString::number(cntAddNode));
    widgetTmp->setStyleSheet("background-color: rgb(173, 173, 173);");
    widgetTmp->setMouseTracking(true);

    QLabel *labelLogo = new QLabel(widgetTmp);
    labelLogo->setObjectName("node_"+QString::number(cntAddNode)+"_content");
    //节点显示的节点名称得标签，用户自己设计，不能再用logo名字了，因为元件有logo字眼了，鼠标右键判断时候会冲突
    labelLogo->setGeometry(QRect(0, 0, 15, 15));
    labelLogo->setText(nameNode);
    labelLogo->setStyleSheet(QStringLiteral("background-color: rgb(85, 200, 50);"));

    widgetTmp->show();
    widgetTmp->setAttribute(Qt::WA_DeleteOnClose);// 这两句代码只能最后加，不然拖动过去不显示port和name标签

    // 添加进入节点结构体集合
    structNode *tmp = new structNode;
    tmp->currNodeObjName = widgetTmp->objectName();
    tmp->name = nameNode;
    tmp->posNode = posN;//输入的这个坐标就是鼠标的坐标，是对的，上述-7只是为了视觉效果使得节点中心在鼠标位置
    listStructNode.append(tmp);
    cntAddNode++;//这个是一直递增的，不会因为节点的删除而减小，仍然是放在同名的
    numNode++;
    return widgetTmp;

}

QList<int> AllWidget::findPortListByWidget(QString objNameWidget)
{
    QList<int> tmp;
    for (int i = 0; i < listStructPort.size(); ++i) {
        if(listStructPort.at(i)->portName.contains(objNameWidget))
            tmp.append(i);
    }
    return tmp;
}
//nameComponent是元件名字（可以用户自己设置的），而不是元件的对象名
QString AllWidget::findPortLinkedNodeNameByComponentName(QString port, QString nameComponent)
{
    QString nodeRes="";
    if(port=="port0")//代表是此元件node-的端口
    {
        int index=-1;
        for (int j = 0; j < listStructWidget.size(); ++j) {
            if(listStructWidget.at(j)->name==nameComponent)
                index=j;
        }//找到这个元件在集合中的序号了
        QString objNameWidget=listStructWidget.at(index)->currWidgObjName;
        for (int i = 0; i < listStructPort.size(); ++i) {
            if(listStructPort.at(i)->portName==objNameWidget+"_port0")
                return listStructPort.at(i)->nodeNameLinked;
        }
    }
    else if(port=="port1")
    {
        int index=-1;
        for (int j = 0; j < listStructWidget.size(); ++j) {
            if(listStructWidget.at(j)->name==nameComponent)
                index=j;
        }//找到这个元件在集合中的序号了
        QString objNameWidget=listStructWidget.at(index)->currWidgObjName;
        for (int i = 0; i < listStructPort.size(); ++i) {
            if(listStructPort.at(i)->portName==objNameWidget+"_port1")
            {
                nodeRes=listStructPort.at(i)->nodeNameLinked;
                return nodeRes;

            }
        }
    }
    else if(port=="port2")
    {
        int index=-1;
        for (int j = 0; j < listStructWidget.size(); ++j) {
            if(listStructWidget.at(j)->name==nameComponent)
                index=j;
        }//找到这个元件在集合中的序号了
        QString objNameWidget=listStructWidget.at(index)->currWidgObjName;
        for (int i = 0; i < listStructPort.size(); ++i) {
            if(listStructPort.at(i)->portName==objNameWidget+"_port2")
            {
                nodeRes=listStructPort.at(i)->nodeNameLinked;
                return nodeRes;

            }
        }
    }
    else if(port=="port3")
    {
        int index=-1;
        for (int j = 0; j < listStructWidget.size(); ++j) {
            if(listStructWidget.at(j)->name==nameComponent)
                index=j;
        }//找到这个元件在集合中的序号了
        QString objNameWidget=listStructWidget.at(index)->currWidgObjName;
        for (int i = 0; i < listStructPort.size(); ++i) {
            if(listStructPort.at(i)->portName==objNameWidget+"_port3")
            {
                nodeRes=listStructPort.at(i)->nodeNameLinked;
                return nodeRes;

            }
        }
    }
    return nodeRes;
}

QPoint AllWidget::findPosOfWidgetOrLabel(QPoint posTmp)
{
    QWidget *childLastPortLabel = static_cast<QWidget*>(m_mainwindow->childAt(posTmp));
    QString LastPortLabelName = childLastPortLabel->objectName();
    QLabel *label = m_mainwindow->findChild<QLabel *>(LastPortLabelName);
    QWidget *wid = label->parentWidget();
    QPoint tmp;
    int a=label->pos().x();
    int b=label->pos().y();
    int c=wid->pos().x();
    int d=wid->pos().y();
    tmp=QPoint(a+c,b+d);
    return tmp;
}

QWidget *AllWidget::addLineWithName(QString name, QPoint pointStart, QPoint pointEnd){
    structLine*tmpLine = new structLine;
    tmpLine->currLineName = name;
    tmpLine->pointStart = pointStart;
    tmpLine->pointEnd = pointEnd ;
    cntAddLine++;
    listStructLine.append(tmpLine);  // 记录直线
}

QWidget *AllWidget::addWidgetWithName(QString name, QString objName, QWidget *widBack, QPoint position){
    QWidget *widgetTmp;
    widgetTmp = new QWidget(widBack);
    widgetTmp->setGeometry(QRect(position.x(), position.y(), 120, 80));
    widgetTmp->setObjectName(objName);
    widgetTmp->setStyleSheet("background-color: rgb(173, 173, 173);");
    widgetTmp->setStyleSheet("background:transparent");
    widgetTmp->setMouseTracking(true);

    QLabel *labelLogo = new QLabel(widgetTmp);
    labelLogo->setObjectName(objName + "_logo");
    labelLogo->setGeometry(QRect(0, 40, 120, 40));

    QPixmap pixmapR(":/new/prefix1/" + objName.split("_")[0] +".png");
    labelLogo->setPixmap(pixmapR);

    QLabel *labelName = new QLabel(widgetTmp);
    labelName->setObjectName(objName + "_name");
    labelName->setGeometry(QRect(20, 0, 80, 40));
    labelName->setAlignment(Qt::AlignCenter);
    labelName->setText(name);

    QLabel *labelPort0 = new QLabel(widgetTmp);
    labelPort0->setObjectName(objName + "_port0");
    labelPort0->setGeometry(QRect(0, 50, 20, 20));

    // 添加端口0对象（结构体）
    structPort *port0=new structPort;
    port0->portName=labelPort0->objectName();
    port0->posPort=widgetTmp->pos()+labelPort0->pos()+QPoint(labelPort0->width()/2,labelPort0->height()/2);
    listStructPort.append(port0);

    QLabel *labelPort1 = new QLabel(widgetTmp);
    labelPort1->setObjectName(objName + "_port1");
    labelPort1->setGeometry(QRect(100, 50, 20, 20));

    // 添加端口1对象（结构体）
    structPort *port1=new structPort;
    port1->portName=labelPort1->objectName();
    port1->posPort=widgetTmp->pos()+labelPort1->pos()+QPoint(labelPort1->width()/2,labelPort1->height()/2);
    listStructPort.append(port1);

    widgetTmp->show();
    widgetTmp->setAttribute(Qt::WA_DeleteOnClose);

    numComponent++;
    structWidget *tmp = new structWidget;
    tmp->currWidgObjName = widgetTmp->objectName();
    tmp->point = widgetTmp->pos();
    tmp->name = name;
    listStructWidget.append(tmp);
    return widgetTmp;
}

//元件图片100*30,引脚距离边缘4个像素
QWidget *AllWidget::addWidget(QString type, QWidget *widBack, QPoint position)
{
    QWidget *widgetTmp;
    if(type == "R"||type == "C"||type == "FC"||type == "L"||type == "FL"||type == "E"||type == "I"||type=="CS"||type=="VS"||type=="Scope") // 代表是两端口的元件，电阻，电容
    {
        widgetTmp = new QWidget(widBack);
        widgetTmp->setGeometry(QRect(position.x(), position.y(), 120, 80));
        widgetTmp->setObjectName(type+"_"+QString::number(cntAddComponent));
        widgetTmp->setStyleSheet("background-color: rgb(173, 173, 173);");
        widgetTmp->setStyleSheet("background:transparent");
        widgetTmp->setMouseTracking(true);

        QLabel *labelLogo = new QLabel(widgetTmp);
        labelLogo->setObjectName(type+"_"+QString::number(cntAddComponent)+"_logo");
        labelLogo->setGeometry(QRect(0, 40, 120, 40));

        QPixmap pixmapR(":/new/prefix1/"+type+".png");
        labelLogo->setPixmap(pixmapR);


        //        labelLogo->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));

        QLabel *labelName = new QLabel(widgetTmp);
        labelName->setObjectName(type+"_"+QString::number(cntAddComponent)+"_name");
        labelName->setGeometry(QRect(20, 0, 80, 40));
        labelName->setAlignment(Qt::AlignCenter);
        labelName->setText(type+"_"+QString::number(cntAddComponent));
        //        labelName->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));

        QLabel *labelPort0 = new QLabel(widgetTmp);
        labelPort0->setObjectName(type+"_"+QString::number(cntAddComponent)+"_port0");
        labelPort0->setGeometry(QRect(0, 50, 20, 20));
        //        labelPort0->setText("-");
        //        labelPort0->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));
        // 添加端口0对象（结构体）
        structPort *port0=new structPort;
        port0->portName=labelPort0->objectName();
        port0->posPort=widgetTmp->pos()+labelPort0->pos()+QPoint(labelPort0->width()/2,labelPort0->height()/2);
        listStructPort.append(port0);

        QLabel *labelPort1 = new QLabel(widgetTmp);
        labelPort1->setObjectName(type+"_"+QString::number(cntAddComponent)+"_port1");
        labelPort1->setGeometry(QRect(100, 50, 20, 20));
        //        labelPort1->setText("+");
        //        labelPort1->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));
        // 添加端口1对象（结构体）
        structPort *port1=new structPort;
        port1->portName=labelPort1->objectName();
        port1->posPort=widgetTmp->pos()+labelPort1->pos()+QPoint(labelPort1->width()/2,labelPort1->height()/2);
        listStructPort.append(port1);

        widgetTmp->show();
        widgetTmp->setAttribute(Qt::WA_DeleteOnClose);

    }
    else if(type == "T"||type == "K")//四个端口的元件，比如变压器
    {
        widgetTmp = new QWidget(widBack);
        widgetTmp->setGeometry(QRect(position.x(), position.y(), 120, 80));
        widgetTmp->setObjectName(type+"_"+QString::number(cntAddComponent));
        widgetTmp->setStyleSheet("background-color: rgb(173, 173, 173);");
        widgetTmp->setStyleSheet("background:transparent");
        widgetTmp->setMouseTracking(true);

        QLabel *labelLogo = new QLabel(widgetTmp);
        labelLogo->setObjectName(type+"_"+QString::number(cntAddComponent)+"_logo");
        labelLogo->setGeometry(QRect(0, 40, 120, 40));

        QPixmap pixmapR(":/new/prefix1/"+type+".png");
        labelLogo->setPixmap(pixmapR);


        //        labelLogo->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));

        QLabel *labelName = new QLabel(widgetTmp);
        labelName->setObjectName(type+"_"+QString::number(cntAddComponent)+"_name");
        labelName->setGeometry(QRect(20, 0, 80, 40));
        labelName->setAlignment(Qt::AlignCenter);
        labelName->setText(type+"_"+QString::number(cntAddComponent));
        //        labelName->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));

        QLabel *labelPort0 = new QLabel(widgetTmp);
        labelPort0->setObjectName(type+"_"+QString::number(cntAddComponent)+"_port0");
        labelPort0->setGeometry(QRect(0, 40, 20, 20));
        //        labelPort0->setText("-");
        //        labelPort0->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));
        // 添加端口0对象（结构体）
        structPort *port0=new structPort;
        port0->portName=labelPort0->objectName();
        port0->posPort=widgetTmp->pos()+labelPort0->pos()+QPoint(labelPort0->width()/2,labelPort0->height()/2);
        listStructPort.append(port0);

        QLabel *labelPort1 = new QLabel(widgetTmp);
        labelPort1->setObjectName(type+"_"+QString::number(cntAddComponent)+"_port1");
        labelPort1->setGeometry(QRect(0, 60, 20, 20));
        //        labelPort1->setText("+");
        //        labelPort1->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));
        // 添加端口1对象（结构体）
        structPort *port1=new structPort;
        port1->portName=labelPort1->objectName();
        port1->posPort=widgetTmp->pos()+labelPort1->pos()+QPoint(labelPort1->width()/2,labelPort1->height()/2);
        listStructPort.append(port1);

        QLabel *labelPort2 = new QLabel(widgetTmp);
        labelPort2->setObjectName(type+"_"+QString::number(cntAddComponent)+"_port2");
        labelPort2->setGeometry(QRect(100, 60, 20, 20));
        //        labelPort0->setText("-");
        //        labelPort0->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));
        // 添加端口2对象（结构体）
        structPort *port2=new structPort;
        port2->portName=labelPort2->objectName();
        port2->posPort=widgetTmp->pos()+labelPort2->pos()+QPoint(labelPort2->width()/2,labelPort2->height()/2);
        listStructPort.append(port2);

        QLabel *labelPort3 = new QLabel(widgetTmp);
        labelPort3->setObjectName(type+"_"+QString::number(cntAddComponent)+"_port3");
        labelPort3->setGeometry(QRect(100, 40, 20, 20));
        //        labelPort1->setText("+");
        //        labelPort1->setStyleSheet(QStringLiteral("background-color: rgb(85, 170, 255);"));
        // 添加端口3对象（结构体）
        structPort *port3=new structPort;
        port3->portName=labelPort3->objectName();
        port3->posPort=widgetTmp->pos()+labelPort3->pos()+QPoint(labelPort3->width()/2,labelPort3->height()/2);
        listStructPort.append(port3);

        widgetTmp->show();
        widgetTmp->setAttribute(Qt::WA_DeleteOnClose);

    }
    cntAddComponent++;//这个是一直递增的，不会因为元件的删除而减小
    numComponent++;
    structWidget *tmp = new structWidget;
    tmp->currWidgObjName = widgetTmp->objectName();
    tmp->point = widgetTmp->pos();
    tmp->name=widgetTmp->objectName();
    listStructWidget.append(tmp);
    return widgetTmp;
}

bool AllWidget::isWidgetInnerOrOuter(QPoint posMouse)
{
    QWidget *child = static_cast<QWidget*>(m_mainwindow->childAt(posMouse));
    QString childName = child->objectName();
    for (int j = 0; j < numComponentExistLeft; ++j) {
        if(childName.contains(arrayNameLeftComponent[j]))
        {
            return true;   // 在控件内部
        }

    }
    return false;   // 在控件外部
}

bool AllWidget::isBlankOfWidget(QPoint posMouse)
{
    QWidget *child = static_cast<QWidget*>(m_mainwindow->childAt(posMouse));
    QString childName = child->objectName();
    for (int i = 0; i < listStructWidget.size(); ++i) {
        if(childName == listStructWidget.at(i)->currWidgObjName)
            return true;
    }
    return false;           // 不是
}
bool AllWidget::isPortLabel(QPoint posMouse)
{
    QWidget *child = static_cast<QWidget*>(m_mainwindow->childAt(posMouse));
    QString childName = child->objectName();

    if(childName.contains("port"))  // 只能是点击了端口标签才会有作用
    {
        return true;
    }
    else
    {
        return false;
    }
}
QPoint AllWidget::getOffset(QPoint pointMouse)
{
    QPoint offset;
    if(isWidgetInnerOrOuter(pointMouse))
    {
        if(isBlankOfWidget(pointMouse) == 1)
        {
            QWidget *child = static_cast<QWidget*>(m_mainwindow->childAt(pointMouse));
            QPoint cp = child->pos();
            offset = pointMouse - cp;
            return offset;
        }
        else    // indicate we clicked label or lineEidit
        {
            QWidget *child = static_cast<QWidget*>(m_mainwindow->childAt(pointMouse));
            QWidget *wid = (QWidget *)child->parent();
            offset = pointMouse - wid->pos();
            return offset;
        }
    }
    else    // indicate clicked none widget
    {
        offset = pointMouse;
        return pointMouse;
    }
}

bool AllWidget::isLabelLogoOrName(QPoint pointMouse)
{
    QWidget *child = static_cast<QWidget*>(m_mainwindow->childAt(pointMouse));
    QString childName = child->objectName();
    if(childName.contains("logo")||childName.contains("name"))
    {
        return true;
    }
    return false;
}
bool AllWidget::isLabelLogoOrName(QString childName)
{
    if(childName.contains("logo")||childName.contains("name"))
    {
        return true;
    }
    return false;
}

bool AllWidget::isRoundOfPointP1(QPoint p1, QPoint pGoal, int d)
{
    if((abs(pGoal.x()-p1.x())<=d)&&(abs(pGoal.y()-p1.y())<=d))
        return true;
    else
        return false;
}
bool AllWidget::isComponentExistLeft(QString childName)
{
    for (int i = 0; i < numComponentExistLeft; ++i) {
        if(childName.contains(arrayNameLeftComponent[i]))
        {
            return true;
        }

    }
    return false;
}
int AllWidget::getIndexFromListStructWidget(QString objName)
{
    for (int i = 0; i < listStructWidget.size(); ++i) {
        if(listStructWidget.at(i)->currWidgObjName == objName)
        {
            return i;
        }
    }
    return -1;
}
/***************************************************************************/
/* 前提是鼠标已经点在控件上面任何位置（包括标签）了 */
/***************************************************************************/
bool AllWidget::isWidgetRight(QPoint posMouse)
{
    QWidget *child = static_cast<QWidget*>(m_mainwindow->childAt(posMouse));
    QString childName = child->objectName();
    int id = getIdWidget(childName);
    if(id <numComponentExistLeft)
    {
        return false;   // 是左侧的控件
    }
    else if(id >= numComponentExistLeft)
    {
        return true;   // 右侧的控件
    }
}
void AllWidget::selectdLineByObjName(QString objName)
{
    if(!listSelectedElem.contains(objName))
        listSelectedElem.append(objName);
    for (int i = 0; i < listStructWidget.size(); ++i) {
        if(listStructLine.at(i)->currLineName == objName)   // use lineName as its objName
        {
            listStructLine.at(i)->lineColor=Qt::blue;
            return;
        }
    }

}
/***************************************************************************/
/* 这里不支持多选，每次选择了一条直线，自动清除上一条选中状态直线 */
/***************************************************************************/
void AllWidget::selectLine(int whichLineSlected)
{
    selectdLineByObjName(listStructLine.at(whichLineSlected)->currLineName);
}
structLine *AllWidget::findStructLine(QString objName)
{
    for (int i = 0; i < listStructLine.size(); ++i) {
        if(listStructLine.at(i)->currLineName == objName)
        {
            return listStructLine.at(i);
        }
    }
    return NULL;
}
void AllWidget::deletLine(QString objName)
{
    //    listSelectedElem.removeOne(objName);
    structLine *tmp = findStructLine(objName);
    listStructLine.removeOne(tmp);
}
void AllWidget::selectdWidget(int whichWidgetSlected)
{
    selectdWidgetByObjName(listStructWidget.at(whichWidgetSlected)->currWidgObjName);
}

void AllWidget::selectdWidgetByObjName(QString objName)
{
    if(!listSelectedElem.contains(objName))
        listSelectedElem.append(objName);

    QWidget *wid = m_mainwindow->findChild<QWidget *>(objName);

    wid->setStyleSheet("background-color: rgb(120, 120, 120);");
    wid->show();

}
void AllWidget::deletWidget(QString objName)//objName是R_1这样的元件底widget
{
    QWidget *wid = m_mainwindow->findChild<QWidget *>(objName);
    /********************************************************************************/
    //    listSelectedElem.removeOne(wid->objectName());
    structWidget *tmp = findStructWidget(objName);
    listStructWidget.removeOne(tmp);
    // 还得删除它对应的端口
    structPort *tmpPort0 = findStructPort(objName+"_port0");
    listStructPort.removeOne(tmpPort0);
    structPort *tmpPort1 = findStructPort(objName+"_port1");
    listStructPort.removeOne(tmpPort1);
    delete wid;
    numComponent--;
}

QList<structLinePort*> AllWidget::getLinePortsAtLine(structLine* line){
    QList<structLinePort*> lineIdxList;
    bool isStartOnLine, isEndOnLine;
    int targetIdx = listStructLine.indexOf(line);
    structLine* findLine = NULL;
    structLinePort* linePort = new structLinePort;
    for (int i = 0; i < listStructLine.size(); ++i) {
        if(targetIdx == i){
            continue;
        }
        findLine = listStructLine.at(i);
        isStartOnLine = _isAtLine(line, lineWidth, findLine->pointStart);
        isEndOnLine = _isAtLine(line, lineWidth, findLine->pointEnd);
        if(isStartOnLine == true){
            linePort->lineIdx = i;
            linePort->pointConnect = findLine->pointStart;
            lineIdxList.append(linePort);
        }else if(isEndOnLine == true){
            linePort->lineIdx = i;
            linePort->pointConnect = findLine->pointEnd;
            lineIdxList.append(linePort);
        }
    }
    return lineIdxList;
}


structWidget *AllWidget::findStructWidget(QString objName)
{
    for (int i = 0; i < listStructWidget.size(); ++i) {
        if(listStructWidget.at(i)->currWidgObjName == objName)
        {
            return listStructWidget.at(i);
        }
    }
    return NULL;
}
structPort *AllWidget::findStructPort(QString objName)
{
    for (int i = 0; i < listStructPort.size(); ++i) {
        if(listStructPort.at(i)->portName == objName)
        {
            return listStructPort.at(i);
        }
    }
    return NULL;
}
/***************************************************************************/
/* input_2_lineEdit 提取出中间的2 */
/***************************************************************************/
int AllWidget::getNumFromQString(QString str)
{
    int indexhead=0;
    int indextall=0;
    QString tmp;
    for (int i = 0; i < str.length(); ++i) {
        if(str[i]=='_')
        {
            indexhead = i;
            break;
        }
    }
    for (int i = indexhead+1; i < str.length(); ++i) {
        if(i==str.length()-1)
        {
            indextall= str.length()-1;
            break;
        }
        else if(str[i]=='_')
        {
            indextall = i-1;
            break;
        }
    }

    for (int i = indexhead+1; i <= indextall; ++i) {
        tmp+=str[i];
    }
    return tmp.toInt();
}

QString AllWidget::getName(QString str)//获取lineedit_haha_nihao的lineedit_haha，即最后一个下划线之前的字符
{
    int indexhead=0;
    int indextall=0;
    QString tmp;
    for (int i = 0; i < str.length(); ++i) {
        if(str[i]=='_')
        {
            indextall = i;
            break;
        }
    }
    for (int i = indextall+1; i < str.length(); ++i) {
        if(i==str.length()-1)
        {
            indextall= str.length()-1;
            break;
        }
        else if(str[i]=='_')
        {
            indextall = i-1;
            break;
        }
    }

    for (int i = indexhead; i <= indextall; ++i) {
        tmp+=str[i];
    }
    return tmp;

}

QString AllWidget::getTypeOfObjectName(QString str)
{
    int indextall=0;
    QString tmp;
    if(str.contains("_"))
    {
        for (int i = 0; i < str.length(); ++i) {
            if(str[i]=='_')
            {
                indextall = i;
                break;
            }
        }
        for (int i = 0; i <indextall; ++i) {
            tmp+=str[i];
        }
    }
    return tmp;
}
/**********************************************************************************
* @brief            从字符串中提取出通过空格，逗号，tab键隔开的双精度数
*
* @param_i  str     输入字符串
* @param_o  doubleNum  输出浮点转换结果的数组
* @return           数组长度
* @note:            (1).能提出出整型数，小数
**********************************************************************************/
int AllWidget::strToEveryDoubleNum(QString str, double *doubleNum)
{
    int numNum=0;
    int head=0;
    int tail=0;
    QString tmp;
    for (int i = 0; i < str.length(); ++i) {
        if(str[i]!=' '&&str[i]!=','&&str[i]!=9)
        {
            if(i==0)
                head=0;
            else if(str[i-1]==' '||str[i-1]==','||str[i-1]==9)
            {
                head=i;
            }
        }
        else if(str[i]==' '||str[i]==','||str[i]==9)
        {
            if(i==0)
                ;
            else if(str[i-1]!=' '&&str[i-1]!=','&&str[i-1]!=9)
            {
                tail=i;
                for (int j = head; j < tail; ++j) {
                    tmp[j-head]=str[j];
                }
                doubleNum[numNum++]=tmp.toDouble();
                tmp.clear();
                // 这里记得清空，不然上一次的字符串没有被完全覆盖掉的话，就会对这次转为整形产生影响
                // 比如“234  1”，那么得到第二个数的结果就是134，“34”这两个字符影响了
            }
        }
        if(i==str.length()-1)
        {
            if(str[i]!=' '&&str[i]!=','&&str[i]!=9)
            {
                tail=i;
                for (int j = head; j < tail+1; ++j) {
                    tmp[j-head]=str[j];
                }
                doubleNum[numNum++]=tmp.toDouble();
                tmp.clear();
            }
        }
    }
    return numNum;
}
int AllWidget::getIdWidget(QString childName)
{
    return getNumFromQString(childName);
}

/****************************************************************************
** 函数原型:    int AllLine::atWhichLine(QPoint pGoal)
** 功能描述:    判断目标点pGoal在哪条直线上

** 返回 :      在哪条直线上，-1：不在直线上
****************************************************************************/

int AllWidget::atWhichPort(QPoint pos){
    for (int i = 0; i < listStructPort.size(); ++i) {
        if(listStructPort.at(i)->posPort == pos){
            return i;
        }
    }
    return -1;
}

int AllWidget::atWhichLine(QPoint pos)
{
    for (int i = 0; i < listStructLine.size(); ++i) {
        if(_isAtLine(listStructLine.at(i),lineWidth,pos))
        {
            return i;
        }
    }
    return -1;
}

bool AllWidget::isAtLine(QPoint pos)
{
    for (int i = 0; i < listStructLine.size(); ++i) {
        if(_isAtLine(listStructLine.at(i),lineWidth,pos))//这里容许的误差为线宽个像素
        {
            return true;
        }
    }
    return false;
}
bool AllWidget::_isAtLine(structLine *linePara,int offset,QPoint pGoal)
{
    return __isAtLine(linePara->pointStart,linePara->pointEnd,offset,pGoal);
}
bool AllWidget::__isAtLine(QPoint p1, QPoint p2,int offset, QPoint pGoal)
{
    //qDebug()<<p1<<p2<<pGoal<<endl;
    double x0 = pGoal.x();
    double y0 = pGoal.y();

    double x1 = p1.x();
    double y1 = p1.y();
    double x2 = p2.x();
    double y2 = p2.y();

    double A = y2 - y1;
    double B = x1-x2  ;
    double C = x2*y1 - x1*y2;

    double d = abs((A*x0 + B*y0 +C) / sqrt(A*A +B*B));  //得到点到直线的距离
    //qDebug()<<"d"<<d<<"offset"<<offset<<endl;
    if(d<=offset)
    {
        if(B<=0)
        {
            if(x0>=(x1-2)&&x0<=(x2+2))//按道理说这里不应该减1加1的，但是当是竖直直线的时候，这里就只有刚好鼠标在直线上才可以，
                //这样就很难点，所以这里让x左右放宽了一个像素，从而好点击，但是缺点就是相当于默认直线两端加长了1像素，但是这没有关系，因为我们
                //直线两端有一个像素的误判也没关系的，我们可以接受
                //改成左右2像素了，水平直线没问题，因为是以x作为判断的
                return true;
            else
                return false;
        }
        else
        {
            if(x0<=(x1+2)&&x0>=(x2-2))
                return true;
            else
                return false;
        }
    }
    else
        return false;
}

int AllWidget::atWhichNode(QPoint posTmp)
{
    QWidget *node = static_cast<QWidget*>(m_mainwindow->childAt(posTmp));
    QString nodeName = node->objectName();
    if(nodeName.contains("node"))//只有这个控件包括了node，根据节点的命名规则，这个右键的位置才是节点控件
    {//因为不经过这个判断，鼠标右键在centralwidget而不是节点控件什么的话，他是没有父控件的，用parentWidget()函数会死机
        QLabel *label = m_mainwindow->findChild<QLabel *>(nodeName);//其实这两步是多余的，node就是正确的地址了，还有很多其它地方也是这样的多余
        //这里是直接找父控件了，因为我设计的node控件表明被标签刚好完全覆盖的
        QWidget *child = label->parentWidget();
        for (int i = 0; i < listStructNode.size(); ++i) {
            if(listStructNode.at(i)->currNodeObjName==child->objectName())
            {
                return i;
            }
        }
        return -1;
    }
    else
        return -1;
}

void AllWidget::deletNode(QString objName)
{
    QWidget *wid = m_mainwindow->findChild<QWidget *>(objName);
    /********************************************************************************/
    structNode *tmp = findStructNode(objName);
    listStructNode.removeOne(tmp);
    delete wid;
    numNode--;
}

structNode *AllWidget::findStructNode(QString objName)
{
    for (int i = 0; i < listStructNode.size(); ++i) {
        if(listStructNode.at(i)->currNodeObjName == objName)
        {
            return listStructNode.at(i);
        }
    }
    return NULL;
}

bool AllWidget::isHaveNodeOnThisLine(QString objName)
{
    for (int i = 0; i < listStructNode.size(); ++i) {
        int n=atWhichLine(listStructNode.at(i)->posNode);
        if(n==findLineByName(objName))
            return true;
    }
    return false;
}

int AllWidget::findLineByName(QString objName)
{
    for (int i = 0; i < listStructLine.size(); ++i) {
        if(listStructLine.at(i)->currLineName==objName)
            return i;
    }
    return -1;
}
