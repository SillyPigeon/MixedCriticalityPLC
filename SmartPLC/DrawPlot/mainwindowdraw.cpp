#include "DrawPlot/mainwindowdraw.h"
#include "ui_mainwindowdraw.h"
#include "dialogformatdatainfo.h"

#include <QFileDialog>
#include <QMessageBox>

#include <QPdfWriter>
#include <QPainter>
#include <cmath>

MainWindowDraw::MainWindowDraw(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowDraw)
{
    ui->setupUi(this);
    setFixedSize(size());
    setAcceptDrops(true);
    
    
    // init customplot
    ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

    ////////////////////////////////////////////////////////////
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    ui->customPlot->setBackground(plotGradient);      // 设置背景颜色

    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    ui->customPlot->axisRect()->setBackground(axisRectGradient);   // 设置QCPAxisRect背景颜色


    //设置坐标轴
    ui->customPlot->xAxis->setBasePen(QPen(Qt::white, 1));  // 轴线的画笔
    ui->customPlot->xAxis->setTickPen(QPen(Qt::white, 1));  // 轴刻度线的画笔
    ui->customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1)); // 轴子刻度线的画笔
    ui->customPlot->xAxis->setTickLabelColor(Qt::white);  // 轴刻度文字颜色
    ui->customPlot->xAxis->setLabel("time");  // 只有设置了标签，轴标签的颜色才会显示
    ui->customPlot->xAxis->setLabelColor(Qt::white);   // 轴标签颜色
    ui->customPlot->xAxis->setTickLengthIn(3);       // 轴线内刻度的长度
    ui->customPlot->xAxis->setTickLengthOut(5);      // 轴线外刻度的长度
    ui->customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);  // 设置轴线结束时的风格为 实角三角形但内部有凹陷的形状， setLowerEnding设置轴线开始时的风格

    ui->customPlot->yAxis->setBasePen(QPen(Qt::white, 1));  // 轴线的画笔
    ui->customPlot->yAxis->setTickPen(QPen(Qt::white, 1));  // 轴刻度线的画笔
    ui->customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1)); // 轴子刻度线的画笔
    ui->customPlot->yAxis->setTickLabelColor(Qt::white);  // 轴刻度文字颜色
    ui->customPlot->yAxis->setLabel("voltage");  // 只有设置了标签，轴标签的颜色才会显示
    ui->customPlot->yAxis->setLabelColor(Qt::white);   // 轴标签颜色
    ui->customPlot->yAxis->setTickLengthIn(3);       // 轴线内刻度的长度
    ui->customPlot->yAxis->setTickLengthOut(5);      // 轴线外刻度的长度
    ui->customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);  // 设置轴线结束时的风格为 实角三角形但内部有凹陷的形状， setLowerEnding设置轴线开始时的风格


    // 每条网格对应一个刻度
    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));     // 网格线(对应刻度)画笔
    ui->customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine)); // 子网格线(对应子刻度)画笔
    ui->customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->customPlot->xAxis->grid()->setSubGridVisible(true);     // 显示子网格线
    ui->customPlot->yAxis->grid()->setSubGridVisible(true);
    ui->customPlot->xAxis->grid()->setZeroLinePen(QPen(Qt::cyan));   // 设置刻度为0时的网格线的画笔
    ui->customPlot->yAxis->grid()->setZeroLinePen(QPen(Qt::cyan));



    for (int i = 0; i < 4; ++i) {
        QCheckBox* cb = this->findChild<QCheckBox *>("checkBox_"+QString::number(i+1));
        cb->hide();
    }

    drawSinCos();
}

MainWindowDraw::~MainWindowDraw()
{
    delete ui;
}

void MainWindowDraw::drawSinCos()
{
    QVector<double>* x = new QVector<double>;
    QVector<double>* y = new QVector<double>;

    for(double xi=-2*M_PI;xi<2*M_PI;xi+=0.1)
    {
        x->push_back(xi);
        y->push_back(sin(xi));
    }


    QVector<double>* x1 = new QVector<double>;
    QVector<double>* y1 = new QVector<double>;

    for(double xi=-2*M_PI;xi<2*M_PI;xi+=0.1)
    {
        x1->push_back(xi);
        y1->push_back(cos(xi));
    }

    QList<QVector<double>*> listxy;
    listxy.append(x);
    listxy.append(y);
    listxy.append(y1);

    QStringList listname;
    listname.append("通道 1");
    listname.append("通道 2");

    plot_my(listxy,listname,"time","voltage");
    updateCheckBoxList(listname);
}



void MainWindowDraw::plot_my(QVector<double> x, QVector<double> y,Qt::GlobalColor color,QString name,QString xaxis,QString yaxis)
{
    // create graph and assign data to it:
    ui->customPlot->addGraph();
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setData(x, y);
    
    QPen pen;
    pen.setWidth(2);//曲线的粗细
    if(color == Qt::transparent)    //没有颜色可用了，只能是搞个黑色
        color = Qt::black;
    if((int)color >= (int)(Qt::red)+13)
        color = Qt::gray;
    pen.setColor(color);
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setPen(pen);
    
    
    ui->customPlot->graph(ui->customPlot->graphCount()-1)->setName(name);
    on_pushButtonAutoRange_clicked();
    ui->customPlot->legend->setVisible(true);//图例是否显示
    
    ui->customPlot->xAxis->setLabel(xaxis);
    ui->customPlot->yAxis->setLabel(yaxis);
    ui->customPlot->replot();
    
}
//*listxyy[0]作为x轴坐标，其他的都是不同曲线的y坐标
void MainWindowDraw::plot_my(QList<QVector<double>*> listxyy,QStringList listname,QString xaxis,QString yaxis)
{
    ui->lineEditAverage->clear();
    numGraph = listxyy.size()-1;

    for (int i = 0; i < listxyy.size()-1; ++i) {
        plot_my(*listxyy.at(0),*listxyy.at(i+1),(Qt::GlobalColor)((int)(Qt::red)+(i%14)),listname.at(i),xaxis,yaxis);//从红色开始，曲线10种颜色循环
        
        double average=0;
        for (int j = 0; j < listxyy.at(i+1)->size(); ++j) {
            average += listxyy.at(i+1)->at(j);
        }
        average /= listxyy.at(i+1)->size();
        
        if(ui->lineEditAverage->text() == "")
            ui->lineEditAverage->setText(QString::number(average));
        else
            ui->lineEditAverage->setText(ui->lineEditAverage->text()+",  "+QString::number(average));
    }
}


void MainWindowDraw::on_action_T_triggered()
{
    QMessageBox::warning(this,"!!!","获取该软件需要升级");
}

void MainWindowDraw::on_action_U_triggered()
{
    QMessageBox::warning(this,"!!!","获取该软件需要升级");
}








void MainWindowDraw::on_pushButtonOkRange_clicked()
{
    ui->customPlot->xAxis->setRange(ui->lineEditx1->text().toFloat(),ui->lineEditx2->text().toFloat());
    ui->customPlot->yAxis->setRange(ui->lineEdity1->text().toFloat(),ui->lineEdity2->text().toFloat());
    ui->customPlot->replot();
}

void MainWindowDraw::on_pushButtonAutoRange_clicked()
{
    ui->customPlot->rescaleAxes();
    double left = ui->customPlot->xAxis->range().lower;
    double right = ui->customPlot->xAxis->range().upper;
    double deta1 = (right - left)*0.05;
    ui->customPlot->xAxis->setRange(left-deta1,right+deta1);
    
    double bottom = ui->customPlot->yAxis->range().lower;
    double up = ui->customPlot->yAxis->range().upper;
    double deta2 = (up - bottom)*0.05;
    ui->customPlot->yAxis->setRange(bottom-deta2,up+deta2);
    
    ui->customPlot->replot();
}




void MainWindowDraw::on_pushButtonPosLegend_clicked()
{
    static int posLegend = 1;
    if(posLegend == 1)
    {
        ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignTop|Qt::AlignLeft);
        posLegend = 2;
    }
    else if(posLegend == 2)
    {
        ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignBottom|Qt::AlignLeft);
        posLegend = 3;
    }
    else if(posLegend == 3)
    {
        ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignBottom|Qt::AlignRight);
        posLegend = 4;
    }
    else if(posLegend == 4)
    {
        ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignTop|Qt::AlignRight);
        posLegend = 1;
    }
    ui->customPlot->replot();
}

//该函数只在重新拷贝坐标数据绘图时候执行
void MainWindowDraw::updateCheckBoxList(QStringList listname)
{
    for (int i = 0; i < 4; ++i) {
        QCheckBox* cb = this->findChild<QCheckBox *>("checkBox_"+QString::number(i+1));
        cb->setChecked(false);
        cb->hide();
    }
    mapLegend.clear();  //重新绘图，当然得清空
    for (int i = 0; i < listname.size(); ++i) {
        QCheckBox* cb = this->findChild<QCheckBox *>("checkBox_"+QString::number(i+1));
        QString str = listname.at(i);
        if(cb != nullptr)
        {
            cb->setText(str);
            cb->setChecked(true);
            cb->show();
        }
        QCPLayoutElement* elem = ui->customPlot->legend->elementAt(i);
        mapLegend.insert(str,elem);
    }
}

void MainWindowDraw::updateLegendListAndGraph()
{
    //隐藏所有图例和曲线
    for (int i = 0; i < ui->customPlot->legend->elementCount(); ++i)
    {
        //takeAt(i)函数不会导致图例总数减少，只是让全局数组mElements[row][col]中那个图例位置指针为0，
        //且那个图例未删除，只是未显示
        if(ui->customPlot->legend->elementAt(i))
        {
            ui->customPlot->legend->elementAt(i)->setVisible(false);
            ui->customPlot->legend->takeAt(i);
        }
    }
    for (int i = 0; i < ui->customPlot->graphCount(); ++i) {
        ui->customPlot->graph(i)->setVisible(false);
    }
    
    //遍历出哪些图例和曲线需要显示
    cntActualGraph = 0;
    for (int i = 0; i < numGraph; ++i) {
        QCheckBox* cb = this->findChild<QCheckBox *>("checkBox_"+QString::number(i+1));
        if(cb->isChecked())
        {
            ui->customPlot->graph(i)->setVisible(true);
            ui->customPlot->legend->addElement(mapLegend[cb->text()]);
            mapLegend[cb->text()]->setVisible(true);
            ++cntActualGraph;
        }
    }
    ui->customPlot->legend->simplify(); //上面的take函数拿掉了某个图例，一定需要最后调用一下这个函数，使得无效图例不占用空间，仅仅包裹
    ui->customPlot->replot();
}

void MainWindowDraw::on_checkBox_1_clicked()
{
    updateLegendListAndGraph();
}
void MainWindowDraw::on_checkBox_2_clicked()
{
    updateLegendListAndGraph();
}
void MainWindowDraw::on_checkBox_3_clicked()
{
    updateLegendListAndGraph();
}
void MainWindowDraw::on_checkBox_4_clicked()
{
    updateLegendListAndGraph();
}





void MainWindowDraw::on_checkBoxShowBorderLegend_clicked()
{
    static QPen pen;
    if(ui->checkBoxShowBorderLegend->isChecked())
    {
        ui->customPlot->legend->setBorderPen(pen);
    }
    else
    {
        pen = ui->customPlot->legend->borderPen();  //这个一定会先执行
        ui->customPlot->legend->setBorderPen(Qt::NoPen);
    }
    ui->customPlot->replot();
}


void MainWindowDraw::on_radioButtonX_clicked()
{
    ui->customPlot->axisRect()->setRangeZoomFactor(0.85,1);
}

void MainWindowDraw::on_radioButtonY_clicked()
{
    ui->customPlot->axisRect()->setRangeZoomFactor(1,0.85);
}

void MainWindowDraw::on_radioButtonXY_clicked()
{
    //    double factorH = ui->customPlot->axisRect()->rangeZoomFactor(Qt::Horizontal);
    //    double factorV = ui->customPlot->axisRect()->rangeZoomFactor(Qt::Vertical);
    //    qDebug()<<factorH<<factorV;
    ui->customPlot->axisRect()->setRangeZoomFactor(0.85,0.85);
    
}


void MainWindowDraw::on_pushButtonScreenshot_clicked()
{
    QPoint tmp = QPoint(ui->customPlot->x(),ui->customPlot->y()+ui->menubar->height());
    QPixmap pix=QWidget::grab(QRect(tmp,ui->customPlot->size()));
    
    QClipboard *clipboard = QApplication::clipboard();//获取系统剪贴板指针
    clipboard->setPixmap(pix);
    
}







// a="abcdefg" b="abc"    res = "defg"     字符串减法
QString MainWindowDraw::cutString(QString a, QString b)
{
    QString res;
    if(a.contains(b))
    {
        int num = b.size();
        int num2 = a.size();
        res = a.right(num2-num);    //这样子做减法实际上是有问题的，内部某个地方含有了b字符串，就会减错了，暂时先这样吧，只要按照规定写，也是没问题的
        return res;
    }
    else
    {
        QMessageBox::warning(this,"!!!","cutString失败");
        return res;
    }
}
