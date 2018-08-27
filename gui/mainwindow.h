#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMainWindow>
#include "QPushButton"
#include "QDoubleSpinBox"
#include "QLCDNumber"
#include "QCheckBox"
#include <QListWidgetItem>
#include <QStandardItemModel>

#include "systemcontrollerclass.h"
#include "environmentcontrolclass.h"
#include "VoltageControl/powercontrolclass.h"
#include "additionalwindow.h"
#include "additionalthread.h"

namespace Ui {
class MainWindow;
class SystemControllerClass;
}

struct output_pointer_t {
    QLayout *layout;
    QDoubleSpinBox *i_set;
    QDoubleSpinBox *v_set;
    QLCDNumber *i_applied;
    QLCDNumber *v_applied;
    QCheckBox *onoff_button;
};

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

    void on_listOfCommands_doubleClicked(const QModelIndex &pIndex);

    void on_readConfig_push_button_clicked();

    void on_Start_pushButton_clicked();

    void RaspWidget(QString pStr);

    void updateGetVAC(PowerControlClass::fVACvalues *pObject);

    void updateGetVACKeithley(PowerControlClass::fVACvalues *pObject);

    void on_AddedComands_tabelView_doubleClicked(const QModelIndex &pIndex);

    void removeRow(int pRow);

    void on_AddedComands_tabelView_clicked(const QModelIndex &pIndex);

    void on_Up_pushButton_clicked();

    void on_Down_pushButton_clicked();

    void initHard();

    void readXmlFile();


private:
    Ui::MainWindow *ui;
    int fRowClicked;
    QStandardItemModel *model;
    QModelIndex fIndex;
    vector<string> fSources;
    output_pointer_t* gui_pointers_low_voltage_1;
    output_pointer_t* gui_pointers_low_voltage_2;
    output_pointer_t* gui_pointers_high_voltage_1;

    void doListOfCommands();
    void getVoltAndCurr();
    void getVoltAndCurrKeithley();
    void getMeasurments();
    QString transformQString(QString pStr);
    QString deleteSpaces(QString pStr);
    output_pointer_t SetSourceOutputLayout(std::string pType);
    output_pointer_t *SetVoltageSource(QLayout *pMainLayout, std::string pName, std::string pType,
                                       int pNoutputs);

    void on_V_set_doubleSpinBox_valueChanged(string pSourceName , int pId , double pVolt);

    void on_OnOff_button_stateChanged(string pSourceName , int pId , bool pArg);

    void on_I_set_doubleSpinBox_valueChanged(string pSourceName , int pId, double pCurr);
};

#endif // MAINWINDOW_H
