#ifndef CONTROLKEITHLEYPOWER_H
#define CONTROLKEITHLEYPOWER_H

#include "powercontrolclass.h"

class ControlKeithleyPower:public PowerControlClass
{
public:
    ControlKeithleyPower();

    bool InitPwr();
    PowerControlClass::fVACvalues* getVoltAndCurr();
    void setVolt(double pVoltage, int pId);
    void setCurr(double pCurrent , int pId);
    void onPower(int pId);
    void offPower(int pId);

    void makeVolt(double pVoltage);
    void checkVAC();

    double fVolt;
    double fVoltSet;
    double fCurr;
    double fStep;
    double fCurrCompliance;

    ViSession fDefaultRm;
    ViSession fVi;
    ViStatus fStatus;
};

#endif // CONTROLKEITHLEYPOWER_H
