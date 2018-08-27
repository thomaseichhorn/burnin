#include <iostream>

#include <QThread>
#include <QString>
#include <QTextCodec>

#include "connectioninterfaceclass.h"
#include "GUI/mainwindow.h"

using namespace std;

ConnectionInterfaceClass::ConnectionInterfaceClass(string pAddress , string pPort)
{
    fAddress = pAddress;
    fPort = QString::fromStdString(pPort).toInt();
}

bool ConnectionInterfaceClass::raspInitialize()
{
    fSocket = new QTcpSocket(this);
    fSocket->connectToHost(QString::fromStdString(fAddress) , fPort);
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
    fSocket->connectToHost(QString::fromStdString(fAddress) , fPort);
    fSocket->waitForReadyRead();
    QByteArray buffer;
    buffer = fSocket->readAll();
    fSocket->close();
    QString cData = QTextCodec::codecForMib(106)->toUnicode(buffer);

    return cData;
}
