#ifndef CONTROLTTIPOWER_H
#define CONTROLTTIPOWER_H

#include "powercontrolclass.h"

#include "visa.h"
#include "visatype.h"

#include <QObject>

#include <string>
#include <vector>

using namespace std;

class ControlTTiPower:public PowerControlClass
{
public:
    ControlTTiPower();
    bool InitPwr();
    void getVoltAndCurrSet(int pId);
    void getVoltAndCurrApp(int pId);
    void setVolt(int pId , double pVoltage);
    void setCurr(int pId , double pCurrent);
    void setVoltAndCurr(double cI1 , double cV1 , double cI2 , double cV2);
    void onPower(int pId);
    void offPower(int pId);
    void onPowerAll();
    void offPowerAll();
private:
};
#endif // CONTROLTTIPOWER_H
