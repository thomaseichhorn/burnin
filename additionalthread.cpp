#include "additionalthread.h"
#include "mainwindow.h"

#include <iostream>

#include <QThread>
#include <QDebug>

AdditionalThread::AdditionalThread(QString pName, SystemControllerClass *pControl) : fName(pName) , fObj(pControl)
{
}

void AdditionalThread::getVAC1()
{
    fThread = new ControlTTiPower();
    while (true) {
        QString cStr = fObj->fTTiVolt->getVoltAndCurr(1);
        emit sendToThread(cStr);
        QThread::sleep(5);
    }
}
void AdditionalThread::getRaspSensors()
{
    fCon = new ConnectionInterfaceClass();
    QString cStr;
    while(1){
        cStr = fCon->getInfoFromSensors();
        emit sendToThread(cStr);
        QThread::sleep(8);
    }
}
