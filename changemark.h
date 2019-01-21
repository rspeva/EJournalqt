#ifndef CHANGEMARK_H
#define CHANGEMARK_H

#include <QDialog>

namespace Ui {
class ChangeMark;
}

class ChangeMark : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeMark(QWidget *parent = 0);
    ~ChangeMark();
    QString getMark();
signals:
    void onChangeMark();
private slots:
    void on_btnChangeMarkNo_clicked();
    void on_btnChangeMarkYes_clicked();

private:
    Ui::ChangeMark *ui;
    QString mark;
};

#endif // CHANGEMARK_H
