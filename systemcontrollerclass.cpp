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
    fConnectRasp = NULL;
}

SystemControllerClass::~SystemControllerClass()
{}

bool SystemControllerClass::Initialize()
{
    fDaqControl->InitDAQ();
    fDatabase->InitDatabase();

    for(size_t i = 0 ; i != fNamesSources.size() ; i++){
        getObject(fNamesSources[i])->initialize();
        //getObject(fNamesSources[i])->offPower(0);
    }
    //if(fConnectRasp != NULL);
        //fConnectRasp->raspInitialize();

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
void SystemControllerClass::startDoingList()
{
    QThread *cThread = new QThread();
    for(vector<fParameters>::iterator cIter= fListOfCommands.begin() ; cIter != fListOfCommands.end() ; ++cIter){
        string cStr = (*cIter).cName;

        double cValue = (*cIter).cValue;
        if(cStr == "Set Temperature (°C)"){
            moveToThread(cThread);
            connect(cThread, &QThread::started, [this, cValue]
            {this->setTemperature(cValue);});
            cThread->start();
        }
        if(cStr == "Wait (Sec)"){
            connect(cThread, &QThread::started, [this, cValue]
            {this->Wait(cValue);});
            cThread->start();
        }
        for(size_t i = 0 ; i != fNamesSources.size() ; i++){
            if(cStr == "On  " + fNamesSources[i] + "  power supply"){
                connect(cThread, &QThread::started, [this, i]
                {this->onPower(fNamesSources[i]);});
                cThread->start();
                getObject(fNamesSources[i])->onPower(1);
                emit sendOnOff(fNamesSources[i] , true);
            }
            if(cStr == "Off  " + fNamesSources[i] + "  power supply"){
                connect(cThread, &QThread::started, [this, i]
                {this->offPower(fNamesSources[i]);});
                cThread->start();
                getObject(fNamesSources[i])->offPower(1);
                emit sendOnOff(fNamesSources[i] , false);
            }
        }
    }
}

void SystemControllerClass::setTemperature(double pTemp)
{
//    getGenericInstrObj("JulaboFP50")->SetWorkingTemperature(pTemp);
}

void SystemControllerClass::wait(double pTime)
{
    QThread::sleep(pTime);
}

void SystemControllerClass::onPower(string pSourceName)
{
    getObject(pSourceName)->onPower(0);
}

void SystemControllerClass::offPower(string pSourceName)
{
    getObject(pSourceName)->offPower(0);
}


//reads file and makes map with name and object of power supply
void SystemControllerClass::ParseVSources()
{
    string cConnection , cSetVolt , cSetCurr , cAddress , cPort, cnOutputs;
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
                fGenericInstrumentMap.insert(pair<string , GenericInstrumentClass*>(fHWDescription[i].name , fPowerLow));
                fNamesSources.push_back(fHWDescription[i].name);
            }
        }
        if(fHWDescription[i].type == "HighVoltageSource"){

            if(fHWDescription[i].typeOfClass == "Keithley2410"){
                cConnection = fHWDescription[i].interface_settings["connection"];
                cAddress = fHWDescription[i].interface_settings["address"];
                cPort = fHWDescription[i].interface_settings["port"];
                cSetVolt = fHWDescription[i].operational_settings[0]["Voltage"];
                cSetCurr = fHWDescription[i].operational_settings[0]["CurrentLimit"];
                string cConnectStr = getTypeOfConnection(cConnection , cAddress , cPort);
                PowerControlClass *fPowerHigh = new ControlKeithleyPower(cConnectStr, cSetVolt, cSetCurr);
                fMapSources.insert(pair<string , PowerControlClass*>(fHWDescription[i].name , fPowerHigh));
                fGenericInstrumentMap.insert(pair<string , GenericInstrumentClass*>(fHWDescription[i].name , fPowerHigh));
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

            //fConnectRasp = new ConnectionInterfaceClass(cAddress , cPort);
            //fGenericInstrumentMap.insert(pair<string , GenericInstrumentClass*>(fHWDescription[i].name , fConnectRasp));

            for(int j = 0 ; j != fHWDescription[i].operational_settings.size() ; j++){
                fRaspberrySensorsNames.push_back(fHWDescription[i].operational_settings[j]["sensor"]);
            }
        }
    }
}

void SystemControllerClass::ParseChiller()
{
    string cAddress, cConnection;
    for(int i = 0 ; i != fHWDescription.size() ; i++){

        if(fHWDescription[i].type == "Chiller"){
            cAddress = fHWDescription[i].interface_settings["address"];
            cConnection = fHWDescription[i].interface_settings["connection"];
            //cAddress = getTypeOfConnection(cConnection, cAddress, "");
            EnvironmentControlClass *fChiller = new JulaboFP50(cAddress.c_str());

            fGenericInstrumentMap.insert(pair<string , GenericInstrumentClass*>(fHWDescription[i].name , fChiller));

        }
    }
}

GenericInstrumentClass* SystemControllerClass::getGenericInstrObj(string pStr)
{
    return fGenericInstrumentMap[pStr];
}

void SystemControllerClass::ReadXmlFile(std::string pFileName)
{
    HWDescriptionParser cParser;
    fHWDescription = cParser.ParseXML(pFileName);

    this->ParseVSources();
    this->ParseRaspberry();
    this->ParseChiller();

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
        int cNum = stoi(pAddress);
        pAddress = to_string(cNum + 1);
        cStr = "ASRL" + pAddress + "::INSTR";
        }
    return cStr;
}

void SystemControllerClass::closeConneections()
{
    for(size_t i = 0 ; i != fNamesSources.size() ; i++){
        getObject(fNamesSources[i])->closeConnection();
    }
    fConnectRasp->closeConnection();
}

