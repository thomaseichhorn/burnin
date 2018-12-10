// Project includes
#include "hwdescriptionparser.h"

#include "general/BurnInException.h"

// Qt includes
#include "QFile"
#include "QXmlStreamReader"
#include "QXmlStreamAttributes"

// C++ includes
#include "iostream"

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
            if (cXmlFile->name() == "Power")
                ParsePower(cXmlFile, cInstruments);
            if (cXmlFile->name() == "Environment")
                ParseEnvironment(cXmlFile , cInstruments);
        }
    }
    delete cXmlFile;
    delete cFile;

    return cInstruments;
}

void HWDescriptionParser::ParsePower(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments) {

    while (!(pXmlFile->tokenType() == QXmlStreamReader::EndElement && pXmlFile->name() == "Power"))
    {
        pXmlFile->readNext();
        if (pXmlFile->tokenType() == QXmlStreamReader::StartElement)
        {
            if(pXmlFile->name() == "LowVoltage")
                continue;
            if(pXmlFile->name() == "HighVoltage")
                continue;
            if(pXmlFile->name() == "VoltageSource")
                ParseVoltageSource(pXmlFile, pInstruments);
        }

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

    while (!(pXmlFile->tokenType() == QXmlStreamReader::EndElement && pXmlFile->name() == "VoltageSource"))
    {
        pXmlFile->readNext();
        if (pXmlFile->tokenType() == QXmlStreamReader::StartElement)
        {
            if(pXmlFile->name() == "Output") {
                std::map<std::string, std::string> cMap;
                QXmlStreamAttributes attributes = pXmlFile->attributes();
                for(int i = 0; i < attributes.length(); i++) {
                    cMap[attributes.at(i).name().toString().toStdString()] = attributes.at(i).value().toString().toStdString();
                }
                cInstrument.operational_settings.push_back(cMap);
            }
        }
    }

    // push back now
    pInstruments.push_back(cInstrument);

}

void HWDescriptionParser::ParseEnvironment(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments)
{
    while (!(pXmlFile->tokenType() == QXmlStreamReader::EndElement && pXmlFile->name() == "Environment"))
    {
        pXmlFile->readNext();
        if (pXmlFile->tokenType() == QXmlStreamReader::StartElement)
        {
            if(pXmlFile->name() == "ChillerControl")
                ParseChiller(pXmlFile , pInstruments);
            if(pXmlFile->name() == "PeltierControl")
                ParsePeltier(pXmlFile , pInstruments);
            if(pXmlFile->name() == "RaspberryControl")
                ParseRaspberry(pXmlFile, pInstruments);
        }

    }
}

void HWDescriptionParser::ParseChiller(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments)
{
    GenericInstrumentDescription_t cInstrument = ParseGeneric(pXmlFile);
    // push back now
    pInstruments.push_back(cInstrument);
}

void HWDescriptionParser::ParsePeltier(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments)
{
    GenericInstrumentDescription_t cInstrument = ParseGeneric(pXmlFile);
    pInstruments.push_back(cInstrument);
}

void HWDescriptionParser::ParseRaspberry(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments)
{
    GenericInstrumentDescription_t cInstrument = ParseGeneric(pXmlFile);

    while (!(pXmlFile->tokenType() == QXmlStreamReader::EndElement && pXmlFile->name() == "RaspberryControl"))
    {
        pXmlFile->readNext();
        if (pXmlFile->tokenType() == QXmlStreamReader::StartElement)
        {
            if(pXmlFile->name() == "Sensor") {
                std::map<std::string, std::string> cMap;
                QXmlStreamAttributes attributes = pXmlFile->attributes();
                for(int i = 0; i < attributes.length(); i++) {
                    cMap[attributes.at(i).name().toString().toStdString()] = attributes.at(i).value().toString().toStdString();
                }
                cInstrument.operational_settings.push_back(cMap);
            }
        }
    }
    pInstruments.push_back(cInstrument);
}

