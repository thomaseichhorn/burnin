#include <iostream>

#include <QThread>
#include <QString>
#include <QTextCodec>

#include "connectioninterfaceclass.h"
#include "mainwindow.h"

using namespace std;

ConnectionInterfaceClass::ConnectionInterfaceClass(QObject *parent) : QObject(parent)
{}

bool ConnectionInterfaceClass::raspInitialize()
{
    fSocket = new QTcpSocket(this);
    fSocket->connectToHost("fhlthermorasp1.desy.de", 50007);
    if(!fSocket->waitForDisconnected(5000)){
        qDebug() << "Error:" << fSocket->errorString();
        return false;
    }
    else{
        return true;
    }
    fSocket->close();
}

QString ConnectionInterfaceClass::getInfoFromSensors()
{
    fSocket = new QTcpSocket(this);
    fSocket->connectToHost("fhlthermorasp1.desy.de" , 50007);
    fSocket->waitForReadyRead();
    QByteArray buffer;
    buffer = fSocket->readAll();
    fSocket->close();
    QString cData = QTextCodec::codecForMib(106)->toUnicode(buffer);

    return cData;
}
