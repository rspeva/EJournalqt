#include "dbhelper.h"

DBHelper::DBHelper()
{}

bool DBHelper::openDB(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:\\database.db3");
    return db.open();
}

QSqlQuery DBHelper::makeQuery(QString string){
    QSqlQuery query;
    query.exec(string);
    return query;
}

void DBHelper::createSubject(QString group, QString titleEng, QString id[], int n){
    QString IDs;
    for (int i = 0; i < n; i++){
        IDs += ", " + id[i] + " TEXT";
    }
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS S_" + group + "_" + titleEng +
               " (Timestamp INTEGER, Date TEXT, Theme TEXT, DZ TEXT" + IDs + ");");
}



