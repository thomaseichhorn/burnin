#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "systemcontrollerclass.h"
#include "environmentcontrolclass.h"
#include "powercontrolclass.h"
#include "controlttipower.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
class SystemControllerClass;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    SystemControllerClass *fControl;
    void getVoltAndCurr();

private slots:

    void on_V1_set_doubleSpinBox_valueChanged(double arg1);

    void on_Status1_On_checkBox_stateChanged(int arg1);

    void on_Status2_On_checkBox_3_stateChanged(int arg1);


    void on_I1_set_doubleSpinBox_valueChanged(double pCurr);

    void on_V2_set_doubleSpinBox_valueChanged(double pVolt);

    void on_I2_set_doubleSpinBox_valueChanged(double pCurr);

private:
    int fIDMW1;
    int fIDMW2;
    ControlTTiPower *fTTI;
    Ui::MainWindow *ui;
    void doListOfCommands();

};

#endif // MAINWINDOW_H
