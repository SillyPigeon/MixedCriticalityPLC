#ifndef MAINWINDOWSETCOMPONENTPARAMETER_H
#define MAINWINDOWSETCOMPONENTPARAMETER_H

#include <QMainWindow>
#include "wire.h"
#include "mainwindow.h"

namespace Ui {
class MainWindowSetComponentParameter;
}

class MainWindowSetComponentParameter : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowSetComponentParameter(structWidget *tmp, MainWindow *parent = 0);
    ~MainWindowSetComponentParameter();
    MainWindow *tmpMainwindow;
private slots:
    void on_pushButtonOk_clicked();

private:
    Ui::MainWindowSetComponentParameter *ui;
};

#endif // MAINWINDOWSETCOMPONENTPARAMETER_H
