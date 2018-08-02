#ifndef POWERCONTROLCLASS_H
#define POWERCONTROLCLASS_H

#include <QTcpSocket>

class PowerControlClass
{
public:
    PowerControlClass();
    void InitPwr();
    void setVoltage(double volt);
    double getVoltage();
    bool onSupply();
    bool offSupply();
private:

};

#endif // POWERCONTROLCLASS_H
