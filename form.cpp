#include "form.h"
#include "ui_form.h"
#include "userwindow.h"

#include <QMovie>


Form::Form(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->frame->setVisible(false);
    ftp = new Ftp();
    addLesson = new AddLesson();
    changeMark = new ChangeMark();
    changeDate = new ChangeDate();
    studentAbout = new StudentAbout();
    rating = new Rating();
    connect(ftp, &Ftp::onReadyUpload, this, &Form::uploadFinish);
    connect(addLesson, &AddLesson::onAddLesson, this, &Form::onAddLessonFinish);
    connect(changeMark, &ChangeMark::onChangeMark, this, &Form::onChangeMarkFinish);
    connect(changeDate, &ChangeDate::onChangeDate, this, &Form::onChangeDateFinish);
    connect(changeDate, &ChangeDate::onDeleteLesson, this, &Form::onDeleteLesson);
    connect(studentAbout, &StudentAbout::onStudentAbout, this, &Form::onAboutStudent);

    connect(this, SIGNAL(sendRatingInfo(QString,QString,QString)),rating, SLOT(recieveInfo(QString,QString,QString)));
    connect(this, SIGNAL(sendLessonInfo(QString, QString, QString, bool)), changeDate, SLOT(recieveLessonInfo(QString,QString,QString, bool)));
    connect(this, SIGNAL(sendStudentInfo(Student)), studentAbout, SLOT(recieveStudentInfo(Student)));
    for (int i = 0; i < 9; i++){
    ui->comboBoxClass->addItem(QString::number(i+1)+"A");
    ui->comboBoxClass->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    ui->comboBoxClassStudnet->addItem(QString::number(i+1)+"A");
    ui->comboBoxClassStudnet->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
}
    QHeaderView *headerDate = qobject_cast<QTableView *>(ui->tableJournal)->horizontalHeader();
                connect(headerDate, &QHeaderView::sectionDoubleClicked, [this](int logicalIndex){
        oldDate = ui->tableJournal->horizontalHeaderItem(logicalIndex)->text();
        indexHeaderDate = logicalIndex;;
        changeDate->setWindowFlags(changeDate->windowFlags() & ~Qt::WindowContextHelpButtonHint);
        changeDate->show();
        emit sendLessonInfo(oldDate, theme[indexHeaderDate], DZ[indexHeaderDate], login);
    });


    QHeaderView *headerName = qobject_cast<QTableView *>(ui->tableJournal)->verticalHeader();
                connect(headerName, &QHeaderView::sectionDoubleClicked, [this](int logicalIndex){
        indexHeaderName = logicalIndex;
        emit sendStudentInfo(student[indexHeaderName]);
        studentAbout->setWindowFlags(studentAbout->windowFlags() & ~Qt::WindowContextHelpButtonHint);
        studentAbout->show();
    });

    ui->tableMark->verticalHeader()->setFixedWidth(97);
    ui->tableMark->setColumnWidth(0,88);
    ui->tableMark->setColumnWidth(1,307);
    ui->tableMark->setColumnWidth(2,307);
    ui->tableAvrMark->verticalHeader()->setFixedWidth(123);

    QSqlQuery query = dbHelper->makeQuery("SELECT id, Surname FROM TEACHERS");
    while (query.next())
    {
        teacherIdRasp.push_back(query.value(0).toString());
        ui->comboBoxTeacher->addItem(query.value(1).toString());
    }



}
Form::~Form()
{
    delete ui;
}

void Form::recieveLogin(bool login)
{
    this->login = login;
    if (!login)
        ui->btnAddLess->setVisible(false);
}


void Form::on_btnJournalOk_clicked()
{
    ui->tableJournal->clear();
    QSqlQuery query = dbHelper->makeQuery("SELECT Teacher, TitleEng, id FROM SUBJECTS WHERE Groups = '" + ui->comboBoxClass->currentText() + "' AND Title = '" + ui->comboBoxSubject->currentText() + "'");
    query.first();
    teacherJournal = query.value(2).toString();
    teacherJournal.remove(0,2);
    ui->btnPrep->setText(query.value(0).toString());
    titleEng = query.value(1).toString();
    group = ui->comboBoxClass->currentText();
    ui->btnPrep->setEnabled(true);
    createJournal();
    ui->btnAddLess->setEnabled(true);
}

void Form::on_radioButton_class_clicked()
{
    ui->RaspComboBox->clear();
    for (int i = 1; i < 10; i++){
    ui->RaspComboBox->addItem(QString::number(i) + "A");
    ui->btnRaspOk->setEnabled(true);
    }
}

void Form::on_radioButton_prep_clicked()
{
    ui->RaspComboBox->clear();
    QSqlQuery query = dbHelper->makeQuery("SELECT Surname FROM TEACHERS");
    while (query.next())
        ui->RaspComboBox->addItem(query.value(0).toString());

    ui->btnRaspOk->setEnabled(true);

}

void Form::on_btnRaspOk_clicked()
{
    ui->tableRasp->clear();
    QString days[] = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница"};
    for (int i = 0; i < 5; i++)
    {
        QTableWidgetItem* day = new QTableWidgetItem;
        day->setText(days[i]);
        day->setTextAlignment(Qt::AlignCenter);
        ui->tableRasp->setHorizontalHeaderItem(i, day);
    }
    if (ui->radioButton_class->isChecked())
    {
        QSqlQuery query = dbHelper->makeQuery("SELECT Days, Lesson, Title, TitleEng, idTeacher FROM TIMETABLE WHERE Clas = '"
                                              + ui->RaspComboBox->currentText() + "' ORDER BY Days");
        while (query.next())
        {
            QTableWidgetItem* item = new QTableWidgetItem;
            QSqlQuery query1 = dbHelper->makeQuery("SELECT Surname, Cabinet FROM TEACHERS WHERE id = '"
                                                  + query.value(4).toString() + "'");
            query1.first();
            item->setText(query.value(2).toString() + "\n " + query1.value(0).toString() + "\n каб. " + query1.value(1).toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableRasp->setItem(query.value(1).toInt(), query.value(0).toInt(), item);
        }
    }

    if (ui->radioButton_prep->isChecked())
    {
        QSqlQuery query = dbHelper->makeQuery("SELECT Clas, Days, Lesson, Title, TitleEng FROM TIMETABLE WHERE idTeacher = '"
                                              + teacherIdRasp[ui->RaspComboBox->currentIndex()] + "' ORDER BY Days");
        while (query.next())
        {
            QTableWidgetItem* item = new QTableWidgetItem;
            QSqlQuery query1 = dbHelper->makeQuery("SELECT Cabinet FROM TEACHERS WHERE id = '"
                                                  + teacherIdRasp[ui->RaspComboBox->currentIndex()] + "'");
            query1.first();
            item->setText(query.value(3).toString() + "\n " + query.value(0).toString() + "\n каб. " + query1.value(0).toString());
            item->setTextAlignment(Qt::AlignCenter);
            ui->tableRasp->setItem(query.value(2).toInt(), query.value(1).toInt(), item);
        }
    }
}

void Form::closeEvent(QCloseEvent *event)
{
    if (login)
    {
        event->ignore();
        QMessageBox ms;
        ms.setStyleSheet("QLabel {font: 75 12pt 'Segoe Print' ;}");
        ms.setIcon(QMessageBox::Question);
        QAbstractButton *yes = ms.addButton("Да",QMessageBox::YesRole);
        QAbstractButton *cancel = ms.addButton("Отмена",QMessageBox::YesRole);
        QAbstractButton *no = ms.addButton("Нет",QMessageBox::NoRole);

        yes->setStyleSheet("QPushButton{ color: white;"
                           "background-color: qlineargradient(spread:repeat, x1:0.511, y1:0, x2:0.506, y2:1, stop:0 rgba(53, 200, 102, 255), stop:1 rgba(4, 142, 50, 255));"
                           "border-radius:8px;"
                           "border-style: solid;"
                           "border-color: rgb(147, 147, 147);"
                           "border-width: 1px;"
                           "font: 75 12pt 'Segoe Print' ; }"
                           "QPushButton:hover{"
                           "background-color: rgb(7, 193, 82);"
                           "}"
                           "QPushButton:pressed{"
                           "background-color: qlineargradient(spread:repeat, x1:0.511, y1:0, x2:0.506, y2:1, stop:0 rgba(53, 200, 102, 255), stop:1 rgba(4, 142, 50, 255));"
                           "}");
        yes->setMinimumWidth(100);
        yes->setMinimumHeight(30);

        cancel->setStyleSheet("QPushButton{ color: white;"
                           "background-color: qlineargradient(spread:repeat, x1:0.511, y1:0, x2:0.506, y2:1, stop:0 rgba(53, 200, 102, 255), stop:1 rgba(4, 142, 50, 255));"
                           "border-radius:8px;"
                           "border-style: solid;"
                           "border-color: rgb(147, 147, 147);"
                           "border-width: 1px;"
                           "font: 75 12pt 'Segoe Print' ; }"
                           "QPushButton:hover{"
                           "background-color: rgb(7, 193, 82);"
                           "}"
                           "QPushButton:pressed{"
                           "background-color: qlineargradient(spread:repeat, x1:0.511, y1:0, x2:0.506, y2:1, stop:0 rgba(53, 200, 102, 255), stop:1 rgba(4, 142, 50, 255));"
                           "}");
        cancel->setMinimumWidth(100);
        cancel->setMinimumHeight(30);

        no->setStyleSheet("QPushButton{ color: white;"
                           "background-color: qlineargradient(spread:repeat, x1:0.511, y1:0, x2:0.506, y2:1, stop:0 rgba(238, 48, 48, 255), stop:1 rgba(192, 1, 1, 255));"
                           "border-radius:8px;"
                           "border-style: solid;"
                           "border-color: rgb(147, 147, 147);"
                           "border-width: 1px;"
                           "font: 75 12pt 'Segoe Print' ; }"
                           "QPushButton:hover{"
                           "background-color: rgb(255, 55, 55);"
                           "}"
                           "QPushButton:pressed{"
                           "background-color: qlineargradient(spread:repeat, x1:0.511, y1:0, x2:0.506, y2:1, stop:0 rgba(238, 48, 48, 255), stop:1 rgba(192, 1, 1, 255));"
                           "}");
        no->setMinimumWidth(100);
        no->setMinimumHeight(30);



        ms.setText("Сохранить изменения?");
        ms.exec();
        if(ms.clickedButton() == no) event->accept();
        if(ms.clickedButton() == yes)
        {
            ui->frame->setVisible(true);
            ui->lbl_load_2->setAlignment(Qt::AlignCenter);
            ui->lbl_load_2->setText("Сохранение БД");
            QMovie *movie = new QMovie(":/img/drawable/load.gif");
            ui->lbl_2->setMovie(movie);
            ui->lbl_2->show();
            movie->start();
            ftp->uploadDB();
        }
    }
    QApplication::closeAllWindows();
}


void Form::uploadFinish() {
    this->destroy();
}

void Form::createJournal()
{
    ui->tableJournal->setColumnCount(0);
    QSqlQuery query = dbHelper->makeQuery("SELECT id, Surname, Name, Patronymic, Adress FROM STUDENTS_" + ui->comboBoxClass->currentText()+ " ORDER BY Surname");
    query.last();
    int studentsAmt = query.at()+1;
    student = new Student[studentsAmt];
    ui->tableJournal->setRowCount(studentsAmt);
    id = new QString[studentsAmt];
    int i = 0;
    query.first();
    query.previous();
    while (query.next())
       {
       id[i] = query.value(0).toString();
       student[i].setId(query.value(0).toString());
       student[i].setSurname(query.value(1).toString());
       student[i].setName(query.value(2).toString());
       student[i].setPatronymic(query.value(3).toString());
       student[i].setAdress(query.value(4).toString());
       QTableWidgetItem* surname = new QTableWidgetItem;
       surname->setText(student[i].getSurname() + " " + student[i].getName()[0] +"." );
       surname->setTextAlignment(Qt::AlignCenter);
       ui->tableJournal->setVerticalHeaderItem(i, surname);
       i++;
       }

    try{
    query = dbHelper->makeQuery("SELECT * FROM S_" + group + "_" + titleEng + " ORDER BY Timestamp");
    query.last();
    int lessonsAmt = query.at()+1;
    ui->tableJournal->setColumnCount(lessonsAmt);
    theme = new QString[255];
    DZ = new QString[255];
    i = 0;
    query.first();
    query.previous();
    while (query.next())
       {
       theme[i] = query.value(2).toString();
       DZ[i] = query.value(3).toString();
       QTableWidgetItem* date = new QTableWidgetItem;
       date->setText(query.value(1).toString());
       date->setTextAlignment(Qt::AlignCenter);
       ui->tableJournal->setHorizontalHeaderItem(i, date);
       for (int j = 0; j < studentsAmt; j++)
       {
           QTableWidgetItem* m = new QTableWidgetItem;
           m->setTextAlignment(Qt::AlignCenter);
           m->setText(query.value(4+j).toString());
           ui->tableJournal->setItem(j,i,m);
       }
       i++;
       }
    emit sendRatingInfo(group, ui->comboBoxSubject->currentText(), titleEng);
    ui->btnRating->setEnabled(true);
    } catch (std::exception & e)
    {
        QMessageBox msgBox;
        msgBox.setText(e.what());
        msgBox.exec();
    }
}


void Form::on_comboBoxClass_currentIndexChanged(int index)
{
    index++;
    ui->comboBoxSubject->clear();
    QSqlQuery query = dbHelper->makeQuery("SELECT Title, TitleEng FROM SUBJECTS WHERE Groups = '" + QString::number(index) + "A' ORDER BY Title");
    while (query.next())
        ui->comboBoxSubject->addItem(query.value(0).toString());
}

void Form::on_btnAddLess_clicked()
{
    addLesson->setWindowFlags(addLesson->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    addLesson->show();
}

void Form::onAddLessonFinish()
{
  dbHelper->makeQuery("INSERT INTO S_" + group + "_" + titleEng + " (Timestamp, Date, Theme, DZ) VALUES ('"
                      + QString::number(addLesson->getTimestamp()) + "', '" + addLesson->getDate() + "', '"
                      + addLesson->getTheme() +  "', '" + addLesson->getDZ() + "')");
  ui->tableJournal->insertColumn(ui->tableJournal->columnCount());
  QTableWidgetItem* date = new QTableWidgetItem;
  date->setText(addLesson->getDate());
  date->setTextAlignment(Qt::AlignCenter);
  ui->tableJournal->setHorizontalHeaderItem(ui->tableJournal->columnCount()-1,date);
  theme[ui->tableJournal->columnCount()-1] = addLesson->getTheme();
  DZ[ui->tableJournal->columnCount()-1] = addLesson->getDZ();
}

void Form::on_tableJournal_cellDoubleClicked(int row, int column)
{
    if (login){
    rowM = row;
    columnM = column;
    changeMark->setWindowFlags(changeMark->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    changeMark->show();
    }
}
void Form::onChangeMarkFinish()
{
    QTableWidgetItem* m = new QTableWidgetItem;
    m->setTextAlignment(Qt::AlignCenter);
    m->setText(changeMark->getMark());
    ui->tableJournal->setItem(rowM, columnM, m);
    dbHelper->makeQuery("UPDATE S_" + group + "_" + titleEng + " SET " + id[rowM] + " = '" + changeMark->getMark()
                       + "' WHERE Date = '" + ui->tableJournal->horizontalHeaderItem(columnM)->text() + "'");
}

void Form::onChangeDateFinish()
{
    QTableWidgetItem* date = new QTableWidgetItem;
    date->setText(changeDate->getChangedDate());
    ui->tableJournal->setHorizontalHeaderItem(indexHeaderDate, date);
    theme[indexHeaderDate] = changeDate->getChangedTheme();
    DZ[indexHeaderDate] = changeDate->getChangedDZ();
    dbHelper->makeQuery("UPDATE S_" + group + "_" + titleEng + " SET Timestamp = '" + QString::number(changeDate->getChangedTimestamp())
                        + "', Date = '" + changeDate->getChangedDate() + "', Theme = '"
                        + theme[indexHeaderDate] + "', DZ = '" + DZ[indexHeaderDate] + "' WHERE Date = '" + oldDate + "'");
}

void Form::onDeleteLesson()
{
    dbHelper->makeQuery("DELETE FROM S_" + group + "_" + titleEng + " WHERE Date ='" + oldDate + "'");
    on_btnJournalOk_clicked();
}

void Form::on_comboBoxClassStudnet_currentIndexChanged(int index)
{
    index++;
    ui->comboBoxStuent->clear();
    QSqlQuery query = dbHelper->makeQuery("SELECT id, Surname FROM STUDENTS_" + QString::number(index) + "A ORDER BY Surname");
    query.last();
    studentId = new QString[query.at()+1];
    int i = 0;
    query.first();
    query.previous();
    while (query.next()){
        studentId[i] = query.value(0).toString();
        ui->comboBoxStuent->addItem(query.value(1).toString());
        i++;
    }
}

void Form::on_btnStudentOk_clicked()
{
    infoStudent(studentId[ui->comboBoxStuent->currentIndex()]);
}

void Form::onAboutStudent()
{
    QString id = studentAbout->getStudentId();
    QString clas(id[2]);
    ui->tabWidget->setCurrentIndex(2);
    ui->comboBoxClassStudnet->setCurrentIndex(clas.toInt()-1);
    QString index = id;
    ui->comboBoxStuent->setCurrentIndex(index.remove(0,3).toInt()-1);
    infoStudent(id);
}

void Form::infoStudent(QString id)
{   subjects.clear();

    Student stud;
    stud.setAll(id);
    clas = QString(stud.getId()[2]);
    ui->etId->setText(stud.getId());
    ui->etSurname->setText(stud.getSurname());
    ui->etName->setText(stud.getName());
    ui->etPatronymic->setText(stud.getPatronymic());
    ui->etAdress->setText(stud.getAdress());
    QSqlQuery query = dbHelper->makeQuery("SELECT Surname, Name, Patronymic, id FROM TEACHERS WHERE Clas='"
                                          + clas + "A'");
    query.first();
    teacherStudDrv = query.value(3).toString();
    teacherStudDrv.remove(0,2);
    ui->btnPrepStudent->setEnabled(true);
    ui->btnPrepStudent->setText(query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString());
    ui->tableAvrMark->setRowCount(0);
    query = dbHelper->makeQuery("SELECT TitleEng, Title, Teacher, id FROM SUBJECTS WHERE Groups='" + clas + "A' ORDER BY Title");
    query.last();
    int lessonsAmt = query.at()+1;
    ui->tableAvrMark->setRowCount(lessonsAmt);
    query.first();
    query.previous();
    int j = 0;
    Subject subject;
    while (query.next())
       {
       subject.setClas(clas+"A");
       subject.setTitleEng(query.value(0).toString());
       subject.setTitle(query.value(1).toString());
       subject.setTeacher(query.value(2).toString());
       subject.setTeacherId(query.value(3).toString());
       subjects.push_back(subject);
       j++;
       }
    int missSum = 0, miss, amt, avrAmt = 0;
    double avrSum = 0, avr;
    for (int i = 0; i < lessonsAmt; i++)
    {
        QTableWidgetItem* lesson = new QTableWidgetItem;
        lesson->setText(subjects[i].getTitle());
        lesson->setTextAlignment(Qt::AlignCenter);
        ui->tableAvrMark->setVerticalHeaderItem(i, lesson);

        subjects[i].getSubject(stud.getId(),subjects[i].getTitleEng());

        miss = subjects[i].getMiss();
        if (miss == -1)
            ui->tableAvrMark->setItem(i,0,new QTableWidgetItem(""));
        else{
            QTableWidgetItem* m = new QTableWidgetItem;
            m->setText(QString::number(miss));
            m->setTextAlignment(Qt::AlignCenter);
            ui->tableAvrMark->setItem(i,0,m);
            missSum += miss;
        }

        amt = subjects[i].getAmt();
        if (amt == -1)
            ui->tableAvrMark->setItem(i,1,new QTableWidgetItem(""));
        else{
            QTableWidgetItem* a = new QTableWidgetItem;
            a->setText(QString::number(amt));
            a->setTextAlignment(Qt::AlignCenter);
            ui->tableAvrMark->setItem(i,1,a);
        }

        avr = subjects[i].getAvr();
        if (avr == -1)
            ui->tableAvrMark->setItem(i,2,new QTableWidgetItem(""));
        else{
            QTableWidgetItem* av = new QTableWidgetItem;
            av->setText(QString::number(avr).left(4));
            av->setTextAlignment(Qt::AlignCenter);
            ui->tableAvrMark->setItem(i,2,av);
            if (avr != 0){
                avrSum += avr;
                avrAmt++;
            }
        }

    }
    ui->etCommonMiss->setText(QString::number(missSum));
    ui->etCommonAvr->setText(QString::number((double)avrSum/avrAmt));
    createTableMark(0);
    QHeaderView *headerSubject = qobject_cast<QTableView *>(ui->tableAvrMark)->verticalHeader();
                connect(headerSubject, &QHeaderView::sectionClicked, [this](int logicalIndex){
            createTableMark(logicalIndex);
    });
}

void Form::createTableMark(int index)
{
    teacherStud = subjects[index].getTeacherId();
    teacherStud.remove(0,2);
    ui->btnSubject->setEnabled(true);
    ui->btnTeacher->setEnabled(true);
    ui->btnSubject->setText(subjects[index].getTitle());
    ui->btnTeacher->setText(subjects[index].getTeacher());
    vector<QString> date = subjects[index].getDate();
    vector<QString> marks = subjects[index].getMarks();
    vector<QString> theme = subjects[index].getTheme();
    vector<QString> dz = subjects[index].getDZ();
    ui->tableMark->setRowCount(date.size());
    for (int i = 0; i < date.size(); i++)
    {
        QTableWidgetItem* lesson = new QTableWidgetItem;
        lesson->setText(date[i]);
        lesson->setTextAlignment(Qt::AlignCenter);
        ui->tableMark->setVerticalHeaderItem(i, lesson);
        QTableWidgetItem* m = new QTableWidgetItem;
        m->setTextAlignment(Qt::AlignCenter);
        m->setText(marks[i]);
        ui->tableMark->setItem(i,0,m);
        QTableWidgetItem* t = new QTableWidgetItem;
        t->setTextAlignment(Qt::AlignCenter);
        t->setText(theme[i]);
        ui->tableMark->setItem(i,1,t);
        QTableWidgetItem* d = new QTableWidgetItem;
        d->setTextAlignment(Qt::AlignCenter);
        d->setText(dz[i]);
        ui->tableMark->setItem(i,2,d);
    }
}



void Form::on_btnTeacherOk_clicked()
{
    prepSubjects.clear();
    QString id = teacherIdRasp[ui->comboBoxTeacher->currentIndex()];
    QSqlQuery query = dbHelper->makeQuery("SELECT Surname, Name, Patronymic, Clas, Cabinet FROM TEACHERS WHERE id= '" + id + "'");
    query.first();
    ui->etIdTeacher->setText(id);
    ui->etSurnameTeacher->setText(query.value(0).toString());
    ui->etNameTeacher->setText(query.value(1).toString());
    ui->etPatronymicTeacher->setText(query.value(2).toString());
    ui->etClasTeacher->setText(query.value(3).toString());
    ui->etCabinetTeacher->setText(query.value(4).toString());

    ui->tableTeacherSubject->setColumnCount(2);

    QTableWidgetItem* item = new QTableWidgetItem;
    item->setText("Класс");
    item->setTextAlignment(Qt::AlignCenter);
    ui->tableTeacherSubject->setHorizontalHeaderItem(0, item);

    QTableWidgetItem* item1 = new QTableWidgetItem;
    item1->setText("Предмет");
    item1->setTextAlignment(Qt::AlignCenter);
    ui->tableTeacherSubject->setHorizontalHeaderItem(1, item1);

    ui->tableTeacherSubject->verticalHeader()->setFixedWidth(20);
    ui->tableTeacherSubject->setColumnWidth(0,163);
    ui->tableTeacherSubject->setColumnWidth(1,330);

    query = dbHelper->makeQuery("SELECT Groups, Title FROM SUBJECTS WHERE id= '" + id + "'");
    query.last();
    int suvbjectsAmt = query.at()+1;
    ui->tableTeacherSubject->setRowCount(suvbjectsAmt);
    query.first();
    query.previous();
    int i = 0;
    while (query.next())
    {
        prepSubjects.push_back(make_pair(query.value(0).toString(),query.value(1).toString()));
        QTableWidgetItem* item = new QTableWidgetItem;
        item->setTextAlignment(Qt::AlignCenter);
        item->setText(query.value(0).toString());
        ui->tableTeacherSubject->setItem(i,0,item);

        QTableWidgetItem* item1 = new QTableWidgetItem;
        item1->setTextAlignment(Qt::AlignCenter);
        item1->setText(query.value(1).toString());
        ui->tableTeacherSubject->setItem(i,1,item1);
        i++;
    }
}

void Form::on_btnPrep_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
    ui->comboBoxTeacher->setCurrentIndex(teacherJournal.toInt()-1);
    on_btnTeacherOk_clicked();
}

void Form::on_btnTeacher_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
    ui->comboBoxTeacher->setCurrentIndex(teacherStud.toInt()-1);
    on_btnTeacherOk_clicked();
}

void Form::on_btnPrepStudent_clicked()
{
    ui->tabWidget->setCurrentIndex(3);
    ui->comboBoxTeacher->setCurrentIndex(teacherStudDrv.toInt()-1);
    on_btnTeacherOk_clicked();
}

void Form::on_btnSubject_clicked()
{
    ui->tabWidget->setCurrentIndex(0);
    ui->comboBoxClass->setCurrentIndex(clas.toInt()-1);
    ui->comboBoxSubject->setCurrentText(ui->btnSubject->text());
    on_btnJournalOk_clicked();
}

void Form::on_tableTeacherSubject_cellDoubleClicked(int row)
{
    ui->tabWidget->setCurrentIndex(0);
    QString cl(prepSubjects[row].first);
    ui->comboBoxClass->setCurrentText(cl);
    ui->comboBoxSubject->setCurrentText(prepSubjects[row].second);
    on_btnJournalOk_clicked();
}

void Form::on_btnRating_clicked()
{
    rating->setWindowFlags(rating->windowFlags() & ~Qt::WindowContextHelpButtonHint);
    rating->show();
}
