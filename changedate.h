#ifndef CHANGEDATE_H
#define CHANGEDATE_H

#include <QDialog>

namespace Ui {
class ChangeDate;
}

class ChangeDate : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeDate(QWidget *parent = 0);
    ~ChangeDate();
    QString getChangedDate();
    QString getChangedTheme();
    QString getChangedDZ();
    int getChangedTimestamp();
signals:
    void onChangeDate();
    void onDeleteLesson();
private slots:
    void recieveLessonInfo(QString date, QString theme, QString DZ, bool login);
    void on_btnChangeDateNo_clicked();
    void on_btnChangeDateYes_clicked();

    void on_btnDelete_clicked();

private:
    Ui::ChangeDate *ui;
    QString date, theme, DZ;
    int timestamp;
};

#endif // CHANGEDATE_H
