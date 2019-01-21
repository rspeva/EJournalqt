#include "studentabout.h"
#include "ui_studentabout.h"

StudentAbout::StudentAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StudentAbout)
{
    ui->setupUi(this);
}

StudentAbout::~StudentAbout()
{
    delete ui;
}

void StudentAbout::recieveStudentInfo(Student student)
{
    id = student.getId();
    ui->etName->setText(student.getName());
    ui->etSurname->setText(student.getSurname());
    ui->etPatronymic->setText(student.getPatronymic());
    ui->etAdress->setText(student.getAdress());
}

void StudentAbout::on_btnMore_clicked()
{
    emit onStudentAbout();
    this->close();
}

QString StudentAbout::getStudentId()
{
    return id;
}
