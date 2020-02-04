#ifndef CAMTHREAD3_H
#define CAMTHREAD3_H
#include <QMessageBox>
#include <QThread>
#include <QImage>
#include <iostream>
#include<fstream>
#include<read_para.h>
#include "myFunction.h"
#include "KalmanTracker.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/videoio.hpp>
#include <stdio.h>

using namespace std;
using namespace cv;


extern bool load_cam3_flag;
extern int value_hd1_x;
extern int value_hd1_y;
extern int value_hd1_width;
extern int value_hd1_height;
extern int value_hd1_p1_in_x;
extern int value_hd1_p1_in_y;
extern int value_hd1_p2_in_x;
extern int value_hd1_p2_in_y;
extern int value_hd1_p3_in_x;
extern int value_hd1_p3_in_y;
extern int value_hd1_p4_in_x;
extern int value_hd1_p4_in_y;
extern int value_hd1_p1_out_x;
extern int value_hd1_p1_out_y;
extern int value_hd1_p2_out_x;
extern int value_hd1_p2_out_y;
extern int value_hd1_p3_out_x;
extern int value_hd1_p3_out_y;
extern int value_hd1_p4_out_x;
extern int value_hd1_p4_out_y;

extern bool SetRoi_hd1_flag;

class CamThread3 : public QThread
{
    Q_OBJECT
public:
    CamThread3();
    ~CamThread3();

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
    void getImage3(QImage);
    void para_error();

protected:
    void run();
};

#endif // CAMTHREAD3_H
