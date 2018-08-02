#ifndef SYSTEMCONTROLLERCLASS_H
#define SYSTEMCONTROLLERCLASS_H

#include "powercontrolclass.h"
#include "daqcontrolclass.h"
#include "environmentcontrolclass.h"
#include "databaseinterfaceclass.h"

#include <string>
#include <map>

using namespace  std;

class PowerControlClass;

class SystemControllerClass
{
public:
    SystemControllerClass();
    void Initialize();
private:
    PowerControlClass *fVolt;
    DAQControlClass *fDaqControl;
    EnvironmentControlClass *fEnv;
    DatabaseInterfaceClass *fDatabase;

    void doWait();
    void doList(vector<string> *);
    vector<string>* readFile();
};

#endif // SYSTEMCONTROLLERCLASS_H
