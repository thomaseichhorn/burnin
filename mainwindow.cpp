#include <iostream>

#include <QFile>
#include <QFileDialog>
#include <QThread>
#include <QStandardItemModel>
#include <QTimer>
#include <QAbstractItemModel>
#include <QMessageBox>


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "additionalthread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    fIDTTi1 = 0;
    fIDTTi2 = 0;
    fRowMax = 1;
    fRowClicked = 0;

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
    ui->AddedComands_tabelView->setDragEnabled(true);
    ui->AddedComands_tabelView->setAcceptDrops(true);
    ui->AddedComands_tabelView->setDropIndicatorShown(true);

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
    fIDTTi1 = pArg;
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
    fIDTTi2 = pArg;
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
    AdditionalThread *cThread  = new AdditionalThread("A", fControl);
    QThread *cQThread = new QThread();
    connect(cQThread , SIGNAL(started()), cThread, SLOT(getVAC()));
    connect(cThread, SIGNAL(sendToThread(QString)),this , SLOT(updateGetVAC(QString)));
    cThread->moveToThread(cQThread);
    cQThread->start();

}

void MainWindow::updateGetVAC(QString pStr)
{
    QString cStrV1Set , cStrI1Set , cStrV1App , cStrI1App;

    for(int i = 0 ; i != 9 ; i++){
        cStrV1Set[i] = pStr[i];
    }
    cStrV1Set = deleteSpaces(cStrV1Set);
    cStrV1Set = cStrV1Set.remove(0, 3);
    ui->V1_setted->setText(cStrV1Set);

    for(int i = 10 ; i != 19 ; i++){
        cStrI1Set[i] = pStr[i];
    }
    cStrI1Set = deleteSpaces(cStrI1Set);
    cStrI1Set = cStrI1Set.remove(0 , 3);
    ui->I1_setted->setText(cStrI1Set);

    for(int i = 20 ; i != 29 ; i++){
        cStrV1App[i] = pStr[i];
    }
    cStrV1App = transformQString(cStrV1App);
    cStrV1App = deleteSpaces(cStrV1App);
    ui->V1_App->setText(cStrV1App);

    for(int i = 30 ; i != 39 ; i++){
        cStrI1App[i] = pStr[i];
    }
    cStrI1App = transformQString(cStrI1App);
    cStrI1App = deleteSpaces(cStrI1App);
    ui->I1_App->setText(cStrI1App);

    QString cStrV2Set , cStrI2Set , cStrV2App , cStrI2App;

    for(int i = 40 ; i != 49 ; i++){
        cStrV2Set[i] = pStr[i];
    }
    cStrV2Set = deleteSpaces(cStrV2Set);
    cStrV2Set = cStrV2Set.remove(0 , 3);
    ui->V2_setted->setText(cStrV2Set);

    for(int i = 50 ; i != 59 ; i++){
        cStrI2Set[i] = pStr[i];
    }
    cStrI2Set = deleteSpaces(cStrI2Set);
    cStrI2Set = cStrI2Set.remove(0 , 3);
    ui->I2_setted->setText(cStrI2Set);

    for(int i = 60 ; i != 69 ; i++){
        cStrV2App[i] = pStr[i];
    }
    cStrV2App = transformQString(cStrV2App);
    cStrV2App = deleteSpaces(cStrV2App);
    ui->V2_app->setText(cStrV2App);

    for(int i = 69 ; i != 79 ; i++){
        cStrI2App[i] = pStr[i];
    }
    cStrI2App = transformQString(cStrI2App);
    cStrI2App = deleteSpaces(cStrI2App);
    ui->I2_app->setText(cStrI2App);
}

//creates a List with all commands
void MainWindow::doListOfCommands()
{
    QStandardItemModel *cModel = new QStandardItemModel(this);

    QStandardItem *cItem1 = new QStandardItem("Set Temperature (°C)");
    cModel->setItem( 0 , cItem1);
    cModel->index( 0 , 0);

    QStandardItem *cItem2 = new QStandardItem("On power supply");
    cModel->setItem( 1 , cItem2);
    cModel->index( 1 , 0);

    QStandardItem *cItem3 = new QStandardItem("Off power supply");
    cModel->setItem( 2 , cItem3);
    cModel->index( 2 , 0);

    QStandardItem *cItem4 = new QStandardItem("Wait (Sec)");
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
    if(cStr == "Set Temperature (°C)" || cStr == "Wait (Sec)"){
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
    model->setItem(fRowMax , 0, cItem);
    model->index(fRowMax, 0);
    fRowMax++;
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
        model->setItem(fRowMax , 0, cItem);
        model->index(fRowMax , 0);
        fRowMax++;
        ui->AddedComands_tabelView->setModel(model);
        ui->AddedComands_tabelView->resizeColumnsToContents();
    }
}

void MainWindow::on_Start_pushButton_clicked()
{
    for(vector<SystemControllerClass::fObjParam>::iterator iter = fVec.begin() ; iter != fVec.end(); ++iter){
        cout << (*iter).cName << "   " << (*iter).cValue << endl;
    }
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

void MainWindow::on_AddedComands_tabelView_doubleClicked(const QModelIndex &pIndex)
{
    int cRow = pIndex.row();

    QMessageBox cMsgBox;
    cMsgBox.setText("Are you sure?");
    cMsgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    cMsgBox.setIcon(QMessageBox::Warning);
    cMsgBox.setDefaultButton(QMessageBox::Ok);
    int res = cMsgBox.exec();
    if (res == QMessageBox::Ok) //нажата кнопка Ok
            removeRow(cRow);
}

QString MainWindow::transformQString(QString pStr)
{
    string cStr;
    cStr = pStr.toStdString();
    size_t cPos1 = cStr.find("A");
    size_t cPos2 = cStr.find("V");
    if(cPos1)
        cStr = cStr.substr(0 , cPos1);

    if(cPos2)
        cStr = cStr.substr(0 , cPos2);

    pStr = QString::fromStdString(cStr);
    return pStr;
}

QString MainWindow::deleteSpaces(QString pStr)
{
    string str = pStr.toStdString();
    for (std::string::iterator it = str.begin(); it != str.end(); it++){
        std::string::iterator begin = it;
        while (it != str.end() && ::isspace(*it) )it++;
        if (it - begin > 1)
            it = str.erase(begin + 1, it) - 1;
    }
    return QString::fromStdString(str);
}

void MainWindow::removeRow(int pRow)
{
    ui->AddedComands_tabelView->model()->removeRow(pRow);
    vector<SystemControllerClass::fObjParam>::iterator iter = fVec.begin();
    fVec.erase(iter + pRow - 1);
    cout << pRow << endl;
    fRowMax--;
}

void MainWindow::on_AddedComands_tabelView_clicked(const QModelIndex &pIndex)
{
    fIndex = pIndex;
}

void MainWindow::on_Up_pushButton_clicked()
{
    int cRowIndex = fIndex.row();
    if( cRowIndex > 1 ){
        QString cData = ui->AddedComands_tabelView->model()->data(fIndex).toString();
        QString cTempData = ui->AddedComands_tabelView->model()->data(ui->AddedComands_tabelView->
                                                                      model()->index(cRowIndex-1, 0)).toString();
        QStandardItem *cItem = new QStandardItem(cData);
        QStandardItem *cItemTemp = new QStandardItem(cTempData);
        model->setItem(cRowIndex-1 , cItem);
        model->index(cRowIndex-1 , 0);
        model->setItem(cRowIndex , cItemTemp);
        model->index(cRowIndex , 0);

        SystemControllerClass::fObjParam cObject;
        vector<SystemControllerClass::fObjParam>::iterator cIter = fVec.begin() + cRowIndex -1;
        vector<SystemControllerClass::fObjParam>::iterator cIterTemp = fVec.begin() +cRowIndex -2;

        cObject.cName = (*cIter).cName;
        cObject.cValue = (*cIter).cValue;

        (*cIter).cName = (*cIterTemp).cName;
        (*cIter).cValue = (*cIterTemp).cValue;

        (*cIterTemp).cName = cObject.cName;
        (*cIterTemp).cValue = cObject.cValue;
    }
}

void MainWindow::on_Down_pushButton_clicked()
{
    int cRowIndex = fIndex.row();
    if(cRowIndex != fRowMax - 1){
        QString cData = ui->AddedComands_tabelView->model()->data(fIndex).toString();
        QString cTempData = ui->AddedComands_tabelView->model()->data(ui->AddedComands_tabelView->
                                                                      model()->index(cRowIndex+1, 0)).toString();
        QStandardItem *cItem = new QStandardItem(cData);
        QStandardItem *cItemTemp = new QStandardItem(cTempData);
        model->setItem(cRowIndex+1 , cItem);
        model->index(cRowIndex+1 , 0);
        model->setItem(cRowIndex , cItemTemp);
        model->index(cRowIndex , 0);

        SystemControllerClass::fObjParam cObject;
        vector<SystemControllerClass::fObjParam>::iterator cIter = fVec.begin() + cRowIndex -1;
        vector<SystemControllerClass::fObjParam>::iterator cIterTemp = fVec.begin() +cRowIndex;

        cObject.cName = (*cIter).cName;
        cObject.cValue = (*cIter).cValue;

        (*cIter).cName = (*cIterTemp).cName;
        (*cIter).cValue = (*cIterTemp).cValue;

        (*cIterTemp).cName = cObject.cName;
        (*cIterTemp).cValue = cObject.cValue;
    }
}

void MainWindow::on_Keithley_checkBox_stateChanged(int pArg)
{
    fIDForKeithley = pArg;
    if( pArg){
        fControl->fKeithleyControl->onPower();
        fControl->Wait(0.5);
    }
    else{
        fControl->fKeithleyControl->offPower();
        fControl->Wait(0.5);
    }
}

void MainWindow::on_Keithley_V_set_doubleSpinBox_valueChanged(double pVolt)
{
    fControl->fKeithleyControl->setVolt(pVolt);
}

void MainWindow::on_Keithley_CurrentCompliance_set_doubleSpinBox_valueChanged(double pCurrCompliance)
{
    fControl->fKeithleyControl->fCurrCompliance = pCurrCompliance;
    fControl->Wait(0.5);
}

