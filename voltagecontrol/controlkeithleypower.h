#ifndef CONTROLKEITHLEYPOWER_H
#define CONTROLKEITHLEYPOWER_H

#include <QObject>
#include <QThread>

#include "powercontrolclass.h"
#include "external/cmstkmodlab/devices/Julabo/FP50ComHandler.h"

class ControlKeithleyPower:public PowerControlClass, public QThread
{
public:
    ControlKeithleyPower(string pConnection , string pSetVolt , string pSetCurr);

    bool initialize();
    PowerControlClass::fVACvalues* getVoltAndCurr();
    void setVolt(double pVoltage, int pId);
    void setCurr(double pCurrent , int pId);
    void onPower(int pId);
    void offPower(int pId);
    void closeConnection();

    void sweepVolt(double pVoltage);
    void checkVAC();

    string getName(){
        string cStr = "ControlKeithleyClass";
        return cStr;
    }

    double fVolt;
    double fVoltSet;
    double fCurr;
    double fStep;
    double fCurrCompliance;
    string fConnection;

    FP50ComHandler* comHandler_;
    //bool isCommmunication_;
};

#endif // CONTROLKEITHLEYPOWER_H
