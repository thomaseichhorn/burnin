#include "systemcontrollerclass.h"

#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QTime>

#include <string>
#include <vector>

using namespace std;

SystemControllerClass::SystemControllerClass()
{
    fTTiVolt = new ControlTTiPower();
    fDaqControl = new DAQControlClass();
    fEnv = new EnvironmentControlClass();
    fDatabase = new DatabaseInterfaceClass();
    fConnectRasp = new ConnectionInterfaceClass();
//    fDaqControl = nullptr;
//    fEnv = nullptr;
//    fDatabase = nullptr;
//    fTTiVolt = nullptr;
//    fConnect = nullptr;
}

SystemControllerClass::~SystemControllerClass()
{
//    delete fConnectRasp;
//    delete fTTiVolt;
//    delete fDaqControl;
//    delete fEnv;
//    delete fDatabase;
}

void SystemControllerClass::Initialize()
{
    fTTiVolt->InitPwr();
    fDaqControl->InitDAQ();
    fEnv->InitEnv();
    fDatabase->InitDatabase();
    fConnectRasp->raspInitialize();
}

vector<string>* SystemControllerClass::readFile()
{
    vector<string> *cVec = new vector<string>();
    QFile cFile("/home/fedorcht/TestFile.txt");
    cFile.open(QFile::ReadOnly);
    QTextStream cStream(&cFile);
    while(!cStream.atEnd()){
        string cStr = cStream.readLine().toStdString();
        cVec->push_back(cStr);
    }
    cFile.flush();
    cFile.close();
    return cVec;
}

void SystemControllerClass::Wait(int pSec)
{
    QTime cDieTime= QTime::currentTime().addSecs(pSec);
       while (QTime::currentTime() < cDieTime){
           QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
       }
}

vector<SystemControllerClass::fParam>* SystemControllerClass::doList(vector<string>* pVec)
{
    vector<fParam> *cList;
    size_t cTemp , cVolt , cWait;
    string cStr;
    fParam cObj;
    string cPosStr;
    size_t cPos;
    for(vector<string>::iterator cIter = pVec->begin() ; cIter != pVec->end() ; cIter++){
        cStr = (*cIter);
        cout << cStr << endl;
        cTemp = cStr.find('Temperature');
        cVolt = cStr.find('Voltage');
        cWait = cStr.find('Wait');

        //Temp
        if( !cTemp ){
            cPos = cStr.find("=");
            cPosStr = cStr.substr(cPos+1);
            double cTemp = stod(cPosStr.c_str(), 0);
            //fEnv->setTemp(pTemp);
            cout << cTemp << endl;
            cObj.cName = "Temperature";
            cObj.cValue = cTemp;

            cList->push_back(cObj);
        }
        //Voltage
        if( !cVolt ){
            cPos = cStr.find("=");
            cPosStr = cStr.substr(cPos+1);
            double cVolt = stod(cPosStr.c_str(), 0);
            cout << "voltage" << cVolt << endl;
            cObj.cName = "Voltage";
            cObj.cValue = cVolt;
            cList->push_back(cObj);
        }
        if( !cWait ){
            string cPosStr;
            size_t cPos;
            cPos = cStr.find("=");
            cPosStr = cStr.substr(cPos+1);
            double cW = stod(cStr.substr(cPos+1));
            cout << "Wait" << cW << endl;
            cObj.cName = "Wait";
            cObj.cValue = cW;
        }

    }
    return cList;
}

