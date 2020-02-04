#include "dialog_2.h"
#include "ui_dialog_2.h"

int value_th_x = 100;
int value_th_y = 10;
int value_th_width = 360;
int value_th_height = 340;

bool SetRoi_th_flag = false;

Dialog_2::Dialog_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_2)
{
    ui->setupUi(this);

    ui->lineEdit->setText(QString::number(value_th_x,10));
    ui->lineEdit_2->setText(QString::number(value_th_y,10));
    ui->lineEdit_3->setText(QString::number(value_th_width,10));
    ui->lineEdit_4->setText(QString::number(value_th_height,10));
}

Dialog_2::~Dialog_2()
{
    delete ui;
}
void Dialog_2::load_text()
{

    ui->lineEdit->setText(QString::number(value_th_x,10));
    ui->lineEdit_2->setText(QString::number(value_th_y,10));
    ui->lineEdit_3->setText(QString::number(value_th_width,10));
    ui->lineEdit_4->setText(QString::number(value_th_height,10));
}
void Dialog_2::on_pushButton_clicked()
{
    bool ok;
    QString valueStr;

    valueStr=ui->lineEdit->text();
    value_th_x = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_2->text();
    value_th_y = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_3->text();
    value_th_width = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_4->text();
    value_th_height = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    SetRoi_th_flag = true;
}
