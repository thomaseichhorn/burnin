//main control class which supervises all the other classes
#ifndef SYSTEMCONTROLLERCLASS_H
#define SYSTEMCONTROLLERCLASS_H

#include <string>

#include "VoltageControl/powercontrolclass.h"
#include "daqcontrolclass.h"
#include "environmentcontrolclass.h"
#include "databaseinterfaceclass.h"
#include "VoltageControl/controlttipower.h"
#include "connectioninterfaceclass.h"
#include "VoltageControl/controlkeithleypower.h"
#include "External/JulaboFP50.h"


using namespace  std;

class SystemControllerClass
{
public:
    SystemControllerClass();
    ~SystemControllerClass();

    ConnectionInterfaceClass *fConnectRasp;
    DAQControlClass *fDaqControl;
    EnvironmentControlClass *fEnv;
    DatabaseInterfaceClass *fDatabase;
    ControlKeithleyPower *fKeithleyVolt;
    ControlTTiPower *fTTiVolt;
    //struct for the vector which contains commands
    struct fObjParam{
        string cName;
        double cValue;
    };
    PowerControlClass* getObject(string pStr);
    vector<string> getSourceNameVec();
    vector<SystemControllerClass::fObjParam> fListOfCommands;
    //check if all devices are connected
    void Initialize();
    vector<QString>* readFile();
    void doList();
    void Wait(int pSec);
    map<string, PowerControlClass* > fMapSources;
    vector<string> fNamesSources;
    void ParseVSources();
private:
};

#endif // SYSTEMCONTROLLERCLASS_H
