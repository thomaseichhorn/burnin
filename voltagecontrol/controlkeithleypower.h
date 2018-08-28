#ifndef CONTROLKEITHLEYPOWER_H
#define CONTROLKEITHLEYPOWER_H

#include <QObject>
#include <QThread>

#include "powercontrolclass.h"

class ControlKeithleyPower:public PowerControlClass, public QThread
{
public:
    ControlKeithleyPower(string pConnection , string pSetVolt , string pSetCurr);

    bool InitPwr();
    PowerControlClass::fVACvalues* getVoltAndCurr();
    void setVolt(double pVoltage, int pId);
    void setCurr(double pCurrent , int pId);
    void onPower(int pId);
    void offPower(int pId);

    void sweepVolt(double pVoltage);
    void checkVAC();

    double fVolt;
    double fVoltSet;
    double fCurr;
    double fStep;
    double fCurrCompliance;
    string fConnection;

    ViSession fDefaultRm;
    ViSession fVi;
    ViStatus fStatus;
};

#endif // CONTROLKEITHLEYPOWER_H
