#include "mainwindow.h"
#include <QApplication>
#include "systemcontrollerclass.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    SystemControllerClass at;
    at.Initialize();


    return a.exec();
}
