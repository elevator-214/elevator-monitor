#include "dialog_5.h"
#include "ui_dialog_5.h"
#include<iostream>
int value_hd2_x = 340;
int value_hd2_y = 0;
int value_hd2_width = 100;
int value_hd2_height = 480;

int value_hd2_p1_out_x = 5;
int value_hd2_p1_out_y = 5;
int value_hd2_p2_out_x = 10;
int value_hd2_p2_out_y = 5;
int value_hd2_p3_out_x = 65;
int value_hd2_p3_out_y = 475;
int value_hd2_p4_out_x = 40;
int value_hd2_p4_out_y = 475;

int value_hd2_p1_in_x = 10;
int value_hd2_p1_in_y = 5;
int value_hd2_p2_in_x = 15;
int value_hd2_p2_in_y = 5;
int value_hd2_p3_in_x = 88;
int value_hd2_p3_in_y = 475;
int value_hd2_p4_in_x = 65;
int value_hd2_p4_in_y = 475;

bool SetRoi_hd2_flag = false;

Dialog_5::Dialog_5(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_5)
{
    ui->setupUi(this);

    ui->lineEdit->setText(QString::number(value_hd2_x,10));
    ui->lineEdit_2->setText(QString::number(value_hd2_y,10));
    ui->lineEdit_3->setText(QString::number(value_hd2_width,10));
    ui->lineEdit_4->setText(QString::number(value_hd2_height,10));

    ui->lineEdit_5->setText(QString::number(value_hd2_p1_in_x,10));
    ui->lineEdit_6->setText(QString::number(value_hd2_p2_in_x,10));
    ui->lineEdit_7->setText(QString::number(value_hd2_p3_in_x,10));
    ui->lineEdit_8->setText(QString::number(value_hd2_p4_in_x,10));
    ui->lineEdit_13->setText(QString::number(value_hd2_p1_in_y,10));
    ui->lineEdit_14->setText(QString::number(value_hd2_p2_in_y,10));
    ui->lineEdit_15->setText(QString::number(value_hd2_p3_in_y,10));
    ui->lineEdit_16->setText(QString::number(value_hd2_p4_in_y,10));

    ui->lineEdit_9->setText(QString::number(value_hd2_p1_out_x,10));
    ui->lineEdit_10->setText(QString::number(value_hd2_p2_out_x,10));
    ui->lineEdit_11->setText(QString::number(value_hd2_p3_out_x,10));
    ui->lineEdit_12->setText(QString::number(value_hd2_p4_out_x,10));
    ui->lineEdit_17->setText(QString::number(value_hd2_p1_out_y,10));
    ui->lineEdit_18->setText(QString::number(value_hd2_p2_out_y,10));
    ui->lineEdit_19->setText(QString::number(value_hd2_p3_out_y,10));
    ui->lineEdit_20->setText(QString::number(value_hd2_p4_out_y,10));
}

Dialog_5::~Dialog_5()
{
    delete ui;
}
void Dialog_5::load_text()
{
    ui->lineEdit->setText(QString::number(value_hd2_x,10));
    ui->lineEdit_2->setText(QString::number(value_hd2_y,10));
    ui->lineEdit_3->setText(QString::number(value_hd2_width,10));
    ui->lineEdit_4->setText(QString::number(value_hd2_height,10));

    ui->lineEdit_5->setText(QString::number(value_hd2_p1_in_x,10));
    ui->lineEdit_6->setText(QString::number(value_hd2_p2_in_x,10));
    ui->lineEdit_7->setText(QString::number(value_hd2_p3_in_x,10));
    ui->lineEdit_8->setText(QString::number(value_hd2_p4_in_x,10));
    ui->lineEdit_13->setText(QString::number(value_hd2_p1_in_y,10));
    ui->lineEdit_14->setText(QString::number(value_hd2_p2_in_y,10));
    ui->lineEdit_15->setText(QString::number(value_hd2_p3_in_y,10));
    ui->lineEdit_16->setText(QString::number(value_hd2_p4_in_y,10));

    ui->lineEdit_9->setText(QString::number(value_hd2_p1_out_x,10));
    ui->lineEdit_10->setText(QString::number(value_hd2_p2_out_x,10));
    ui->lineEdit_11->setText(QString::number(value_hd2_p3_out_x,10));
    ui->lineEdit_12->setText(QString::number(value_hd2_p4_out_x,10));
    ui->lineEdit_17->setText(QString::number(value_hd2_p1_out_y,10));
    ui->lineEdit_18->setText(QString::number(value_hd2_p2_out_y,10));
    ui->lineEdit_19->setText(QString::number(value_hd2_p3_out_y,10));
    ui->lineEdit_20->setText(QString::number(value_hd2_p4_out_y,10));
}
void Dialog_5::on_pushButton_clicked()
{
    bool ok;
    QString valueStr;

    valueStr=ui->lineEdit->text();
    value_hd2_x = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_2->text();
    value_hd2_y = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_3->text();
    value_hd2_width = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_4->text();
    value_hd2_height = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_5->text();
    value_hd2_p1_in_x = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_6->text();
    value_hd2_p2_in_x = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_7->text();
    value_hd2_p3_in_x = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_8->text();
    value_hd2_p4_in_x = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_13->text();
    value_hd2_p1_in_y = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_14->text();
    value_hd2_p2_in_y = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_15->text();
    value_hd2_p3_in_y = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_16->text();
    value_hd2_p4_in_y = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }
    valueStr=ui->lineEdit_9->text();
    value_hd2_p1_out_x = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_10->text();
    value_hd2_p2_out_x = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_11->text();
    value_hd2_p3_out_x = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_12->text();
    value_hd2_p4_out_x = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_17->text();
    value_hd2_p1_out_y = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_18->text();
    value_hd2_p2_out_y = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_19->text();
    value_hd2_p3_out_y = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }

    valueStr=ui->lineEdit_20->text();
    value_hd2_p4_out_y = valueStr.toInt(&ok);
    if(!ok)
    {
        QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
        return;
    }


    SetRoi_hd2_flag = true;
}
