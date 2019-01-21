#include "addlesson.h"
#include "ui_addlesson.h"

#include <QMessageBox>


AddLesson::AddLesson(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddLesson)
{
    ui->setupUi(this);
    ui->etAddLessDate->setInputMask("99.99");
}

AddLesson::~AddLesson()
{
    delete ui;
}

void AddLesson::on_btnAddLessNo_clicked()
{
    this->close();
}

void AddLesson::on_btnAddLessYes_clicked()
{
    QMessageBox msgBox;
    date = ui->etAddLessDate->text();
    int day = date.left(2).toInt();
    int month = date.right(2).toInt();
    if ((day < 1 || day > 31) || (month < 1 || month > 12)){
             msgBox.setText("Введите дату в формате ДД.ММ   ");
             msgBox.exec();
    } else {
        timestamp = day * 86400 + month * 2629743;
        theme = ui->etAddLessTheme->text();
        DZ = ui->etAddLessDZ->text();
        emit onAddLesson();
        this->close();
    }
}

QString AddLesson::getDate(){
    return date;
}
QString AddLesson::getDZ(){
    return DZ;
}
QString AddLesson::getTheme(){
    return theme;
}
int AddLesson::getTimestamp(){
    return timestamp;
}

