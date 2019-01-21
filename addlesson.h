#ifndef ADDLESSON_H
#define ADDLESSON_H

#include <QDialog>
#include "dbhelper.h"

namespace Ui {
class AddLesson;
}

class AddLesson : public QDialog
{
    Q_OBJECT

public:
    explicit AddLesson(QWidget *parent = 0);
    ~AddLesson();
    int getTimestamp();
    QString getDate();
    QString getTheme();
    QString getDZ();
signals:
    void onAddLesson();
private slots:
    void on_btnAddLessNo_clicked();
    void on_btnAddLessYes_clicked();

private:
    Ui::AddLesson *ui;
    DBHelper *dbHelper;
    QString date, DZ, theme;
    int timestamp;
};

#endif // ADDLESSON_H
