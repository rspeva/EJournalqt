#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <form.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void sendLogin(bool login);
private slots:
    void downloadSuccess();
    void downloadFailed();
    void on_btnPrep_clicked();
    void on_btnPassOk_clicked();
    void on_etPass_textEdited(const QString &arg1);
    void on_etPass_returnPressed();
    void on_btnStud_clicked();
    void on_btnRefresh_clicked();

private:
    Ui::MainWindow *ui;
    Form *form;
    Ftp *ftp;
    DBHelper *dbHelper;
};

#endif // MAINWINDOW_H
