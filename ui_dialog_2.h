/********************************************************************************
** Form generated from reading UI file 'dialog_2.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_2_H
#define UI_DIALOG_2_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Dialog_2
{
public:
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QPushButton *pushButton;

    void setupUi(QDialog *Dialog_2)
    {
        if (Dialog_2->objectName().isEmpty())
            Dialog_2->setObjectName(QStringLiteral("Dialog_2"));
        Dialog_2->resize(400, 300);
        label = new QLabel(Dialog_2);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 20, 81, 17));
        label_2 = new QLabel(Dialog_2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(50, 70, 81, 17));
        label_3 = new QLabel(Dialog_2);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 120, 67, 17));
        label_4 = new QLabel(Dialog_2);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(50, 170, 67, 17));
        lineEdit = new QLineEdit(Dialog_2);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(170, 20, 113, 27));
        lineEdit_2 = new QLineEdit(Dialog_2);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(170, 70, 113, 27));
        lineEdit_3 = new QLineEdit(Dialog_2);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(170, 120, 113, 27));
        lineEdit_4 = new QLineEdit(Dialog_2);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(170, 170, 113, 27));
        pushButton = new QPushButton(Dialog_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(30, 230, 271, 27));

        retranslateUi(Dialog_2);

        QMetaObject::connectSlotsByName(Dialog_2);
    } // setupUi

    void retranslateUi(QDialog *Dialog_2)
    {
        Dialog_2->setWindowTitle(QApplication::translate("Dialog_2", "Dialog", 0));
        label->setText(QApplication::translate("Dialog_2", "Left Top x :", 0));
        label_2->setText(QApplication::translate("Dialog_2", "Left Top y :", 0));
        label_3->setText(QApplication::translate("Dialog_2", "width :", 0));
        label_4->setText(QApplication::translate("Dialog_2", "height :", 0));
        pushButton->setText(QApplication::translate("Dialog_2", "Sure", 0));
    } // retranslateUi

};

namespace Ui {
    class Dialog_2: public Ui_Dialog_2 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_2_H
