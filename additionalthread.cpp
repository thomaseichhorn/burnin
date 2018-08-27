#include <QThread>

#include "additionalthread.h"
#include "GUI/mainwindow.h"

AdditionalThread::AdditionalThread(QString pName, SystemControllerClass *pControl) : fName(pName)
{
    fAddControl = pControl;
}

//sends info from TTi to thread
void AdditionalThread::getVAC()
{
    while (true) {
        PowerControlClass *cPowerObj;
        cPowerObj = fAddControl->getObject("TTI1");
        emit sendToThread(cPowerObj->getVoltAndCurr());
        QThread::sleep(10);
    }
}

//sends info from Raspberry Pi sensors
void AdditionalThread::getRaspSensors()
{
    while(true){
        QString cStr = fAddControl->fConnectRasp->getInfoFromSensors();
        emit sendToThreadString(cStr);
        QThread::sleep(10);
    }
}

//sends info from Keithley
void AdditionalThread::getVACKeithley()
{
    while(true){
        PowerControlClass *cPowerObj;
        cPowerObj = fAddControl->getObject("Keithley2410");
        emit sendToThreadKeithley(cPowerObj->getVoltAndCurr());
        QThread::sleep(5);
    }
}
