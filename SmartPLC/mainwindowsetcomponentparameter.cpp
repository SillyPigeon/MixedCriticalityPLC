#include "mainwindowsetcomponentparameter.h"
#include "ui_mainwindowsetcomponentparameter.h"
#include "mainwindow.h"

extern MainWindow aGlobal;

MainWindowSetComponentParameter::MainWindowSetComponentParameter(structWidget *tmp, MainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindowSetComponentParameter)
{
    tmpMainwindow=parent;
    ui->setupUi(this);
    setWindowModality(Qt::WindowModal);
    if(tmp->currWidgObjName.contains("R"))
    {
        ui->lineEditValue->setText(tmp->value);
        ui->lineEditValue->setEnabled(true);
    }
    else if(tmp->currWidgObjName.contains("C"))
    {
        ui->lineEditValue->setText(tmp->value);
        ui->lineEditValue->setEnabled(true);
        ui->lineEditValueStartValue->setText(tmp->starValue);
        ui->lineEditValueStartValue->setEnabled(true);
    }
    else if(tmp->currWidgObjName.contains("L"))
    {
        ui->lineEditValue->setText(tmp->value);
        ui->lineEditValue->setEnabled(true);
        ui->lineEditValueStartValue->setText(tmp->starValue);
        ui->lineEditValueStartValue->setEnabled(true);
    }
    else if(tmp->currWidgObjName.contains("FL"))
    {
        ui->lineEditValue->setText(tmp->value);
        ui->lineEditValue->setEnabled(true);
        ui->lineEditNameNOrder->setText(tmp->nOrder);
        ui->lineEditNameNOrder->setEnabled(true);
        ui->lineEditValueStartValue->setText(tmp->starValue);
        ui->lineEditValueStartValue->setEnabled(true);
    }
    else if(tmp->currWidgObjName.contains("E"))
    {
        ui->lineEditValueFz->setText(tmp->fz);
        ui->lineEditValueFz->setEnabled(true);
        ui->lineEditValueFre->setText(tmp->fre);
        ui->lineEditValueFre->setEnabled(true);
        ui->lineEditValueCxw->setText(tmp->cxw);
        ui->lineEditValueCxw->setEnabled(true);
    }
    else if(tmp->currWidgObjName.contains("I"))
    {
        ui->lineEditValueFz->setText(tmp->fz);
        ui->lineEditValueFz->setEnabled(true);
        ui->lineEditValueFre->setText(tmp->fre);
        ui->lineEditValueFre->setEnabled(true);
        ui->lineEditValueCxw->setText(tmp->cxw);
        ui->lineEditValueCxw->setEnabled(true);
    }
    else if(tmp->currWidgObjName.contains("VS")||tmp->currWidgObjName.contains("CS"))
    {
        ui->lineEditValue->setText(tmp->value);
        ui->lineEditValue->setEnabled(true);
        ui->lineEditNameNADD->setText(tmp->NADD);
        ui->lineEditNameNADD->setEnabled(true);
        ui->lineEditValueNCUT->setText(tmp->NCUT);
        ui->lineEditValueNCUT->setEnabled(true);
    }
    else if(tmp->currWidgObjName.contains("T"))
    {
        ui->lineEditValueK1->setText(tmp->k1);
        ui->lineEditValueK1->setEnabled(true);
    }
    else if(tmp->currWidgObjName.contains("H"))
    {

    }
    else if(tmp->currWidgObjName.contains("K"))
    {
        ui->lineEditValueOrderLeft->setText(tmp->orderLeft);
        ui->lineEditValueOrderLeft->setEnabled(true);
        ui->lineEditValueOrderRight->setText(tmp->orderRight);
        ui->lineEditValueOrderRight->setEnabled(true);
    }
    ui->lineEditName->setText(tmp->name);
}

MainWindowSetComponentParameter::~MainWindowSetComponentParameter()
{
    delete ui;
}

void MainWindowSetComponentParameter::on_pushButtonOk_clicked()
{
    structWidget *tt=new structWidget;
    tt->name=ui->lineEditName->text();
    tt->value=ui->lineEditValue->text();
    tt->starValue=ui->lineEditValueStartValue->text();
    tt->nOrder=ui->lineEditNameNOrder->text();
    tt->fz=ui->lineEditValueFz->text();
    tt->fre=ui->lineEditValueFz->text();
    tt->cxw=ui->lineEditValueCxw->text();
    tt->NADD=ui->lineEditNameNADD->text();
    tt->NCUT=ui->lineEditValueNCUT->text();
    tt->k1=ui->lineEditValueK1->text();
    tt->G=ui->lineEditNameG->text();
    tt->orderLeft=ui->lineEditValueOrderLeft->text();
    tt->orderRight=ui->lineEditValueOrderRight->text();
    tt->hgxs=ui->lineEditValueHgxs->text();

    tmpMainwindow->setComponentPatameterByOtherMainwindow(tt);
    close();
}
