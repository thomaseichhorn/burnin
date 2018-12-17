// Project includes
#include "hwdescriptionparser.h"

#include "general/BurnInException.h"

// Qt includes
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamAttributes>

// C++ includes
#include <iostream>

// Code
HWDescriptionParser::HWDescriptionParser()
{}

std::vector<GenericInstrumentDescription_t> HWDescriptionParser::ParseXML(std::string pFileName)
{
    // creating the vector to return
    std::vector<GenericInstrumentDescription_t> cInstruments;

    // opening file    
    QFile *cFile =  new QFile(pFileName.c_str());
    if(!cFile->open(QFile::ReadOnly))
        throw BurnInException("Unable to open XML file");
    QXmlStreamReader *cXmlFile = new QXmlStreamReader(cFile);

    while (!cXmlFile->atEnd() && !cXmlFile->hasError())
    {
        QXmlStreamReader::TokenType token = cXmlFile->readNext();
        if (token == QXmlStreamReader::StartDocument)
            continue;
        if (token == QXmlStreamReader::StartElement)
        {
            if (cXmlFile->name() == "HardwareDescription")
                continue;
            else if (cXmlFile->name() == "Power")
                ParsePower(cXmlFile, cInstruments);
            else if (cXmlFile->name() == "Environment")
                ParseEnvironment(cXmlFile , cInstruments);
            else if (cXmlFile->name() == "DataAcquisition")
                ParseDataAquisition(cXmlFile, cInstruments);
        }
    }
    if (cXmlFile->hasError())
        throw BurnInException("Invalid XML in configuration file. Missing end tag?");
    delete cXmlFile;
    delete cFile;

    return cInstruments;
}

void HWDescriptionParser::ParsePower(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments) {

    while (pXmlFile->readNextStartElement()) {
        std::string name = pXmlFile->name().toString().toStdString();
        if(name == "VoltageSource")
            ParseVoltageSource(pXmlFile, pInstruments);
        else
            throw BurnInException("Invalid Power tag \"" + name + "\". Valid tags are: VoltageSource");
    }

}

GenericInstrumentDescription_t HWDescriptionParser::ParseGeneric(const QXmlStreamReader *pXmlFile) const {
    GenericInstrumentDescription_t cInstrument;
    QXmlStreamAttributes attributes = pXmlFile->attributes();
    for (const auto& attribute: attributes) {
        std::string name = attribute.name().toString().toStdString();
        std::string value = attribute.value().toString().toStdString();
        if (name == "name")
            cInstrument.name = value;
        else if (name == "class")
            cInstrument.classOfInstr = value;
        else if (name == "desciption")
            cInstrument.description = value;
        else
            cInstrument.interface_settings[name] = value;
    }
    
    return cInstrument;
}

void HWDescriptionParser::ParseVoltageSource(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments) {

    GenericInstrumentDescription_t cInstrument = ParseGeneric(pXmlFile);
    cInstrument.section = "VoltageSource";

    while (pXmlFile->readNextStartElement()) {
        std::string name = pXmlFile->name().toString().toStdString();
        if(name == "Output") {
            std::map<std::string, std::string> cMap;
            QXmlStreamAttributes attributes = pXmlFile->attributes();
            for(int i = 0; i < attributes.length(); i++) {
                cMap[attributes.at(i).name().toString().toStdString()] = attributes.at(i).value().toString().toStdString();
            }
            cInstrument.operational_settings.push_back(cMap);
            pXmlFile->skipCurrentElement();
            
        } else
            throw BurnInException("Invalid VoltageSource tag \"" + name + "\". Valid tags are: Output");
    }

    // push back now
    pInstruments.push_back(cInstrument);

}

void HWDescriptionParser::ParseEnvironment(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments)
{
    while (pXmlFile->readNextStartElement()) {
        std::string name = pXmlFile->name().toString().toStdString();
        if(name == "ChillerControl")
            ParseChiller(pXmlFile, pInstruments);
        else if(name == "PeltierControl")
            ParsePeltier(pXmlFile, pInstruments);
        else if(name == "RaspberryControl")
            ParseRaspberry(pXmlFile, pInstruments);
        else
            throw BurnInException("Invalid DataAquisition tag \"" + name + "\". Valid tags are: DAQModule");
    }
}

void HWDescriptionParser::ParseChiller(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments)
{
    GenericInstrumentDescription_t cInstrument = ParseGeneric(pXmlFile);
    cInstrument.section = "ChillerControl";
    pXmlFile->skipCurrentElement();
    // push back now
    pInstruments.push_back(cInstrument);
}

void HWDescriptionParser::ParsePeltier(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments)
{
    GenericInstrumentDescription_t cInstrument = ParseGeneric(pXmlFile);
    cInstrument.section = "PeltierControl";
    pXmlFile->skipCurrentElement();
    pInstruments.push_back(cInstrument);
}

void HWDescriptionParser::ParseRaspberry(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments)
{
    GenericInstrumentDescription_t cInstrument = ParseGeneric(pXmlFile);
    cInstrument.section = "RaspberryControl";

    while (pXmlFile->readNextStartElement()) {
        std::string name = pXmlFile->name().toString().toStdString();
        if(name == "Sensor") {
            std::map<std::string, std::string> cMap;
            QXmlStreamAttributes attributes = pXmlFile->attributes();
            for(int i = 0; i < attributes.length(); i++) {
                cMap[attributes.at(i).name().toString().toStdString()] = attributes.at(i).value().toString().toStdString();
            }
            cInstrument.operational_settings.push_back(cMap);
            pXmlFile->skipCurrentElement();
        } else
            throw BurnInException("Invalid RaspberryControl tag \"" + name + "\". Valid tags are: Sensor");
        
    }
    pInstruments.push_back(cInstrument);
}

void HWDescriptionParser::ParseDataAquisition(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments) {
    while (pXmlFile->readNextStartElement()) {
        std::string name = pXmlFile->name().toString().toStdString();
        if(name == "DAQModule")
            ParseDAQModule(pXmlFile , pInstruments);
        else
            throw BurnInException("Invalid DataAquisition tag \"" + name + "\". Valid tags are: DAQModule");
    }
}

void HWDescriptionParser::ParseDAQModule(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments) {
    GenericInstrumentDescription_t cInstrument = ParseGeneric(pXmlFile);
    cInstrument.section = "DAQModule";
    pXmlFile->skipCurrentElement();
    pInstruments.push_back(cInstrument);
}
