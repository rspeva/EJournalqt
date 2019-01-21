#ifndef RATING_H
#define RATING_H

#include <QDialog>
#include "vector"
#include "subject.h"
#include "dbhelper.h"
using namespace std;

namespace Ui {
class Rating;
}

class Rating : public QDialog
{
    Q_OBJECT

public:
    explicit Rating(QWidget *parent = 0);
    ~Rating();
private slots:
    void recieveInfo(QString clas, QString title, QString titleEng);
    void on_btnChangeMarkYes_clicked();
    void fillVectors();
    void calcAmt();
    void calcAct();
    void calcAvr();
    void calcMiss();
private:
    Ui::Rating *ui;
    Subject *subject;
    DBHelper *dbHelper;
    QString clas, titleEng;
    vector <QString> id, surname;
    vector <pair <QString, int>> lessAmt;
    vector <pair <QString, double>> miss;
    vector <pair <QString, int>> amt;
    vector <pair <QString, double>> avr;
    vector <pair <QString, double>> act;
};

#endif // RATING_H
