#ifndef KALMANTRACKER_H
#define KALMANTRACKER_H

/************************************************************/
/*  File Name: KalmanTracker.cpp							*/
/*  Description: Class for multi-target Kalman tracking.	*/
/*  Author:     HLG                             			*/
/*  EMail: huang.liguang@qq.com								*/
/************************************************************/

#include "myFunction.h"

class MyKalmanFilter : public KalmanFilter
{
public:
    MyKalmanFilter(int id_0 = 0, Point2f measurement_0 = Point2f(0, 0),Rect box=Rect());
    ~MyKalmanFilter();
    int id;
    float confidence;
    Rect box;
    vector<Point2f> trajectory;
    int inside_inc;
    int outside_inc;
    Point2f position();
    void confidenceIncrease();
    bool confidenceDecrease();
    void kalman_correct(const Point2f center_point);
    void kalman_predict();
private:
    int confidence_inc;
    int confidence_dec;

};

class MyPersonKalmanFilter : public MyKalmanFilter
{
public:
    MyPersonKalmanFilter(int id_0 = 0, Point2f measurement_0 = Point2f(0, 0),Rect box=Rect());
    ~MyPersonKalmanFilter(){}
    enum side{defaut,up,down};
    enum side appearside;//出现的位置
    enum side disappearside;//消失位置
    bool UpsideFlag;//行人轨迹有路过上部分,置true
    bool DownsideFlag;//行人轨迹有路过下部分,置true
    bool count_flag;//该行人是否已计数
    unsigned int track_frame;//连续匹配成功帧数
};

class KalmanTracker
{
public:
    KalmanTracker(float targetSize_0 = 60, string targetName_0 = "target");
    ~KalmanTracker();
    vector<MyKalmanFilter> target;
    float size;//轨迹长度
    string name;
    int count;
    void track(vector<Point2f> measurement);
    vector<Point2f> trackment();
    void print(int frameCount = 0);
    Mat show(Mat src, int type = 0);
protected:
    vector<bool> idTabel;
    void idTabelUpdate(int id);
    int idCreator();
private:

    Scalar colorTabel(int id_0 = 0);

};

class PeopleKalmanTracker:public KalmanTracker
{
public:
    PeopleKalmanTracker(float targetSize_0 = 60, string targetName_0 = "target");
    ~PeopleKalmanTracker(){}
    void track(vector<Point2f> measurement,std::vector<cv::Rect>ppDetectionRect);
    vector<MyPersonKalmanFilter> target;
    void PeopleFlow();
    void PeopleFlowFix(MyPersonKalmanFilter person);//由于有可能有的行人折返,因此需要对人流的走出位置进行判断,从而对人流量进行修正
    int up_to_down_count;
    int down_to_up_count;
};


class ThingKalmanTracker:public KalmanTracker
{
public:
    ThingKalmanTracker(float targetSize_0 = 60, string targetName_0 = "target")
        :KalmanTracker(targetSize_0,targetName_0){}
    ~ThingKalmanTracker(){}
};
extern int ppFlowLine;
#endif // KALMANTRACKER_H
