#include "systemcontrollerclass.h"

#include <QFile>
#include <QTextStream>
#include <QString>

#include <string>
#include <vector>

using namespace std;

SystemControllerClass::SystemControllerClass()
{
    fVolt = nullptr;
    fDaqControl = nullptr;
    fEnv = nullptr;
    fDatabase = nullptr;
}
void SystemControllerClass::Initialize()
{
    fVolt->InitPwr();
    fDaqControl->InitDAQ();
    fEnv->InitEnv();
    fDatabase->InitDatabase();
    SystemControllerClass *fSys = new SystemControllerClass();
    vector<string> *cVec;
    cVec = fSys->readFile();
    fSys->doList(cVec);
}

vector<string>* SystemControllerClass::readFile()
{
    vector<string> *cVec = new vector<string>();
    QFile cFile("/home/taras/testfile.txt");
    cFile.open(QFile::ReadOnly);
    QTextStream pStream(&cFile);
    while(!pStream.atEnd()){
        string pStr = pStream.readLine().toStdString();
        cVec->push_back(pStr);
    }
    cFile.flush();
    cFile.close();
    return cVec;
}

void SystemControllerClass::doList(vector<string>* cVec)
{
    struct cParam{
        string cName;
        double cValue;
    };
    vector<cParam> cList;
    size_t cTemp , cVolt , cDAQ , cWait;
    string cStr;
    cParam cObj;
    for(vector<string>::iterator i = cVec->begin() ; i != cVec->end() ; i++){
        cStr = (*i);
        cout << cStr << endl;
        cTemp = cStr.find('T');
        cVolt = cStr.find('V');
        cDAQ = cStr.find('D');
        cWait = cStr.find('W');

        //Temp
        if( !cTemp ){
            //string tPosStr;
            size_t cPos;
            cPos = cStr.find("=");
            auto cPosStr = cStr.substr(cPos+1);
            double cTemp = stod(cPosStr.c_str(), 0);
            //fEnv->setTemp(pTemp);
            cout << cTemp << endl;
            cObj.cName = "T";
            cObj.cValue = cTemp;
            cList.push_back(cObj);
        }
        //Voltage
        if( !cVolt ){
            string cPosStr;
            size_t cPos;
            cPos = cStr.find("=");
            cPosStr = cStr.substr(cPos+1);
            double cVolt = stod(cPosStr.c_str(), 0);
            cObj.cName = "V";
            cObj.cValue = cVolt;
            cList.push_back(cObj);
        }
        if( !cDAQ ){

        }
        if( !cWait ){
            string cPosStr;
            size_t cPos;
            cPos = cStr.find("=");
            cPosStr = cStr.substr(cPos+1);
            double cW = stod(cStr.substr(cPos+1));
            cObj.cName = "W";
            cObj.cValue = cW;
        }

    }

}

