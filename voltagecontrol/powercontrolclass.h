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
    struct fVACvalues
    {
        double pVApp1;
        double pIApp1;
        double pVSet1;
        double pISet1;
        double pVApp2;
        double pIApp2;
        double pVSet2;
        double pISet2;
    };

    virtual bool InitPwr() = 0;
    virtual fVACvalues* getVoltAndCurr() = 0;
    virtual void setVolt(double pVoltage, int pId) = 0;
    virtual void setCurr(double pCurrent , int pId) = 0;
    virtual void onPower(int pId) = 0;
    virtual void offPower(int pId) = 0;
    
    int fNOutputs;
};

#endif // POWERCONTROLCLASS_H