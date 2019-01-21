#include "student.h"
#include "QSqlQuery"
#include "QVariant"
Student::Student()
{
}

void Student::setId(QString id){this->id = id;}
void Student::setName(QString name){this->name = name;}
void Student::setSurname(QString surname){this->surname = surname;}
void Student::setPatronymic(QString patronymic){this->patronymic = patronymic;}
void Student::setAdress(QString adress){this->adress = adress;}

QString Student::getId(){return this->id;}
QString Student::getName(){return this->name;}
QString Student::getSurname(){return this->surname;}
QString Student::getPatronymic(){return this->patronymic;}
QString Student::getAdress(){return this->adress;}


void Student::setAll(QString id)
{
    DBHelper *dbHelper = new DBHelper();
    QSqlQuery query = dbHelper->makeQuery("SELECT Surname, Name, Patronymic, Adress FROM STUDENTS_" + id[2] + "A WHERE id='" + id + "'");
    query.next();
    this->id = id;
    this->surname = query.value(0).toString();
    this->name = query.value(1).toString();
    this->patronymic = query.value(2).toString();
    this->adress = query.value(3).toString();
}

