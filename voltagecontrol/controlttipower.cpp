#include "controlttipower.h"

#include <QThread>
#include <QDebug>

#include <string>
#include <iostream>


using namespace std;

ControlTTiPower::ControlTTiPower(string pConnection , vector<string> pId , vector<string> pVolt, vector<string> pCurr)
{
    fConnection = pConnection;

    fId1 = QString::fromStdString(pId[0]).toDouble() + 1;
    fId2 = QString::fromStdString(pId[1]).toDouble() + 1;

    fSetVolt1 = QString::fromStdString(pVolt[0]).toDouble();
    fSetVolt2 = QString::fromStdString(pVolt[1]).toDouble();
    fSetCurr1 = QString::fromStdString(pCurr[0]).toDouble();
    fSetCurr2 = QString::fromStdString(pCurr[1]).toDouble();
}

bool ControlTTiPower::InitPwr()
{
    ViUInt32 pTimeout = 5000;
    fStatus = viOpenDefaultRM(&fDefaultRm);

    if(fStatus < VI_SUCCESS){
        printf("Could not open a session to the VISA Resource Manager!\n");
        exit(EXIT_FAILURE);
    }
    else{
        char cStrInput[50];
        //fConnection = fConnection.erase(fConnection.find('E')+2 , fConnection.size());
        for(int i = 0 ; i != fConnection.size() ; i++){
            cStrInput[i] = fConnection[i];
        }
        fStatus = viOpen(fDefaultRm , cStrInput ,VI_NULL , pTimeout , &fVi);
        if(fStatus < VI_SUCCESS){
            cout << "Couldn`t connect to TTI" << endl;
            exit(EXIT_FAILURE);
        }
        else{
            cout << "Connected to TTi!" << endl;
        }
    }
    setVolt(fSetVolt1 , fId1);
    setVolt(fSetVolt2 , fId2);
    setCurr(fSetCurr1 , fId1);
    setCurr(fSetCurr2 , fId2);
}

void ControlTTiPower::setVolt(double pVoltage , int pId)
{
    viPrintf(fVi , "V%d %f\n" , pId , pVoltage);
}

void ControlTTiPower::setCurr(double pCurrent , int pId)
{
    viPrintf(fVi , "I%d %f\n" , pId , pCurrent);
}

void ControlTTiPower::onPower(int pId)
{
    if(pId)
        viPrintf(fVi , "OP%d 1 \n" , pId);
    else viPrintf(fVi, "OPALL 1\n");
}

void ControlTTiPower::offPower(int pId)
{
    if(pId)
        viPrintf(fVi , "OP%d 0 \n" , pId);
    else viPrintf(fVi, "OPALL 0\n");
}

PowerControlClass::fVACvalues* ControlTTiPower::getVoltAndCurr()
{
    fVACvalues *cObject = new fVACvalues;
    char cBuff[256];
    viPrintf(fVi , "V%d? \n" , 1);
    viPrintf(fVi , "I%d? \n" , 1);
    viPrintf(fVi , "V%dO? \n" , 1);
    viPrintf(fVi , "I%dO? \n" , 1);

    viPrintf(fVi , "V%d? \n" , 2);
    viPrintf(fVi , "I%d? \n" , 2);
    viPrintf(fVi , "V%dO? \n" , 2);
    viPrintf(fVi , "I%dO? \n" , 2);

    viScanf(fVi , "%t" , cBuff);

    QString pStr = QString(cBuff);
    string cStr = pStr.toStdString();
    vector<string> cVecTemp;

    for(int i = 0 ; i != 8 ; i++){
        size_t cPos = cStr.find("\r");
        string temp = cStr.substr(0 , cPos);
        cVecTemp.push_back(temp);
        cStr = cStr.substr(cPos +1 , cStr.size());
    }
    for(size_t i = 0 ; i != cVecTemp.size() ; i++){
        size_t pos1 = cVecTemp[i].find("V");
        size_t pos2 = cVecTemp[i].find("I");
        if(pos1 < 5 || pos2 < 5){
            cVecTemp[i].erase(0 , 3);
        }
        else{
            cVecTemp[i].erase(cVecTemp[i].size()-1 , cVecTemp.size());
        }
    }

    cObject->pVSet1 = QString::fromStdString(cVecTemp[0]).toDouble();
    cObject->pISet1 = QString::fromStdString(cVecTemp[1]).toDouble();
    cObject->pVApp1 = QString::fromStdString(cVecTemp[2]).toDouble();
    cObject->pIApp1 = QString::fromStdString(cVecTemp[3]).toDouble();

    cObject->pVSet2 = QString::fromStdString(cVecTemp[4]).toDouble();
    cObject->pISet2 = QString::fromStdString(cVecTemp[5]).toDouble();
    cObject->pVApp2 = QString::fromStdString(cVecTemp[6]).toDouble();
    cObject->pIApp2 = QString::fromStdString(cVecTemp[7]).toDouble();

    return cObject;
}

void ControlTTiPower::closeConnection()
{
    if(fVi)
        viClose(fVi);
    if(fDefaultRm)
        viClose(fDefaultRm);
}
