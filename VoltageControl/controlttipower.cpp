#include "controlttipower.h"

#include <QThread>
#include <QDebug>

#include <string>
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
    viPrintf(fVi , "OP%d 1 \n" , pId);
}

void ControlTTiPower::offPower(int pId)
{
    viPrintf(fVi , "OP%d 0 \n" , pId);
}

PowerControlClass::fVACvalues* ControlTTiPower::getVoltAndCurr()
{
    fVACvalues *cObject = new fVACvalues();
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

QString ControlTTiPower::deleteSpaces(QString pStr)
{
    string str = pStr.toStdString();
    for (std::string::iterator it = str.begin(); it != str.end(); it++){
        std::string::iterator begin = it;
        while (it != str.end() && ::isspace(*it) )it++;
        if (it - begin > 1)
            it = str.erase(begin + 1, it) - 1;
    }
    return QString::fromStdString(str);
}

QString ControlTTiPower::transformQString(QString pStr)
{
    string cStr;
    cStr = pStr.toStdString();
    size_t cPos1 = cStr.find("A");
    size_t cPos2 = cStr.find("V");
    if(cPos1)
        cStr = cStr.substr(0 , cPos1);

    if(cPos2)
        cStr = cStr.substr(0 , cPos2);

    pStr = QString::fromStdString(cStr);
    return pStr;
}
