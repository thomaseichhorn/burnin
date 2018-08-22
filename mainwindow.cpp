#include <iostream>

#include <QFile>
#include <QFileDialog>
#include <QThread>
#include <QStandardItemModel>
#include <QTimer>
#include <QAbstractItemModel>
#include <QMessageBox>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QLCDNumber>
#include <QVBoxLayout>
#include <QLabel>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "additionalthread.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    fRowMax = 1;
    fRowClicked = 0;

    ui->setupUi(this);

    QHBoxLayout *low_layout = new QHBoxLayout;
    gui_pointers_low_voltage_1 = SetVoltageSource(low_layout, "TTI 1", "TTI", 2);
    gui_pointers_low_voltage_2 = SetVoltageSource(low_layout, "TTI 2", "TTI", 2);
    ui->groupBox->setLayout(low_layout);

    QHBoxLayout *high_layout = new QHBoxLayout;
    gui_pointers_high_voltage_1 = SetVoltageSource(high_layout, "Keithley 1", "Keithley2410", 1);
    ui->groupBox_2->setLayout(high_layout);

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
    //connections for low voltage
    connect(gui_pointers_low_voltage_1->i_set ,SIGNAL(valueChanged(double)) , this, SLOT(on_I1_set_doubleSpinBox_valueChanged(double)));
    connect(gui_pointers_low_voltage_1->v_set, SIGNAL(valueChanged(double)), this, SLOT(on_V1_set_doubleSpinBox_valueChanged(double)));
    connect(gui_pointers_low_voltage_1->onoff_button, SIGNAL(stateChanged(int)), this , SLOT(on_OnOff_button1_stateChanged(int)));
    //connections for high voltage
    connect(gui_pointers_high_voltage_1->onoff_button, SIGNAL(stateChanged(int)) , this , SLOT(on_Keithley_OnOff_button_stateChanged(int)));
    connect(gui_pointers_high_voltage_1->i_set , SIGNAL(valueChanged(double)) , this , SLOT(on_Keithley_CurrentCompliance_set_doubleSpinBox_valueChanged(double)));
    connect(gui_pointers_high_voltage_1->v_set , SIGNAL(valueChanged(double)) , this , SLOT(on_Keithley_V_set_doubleSpinBox_valueChanged(double)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

output_pointer_t MainWindow::SetSourceOutputLayout(std::string pType)
{
    // creta pointer list
    output_pointer_t cOutputPointers;

    // create layout
    cOutputPointers.layout = new QVBoxLayout;

    // type of the power source
    QLabel *type = new QLabel(pType.c_str());
    type->setMaximumHeight(20);
    cOutputPointers.layout->addWidget(type);

    // i/v set
    cOutputPointers.i_set = new QDoubleSpinBox();
    cOutputPointers.i_set->setMaximumHeight(20);
    cOutputPointers.layout->addWidget(cOutputPointers.i_set);
    cOutputPointers.v_set = new QDoubleSpinBox();
    cOutputPointers.v_set->setMaximumHeight(20);
    cOutputPointers.layout->addWidget(cOutputPointers.v_set);

    // applied
    cOutputPointers.i_applied = new QLCDNumber();
    cOutputPointers.i_applied->setMaximumHeight(20);
    cOutputPointers.layout->addWidget(cOutputPointers.i_applied);
    cOutputPointers.v_applied = new QLCDNumber();
    cOutputPointers.v_applied->setMaximumHeight(20);
    cOutputPointers.layout->addWidget(cOutputPointers.v_applied);

    // on off
    cOutputPointers.onoff_button = new QCheckBox("On/Off");
    cOutputPointers.onoff_button->setMaximumHeight(20);
    cOutputPointers.layout->addWidget(cOutputPointers.onoff_button);

    // return
    return cOutputPointers;
}

output_pointer_t* MainWindow::SetVoltageSource(QLayout *pMainLayout, std::string pName, std::string pType, int pNoutputs)
{
    // create the output pointers
    output_pointer_t* cOutputPointers = new output_pointer_t[pNoutputs];

    // horizontal layout
    QGroupBox *group_box = new QGroupBox(pName.c_str());
    QHBoxLayout *group_box_layout = new QHBoxLayout;

    QSize size(80,20);

    // set the labels
    QVBoxLayout *label_layout = new QVBoxLayout;
    QLabel *label_type = new QLabel("Type:");
    label_type->setMinimumSize(size);
    label_layout->addWidget(label_type);
    QLabel *label_i_set = new QLabel("I(set), A:");
    label_i_set->setMinimumSize(size);
    label_layout->addWidget(label_i_set);
    QLabel *label_v_set = new QLabel("V(set), V:");
    label_v_set->setMinimumSize(size);
    label_layout->addWidget(label_v_set);
    QLabel *label_i_applied = new QLabel("I(applied), A:");
    label_i_applied->setMinimumSize(size);
    label_layout->addWidget(label_i_applied);
    QLabel *label_v_applied = new QLabel("V(applied), V:");
    label_v_applied->setMinimumSize(size);
    label_layout->addWidget(label_v_applied);
    QLabel *label_onoff = new QLabel("On/Off:");
    label_onoff->setMinimumSize(size);
    label_layout->addWidget(label_onoff);
    // now set
    group_box_layout->addItem(label_layout);

    // set the outputs
    for (int i = 0; i < pNoutputs; i++) {
        cOutputPointers[i] = SetSourceOutputLayout(pType);
        group_box_layout->addItem(cOutputPointers[i].layout);
    }

    // set the group box
    group_box->setLayout(group_box_layout);

    // finally add to the main layout
    pMainLayout->addWidget(group_box);

    // return everything
    return cOutputPointers;
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

void MainWindow::getVoltAndCurr()
{
    AdditionalThread *cThread  = new AdditionalThread("A", fControl);
    QThread *cQThread = new QThread();
    connect(cQThread , SIGNAL(started()), cThread, SLOT(getVAC()));
    connect(cThread, SIGNAL(sendToThread(PowerControlClass::fVACvalues*)),this,
            SLOT(updateGetVAC(PowerControlClass::fVACvalues*)));
    cThread->moveToThread(cQThread);
    cQThread->start();

}

//func which adds command to the list and vector from listOfCommands to AddedWidget
void MainWindow::on_listOfCommands_doubleClicked(const QModelIndex &pIndex)
{
    SystemControllerClass::fObjParam cObj;
    QString cValue;
    QString cStr = pIndex.data().toString();

    fAddWnd = new AdditionalWindow(this);
    if(cStr == "Set Temperature (°C)" || cStr == "Wait (Sec)"){
        fAddWnd->show();
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
    connect(cThread, SIGNAL(sendToThreadString(QString)),this , SLOT(RaspWidget(QString)));
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
    if (res == QMessageBox::Ok)
            removeRow(cRow);
}

void MainWindow::removeRow(int pRow)
{
    ui->AddedComands_tabelView->model()->removeRow(pRow);
    vector<SystemControllerClass::fObjParam>::iterator iter = fVec.begin();
    fVec.erase(iter + pRow - 1);
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
    if(cRowIndex != fRowMax - 1){//нажата кнопка Ok
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

void MainWindow::on_OnOff_button1_stateChanged(int pArg)
{
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
void MainWindow::on_OnOff_button2_stateChanged(int pArg)
{
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
void MainWindow::on_V1_set_doubleSpinBox_valueChanged(double pVolt)
{
    fControl->fTTiVolt->setVolt(pVolt , 1);
    QThread::sleep(0.5);
}

void MainWindow::on_I1_set_doubleSpinBox_valueChanged(double pCurr)
{
    fControl->fTTiVolt->setCurr(pCurr, 1);
    QThread::sleep(0.5);
}

void MainWindow::on_V2_set_doubleSpinBox_valueChanged(double pVolt)
{
    fControl->fTTiVolt->setVolt(pVolt , 2);
    fControl->Wait(0.5);
}

void MainWindow::on_I2_set_doubleSpinBox_valueChanged(double pCurr)
{
    fControl->fTTiVolt->setCurr(pCurr , 2);
    fControl->Wait(0.5);
}

void MainWindow::updateGetVAC(PowerControlClass::fVACvalues* pObject)
{
    gui_pointers_low_voltage_1->i_set->setValue(pObject->pISet1);
    gui_pointers_low_voltage_1->v_set->setValue(pObject->pVSet1);
    gui_pointers_low_voltage_1->i_applied->display(pObject->pIApp1);
    gui_pointers_low_voltage_1->v_applied->display(pObject->pVApp1);

}

void MainWindow::on_Keithley_OnOff_button_stateChanged(int pArg)
{
//    if( pArg){
//        fControl->fKeithleyControl->onPower(0);
//        fControl->Wait(0.5);
//    }
//    else{
//        fControl->fKeithleyControl->offPower(0);
//        fControl->Wait(0.5);
//    }
}

void MainWindow::on_Keithley_V_set_doubleSpinBox_valueChanged(double pVolt)
{
//    fControl->fKeithleyControl->fVoltSet = pVolt;
//    fControl->fKeithleyControl->setVolt(pVolt , 0);
}

void MainWindow::on_Keithley_CurrentCompliance_set_doubleSpinBox_valueChanged(double pCurrCompliance)
{
//    fControl->fKeithleyControl->fCurrCompliance = pCurrCompliance;
//    fControl->Wait(0.5);
}


void MainWindow::on_Keithley_Step_spinbox_valueChanged(int pStep)
{
//    fControl->fKeithleyControl->fStep = pStep;
//    fControl->Wait(0.5);
}
