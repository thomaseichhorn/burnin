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
    //getObject(fNamesSources[0]) = new ControlTTiPower();
    fTTiVolt = new ControlTTiPower();
    fDaqControl = new DAQControlClass();
    fDatabase = new DatabaseInterfaceClass();
    fConnectRasp = new ConnectionInterfaceClass();



//    fKeithleyControl = new ControlKeithleyPower();
    this->ParseVSources();
}

SystemControllerClass::~SystemControllerClass()
{}

void SystemControllerClass::Initialize()
{
    fDaqControl->InitDAQ();
    fDatabase->InitDatabase();
    fConnectRasp->raspInitialize();
    getObject("TTi1")->InitPwr();
    getObject("Keithley1")->InitPwr();
//    EnvironmentControlClass *cJulabo = new JulaboFP50();
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

void SystemControllerClass::doList()
{
//    fObjParam cObject;
//    for(vector<fObjParam>::iterator cIter= fListOfCommands.begin() ; cIter != fListOfCommands.end() ; ++cIter){
//        string cStr = (*cIter).cName;
//        double cValue = (*cIter).cValue;
//        if(cStr == "Set Temperature (°C)"){

//        }
//        if(cStr == "Wait (Sec)"){

//        }
//        if(cStr == "On power supply"){

//        }
//        if(cStr == "Off power supply"){

//        }
//    }
}

//reads file and makes map with name and object of power supply
void SystemControllerClass::ParseVSources()
{

    PowerControlClass *fPowerKeithley = new ControlKeithleyPower();
    PowerControlClass *fPowerTTi = new ControlTTiPower();

    fMapSources.insert( pair<string , PowerControlClass*>("TTi1" , fPowerTTi));
    fNamesSources.push_back("TTi1");
    fMapSources.insert( pair<string , PowerControlClass*>("Keithley1" , fPowerKeithley));
    fNamesSources.push_back("Keithley1");
}

//gets the value of key pStr
PowerControlClass* SystemControllerClass::getObject(string pStr)
{
    return fMapSources[pStr];
}
vector<string> SystemControllerClass::getSourceNameVec()
{
    return fNamesSources;
}

