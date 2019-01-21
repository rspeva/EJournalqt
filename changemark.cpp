#include "changemark.h"
#include "ui_changemark.h"

ChangeMark::ChangeMark(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeMark)
{
    ui->setupUi(this);
}

ChangeMark::~ChangeMark()
{
    delete ui;
}

void ChangeMark::on_btnChangeMarkNo_clicked()
{
    this->close();
}

void ChangeMark::on_btnChangeMarkYes_clicked()
{
    mark = ui->comboBoxMark->currentText();
    emit onChangeMark();
    ui->comboBoxMark->setCurrentIndex(0);
    this->close();
}

QString ChangeMark::getMark(){
    return mark;
}
