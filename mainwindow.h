#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "camthread1.h"
#include "camthread2.h"
#include "camthread3.h"
#include "camthread4.h"
#include "communication.h"
#include <unistd.h>


#include <bitset>

#include <QMessageBox>

#include "dialog.h"
#include "dialog_2.h"
#include "dialog_3.h"
#include "dialog_4.h"
#include "dialog_5.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void showImage1(QImage img);
    void showImage2(QImage img);
    void showImage3(QImage img);
    void showImage4(QImage img);

    void show_error();

    void on_actionPeople_ROI_1_triggered();


    void on_actionThing_ROI_triggered();

    void on_actionPeople_ROI_2_triggered();

    void on_actionHand_ROI_1_triggered();

    void on_actionHand_ROI_2_triggered();

    void on_actionLoad_all_parameter_triggered();

    void on_actionLoad_cam1_triggered();

    void on_actionLoad_cam2_triggered();

    void on_actionLoad_cam3_triggered();

    void on_actionLoad_cam4_triggered();

    void on_actiontrajectory_Yes_triggered();

    void on_actiontrajectory_No_triggered();

    void on_actionDrawSkeletonYes_triggered();

    void on_actionDrawSkeletonNo_triggered();

    void on_actionDrawIDYes_triggered();

    void on_actionDrawIDNo_triggered();

    void on_actionDrawBoxYes_triggered();

    void on_actionDrawBoxNo_triggered();

    void on_actionStop_cam1_triggered();

    void on_actionStop_cam2_triggered();

    void on_actionStop_cam3_triggered();

    void on_actionStop_cam4_triggered();

    void on_actionStart_cam1_triggered();

    void on_actionStart_cam2_triggered();

    void on_actionStart_cam3_triggered();

    void on_actionStart_cam4_triggered();

private:
    Ui::MainWindow *ui;
    CamThread1 cam1;
    CamThread2 cam2;
    CamThread3 cam3;
    CamThread4 cam4;
    Communication com_1;



    Dialog *Dlg;
    Dialog_2 *Dlg_2;
    Dialog_3 *Dlg_3;
    Dialog_4 *Dlg_4;
    Dialog_5 *Dlg_5;

};

#endif // MAINWINDOW_H
