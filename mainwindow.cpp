#include <QFile>
#include <QFileDialog>
#include <QThread>
#include <QStandardItemModel>
#include <QTimer>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "additionalthread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    fIDMW1 = 0;
    fIDMW2 = 0;

    fControl = new SystemControllerClass();
    fControl->Initialize();
    ui->setupUi(this);
    this->doListOfCommands();
    this->getVoltAndCurr();
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

void MainWindow::getVoltAndCurr()
{
    AdditionalThread *cThread = cThread = new AdditionalThread("A", fControl);
    QThread *cQThread = cQThread = new QThread();
    connect(cQThread , SIGNAL(started()), cThread, SLOT(getVAC1()));
    connect(cThread, SIGNAL(sendToThread(QString)),this , SLOT(updateGetVAC(QString)));
    cThread->moveToThread(cQThread);
    cQThread->start();

}

void MainWindow::updateGetVAC(QString cStrVAC1)
{
    ui->ID1_VoltAndCurr_label->setText(cStrVAC1);
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
    QListWidgetItem *cItem = new QListWidgetItem(cStr);
    ui->AddedListWidget->addItem(cItem);
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

        QListWidgetItem *cItem = new QListWidgetItem(cStr);
        ui->AddedListWidget->addItem(cItem);
    }
}

//removes item from widget and vector(fVec) when doule-clicked
void MainWindow::on_AddedListWidget_itemDoubleClicked(QListWidgetItem *pItem)
{
    int cRow = ui->AddedListWidget->row(pItem);
    delete ui->AddedListWidget->takeItem(ui->AddedListWidget->row(pItem));
    vector<SystemControllerClass::fObjParam>::iterator iter = fVec.begin();
    fVec.erase(iter + cRow);
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
    AdditionalThread *cThread = cThread = new AdditionalThread("A" , fControl);
    QThread *cQThread = cQThread = new QThread();
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
