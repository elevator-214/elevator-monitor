#include "camthread3.h"
bool cam3_hdDrail=false;
bool no_camera3=false;
extern bool cam3_start_flag;
extern bool load_cam3_flag;
extern int value_hd1_x;
extern int value_hd1_y;
extern int value_hd1_width;
extern int value_hd1_height;
extern int value_hd1_p1_in_x;
extern int value_hd1_p1_in_y;
extern int value_hd1_p2_in_x;
extern int value_hd1_p2_in_y;
extern int value_hd1_p3_in_x;
extern int value_hd1_p3_in_y;
extern int value_hd1_p4_in_x;
extern int value_hd1_p4_in_y;
extern int value_hd1_p1_out_x;
extern int value_hd1_p1_out_y;
extern int value_hd1_p2_out_x;
extern int value_hd1_p2_out_y;
extern int value_hd1_p3_out_x;
extern int value_hd1_p3_out_y;
extern int value_hd1_p4_out_x;
extern int value_hd1_p4_out_y;
extern bool SetRoi_hd1_flag;
CamThread3 :: CamThread3()
{
    CAMERA_WIDTH = hlg::cam3_CAMERA_WIDTH;
    CAMERA_HEIGHT = hlg::cam3_CAMERA_HEIGHT;
   // CAMERA_NUM = 5;
     CAMERA_NUM = hlg::cam3_CAMERA_NUM;

    CAMERA_STOP = false;
    fmMinue = hlg::cam3_fmMinue;

    //hdROI
    hdROI.x = hlg::cam3_hdROI_x;
    hdROI.y = hlg::cam3_hdROI_y;
    hdROI.width = hlg::cam3_hdROI_width;
    hdROI.height = hlg::cam3_hdROI_height;

    //Activity
    hdDrail = false;
}

CamThread3 :: ~CamThread3()
{
    ;
}

void CamThread3 :: run()
{
//    while(true)
//    {
//        if(SetRoi_hd1_flag)
//        {
//            cout<<"cam3"<<endl;
//            SetRoi_hd1_flag=false;
//        }


//    }

    //取一些参数的地址,下面保存参数和加载参数的时候要用到.
    const int cam3_paras_num=24;
    int *cam3_paras[cam3_paras_num]={&(hdROI.x),&(hdROI.y),&(hdROI.width),&(hdROI.height),
                        &(value_hd1_p1_in_x),&(value_hd1_p1_in_y),
                        &(value_hd1_p2_in_x),&(value_hd1_p2_in_y),
                        &(value_hd1_p3_in_x),&(value_hd1_p3_in_y),
                        &(value_hd1_p4_in_x),&(value_hd1_p4_in_y),
                        &(value_hd1_p1_out_x), &(value_hd1_p1_out_y),
                        &(value_hd1_p2_out_x), &(value_hd1_p2_out_y),
                        &(value_hd1_p3_out_x), &(value_hd1_p3_out_y),
                        &(value_hd1_p4_out_x), &(value_hd1_p4_out_y),
                        &value_hd1_x,&value_hd1_y,&value_hd1_width,&value_hd1_height};


    /************************/
    /*  Frame Patameters	*/
    /************************/
    VideoCapture capture;
    while(!capture.open(CAMERA_NUM))
    {
        no_camera3=true;
        msleep(1000);
        capture.release();
    }
    capture.set(cv::CAP_PROP_FRAME_WIDTH, CAMERA_WIDTH);
    capture.set(cv::CAP_PROP_FRAME_HEIGHT, CAMERA_HEIGHT);
    no_camera3=false;
    int fmCurrent = 1;
    Mat src, dst;

    /************************/
    /*  Handrail Patameters	*/
    /************************/
    //Foreground
    Ptr<BackgroundSubtractor> mog = createBackgroundSubtractorMOG2();
    float learning_rate=0.005;
//    BackgroundSubtractorMOG mog(20, 5, 0.5, NULL);
    Mat dstBg, dstFg;
    Mat hdROI_out_mask;
    Mat hdROI_in_mask;

    //Judgement
    vector<Rect> hdDrailRect;

    /************************/
    /*  Image Processing	*/
    /************************/
    while(!CAMERA_STOP)
    {
        if(!cam3_start_flag)//是否设置暂停
        {

            putText(dst, "STOP!!!", cv::Point2d(100, 250), FONT_HERSHEY_SIMPLEX, 5, Scalar(0, 0, 255), 5);
            const uchar *pSrc = (const uchar*)dst.data;
            QImage temp(pSrc, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
            qtImage = temp.copy();
            qtImage = qtImage.rgbSwapped();
            emit getImage3(qtImage);
            while(!cam3_start_flag)
            {
                QThread::msleep(1000);
            }

        }
        double fps = (double)getTickCount();
        capture >> src;
        while(src.empty())
        {
            cerr << "Empty capture, reloading Camera " << CAMERA_NUM << "!!" << endl;
            capture.release();
            while(!capture.open(CAMERA_NUM))
            {
                no_camera3=true;
            }
            capture >> src;
        }
        no_camera3=false;
        dst = src.clone();

        //hdROI set
        if (SetRoi_hd1_flag)
        {
            std::cout<<"value_hd1_height"<<value_hd1_height<<std::endl;
            if(value_hd1_x>=0 && value_hd1_y>=0 && value_hd1_x + value_hd1_width<=CAMERA_WIDTH && value_hd1_y + value_hd1_height<=CAMERA_HEIGHT)
            {
                hdROI.x=value_hd1_x;
                hdROI.y=value_hd1_y;
                hdROI.width=value_hd1_width;
                hdROI.height=value_hd1_height;
                std::cout<<"if"<<std::endl;
            }
            else
            {
//                hdROI.x=0;
//                hdROI.y=0;
//                hdROI.width=CAMERA_WIDTH;
//                hdROI.height=CAMERA_HEIGHT;
                value_hd1_x=hdROI.x;
                value_hd1_y=hdROI.y;
                value_hd1_width=hdROI.width;
                value_hd1_height=hdROI.height;
                std::cout<<"else"<<std::endl;
                emit para_error();
            }

           // if (value_hd1_p1_in_x <0 || value_hd1_p1_out_x >hdROI.width || value_hd1_p2_in_x <0 || value_hd1_p2_out_x >hdROI.width || value_hd1_p3_in_x <0 || value_hd1_p3_out_x >hdROI.width
                   // || value_hd1_p4_in_x <0 || value_hd1_p4_out_x >hdROI.width || value_hd1_p1_in_y <0 || value_hd1_p1_out_y >hdROI.height || value_hd1_p2_in_y <0 || value_hd1_p2_out_y >hdROI.height
                   // || value_hd1_p3_in_y <0 || value_hd1_p3_out_y >hdROI.height || value_hd1_p4_in_y <0 || value_hd1_p4_out_y >hdROI.height)
            if(value_hd1_p1_out_x >hdROI.width || value_hd1_p1_out_x <0)
            {
                value_hd1_p1_out_x = hdROI.width;
                emit para_error();
            }

            if(value_hd1_p2_out_x >hdROI.width || value_hd1_p2_out_x <0)
            {
                value_hd1_p2_out_x = hdROI.width;
                emit para_error();
            }

            if(value_hd1_p3_out_x >hdROI.width || value_hd1_p3_out_x <0)
            {
                value_hd1_p3_out_x = hdROI.width;
                emit para_error();
            }

            if(value_hd1_p4_out_x >hdROI.width || value_hd1_p4_out_x <0)
            {
                value_hd1_p4_out_x = hdROI.width;
                emit para_error();
            }

            if(value_hd1_p1_in_x >hdROI.width || value_hd1_p1_in_x <0)
            {
                value_hd1_p1_in_x = hdROI.width;
                emit para_error();
            }

            if(value_hd1_p2_in_x >hdROI.width || value_hd1_p2_in_x <0)
            {
                value_hd1_p2_in_x = hdROI.width;
                emit para_error();
            }

            if(value_hd1_p3_in_x >hdROI.width || value_hd1_p3_in_x <0)
            {
                value_hd1_p3_in_x = hdROI.width;
                emit para_error();
            }

            if(value_hd1_p4_in_x >hdROI.width || value_hd1_p4_in_x <0)
            {
                value_hd1_p4_in_x = hdROI.width;
                emit para_error();
            }

            if(value_hd1_p1_out_y >hdROI.height || value_hd1_p1_out_y <0)
            {
                value_hd1_p1_out_y = hdROI.height;
                emit para_error();
            }

            if(value_hd1_p2_out_y >hdROI.height || value_hd1_p2_out_y <0)
            {
                value_hd1_p2_out_y = hdROI.height;
                emit para_error();
            }

            if(value_hd1_p3_out_y >hdROI.height || value_hd1_p3_out_y <0)
            {
                value_hd1_p3_out_y = hdROI.height;
                emit para_error();
            }

            if(value_hd1_p4_out_y >hdROI.height || value_hd1_p4_out_y <0)
            {
                value_hd1_p4_out_y = hdROI.height;
                emit para_error();
            }

            if(value_hd1_p1_in_y >hdROI.height || value_hd1_p1_in_y <0)
            {
                value_hd1_p1_in_y = hdROI.height;
                emit para_error();
            }

            if(value_hd1_p2_in_y >hdROI.height || value_hd1_p2_in_y <0)
            {
                value_hd1_p2_in_y = hdROI.height;
                emit para_error();
            }

            if(value_hd1_p3_in_y >hdROI.height || value_hd1_p3_in_y <0)
            {
                value_hd1_p3_in_y = hdROI.height;
                emit para_error();
            }

            if(value_hd1_p4_in_y >hdROI.height || value_hd1_p4_in_y <0)
            {
                value_hd1_p4_in_y = hdROI.height;
                emit para_error();
            }
            //保存参数
            ofstream outfile("./save_cam3.dat",ios::binary);
            int save_cam3[cam3_paras_num];
            for(int i=0;i<cam3_paras_num;i++)
                save_cam3[i]=*cam3_paras[i];
            outfile.write((char*)&save_cam3,sizeof(save_cam3));
            outfile.close();

            SetRoi_hd1_flag = false;
        }
        //加载cam3参数
        if(load_cam3_flag)
        {
            load_cam3_flag=false;
            ifstream infile("./save_cam3.dat",ios::binary);
            if(!infile)
            {
                QMessageBox::about(NULL, "About", "请先保存参数,才能加载!");
            }
            else
            {
                int load_nums[cam3_paras_num];
                infile.read((char*)&load_nums,sizeof(load_nums));
                infile.close();
                for(int i=0;i<cam3_paras_num;i++)
                    *cam3_paras[i]=load_nums[i];
                QMessageBox::about(NULL, "About", "加载参数cam3成功!!!");
            }


        }
//        std::cout<<hdROI.x<<" "<<hdROI.y<<" "<<hdROI.width<<" "<<hdROI.height<<endl;
//        std::cout<<dst.size<<endl;
//        hdROI.x=340;
//        hdROI.y=0;
//        hdROI.width=100;
//        hdROI.height=960;
//        rectangle(dst, hdROI, Scalar(0, 255, 255), 1);
        rectangle(dst, hdROI, Scalar(255,0 ,0), 1);

        vector<Point> hdROI_out_pt;
        vector<vector<Point> > hdROI_out_contour;
        hdROI_out_mask = Mat::zeros(Size(hdROI.width, hdROI.height), CV_8UC1);
        hdROI_out_pt.push_back(Point(value_hd1_p1_in_x, value_hd1_p1_in_y));
        hdROI_out_pt.push_back(Point(value_hd1_p2_in_x, value_hd1_p2_in_y));
        hdROI_out_pt.push_back(Point(value_hd1_p3_in_x, value_hd1_p3_in_y));
        hdROI_out_pt.push_back(Point(value_hd1_p4_in_x, value_hd1_p4_in_y));
        hdROI_out_contour.push_back(hdROI_out_pt);
        drawContours(hdROI_out_mask, hdROI_out_contour, 0, Scalar::all(255), cv::FILLED, 8, 1, 0, Point());

        vector<Point> hdROI_in_pt;
        vector<vector<Point> > hdROI_in_contour;
        hdROI_in_mask = Mat::zeros(Size(hdROI.width, hdROI.height), CV_8UC1);
        hdROI_in_pt.push_back(Point(value_hd1_p1_out_x, value_hd1_p1_out_y));
        hdROI_in_pt.push_back(Point(value_hd1_p2_out_x, value_hd1_p2_out_y));
        hdROI_in_pt.push_back(Point(value_hd1_p3_out_x, value_hd1_p3_out_y));
        hdROI_in_pt.push_back(Point(value_hd1_p4_out_x, value_hd1_p4_out_y));
        hdROI_in_contour.push_back(hdROI_in_pt);
        drawContours(hdROI_in_mask, hdROI_in_contour, 0, Scalar::all(255), cv::FILLED, 8, 1, 0, Point());

        drawContours(dst, hdROI_out_contour, 0, Scalar(255, 0, 255), 1, 8, 1, 0, Point(hdROI.x, hdROI.y));
        drawContours(dst, hdROI_in_contour, 0, Scalar(0, 255, 255), 1, 8, 1, 0, Point(hdROI.x, hdROI.y));

        /************************/
        /*  Handrail Monitoring	*/
        /************************/
        //Foreground
        mog->apply(src(hdROI), dstFg, learning_rate);
        mog->getBackgroundImage(dstBg);
//        mog(src(hdROI), dstFg, 0.01);
//        mog.getBackgroundImage(dstBg);
        medianBlur(dstFg, dstFg, 3);
        morphologyEx(dstFg, dstFg, MORPH_OPEN, NULL);
        vector<vector<Point> > contours = liSegmentFGMask(dstFg.clone());
        dstFg = Mat::zeros(dstFg.size(), CV_8UC1);
        drawContours(dstFg, contours, -1, Scalar::all(255), cv::FILLED, 8, 1, 0, Point());
        //namedWindow("dstFg", CV_WINDOW_AUTOSIZE);
        //imshow("dstFg", dstFg);

        //outside and inside
        Mat dstFg_out, dstFg_in;
        vector<vector<Point> > contours_out, contours_in;
        dstFg.copyTo(dstFg_out, hdROI_out_mask);
        dstFg.copyTo(dstFg_in, hdROI_in_mask);
        findContours(dstFg_out, contours_out, RETR_EXTERNAL, CHAIN_APPROX_NONE);
        findContours(dstFg_in, contours_in, RETR_EXTERNAL, CHAIN_APPROX_NONE);
        //namedWindow("dstFg_out", CV_WINDOW_AUTOSIZE);
        //imshow("dstFg_out", dstFg_out);
        //namedWindow("dstFg_in", CV_WINDOW_AUTOSIZE);
        //imshow("dstFg_in", dstFg_in);

        //Judgement
        hdDrail = false;
        cam3_hdDrail=false;
        hdDrailRect.clear();
        for (int i = 0; i < contours_out.size(); i++)
        {
            Rect hdRect = boundingRect(contours_out[i]);
            //mask dstFg_in again according to hdRect in dstFg_out
            Mat this_in_mask = Mat::zeros(Size(hdROI.width, hdROI.height), CV_8UC1);
            rectangle(this_in_mask, Rect(0, hdRect.y, hdROI.width, hdRect.height), Scalar::all(255), 1);
            Mat this_in;
            dstFg_in.copyTo(this_in, this_in_mask);
            vector<vector<Point> > this_in_contours;
            findContours(this_in, this_in_contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
            if (this_in_contours.size())
            {
                hdDrailRect.push_back(hdRect);
                hdDrail = true;
                cam3_hdDrail=true;
            }
        }

        for (int i = 0; i < hdDrailRect.size(); i++)
            rectangle(dst, Rect(hdDrailRect[i].x + hdROI.x, hdDrailRect[i].y + hdROI.y, hdDrailRect[i].width, hdDrailRect[i].height), Scalar(0, 0, 255), 2);

        /************************/
        /*  Display Result		*/
        /************************/
        Point disPos(5, 0);

        //hdDrail
        disPos.y += 20;
        if (hdDrail)
            putText(dst, "hdDrail:yes", disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2);
        else
            putText(dst, "hdDrail:no", disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);

        //fps
        disPos.y += 20;
        fps = (double)getTickFrequency() / ((double)getTickCount() - fps);
        //cout << "fps:" << fps << endl;
        stringstream ssFps;
        string sFps;
        ssFps << fps;
        ssFps >> sFps;
        sFps = "fps:" + sFps;
        putText(dst, sFps, disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);

        //camera location
        disPos.x = 520;
        disPos.y = 20;
        string cam3_location;
        cam3_location = "right drail";
        putText(dst, cam3_location, disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 0, 0), 1);

        //dst
        const uchar *pSrc = (const uchar*)dst.data;
        QImage temp(pSrc, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
        qtImage = temp.copy();
        qtImage = qtImage.rgbSwapped();
        emit getImage3(qtImage);

        //Next Frame
        fmCurrent++;

        //Video Control
        msleep(1);

    }
    cerr << "Camera " << CAMERA_NUM << " Stop!!" << endl;
    return;

}
