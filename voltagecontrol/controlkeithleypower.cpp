
#include <stdio.h>
#include <sstream>
#include <locale>

#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>
#include <fstream>
#include <cmath>

#include <QDebug>
#include <QThread>

#include "controlkeithleypower.h"
#include "additional/additionalthread.h"
#include "general/systemcontrollerclass.h"

using namespace std;

const int SWEEP_INTERVAL = 500; //ms
const double SWEEP_STEP = 10; //V

KeithleyPowerSweepWorker::KeithleyPowerSweepWorker(ControlKeithleyPower* keithley):
    _keithley(keithley),
    _timer(this)
{
    connect(&_timer, SIGNAL(timeout()), this, SLOT(doSweeping()));
    _timer.start(SWEEP_INTERVAL);
    
    _voltTarget = _keithley->fVoltSet;
    _voltApplied = _keithley->fVolt;
    _outputState = _keithley->keithleyOutputOn;
    if (_outputState)
	cout << "KeithleyPowerSweepWorker constructed with turned on Keithley" << endl;
}

void KeithleyPowerSweepWorker::doSweeping() {
    if (not _outputState or _voltTarget == _voltApplied) {
	_timer.start(SWEEP_INTERVAL);
	return;
    }
    
    if (abs(_voltTarget - _voltApplied) < SWEEP_STEP)
	_voltApplied = _voltTarget;
    else
	_voltApplied += copysign(SWEEP_STEP, _voltTarget - _voltApplied);
    
    _keithley->sendVoltageCommand(_voltApplied);
    if (_voltApplied == _voltTarget)
	emit targetReached(_voltTarget);
    
    _timer.start(SWEEP_INTERVAL);
}

void KeithleyPowerSweepWorker::doVoltSet(double volts) {
    _voltTarget = volts;
}

void KeithleyPowerSweepWorker::doVoltApp(double volts) {
    cout << "doVoltApp(" << volts << ")" << endl;
    _voltApplied = volts;
}

void KeithleyPowerSweepWorker::doOutputState(bool state) {
    _outputState = state;
}

ControlKeithleyPower::ControlKeithleyPower(string pConnection, double pSetVolt, double pSetCurr)
{
    fConnection = pConnection;
    fVoltSet = pSetVolt;
    fCurrCompliance = pSetCurr;
//    set the number of steps
    fStep = 10;
    _turnOffScheduled = false;
    
    KeithleyPowerSweepWorker* worker = new KeithleyPowerSweepWorker(this);
    worker->moveToThread(&_sweepThread);
    connect(&_sweepThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &ControlKeithleyPower::voltSetChanged, worker, &KeithleyPowerSweepWorker::doVoltSet);
    connect(this, &ControlKeithleyPower::voltAppChanged, worker, &KeithleyPowerSweepWorker::doVoltApp);
    connect(this, &ControlKeithleyPower::outputStateChanged, worker, &KeithleyPowerSweepWorker::doOutputState);
    connect(worker, &KeithleyPowerSweepWorker::targetReached, this, &ControlKeithleyPower::onTargetVoltageReached);
    _sweepThread.start();
}

void ControlKeithleyPower::initialize(){

    const ioport_t ioPort = fConnection.c_str();
    speed_t keithleybaud = B19200;
    comHandler_ = new ComHandler( ioPort, keithleybaud );
    std::cout << "Created ComHandler on port " << ioPort << " at " << comHandler_ << std::endl;
    setKeithleyOutputState(0);
    setCurr(fCurrCompliance);
}


void ControlKeithleyPower::onPower(int)
{
    _turnOffScheduled = false;
    setKeithleyOutputState ( 1 );
}

void ControlKeithleyPower::offPower(int)
{
    _turnOffScheduled = true;
    setVolt(0);
}

void ControlKeithleyPower::setVolt(double pVoltage , int)
{
    fVoltSet = pVoltage;
    emit voltSetChanged(fVoltSet);
}

void ControlKeithleyPower::sendVoltageCommand(double pVoltage) {
    char buf[512];
    cout << "sendVoltageCommand(" << pVoltage << ")" << endl;
    sprintf(buf ,":SOUR:VOLT:LEV %G\r\n", pVoltage);
    comHandler_->SendCommand(buf);
    QThread::msleep(100);
}

void ControlKeithleyPower::onTargetVoltageReached(double voltage) {
    if (_turnOffScheduled and voltage == 0)
	setKeithleyOutputState(0);
}

void ControlKeithleyPower::setCurr(double pCurrent, int)
{
    char stringinput[512];

    sprintf(stringinput ,":SENS:CURR:PROT %lGE-6\r\n" , pCurrent);
    comHandler_->SendCommand(stringinput);
}

PowerControlClass::fVACvalues *ControlKeithleyPower::getVoltAndCurr()
{
    PowerControlClass::fVACvalues *cObject = new PowerControlClass::fVACvalues();
    memset(cObject, 0, sizeof(PowerControlClass::fVACvalues));
    
    cObject->pVSet1 = fVoltSet;
    cObject->pISet1 = fCurrCompliance;

    if ( getKeithleyOutputState ( ) )
    {
	checkVAC();

	cObject->pVApp1 = fVolt;
	cObject->pIApp1 = fCurr;
    }

    return cObject;

}

void ControlKeithleyPower::checkVAC()
{
    char stringinput[512];
    char buffer[1024];
    buffer[0] = 0;

    strcpy(stringinput , ":READ?\r\n");
    comHandler_->SendCommand(stringinput);
    QThread::msleep(500);

    comHandler_->ReceiveString(buffer);
    
    string str(buffer);
    size_t cPos = str.find(',');

    QString fVoltStr = QString::fromStdString(str.substr(0 , cPos));
    str = str.substr(cPos+1, cPos + 13);
    QString fCurrStr = QString::fromStdString(str.substr(0 , 13));

    fVolt = fVoltStr.toDouble();

    fCurr = fCurrStr.toDouble();

    emit voltAppChanged(fVolt);
}

void ControlKeithleyPower::closeConnection()
{
    setKeithleyOutputState ( 0 );
}

void ControlKeithleyPower::setKeithleyOutputState ( int outputsetting )
{
    if ( outputsetting == 0 and keithleyOutputOn)
    {
	char stringinput[512];
	char buffer[1024];
	strcpy(stringinput , ":OUTPUT1:STATE OFF\r\n");
	comHandler_->SendCommand(stringinput);
	comHandler_->ReceiveString(buffer);
	std::cout << buffer << std::endl;
	usleep(1000);
	keithleyOutputOn = false;
    }
    else if ( outputsetting == 1 and not keithleyOutputOn)
    {
	char stringinput[512];
	char buffer[1024];
	strcpy(stringinput , ":*RST\r\n");
	comHandler_->SendCommand(stringinput);
	comHandler_->ReceiveString(buffer);
	std::cout << buffer << std::endl;
	usleep(1000);

	strcpy(stringinput , ":*IDN?\r\n");
	comHandler_->SendCommand(stringinput);
	comHandler_->ReceiveString(buffer);
	std::cout << buffer << std::endl;
	usleep(1000);

	strcpy(stringinput , ":OUTPUT1:STATE ON\r\n");
	comHandler_->SendCommand(stringinput);
	comHandler_->ReceiveString(buffer);
	std::cout << buffer << std::endl;
	usleep(1000);

	strcpy(stringinput , ":SOURCE:VOLTAGE:RANGE 1000\r\n");
	comHandler_->SendCommand(stringinput);
	comHandler_->ReceiveString(buffer);
	std::cout << buffer << std::endl;
	usleep(1000);
	
	strcpy(stringinput , ":SENSE:FUNCTION 'CURRENT:DC'\r\n");
	comHandler_->SendCommand(stringinput);
	comHandler_->ReceiveString(buffer);
	std::cout << buffer << std::endl;
	usleep(1000);

	keithleyOutputOn = true;
    }
    
    emit outputStateChanged(keithleyOutputOn);
}

bool ControlKeithleyPower::getKeithleyOutputState ( )
{
    return ( keithleyOutputOn );
}
