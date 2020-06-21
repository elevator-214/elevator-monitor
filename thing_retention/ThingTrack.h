#pragma once
#include"myFunction.h"
#include"ThingInterface.h"
namespace hlg
{
    class Thing
    {
    public:
        Thing(int id, const Rect &detected_box);
        unsigned int track_frame;//连续匹配成功帧数，如果超过一定数目，则判断为滞留
        int id;//id号
        bool confidenceDecrease();
        void confidenceIncrease();
        Rect box;//外接矩形框
    private:
        double confidence;//置信度
        int confidence_inc;
        int confidence_dec;
    };
    class ThingInterface::ThingTracker//由于是判断物体滞留，因此个人感觉没有必要使用kalman进行运动估计徒增计算量，故把kalman去除了，匈牙利匹配依然保留
    {
    public:
        ThingTracker(string thing_name = "th");
        void track(const vector<Rect>&Thing_Detected ,const vector<Rect>&people_boxes);
        vector<vector<int>>&GetThingsInfo();
        

    private:
        vector<Thing> tracking_things;
        vector<vector<int>>ThingsInfo;//x1 y1 x3 y3 frame id
        string name;
        vector<bool> idTabel;
        void Filter_People(vector<Rect>&things_boxes,const vector<Rect>&people_boxes);
        void idTabelDelete(int id);
        int idCreator();
    };
}
