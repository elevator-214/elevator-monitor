/********************************************************************************
** Form generated from reading UI file 'dialog_3.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DIALOG_3_H
#define UI_DIALOG_3_H

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

class Ui_Dialog_3
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

    void setupUi(QDialog *Dialog_3)
    {
        if (Dialog_3->objectName().isEmpty())
            Dialog_3->setObjectName(QStringLiteral("Dialog_3"));
        Dialog_3->resize(400, 300);
        label = new QLabel(Dialog_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(50, 20, 81, 17));
        label_2 = new QLabel(Dialog_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(50, 70, 81, 17));
        label_3 = new QLabel(Dialog_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(50, 120, 67, 17));
        label_4 = new QLabel(Dialog_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(50, 170, 67, 17));
        lineEdit = new QLineEdit(Dialog_3);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setGeometry(QRect(170, 20, 113, 27));
        lineEdit_2 = new QLineEdit(Dialog_3);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(170, 70, 113, 27));
        lineEdit_3 = new QLineEdit(Dialog_3);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(170, 120, 113, 27));
        lineEdit_4 = new QLineEdit(Dialog_3);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(170, 170, 113, 27));
        pushButton = new QPushButton(Dialog_3);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(30, 220, 271, 27));

        retranslateUi(Dialog_3);

        QMetaObject::connectSlotsByName(Dialog_3);
    } // setupUi

    void retranslateUi(QDialog *Dialog_3)
    {
        Dialog_3->setWindowTitle(QApplication::translate("Dialog_3", "Dialog", Q_NULLPTR));
        label->setText(QApplication::translate("Dialog_3", "Left Top x :", Q_NULLPTR));
        label_2->setText(QApplication::translate("Dialog_3", "Left Top y :", Q_NULLPTR));
        label_3->setText(QApplication::translate("Dialog_3", "Width :", Q_NULLPTR));
        label_4->setText(QApplication::translate("Dialog_3", "Height :", Q_NULLPTR));
        pushButton->setText(QApplication::translate("Dialog_3", "Sure", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Dialog_3: public Ui_Dialog_3 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOG_3_H
