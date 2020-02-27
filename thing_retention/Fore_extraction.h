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
        Size scaledSize;//ǰ����ȡͼƬ��С
        bool extract(const Mat & inputFrame, Mat &foregroundMask);
        
    private:
        const int init_background_length = 600;//��ʼ��600֡����ѭ��������������ģ
        const int init_background_frame_length = 2;//������ͷ��ȡǰ��2֡��������
        const int image_width = 640;
        
        bool show_flag;//�Ƿ���ʾǰ����ȡ���
        /*knn����*/
        vector<Mat>frame_buff;
        Ptr<BackgroundSubtractor> model;
        const double learning_rate = 0.001;
        const int history = 20000;
        const double dist2thrshold = 2500;
        const bool detectShadow = true;//�����Ӱ
        void getBackgroundImage(Mat&background);
    };

    
}

