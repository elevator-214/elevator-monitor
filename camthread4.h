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
