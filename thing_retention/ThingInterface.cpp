#include"ThingInterface.h"
#include"Fore_extraction.h"
#include"ThingDetect.h"
#include"ThingTrack.h"
namespace hlg 
{
    //提取前景
    void ThingInterface::create_ForeExtraction(const double &inputFrame_rows, const double &inputFrame_cols, bool show_flag)
    {
        if (!fore_extracter)
            delete fore_extracter;
        fore_extracter= new ForeExtraction(inputFrame_rows, inputFrame_cols, show_flag);
    }
    bool ThingInterface::fore_ExtractFore(const Mat &inputFrame, Mat &foregroundMask)
    {
        return fore_extracter->extract(inputFrame, foregroundMask);
    }
    Size ThingInterface::fore_getScaledSize()
    {
        return fore_extracter->scaledSize;
    }
    //提取前景end


    //物体检测
    void ThingInterface::create_Thingdetector()
    {
        if (!thingdetector)
            delete thingdetector;
        thingdetector= new ThingDetector();
    }
    void ThingInterface::detect_ThingsDetect(const Mat &ForemaskImage)
    {
        thingdetector->ThingsDetect(ForemaskImage);
    }
    void ThingInterface::detect_SetOutputCoordScale(double OriginImage_Height, double OriginImage_Width, Size Current_Size)
    {
        thingdetector->SetOutputCoordScale(OriginImage_Height, OriginImage_Width, Current_Size);
    }
    void ThingInterface::detect_Get_Thing_Result(vector<Rect>&things_boxes, const vector<Rect>&people_boxes,const Rect& thROI)
    {
        thingdetector->Get_Thing_Result(things_boxes, people_boxes,thROI);
    }

    //物体检测end


    //物体跟踪
    void ThingInterface::create_Thingtracker()
    {
        if (!thingtracker)
            delete thingtracker;
        thingtracker = new ThingTracker();
    }
    void ThingInterface::track(const vector<Rect>&Thing_Detected)
    {
        thingtracker->track(Thing_Detected);
    }
    vector<vector<int>>& ThingInterface::track_GetThingsInfo()
    {
        return thingtracker->GetThingsInfo();
    }
    //物体跟踪end
}