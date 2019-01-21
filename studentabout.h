#ifndef STUDENTABOUT_H
#define STUDENTABOUT_H

#include <QDialog>
#include <student.h>

namespace Ui {
class StudentAbout;
}

class StudentAbout : public QDialog
{
    Q_OBJECT

public:
    explicit StudentAbout(QWidget *parent = 0);
    ~StudentAbout();
    QString getStudentId();
signals:
    void onStudentAbout();
private slots:
    void recieveStudentInfo(Student student);
    void on_btnMore_clicked();

private:
    Ui::StudentAbout *ui;
    QString id;
};

#endif // STUDENTABOUT_H
