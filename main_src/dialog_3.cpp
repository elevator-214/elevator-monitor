#include "dialog_3.h"
#include "ui_dialog_3.h"

int value_pp2_x = 260;
int value_pp2_y = 10;
int value_pp2_width = 240;
int value_pp2_height = 460;

bool SetRoi_pp2_flag = false;

Dialog_3::Dialog_3(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_3)
{
    ui->setupUi(this);

    ui->lineEdit->setText(QString::number(value_pp2_x,10));
    ui->lineEdit_2->setText(QString::number(value_pp2_y,10));
    ui->lineEdit_3->setText(QString::number(value_pp2_width,10));
    ui->lineEdit_4->setText(QString::number(value_pp2_height,10));
}

Dialog_3::~Dialog_3()
{
    delete ui;
}

void Dialog_3::on_pushButton_clicked()
{
    bool ok;
    QString valueStr;

    valueStr=ui->lineEdit->text();
    value_pp2_x = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_2->text();
    value_pp2_y = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_3->text();
    value_pp2_width = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_4->text();
    value_pp2_height = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    SetRoi_pp2_flag = true;
}
