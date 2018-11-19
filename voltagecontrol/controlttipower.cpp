#include "controlttipower.h"

#include <QThread>
#include <QDebug>

#include <string>
#include <vector>
#include <iostream>


using namespace std;

const int cTimeOut = 1000;
const int BUFLEN = 256;

ControlTTiPower::ControlTTiPower(string pConnection , vector<string> pId , vector<string> pVolt, vector<string> pCurr)
{
    fConnection = pConnection;

    fId1 = QString::fromStdString(pId[0]).toInt() + 1;
    fId2 = QString::fromStdString(pId[1]).toInt() + 1;

    fDevice = 0;
    
    for(size_t i = 0; i != pVolt.size(); i++){
        fVoltSet.push_back(stod(pVolt[i]));
        fCurrSet.push_back(stod(pCurr[i]));
    }
}

void ControlTTiPower::initialize()
{
    int pPort = 9221;

    lxi_init();

    // Current installed lxi version accepts char* instead of const char*
    // Make a non-const copy as a workaround
    char* pConn = new char[fConnection.length() + 1];
    strcpy(pConn, fConnection.c_str());
    fDevice = lxi_connect(pConn, pPort, NULL, cTimeOut, RAW);
    delete[] pConn;
    
    if (fDevice == -1) {
        std::cerr << "Could not open ControlTTiPower on " << fConnection << " and port " << pPort << std::endl;
    }

    for(size_t i = 0; i!= fVoltSet.size(); i++){
        setVolt(fVoltSet[i], i);
        setCurr(fCurrSet[i], i);
    }
}

void ControlTTiPower::setVolt(double pVoltage , int pId)
{
//    viPrintf(fVi , "V%d %f\n" , pId , pVoltage);
    char cCommand[BUFLEN];
    snprintf(cCommand, sizeof(cCommand), "V%d %.f\n", pId, pVoltage);
    lxi_send(fDevice, cCommand, strlen(cCommand), cTimeOut);
}

void ControlTTiPower::setCurr(double pCurrent , int pId)
{
    char cCommand[BUFLEN];
    snprintf(cCommand, sizeof(cCommand), "V%d %f\n", pId, pCurrent);
    lxi_send(fDevice, cCommand, strlen(cCommand), cTimeOut);
}

void ControlTTiPower::onPower(int pId)
{
    char cCommand[BUFLEN];
    if (pId)
        snprintf(cCommand, sizeof(cCommand), "OP%d 1 \n", pId);
    else
        snprintf(cCommand, sizeof(cCommand), "OPALL 1\n");
    lxi_send(fDevice, cCommand, strlen(cCommand), cTimeOut);
}

void ControlTTiPower::offPower(int pId)
{
    char cCommand[BUFLEN];
    if(pId){
        snprintf(cCommand, sizeof(cCommand), "OP%d 0 \n", pId);
        lxi_send(fDevice, cCommand, strlen(cCommand), cTimeOut);
    }
    else{
        snprintf(cCommand, sizeof(cCommand), "OPALL 0\n");
        lxi_send(fDevice, cCommand, strlen(cCommand), cTimeOut);
    }
}

PowerControlClass::fVACvalues* ControlTTiPower::getVoltAndCurr()
{
    fVACvalues *cObject = new fVACvalues;
    char cBuff[256];
    char cCommand[BUFLEN];

    for(int i = 1; i!= 3 ; i++){
        snprintf(cCommand, sizeof(cCommand), "V%d? \n", i);
        lxi_send(fDevice, cCommand, strlen(cCommand), cTimeOut);
        snprintf(cCommand, sizeof(cCommand), "I%d? \n", i);
        lxi_send(fDevice, cCommand, strlen(cCommand), cTimeOut);
        snprintf(cCommand, sizeof(cCommand), "V%dO? \n", i);
        lxi_send(fDevice, cCommand, strlen(cCommand), cTimeOut);
        snprintf(cCommand, sizeof(cCommand), "I%dO? \n", i);
        lxi_send(fDevice, cCommand, strlen(cCommand), cTimeOut);

    }

    int len;
    if ((len = lxi_receive(fDevice, cBuff, sizeof(cBuff), cTimeOut)) == LXI_ERROR) {
        std::cerr << "ControlTTiPower::getVoltAndCurr: Could not receive values." << std::endl;

        delete cObject;
        return nullptr;
    }
    
    cBuff[255] = 0;

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
    lxi_disconnect(fDevice);
}
