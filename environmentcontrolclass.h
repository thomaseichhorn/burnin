//class wich interfaces all enviromental
#ifndef ENVIRONMENTCONTROLCLASS_H
#define ENVIRONMENTCONTROLCLASS_H

#include "connectioninterfaceclass.h" 

class EnvironmentControlClass
{
public:
    EnvironmentControlClass();
    void InitEnv();
    double getTemp();
    void setTemp(double pTemp);
private:

};

#endif // ENVIRONMENTCONTROLCLASS_H
