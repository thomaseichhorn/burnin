#ifndef CONTROLKEITHLEYPOWER_H
#define CONTROLKEITHLEYPOWER_H

#include <QObject>
#include <QThread>

#include "powercontrolclass.h"
#include "general/ComHandler.h"

class ControlKeithleyPower:public PowerControlClass, public QThread
{
public:
    ControlKeithleyPower(string pConnection, double pSetVolt, double pSetCurr);

    void initialize();
    PowerControlClass::fVACvalues* getVoltAndCurr();
    void setVolt(double pVoltage, int = 0);
    void setCurr(double pCurrent , int = 0);
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

    ComHandler* comHandler_;

    bool keithleyOutputOn;

    void setKeithleyOutputState ( int outputsetting );
    bool getKeithleyOutputState ( );
};

#endif // CONTROLKEITHLEYPOWER_H
