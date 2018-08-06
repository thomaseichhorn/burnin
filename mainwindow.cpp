#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "environmentcontrolclass.h"
#include "powercontrolclass.h"
#include "controlttipower.h"

#include <QThread>
#include <QStandardItemModel>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    fIDMW1 = 0;
    fIDMW2 = 0;
    fControl = new SystemControllerClass();
    ui->setupUi(this);
    this->doListOfCommands();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Status1_On_checkBox_stateChanged(int pState)
{
    fIDMW1 = pState;
    if( pState){
        fControl->fTTiVolt->onPower(1);
        fControl->Wait(1);
    }
    else{
        fControl->fTTiVolt->offPower(1);
        fControl->Wait(1);
    }
}

void MainWindow::on_Status2_On_checkBox_3_stateChanged(int pState)
{
    fIDMW2 = pState;
    if( pState ){
        fControl->fTTiVolt->onPower(2);
        fControl->Wait(1);
    }
    else{
        fControl->fTTiVolt->offPower(2);
        fControl->Wait(1);
    }
}

void MainWindow::on_V1_set_doubleSpinBox_valueChanged(double pVolt)
{
    fControl->fTTiVolt->setVolt(1 , pVolt);
    QThread::sleep(1);
}

void MainWindow::on_I1_set_doubleSpinBox_valueChanged(double pCurr)
{
    fControl->fTTiVolt->setCurr(1 , pCurr);
    QThread::sleep(1);
}

void MainWindow::on_V2_set_doubleSpinBox_valueChanged(double pVolt)
{
    fControl->fTTiVolt->setVolt(2 , pVolt);
    fControl->Wait(1);
}

void MainWindow::on_I2_set_doubleSpinBox_valueChanged(double pCurr)
{
    fControl->fTTiVolt->setCurr(2 , pCurr);
    fControl->Wait(1);
}

void MainWindow::getVoltAndCurr()
{
    QString cStrVSet1 , cStrVApp1 , cStrISet1 , cStrIApp1;

    cStrVSet1 = fControl->fTTiVolt->getVoltSet(1);
    cStrISet1 = fControl->fTTiVolt->getCurrSet(1);
    cStrVApp1 = fControl->fTTiVolt->getVoltApp(1);
    cStrIApp1 = fControl->fTTiVolt->getCurrApp(1);

    ui->V1_setted_label->setText(cStrVSet1);
    ui->V1_App_label->setText(cStrVApp1);
    ui->I1_setted_label->setText(cStrISet1);
    ui->I1_App_label->setText(cStrIApp1);

    QString cStrVSet2 , cStrVApp2 , cStrISet2 , cStrIApp2;

    cStrVSet2 = fControl->fTTiVolt->getVoltSet(2);
    cStrISet2 = fControl->fTTiVolt->getCurrSet(2);
    cStrVApp2  = fControl->fTTiVolt->getVoltApp(2);
    cStrIApp2 = fControl->fTTiVolt->getCurrApp(2);

    ui->V2_setted_label->setText(cStrVSet2);
    ui->V2_App_label->setText(cStrVApp2);
    ui->I2_settted_label->setText(cStrISet2);
    ui->I2_App_label->setText(cStrIApp2);


}

void MainWindow::doListOfCommands()
{
    QStandardItemModel *cModel = new QStandardItemModel(this);

    QStandardItem *cItem1 = new QStandardItem("Set Temperature (in C)");
    cModel->setItem( 0 , cItem1);
    cModel->index( 0 , 0);

    QStandardItem *cItem2 = new QStandardItem("On power supply");
    cModel->setItem( 1 , cItem2);
    cModel->index( 1 , 0);

    QStandardItem *cItem3 = new QStandardItem("Off power supply");
    cModel->setItem( 2 , cItem3);
    cModel->index( 2 , 0);

    QStandardItem *cItem4 = new QStandardItem("Wait (in sec)");
    cModel->setItem( 3 , cItem4);
    cModel->index( 3 , 0);


    ui->listOfCommands->setModel(cModel);
    ui->listOfCommands->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

