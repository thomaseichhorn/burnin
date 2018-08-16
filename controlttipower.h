#ifndef CONTROLTTIPOWER_H
#define CONTROLTTIPOWER_H

#include <QObject>

#include "visa.h"
#include "visatype.h"

#include "powercontrolclass.h"

using namespace std;

class ControlTTiPower:public PowerControlClass
{
public:
    ControlTTiPower();
    bool InitPwr();
    QString getVoltAndCurr();
    QString getVoltSet(int pId);
    QString getCurrSet(int pId);
    QString getVoltApp(int pId);
    QString getCurrApp(int pId);
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
