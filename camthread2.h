#ifndef CAMTHREAD2_H
#define CAMTHREAD2_H
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
#include<thread>
using namespace std;
using namespace cv;





class CamThread2 : public QThread
{
    Q_OBJECT
public:
    CamThread2();
    ~CamThread2();

    QImage qtImage;

    int CAMERA_WIDTH;
    int CAMERA_HEIGHT;
    int CAMERA_NUM;
    bool CAMERA_STOP;
    int fmMinue;

    //ROI
    Rect ppROI;

    //Activity
    bool ppCount;

signals:
    void getImage2(QImage);
    void para_error();

protected:
    void run();
};

#endif // CAMTHREAD2_H
