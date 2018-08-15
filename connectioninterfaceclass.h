//connects to the raspberry and get the information from sensors
#ifndef CONNECTIONINTERFACECLASS_H
#define CONNECTIONINTERFACECLASS_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <iostream>

class ConnectionInterfaceClass : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionInterfaceClass(QObject *parent = nullptr);
    bool raspInitialize();
    double getMeasurement();
    QString getInfoFromSensors();
signals:

public slots:
private:
    QTcpSocket *fSocket;
};

#endif // CONNECTIONINTERFACECLASS_H
