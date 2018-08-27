//connects to the raspberry and get the information from sensors
#ifndef CONNECTIONINTERFACECLASS_H
#define CONNECTIONINTERFACECLASS_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <iostream>

using namespace  std;
class ConnectionInterfaceClass : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionInterfaceClass(string pAddress , string pPort);
    bool raspInitialize();
    QString getInfoFromSensors();
signals:

public slots:
private:
    QTcpSocket *fSocket;
    quint16 fPort;
    string fAddress;
};

#endif // CONNECTIONINTERFACECLASS_H
