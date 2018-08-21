#ifndef CONTROLKEITHLEYPOWER_H
#define CONTROLKEITHLEYPOWER_H

#include "powercontrolclass.h"

class ControlKeithleyPower:public PowerControlClass
{
public:
    ControlKeithleyPower();
    bool InitPwr();
    QString getVoltAndCurr();
    void setVolt(double pVolt);
    void onPower();
    void offPower();

    double fVolt;
    double fCurr;
    double fStep;
    double fCurrCompliance;

    ViSession fDefaultRm;
    ViSession fVi;
    ViStatus fStatus;
};

#endif // CONTROLKEITHLEYPOWER_H
