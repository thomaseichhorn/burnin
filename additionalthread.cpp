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
        PowerControlClass::fVACvalues *cObject;
//        PowerControlClass *cObj;
//        cObj = fAddControl->fVecSources[0];
//        PowerControlClass::fVACvalues *cObject1 = fAddControl->fTTiVolt->getVoltAndCurr();
        cObject = fAddControl->fTTiVolt->getVoltAndCurr();
        emit sendToThread(cObject);
        QThread::sleep(5);
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
//    while(true){
//        PowerControlClass::fVACvalues *cObject;
//        cObject = fAddControl->fKeithleyControl->getVoltAndCurr();
//        emit sendToThread(cObject);
//        QThread::sleep(5);
//    }
}
