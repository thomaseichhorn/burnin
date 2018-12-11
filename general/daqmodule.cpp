#include "daqmodule.h"

#include "BurnInException.h"

#include <QDir>
#include <QFileInfo>
#include <QProcess>

#include <iostream>

DAQModule::DAQModule(const QString& controlhubPath, const QString& ph2acfPath, const QString& daqHwdescFile, const QString& daqImage)
{
	
	_contrStartPath = _pathjoin({controlhubPath, "bin", "controlhub_start"});
	_ph2SetupPath = _pathjoin({ph2acfPath, "setup.sh"});
	_ph2SetupCommand = "cd \"" + ph2acfPath + "\"; source \"" + _ph2SetupPath + "\"";
	_ph2FpgaConfigPath = _pathjoin({ph2acfPath, "bin", "fpgaconfig"});
	_ph2SystemtestPath = _pathjoin({ph2acfPath, "bin", "systemtest"});
	_ph2DatatestPath = _pathjoin({ph2acfPath, "bin", "datatest"});
	_ph2HybridtestPath = _pathjoin({ph2acfPath, "bin", "hybridtest"});
	_ph2CmtestPath = _pathjoin({ph2acfPath, "bin", "cmtest"});
	_ph2CommissionPath = _pathjoin({ph2acfPath, "bin", "commission"});
	_daqHwdescFile = daqHwdescFile;
	_daqImage = daqImage;
	
	_fc7power = false;
	
	// Check if all needed executables and files exist
	if (not QFileInfo(_contrStartPath).isExecutable())
		throw BurnInException("Can not execute controlhub_start in controlhubPath " + _contrStartPath.toStdString());
	if (not QFileInfo(_ph2SystemtestPath).isExecutable())
		throw BurnInException("Can not execute systemtest in controlhubPath " + _ph2SystemtestPath.toStdString());
	if (not QFileInfo(_ph2DatatestPath).isExecutable())
		throw BurnInException("Can not execute datatest in controlhubPath " + _ph2DatatestPath.toStdString());
	if (not QFileInfo(_ph2HybridtestPath).isExecutable())
		throw BurnInException("Can not execute hybridtest in controlhubPath " + _ph2HybridtestPath.toStdString());
	if (not QFileInfo(_ph2CmtestPath).isExecutable())
		throw BurnInException("Can not execute cmtest in controlhubPath " + _ph2CmtestPath.toStdString());
	if (not QFileInfo(_ph2CommissionPath).isExecutable())
		throw BurnInException("Can not execute commission in controlhubPath " + _ph2CommissionPath.toStdString());
	if (not QFileInfo(_daqHwdescFile).isReadable())
		throw BurnInException("Can not read daqHwdescFile " + _daqHwdescFile.toStdString());
}

void DAQModule::initialize() {
	// Start controlhub
	QProcess controlhub_start;
	if (not controlhub_start.startDetached(_contrStartPath, {}))
		throw BurnInException("Unable to start the controlhub " + _contrStartPath.toStdString());
		
	// Get whether FC7 is powered on
	// TODO
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
	QProcess systemtest;
	
	QString cmd = _ph2SetupCommand + "; \"" + _ph2SystemtestPath + "\" -f \"" + _daqHwdescFile + "\"";
	if (not systemtest.startDetached("/bin/bash", {"-c", cmd}))
		throw BurnInException("Unable to run system test. Command: " + cmd.toStdString());
}

void DAQModule::runCalibrate() const {
	
}

void DAQModule::runDatatest() const {
	
}

void DAQModule::runHybridtest() const {
	
}

void DAQModule::runCmtest() const {
	
}

void DAQModule::runCommission() const {
	
}

