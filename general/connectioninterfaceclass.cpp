#include <iostream>

#include <QThread>
#include <QString>
#include <QTextCodec>

#include "general/connectioninterfaceclass.h"
#include "gui/mainwindow.h"

using namespace std;

ConnectionInterfaceClass::ConnectionInterfaceClass(string pAddress , string pPort)
{
    fAddress = pAddress;
    fPort = QString::fromStdString(pPort).toInt();
}

bool ConnectionInterfaceClass::initialize()
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
//    fSocket->close();
}

QString ConnectionInterfaceClass::getInfoFromSensors()
{
//    fSocket = new QTcpSocket(this);
//    fSocket->connectToHost(QString::fromStdString(fAddress) , fPort);
    fSocket->waitForReadyRead();
    QByteArray buffer;
    buffer = fSocket->readAll();
//    fSocket->close();
//    delete fSocket;
    QString cData = QTextCodec::codecForMib(106)->toUnicode(buffer);

    return cData;
}

void ConnectionInterfaceClass::closeConnection()
{
    if(fSocket->isOpen()){
        fSocket->close();
        delete fSocket;
    }
}
