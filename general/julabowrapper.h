#ifndef JULABOWRAPPER_H
#define JULABOWRAPPER_H


#include "external/cmstkmodlab/devices/Julabo/JulaboFP50.h"
#include "general/genericinstrumentclass.h"

class JulaboWrapper:public GenericInstrumentClass
{
public:
    JulaboWrapper(string);

    string fIoport_t;
    JulaboFP50 *fJulabo;

    bool initialize();
};

#endif // JULABOWRAPPER_H

