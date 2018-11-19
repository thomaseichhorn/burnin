
#include <stdio.h>
#include <sstream>
#include <locale>

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>

#include <QDebug>
#include <QThread>

#include "controlkeithleypower.h"
#include "additional/additionalthread.h"
#include "general/systemcontrollerclass.h"

using namespace std;

ControlKeithleyPower::ControlKeithleyPower(string pConnection , string pSetVolt , string pSetCurr)
{
    fConnection = pConnection;
    fVoltSet = QString::fromStdString(pSetVolt).toDouble();
    fCurrCompliance = QString::fromStdString(pSetCurr).toDouble();
//    set the number of steps
    fStep = 10;
}

void ControlKeithleyPower::initialize(){

    const ioport_t ioPort = fConnection.c_str();
    comHandler_ = new FP50ComHandler( ioPort );

	std::cout << "Created FP50ComHandler on port " << ioPort << " at " << comHandler_ << std::endl;
}


void ControlKeithleyPower::onPower(int)
{
    sweepVolt(fVoltSet);
    QThread::sleep(1);
}

void ControlKeithleyPower::offPower(int)
{
    sweepVolt(0);
}

void ControlKeithleyPower::setVolt(double pVoltage , int)
{
    fVoltSet = pVoltage;
}

void ControlKeithleyPower::sweepVolt(double pVoltage)
{
    checkVAC();

    char stringinput[512];
    double step = fStep;
    double sign = 1;
    double current_volt = fVolt;
    if ( current_volt != pVoltage){
        if(pVoltage < current_volt ){
            step = step*(-1);
            sign = -1;
        }
        current_volt = current_volt + step;
        while((sign*(pVoltage - current_volt))>=0){

            sprintf(stringinput ,":SOUR:VOLT:LEV %G\r" , current_volt);
            comHandler_->SendCommand(stringinput);
            current_volt = current_volt + step;
            QThread::sleep(1.0);
            checkVAC();
        }
    }
}

void ControlKeithleyPower::setCurr(double pCurrent, int)
{
    char stringinput[512];

    sprintf(stringinput ,":SENS:CURR:PROT %lGE-6\r\n" , pCurrent);
    comHandler_->SendCommand(stringinput);
}

PowerControlClass::fVACvalues *ControlKeithleyPower::getVoltAndCurr()
{
    PowerControlClass::fVACvalues *cObject = new PowerControlClass::fVACvalues();

    checkVAC();

    cObject->pVSet1 = fVoltSet;
    cObject->pISet1 = fCurrCompliance;
    cObject->pVApp1 = fVolt;
    cObject->pIApp1 = fCurr;
    cObject->pVSet2 = 0;
    cObject->pISet2 = 0;
    cObject->pVApp2 = 0;
    cObject->pIApp2 = 0;

    return cObject;

}

void ControlKeithleyPower::checkVAC()
{
    char stringinput[512];
    char buffer[512];

    strcpy(stringinput , ":READ?\r\n");
    comHandler_->SendCommand(stringinput);
    usleep(1000);

    comHandler_->ReceiveString(buffer);

    QString *cStr = new QString(reinterpret_cast<const char*>(buffer));

    string str = cStr->toStdString();
    size_t cPos = str.find(',');


    QString fVoltStr = QString::fromStdString(str.substr(0 , cPos));
    str = str.substr(cPos+1, cPos + 13);
    QString fCurrStr = QString::fromStdString(str.substr(0 , 13));

    fVolt = fVoltStr.toDouble();

    fCurr = fCurrStr.toDouble();

}

void ControlKeithleyPower::closeConnection()
{

}
