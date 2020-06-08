/* 
Author:HLG
Date:2020年6月5日
huang.liguang@qq.com
*/
//#define _GLIBCXX_USE_CXX11_ABI 0//如果电脑的gcc版本比opencv编译的版本低，则需要使用这一行来避免string编译出错，gcc版本与opencv版本一致，将其注释即可
#include<opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include<vector>
#include<string>
#include<ctime>
#include<iostream>
#include"keypoints_tracking.h"

using namespace std;
using namespace cv;
const static int keypoints_num=14;//在删减不必要的关键点之后，还剩下14个关键点
const std::vector<int>bones={1,2, 1,5, 2,3, 3,4, 5,6, 6,7, 2,8, 5,11, 8,9, 9,10, 11,12, 12,13, 1,0};//骨架连接

void keypoints_num_resort_test()
{
    int keypoints_num= count_keypoints_num();
    cout<<"keypoints_num"<<keypoints_num<<endl;
}

void camera_test()
{
    cv::Mat img;
    cv::VideoCapture cap(0);
    cv::namedWindow("test");
    while(cap.read(img))
    {
        cv::imshow("test",img);
        cv::waitKey(1);
        // cout<<"hello,world!!"<<endl;
    }
}
std::vector<std::vector<double>> generate_skeleton_data()
{
    //产生随机骨架点，其实是调试用的一定范围的数据点
    int people_num=rand()%4;
    // cout<<people_num<<endl;
    std::vector<std::vector<double>>skeleton_data(people_num,std::vector<double>(keypoints_num*3));
    for(int i=0;i<people_num;i++)
    {
        for(int keypoint_id=0;keypoint_id<keypoints_num;keypoint_id++)
        {
            double x=0;
            double y=rand()%160+160;
            double c=(rand()%100)*0.01;
            switch(i)
            {
                case 0:
                x=rand()%200;
                break;
                case 1:
                x=rand()%200+200;
                break;
                case 2:
                x=rand()%200+400;
                break; 

            }

            skeleton_data[i][keypoint_id*3]=x;
            skeleton_data[i][keypoint_id*3+1]=y;
            skeleton_data[i][keypoint_id*3+2]=c;
            // printf("%d:,%f,%f,%f\n",i,x,y,c);
        }
    }
    return skeleton_data;
}

int main()
{

    int img_row=480;
    int img_col=640;
    Skeleton_Tracking skeleton_tracker(keypoints_num,bones);
    cv::namedWindow("skeleton_track_result",0);
    while(true)
    {
        cv::Mat img=cv::Mat::zeros(img_row,img_col,CV_8UC3);
        std::vector<std::vector<double>>skeleton_data=generate_skeleton_data();
        clock_t start_time = clock();
        skeleton_tracker.skeletons_track(skeleton_data);
        cout << "main time:" << double(clock() - start_time) / CLOCKS_PER_SEC << endl;
        skeleton_tracker.draw_skeletons(img);
        cv::imshow("skeleton_track_result",img);
        cv::waitKey(30);
    }


    
    
    return 0;
}
