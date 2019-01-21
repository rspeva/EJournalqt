#ifndef FORM_H
#define FORM_H
#include "subject.h"
#include "student.h"
#include "ftp.h"
#include "dbhelper.h"
#include "addlesson.h"
#include "changemark.h"
#include "changedate.h"
#include "studentabout.h"
#include "rating.h"
#include <QAbstractButton>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
#include <QUrl>
#include <QDebug>
#include <QMainWindow>
#include <QTableWidget>

using namespace std;

namespace Ui {
class Form;
}

class Form : public QMainWindow
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
    QString oldDate;
signals:
    void sendLessonInfo(QString date, QString theme, QString DZ, bool login);
    void sendStudentInfo(Student student);
    void sendRatingInfo(QString clas, QString title, QString titleEng);
private slots:
    void recieveLogin(bool login);
    void on_radioButton_class_clicked();
    void on_radioButton_prep_clicked();
    void on_btnRaspOk_clicked();
    void on_btnJournalOk_clicked();
    void createJournal();
    void createTableMark(int index);
    void uploadFinish();
    void onAddLessonFinish();
    void onChangeMarkFinish();
    void onChangeDateFinish();
    void onDeleteLesson();
    void on_comboBoxClass_currentIndexChanged(int index);
    void on_btnAddLess_clicked();
    void on_tableJournal_cellDoubleClicked(int row, int column);
    void on_comboBoxClassStudnet_currentIndexChanged(int index);
    void on_btnStudentOk_clicked();
    void onAboutStudent();
    void infoStudent(QString id);
    void on_btnTeacherOk_clicked();
    void on_btnPrep_clicked();
    void on_btnTeacher_clicked();
    void on_btnPrepStudent_clicked();
    void on_btnSubject_clicked();
    void on_tableTeacherSubject_cellDoubleClicked(int row);

    void on_btnRating_clicked();

private:
    Ui::Form *ui;
    bool login = false;
    AddLesson *addLesson;
    ChangeMark *changeMark;
    ChangeDate *changeDate;
    StudentAbout *studentAbout;
    Rating *rating;
    Ftp *ftp;
    DBHelper *dbHelper;
    QString titleEng, group;
    int rowM, columnM, indexHeaderDate, indexHeaderName;
    QString *id, *theme, *DZ, *studentId;
    vector<Subject> subjects;
    vector<QString> teacherIdRasp;
    vector <pair<QString, QString>>  prepSubjects;
    Student *student;
    QString teacherJournal, teacherStudDrv, teacherStud, clas;
    void closeEvent(QCloseEvent *event);
};


#endif // FORM_H
