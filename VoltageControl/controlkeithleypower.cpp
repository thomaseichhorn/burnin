

#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <locale>

#include <QDebug>
#include <QThread>

#include "controlkeithleypower.h"

using namespace std;

ControlKeithleyPower::ControlKeithleyPower(string pConnection , string pSetVolt , string pSetCurr)
{
    fConnection = pConnection;
    fVoltSet = QString::fromStdString(pSetVolt).toDouble();
    fCurrCompliance = QString::fromStdString(pSetCurr).toDouble();
//    set the number of steps
    fStep = 10;
}

bool ControlKeithleyPower::InitPwr(){

    fStatus = viOpenDefaultRM(&fDefaultRm);
    if(fStatus < VI_SUCCESS){
        printf("Could not open a session to the VISA Resource Manager!\n");
        exit(EXIT_FAILURE);
    }
    else{
        char cStrInput[20];
        fConnection = fConnection.erase(fConnection.find('T')+2 , fConnection.size());

        for(int i = 0 ; i!= fConnection.size() ; i++){
            cStrInput[i] = fConnection[i];
        }

        fStatus = viOpen( fDefaultRm, cStrInput, VI_NULL, VI_NULL, &fVi);
        fStatus = viSetAttribute(fVi , VI_ATTR_TMO_VALUE, 5000);
        fStatus = viSetAttribute(fVi, VI_ATTR_ASRL_BAUD, 9600);
        fStatus = viSetAttribute(fVi, VI_ATTR_ASRL_DATA_BITS, 8);
        fStatus = viSetAttribute(fVi , VI_ATTR_TERMCHAR_EN , VI_TRUE);
        fStatus = viSetAttribute(fVi , VI_ATTR_TERMCHAR , 0xD);
        if(fStatus < VI_SUCCESS){
            cout << "Couldn`t connect" << endl;
            return false;
        }
        else{
            cout << "Connected Keithley!" << endl;
            ViUInt32 writeCount;
            char stringinput[512];

            strcpy(stringinput , "*RST\r");
            fStatus = viWrite (fVi, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
            strcpy(stringinput , ":SOUR:FUNC VOLT\r");
            fStatus = viWrite (fVi, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
            strcpy(stringinput , ":SOUR:VOLT:RANG:AUTO ON\r");
            fStatus = viWrite (fVi, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
            strcpy(stringinput , ":SENS:FUNC \"CURR\"\r");
            fStatus = viWrite (fVi, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
            strcpy(stringinput , ":SENS:CURR:RANG:AUTO ON\r");
            fStatus = viWrite (fVi, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);



            //sprintf(stringinput ,":SENS:CURR:PROT 0.0E-6\r");
            //fStatus = viWrite (fVi, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
            strcpy(stringinput , ":SOUR:VOLT:LEV 0\r");
            fStatus = viWrite (fVi, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);

            strcpy(stringinput , ":OUTP ON\r");
            fStatus = viWrite(fVi , (ViBuf)stringinput , (ViUInt32)strlen(stringinput) , &writeCount);
            makeVolt(0);
        }
    }
    setVolt(fVoltSet, 0);
    setCurr(fCurrCompliance, 0);
}


void ControlKeithleyPower::onPower(int pId)
{
    makeVolt(fVolt);
    QThread::sleep(1);

}

void ControlKeithleyPower::offPower(int pId)
{

    makeVolt(0);
    QThread::sleep(0.5);
    ViUInt32 writeCount ;
    char stringinput[512];

    strcpy(stringinput , ":OUTP OFF\r");
    fStatus = viWrite(fVi , (ViBuf)stringinput , (ViUInt32)strlen(stringinput) , &writeCount);
}

void ControlKeithleyPower::setVolt(double pVoltage , int pId)
{
    fVoltSet = pVoltage;
    fVolt = pVoltage;
}

void ControlKeithleyPower::makeVolt(double pVoltage)
{
    checkVAC();
    ViUInt32 writeCount;
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
            fStatus = viWrite(fVi , (ViBuf)stringinput , (ViUInt32)strlen(stringinput) , &writeCount);
            current_volt = current_volt + step;
            QThread::sleep(1.5);
            checkVAC();
        }
        sprintf(stringinput , ":SOUR:VOLT:LEV %G\r" , pVoltage);
        fStatus = viWrite(fVi , (ViBuf)stringinput , (ViUInt32)strlen(stringinput) , &writeCount);
    }
}

void ControlKeithleyPower::setCurr(double pCurrent, int pId)
{
    ViUInt32 writeCount;
    char stringinput[512];

    sprintf(stringinput ,":SENS:CURR:PROT %lGE-6\r" , pCurrent);
    fStatus = viWrite (fVi, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);

}

PowerControlClass::fVACvalues *ControlKeithleyPower::getVoltAndCurr()
{
    ViUInt32 writeCount , retCount;
    char stringinput[512];
    unsigned char buffer[512];
    PowerControlClass::fVACvalues *cObject = new PowerControlClass::fVACvalues();

    strcpy(stringinput , ":READ?\r");
    fStatus = viWrite (fVi, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
    fStatus = viRead(fVi , buffer , 100 , &retCount);
    QString *cStr = new QString(reinterpret_cast<const char*>(buffer));
    string str = cStr->toStdString();
    size_t cPos = str.find(',');
    QString fVoltStr = QString::fromStdString(str.substr(0 , cPos));
    str = str.substr(cPos+1, cPos + 13);
    QString fCurrStr = QString::fromStdString(str.substr(0 , 13));
    fVolt = fVoltStr.toDouble();
    fCurr = fCurrStr.toDouble();

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
    ViUInt32 writeCount , retCount;
    char stringinput[512];
    unsigned char buffer[512];

    strcpy(stringinput , ":READ?\r");
    fStatus = viWrite (fVi, (ViBuf)stringinput, (ViUInt32)strlen(stringinput), &writeCount);
    fStatus = viRead(fVi , buffer , 100 , &retCount);

    QString *cStr = new QString(reinterpret_cast<const char*>(buffer));

    string str = cStr->toStdString();
    size_t cPos = str.find(',');


    QString fVoltStr = QString::fromStdString(str.substr(0 , cPos));
    str = str.substr(cPos+1, cPos + 13);
    QString fCurrStr = QString::fromStdString(str.substr(0 , 13));

    fVolt = fVoltStr.toDouble();
    fCurr = fCurrStr.toDouble();

}
