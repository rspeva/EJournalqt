#ifndef DBHELPER_H
#define DBHELPER_H

#include <QObject>
#include "QtSql/QSqlDatabase"
#include "QSqlQuery"


class DBHelper
{
public:
    DBHelper();
public slots:
    QSqlQuery makeQuery(QString);
    void createSubject(QString, QString, QString[], int);
    bool openDB();
private:
     QSqlDatabase db;
};

#endif // DBHELPER_H
