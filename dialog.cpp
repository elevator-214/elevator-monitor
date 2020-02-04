#include "dialog.h"
#include "ui_dialog.h"

#include<iostream>

using namespace std;

int value_pp1_x = 105;
int value_pp1_y = 30;
int value_pp1_width = 350;
int value_pp1_height = 330;
int value_pp1_flowline = 220;

bool SetRoi_pp1_flag = false;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->lineEdit->setText(QString::number(value_pp1_x,10));
    ui->lineEdit_2->setText(QString::number(value_pp1_y,10));
    ui->lineEdit_3->setText(QString::number(value_pp1_width,10));
    ui->lineEdit_4->setText(QString::number(value_pp1_height,10));
    ui->lineEdit_5->setText(QString::number(value_pp1_flowline,10));
}

Dialog::~Dialog()
{
    delete ui;
}
void Dialog::load_text()
{

    ui->lineEdit->setText(QString::number(value_pp1_x,10));
    ui->lineEdit_2->setText(QString::number(value_pp1_y,10));
    ui->lineEdit_3->setText(QString::number(value_pp1_width,10));
    ui->lineEdit_4->setText(QString::number(value_pp1_height,10));
    ui->lineEdit_5->setText(QString::number(value_pp1_flowline,10));
}
void Dialog::on_pushButton_clicked()
{
    bool ok;
    QString valueStr;

    valueStr=ui->lineEdit->text();
    value_pp1_x = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_2->text();
    value_pp1_y = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_3->text();
    value_pp1_width = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_4->text();
    value_pp1_height = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_5->text();
    value_pp1_flowline = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    SetRoi_pp1_flag = true;
}
