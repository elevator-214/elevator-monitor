#pragma once
#include<opencv2/opencv.hpp>
#include<vector>
using namespace cv;
using namespace std;
namespace hlg
{
    class ThingInterface//�ӿ���
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
        //ǰ����ȡ
        void create_ForeExtraction(const double &inputFrame_rows, const double &inputFrame_cols, bool show_flag);
        bool fore_ExtractFore(const Mat &inputFrame, Mat &foregroundMask);
        Size fore_getScaledSize();
        //ǰ����ȡend

        //������
        void create_Thingdetector();
        void detect_ThingsDetect(const Mat &ForemaskImage);//����ǰ��ͼ�������������
        void detect_SetOutputCoordScale(double OriginImage_Height, double OriginImage_Width, Size Current_Size);
        void detect_Get_Thing_Result(vector<Rect>&things_boxes, const vector<Rect>&people_boxes, const Rect& thROI);//����ת����ͬʱ����iou����ͷ�˳���
        //������end
        
        //�������
        void create_Thingtracker();
        void track(const vector<Rect>&Thing_Detected);
        vector<vector<int>>& track_GetThingsInfo();
        //�������end
    private:
        ForeExtraction *fore_extracter;
        ThingDetector *thingdetector;
        ThingTracker *thingtracker;

    };
    
}