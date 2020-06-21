#pragma once
#include<opencv2/opencv.hpp>
#include<vector>
#include"ThingInterface.h"
using namespace cv;
using namespace std;
namespace hlg
{
    class ThingInterface::ForeExtraction
    {
    public:
        ForeExtraction(const double &inputFrame_rows, const double &inputFrame_cols, bool show_flag=false);
        ~ForeExtraction() { }
        Size scaledSize;//前景提取图片大小
        bool extract(const Mat & inputFrame, Mat &foregroundMask);
        
    private:
        const int init_background_length = 600;//开始的600帧不断循环，当作背景建模
        const int init_background_frame_length = 2;//从摄像头读取前面2帧当作背景
        const int image_width = 640;
        
        bool show_flag;//是否显示前景提取结果
        /*knn参数*/
        vector<Mat>frame_buff;
        Ptr<BackgroundSubtractor> model;
        const double learning_rate = 0.001;
        const int history = 20000;
        const double dist2thrshold = 2500;
        const bool detectShadow = true;//检测阴影
        void getBackgroundImage(Mat&background);
    };

    
}

