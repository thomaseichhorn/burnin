#include <iostream>

#include <QFile>
#include <QFileDialog>
#include <QThread>
#include <QStandardItemModel>
#include <QTimer>
#include <QAbstractItemModel>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "additionalthread.h"

int i = 1;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    fIDMW1 = 0;
    fIDMW2 = 0;

    ui->setupUi(this);

    fControl = new SystemControllerClass();
    fControl->Initialize();
    model = new QStandardItemModel(this);

    QStandardItem *cItem1 = new QStandardItem("Added commands:");
    model->setItem( 0 , 0 , cItem1);
    model->index(0 , 0);
    QStandardItem *cItem2 = new QStandardItem("Start time:");
    model->setItem( 0 , 1 , cItem2);
    model->index(0 , 1);
    QStandardItem *cItem3 = new QStandardItem("End time:");
    model->setItem( 0 , 2 , cItem3);
    model->index(0 , 2);
    QStandardItem *cItem4 = new QStandardItem("Status:");
    model->setItem( 0 , 3 , cItem4);
    model->index(0 , 3);

    ui->AddedComands_tabelView->setModel(model);
    ui->AddedComands_tabelView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->AddedComands_tabelView->horizontalHeader()->setStretchLastSection(true);
    ui->AddedComands_tabelView->resizeColumnsToContents();
    ui->AddedComands_tabelView->verticalHeader()->setVisible(false);
    ui->AddedComands_tabelView->horizontalHeader()->setVisible(false);

    this->doListOfCommands();
    this->getVoltAndCurr1();
    this->getVoltAndCurr2();
    this->getMeasurments();
}

MainWindow::~MainWindow()
{
    delete ui;
}


//Turn on the first part of TTi
void MainWindow::on_Status1_On_checkBox_stateChanged(int pArg)
{
    fIDMW1 = pArg;
    if( pArg){
        fControl->fTTiVolt->onPower(1);
        fControl->Wait(1);
    }
    else{
        fControl->fTTiVolt->offPower(1);
        fControl->Wait(1);
    }
}

//Turns on the second part of the TTi
void MainWindow::on_Status2_On_checkBox_3_stateChanged(int pArg)
{
    fIDMW2 = pArg;
    if( pArg ){
        fControl->fTTiVolt->onPower(2);
        fControl->Wait(1);
    }
    else{
        fControl->fTTiVolt->offPower(2);
        fControl->Wait(1);
    }
}

//Set func
void MainWindow::on_V1_set_doubleSpinBox_valueChanged(double pArg)
{
    fControl->fTTiVolt->setVolt(1 , pArg);
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

void MainWindow::getVoltAndCurr1()
{
    AdditionalThread *cThread  = new AdditionalThread("A", fControl);
    QThread *cQThread = new QThread();
    connect(cQThread , SIGNAL(started()), cThread, SLOT(getVAC1()));
    connect(cThread, SIGNAL(send(vector<QString>)),this , SLOT(updateGetVAC1(vector<QString>)));
    cThread->moveToThread(cQThread);
    cQThread->start();

}

void MainWindow::getVoltAndCurr2()
{
    AdditionalThread *cThread = new AdditionalThread("C", fControl);
    QThread *cQThread = new QThread();
    connect(cQThread , SIGNAL(started()), cThread, SLOT(getVAC2()));
    connect(cThread, SIGNAL(sendToThread2(QString)),this , SLOT(updateGetVAC2(QString)));
    cThread->moveToThread(cQThread);
    cQThread->start();
}

void MainWindow::updateGetVAC1(vector<QString> pVec)
{
    ui->ID1_VoltAndCurr_label->setText(pVec[1]);
}

void MainWindow::updateGetVAC2(QString pStr)
{
    ui->ID2_VoltAndCurr_label->setText(pStr);
}


//creates a List with all commands
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

//func which adds command to the list and vector from listOfCommands to AddedWidget
void MainWindow::on_listOfCommands_doubleClicked(const QModelIndex &pIndex)
{
    SystemControllerClass::fObjParam cObj;
    QString cValue;

    QString cStr = pIndex.data().toString();
    fAddWnd = new AdditionalWindow(this);
    if(cStr == "Set Temperature (in C)" || cStr == "Wait (in sec)"){
        fAddWnd->show();cout << endl;
        fAddWnd->exec();
        cValue = fAddWnd->getValue();
        cObj.cName = cStr.toStdString();
        cObj.cValue = cValue.toDouble();
        fVec.push_back(cObj);
        cStr = cStr + " =" + cValue;
    }
    else{
        cObj.cName = cStr.toStdString();
        cObj.cValue = 0;
        fVec.push_back(cObj);
    }
    QStandardItem *cItem = new QStandardItem(cStr);
    model->setItem(i , 0, cItem);
    model->index(i, 0);
    i++;
    ui->AddedComands_tabelView->resizeColumnsToContents();
}

//reads file and writes it to AddWidget, QString into name field of fParam and string after = to value field
void MainWindow::on_readConfig_push_button_clicked()
{
    SystemControllerClass::fObjParam cObj;
    QString cStr;
    vector<QString> *cVec = new vector<QString>();
    cVec = fControl->readFile();
    for(vector<QString>::iterator iter = cVec->begin(); iter != cVec->end(); ++iter){
        cStr = (*iter);
        string cString = cStr.toStdString();
        size_t cPos = cString.find("=");
        if( cPos ){
            string cPosStr = cString.substr(cPos+1);
            QString cStrTemp = QString::fromStdString(cPosStr);
            cString = cString.substr(0 , cPos);
            cObj.cName = cString;
            cObj.cValue = cStrTemp.toDouble();
            fVec.push_back(cObj);
        }
        else{
            cObj.cName = cStr.toStdString();
            cObj.cValue = 0;
            fVec.push_back(cObj);
        }
    }
    for(vector<QString>::iterator iter = cVec->begin(); iter != cVec->end(); ++iter){
        cStr = (*iter);
        QStandardItem *cItem = new QStandardItem(cStr);
        model->setItem(i , 0, cItem);
        model->index(i , 0);
        i++;
        ui->AddedComands_tabelView->setModel(model);
        ui->AddedComands_tabelView->resizeColumnsToContents();
    }
}


void MainWindow::statusWidget()
{

}


void MainWindow::on_Start_pushButton_clicked()
{
    //reads data from fVec and calls needed objects
}

void MainWindow::getMeasurments()
{
    AdditionalThread *cThread = new AdditionalThread("B" , fControl);
    QThread *cQThread = new QThread();
    connect(cQThread , SIGNAL(started()), cThread, SLOT(getRaspSensors()));
    connect(cThread, SIGNAL(sendToThread(QString)),this , SLOT(RaspWidget(QString)));
    cThread->moveToThread(cQThread);
    cQThread->start();
}

//reads sensor on rasp and sets info to Raspberry sensors
void MainWindow::RaspWidget(QString pStr)
{
    ui->raspberrySensors_textBrowser->setText(pStr);
}

void MainWindow::on_AddedComands_tabelView_doubleClicked(const QModelIndex &index)
{
    int cRow = index.row();
    ui->AddedComands_tabelView->model()->removeRow(cRow);
    vector<SystemControllerClass::fObjParam>::iterator iter = fVec.begin();
    fVec.erase(iter + cRow);
    i--;

}
