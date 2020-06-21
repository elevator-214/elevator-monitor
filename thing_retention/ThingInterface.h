#pragma once
#include<opencv2/opencv.hpp>
#include<vector>
using namespace cv;
using namespace std;
namespace hlg
{
    class ThingInterface//接口类
    {
    public:
        class ForeExtraction;
        class ThingDetector;
        class ThingTracker;
        ThingInterface() {}
        ~ThingInterface() {
            if (!fore_extracter)
                delete fore_extracter;
            if (!thingdetector)
                delete thingdetector;
            if (!thingtracker)
                delete thingtracker;
        };
       //前景提取
        void create_ForeExtraction(const double &inputFrame_rows, const double &inputFrame_cols, bool show_flag);
        bool fore_ExtractFore(const Mat &inputFrame, Mat &foregroundMask);
        Size fore_getScaledSize();
        //前景提取end

        //物体检测
        void create_Thingdetector();
        void detect_ThingsDetect(const Mat &ForemaskImage);//根据前景图来把物体检测出来
        void detect_SetOutputCoordScale(double OriginImage_Height, double OriginImage_Width, Size Current_Size);
        void detect_Get_Thing_Result(vector<Rect>&things_boxes,  const Rect& thROI);//坐标转换，同时根据iou把人头滤除掉
        //物体检测end
        
        //物体跟踪
        void create_Thingtracker();
        void track(const vector<Rect>&Thing_Detected ,const vector<Rect>&people_boxes);
        vector<vector<int>>& track_GetThingsInfo();
        //物体跟踪end
    private:
        ForeExtraction *fore_extracter;
        ThingDetector *thingdetector;
        ThingTracker *thingtracker;

    };
    
}
