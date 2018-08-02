#include "connectioninterfaceclass.h"

#include <QThread>
#include <QString>

#include <iostream>

using namespace std;

ConnectionInterfaceClass::ConnectionInterfaceClass(QObject *parent) : QObject(parent)
{
}

bool ConnectionInterfaceClass::doConnect(){
    //connects to raspberry and returns true or false
//    socket = new QTcpSocket(this);
//    socket->connectToHost("localhost" , 50007);
//    if(!socket->waitForDisconnected()){
//        qDebug() << "Error:" << socket->errorString();
//    }
//    while(1){
//        QString str;
//        int nbytes = 256;
//        char array[256];
//        socket->read(array , nbytes);
//        for(int i = 0 ; i != nbytes ; i++){
//            str[i] = array[i];
//        }
//        str.fromUtf8(array, 256);
//        cout << str.toStdString() << endl;
//        QThread::sleep(5);
//    }
}
double ConnectionInterfaceClass::getMeasurement(){
    //returns information from sensors on raspberry
}
