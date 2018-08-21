//class wich interfaces all enviromental
#ifndef ENVIRONMENTCONTROLCLASS_H
#define ENVIRONMENTCONTROLCLASS_H

#include "visa.h"
#include "visatype.h"

#include "connectioninterfaceclass.h" 

class EnvironmentControlClass
{
public:
    EnvironmentControlClass();
    bool InitEnv();
    bool InitChiller();
    double getTemp();
    void setTemp(double pTemp);

private:
    ViSession fVi , fDefaultRm;
    ViStatus fStatus;
};

#endif // ENVIRONMENTCONTROLCLASS_H
