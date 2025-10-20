#-------------------------------------------------
#
# Project created by QtCreator 2019-08-01T19:24:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = highOrderCircuitSimulate
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    wire.cpp \
    allwire.cpp \
    port.cpp \
    allport.cpp \
    component.cpp \
    allwidget.cpp \
    mainwindowsetcomponentparameter.cpp \
    DrawPlot/dialogformatdatainfo.cpp \
    DrawPlot/mainwindowdraw.cpp \
    lib/qcustomplot.cpp

HEADERS  += mainwindow.h \
    wire.h \
    allwire.h \
    port.h \
    allport.h \
    component.h \
    allwidget.h \
    mainwindowsetcomponentparameter.h \
    DrawPlot/dialogformatdatainfo.h \
    DrawPlot/mainwindowdraw.h \
    lib/qcustomplot.h

FORMS    += mainwindow.ui \
    mainwindowsetcomponentparameter.ui \
    DrawPlot/dialogformatdatainfo.ui \
    DrawPlot/mainwindowdraw.ui

RC_ICONS=myico.ico

RESOURCES += \
    component.qrc

DISTFILES += \
    myIco.ico
