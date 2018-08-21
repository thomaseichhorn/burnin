#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QStandardItemModel>

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
    int fRowMax;
    SystemControllerClass *fControl;
    AdditionalWindow *fAddWnd;

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

    void RaspWidget(QString pStr);

    void updateGetVAC(QString pStr);

    void on_AddedComands_tabelView_doubleClicked(const QModelIndex &pIndex);

    void removeRow(int pRow);

    void on_AddedComands_tabelView_clicked(const QModelIndex &pIndex);

    void on_Up_pushButton_clicked();

    void on_Keithley_checkBox_stateChanged(int pVolt);

    void on_Keithley_V_set_doubleSpinBox_valueChanged(double arg1);

    void on_Keithley_CurrentCompliance_set_doubleSpinBox_valueChanged(double pCurrCompliance);

    void on_Down_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    int fRowClicked;
    int fIDTTi1;
    int fIDTTi2;
    int fIDForKeithley;
    vector<SystemControllerClass::fObjParam> fVec;
    void doListOfCommands();
    void getVoltAndCurr();
    void getMeasurments();
    QStandardItemModel *model;
    QModelIndex fIndex;
    QString transformQString(QString pStr);
    QString deleteSpaces(QString pStr);
};

#endif // MAINWINDOW_H
