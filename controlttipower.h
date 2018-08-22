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

    ViSession fVi;
    ViSession fDefaultRm;
    ViStatus fStatus;
    int fId;

    bool InitPwr();
    fVACvalues* getVoltAndCurr();

    void setVolt(double pVoltage , int pId);
    void setCurr(double pCurrent , int pId);
    void onPower(int pId);
    void offPower(int pId);

    QString deleteSpaces(QString pStr);
    QString transformQString(QString pStr);
private:
};
#endif // CONTROLTTIPOWER_H
