#include "userwindow.h"
#include "ui_userwindow.h"
#include <QMessageBox>
#include <QtGui>

bool flag = false;
QString password = "1";

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->btnRefresh->setVisible(false);
    ui->etPass->setVisible(false);
    ui->btnPassOk->setVisible(false);
    ui->lbl_load->setAlignment(Qt::AlignCenter);
    ui->lbl_load->setText("Обновление БД");
    ftp = new Ftp();
    connect(ftp, &Ftp::onReadyDownload, this, &MainWindow::downloadSuccess);
    connect(ftp, &Ftp::onErrorDownload, this, &MainWindow::downloadFailed);
    ftp->downloadDB();

    QMovie *movie = new QMovie(":/img/drawable/load.gif");
    ui->lbl->setMovie(movie);
    ui->lbl->show();
    movie->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btnPrep_clicked()
{
    flag = !flag;
    if (flag) {
    ui->etPass->setVisible(true);
    ui->btnPassOk->setEnabled(false);
    ui->btnPassOk->setVisible(true);
    } else {
    ui->etPass->setVisible(false);
    ui->btnPassOk->setVisible(false);
    ui->etPass->setText("");
    }
    ui->etPass->setPlaceholderText(QApplication::translate("Form", "Введите пароль...", 0));

}

void MainWindow::on_btnPassOk_clicked()
{
    if (ui->etPass->text()!=password) {
        QMessageBox msgBox;
         msgBox.setText("Неправильный пароль!       ");
         msgBox.exec();
    } else {
        form = new Form();
        connect(this, SIGNAL(sendLogin(bool)), form , SLOT(recieveLogin(bool)));
        emit sendLogin(true);
        form->showMaximized();
        this->close();
    }
}



void MainWindow::on_etPass_textEdited(const QString &arg1)
{
    if (arg1 != "")
    ui->btnPassOk->setEnabled(true);
    else     ui->btnPassOk->setEnabled(false);

}

void MainWindow::on_etPass_returnPressed()
{

    if (ui->etPass->text()!=password && ui->etPass->text()!="")
    {
        QMessageBox msgBox;
         msgBox.setText("Неправильный пароль!       ");
         msgBox.exec();
    } else {
        form = new Form();
        connect(this, SIGNAL(sendLogin(bool)), form , SLOT(recieveLogin(bool)));
        emit sendLogin(true);
        form->showMaximized();
        this->close();
    }
}

void MainWindow::on_btnStud_clicked()
{
    form = new Form();
    connect(this, SIGNAL(sendLogin(bool)), form , SLOT(recieveLogin(bool)));
    emit sendLogin(false);
    form->showMaximized();
    this->close();
}

void MainWindow::downloadSuccess()
{
    dbHelper = new DBHelper();
    dbHelper->openDB();
    ui->frame->setVisible(false);
}

void MainWindow::downloadFailed()
{
    ui->lbl_load->setText("Не удалось обновить БД \n Проверьте подключение к интернету");
    ui->lbl->setVisible(false);
    ui->btnRefresh->setVisible(true);
}

void MainWindow::on_btnRefresh_clicked()
{
    ui->btnRefresh->setVisible(false);
    ui->lbl->setVisible(true);
    ui->lbl_load->setText("Обновление БД");
    ftp->downloadDB();

}
