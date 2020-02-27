/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionPeople_ROI_1;
    QAction *actionThing_ROI;
    QAction *actionPeople_ROI_2;
    QAction *actionHand_ROI_1;
    QAction *actionHand_ROI_2;
    QAction *actionLoad_all_parameter;
    QAction *actionLoad_cam1;
    QAction *actionLoad_cam2;
    QAction *actionLoad_cam3;
    QAction *actionLoad_cam4;
    QAction *actionDrawIDYes;
    QAction *actionDrawIDNo;
    QAction *actionDrawSkeletonYes;
    QAction *actionDrawSkeletonNo;
    QAction *actiontrajectory_Yes;
    QAction *actiontrajectory_No;
    QAction *actionStop_cam1;
    QAction *actionStop_cam2;
    QAction *actionStop_cam3;
    QAction *actionStop_cam4;
    QAction *actionStart_cam1;
    QAction *actionStart_cam2;
    QAction *actionStart_cam3;
    QAction *actionStart_cam4;
    QAction *actionDrawBoxYes;
    QAction *actionDrawBoxNo;
    QWidget *centralWidget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QLabel *label_4;
    QMenuBar *menuBar;
    QMenu *menuSet_ROI;
    QMenu *menuRead_Parameter;
    QMenu *menuShow;
    QMenu *menuDrawID;
    QMenu *menuDrawSkeleton;
    QMenu *menuDrawTrajectory;
    QMenu *menuDrawBox;
    QMenu *menuStop;
    QMenu *menuStart;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1330, 1050);
        actionPeople_ROI_1 = new QAction(MainWindow);
        actionPeople_ROI_1->setObjectName(QStringLiteral("actionPeople_ROI_1"));
        actionThing_ROI = new QAction(MainWindow);
        actionThing_ROI->setObjectName(QStringLiteral("actionThing_ROI"));
        actionPeople_ROI_2 = new QAction(MainWindow);
        actionPeople_ROI_2->setObjectName(QStringLiteral("actionPeople_ROI_2"));
        actionHand_ROI_1 = new QAction(MainWindow);
        actionHand_ROI_1->setObjectName(QStringLiteral("actionHand_ROI_1"));
        actionHand_ROI_2 = new QAction(MainWindow);
        actionHand_ROI_2->setObjectName(QStringLiteral("actionHand_ROI_2"));
        actionLoad_all_parameter = new QAction(MainWindow);
        actionLoad_all_parameter->setObjectName(QStringLiteral("actionLoad_all_parameter"));
        actionLoad_cam1 = new QAction(MainWindow);
        actionLoad_cam1->setObjectName(QStringLiteral("actionLoad_cam1"));
        actionLoad_cam2 = new QAction(MainWindow);
        actionLoad_cam2->setObjectName(QStringLiteral("actionLoad_cam2"));
        actionLoad_cam3 = new QAction(MainWindow);
        actionLoad_cam3->setObjectName(QStringLiteral("actionLoad_cam3"));
        actionLoad_cam4 = new QAction(MainWindow);
        actionLoad_cam4->setObjectName(QStringLiteral("actionLoad_cam4"));
        actionDrawIDYes = new QAction(MainWindow);
        actionDrawIDYes->setObjectName(QStringLiteral("actionDrawIDYes"));
        actionDrawIDYes->setCheckable(false);
        actionDrawIDNo = new QAction(MainWindow);
        actionDrawIDNo->setObjectName(QStringLiteral("actionDrawIDNo"));
        actionDrawIDNo->setCheckable(false);
        actionDrawSkeletonYes = new QAction(MainWindow);
        actionDrawSkeletonYes->setObjectName(QStringLiteral("actionDrawSkeletonYes"));
        actionDrawSkeletonYes->setCheckable(false);
        actionDrawSkeletonNo = new QAction(MainWindow);
        actionDrawSkeletonNo->setObjectName(QStringLiteral("actionDrawSkeletonNo"));
        actionDrawSkeletonNo->setCheckable(false);
        actiontrajectory_Yes = new QAction(MainWindow);
        actiontrajectory_Yes->setObjectName(QStringLiteral("actiontrajectory_Yes"));
        actiontrajectory_Yes->setCheckable(false);
        actiontrajectory_No = new QAction(MainWindow);
        actiontrajectory_No->setObjectName(QStringLiteral("actiontrajectory_No"));
        actiontrajectory_No->setCheckable(false);
        actionStop_cam1 = new QAction(MainWindow);
        actionStop_cam1->setObjectName(QStringLiteral("actionStop_cam1"));
        actionStop_cam1->setCheckable(false);
        actionStop_cam1->setEnabled(true);
        actionStop_cam2 = new QAction(MainWindow);
        actionStop_cam2->setObjectName(QStringLiteral("actionStop_cam2"));
        actionStop_cam3 = new QAction(MainWindow);
        actionStop_cam3->setObjectName(QStringLiteral("actionStop_cam3"));
        actionStop_cam4 = new QAction(MainWindow);
        actionStop_cam4->setObjectName(QStringLiteral("actionStop_cam4"));
        actionStart_cam1 = new QAction(MainWindow);
        actionStart_cam1->setObjectName(QStringLiteral("actionStart_cam1"));
        actionStart_cam2 = new QAction(MainWindow);
        actionStart_cam2->setObjectName(QStringLiteral("actionStart_cam2"));
        actionStart_cam3 = new QAction(MainWindow);
        actionStart_cam3->setObjectName(QStringLiteral("actionStart_cam3"));
        actionStart_cam4 = new QAction(MainWindow);
        actionStart_cam4->setObjectName(QStringLiteral("actionStart_cam4"));
        actionDrawBoxYes = new QAction(MainWindow);
        actionDrawBoxYes->setObjectName(QStringLiteral("actionDrawBoxYes"));
        actionDrawBoxYes->setCheckable(false);
        actionDrawBoxNo = new QAction(MainWindow);
        actionDrawBoxNo->setObjectName(QStringLiteral("actionDrawBoxNo"));
        actionDrawBoxNo->setCheckable(false);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 640, 480));
        label->setFrameShape(QFrame::Box);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(670, 10, 640, 480));
        label_2->setFrameShape(QFrame::Box);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(20, 500, 640, 480));
        label_3->setFrameShape(QFrame::Box);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(670, 500, 640, 480));
        label_4->setFrameShape(QFrame::Box);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1330, 31));
        menuSet_ROI = new QMenu(menuBar);
        menuSet_ROI->setObjectName(QStringLiteral("menuSet_ROI"));
        menuRead_Parameter = new QMenu(menuBar);
        menuRead_Parameter->setObjectName(QStringLiteral("menuRead_Parameter"));
        menuShow = new QMenu(menuBar);
        menuShow->setObjectName(QStringLiteral("menuShow"));
        menuDrawID = new QMenu(menuShow);
        menuDrawID->setObjectName(QStringLiteral("menuDrawID"));
        menuDrawSkeleton = new QMenu(menuShow);
        menuDrawSkeleton->setObjectName(QStringLiteral("menuDrawSkeleton"));
        menuDrawTrajectory = new QMenu(menuShow);
        menuDrawTrajectory->setObjectName(QStringLiteral("menuDrawTrajectory"));
        menuDrawBox = new QMenu(menuShow);
        menuDrawBox->setObjectName(QStringLiteral("menuDrawBox"));
        menuStop = new QMenu(menuBar);
        menuStop->setObjectName(QStringLiteral("menuStop"));
        menuStart = new QMenu(menuBar);
        menuStart->setObjectName(QStringLiteral("menuStart"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuSet_ROI->menuAction());
        menuBar->addAction(menuRead_Parameter->menuAction());
        menuBar->addAction(menuShow->menuAction());
        menuBar->addAction(menuStop->menuAction());
        menuBar->addAction(menuStart->menuAction());
        menuSet_ROI->addAction(actionPeople_ROI_1);
        menuSet_ROI->addAction(actionThing_ROI);
        menuSet_ROI->addAction(actionPeople_ROI_2);
        menuSet_ROI->addAction(actionHand_ROI_1);
        menuSet_ROI->addAction(actionHand_ROI_2);
        menuRead_Parameter->addAction(actionLoad_all_parameter);
        menuRead_Parameter->addAction(actionLoad_cam1);
        menuRead_Parameter->addAction(actionLoad_cam2);
        menuRead_Parameter->addAction(actionLoad_cam3);
        menuRead_Parameter->addAction(actionLoad_cam4);
        menuShow->addAction(menuDrawTrajectory->menuAction());
        menuShow->addAction(menuDrawSkeleton->menuAction());
        menuShow->addAction(menuDrawID->menuAction());
        menuShow->addAction(menuDrawBox->menuAction());
        menuDrawID->addAction(actionDrawIDYes);
        menuDrawID->addAction(actionDrawIDNo);
        menuDrawSkeleton->addAction(actionDrawSkeletonYes);
        menuDrawSkeleton->addAction(actionDrawSkeletonNo);
        menuDrawTrajectory->addAction(actiontrajectory_Yes);
        menuDrawTrajectory->addAction(actiontrajectory_No);
        menuDrawBox->addAction(actionDrawBoxYes);
        menuDrawBox->addAction(actionDrawBoxNo);
        menuStop->addAction(actionStop_cam1);
        menuStop->addAction(actionStop_cam2);
        menuStop->addAction(actionStop_cam3);
        menuStop->addAction(actionStop_cam4);
        menuStart->addAction(actionStart_cam1);
        menuStart->addAction(actionStart_cam2);
        menuStart->addAction(actionStart_cam3);
        menuStart->addAction(actionStart_cam4);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        actionPeople_ROI_1->setText(QApplication::translate("MainWindow", "People ROI 1 ", Q_NULLPTR));
        actionThing_ROI->setText(QApplication::translate("MainWindow", "Thing ROI", Q_NULLPTR));
        actionPeople_ROI_2->setText(QApplication::translate("MainWindow", "People ROI 2", Q_NULLPTR));
        actionHand_ROI_1->setText(QApplication::translate("MainWindow", "Hand ROI 1", Q_NULLPTR));
        actionHand_ROI_2->setText(QApplication::translate("MainWindow", "Hand ROI 2", Q_NULLPTR));
        actionLoad_all_parameter->setText(QApplication::translate("MainWindow", "load_all_parameter", Q_NULLPTR));
        actionLoad_cam1->setText(QApplication::translate("MainWindow", "load_cam1", Q_NULLPTR));
        actionLoad_cam2->setText(QApplication::translate("MainWindow", "load_cam2", Q_NULLPTR));
        actionLoad_cam3->setText(QApplication::translate("MainWindow", "load_cam3", Q_NULLPTR));
        actionLoad_cam4->setText(QApplication::translate("MainWindow", "load_cam4", Q_NULLPTR));
        actionDrawIDYes->setText(QApplication::translate("MainWindow", "yes", Q_NULLPTR));
        actionDrawIDNo->setText(QApplication::translate("MainWindow", "no", Q_NULLPTR));
        actionDrawSkeletonYes->setText(QApplication::translate("MainWindow", "yes", Q_NULLPTR));
        actionDrawSkeletonNo->setText(QApplication::translate("MainWindow", "no", Q_NULLPTR));
        actiontrajectory_Yes->setText(QApplication::translate("MainWindow", "yes", Q_NULLPTR));
        actiontrajectory_No->setText(QApplication::translate("MainWindow", "no", Q_NULLPTR));
        actionStop_cam1->setText(QApplication::translate("MainWindow", "stop_cam1", Q_NULLPTR));
        actionStop_cam2->setText(QApplication::translate("MainWindow", "stop_cam2", Q_NULLPTR));
        actionStop_cam3->setText(QApplication::translate("MainWindow", "stop_cam3", Q_NULLPTR));
        actionStop_cam4->setText(QApplication::translate("MainWindow", "stop_cam4", Q_NULLPTR));
        actionStart_cam1->setText(QApplication::translate("MainWindow", "start_cam1", Q_NULLPTR));
        actionStart_cam2->setText(QApplication::translate("MainWindow", "start_cam2", Q_NULLPTR));
        actionStart_cam3->setText(QApplication::translate("MainWindow", "start_cam3", Q_NULLPTR));
        actionStart_cam4->setText(QApplication::translate("MainWindow", "start_cam4", Q_NULLPTR));
        actionDrawBoxYes->setText(QApplication::translate("MainWindow", "yes", Q_NULLPTR));
        actionDrawBoxNo->setText(QApplication::translate("MainWindow", "no", Q_NULLPTR));
        label->setText(QString());
        label_2->setText(QString());
        label_3->setText(QString());
        label_4->setText(QString());
        menuSet_ROI->setTitle(QApplication::translate("MainWindow", "Set ROI", Q_NULLPTR));
        menuRead_Parameter->setTitle(QApplication::translate("MainWindow", "Read Paras", Q_NULLPTR));
        menuShow->setTitle(QApplication::translate("MainWindow", "Draw", Q_NULLPTR));
        menuDrawID->setTitle(QApplication::translate("MainWindow", "drawID", Q_NULLPTR));
        menuDrawSkeleton->setTitle(QApplication::translate("MainWindow", "drawSkeleton", Q_NULLPTR));
        menuDrawTrajectory->setTitle(QApplication::translate("MainWindow", "drawTrajectory", Q_NULLPTR));
        menuDrawBox->setTitle(QApplication::translate("MainWindow", "drawBox", Q_NULLPTR));
        menuStop->setTitle(QApplication::translate("MainWindow", "Stop", Q_NULLPTR));
        menuStart->setTitle(QApplication::translate("MainWindow", "Start", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
