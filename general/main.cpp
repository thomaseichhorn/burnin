#include "gui/mainwindow.h"
#include "systemcontrollerclass.h"
#include <QApplication>

#include <vector>
#include <iostream>
#include <locale>

using namespace std;

int main(int argc, char *argv[])
{
    //replaces commas with dots in printf
    setlocale(LC_ALL,"");
    setlocale(LC_NUMERIC,"");
    qRegisterMetaType<QMap<QString, QString>>("QMap<QString, QString>");

    QApplication a(argc, argv);

    MainWindow w;

    w.show();

    return a.exec();
}
