#ifndef ADDITIONALTHREAD_H
#define ADDITIONALTHREAD_H

#include <vector>

#include <QObject>
#include <QString>

#include "VoltageControl/controlttipower.h"
#include "systemcontrollerclass.h"
#include "connectioninterfaceclass.h"


class AdditionalThread : public QObject
{
    Q_OBJECT
public:
    AdditionalThread(QString pName , SystemControllerClass *pControl);

signals:
    void sendToThread(PowerControlClass::fVACvalues *);
    void sendToThreadKeithley(PowerControlClass::fVACvalues *);
    void sendToThreadString(QString);

public slots:
    void getVAC();
    void getRaspSensors();
    void getVACKeithley();

private:
    QString fName;
    SystemControllerClass *fRaspControl;
    SystemControllerClass *fAddControl;
    SystemControllerClass *fkeithley;
};

#endif // ADDITIONALTHREAD_H
