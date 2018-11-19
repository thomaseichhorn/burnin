#include "julabowrapper.h"

JulaboWrapper::JulaboWrapper(string pStr)
{
    fIoport_t = pStr;
}

void JulaboWrapper::initialize()
{
    const char* ioport_t  = fIoport_t.c_str();
    JulaboFP50 *cJulabo = new JulaboFP50(ioport_t);
    fJulabo = cJulabo;
}
