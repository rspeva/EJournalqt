#include "changedate.h"
#include "ui_changedate.h"
#include "form.h"
#include <QMessageBox>

ChangeDate::ChangeDate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeDate)
{
    ui->setupUi(this);
    ui->etChangeDate->setText("");
    ui->etChangeDate->setInputMask("99.99");
}

ChangeDate::~ChangeDate()
{
    delete ui;
}

void ChangeDate::on_btnChangeDateNo_clicked()
{
    this->close();
}

void ChangeDate::on_btnChangeDateYes_clicked()
{
    QMessageBox msgBox;
    date = ui->etChangeDate->text();
    int day = date.left(2).toInt();
    int month = date.right(2).toInt();
    if ((day < 1 || day > 31) || (month < 1 || month > 12)){
             msgBox.setText("Введите дату в формате ДД.ММ   ");
             msgBox.exec();
    } else {
        timestamp = day * 86400 + month * 2629743;
        theme = ui->etChangeTheme->text();
        DZ = ui->etChangeDZ->text();
        emit onChangeDate();
        this->close();
    }
}
QString ChangeDate::getChangedDate()
{
    return date;
}
int ChangeDate::getChangedTimestamp()
{
    return timestamp;
}

QString ChangeDate::getChangedTheme()
{
    return theme;
}

QString ChangeDate::getChangedDZ()
{
    return DZ;
}


void ChangeDate::recieveLessonInfo(QString date, QString theme, QString DZ, bool login)
{
    ui->etChangeDate->setText(date);
    ui->etChangeTheme->setText(theme);
    ui->etChangeDZ->setText(DZ);
    if (!login)
    {
        ui->btnDelete->setVisible(false);
        ui->btnChangeDateYes->setVisible(false);
    }
}

void ChangeDate::on_btnDelete_clicked()
{

    QMessageBox ms;
    ms.setStyleSheet("QLabel {font: 75 12pt 'Segoe Print' ;}");
    ms.setIcon(QMessageBox::Question);
    QAbstractButton *no = ms.addButton("Отмена",QMessageBox::NoRole);
    QAbstractButton *yes = ms.addButton("Да",QMessageBox::YesRole);
    no->setStyleSheet("QPushButton{ color: white;"
                       "background-color: qlineargradient(spread:repeat, x1:0.511, y1:0, x2:0.506, y2:1, stop:0 rgba(53, 200, 102, 255), stop:1 rgba(4, 142, 50, 255));"
                       "border-radius:8px;"
                       "border-style: solid;"
                       "border-color: rgb(147, 147, 147);"
                       "border-width: 1px;"
                       "font: 75 12pt 'Segoe Print' ; }"
                       "QPushButton:hover{"
                       "background-color: rgb(7, 193, 82);"
                       "}"
                       "QPushButton:pressed{"
                       "background-color: qlineargradient(spread:repeat, x1:0.511, y1:0, x2:0.506, y2:1, stop:0 rgba(53, 200, 102, 255), stop:1 rgba(4, 142, 50, 255));"
                       "}");
    no->setMinimumWidth(100);
    no->setMinimumHeight(30);

    yes->setStyleSheet("QPushButton{ color: white;"
                       "background-color: qlineargradient(spread:repeat, x1:0.511, y1:0, x2:0.506, y2:1, stop:0 rgba(238, 48, 48, 255), stop:1 rgba(192, 1, 1, 255));"
                       "border-radius:8px;"
                       "border-style: solid;"
                       "border-color: rgb(147, 147, 147);"
                       "border-width: 1px;"
                       "font: 75 12pt 'Segoe Print' ; }"
                       "QPushButton:hover{"
                       "background-color: rgb(255, 55, 55);"
                       "}"
                       "QPushButton:pressed{"
                       "background-color: qlineargradient(spread:repeat, x1:0.511, y1:0, x2:0.506, y2:1, stop:0 rgba(238, 48, 48, 255), stop:1 rgba(192, 1, 1, 255));"
                       "}");
    yes->setMinimumWidth(100);
    yes->setMinimumHeight(30);

    ms.setText("Удалить урок?");
    ms.exec();
    if(ms.clickedButton() == yes)
    {
        emit onDeleteLesson();
        this->close();
    }
}
