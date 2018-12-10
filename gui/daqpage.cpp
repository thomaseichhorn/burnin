#include "daqpage.h"

#include <QPushButton>

DAQPage::DAQPage(QWidget* daqPageWidget)
{
    _daqPageWidget = daqPageWidget;
    _module = nullptr;
    
    QPushButton* loadfirmware_button = _daqPageWidget->findChild<QPushButton*>("loadfirmware_button");
    connect(loadfirmware_button, SIGNAL(clicked()), this, SLOT(onLoadfirmwareClicked()));
    
    QPushButton* systemtest_button = _daqPageWidget->findChild<QPushButton*>("systemtest_button");
    connect(systemtest_button, SIGNAL(clicked()), this, SLOT(onSystemtestClicked()));
    
    QPushButton* calibrate_button = _daqPageWidget->findChild<QPushButton*>("calibrate_button");
    connect(calibrate_button, SIGNAL(clicked()), this, SLOT(onCalibrateClicked()));
    
    QPushButton* datatest_button = _daqPageWidget->findChild<QPushButton*>("datatest_button");
    connect(datatest_button, SIGNAL(clicked()), this, SLOT(onDatatestClicked()));
    
    QPushButton* hybridtest_button = _daqPageWidget->findChild<QPushButton*>("hybridtest_button");
    connect(hybridtest_button, SIGNAL(clicked()), this, SLOT(onHybridtestClicked()));
    
    QPushButton* cmtest_button = _daqPageWidget->findChild<QPushButton*>("cmtest_button");
    connect(cmtest_button, SIGNAL(clicked()), this, SLOT(onCmtestClicked()));
    
    QPushButton* commission_button = _daqPageWidget->findChild<QPushButton*>("commission_button");
    connect(commission_button, SIGNAL(clicked()), this, SLOT(onCommissionClicked()));
}

void DAQPage::setDAQModule(DAQModule* module) {
    _module = module;
}

void DAQPage::onLoadfirmwareClicked() {
    _module->loadFirmware();
}

void DAQPage::onSystemtestClicked() {
    _module->runSystemTest();
}

void DAQPage::onCalibrateClicked() {
    _module->runCalibrate();
}

void DAQPage::onDatatestClicked() {
    _module->runDatatest();
}

void DAQPage::onHybridtestClicked() {
    _module->runHybridtest();
}

void DAQPage::onCmtestClicked() {
    _module->runCmtest();
}

void DAQPage::onCommissionClicked() {
    _module->runCommission();
}


