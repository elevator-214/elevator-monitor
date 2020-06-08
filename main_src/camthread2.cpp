#include "camthread2.h"
#include <boost/asio.hpp>//gai
#include <OpenPose.hpp>
#include<keypoints_tracking.h>
#include <ctime>

Mat src2;
bool src1Ready = false;//cam2标志位
bool src2Ready = false;//cam4标志位

bool cam2_ppCount=false;
bool no_camera2=false;

extern bool load_cam2_flag;
extern int value_pp2_x;
extern int value_pp2_y;
extern int value_pp2_width;
extern int value_pp2_height;

extern bool SetRoi_pp2_flag;
extern bool draw_trajectory_flag;//是否画行人轨迹标志位
extern bool drawSkeleton_flag;//是否画骨架标志位
extern bool drawID_flag;//是否画id标志位
extern bool drawBox_flag;//是否画检测框标志位
extern bool cam2_start_flag;
// Body parts mapping
const std::map<unsigned int, std::string> POSE_BODY_25_BODY_PARTS {
    {0,  "Nose"},
    {1,  "Neck"},
    {2,  "RShoulder"},
    {3,  "RElbow"},
    {4,  "RWrist"},
    {5,  "LShoulder"},
    {6,  "LElbow"},
    {7,  "LWrist"},
    {8,  "MidHip"},
    {9,  "RHip"},
    {10, "RKnee"},
    {11, "RAnkle"},
    {12, "LHip"},
    {13, "LKnee"},
    {14, "LAnkle"},
    {15, "REye"},
    {16, "LEye"},
    {17, "REar"},
    {18, "LEar"},
    {19, "LBigToe"},
    {20, "LSmallToe"},
    {21, "LHeel"},
    {22, "RBigToe"},
    {23, "RSmallToe"},
    {24, "RHeel"},
    {25, "Background"}
};
const vector<int>keypoint_ids={0,1,2,3,4,5,6,7,9,10,11,12,13,14};//用到的关键点序号
const vector<int>bones={  1,2,   1,5,   2,3,   3,4,   5,6,   6,7,   2,9,   5,12,     9,10,    10,11,    12,13, 13,14,  1,0  };//tensorrt 提取结果的骨架号
const vector<int>bones_resort={1,2, 1,5, 2,3, 3,4, 5,6, 6,7, 2,8, 5,11, 8,9, 9,10, 11,12, 12,13, 1,0};//将骨架号按顺序重新排序后的骨架号
const int keypoints_num=25;//tensorrt提取结果关键点数量
const int keypoints_num_track=keypoint_ids.size();//重新排序后关键点数量
const int value_per_person=keypoints_num*3;
CamThread2 :: CamThread2()
{
    CAMERA_WIDTH = hlg::cam2_CAMERA_WIDTH;
    CAMERA_HEIGHT = hlg::cam2_CAMERA_HEIGHT;
   // CAMERA_NUM = 6;
     CAMERA_NUM = hlg::cam2_CAMERA_NUM;
    CAMERA_STOP = false;
    fmMinue = hlg::cam2_fmMinue;

    //ppROI
    ppROI.x = hlg::cam2_ppROI_x;
    ppROI.y = hlg::cam2_ppROI_y;
    ppROI.width = hlg::cam2_ppROI_width;
    ppROI.height = hlg::cam2_ppROI_height;

    //Activity
    ppCount = false;
}

CamThread2 :: ~CamThread2()
{
    ;
}

void CamThread2 :: run()
{
//    while(true)
//    {
//        if(SetRoi_pp2_flag)
//        {
//            cout<<"cam2"<<endl;
//            SetRoi_pp2_flag=false;
//        }


//    }


    // while(true);
    /************************/
    /*  Frame Patameters	*/
    /************************/
    VideoCapture capture;
    capture.open(CAMERA_NUM);
    while(!capture.isOpened())
    {
        no_camera2=true;
        msleep(1000);
        capture.release();
        capture.open(CAMERA_NUM);
    }
    capture.set(cv::CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);
    no_camera2=false;
    int fmCurrent = 1;
    Mat src,src_camthread2;


    //Judgement
    int ppCount_inc = 0;
    double fps = (double)getTickCount();
    /*开2个线程，第一个线程读取摄像头,并发送给cam4,第二个线程提取关键点*/
    std::vector<std::thread> ths;
    ths.push_back(std::thread([&](){
        while(!CAMERA_STOP)
        {
            capture >> src_camthread2;
            if(!src1Ready)
            {
                src = src_camthread2.clone();
                src1Ready = true;
            }
            if(!src2Ready)
            {
                src2 = src_camthread2.clone();//将第二个摄像头采集的图像发给第四个摄像头
                src2Ready = true;
            }
        }



    }));
    ths.push_back(std::thread([&](){
        const std::string prototxt = "./openpose_model/pose_deploy.prototxt";
        const std::string caffemodel = "./openpose_model/pose_iter_584000.caffemodel";
        const std::string save_engine = "./openpose_model/sample.engine";
        // const std::string img_name = "./test.jpg";
        int run_mode = 0;//0 for float32, 1 for float16, 2 for int8
        int N = 1;
        int C = 3;
        int H = 480;
        int W = 640;
        int maxBatchSize = N;
        std::vector<std::vector<float>> calibratorData;
        calibratorData.resize(3);
        for(size_t i = 0;i<calibratorData.size();i++) {
            calibratorData[i].resize(3*480*640);
            for(size_t j=0;j<calibratorData[i].size();j++) {
                calibratorData[i][j] = 0.05;
            }
        }
        std::vector<std::string> outputBlobname{"net_output"};
        OpenPose openpose(prototxt,
                            caffemodel,
                            save_engine,
                            outputBlobname,
                            calibratorData,
                            maxBatchSize,
                            run_mode);
        hlg::Skeleton_Tracking skeleton_tracker(keypoints_num_track,bones_resort);
        std::vector<float> inputData;
        inputData.resize(N * C * H * W);
        std::vector<float> result;
        Mat dst;
        while(!CAMERA_STOP)
        {
            fps = (double)getTickCount();
            if(!cam2_start_flag)
            {

                putText(dst, "STOP!!!", cv::Point2d(100, 250), FONT_HERSHEY_SIMPLEX, 5, Scalar(0, 0, 255), 5);
                const uchar *pSrc = (const uchar*)dst.data;
                QImage temp(pSrc, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
                qtImage = temp.copy();
                qtImage = qtImage.rgbSwapped();
                emit getImage2(qtImage);
                while(!cam2_start_flag)
                {
                    QThread::msleep(1000);
                    // cout<<"hello"<<endl;
                }

            }
            if(src1Ready)
            {
                src1Ready = false;
                dst=src.clone();
                cv::cvtColor(dst,dst,cv::COLOR_BGR2RGB);
                unsigned char* data = dst.data;
                for(int n=0; n<N;n++) {
                    for(int c=0;c<3;c++) {
                        for(int i=0;i<640*480;i++) {
                            inputData[i+c*640*480+n*3*480*640] = (float)data[i*3+c];
                        }
                    }
                }
                openpose.DoInference(inputData,result);
                cv::cvtColor(dst,dst,cv::COLOR_RGB2BGR);
                // cout<<"result.size()/3:"<<result.size()/3<<endl;
                int people_num=result.size()/value_per_person;
                std::vector<std::vector<double>>keypoints(people_num,std::vector<double>(keypoints_num_track*3));
                double total_confidence=0.0;
                int non_zero_confidence_num=0;
                for(size_t i=0;i<result.size()/value_per_person;++i) {
                    total_confidence=0.0;
                    non_zero_confidence_num=0;
                    for(int j=0;j<keypoints_num_track;++j)
                    {
                        const int index=keypoint_ids[j];
                        const double x=result[value_per_person*i+3*index];
                        const double y=result[value_per_person*i+3*index+1];
                        const double confidence=result[value_per_person*i+3*index+2];
                        keypoints[i][3*j]=x;
                        keypoints[i][3*j+1]=y;
                        keypoints[i][3*j+2]=confidence;
                        if(abs(confidence-0)>0.000001)
                        {
                            total_confidence+=confidence;
                            non_zero_confidence_num+=1;
                        }         
                        // cv::circle(dst,cv::Point(result[i*3],result[i*3+1]),2,cv::Scalar(0,255,0),-1);
                        // putText(dst, to_string(i), cv::Point(result[i*3],result[i*3+1]), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);
                    }
                    cout<<"total_confidence"<<total_confidence/non_zero_confidence_num<<endl;                   
                }
//                std::cout<<"before skeleton_tracker"<<std::endl;
                skeleton_tracker.skeletons_track(keypoints);
//                std::cout<<"after skeleton_tracker"<<std::endl;
                skeleton_tracker.draw_skeletons(dst,0.05);
//                std::cout<<"after skeleton_draw"<<std::endl;
                
              

                
                
                //dst  将检测结果发送给QT
                cv::Mat QT_img;
                cv::resize(dst,QT_img,Size(640,480));//把QT图像归一化到固定大小
                /************************/
                /*  Display Result	将检测结果在图片中显示出来	*/
                /************************/
                Point disPos(5, 0);

                //fps  处理速度
                disPos.y += 20;
                fps = (double)getTickFrequency() / ((double)getTickCount() - fps);
                //cout << "fps:" << fps << endl;
                stringstream ssFps;
                string sFps;
                ssFps << fps;
                ssFps >> sFps;
                sFps = "fps:" + sFps;
                putText(QT_img, sFps, disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);

                //camera location  摄像头安装位置
                disPos.x = 500;
                disPos.y = 20;
                string cam1_location;
                cam1_location = "detect people";
                putText(QT_img, cam1_location, disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 0, 0), 1);

                const uchar *pSrc = (const uchar*)QT_img.data;
                QImage temp(pSrc, QT_img.cols, QT_img.rows, QT_img.step, QImage::Format_RGB888);
                qtImage = temp.copy();
                qtImage = qtImage.rgbSwapped();
                emit getImage2(qtImage);

                //Next Frame
                fmCurrent++;

                
            }

        }


    }));




    for (auto &th : ths) { th.join();}
    cerr << "Camera " << CAMERA_NUM << " Stop!!" << endl;
    return;



//    /************************/
//    /*  Image Processing	*/
//    /************************/
//    while(!CAMERA_STOP)
//    {
//        double fps = (double)getTickCount();
//        capture >> src;
//        while(src.empty())
//        {
//            cerr << "Empty capture, reloading Camera " << CAMERA_NUM << "!!" << endl;
//            capture.release();
//            while(!capture.open(CAMERA_NUM))
//            {
//                no_camera2=true;
//            }
//            capture >> src;
//        }
//        no_camera2=false;
//        dst = src.clone();

//        //ppROI set
//        if (SetRoi_pp2_flag)
//        {
//            if(value_pp2_x>=0 && value_pp2_y>=0 && value_pp2_x + value_pp2_width<=CAMERA_WIDTH && value_pp2_y + value_pp2_height<=CAMERA_HEIGHT)
//            {
//                ppROI.x=value_pp2_x;
//                ppROI.y=value_pp2_y;
//                ppROI.width=value_pp2_width;
//                ppROI.height=value_pp2_height;
//            }
//            else
//            {
//                ppROI.x=0;
//                ppROI.y=0;
//                ppROI.width=CAMERA_WIDTH;
//                ppROI.height=CAMERA_HEIGHT;
//                emit para_error();
//            }
//            SetRoi_pp2_flag = false;
//        }

//        rectangle(dst, ppROI, Scalar(255, 255, 0), 1);


//        /************************/
//        /*  Send src2           */
//        /************************/
//        if(!src2Ready)
//        {
//            src2 = src.clone();
//            src2Ready = true;
//        }

//        /************************/
//        /*  People Monitoring	*/
//        /************************/
//        //Detection
//        vector<Rect> ppDetectionRect;
//        vector<Rect>::const_iterator pPpRect;
//        cascade.detectMultiScale(src(ppROI), ppDetectionRect, 1.1, 5, 0, Size(40, 40), Size(190, 190));
//        for (pPpRect = ppDetectionRect.begin(); pPpRect != ppDetectionRect.end(); pPpRect++)
//            rectangle(dst, Rect((*pPpRect).x + ppROI.x, (*pPpRect).y + ppROI.y, (*pPpRect).width, (*pPpRect).height), Scalar(255, 255, 0), 1);

//        //Judgement
//        ppCount = false;	//count
//        cam2_ppCount=false;
//        if (ppDetectionRect.size())
//            ppCount_inc = fmMinue / 60;
//        else
//        {
//            if (ppCount_inc > 0)
//                ppCount_inc--;
//        }

//            if (ppCount_inc)
//            {
//                 ppCount = true;
//                 cam2_ppCount=true;
//            }

//        /************************/
//        /*  Display Result		*/
//        /************************/
//        Point disPos(5, 0);

//        //ppCount
//        disPos.y += 20;
//        if (ppCount)
//            putText(dst, "pp:yes", disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2);
//        else
//            putText(dst, "pp:no", disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);

//        //fps
//        disPos.y += 20;
//        fps = (double)getTickFrequency() / ((double)getTickCount() - fps);
//        //cout << "fps:" << fps << endl;
//        stringstream ssFps;
//        string sFps;
//        ssFps << fps;
//        ssFps >> sFps;
//        sFps = "fps:" + sFps;
//        putText(dst, sFps, disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);

//        //camera location
//        disPos.x = 500;
//        disPos.y = 20;
//        string cam2_location;
//        cam2_location = "detect people";
//        putText(dst, cam2_location, disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 0, 0), 1);
//        //dst
//        const uchar *pSrc = (const uchar*)dst.data;
//        QImage temp(pSrc, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
//        qtImage = temp.copy();
//        qtImage = qtImage.rgbSwapped();
//        emit getImage2(qtImage);

//        //Next Frame
//        fmCurrent++;
//        fmMinue = fps * 60;

//        //Video Control
//        msleep(1);

//    }

}
