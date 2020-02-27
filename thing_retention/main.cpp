#include<iostream>
#include<opencv2/opencv.hpp>
#include <string>
#include <vector>
#include<ctime>
#include<cmath>
#include<ctime>
#include <mutex>
#include"ThingInterface.h"

using namespace std;
using namespace cv;
using namespace hlg;
//内存泄漏检测
#include <vld.h> 
//内存泄漏检测end

int main(int argc, const char** argv)
{
	string video_path = "E:\\数据集\\扶梯\\地铁\\loucengban\\clip_video\\ch03_20170315060019_31.avi";
	VideoCapture cap;
	if(!cap.open(0))//video_path
        printf("video/camera open failed!!!!!\n");
    //cap.set(CAP_PROP_FRAME_HEIGHT, 480);
    //cap.set(CAP_PROP_FRAME_WIDTH, 640);
    ThingInterface thing_interface;
    thing_interface.create_ForeExtraction(cap.get(CAP_PROP_FRAME_HEIGHT), cap.get(CAP_PROP_FRAME_WIDTH), false);
    thing_interface.create_Thingdetector();
    thing_interface.create_Thingtracker();
    //ForeExtraction fore_extracter();
	Mat inputFrame, foregroundMask;
    
    Rect setROI = Rect(100, 50, int(0.6*int(cap.get(CAP_PROP_FRAME_WIDTH))), int(0.7*int(cap.get(CAP_PROP_FRAME_WIDTH))));
    //clock_t total_time=0;
	for (int i = 0;; i++)
	{
		cap >> inputFrame;//原始大小
		if (inputFrame.empty())
		{
			cout << "Finished reading: empty frame" << endl;
			break;
		}
        clock_t start_time = clock();
        if (thing_interface.fore_ExtractFore(inputFrame, foregroundMask))//提取前景
        {                        
            thing_interface.detect_ThingsDetect(foregroundMask);//从前景中提取出大件物体           
            static once_flag SetOutputCoordScale_flag;//由于在提取的时候，尺寸是固定的，因此输出的坐标需要进行尺度转换
            auto SetOutputCoordScale_fun= std::bind(&ThingInterface::detect_SetOutputCoordScale, std::ref(thing_interface), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);//std::ref：为了只析构1次
            std::call_once(SetOutputCoordScale_flag, SetOutputCoordScale_fun, cap.get(CAP_PROP_FRAME_HEIGHT), cap.get(CAP_PROP_FRAME_WIDTH), thing_interface.fore_getScaledSize());
            static vector<Rect>Thing_Detected;
            static vector<Rect>people_boxes;//由于是测试，所以没有行人检测结果，设为空即可
            thing_interface.detect_Get_Thing_Result(Thing_Detected, people_boxes, setROI);//滤除行人，并进行尺度变换
            thing_interface.track(Thing_Detected);
            cout << "main time:" << double(clock() - start_time) / CLOCKS_PER_SEC << endl;
            //total_time += clock() - start_time;
            const int retention_frame_threshold = 40;//滞留帧数阈值
            const vector<vector<int>>&tracking_result = thing_interface.track_GetThingsInfo();
            for (int i = 0; i < tracking_result.size(); ++i)
            {
                const int  x1 = max(tracking_result[i][0], setROI.x);
                const int  y1 = max(tracking_result[i][1], setROI.y);
                const int x3 = min(tracking_result[i][2], setROI.x+setROI.width-1);
                const int y3 = min(tracking_result[i][3], setROI.y + setROI.height - 1);

                const int &track_frame = tracking_result[i][4];
                if (track_frame > retention_frame_threshold)
                {//达到滞留帧数阈值，用红色画框
                    cv::rectangle(inputFrame, Point(x1, y1),
                        Point(x3, y3),
                        cv::Scalar(0, 0, 255));
                }
                else
                {
                    cv::rectangle(inputFrame, Point(x1, y1),
                        Point(x3, y3),
                        cv::Scalar(0, 255, 255));
                }                   
            } 
            cv::rectangle(inputFrame, setROI, (255, 255, 255), 1);
            namedWindow("result_image", 0);
            imshow("result_image", inputFrame);
            // interact with user
            const char key = (char)waitKey(1);
        }	
	}
    //cout << "total time:" << double(total_time) / CLOCKS_PER_SEC << endl;
	return 0;
}
