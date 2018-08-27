#include <string>
#include <vector>
#include <regex>

#include <QCoreApplication>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QTime>

#include "systemcontrollerclass.h"
#include "hwdescriptionparser.h"

using namespace std;

SystemControllerClass::SystemControllerClass()
{
    fDaqControl = new DAQControlClass();
    fDatabase = new DatabaseInterfaceClass();
}

SystemControllerClass::~SystemControllerClass()
{}

void SystemControllerClass::Initialize()
{
    fDaqControl->InitDAQ();
    fDatabase->InitDatabase();

    for(size_t i = 0 ; i != fNamesSources.size() ; i++){
        getObject(fNamesSources[i])->InitPwr();
        getObject(fNamesSources[i])->offPower(0);
    }
    fConnectRasp->raspInitialize();

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

//
void SystemControllerClass::doListOfCommands()
{
//    fObjParam cObject;
//    for(vector<fObjParam>::iterator cIter= fListOfCommands.begin() ; cIter != fListOfCommands.end() ; ++cIter){
//        string cStr = (*cIter).cName;
//        double cValue = (*cIter).cValue;
//        if(cStr == "Set Temperature (°C)"){

//        }
//        if(cStr == "Wait (Sec)"){

//        }
//        for(size_t i = 0 ; i != fNamesSources.size() ; i++){
//            if(cStr == "On  " + fNamesSources[i] + "  power supply"){
//                getObject(fNamesSources[i])->onPower(0);

//            }
//            if(cStr == "Off  " + fNamesSources[i] + "  power supply"){
//                getObject(fNamesSources[i])->offPower(0);

//            }

//        }
//    }
}

//reads file and makes map with name and object of power supply
void SystemControllerClass::ParseVSources()
{

//    PowerControlClass *fPowerKeithley = new ControlKeithleyPower();
//    PowerControlClass *fPowerTTi = new ControlTTiPower();

//    fMapSources.insert( pair<string , PowerControlClass*>("TTi1" , fPowerTTi));
//    fNamesSources.push_back("TTi1");
//    fMapSources.insert( pair<string , PowerControlClass*>("Keithley1" , fPowerKeithley));
//    fNamesSources.push_back("Keithley1");

    string cConnection , cSetVolt , cSetCurr , cAddress , cPort;
    //vectors for id , voltage and current
    vector<string> cId;
    vector<string> cVolt;
    vector<string> cCurr;
    for(int i = 0 ; i != fHWDescription.size() ; i++){

        if(fHWDescription[i].type == "LowVoltageSource"){

            if(fHWDescription[i].typeOfClass == "TTI"){
                cConnection = fHWDescription[i].interface_settings["connection"];
                cAddress = fHWDescription[i].interface_settings["address"];
                cPort = fHWDescription[i].interface_settings["port"];

                for(size_t j = 0 ; j != fHWDescription[i].operational_settings.size() ; j++){

                    cId.push_back(fHWDescription[i].operational_settings[j]["output_id"]);
                    cVolt.push_back(fHWDescription[i].operational_settings[j]["Voltage"]);
                    cCurr.push_back(fHWDescription[i].operational_settings[j]["CurrentLimit"]);

                }
                string cConnectStr = getTypeOfConnection(cConnection , cAddress , cPort);
                PowerControlClass *fPowerLow = new ControlTTiPower(cConnectStr , cId , cVolt, cCurr);
                fMapSources.insert(pair<string , PowerControlClass*>(fHWDescription[i].name , fPowerLow));
                fNamesSources.push_back(fHWDescription[i].name);
            }
        }
        if(fHWDescription[i].type == "HighVoltageSource"){

            if(fHWDescription[i].typeOfClass == "Keithley"){
                cConnection = fHWDescription[i].interface_settings["connection"];
                cAddress = fHWDescription[i].interface_settings["address"];
                cPort = fHWDescription[i].interface_settings["port"];
                cSetVolt = fHWDescription[i].operational_settings[0]["Voltage"];
                cSetCurr = fHWDescription[i].operational_settings[0]["CurrentLimit"];
                string cConnectStr = getTypeOfConnection(cConnection , cAddress , cPort);
                PowerControlClass *fPowerHigh = new ControlKeithleyPower(cConnectStr, cSetVolt, cSetCurr);
                fMapSources.insert(pair<string , PowerControlClass*>(fHWDescription[i].name , fPowerHigh));
                fNamesSources.push_back(fHWDescription[i].name);
            }

        }

    }

}

void SystemControllerClass::ParseRaspberry()
{
    string cConnection , cAddress , cPort;

    for(int i = 0 ; i != fHWDescription.size() ; i++){

        if(fHWDescription[i].type == "Raspberry"){
            cConnection = fHWDescription[i].interface_settings["connection"];
            cAddress = fHWDescription[i].interface_settings["address"];
            cPort = fHWDescription[i].interface_settings["port"];
            fConnectRasp = new ConnectionInterfaceClass(cAddress , cPort);
        }
    }
}

void SystemControllerClass::ParseChiller()
{
//    string cAddress;
//    for(int i = 0 ; i != fHWDescription.size() ; i++){

//        if(fHWDescription[i].type == "Chiller"){
//            cAddress = fHWDescription[i].interface_settings["address"];

//            EnvironmentControlClass *fChiller = new JulaboFP50(cAddress);

//        }
//    }
}

void SystemControllerClass::ReadXmlFile()
{

    HWDescriptionParser cParser;
    fHWDescription = cParser.ParseXML();
    ParseVSources();
    ParseRaspberry();

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

//returns a string for connection to power supply
string SystemControllerClass::getTypeOfConnection(string pConnection, string pAddress , string pPort)
{
    string cStr;

    if(pConnection == "ethernet")
       cStr = "TCPIP::" + pAddress + "::" + pPort + "::SOCKET";

    if(pConnection == "rs232"){
        size_t cPos = pAddress.find('S');
        pAddress = pAddress[cPos+1];
        cStr = "ASRL" + pAddress + "::INSTR";
        }
    return cStr;
}

