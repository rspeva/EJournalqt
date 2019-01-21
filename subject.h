#ifndef SUBJECT_H
#define SUBJECT_H
#include "student.h"
#include "vector"
#include <dbhelper.h>
#include <QVariant>

using namespace std;

class Subject
{
public:
    Subject();
    void setClas(QString clas);
    void setTitle(QString title);
    void setTitleEng(QString titleEng);
    void setTeacher(QString teacher);
    void setTeacherId(QString idTeacher);

    QString getClas();
    QString getTitle();
    QString getTitleEng();
    QString getTeacher();
    QString getTeacherId();
    vector<int> getTimestamp();
    vector<QString> getDate();
    vector<QString> getTheme();
    vector<QString> getDZ();
    vector<QString> getMarks();

    void getSubject(QString id, QString titleEng);
    int getMarkAmt();
    double getAvr();
    int getMiss();
    int getAmt();

    private:
        DBHelper *dbHelper;
        QString title, titleEng, teacher, idTeacher, idStudent, clas;
        int miss, amt, markAmt;
        vector<int> timestamp;
        vector<Student> studnets;
        vector<QString> marks, date, theme, dz;
        double avr;
};

#endif // SUBJECT_H
