#include "rating.h"
#include "ui_rating.h"
#include "QDebug"

Rating::Rating(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Rating)
{
    ui->setupUi(this);
}

Rating::~Rating()
{
    delete ui;
}

void Rating::recieveInfo(QString clas, QString title, QString titleEng)
{
    this->setWindowTitle("Рейтинг " + clas + " | " + title);
    this->clas = clas;
    this->titleEng = titleEng;

    ui->etAmtSurname->clear();
    ui->etActSurname->clear();
    ui->etAvrMaxSurname->clear();
    ui->etAvrMinSurname->clear();
    ui->etMissSurname->clear();
    ui->etAmt->clear();
    ui->etAct->clear();
    ui->etAvrMax->clear();
    ui->etAvrMin->clear();
    ui->etMiss->clear();

    fillVectors();
}

void Rating::on_btnChangeMarkYes_clicked()
{
    this->close();
}

void Rating::fillVectors()
{
    id.clear();
    surname.clear();
    lessAmt.clear();
    miss.clear();
    amt.clear();
    avr.clear();
    act.clear();
    QSqlQuery query = dbHelper->makeQuery("SELECT id, Surname FROM STUDENTS_" + clas + " ORDER BY Surname");
    while (query.next()){
        id.push_back(query.value(0).toString());
        surname.push_back(query.value(1).toString());
    }
    for (int i = 0; i < id.size(); i++)
    {
        subject = new Subject();
        subject->getSubject(id[i],titleEng);
        amt.push_back(make_pair(surname[i],subject->getAmt()));
        avr.push_back(make_pair(surname[i],subject->getAvr()));
        if ((subject->getMarkAmt() > 0) && (subject->getAmt() > 0))
        miss.push_back(make_pair(surname[i],(double)subject->getMiss() / subject->getMarkAmt() * 100));
        if ((subject->getMarkAmt() > 0) && (subject->getAmt() > 0))
        act.push_back(make_pair(surname[i],(double)subject->getAmt() / subject->getMarkAmt() * 100));
    }

    calcAmt();
    if (act.size()>0)
    calcAct();
    calcAvr();
    if (miss.size()>0)
    calcMiss();

}

void Rating::calcAmt()
{
    //select sort
    for (int i = 0; i < amt.size(); i++)
    {
        int minValueIndex = i;
        for (int j = i + 1; j < amt.size(); j++)
            if (amt[j].second < amt[minValueIndex].second)
                minValueIndex = j;
        swap (amt[i], amt[minValueIndex]);
    }

    if (amt[amt.size()-1].second > 0)
    {
        ui->etAmtSurname->setText(amt[amt.size()-1].first);
        ui->etAmt->setText(QString::number(amt[amt.size()-1].second));
    }
}

void Rating::calcAct()
{
    //shell sort
    pair<QString, double> temp;
    int step = act.size() / 2;
    while (step > 0)
    {
        for (int i = 0; i < (act.size() - step); i++)
        {
            int j = i;
            while (j >= 0 && act[j].second > act[j + step].second)
            {
                temp = act[j];
                act[j] = act[j + step];
                act[j + step] = temp;
                j--;
            }
        }
    step = step / 2;
    }

   if (act[act.size()-1].second > 0)
   {
        ui->etActSurname->setText(act[act.size()-1].first);
        ui->etAct->setText(QString::number(act[act.size()-1].second, 'f', 0) + "%");
   }
}

void Rating::calcAvr()
{
    //insert sort
    pair<QString, double> temp;
    for (int i = 0; i < avr.size(); i++)
    {
        temp = avr[i];
        int j = i - 1;
        while(j >= 0 && avr[j].second > temp.second)
        {
            avr[j + 1] = avr[j];
            j--;
        }
        avr[j + 1] = temp;
    }

    if (avr[avr.size()-1].second > 0)
    {
        ui->etAvrMaxSurname->setText(avr[avr.size()-1].first);
        ui->etAvrMax->setText(QString::number(avr[avr.size()-1].second));
        int i;
        for (i = 0; i < avr.size(); i++)
            if (avr[i].second > 0)
                break;
        ui->etAvrMinSurname->setText(avr[i].first);
        ui->etAvrMin->setText(QString::number(avr[i].second));
    }



}

void Rating::calcMiss()
{
    //bubble sort
    pair<QString, double> temp;
    for (int i = 0; i < miss.size() - 1; i++)
    {
        for (int j = 0; j < miss.size() - i - 1; j++)
        {
            if (miss[j].second > miss[j + 1].second)
            {
                temp = miss[j];
                miss[j] = miss[j + 1];
                miss[j+1] = temp;
            }
        }
    }

    if (miss[miss.size()-1].second > 0)
    {
        ui->etMissSurname->setText(miss[miss.size()-1].first);
        ui->etMiss->setText(QString::number(miss[miss.size()-1].second, 'f', 0) + "%");
    }
}
