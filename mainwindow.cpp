#include "mainwindow.h"
#include "ui_mainwindow.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480
bool load_cam1_flag=false;
bool load_cam2_flag=false;
bool load_cam3_flag=false;
bool load_cam4_flag=false;
bool draw_trajectory_flag=false;
bool drawSkeleton_flag=false;
bool drawID_flag=false;
bool drawBox_flag=false;
bool cam1_start_flag=true;
bool cam2_start_flag=true;
bool cam3_start_flag=true;
bool cam4_start_flag=true;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Dlg = new Dialog;
    Dlg_2 = new Dialog_2;
    Dlg_3 = new Dialog_3;
    Dlg_4 = new Dialog_4;
    Dlg_5 = new Dialog_5;

    ui->actiontrajectory_No->setEnabled(draw_trajectory_flag);//行人轨迹标志初始化
    ui->actiontrajectory_Yes->setEnabled(!draw_trajectory_flag);
    ui->actionDrawSkeletonNo->setEnabled(drawSkeleton_flag);//画骨架标志位初始化
    ui->actionDrawSkeletonYes->setEnabled(!drawSkeleton_flag);
    ui->actionDrawIDNo->setEnabled(drawID_flag);//画ID标志位初始化
    ui->actionDrawIDYes->setEnabled(!drawID_flag);
    ui->actionDrawBoxNo->setEnabled(drawBox_flag);//画检测框标志位初始化
    ui->actionDrawBoxYes->setEnabled(!drawBox_flag);

    ui->actionStart_cam1->setEnabled(!cam1_start_flag);//cam1启动标志位
    ui->actionStop_cam1->setEnabled(cam1_start_flag);//cam1停止标志位

    ui->actionStart_cam2->setEnabled(!cam2_start_flag);//cam2启动标志位
    ui->actionStop_cam2->setEnabled(cam2_start_flag);//cam2停止标志位

    ui->actionStart_cam3->setEnabled(!cam3_start_flag);//cam3启动标志位
    ui->actionStop_cam3->setEnabled(cam3_start_flag);//cam3停止标志位

    ui->actionStart_cam4->setEnabled(!cam4_start_flag);//cam4启动标志位
    ui->actionStop_cam4->setEnabled(cam4_start_flag);//cam4停止标志位


    connect( &cam1, SIGNAL(getImage1(QImage)), this, SLOT(showImage1(QImage)) );
    cam1.start();

    connect( &cam2, SIGNAL(getImage2(QImage)), this, SLOT(showImage2(QImage)) );
    cam2.start();

    connect( &cam3, SIGNAL(getImage3(QImage)), this, SLOT(showImage3(QImage)) );
    cam3.start();

    connect( &cam4, SIGNAL(getImage4(QImage)), this, SLOT(showImage4(QImage)) );
    cam4.start();

    com_1.start();

    connect( &cam1, SIGNAL(para_error()), this, SLOT(show_error()) );
    connect( &cam2, SIGNAL(para_error()), this, SLOT(show_error()) );
    connect( &cam3, SIGNAL(para_error()), this, SLOT(show_error()) );
    connect( &cam4, SIGNAL(para_error()), this, SLOT(show_error()) );


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showImage1(QImage img)
{
    ui->label->setPixmap(QPixmap::fromImage(img, Qt::AutoColor));


}

void MainWindow::showImage2(QImage img)
{
    ui->label_2->setPixmap(QPixmap::fromImage(img, Qt::AutoColor));
}

void MainWindow::showImage3(QImage img)
{
    ui->label_3->setPixmap(QPixmap::fromImage(img, Qt::AutoColor));
}

void MainWindow::showImage4(QImage img)
{
    ui->label_4->setPixmap(QPixmap::fromImage(img, Qt::AutoColor));
}

void MainWindow::show_error()
{
    QMessageBox::information(this,QString::fromLocal8Bit("errro"),QString::fromLocal8Bit("set para error"));
    //Dlg_3->close();
}

void MainWindow::on_actionPeople_ROI_1_triggered()
{   
    Dlg->setWindowFlags(Dlg->windowFlags()|Qt::WindowStaysOnTopHint);
    QPoint p(3600,3600);
    Dlg->move(p);
    Dlg->load_text();
    Dlg->show();
}


void MainWindow::on_actionThing_ROI_triggered()
{
    Dlg_2->setWindowFlags(Dlg_2->windowFlags()|Qt::WindowStaysOnTopHint);
    QPoint p(3600,3600);
    Dlg_2->move(p);
    Dlg_2->load_text();
    Dlg_2->show();
}

void MainWindow::on_actionPeople_ROI_2_triggered()
{
    Dlg_3->setWindowFlags(Dlg_3->windowFlags()|Qt::WindowStaysOnTopHint);
    QPoint p(3600,3600);
    Dlg_3->move(p);
    Dlg_3->show();
}

void MainWindow::on_actionHand_ROI_1_triggered()
{
    Dlg_4->setWindowFlags(Dlg_4->windowFlags()|Qt::WindowStaysOnTopHint);
    QPoint p(3600,3600);
    Dlg_4->move(p);
    Dlg_2->load_text();
    Dlg_4->show();
}

void MainWindow::on_actionHand_ROI_2_triggered()
{
    Dlg_5->setWindowFlags(Dlg_5->windowFlags()|Qt::WindowStaysOnTopHint);
    QPoint p(3600,3600);
    Dlg_5->move(p);
    Dlg_2->load_text();
    Dlg_5->show();
}

void MainWindow::on_actionLoad_all_parameter_triggered()
{
    /*加载参数*/
    load_cam1_flag=load_cam2_flag=load_cam3_flag=load_cam4_flag=true;
    /*显示加载成功窗口*/
    QMessageBox::about(NULL, "About", "加载所有参数成功!!!");
}

void MainWindow::on_actionLoad_cam1_triggered()
{
    load_cam1_flag=true;
    QMessageBox::about(NULL, "About", "加载参数cam1成功!!!");
}

void MainWindow::on_actionLoad_cam2_triggered()
{
    load_cam2_flag=true;
    QMessageBox::about(NULL, "About", "cam2没有要设置的参数,如果要进一步修改,请到python程序中修改!!!");
}

void MainWindow::on_actionLoad_cam3_triggered()
{
    load_cam3_flag=true;
    QMessageBox::about(NULL, "About", "加载参数cam3成功!!!");
}

void MainWindow::on_actionLoad_cam4_triggered()
{
    load_cam4_flag=true;
    QMessageBox::about(NULL, "About", "加载参数cam4成功!!!");

}
//画轨迹按钮yes
void MainWindow::on_actiontrajectory_Yes_triggered()
{
    QMessageBox:: StandardButton result=QMessageBox::information(NULL, "Title", "画行人轨迹!!!",
                               QMessageBox::No|QMessageBox::Yes, QMessageBox::Yes);
    switch (result)
    {
    case QMessageBox::Yes:
        ui->actiontrajectory_Yes->setEnabled(false);//画行人轨迹的按钮变得不可见
        ui->actiontrajectory_No->setEnabled(true);
        draw_trajectory_flag=true;
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }

}
//画轨迹按钮no
void MainWindow::on_actiontrajectory_No_triggered()
{
    QMessageBox:: StandardButton result=QMessageBox::information(NULL, "Title", "取消画行人轨迹!!!",
            QMessageBox::No|QMessageBox::Yes, QMessageBox::Yes);
    switch (result)
    {
    case QMessageBox::Yes:
        ui->actiontrajectory_No->setEnabled(false);//取消画行人轨迹的按钮变得不可见
        ui->actiontrajectory_Yes->setEnabled(true);
        draw_trajectory_flag=false;
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }

}



//画骨架按钮yes
void MainWindow::on_actionDrawSkeletonYes_triggered()
{
    QMessageBox:: StandardButton result=QMessageBox::information(NULL, "Title", "画行人骨架!!!",
                               QMessageBox::No|QMessageBox::Yes, QMessageBox::Yes);
    switch (result)
    {
    case QMessageBox::Yes:
        ui->actionDrawSkeletonYes->setEnabled(false);//画行人骨架的按钮变得不可见
        ui->actionDrawSkeletonNo->setEnabled(true);
        drawSkeleton_flag=true;
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }
}
//画骨架按钮no
void MainWindow::on_actionDrawSkeletonNo_triggered()
{
    QMessageBox:: StandardButton result=QMessageBox::information(NULL, "Title", "取消画行人骨架!!!",
            QMessageBox::No|QMessageBox::Yes, QMessageBox::Yes);
    switch (result)
    {
    case QMessageBox::Yes:
        ui->actionDrawSkeletonNo->setEnabled(false);//取消画行人骨架的按钮变得不可见
        ui->actionDrawSkeletonYes->setEnabled(true);
        drawSkeleton_flag=false;
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }
}


//画ID按钮yes
void MainWindow::on_actionDrawIDYes_triggered()
{
    QMessageBox:: StandardButton result=QMessageBox::information(NULL, "Title", "画行人ID!!!",
                               QMessageBox::No|QMessageBox::Yes, QMessageBox::Yes);
    switch (result)
    {
    case QMessageBox::Yes:
        ui->actionDrawIDYes->setEnabled(false);//画行人ID的按钮变得不可见
        ui->actionDrawIDNo->setEnabled(true);
        drawID_flag=true;
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }
}
//画ID按钮no
void MainWindow::on_actionDrawIDNo_triggered()
{
    QMessageBox:: StandardButton result=QMessageBox::information(NULL, "Title", "取消画行人ID!!!",
            QMessageBox::No|QMessageBox::Yes, QMessageBox::Yes);
    switch (result)
    {
    case QMessageBox::Yes:
        ui->actionDrawIDNo->setEnabled(false);//取消画行人ID的按钮变得不可见
        ui->actionDrawIDYes->setEnabled(true);
        drawID_flag=false;
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }
}

//画BOX按钮yes
void MainWindow::on_actionDrawBoxYes_triggered()
{
    QMessageBox:: StandardButton result=QMessageBox::information(NULL, "Title", "画行人BOX!!!",
                               QMessageBox::No|QMessageBox::Yes, QMessageBox::Yes);
    switch (result)
    {
    case QMessageBox::Yes:
        ui->actionDrawBoxYes->setEnabled(false);//画行人BOX的按钮变得不可见
        ui->actionDrawBoxNo->setEnabled(true);
        drawBox_flag=true;
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }
}
//画BOX按钮NO
void MainWindow::on_actionDrawBoxNo_triggered()
{
    QMessageBox:: StandardButton result=QMessageBox::information(NULL, "Title", "取消画行人BOX!!!",
            QMessageBox::No|QMessageBox::Yes, QMessageBox::Yes);
    switch (result)
    {
    case QMessageBox::Yes:
        ui->actionDrawBoxNo->setEnabled(false);//取消画行人BOX的按钮变得不可见
        ui->actionDrawBoxYes->setEnabled(true);
        drawBox_flag=false;
        break;
    case QMessageBox::No:
        break;
    default:
        break;
    }
}
void MainWindow::on_actionStart_cam1_triggered()
{
    cam1_start_flag=true;
    ui->actionStart_cam1->setEnabled(!cam1_start_flag);//cam1启动标志位
    ui->actionStop_cam1->setEnabled(cam1_start_flag);//cam1停止标志位

}
void MainWindow::on_actionStop_cam1_triggered()
{
    cam1_start_flag=false;
    ui->actionStart_cam1->setEnabled(!cam1_start_flag);//cam1启动标志位
    ui->actionStop_cam1->setEnabled(cam1_start_flag);//cam1停止标志位
}

void MainWindow::on_actionStart_cam2_triggered()
{
    cam2_start_flag=true;
    ui->actionStart_cam2->setEnabled(!cam2_start_flag);//cam2启动标志位
    ui->actionStop_cam2->setEnabled(cam2_start_flag);//cam2停止标志位
}
void MainWindow::on_actionStop_cam2_triggered()
{
    cam2_start_flag=false;
    ui->actionStart_cam2->setEnabled(!cam2_start_flag);//cam2启动标志位
    ui->actionStop_cam2->setEnabled(cam2_start_flag);//cam2停止标志位
}

void MainWindow::on_actionStart_cam3_triggered()
{
    cam3_start_flag=true;
    ui->actionStart_cam3->setEnabled(!cam3_start_flag);//cam3启动标志位
    ui->actionStop_cam3->setEnabled(cam3_start_flag);//cam3停止标志位
}
void MainWindow::on_actionStop_cam3_triggered()
{
    cam3_start_flag=false;
    ui->actionStart_cam3->setEnabled(!cam3_start_flag);//cam3启动标志位
    ui->actionStop_cam3->setEnabled(cam3_start_flag);//cam3停止标志位
}

void MainWindow::on_actionStart_cam4_triggered()
{
    cam4_start_flag=true;
    ui->actionStart_cam4->setEnabled(!cam4_start_flag);//cam4启动标志位
    ui->actionStop_cam4->setEnabled(cam4_start_flag);//cam4停止标志位
}
void MainWindow::on_actionStop_cam4_triggered()
{
    cam4_start_flag=false;
    ui->actionStart_cam4->setEnabled(!cam4_start_flag);//cam4启动标志位
    ui->actionStop_cam4->setEnabled(cam4_start_flag);//cam4停止标志位
}








