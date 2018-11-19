#ifndef CONTROLKEITHLEYPOWER_H
#define CONTROLKEITHLEYPOWER_H

#include <QObject>
#include <QThread>

#include "powercontrolclass.h"
#include "general/FP50ComHandler.h"

class ControlKeithleyPower:public PowerControlClass, public QThread
{
public:
    ControlKeithleyPower(string pConnection , string pSetVolt , string pSetCurr);

    void initialize();
    PowerControlClass::fVACvalues* getVoltAndCurr();
    void setVolt(double pVoltage, int);
    void setCurr(double pCurrent , int);
    void onPower(int);
    void offPower(int);
    void closeConnection();

    void sweepVolt(double pVoltage);
    void checkVAC();

    double fVolt;
    double fVoltSet;
    double fCurr;
    double fStep;
    double fCurrCompliance;
    string fConnection;

    FP50ComHandler* comHandler_;
    
};

#endif // CONTROLKEITHLEYPOWER_H
