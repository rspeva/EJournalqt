#include "subject.h"
#include "QDebug"
#include "QMessageBox"

Subject::Subject()
{
    dbHelper = new DBHelper();
}

void Subject::setClas(QString clas){this->clas = clas;}
void Subject::setTitle(QString title){this->title = title;}
void Subject::setTitleEng(QString titleEng){this->titleEng = titleEng;}
void Subject::setTeacher(QString teacher){this->teacher = teacher;}
void Subject::setTeacherId(QString teacherId){this->idTeacher = teacherId;}

QString Subject::getClas(){return this->clas;}
QString Subject::getTitle(){return this->title;}
QString Subject::getTitleEng(){return this->titleEng;}
QString Subject::getTeacher(){return this->teacher;}
QString Subject::getTeacherId(){return this->idTeacher;}
vector<int> Subject::getTimestamp(){return this->timestamp;}
vector<QString> Subject::getDate(){return this->date;}
vector<QString> Subject::getTheme(){return this->theme;}
vector<QString> Subject::getDZ(){return this->dz;}
vector<QString> Subject::getMarks(){return this->marks;}




void Subject::getSubject(QString id, QString titleEng){
    marks.clear();
    timestamp.clear();
    date.clear();
    theme.clear();
    dz.clear();
    amt = 0; miss = 0; markAmt = 0;
    try{
    QSqlQuery query = dbHelper->makeQuery("SELECT " + id + ", Timestamp, Date, Theme, DZ FROM S_" + id[2] + "A_" + titleEng + " ORDER BY Timestamp");
    query.last();
    markAmt = query.at()+1;
    if (markAmt+1 != 0)
    {
    query.first();
    query.previous();

    int i = 0;
    while (query.next())
       {
       marks.push_back(query.value(0).toString());
       timestamp.push_back(query.value(1).toInt());
       date.push_back(query.value(2).toString());
       theme.push_back(query.value(3).toString());
       dz.push_back(query.value(4).toString());
       i++;
       }

    int sum = 0;
    for (i = 0; i < markAmt; i++)
    {
       if (marks[i] == "H")
           miss++;
       else if (marks[i] != "" && marks[i] != " ")
       {
           sum += marks[i].toInt();
           amt++;
       }
    }
    avr = (double) sum/amt;
    }
    } catch (exception & e){
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}

double Subject::getAvr()
{
    if (markAmt+1 == 0)
        return -1;
    else if (amt == 0)
        return 0;
    else return  avr;
}

int Subject::getMiss()
{
    if (markAmt+1 != 0)
    return  miss;
    else return -1;
}

int Subject::getAmt()
{
    if (markAmt+1 != 0)
    return  amt;
    else return -1;
}

int Subject::getMarkAmt()
{
    return markAmt;
}
