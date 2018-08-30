#ifndef GENERICINSTRUMENTCLASS_H
#define GENERICINSTRUMENTCLASS_H

#include <string>

using namespace std;

class GenericInstrumentClass
{
public:
    GenericInstrumentClass();

    virtual string getName() = 0;

    //funcs for JulaboFP50
//    virtual unsigned int GetPumpPressure( void ) const = 0;
//    virtual float GetBathTemperature( void ) const = 0;
//    virtual float GetSafetySensorTemperature( void ) const = 0;
//    virtual float GetWorkingTemperature( void ) const = 0;
//    virtual bool SetCirculatorOn( void ) const = 0;
//    virtual bool SetCirculatorOff( void ) const = 0;
//    virtual bool SetWorkingTemperature( const float ) const = 0;

    //funcs for power supplies
//    virtual bool InitPwr() = 0;
//    virtual fVACvalues* getVoltAndCurr() = 0;
//    virtual void setVolt(double pVoltage, int pId) = 0;
//    virtual void setCurr(double pCurrent , int pId) = 0;
//    virtual void onPower(int pId) = 0;
//    virtual void offPower(int pId) = 0;
//    virtual void closeConnection() = 0;
};

#endif // GENERICINSTRUMENTCLASS_H
