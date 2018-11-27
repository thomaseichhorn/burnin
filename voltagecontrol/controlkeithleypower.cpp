
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

ControlKeithleyPower::ControlKeithleyPower(string pConnection, double pSetVolt, double pSetCurr)
{
    fConnection = pConnection;
    fVoltSet = pSetVolt;
    fCurrCompliance = pSetCurr;
//    set the number of steps
    fStep = 10;
}

void ControlKeithleyPower::initialize(){

    const ioport_t ioPort = fConnection.c_str();
    speed_t keithleybaud = B19200;
    comHandler_ = new ComHandler( ioPort, keithleybaud );
    std::cout << "Created ComHandler on port " << ioPort << " at " << comHandler_ << std::endl;
    setKeithleyOutputState ( 0 );
    setCurr(fCurrCompliance);
}


void ControlKeithleyPower::onPower(int)
{
    setKeithleyOutputState ( 1 );
    sweepVolt(fVoltSet);
    QThread::sleep(1);
}

void ControlKeithleyPower::offPower(int)
{
    sweepVolt(0);
    setKeithleyOutputState ( 0 );
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
    double start_volt = fVolt;
    if (start_volt != pVoltage){
        if(pVoltage < start_volt)
            step = -step;
        
        //current_volt = current_volt + step;
        for (int i = 0; i < (pVoltage - start_volt) / step; ++i) {
            sprintf(stringinput ,":SOUR:VOLT:LEV %G\r", start_volt + step * i);
            comHandler_->SendCommand(stringinput);
            QThread::sleep(1.0);
            checkVAC();
        }
        
        // Send final set to take care of differences smaller than step
        sprintf(stringinput ,":SOUR:VOLT:LEV %G\r", pVoltage);
        comHandler_->SendCommand(stringinput);
        QThread::sleep(1.0);
        checkVAC();
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
    memset(cObject, 0, sizeof(PowerControlClass::fVACvalues));
    
    cObject->pVSet1 = fVoltSet;
    cObject->pISet1 = fCurrCompliance;

    if ( getKeithleyOutputState ( ) )
    {
	checkVAC();

	cObject->pVApp1 = fVolt;
	cObject->pIApp1 = fCurr;
    }

    return cObject;

}

void ControlKeithleyPower::checkVAC()
{
    char stringinput[512];
    char buffer[1024];

    strcpy(stringinput , ":READ?\r\n");
    comHandler_->SendCommand(stringinput);
    usleep(1000);

    comHandler_->ReceiveString(buffer);
    
    string str(buffer);
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

void ControlKeithleyPower::setKeithleyOutputState ( int outputsetting )
{
    if ( outputsetting == 0 )
    {
	char stringinput[512];
	char buffer[1024];
	strcpy(stringinput , ":OUTPUT1:STATE OFF\r\n");
	comHandler_->SendCommand(stringinput);
	comHandler_->ReceiveString(buffer);
	std::cout << buffer << std::endl;
	usleep(1000);
	keithleyOutputOn = false;
    }
    else if ( outputsetting == 1 )
    {
	char stringinput[512];
	char buffer[1024];
	strcpy(stringinput , ":*RST\r\n");
	comHandler_->SendCommand(stringinput);
	comHandler_->ReceiveString(buffer);
	std::cout << buffer << std::endl;
	usleep(1000);

	strcpy(stringinput , ":*IDN?\r\n");
	comHandler_->SendCommand(stringinput);
	comHandler_->ReceiveString(buffer);
	std::cout << buffer << std::endl;
	usleep(1000);

	strcpy(stringinput , ":OUTPUT1:STATE ON\r\n");
	comHandler_->SendCommand(stringinput);
	comHandler_->ReceiveString(buffer);
	std::cout << buffer << std::endl;
	usleep(1000);

	strcpy(stringinput , ":SOURCE:VOLTAGE:RANGE 1000\r\n");
	comHandler_->SendCommand(stringinput);
	comHandler_->ReceiveString(buffer);
	std::cout << buffer << std::endl;
	usleep(1000);
	
	strcpy(stringinput , ":SENSE:FUNCTION 'CURRENT:DC'\r\n");
	comHandler_->SendCommand(stringinput);
	comHandler_->ReceiveString(buffer);
	std::cout << buffer << std::endl;
	usleep(1000);

	keithleyOutputOn = true;
    }
}

bool ControlKeithleyPower::getKeithleyOutputState ( )
{
    return ( keithleyOutputOn );
}
