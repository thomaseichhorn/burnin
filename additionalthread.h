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
    void send(vector<QString>);
    void sendToThread(QString);
    void sendToThread2(QString);

public slots:
    void getVAC1();
    void getVAC2();
    void getRaspSensors();

private:
    QString fName;
    SystemControllerClass *fAddControl;
};

#endif // ADDITIONALTHREAD_H
