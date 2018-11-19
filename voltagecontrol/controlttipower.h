#ifndef CONTROLTTIPOWER_H
#define CONTROLTTIPOWER_H

#include <QObject>

extern "C" {
	#include "lxi.h"
}

#include "powercontrolclass.h"

using namespace std;

class ControlTTiPower:public PowerControlClass
{

public:
    ControlTTiPower(string pConnection , vector<string> pId , vector<string> pVolt , vector<string> pCurr);

    int fId;

    void initialize();
    PowerControlClass::fVACvalues* getVoltAndCurr();

    void setVolt(double pVoltage , int pId);
    void setCurr(double pCurrent , int pId);
    void onPower(int pId);
    void offPower(int pId);
    void closeConnection();

    string fConnection;

    int fId1;
    int fId2;

    int fDevice;
    
    vector<double> fVoltSet;
    vector<double> fCurrSet;
private:
};
#endif // CONTROLTTIPOWER_H
