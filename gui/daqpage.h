#ifndef DAQPAGE_H
#define DAQPAGE_H

#include "general/daqmodule.h"

#include <QObject>
#include <QWidget>

class DAQPage: public QObject
{
Q_OBJECT

public:
    DAQPage(QWidget* daqPageWidget);
    void setDAQModule(DAQModule* module);
    
private:
    QWidget* _daqPageWidget;
    DAQModule* _module;
    
private slots:
    void onLoadfirmwareClicked();
    void onSystemtestClicked();
    void onCalibrateClicked();
    void onDatatestClicked();
    void onHybridtestClicked();
    void onCmtestClicked();
    void onCommissionClicked();
};

#endif // DAQPAGE_H
