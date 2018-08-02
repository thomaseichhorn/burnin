#ifndef SYSTEMCONTROLLERCLASS_H
#define SYSTEMCONTROLLERCLASS_H

#include "powercontrolclass.h"
#include "daqcontrolclass.h"
#include "environmentcontrolclass.h"
#include "databaseinterfaceclass.h"
#include "controlttipower.h"
#include "connectioninterfaceclass.h"

#include <string>

using namespace  std;

class SystemControllerClass
{
public:
    SystemControllerClass();
    ~SystemControllerClass();
    struct fParam{
        string cName;
        double cValue;
    };
    void Initialize();
    vector<string>* readFile();
    vector<fParam>* doList(vector<string> *);
    void doWait(int pSec);

private:
    ConnectionInterfaceClass *fConnect;
    ControlTTiPower *fTTiVolt;
    DAQControlClass *fDaqControl;
    EnvironmentControlClass *fEnv;
    DatabaseInterfaceClass *fDatabase;
};

#endif // SYSTEMCONTROLLERCLASS_H
