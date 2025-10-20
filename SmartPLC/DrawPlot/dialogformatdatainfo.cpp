#include "dialogformatdatainfo.h"
#include "ui_dialogformatdatainfo.h"

DialogFormatDataInfo::DialogFormatDataInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFormatDataInfo)
{
    ui->setupUi(this);
    setFixedSize(size());
    ui->textBrowser->setStyleSheet("background:transparent;border-width:0;border-style:outse");
}

DialogFormatDataInfo::~DialogFormatDataInfo()
{
    delete ui;
}

void DialogFormatDataInfo::on_pushButton_clicked()
{
    close();
}
