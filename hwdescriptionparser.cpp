// Project includes
#include "hwdescriptionparser.h"

// Qt includes
#include "QFile"
#include "QXmlStreamReader"
#include "QXmlStreamAttributes"
#include "QFileDialog"

// C++ includes
#include "iostream"

// Code
HWDescriptionParser::HWDescriptionParser()
{}

std::vector<GenericInstrumentDescription_t> HWDescriptionParser::ParseXML()
{
    // creating the vector to return
    std::vector<GenericInstrumentDescription_t> cInstruments;


    // opening file

    QString cFilter = "*.xml";
    QString cFileName = QFileDialog::getOpenFileName( nullptr , "Open a file" , "/home/" , cFilter);
    QFile *cFile =  new QFile(cFileName);
    if(!cFile->open(QFile::ReadOnly))
    {
        std::cout << "Unable to open XML file" << std::endl;
        exit(1);
    }
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

void HWDescriptionParser::ParseVoltageSource(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments) {

    GenericInstrumentDescription_t cInstrument;
    QXmlStreamAttributes attributes = pXmlFile->attributes();
    for(int i = 0; i < attributes.length(); i++) {
        //add name clas type and description to same keys in map and all other attributes to interface settings map
        if (attributes.at(i).name() == "name") cInstrument.name = attributes.at(i).value().toString().toStdString();
        else if (attributes.at(i).name() == "class") cInstrument.typeOfClass = attributes.at(i).value().toString().toStdString();
        else if (attributes.at(i).name() == "type") cInstrument.type = attributes.at(i).value().toString().toStdString();
        else if (attributes.at(i).name() == "description") cInstrument.description = attributes.at(i).value().toString().toStdString();
        else cInstrument.interface_settings[attributes.at(i).name().toString().toStdString()] = attributes.at(i).value().toString().toStdString();
    }

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
    GenericInstrumentDescription_t cInstrument;
    QXmlStreamAttributes attributes = pXmlFile->attributes();
    for(int i = 0; i < attributes.length(); i++) {
        if (attributes.at(i).name() == "name") cInstrument.name = attributes.at(i).value().toString().toStdString();
        else if (attributes.at(i).name() == "type") cInstrument.type = attributes.at(i).value().toString().toStdString();
        else if (attributes.at(i).name() == "description") cInstrument.description = attributes.at(i).value().toString().toStdString();
        else cInstrument.interface_settings[attributes.at(i).name().toString().toStdString()] = attributes.at(i).value().toString().toStdString();
    }

    while (!(pXmlFile->tokenType() == QXmlStreamReader::EndElement && pXmlFile->name() == "ChillerControl"))
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

void HWDescriptionParser::ParsePeltier(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments)
{
    GenericInstrumentDescription_t cInstrument;
    QXmlStreamAttributes attributes = pXmlFile->attributes();
    for(int i = 0; i < attributes.length(); i++) {
        if (attributes.at(i).name() == "name") cInstrument.name = attributes.at(i).value().toString().toStdString();
        else if (attributes.at(i).name() == "type") cInstrument.type = attributes.at(i).value().toString().toStdString();
        else if (attributes.at(i).name() == "description") cInstrument.description = attributes.at(i).value().toString().toStdString();
        else cInstrument.interface_settings[attributes.at(i).name().toString().toStdString()] = attributes.at(i).value().toString().toStdString();
    }
    pInstruments.push_back(cInstrument);
}

void HWDescriptionParser::ParseRaspberry(QXmlStreamReader *pXmlFile, std::vector<GenericInstrumentDescription_t>& pInstruments)
{
    GenericInstrumentDescription_t cInstrument;
    QXmlStreamAttributes attributes = pXmlFile->attributes();
    for(int i = 0; i < attributes.length(); i++) {
        if (attributes.at(i).name() == "name") cInstrument.name = attributes.at(i).value().toString().toStdString();
        else if (attributes.at(i).name() == "type") cInstrument.type = attributes.at(i).value().toString().toStdString();
        else if (attributes.at(i).name() == "description") cInstrument.description = attributes.at(i).value().toString().toStdString();
        else cInstrument.interface_settings[attributes.at(i).name().toString().toStdString()] = attributes.at(i).value().toString().toStdString();
    }
    pInstruments.push_back(cInstrument);
}

