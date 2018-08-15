#include "controlttipower.h"

#include <QThread>
#include <QDebug>

#include <iostream>


using namespace std;

ControlTTiPower::ControlTTiPower()
{}

bool ControlTTiPower::InitPwr()
{
    ViUInt32 pTimeout = 5000;
    fStatus = viOpenDefaultRM(&fDefaultRm);
    if(fStatus < VI_SUCCESS){
        printf("Could not open a session to the VISA Resource Manager!\n");
        exit(EXIT_FAILURE);
    }
    else{
        fStatus = viOpen(fDefaultRm , "TCPIP::192.168.1.180::9221::SOCKET" , VI_NULL ,  pTimeout , &fVi);
        if(fStatus < VI_SUCCESS) return false;
        else{
            cout << "Connected to TTi!" << endl;
            return true;
        }
    }
}

QString ControlTTiPower::getCurrApp(int pId)
{
    char cBuff[256];
    viPrintf(fVi , "I%dO? \n" , pId);
    viScanf(fVi , "%t" , cBuff);
    QString cStr = QString(cBuff);

    return cStr;
}
QString ControlTTiPower::getCurrSet(int pId)
{
    char cBuff[256];
    viPrintf(fVi , "I%d? \n" , pId);
    viScanf(fVi , "%t" , cBuff);
    QString cStr = QString(cBuff);

    return cStr;
}

QString ControlTTiPower::getVoltApp(int pId)
{
    char cBuff[256];
    viPrintf(fVi , "V%dO? \n" , pId);
    viScanf(fVi , "%t" , cBuff);
    QString cStr = QString(cBuff);

    return cStr;
}

QString ControlTTiPower::getVoltSet(int pId)
{
    char cBuff[256];
    viPrintf(fVi , "V%d? \n" , pId);
    viScanf(fVi , "%t" , cBuff);

    QString cStr = QString(cBuff);
    cStr = cStr.remove(0 ,2);
    return cStr;
}

void ControlTTiPower::setVolt(int pId , double pVoltage)
{
    viPrintf(fVi , "V%d %f\n" , pId , pVoltage);
}

void ControlTTiPower::setCurr(int pId, double pCurrent)
{
    viPrintf(fVi , "I%d %f\n" , pId , pCurrent);
}

void ControlTTiPower::setVoltAndCurr(double pI1 , double pV1 , double pI2 , double pV2)
{
    viPrintf(fVi , "I1 %f\n" ,pI1);
    viPrintf(fVi , "V1 %f\n" , pV1);
    viPrintf(fVi , "I2 %f\n" ,pI2);
    viPrintf(fVi , "V2 %f\n" , pV2);
}

void ControlTTiPower::onPower(int pId)
{
    viPrintf(fVi , "OP%d 1 \n" , pId);
}

void ControlTTiPower::onPowerAll()
{
    viPrintf(fVi , "OPALL 1 \n");
}

void ControlTTiPower::offPower(int pId)
{
    viPrintf(fVi , "OP%d 0 \n" , pId);
}

void ControlTTiPower::offPowerAll()
{
    viPrintf(fVi , "OPALL 0 \n");
}

QString ControlTTiPower::getVoltAndCurr(int pId)
{
    char cBuff[256];
    viPrintf(fVi , "V%d? \n" , pId);
    viPrintf(fVi , "I%d? \n" , pId);
    viPrintf(fVi , "V%dO? \n" , pId);
    viPrintf(fVi , "I%dO? \n" , pId);

    viScanf(fVi , "%t" , cBuff);
    QString cStr = QString(cBuff);

    return cStr;
}
