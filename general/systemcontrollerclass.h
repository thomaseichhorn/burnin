//main control class which supervises all the other classes
#ifndef SYSTEMCONTROLLERCLASS_H
#define SYSTEMCONTROLLERCLASS_H

#include <string>

#include <QThread>

#include "voltagecontrol/powercontrolclass.h"
#include "daqcontrolclass.h"
#include "environmentcontrolclass.h"
#include "databaseinterfaceclass.h"
#include "voltagecontrol/controlttipower.h"
#include "connectioninterfaceclass.h"
#include "voltagecontrol/controlkeithleypower.h"
#include "external/cmstkmodlab/devices/Julabo/JulaboFP50.h"
#include "additional/hwdescriptionparser.h"
#include "genericinstrumentclass.h"


using namespace  std;

class SystemControllerClass:public QThread
{
    Q_OBJECT
public:
    SystemControllerClass();
    ~SystemControllerClass();

    ConnectionInterfaceClass *fConnectRasp;
    DAQControlClass *fDaqControl;
    EnvironmentControlClass *fEnv;
    DatabaseInterfaceClass *fDatabase;
    vector<string> fRaspberrySensorsNames;
    vector<GenericInstrumentDescription_t> fHWDescription;
    map<string, PowerControlClass* > fMapSources;
    map<string , GenericInstrumentClass*> fGenericInstrumentMap;
    vector<string> fNamesSources;
    //struct for the vector which contains commands
    struct fParameters{
        string cName;
        double cValue;
    };
    vector<SystemControllerClass::fParameters> fListOfCommands;
    //check if all devices are connected
    bool Initialize();
    void ReadXmlFile(std::string pFileName);
    void startDoingList();
    void Wait(int pSec);
    void closeConneections();
    PowerControlClass* getObject(string pStr);
    GenericInstrumentClass* getGenericInstrObj(string pStr);
    vector<string> getSourceNameVec();
    vector<QString>* readFile();

private:
    string getTypeOfConnection(string pConnection , string pAddress, string pPort);
    string ParseChiller();
    void ParseVSources();
    void ParseRaspberry();

private slots:
    void setTemperature(double pTemp);
    void wait(double pTime);
    void onPower(string pSourceName);
    void offPower(string pSourceName);

signals:
    void sendOnOff(string pSourceName , bool pArg);

};

#endif // SYSTEMCONTROLLERCLASS_H
