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
    SystemControllerClass *fControl;
    AdditionalWindow *fAddWnd;
    void getVoltAndCurr1();
    void getVoltAndCurr2();

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

    void updateGetVAC1(QString pStr);

    void updateGetVAC2(QString pStr);

    void on_AddedComands_tabelView_doubleClicked(const QModelIndex &index);


private:
    int fIDMW1;
    int fIDMW2;
    vector<SystemControllerClass::fObjParam> fVec;
    Ui::MainWindow *ui;
    void doListOfCommands();
    void statusWidget();
    void getMeasurments();
    QStandardItemModel *model;
    QString transformQStringSet(QString pStr);
    QString transformQStringApp(QString pStr);
    QString deleteSpaces(QString pStr);



};

#endif // MAINWINDOW_H
