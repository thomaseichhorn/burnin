#include <string>
#include <vector>

#include <QCoreApplication>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QTime>

#include "systemcontrollerclass.h"

using namespace std;

SystemControllerClass::SystemControllerClass()
{
    fTTiVolt = new ControlTTiPower();
    fDaqControl = new DAQControlClass();
    fEnv = new EnvironmentControlClass();
    fDatabase = new DatabaseInterfaceClass();
    fConnectRasp = new ConnectionInterfaceClass();
//    fKeithleyControl = new ControlKeithleyPower();
    this->ParseVSources();
}

SystemControllerClass::~SystemControllerClass()
{}

void SystemControllerClass::Initialize()
{
    fTTiVolt->InitPwr();
    fDaqControl->InitDAQ();
    fEnv->InitEnv();
    fDatabase->InitDatabase();
    fConnectRasp->raspInitialize();
//    fKeithleyControl->InitPwr();
}

vector<QString>* SystemControllerClass::readFile()
{
    vector<QString> *cVec = new vector<QString>();
    QString cFilter = "*.txt";
    QString cFileName = QFileDialog::getOpenFileName( nullptr , "Open a file" , "/home/" , cFilter);
    QFile cFile(cFileName);
    cFile.open(QFile::ReadOnly);
    QTextStream cStream(&cFile);
    while(!cStream.atEnd()){
        QString cStr = cStream.readLine();
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

vector<SystemControllerClass::fObjParam>* SystemControllerClass::doList(vector<string>* pVec)
{
    vector<fObjParam> *cList;
    size_t cTemp , cVolt , cWait;
    string cStr;
    fObjParam cObj;
    string cPosStr;
    size_t cPos;
    for(vector<string>::iterator cIter = pVec->begin(); cIter != pVec->end(); cIter++){
        cStr = (*cIter);
        cout << cStr << endl;
        cTemp = cStr.find('Temperature');
        cVolt = cStr.find('Voltage');
        cWait = cStr.find('Wait');

        //Temp
        if(!cTemp){
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
        if(!cVolt){
            cPos = cStr.find("=");
            cPosStr = cStr.substr(cPos+1);
            double cVolt = stod(cPosStr.c_str(), 0);
            cout << "voltage" << cVolt << endl;
            cObj.cName = "Voltage";
            cObj.cValue = cVolt;
            cList->push_back(cObj);
        }
        if(!cWait){
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

void SystemControllerClass::ParseVSources()
{


    ControlTTiPower fTTiControl;
    PowerControlClass *fPowerTTi = &fTTiControl;
    ControlKeithleyPower fKeithleyControl;
    PowerControlClass *fPowerKeithley = &fKeithleyControl;

    fMapSources.insert( pair<string , PowerControlClass*>("TTi 1" , fPowerTTi));
    fMapSources.insert( pair<string , PowerControlClass*>("Keithley 1" , fPowerKeithley));
    for(auto it = fMapSources.begin() ; it != fMapSources.end() ; it++){
        cout << it->first << it->second << endl;
    }
}
