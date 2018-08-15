//generic class for control the voltages on the modules
#ifndef POWERCONTROLCLASS_H
#define POWERCONTROLCLASS_H

#include <QString>

#include "visa.h"
#include "visatype.h"

using namespace std;

class PowerControlClass
{
public:
    PowerControlClass();
    ViSession fVi;
    ViSession fDefaultRm;
    ViStatus fStatus;
    int fId;

    virtual bool InitPwr() = 0;
    virtual QString getVoltAndCurr(int pId) = 0;
    virtual QString getVoltSet(int pId) = 0;
    virtual QString getCurrSet(int pId) = 0;
    virtual QString getVoltApp(int pId) = 0;
    virtual QString getCurrApp(int pId) = 0;
    virtual void setVolt(int pId, double pVoltage) = 0;
    virtual void setCurr(int pId , double pCurrent) = 0;
    virtual void setVoltAndCurr(double pI1 , double pV1 , double pI2 , double pV2) = 0;
    virtual void onPower(int pId) = 0;
    virtual void offPower(int pId) = 0;
    virtual void onPowerAll() = 0;
    virtual void offPowerAll() = 0;
};

#endif // POWERCONTROLCLASS_H
