#ifndef DAQMODULE_H
#define DAQMODULE_H

#include "genericinstrumentclass.h"

#include <QString>
#include <initializer_list>

class DAQModule : public GenericInstrumentClass
{
public:
    DAQModule(const QString& controlhubPath, const QString& ph2acfPath, const QString& daqHwdescFile, const QString& daqImage);
    void initialize();
    
    void setFC7Power(bool power);
    bool getFC7Power() const;
    
    void loadFirmware() const;
    void runSystemTest() const;
    void runCalibrate() const;
    void runDatatest() const;
    void runHybridtest() const;
    void runCmtest() const;
    void runCommission() const;
    
private:
    QString _contrStartPath;
    QString _ph2SetupPath;
    QString _ph2SetupCommand;
    QString _ph2FpgaConfigPath;
    QString _ph2SystemtestPath;
    QString _ph2DatatestPath;
    QString _ph2CalibratePath;
    QString _ph2HybridtestPath;
    QString _ph2CmtestPath;
    QString _ph2CommissionPath;
    QString _daqHwdescFile;
    QString _daqImage;
    
    bool _fc7power;
    
    QString _pathjoin(const std::initializer_list<const QString>& parts) const;
    void _run_ph2_binary(const QString& name, const QString& path) const;
};

#endif // DAQMODULE_H
