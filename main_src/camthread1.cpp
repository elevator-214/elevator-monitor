#include "camthread1.h"

bool cam1_ppCount=false;
bool cam1_ppRetent=false;
bool cam1_thRetent=false;
bool no_camera1=false;
int cam1_ppFlow30s=0;

extern bool load_cam1_flag;

extern int value_pp1_x;
extern int value_pp1_y;
extern int value_pp1_width;
extern int value_pp1_height;
extern int value_pp1_flowline;


extern bool SetRoi_pp1_flag;
extern int value_th_x;
extern int value_th_y;
extern int value_th_width;
extern int value_th_height;
extern bool SetRoi_th_flag;

extern bool cam1_start_flag;

CamThread1 :: CamThread1()
{
    CAMERA_WIDTH = hlg::cam1_CAMERA_WIDTH;
    CAMERA_HEIGHT = hlg::cam1_CAMERA_HEIGHT;
//    CAMERA_WIDTH = 1280;
//    CAMERA_HEIGHT = 720;
    CAMERA_NUM =hlg::cam1_CAMERA_NUM;
    VideoPath =hlg::cam1_VideoPath;
    CAMERA_STOP = false;

    //加载yolo长宽
    
    yolo_width=hlg::cam1_yolo_width;
    yolo_height=hlg::cam1_yolo_height;
    print_yolo_width_height_flag=hlg::cam1_print_yolo_width_height_flag;

    //ppROI
    ppROI.x = hlg::cam1_ppROI_x;;
    ppROI.y = hlg::cam1_ppROI_y;;
    ppROI.width = hlg::cam1_ppROI_width;
    ppROI.height = hlg::cam1_ppROI_height;
    ppFlowLine = hlg::cam1_ppFlowLine;

    //thROI
    thROI.x = hlg::cam1_thROI_x;
    thROI.y = hlg::cam1_thROI_y;
    thROI.width = hlg::cam1_thROI_width;         //360
    thROI.height = hlg::cam1_thROI_height;         //340

    //Activity
    ppCount = 0;
    ppRetent = false;

    ppFlowIn = 0;
    ppFlowOut = 0;
    ppFlow30s = 0;
    thRetent = false;
}

CamThread1 :: ~CamThread1()
{
    ;
}

void CamThread1 :: run()
{
    //while(true);
    //取一些参数的地址,下面保存参数和加载参数的时候要用到.
    const int cam1_paras_pp_num=5;
    int *cam1_pp_paras[cam1_paras_pp_num*2]={&(ppROI.x),&(ppROI.y),&(ppROI.width),&(ppROI.height),(&ppFlowLine),
                                             &value_pp1_x,&value_pp1_y,&value_pp1_width,&value_pp1_height,&value_pp1_flowline};
    const int cam1_paras_th_num=4;
    int *cam1_th_paras[cam1_paras_th_num*2]={&(thROI.x),&(thROI.y),&(thROI.width),&(thROI.height),
                                             &value_th_x,&value_th_y,&value_th_width,&value_th_height};

    /************************/
    /*  Frame Patameters	*/
    /************************/
    cv::VideoCapture capture;
    while(!capture.open(CAMERA_NUM))//"./ch03_20170315060019_31.avi"
    {
        no_camera1=true;
        msleep(1000);
        capture.release();
    }
    capture.set(cv::CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);
    no_camera1=false;
    unsigned int fmCurrent = 1;
    Mat src, dst;

    /************************/
    /*  People Patameters	*/
    /************************/
    /*加载yolo参数*/
    std::vector<std::string> class_names;
    {
        std::ifstream class_file("./yolov3/classes.txt");
        if (!class_file)
        {
            std::cerr << "failed to open classes.txt\n";
            return;
        }
        class_names.assign(std::istream_iterator<std::string>(class_file), {});
    }
//    auto net = cv::dnn::readNetFromDarknet("./yolov3/yolov3.cfg", "./yolov3/yolov3.weights");
    auto net = cv::dnn::readNetFromDarknet("./yolov3/yolov3-tiny.cfg", "./yolov3/yolov3-tiny_train_76500.weights");
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_DEFAULT);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    std::cout<<"加载yolov3参数成功!"<<std::endl;
    auto output_names = net.getUnconnectedOutLayersNames();
    cv::Mat blob;
    std::vector<cv::Mat> detections;
    /*加载yolo参数end*/


    //楼层板人头Tracking
    PeopleKalmanTracker ppTracker(60, "pp");
    //客流量以及滞留行为judgement
    const unsigned int ppRetent_T = 60;//滞留判断帧数
    int ppFlowIn30s_last = 0;
    int ppFlowOut30s_last = 0;
    int ppFlowIn30s_inc = 0;
    int ppFlowOut30s_inc = 0;

    /*  开启两个线程,加快算法处理速度	*/
    const int channel_size=2;//缓冲区大小
    channel<cv::Mat> ch(channel_size);
    std::vector<std::thread> ths;
    cv::Mat img;//第一个线程用于存储摄像头图像的img
    /*加载多进程参数end*/

    /*第一个线程用来读取摄像头图片*/
    ths.push_back(std::thread([&](){
        while(!CAMERA_STOP)
        {
            if(!cam1_start_flag)//如果摄像头暂停
            {

                cv::putText(dst, "STOP!!!", cv::Point2d(100, 250), FONT_HERSHEY_SIMPLEX, 5, Scalar(0, 0, 255), 5);
                const uchar *pSrc = (const uchar*)dst.data;
                QImage temp(pSrc, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
                qtImage = temp.copy();
                qtImage = qtImage.rgbSwapped();
                emit getImage1(qtImage);
                while(!cam1_start_flag)
                {
                    QThread::msleep(1000);
                }
            }
            capture>>img;
            while(img.empty())
            {
                cerr << "Empty capture, reloading Camera " << CAMERA_NUM << "!!" << endl;
                capture.release();
                while(!capture.open(CAMERA_NUM))
                {
                    no_camera1=true;
                }
                capture.read(img);
            }
            cv::resize(img,img,Size(CAMERA_WIDTH,CAMERA_HEIGHT));
//            auto total_start = std::chrono::steady_clock::now();
            ch.put(img);
            // cout<<"puting!"<<endl;
        //    std::this_thread::sleep_for(100ms);
//            auto total_end = std::chrono::steady_clock::now();
//            float cost_ms=std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start).count();
//            std::cout<<cost_ms<<"ms"<<std::endl;
        }


    }));
    /*第二个线程用来算法处理,*/
    ths.push_back(std::thread([&](){
        constexpr float confidence_threshold = 0.7;
        constexpr float nms_threshold = 0.1;
        constexpr int num_classes = 80;
        /************************/
        /*  Thing Patameters	*/
        /************************/
        //物体的前景提取，跟踪，以及滞留判断
        hlg::ThingInterface thing_interface;
        thing_interface.create_ForeExtraction(capture.get(CAP_PROP_FRAME_HEIGHT), capture.get(CAP_PROP_FRAME_WIDTH), false);
        thing_interface.create_Thingdetector();
        thing_interface.create_Thingtracker();
        cv::Mat th_image, foregroundMask;
        const int retention_frame_threshold = 60;//滞留帧数阈值
        while(true)
        {
            
            src=ch.get();
            double fps = (double)getTickCount();
            dst = src.clone();
//            auto total_start = std::chrono::steady_clock::now();

            /*行人检测ROI参数设置*/
            if (SetRoi_pp1_flag)
            {
                print_yolo_width_height_flag=true;
                if(value_pp1_x>=0 && value_pp1_y>=0 && value_pp1_x + value_pp1_width<=CAMERA_WIDTH && value_pp1_y + value_pp1_height<=CAMERA_HEIGHT && value_pp1_flowline >=value_pp1_y && value_pp1_flowline <=value_pp1_y + value_pp1_height)
                {
                    ppROI.x=value_pp1_x;
                    ppROI.y=value_pp1_y;
                    ppROI.width=value_pp1_width;
                    ppROI.height=value_pp1_height;
                    ppFlowLine = value_pp1_flowline;

                }
                else
                {
                    if(value_pp1_x<0||value_pp1_x>CAMERA_WIDTH)
                        ppROI.x=value_pp1_x=105;
                    if(value_pp1_y<0||value_pp1_y>CAMERA_HEIGHT)
                        ppROI.y=value_pp1_y=30;
                    if(value_pp1_x + value_pp1_width>CAMERA_WIDTH)
                        ppROI.width=value_pp1_width=CAMERA_WIDTH-value_pp1_x;
                    if(value_pp1_y+value_pp1_height>CAMERA_HEIGHT)
                        ppROI.height=value_pp1_height=CAMERA_HEIGHT-value_pp1_y;
                    if(ppFlowLine<value_pp1_y||ppFlowLine>value_pp1_y + value_pp1_height)
                        ppFlowLine=value_pp1_flowline=(ppROI.height)/2+ppROI.y;
                    emit para_error();
                }
                //保存ROI参数
                ofstream outfile("./save_cam1_pp.dat",ios::binary);
                int save_cam1_pp[cam1_paras_pp_num];
                for(int i=0;i<cam1_paras_pp_num;i++)
                    save_cam1_pp[i]=*cam1_pp_paras[i];
                outfile.write((char*)&save_cam1_pp,sizeof(save_cam1_pp));
                outfile.close();

                SetRoi_pp1_flag = false;
                //cout << "set pp1 roi success" << endl;
            }

            /*大件物体检测ROI设置*/
            if (SetRoi_th_flag)
            {
                if(value_th_x>=0 && value_th_y>=0 && value_th_x + value_th_width<=CAMERA_WIDTH && value_th_y + value_th_height<=CAMERA_HEIGHT)
                {
                    thROI.x=value_th_x;
                    thROI.y=value_th_y;
                    thROI.width=value_th_width;
                    thROI.height=value_th_height;
                }
                else
                {
                    if(value_th_x<0||value_th_x>CAMERA_WIDTH)
                        thROI.x=value_pp1_x=100;
                    if(value_th_y<0||value_th_y>CAMERA_HEIGHT)
                        thROI.y=value_pp1_y=10;
                    if(value_th_x + value_th_width>CAMERA_WIDTH)
                        thROI.width=value_th_width=CAMERA_WIDTH-value_th_x;
                    if(value_th_y+value_th_height>CAMERA_HEIGHT)
                        thROI.height=value_th_height=CAMERA_HEIGHT-value_th_y;
                    emit para_error();
                }
                //保存物体ROI参数
                ofstream outfile("./save_cam1_th.dat",ios::binary);
                int save_cam1_th[cam1_paras_th_num];
                for(int i=0;i<cam1_paras_th_num;i++)
                    save_cam1_th[i]=*cam1_th_paras[i];
                outfile.write((char*)&save_cam1_th,sizeof(save_cam1_th));
                outfile.close();

                SetRoi_th_flag = false;
                
            }
            //加载cam1的各种ROI参数
            if(load_cam1_flag)
            {
                load_cam1_flag=false;
                print_yolo_width_height_flag=true;
                ifstream infile_pp("./save_cam1_pp.dat",ios::binary);
                ifstream infile_th("./save_cam1_th.dat",ios::binary);
                if(!infile_pp||!infile_th)
                {
                    cout<<"请先保存参数,才能加载!"<<endl;
                }
                else
                {
                    int load_nums_pp[cam1_paras_pp_num],load_nums_th[cam1_paras_th_num];
                    infile_pp.read((char*)&load_nums_pp,sizeof(load_nums_pp));
                    infile_pp.close();
                    infile_th.read((char*)&load_nums_th,sizeof(load_nums_th));
                    infile_th.close();
                    for(int i=0;i<cam1_paras_pp_num;i++)
                        *cam1_pp_paras[i]=*cam1_pp_paras[i+cam1_paras_pp_num]=load_nums_pp[i];
                    for(int i=0;i<cam1_paras_th_num;i++)
                        *cam1_th_paras[i]=*cam1_th_paras[i+cam1_paras_th_num]=load_nums_th[i];
                    
                }
            }
            /*画出ROI*/
            rectangle(dst, ppROI, Scalar(255, 255, 0), 1);
            line(dst, Point(ppROI.x, ppFlowLine), Point(ppROI.x + ppROI.width, ppFlowLine), Scalar(255, 255, 0), 1);
            rectangle(dst, thROI, Scalar(0, 255, 255), 1);
        
            //yolo检测人头
            cv::Mat yolo_image=src(ppROI);
            if(print_yolo_width_height_flag)
            {
                print_yolo_width_height_flag=false;
                if(yolo_image.rows>yolo_image.cols)
                {
                    unsigned int suggest_height=round(7.0*yolo_image.rows/yolo_image.cols)*32;
                    if(yolo_width!=224||yolo_height!=suggest_height)
                        cout<<"建议yolo_width:224"<<" 建议yolo_height:"<<suggest_height<<endl;
                }
                else
                {
                    unsigned int suggest_width=round(7.0*yolo_image.cols/yolo_image.rows)*32;
                    if(yolo_height!=224||yolo_width!=suggest_width)
                        cout<<"建议yolo_width:"<<suggest_width<<" 建议yolo_height:224"<<endl;
                }
            }

            //由于训练的时候,图像尺寸为较小.为了迎合训练图片,达到较好的检测效果,且ROI也接近1:1,因此把图片缩小到224*224
//            cv::dnn::blobFromImage(frame, blob, 0.00392, cv::Size(288, 224), cv::Scalar(), true, false, CV_32F);
            cv::dnn::blobFromImage(yolo_image, blob, 0.00392, cv::Size(yolo_width, yolo_height), cv::Scalar(), true, false, CV_32F);
            net.setInput(blob);

//            auto dnn_start = std::chrono::steady_clock::now();
            net.forward(detections, output_names);
//            auto dnn_end = std::chrono::steady_clock::now();

            std::vector<cv::Rect> boxes;
            std::vector<int> class_id;
            std::vector<float> scores;
            for (const auto& output : detections)
            {
                const auto num_boxes = output.rows;
                for (size_t i = 0; i < num_boxes; i++)
                {
                    auto itr = std::max_element(output.ptr<float>(i, 5), output.ptr<float>(i, 5 + num_classes));
                    auto confidence = *itr;
                    auto classid = itr - output.ptr<float>(i, 5);
                    if (confidence >= confidence_threshold&&classid==0)//只显示行人类
                    {
                        const auto &x = output.at<float>(i, 0) * yolo_image.cols;
                        const auto &y = output.at<float>(i, 1) * yolo_image.rows;
                        const auto &width = output.at<float>(i, 2) * yolo_image.cols;
                        const auto &height = output.at<float>(i, 3) * yolo_image.rows;
                        const cv::Rect rect(x - width/2, y - height/2, width, height);
                        boxes.push_back(rect);
                        class_id.push_back(classid);
                        scores.push_back(confidence);
                    }
                }
            }
            std::vector<int> indices;
            cv::dnn::NMSBoxes(boxes, scores, 0.0, nms_threshold, indices);//NMS
            std::vector<cv::Rect>ppDetectionRect;
            for (size_t i = 0; i < indices.size(); ++i)
            {
                auto idx = indices[i];
                ppDetectionRect.push_back(boxes[idx]);
                
            }
            /*YOLOv3检测结束*/

            //将检测框中心点还原到原图像中的坐标
            std::vector<cv::Point2f> ppDetection;
            for (int i =0;i< ppDetectionRect.size(); ++i)
            {
                ppDetectionRect[i].x=max(ppDetectionRect[i].x+ppROI.x,0);
                ppDetectionRect[i].y=max(ppDetectionRect[i].y+ppROI.y,0);
                ppDetection.push_back(Point2f(ppDetectionRect[i].x + ppDetectionRect[i].width / 2 , ppDetectionRect[i].y + ppDetectionRect[i].height / 2));
            }

            //对检测结果进行修正,这个函数感觉需要根据模型检测结果修改---HLG
            vector<Point2f> ppMeasurement=ppDetection;
            // cout<<"begin pp track"<<endl;
            //Tracking 修正行人卡尔曼滤波位置
            ppTracker.track(ppMeasurement,ppDetectionRect);//ppMeasurement为检测中心点 ppDetectionRect检测矩形框
            /*判断行人滞留,统计人流量等功能 modified by HLG*/
            ppRetent = false;
            cam1_ppRetent=false;
            ppCount = ppTracker.target.size();	//count
            
            //滞留判断 画不同颜色矩形框
            static vector<Rect>people_boxes;//存储行人框，用于后面过滤大件物体
            people_boxes.resize(ppCount);
            for (int i = 0; i < ppCount; ++i)
            {
                //retention
                stringstream ssid;
                string sid;
                ssid << ppTracker.target[i].id;
                ssid >> sid;
                people_boxes[i]=ppTracker.target[i].box;
                if (ppTracker.target[i].track_frame > ppRetent_T)
                {
                    Rect box=Rect(int(ppTracker.target[i].position().x-(ppTracker.target[i].box.width)/2),int(ppTracker.target[i].position().y-(ppTracker.target[i].box.height)/2),ppTracker.target[i].box.width,ppTracker.target[i].box.height);
                    cv::rectangle(dst,box,Scalar(0, 0, 255), 2);
                    putText(dst, sid, ppTracker.target[i].position(), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2);
                    ppRetent = true;
                    cam1_ppRetent=true;
                }
                else
                {
                    Rect box=Rect(int(ppTracker.target[i].position().x-(ppTracker.target[i].box.width)/2),int(ppTracker.target[i].position().y-(ppTracker.target[i].box.height)/2),ppTracker.target[i].box.width,ppTracker.target[i].box.height);
                    cv::rectangle(dst,box,Scalar(255, 255, 0), 2);
                    putText(dst, sid, ppTracker.target[i].position(), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 0), 1);
                }

            }

            //flow 人流量进出判断 30秒人流量统计
            ppFlowIn=ppTracker.up_to_down_count;//总从上到下人流量,一直增长,不置0
            ppFlowOut=ppTracker.down_to_up_count;
            ppFlowIn30s_inc=ppFlowIn-ppFlowIn30s_last;
            ppFlowOut30s_inc=ppFlowOut-ppFlowOut30s_last;
            ppFlow30s=abs(ppFlowIn30s_inc - ppFlowOut30s_inc);
            //每隔30秒清一次0
            if (fmCurrent % (fmMinue / 2) == 0)
            {
                ppFlowIn30s_last=ppFlowIn;
                ppFlowOut30s_last=ppFlowOut;
            }


            /************************/
            /*  Thing Monitoring 大件物品滞留检测	*/
            /************************/
            //Foreground
            thRetent = false;
            cam1_thRetent=false;//与电梯控制器通信
            th_image=src.clone();

            if (thing_interface.fore_ExtractFore(th_image, foregroundMask))//提取前景
            {                  
                thing_interface.detect_ThingsDetect(foregroundMask);//从前景中提取出大件物体           
                static once_flag SetOutputCoordScale_flag;//由于在提取的时候，尺寸是固定的，因此输出的坐标需要进行尺度转换
                auto SetOutputCoordScale_fun= std::bind(&hlg::ThingInterface::detect_SetOutputCoordScale, std::ref(thing_interface), std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);//std::ref：为了只析构1次
                std::call_once(SetOutputCoordScale_flag, SetOutputCoordScale_fun, capture.get(CAP_PROP_FRAME_HEIGHT), capture.get(CAP_PROP_FRAME_WIDTH), thing_interface.fore_getScaledSize());
                static vector<Rect>Thing_Detected;
                thing_interface.detect_Get_Thing_Result(Thing_Detected,thROI);//滤除行人，并进行尺度变换
                thing_interface.track(Thing_Detected, people_boxes);
 
                // cout << "main time:" << double(clock() - start_time) / CLOCKS_PER_SEC << endl;

                const vector<vector<int>>&tracking_result = thing_interface.track_GetThingsInfo();
                //滞留检测 画图像
 
                for (int i = 0; i < tracking_result.size(); ++i)
                {   
                    const int x1 = max(tracking_result[i][0], thROI.x);
                    const int y1 = max(tracking_result[i][1], thROI.y);
                    const int x3 = min(tracking_result[i][2], thROI.x+thROI.width-1);
                    const int y3 = min(tracking_result[i][3], thROI.y + thROI.height - 1);
                    const Rect rect(x1,y1,x3-x1+1,y3-y1+1);
                    const int &track_frame = tracking_result[i][4];
                    const int &id = tracking_result[i][5];
                    stringstream ssid;
                    string sid;
                    ssid << id;
                    ssid >> sid;
                    if (track_frame > retention_frame_threshold)
                    {//达到滞留帧数阈值，用红色画框,否则用品红色画框
                        cv::rectangle(dst, rect,cv::Scalar(0, 0, 255));
                        putText(dst, sid,cv::Point((x1+x3)/2,(y1+y3)/2), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 1);
                        thRetent = true;
                        cam1_thRetent=true;
                    }
                    else
                    {
                        cv::rectangle(dst, rect,cv::Scalar(255, 0, 255));
                        putText(dst, sid, cv::Point((x1+x3)/2,(y1+y3)/2), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 0, 255), 1);
                    }                   
                }

            }


            //dst  将检测结果发送给QT
            cv::Mat QT_img;
            cv::resize(dst,QT_img,Size(640,480));//把QT图像归一化到固定大小
            /************************/
            /*  Display Result	将检测结果在图片中显示出来	*/
            /************************/
            Point disPos(5, 0);

            //ppCount 行人数量
            disPos.y += 20;
            if (ppCount>0)
                putText(QT_img, "pp:yes", disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2);
            else
                putText(QT_img, "pp:no", disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);

            //ppRetent 行人是否滞留
            disPos.y += 20;
            if (ppRetent)
                putText(QT_img, "ppRetent:yes", disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2);
            else
                putText(QT_img, "ppRetent:no", disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);

            //ppFlowIn人流进入
            disPos.y += 20;
            stringstream ssPpFlowIn;
            string sPpFlowIn;
            ssPpFlowIn << ppFlowIn;
            ssPpFlowIn >> sPpFlowIn;
            sPpFlowIn = "ppIn:" + sPpFlowIn;
            putText(QT_img, sPpFlowIn, disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);

            //ppFlowOut 人流走出
            disPos.y += 20;
            stringstream ssPpFlowOut;
            string sPpFlowOut;
            ssPpFlowOut << ppFlowOut;
            ssPpFlowOut >> sPpFlowOut;
            sPpFlowOut = "ppOut:" + sPpFlowOut;
            putText(QT_img, sPpFlowOut, disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);

            //ppFlow30s  30秒人流量
            disPos.y += 20;
            stringstream ssPpFlow30s;
            string sPpFlow30s;
            ssPpFlow30s << ppFlow30s;
            ssPpFlow30s >> sPpFlow30s;
            sPpFlow30s = "pp30s:" + sPpFlow30s;
            putText(QT_img, sPpFlow30s, disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);

            //thRetent   大件物品滞留
            disPos.y += 20;
            if (thRetent)
                putText(QT_img, "thRetent:yes", disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2);
            else
                putText(QT_img, "thRetent:no", disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);

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
            disPos.x = 520;
            disPos.y = 20;
            string cam1_location;
            cam1_location = "floor board";
            putText(QT_img, cam1_location, disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 0, 0), 1);

            const uchar *pSrc = (const uchar*)QT_img.data;
            QImage temp(pSrc, QT_img.cols, QT_img.rows, QT_img.step, QImage::Format_RGB888);
            qtImage = temp.copy();
            qtImage = qtImage.rgbSwapped();
            emit getImage1(qtImage);

            //Next Frame
            fmCurrent++;
            //Video Control
//            std::this_thread::sleep_for(150ms);
            msleep(1);
            // cout<<"begin next frame"<<endl; 
        }
    }));

    for (auto &th : ths) { th.join();}
    cerr << "Camera " << CAMERA_NUM << " Stop!!" << endl;
    capture.release();
    return;

}
