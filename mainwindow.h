#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>

#include "systemcontrollerclass.h"
#include "environmentcontrolclass.h"
#include "powercontrolclass.h"
#include "controlttipower.h"
#include "additionalwindow.h"
#include "additionalthread.h"

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
    AdditionalWindow *fAddWnd;
    void getVoltAndCurr();

private slots:

    void on_V1_set_doubleSpinBox_valueChanged(double pArg);

    void on_Status1_On_checkBox_stateChanged(int pArg);

    void on_Status2_On_checkBox_3_stateChanged(int pArg);

    void on_I1_set_doubleSpinBox_valueChanged(double pCurr);

    void on_V2_set_doubleSpinBox_valueChanged(double pVolt);

    void on_I2_set_doubleSpinBox_valueChanged(double pCurr);

    void on_listOfCommands_doubleClicked(const QModelIndex &pIndex);

    void on_readConfig_push_button_clicked();

    void on_Start_pushButton_clicked();

    void on_AddedListWidget_itemDoubleClicked(QListWidgetItem *pItem);

    void RaspWidget(QString);

    void updateGetVAC(QString);

private:
    int fIDMW1;
    int fIDMW2;
    vector<SystemControllerClass::fObjParam> fVec;
    Ui::MainWindow *ui;
    void doListOfCommands();
    void statusWidget();
    void getMeasurments();



};

#endif // MAINWINDOW_H
