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
    ControlTTiPower(string pConnection , vector<string> pId , vector<string> pVolt , vector<string> pCurr);

    ViSession fVi;
    ViSession fDefaultRm;
    ViStatus fStatus;
    int fId;

    bool initialize();
    PowerControlClass::fVACvalues* getVoltAndCurr();

    void setVolt(double pVoltage , int pId);
    void setCurr(double pCurrent , int pId);
    void onPower(int pId);
    void offPower(int pId);
    void closeConnection();

    string getName(){
        string cStr = "ControlTTIClass";
        return cStr;
    }

    string fConnection;
    int fId1;
    double fSetVolt1;
    double fSetCurr1;
    double fSetVolt2;
    double fSetCurr2;
    int fId2;
private:
};
#endif // CONTROLTTIPOWER_H
