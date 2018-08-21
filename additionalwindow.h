//additional window for enter value in case of wait or set temperature
#ifndef ADDITIONALWINDOW_H
#define ADDITIONALWINDOW_H

#include <QDialog>

namespace Ui {
class AdditionalWindow;
}

class AdditionalWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AdditionalWindow(QWidget *parent = 0);
    ~AdditionalWindow();
    QString getValue();
    void setValue(QString);
    QString fValue;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AdditionalWindow *ui;

};

#endif // ADDITIONALWINDOW_H
