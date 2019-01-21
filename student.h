#ifndef STUDENT_H
#define STUDENT_H
#include <QString>
#include <dbhelper.h>
class Student
{
private:

    QString id, name, surname, patronymic, adress;

public:
    Student();
    void setId(QString);
    void setName(QString);
    void setSurname(QString);
    void setPatronymic(QString);
    void setAdress(QString);
    void setAll(QString);

    QString getId();
    QString getName();
    QString getSurname();
    QString getPatronymic();
    QString getAdress();
};

#endif // STUDENT_H
