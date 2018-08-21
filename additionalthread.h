#ifndef ADDITIONALTHREAD_H
#define ADDITIONALTHREAD_H

#include <vector>

#include <QObject>
#include <QString>

#include "controlttipower.h"
#include "systemcontrollerclass.h"
#include "connectioninterfaceclass.h"
#include "mainwindow.h"

class AdditionalThread : public QObject
{
    Q_OBJECT
public:
    AdditionalThread(QString pName , SystemControllerClass *pControl);

signals:
    void sendToThread(QString);

public slots:
    void getVAC();
    void getRaspSensors();

private:
    QString fName;
    SystemControllerClass *fRaspControl;
    SystemControllerClass *fAddControl;
};

#endif // ADDITIONALTHREAD_H
