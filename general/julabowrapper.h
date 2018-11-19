#ifndef JULABOWRAPPER_H
#define JULABOWRAPPER_H


#include "JulaboFP50.h"
#include "genericinstrumentclass.h"

class JulaboWrapper:public GenericInstrumentClass
{
public:
    JulaboWrapper(string);

    string fIoport_t;
    JulaboFP50 *fJulabo;

    void initialize();
};

#endif // JULABOWRAPPER_H

