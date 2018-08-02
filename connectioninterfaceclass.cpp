#include "connectioninterfaceclass.h"

#include <QThread>
#include <QString>

#include <iostream>

using namespace std;

ConnectionInterfaceClass::ConnectionInterfaceClass(QObject *parent) : QObject(parent)
{
}

bool ConnectionInterfaceClass::raspInitialize()
{
    fSocket = new QTcpSocket(this);
    fSocket->connectToHost("fhlthermorasp1.desy.de" , 50007);
    if(!fSocket->waitForDisconnected(5000)){
        qDebug() << "Error:" << fSocket->errorString();
        return false;
    }
    else{
        printf("Connected to raspberry pi!");
        return true;
    }
    fSocket->close();
}

double ConnectionInterfaceClass::getMeasurement()
{
    //returns information from sensors on raspberry
}
