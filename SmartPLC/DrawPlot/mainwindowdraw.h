#ifndef MAINWINDOWDRAW_H
#define MAINWINDOWDRAW_H

#include <QMainWindow>


#include "lib/qcustomplot.h"

namespace Ui {
class MainWindowDraw;
}


class MainWindowDraw : public QMainWindow
{
    Q_OBJECT

public:
    
    explicit MainWindowDraw(QWidget *parent = 0);
    ~MainWindowDraw();

    void drawSinCos();


    void plot_my(QVector<double> x, QVector<double> y, Qt::GlobalColor color, QString name, QString xaxis, QString yaxis);
    void plot_my(QList<QVector<double>*> listxyy, QStringList listname, QString xaxis, QString yaxis);//ylist[0]作为x轴坐标，其他的都是不同曲线的y坐标
    
    void updateCheckBoxList(QStringList listname);
    void updateLegendListAndGraph();

    QString cutString(QString a, QString b);

    

private slots:
    void on_action_T_triggered();

    void on_action_U_triggered();

        
    
    
    void on_pushButtonOkRange_clicked();
    
    void on_pushButtonAutoRange_clicked();

    
    void on_pushButtonPosLegend_clicked();
    
    void on_checkBox_1_clicked();
    void on_checkBox_2_clicked();
    void on_checkBox_3_clicked();
    void on_checkBox_4_clicked();
            
    
    void on_checkBoxShowBorderLegend_clicked();
        
    void on_radioButtonX_clicked();
    
    void on_radioButtonY_clicked();
    
    void on_radioButtonXY_clicked();
    
    void on_pushButtonScreenshot_clicked();
                

private:
    Ui::MainWindowDraw *ui;
    int cntActualGraph;
    int numGraph;
    
    
    QMap<QString,QCPLayoutElement*> mapLegend;
    
};

#endif // MAINWINDOWDRAW_H
