#include "thermorasp.h"

#include <iostream>
#include <QTextCodec>
#include <QThread>
#include <QRegularExpression>

using namespace std;

Thermorasp::Thermorasp(const QString& address, quint16 port) {
    _address = address;
    _port = port;
}

Thermorasp::Thermorasp(const string& address, quint16 port) {
    _address = QString::fromStdString(address);
    _port = port;
}

QMap<QString, QString> Thermorasp::_parseReplyForReadings(QByteArray buffer) {
    QMap<QString, QString> ret;
    QString data = QTextCodec::codecForMib(106)->toUnicode(buffer);
    int nlpos = data.indexOf('\n');
    if (nlpos < 1)
        return ret;
    QString nameline = data.mid(1, nlpos - 1);
    QString readingsline = data.right(data.length() - (nlpos + 1));
    QStringList names = nameline.split(' ');
    
    QString pattern("^");
    for (const QString& name: names) {
        if (pattern != "^")
            pattern += " ";
        
        if (name == "date")
            pattern += "(\\d{4}-\\d{2}-\\d{2})";
        else if (name == "time")
            pattern += "(\\d{2}:\\d{2}:\\d{2}\\.\\d+)";
        else
            //pattern += "(\\d+\\.?\\d*?)?"; // expect float
            pattern += "([^ ]+)?"; // except anything
    }
    pattern += "$";
    QRegularExpression re(pattern);
    QRegularExpressionMatch match = re.match(readingsline);
    
    if (not match.hasMatch()) {
        cerr << "Raspberry line has invalid format: " << readingsline.toStdString() << endl;
        return ret;
    }
    
    for (int i = 0; i < names.length(); ++i)
        ret[names[i]] = match.captured(i + 1);
        
    return ret;
}

QMap<QString, QString> Thermorasp::getReadings(int timeout) {
    QTcpSocket* sock = new QTcpSocket(nullptr);
    QMap<QString, QString> ret;
    sock->connectToHost(_address, _port);
    if (not sock->waitForDisconnected(timeout))
        return ret;
    sock->waitForReadyRead(timeout);
    
    QByteArray buffer = sock->readAll();
    ret = _parseReplyForReadings(buffer);
    
    sock->close();
    delete sock;
    
    return ret;
}
