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
    bool doConnect();
    double getMeasurement();
signals:

public slots:
private:
    QTcpSocket *cSocket;
};

#endif // CONNECTIONINTERFACECLASS_H
