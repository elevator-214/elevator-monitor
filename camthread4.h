#ifndef CAMTHREAD4_H
#define CAMTHREAD4_H
#include <QMessageBox>
#include <QThread>
#include <QImage>
#include <iostream>
#include<fstream>
#include<read_para.h>
#include "myFunction.h"
#include "KalmanTracker.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;


extern bool load_cam4_flag;
extern int value_hd2_x;
extern int value_hd2_y;
extern int value_hd2_width;
extern int value_hd2_height;
extern int value_hd2_p1_in_x;
extern int value_hd2_p1_in_y;
extern int value_hd2_p2_in_x;
extern int value_hd2_p2_in_y;
extern int value_hd2_p3_in_x;
extern int value_hd2_p3_in_y;
extern int value_hd2_p4_in_x;
extern int value_hd2_p4_in_y;
extern int value_hd2_p1_out_x;
extern int value_hd2_p1_out_y;
extern int value_hd2_p2_out_x;
extern int value_hd2_p2_out_y;
extern int value_hd2_p3_out_x;
extern int value_hd2_p3_out_y;
extern int value_hd2_p4_out_x;
extern int value_hd2_p4_out_y;

extern bool SetRoi_hd2_flag;

class CamThread4 : public QThread
{
    Q_OBJECT
public:
    CamThread4();
    ~CamThread4();

    QImage qtImage;

    int CAMERA_WIDTH;
    int CAMERA_HEIGHT;
    int CAMERA_NUM;
    bool CAMERA_STOP;
    int fmMinue;

    //ROI
    Rect hdROI;

    //Activity
    bool hdDrail;

signals:
    void getImage4(QImage);
    void para_error();

protected:
    void run();
};

#endif // CAMTHREAD4_H
