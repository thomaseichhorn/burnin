#include <QDebug>
#include <QMessageBox>

#include "additionalwindow.h"
#include "ui_additionalwindow.h"

AdditionalWindow::AdditionalWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdditionalWindow)
{
    ui->setupUi(this);
}

AdditionalWindow::~AdditionalWindow()
{
    delete ui;
}

//Close pushbutton
void AdditionalWindow::on_pushButton_clicked()
{
    this->close();
}

//Ok pushbutton
void AdditionalWindow::on_pushButton_2_clicked()
{
    QString cStr = ui->lineEdit->text();
    if(!cStr.isEmpty()){
        setValue(cStr);
        this->close();
    }
}

QString AdditionalWindow::getValue()
{
    return fValue;
}

void AdditionalWindow::setValue(QString pStr)
{
    fValue = pStr;
}
