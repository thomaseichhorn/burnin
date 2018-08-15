#ifndef ADDITIONALTHREAD_H
#define ADDITIONALTHREAD_H

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
    void getVAC1();
    void getRaspSensors();

private:
    QString fName;
    ControlTTiPower *fThread;
    SystemControllerClass *fObj;
    ConnectionInterfaceClass *fCon;
};

#endif // ADDITIONALTHREAD_H
