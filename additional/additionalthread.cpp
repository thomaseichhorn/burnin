#include <QThread>

#include "additionalthread.h"
#include "gui/mainwindow.h"
#include "general/environmentcontrolclass.h"
#include "general/genericinstrumentclass.h"
#include "general/julabowrapper.h"

AdditionalThread::AdditionalThread(QString pName, SystemControllerClass *pControl) : fName(pName)
{
    fAddControl = pControl;
}

//sends info from TTi to thread
void AdditionalThread::getVAC()
{
    while (true) {
        PowerControlClass *cPowerObj;
        cPowerObj = dynamic_cast<PowerControlClass*>(fAddControl->getGenericInstrObj("TTI1"));
        if (cPowerObj != nullptr) {
            emit sendToThread(cPowerObj->getVoltAndCurr());
        }
        QThread::sleep(2);
    }
}

//sends info from Raspberry Pi sensors
void AdditionalThread::getRaspSensors()
{
    while(true){
        QString cStr;
        ConnectionInterfaceClass *cConnectRasp;
        cConnectRasp = dynamic_cast<ConnectionInterfaceClass*>(fAddControl->getGenericInstrObj("fhlthermorasp"));
        if(fAddControl->fConnectRasp != nullptr)
            cStr = fAddControl->fConnectRasp->getInfoFromSensors();
//            cStr = cConnectRasp->getInfoFromSensors();
        if (!cStr.isEmpty()) {
            string cTemp = cStr.toStdString();
            size_t cPos = cTemp.find(':');
            cTemp = cTemp.substr(cPos , cTemp.size());
            emit sendToThreadString(QString::fromStdString(cTemp));
        }
        QThread::sleep(10);        
    }
}

//sends info from Keithley
void AdditionalThread::getVACKeithley()
{
    while(true){
        PowerControlClass *cPowerObj;
        //cPowerObj = fAddControl->getObject("Keithley2410");
        cPowerObj = dynamic_cast<PowerControlClass*>(fAddControl->getGenericInstrObj("Keithley2410"));

        emit sendToThreadKeithley(cPowerObj->getVoltAndCurr());
        QThread::sleep(3);
    }
}

void AdditionalThread::onVolt()
{
    PowerControlClass *cPowerObj;
    cPowerObj = fAddControl->getObject("Keithley2410");
    cPowerObj->onPower(0);

}

void AdditionalThread::offVolt()
{
    PowerControlClass *cPowerObj;
    cPowerObj = fAddControl->getObject("Keithley2410");
    cPowerObj->offPower(0);

}

void AdditionalThread::getChillerStatus()
{
    float cBathTemp, cPressure, cSensorTemp, cWorkingTemp;
    string cMeas;
    JulaboWrapper* wrapper = dynamic_cast<JulaboWrapper*>(fAddControl->getGenericInstrObj("JulaboFP50"));
    JulaboFP50* cEnv = wrapper->fJulabo;
    while(true){
        cBathTemp = cEnv->GetBathTemperature();
        cPressure = cEnv->GetPumpPressure();
        cSensorTemp = cEnv->GetSafetySensorTemperature();
        cWorkingTemp = cEnv->GetWorkingTemperature();
        cMeas = std::to_string(cBathTemp) + " " + to_string(cPressure) + " " + to_string(cSensorTemp)
                + " " +to_string(cWorkingTemp);
        emit  sendFromChiller(QString::fromStdString(cMeas));
        QThread::sleep(6);
    }

}

