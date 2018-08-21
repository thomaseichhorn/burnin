#include <QThread>

#include "additionalthread.h"
#include "mainwindow.h"

AdditionalThread::AdditionalThread(QString pName, SystemControllerClass *pControl) : fName(pName)
{
    fRaspControl = pControl;
    fAddControl = pControl;
}

void AdditionalThread::getVAC()
{
    while (true) {
        QString cStr = fAddControl->fTTiVolt->getVoltAndCurr();
        emit sendToThread(cStr);
        QThread::sleep(5);
    }
}


void AdditionalThread::getRaspSensors()
{
    while(true){
        QString cStr = fRaspControl->fConnectRasp->getInfoFromSensors();
        emit sendToThread(cStr);
        QThread::sleep(10);
    }
}
