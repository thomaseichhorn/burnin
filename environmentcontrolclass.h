//class wich interfaces all enviromental
#ifndef ENVIRONMENTCONTROLCLASS_H
#define ENVIRONMENTCONTROLCLASS_H

#include "visa.h"
#include "visatype.h"

#include "connectioninterfaceclass.h"
#include "genericinstrumentclass.h"

class EnvironmentControlClass:public GenericInstrumentClass
{
public:
    EnvironmentControlClass();

    virtual bool SetWorkingTemperature( const float ) const = 0;
    virtual bool SetPumpPressure( const unsigned int ) const = 0;
    virtual bool SetCirculatorOn( void ) const = 0;
    virtual bool SetCirculatorOff( void ) const = 0;
    virtual bool SetControlParameters( float, int, int ) const = 0;

    virtual bool IsCommunication( void ) const  { return isCommunication_; }
    virtual float GetBathTemperature( void ) const = 0;
    virtual float GetSafetySensorTemperature( void ) const = 0;
    virtual float GetWorkingTemperature( void ) const = 0;
    virtual int GetHeatingPower( void ) const = 0;
    virtual unsigned int GetPumpPressure( void ) const = 0;
    virtual bool GetCirculatorStatus( void ) const = 0;
    virtual std::pair<int,std::string> GetStatus( void ) const = 0;
    virtual float GetProportionalParameter( void ) const = 0;
    virtual int GetIntegralParameter( void ) const = 0;
    virtual int GetDifferentialParameter( void ) const = 0;

    virtual bool SaveControlParameters( const std::string& ) const = 0;
    virtual bool LoadControlParametersAndApply( const std::string& ) const = 0;
    virtual void StripBuffer( char* ) const = 0;

private:
    bool isCommunication_;

};

#endif // ENVIRONMENTCONTROLCLASS_H
