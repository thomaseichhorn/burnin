#include <string>
#include <vector>
#include <regex>

#include <QCoreApplication>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QTime>

#include "general/systemcontrollerclass.h"
#include "additional/hwdescriptionparser.h"
#include "julabowrapper.h"

using namespace std;

SystemControllerClass::SystemControllerClass()
{
    fDaqControl = new DAQControlClass();
    fDatabase = new DatabaseInterfaceClass();
    fConnectRasp = NULL;
}

SystemControllerClass::~SystemControllerClass()
{}

void SystemControllerClass::Initialize()
{

//    for(size_t i = 0 ; i != fNamesVoltageSources.size() ; i++){
//        getObject(fNamesVoltageSources[i])->initialize();
//        getObject(fNamesSources[i])->offPower(0);
//    }
    //if(fConnectRasp != NULL);
        //fConnectRasp->raspInitialize();

    for(auto &i: fGenericInstrumentMap){
        i.second->initialize();
    }

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
           // moveToThread(cThread);
            connect(cThread, &QThread::started, [this, cValue] {
                dynamic_cast<JulaboWrapper*>(this->getGenericInstrObj("JulaboFP50"))->fJulabo->SetWorkingTemperature(cValue);
            });
            cThread->start();
        }
        if(cStr == "Wait (Sec)"){
            connect(cThread, &QThread::started, [this, cValue]
            {this->Wait(cValue);});
            cThread->start();
        }
        for(size_t i = 0 ; i != fNamesVoltageSources.size() ; i++){
            if(cStr == "On  " + fNamesVoltageSources[i] + "  power supply"){
                connect(cThread, &QThread::started, [this, i]
                {this->onPower(fNamesVoltageSources[i]);});
                cThread->start();
                getObject(fNamesVoltageSources[i])->onPower(1);
                emit sendOnOff(fNamesVoltageSources[i] , true);
            }
            if(cStr == "Off  " + fNamesVoltageSources[i] + "  power supply"){
                connect(cThread, &QThread::started, [this, i]
                {this->offPower(fNamesVoltageSources[i]);});
                cThread->start();
                getObject(fNamesVoltageSources[i])->offPower(1);
                emit sendOnOff(fNamesVoltageSources[i] , false);
            }
        }
    }
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
    string cConnection, cSetVolt, cSetCurr, cAddress;

    for(size_t i = 0 ; i != fHWDescription.size() ; i++){

        if(fHWDescription[i].typeOfClass == "LowVoltageSource"){

            if(fHWDescription[i].classOfInstr == "TTI"){
                int cPort;
                vector<double> cVolt(2, 0);
                vector<double> cCurr(2, 0);
                
                cConnection = fHWDescription[i].interface_settings["connection"];
                cAddress = fHWDescription[i].interface_settings["address"];
                cPort = stoi(fHWDescription[i].interface_settings["port"]);

                int opset_size = fHWDescription[i].operational_settings.size();
                for (int j = 0; j < min(2, opset_size); ++j) {
                    cVolt[1 - j] = stod(fHWDescription[i].operational_settings[j]["Voltage"]);
                    cCurr[1 - j] = stod(fHWDescription[i].operational_settings[j]["CurrentLimit"]);
                }
                PowerControlClass *fPowerLow = new ControlTTiPower(cAddress, cPort, cVolt, cCurr);
                
                fMapSources.insert(pair<string , PowerControlClass*>(fHWDescription[i].name , fPowerLow));
                fGenericInstrumentMap.insert(pair<string , GenericInstrumentClass*>(fHWDescription[i].name , fPowerLow));
                fNamesVoltageSources.push_back(fHWDescription[i].name);
                fNamesInstruments.push_back(fHWDescription[i].classOfInstr);
                std::cout << "Added \"" << fHWDescription[i].name << "\" on " << cAddress << std::endl;
            }
        }

        if(fHWDescription[i].typeOfClass == "HighVoltageSource"){

            if(fHWDescription[i].classOfInstr == "Keithley2410"){
                cConnection = fHWDescription[i].interface_settings["connection"];
                cAddress = fHWDescription[i].interface_settings["address"];
                cSetVolt = fHWDescription[i].operational_settings[0]["Voltage"];
                cSetCurr = fHWDescription[i].operational_settings[0]["CurrentLimit"];
                PowerControlClass *fPowerHigh = new ControlKeithleyPower(cAddress, cSetVolt, cSetCurr);
                fMapSources.insert(pair<string , PowerControlClass*>(fHWDescription[i].name , fPowerHigh));
                fGenericInstrumentMap.insert(pair<string , GenericInstrumentClass*>(fHWDescription[i].name , fPowerHigh));
                fNamesVoltageSources.push_back(fHWDescription[i].name);
                fNamesInstruments.push_back(fHWDescription[i].classOfInstr);
                std::cout << "Added \"" << fHWDescription[i].name << "\" on " << cAddress << std::endl;
            }
        }
    }
}

void SystemControllerClass::ParseRaspberry()
{
    string cConnection, cAddress;
    quint16 cPort;

    for(size_t i = 0 ; i != fHWDescription.size() ; i++){

        if(fHWDescription[i].classOfInstr == "Raspberry"){
            cConnection = fHWDescription[i].interface_settings["connection"];
            cAddress = fHWDescription[i].interface_settings["address"];
            cPort = stoi(fHWDescription[i].interface_settings["port"]);
            fConnectRasp = new Thermorasp(cAddress , cPort);
            fGenericInstrumentMap.insert(pair<string , GenericInstrumentClass*>(fHWDescription[i].name , fConnectRasp));
            fNamesInstruments.push_back(fHWDescription[i].name);

            for(size_t j = 0 ; j != fHWDescription[i].operational_settings.size() ; j++){
                fRaspberrySensorsNames.push_back(fHWDescription[i].operational_settings[j]["sensor"]);

            }
        }
    }
}

void SystemControllerClass::ParseChiller()
{
    string cAddress, cConnection;
    for(size_t i = 0 ; i != fHWDescription.size() ; i++){

        if(fHWDescription[i].classOfInstr == "Chiller"){
            cAddress = fHWDescription[i].interface_settings["address"];
            cConnection = fHWDescription[i].interface_settings["connection"];

            GenericInstrumentClass* fJulaboWrapper = new JulaboWrapper(cAddress);
            fGenericInstrumentMap.insert(pair<string , GenericInstrumentClass*>(fHWDescription[i].name , fJulaboWrapper));
            fNamesInstruments.push_back(fHWDescription[i].classOfInstr);

        }
    }
}

int SystemControllerClass::countIntrument(string instrument_name) {
    return fGenericInstrumentMap.count(instrument_name);
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
    return fNamesVoltageSources;
}
