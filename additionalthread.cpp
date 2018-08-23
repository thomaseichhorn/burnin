#include <QThread>

#include "additionalthread.h"
#include "mainwindow.h"

AdditionalThread::AdditionalThread(QString pName, SystemControllerClass *pControl) : fName(pName)
{
    fAddControl = pControl;
}

void AdditionalThread::getVAC()
{
    while (true) {
        PowerControlClass *cPowerObj;
        cPowerObj = fAddControl->getObject("TTi1");
        emit sendToThread(cPowerObj->getVoltAndCurr());
        QThread::sleep(10);
    }
}

void AdditionalThread::getRaspSensors()
{
    while(true){
        QString cStr = fAddControl->fConnectRasp->getInfoFromSensors();
        emit sendToThreadString(cStr);
        QThread::sleep(10);
    }
}

void AdditionalThread::getVACKeithley()
{
    while(true){

        PowerControlClass *cPowerObj;
        cPowerObj = fAddControl->getObject("Keithley1");
        emit sendToThreadKeithley(cPowerObj->getVoltAndCurr());
        QThread::sleep(5);
    }
}
