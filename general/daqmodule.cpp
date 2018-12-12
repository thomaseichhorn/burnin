#include "daqmodule.h"

#include "BurnInException.h"

#include <QDir>
#include <QFileInfo>
#include <QProcess>

#include <iostream>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

DAQModule::DAQModule(const QString& fc7Port, const QString& controlhubPath, const QString& ph2acfPath, const QString& daqHwdescFile, const QString& daqImage)
{
	_contrStartPath = _pathjoin({controlhubPath, "bin", "controlhub_start"});
	_ph2SetupPath = _pathjoin({ph2acfPath, "setup.sh"});
	_ph2SetupCommand = "cd \"" + ph2acfPath + "\"; source \"" + _ph2SetupPath + "\"";
	_ph2FpgaConfigPath = _pathjoin({ph2acfPath, "bin", "fpgaconfig"});
	_ph2SystemtestPath = _pathjoin({ph2acfPath, "bin", "systemtest"});
	_ph2DatatestPath = _pathjoin({ph2acfPath, "bin", "datatest"});
	_ph2CalibratePath = _pathjoin({ph2acfPath, "bin", "calibrate"});
	_ph2HybridtestPath = _pathjoin({ph2acfPath, "bin", "hybridtest"});
	_ph2CmtestPath = _pathjoin({ph2acfPath, "bin", "cmtest"});
	_ph2CommissionPath = _pathjoin({ph2acfPath, "bin", "commission"});
	_daqHwdescFile = daqHwdescFile;
	_daqImage = daqImage;
	
	_fc7Port = fc7Port;
	_fc7comhandler = nullptr;
	_fc7power = false;
	
	// Check if all needed executables and files exist
	if (not QFileInfo(_contrStartPath).isExecutable())
		throw BurnInException("Can not execute controlhub_start in controlhubPath " + _contrStartPath.toStdString());
	if (not QFileInfo(_ph2SystemtestPath).isExecutable())
		throw BurnInException("Can not execute systemtest in controlhubPath " + _ph2SystemtestPath.toStdString());
	if (not QFileInfo(_ph2DatatestPath).isExecutable())
		throw BurnInException("Can not execute datatest in controlhubPath " + _ph2DatatestPath.toStdString());
	if (not QFileInfo(_ph2CalibratePath).isExecutable())
		throw BurnInException("Can not execute calibrate in controlhubPath " + _ph2CalibratePath.toStdString());
	if (not QFileInfo(_ph2HybridtestPath).isExecutable())
		throw BurnInException("Can not execute hybridtest in controlhubPath " + _ph2HybridtestPath.toStdString());
	if (not QFileInfo(_ph2CmtestPath).isExecutable())
		throw BurnInException("Can not execute cmtest in controlhubPath " + _ph2CmtestPath.toStdString());
	if (not QFileInfo(_ph2CommissionPath).isExecutable())
		throw BurnInException("Can not execute commission in controlhubPath " + _ph2CommissionPath.toStdString());
	if (not QFileInfo(_daqHwdescFile).isReadable())
		throw BurnInException("Can not read daqHwdescFile " + _daqHwdescFile.toStdString());
}

DAQModule::~DAQModule() {
	if (_fc7comhandler != nullptr)
		delete _fc7comhandler;
}

void DAQModule::initialize() {
	// Start controlhub
	QProcess controlhub_start;
	if (not controlhub_start.startDetached(_contrStartPath, {}))
		throw BurnInException("Unable to start the controlhub " + _contrStartPath.toStdString());

	_fc7comhandler = new ComHandler ( _fc7Port.toStdString ( ) .c_str ( ), B9600 );
	// query status
	// no feed for the arduino
	bool nofeed = false;
	_fc7comhandler -> SendCommand ( "2", nofeed );
	// wait for arduino to process this
	// 1000 from comhandler...
	usleep ( 10000 );
	char buffer[1];
	_fc7comhandler -> ReceiveByte ( buffer );
	if ( buffer[0] == '0' )
	{
	    _fc7power = false;
	    std::cout << "FC7 power is OFF!" << std::endl;
	}
	else if ( buffer[0] == '1' )
	{
	    _fc7power = true;
	    std::cout << "FC7 power is ON!" << std::endl;
	}
	else
	{
	    throw BurnInException ( "Can't get FC7 power status!" );
	}

}

QString DAQModule::_pathjoin(const std::initializer_list<const QString>& parts) const {
	QString path;
	for (const auto& part: parts) {
		if (path.length() != 0)
			path += QDir::separator();
		path += part;
	}
	return QDir::cleanPath(path);
}

void DAQModule::setFC7Power ( bool power )
{
    if ( power )
    {
	_fc7comhandler -> SendCommand ( "1", false );
	//usleep ( 10000 );
	//_fc7comhandler -> SendCommand ( "2", false );
	//usleep ( 10000 );
	//char buffer[1];
	//_fc7comhandler -> ReceiveByte ( buffer );
	//if ( buffer[0] != '1' )
	//{
	//    //throw BurnInException ( "Can't get FC7 power status!" );
	//    std::cout << "asdf " << buffer << std::endl;
	//}
    }
    else
    {
	_fc7comhandler -> SendCommand ( "0", false );
	//usleep ( 10000 );
	//_fc7comhandler -> SendCommand ( "2", false );
	//usleep ( 10000 );
	//char buffer[1];
	//_fc7comhandler -> ReceiveByte ( buffer );
	//if ( buffer[0] != '0' )
	//{
	//    //throw BurnInException ( "Can't get FC7 power status!" );
	//    std::cout << "asdf " << buffer << std::endl;
	//}
    }
    _fc7power = power;
}

bool DAQModule::getFC7Power() const {
	return _fc7power;
}

void DAQModule::loadFirmware() const {
	QProcess fpgaconfig;
	
	QString cmd = _ph2SetupCommand + "; \"" + _ph2FpgaConfigPath + "\" -c \"" + _daqHwdescFile + "\" -i \"" + _daqImage + "\"";
	if (not fpgaconfig.startDetached("/bin/bash", {"-c", cmd}))
		throw BurnInException("Unable to load firmware. Command: " + cmd.toStdString());
}

void DAQModule::runSystemTest() const {
	_run_ph2_binary("systemtest", _ph2SystemtestPath);
}

void DAQModule::runDatatest() const {
	_run_ph2_binary("datatest", _ph2DatatestPath);
}

void DAQModule::runCalibrate() const {
	_run_ph2_binary("calibrate", _ph2CalibratePath);
}

void DAQModule::runHybridtest() const {
	_run_ph2_binary("hybridtest", _ph2HybridtestPath);
}

void DAQModule::runCmtest() const {
	_run_ph2_binary("cmtest", _ph2CmtestPath);
}

void DAQModule::runCommission() const {
	_run_ph2_binary("comission", _ph2CommissionPath);
}

void DAQModule::_run_ph2_binary(const QString& name, const QString& path) const {
	QProcess process;
	
	QString cmd = _ph2SetupCommand + "; \"" + path + "\" -f \"" + _daqHwdescFile + "\"";
	if (not process.startDetached("/bin/bash", {"-c", cmd}))
		throw BurnInException("Unable to run" + name.toStdString() + ". Command: " + cmd.toStdString());
}
