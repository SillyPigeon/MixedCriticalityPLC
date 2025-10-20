#ifndef DIALOGFORMATDATAINFO_H
#define DIALOGFORMATDATAINFO_H

#include <QDialog>

namespace Ui {
class DialogFormatDataInfo;
}

class DialogFormatDataInfo : public QDialog
{
    Q_OBJECT
    
public:
    explicit DialogFormatDataInfo(QWidget *parent = 0);
    ~DialogFormatDataInfo();
    
private slots:
    void on_pushButton_clicked();
    
private:
    Ui::DialogFormatDataInfo *ui;
};

#endif // DIALOGFORMATDATAINFO_H
