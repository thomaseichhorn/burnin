//connects to the raspberry and get the information from sensors
#ifndef CONNECTIONINTERFACECLASS_H
#define CONNECTIONINTERFACECLASS_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <iostream>

#include "genericinstrumentclass.h"

using namespace  std;
class ConnectionInterfaceClass : public QObject,public GenericInstrumentClass
{
    Q_OBJECT
public:
    explicit ConnectionInterfaceClass(string pAddress , string pPort);
    void initialize();
    QString getInfoFromSensors();

    string getName(){
        return "ConnectionInterfaceClass";
    }
    void closeConnection();

signals:

public slots:
private:
    QTcpSocket *fSocket;
    quint16 fPort;
    string fAddress;
};

#endif // CONNECTIONINTERFACECLASS_H
