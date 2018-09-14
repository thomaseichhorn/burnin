#ifndef ADDITIONALTHREAD_H
#define ADDITIONALTHREAD_H

#include <vector>

#include <QObject>
#include <QString>

#include "voltagecontrol/controlttipower.h"
#include "general/systemcontrollerclass.h"
#include "general/connectioninterfaceclass.h"


class AdditionalThread : public QObject
{
    Q_OBJECT
public:
    AdditionalThread(QString pName , SystemControllerClass *pControl);

signals:
    void sendToThread(PowerControlClass::fVACvalues *);
    void sendToThreadKeithley(PowerControlClass::fVACvalues *);
    void sendToThreadString(QString);
    void volt();
    void sendFromChiller(QString);

public slots:
    void getVAC();
    void getRaspSensors();
    void getVACKeithley();
    void onVolt();
    void offVolt();
    void getChillerStatus();
private:
    QString fName;
    SystemControllerClass *fRaspControl;
    SystemControllerClass *fAddControl;
    SystemControllerClass *fkeithley;
};

#endif // ADDITIONALTHREAD_H
