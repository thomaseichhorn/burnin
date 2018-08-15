//main control class which supervises all the other classes
#ifndef SYSTEMCONTROLLERCLASS_H
#define SYSTEMCONTROLLERCLASS_H

#include <string>

#include "powercontrolclass.h"
#include "daqcontrolclass.h"
#include "environmentcontrolclass.h"
#include "databaseinterfaceclass.h"
#include "controlttipower.h"
#include "connectioninterfaceclass.h"


using namespace  std;

class SystemControllerClass
{
public:
    SystemControllerClass();
    ~SystemControllerClass();

    ConnectionInterfaceClass *fConnectRasp;
    ControlTTiPower *fTTiVolt;
    DAQControlClass *fDaqControl;
    EnvironmentControlClass *fEnv;
    DatabaseInterfaceClass *fDatabase;
    //struct for the vector which contains commands
    struct fObjParam{
        string cName;
        double cValue;
    };
    //check if all devices are connected
    void Initialize();
    vector<QString>* readFile();
    vector<fObjParam>* doList(vector<string> *);
    void Wait(int pSec);

private:
};

#endif // SYSTEMCONTROLLERCLASS_H
