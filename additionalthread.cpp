#include "additionalthread.h"
#include "mainwindow.h"

#include <iostream>

#include <QThread>
#include <QDebug>

AdditionalThread::AdditionalThread(QString pName, SystemControllerClass *pControl) : fName(pName) , fAddControl(pControl)
{
}

void AdditionalThread::getVAC1()
{
    while (true) {
        vector<QString> sVec;
        QString cStr1 = fAddControl->fTTiVolt->getVoltSet(1);
        sVec.push_back(cStr1);
        QString cStr2 = fAddControl->fTTiVolt->getVoltApp(1);
        sVec.push_back(cStr2);
        QString cStr3 = fAddControl->fTTiVolt->getCurrApp(1);
        sVec.push_back(cStr3);
        QString cStr4 = fAddControl->fTTiVolt->getCurrSet(1);
        sVec.push_back(cStr4);
        emit send(sVec);
        QThread::sleep(5);
    }
}

void AdditionalThread::getVAC2()
{
    while (true) {
        //QString cStr = fAddControl->fTTiVolt->getVoltAndCurr(2);
        //emit sendToThread2(cStr);
        QThread::sleep(10);
    }
}

void AdditionalThread::getRaspSensors()
{
    while(true){
        QString cStr = fAddControl->fConnectRasp->getInfoFromSensors();
        emit sendToThread(cStr);
        QThread::sleep(10);
    }
}
