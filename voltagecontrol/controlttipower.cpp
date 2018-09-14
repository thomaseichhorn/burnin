#include "controlttipower.h"

#include <QThread>
#include <QDebug>

#include <string>
#include <vector>
#include <iostream>


using namespace std;

ControlTTiPower::ControlTTiPower(string pConnection , vector<string> pId , vector<string> pVolt, vector<string> pCurr)
{
    fConnection = pConnection;

    fId1 = QString::fromStdString(pId[0]).toDouble() + 1;
    fId2 = QString::fromStdString(pId[1]).toDouble() + 1;

    
    for(size_t i = 0; i != pVolt.size(); i++){
        fVoltSet.push_back(stod(pVolt[i]));
        fCurrSet.push_back(stod(pCurr[i]));
    }

    lxi_init();

}

bool ControlTTiPower::initialize()
{
    const char pConn[50] = "192.168.1.180";
    int pPort = 9221;
    int device = lxi_connect(pConn, pPort, "TTi", 1000, VXI11);
    for(size_t i = 0; i!= fVoltSet.size(); i++){
        setVolt(fVoltSet[i],i);
        setCurr(fCurrSet[i], i);
    }

}

void ControlTTiPower::setVolt(double pVoltage , int pId)
{
//    viPrintf(fVi , "V%d %f\n" , pId , pVoltage);
}

void ControlTTiPower::setCurr(double pCurrent , int pId)
{
//    viPrintf(fVi , "I%d %f\n" , pId , pCurrent);
}

void ControlTTiPower::onPower(int pId)
{
//    if(pId)
//        viPrintf(fVi , "OP%d 1 \n" , pId);
//    else viPrintf(fVi, "OPALL 1\n");
}

void ControlTTiPower::offPower(int pId)
{
//    if(pId)
//        viPrintf(fVi , "OP%d 0 \n" , pId);
//    else viPrintf(fVi, "OPALL 0\n");
}

PowerControlClass::fVACvalues* ControlTTiPower::getVoltAndCurr()
{
//    fVACvalues *cObject = new fVACvalues;
//    char cBuff[256];
//    viPrintf(fVi , "V%d? \n" , 1);
//    viPrintf(fVi , "I%d? \n" , 1);
//    viPrintf(fVi , "V%dO? \n" , 1);
//    viPrintf(fVi , "I%dO? \n" , 1);

//    viPrintf(fVi , "V%d? \n" , 2);
//    viPrintf(fVi , "I%d? \n" , 2);
//    viPrintf(fVi , "V%dO? \n" , 2);
//    viPrintf(fVi , "I%dO? \n" , 2);

//    viScanf(fVi , "%t" , cBuff);

//    QString pStr = QString(cBuff);
//    string cStr = pStr.toStdString();
//    vector<string> cVecTemp;

//    for(int i = 0 ; i != 8 ; i++){
//        size_t cPos = cStr.find("\r");
//        string temp = cStr.substr(0 , cPos);
//        cVecTemp.push_back(temp);
//        cStr = cStr.substr(cPos +1 , cStr.size());
//    }
//    for(size_t i = 0 ; i != cVecTemp.size() ; i++){
//        size_t pos1 = cVecTemp[i].find("V");
//        size_t pos2 = cVecTemp[i].find("I");
//        if(pos1 < 5 || pos2 < 5){
//            cVecTemp[i].erase(0 , 3);
//        }
//        else{
//            cVecTemp[i].erase(cVecTemp[i].size()-1 , cVecTemp.size());
//        }
//    }

//    cObject->pVSet1 = QString::fromStdString(cVecTemp[0]).toDouble();
//    cObject->pISet1 = QString::fromStdString(cVecTemp[1]).toDouble();
//    cObject->pVApp1 = QString::fromStdString(cVecTemp[2]).toDouble();
//    cObject->pIApp1 = QString::fromStdString(cVecTemp[3]).toDouble();

//    cObject->pVSet2 = QString::fromStdString(cVecTemp[4]).toDouble();
//    cObject->pISet2 = QString::fromStdString(cVecTemp[5]).toDouble();
//    cObject->pVApp2 = QString::fromStdString(cVecTemp[6]).toDouble();
//    cObject->pIApp2 = QString::fromStdString(cVecTemp[7]).toDouble();

//    return cObject;
}

void ControlTTiPower::closeConnection()
{

}
