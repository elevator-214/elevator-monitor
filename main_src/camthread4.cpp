#include "camthread4.h"

extern Mat src2;
extern bool src2Ready;
extern bool cam4_start_flag;
extern bool load_cam4_flag;
extern int value_hd2_x;
extern int value_hd2_y;
extern int value_hd2_width;
extern int value_hd2_height;
extern int value_hd2_p1_in_x;
extern int value_hd2_p1_in_y;
extern int value_hd2_p2_in_x;
extern int value_hd2_p2_in_y;
extern int value_hd2_p3_in_x;
extern int value_hd2_p3_in_y;
extern int value_hd2_p4_in_x;
extern int value_hd2_p4_in_y;
extern int value_hd2_p1_out_x;
extern int value_hd2_p1_out_y;
extern int value_hd2_p2_out_x;
extern int value_hd2_p2_out_y;
extern int value_hd2_p3_out_x;
extern int value_hd2_p3_out_y;
extern int value_hd2_p4_out_x;
extern int value_hd2_p4_out_y;
extern bool SetRoi_hd2_flag;
bool cam4_hdDrail=false;
CamThread4 :: CamThread4()
{
    CAMERA_WIDTH = hlg::cam4_CAMERA_WIDTH;
    CAMERA_HEIGHT = hlg::cam4_CAMERA_HEIGHT;
   // CAMERA_NUM = 6;
//    CAMERA_NUM = 1;
    CAMERA_NUM = hlg::cam4_CAMERA_NUM;
    CAMERA_STOP = hlg::cam4_CAMERA_WIDTH;
    fmMinue =hlg::cam4_fmMinue;

    //hdROI
    hdROI.x = hlg::cam4_hdROI_x;
    hdROI.y = hlg::cam4_hdROI_y;
    hdROI.width = hlg::cam4_hdROI_width;
    hdROI.height = hlg::cam4_hdROI_height;

    //Activity
    hdDrail = false;
}

CamThread4 :: ~CamThread4()
{
    ;
}

void CamThread4 :: run()
{
    while(true);
//    while(true)
//    {
////        cout<<"cam4"<<endl;
//        if(SetRoi_hd2_flag)
//        {
//            cout<<"cam4"<<endl;
//            SetRoi_hd2_flag=false;
//        }


//    }

    //取一些参数的地址,下面保存参数和加载参数的时候要用到.
    const int cam4_paras_num=24;
    int *cam4_paras[cam4_paras_num]={&(hdROI.x),&(hdROI.y),&(hdROI.width),&(hdROI.height),
                        &(value_hd2_p1_in_x),&(value_hd2_p1_in_y),
                        &(value_hd2_p2_in_x),&(value_hd2_p2_in_y),
                        &(value_hd2_p3_in_x),&(value_hd2_p3_in_y),
                        &(value_hd2_p4_in_x),&(value_hd2_p4_in_y),
                        &(value_hd2_p1_out_x), &(value_hd2_p1_out_y),
                        &(value_hd2_p2_out_x), &(value_hd2_p2_out_y),
                        &(value_hd2_p3_out_x), &(value_hd2_p3_out_y),
                        &(value_hd2_p4_out_x), &(value_hd2_p4_out_y),
                        &value_hd2_x,&value_hd2_y,
                        &value_hd2_width,&value_hd2_height};
    int fmCurrent = 1;
    Mat dst;

    /************************/
    /*  Handrail Patameters	*/
    /************************/
    //Foreground
    Ptr<BackgroundSubtractor> mog = createBackgroundSubtractorMOG2();
    float learning_rate=0.005;


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

        if(!cam4_start_flag)
        {

            putText(dst, "STOP!!!", cv::Point2d(100, 250), FONT_HERSHEY_SIMPLEX, 5, Scalar(0, 0, 255), 5);
            const uchar *pSrc = (const uchar*)dst.data;
            QImage temp(pSrc, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
            qtImage = temp.copy();
            qtImage = qtImage.rgbSwapped();
            emit getImage4(qtImage);
            while(!cam4_start_flag)
            {
                QThread::msleep(1000);
                // cout<<"hello"<<endl;
            }

        }

        /************************/
        /*  Get src2            */
        /************************/
        if(src2Ready)
        {
            double fps = (double)getTickCount();

            dst = src2.clone();

            //hdROI set
            if (SetRoi_hd2_flag)
            {
                if(value_hd2_x>=0 && value_hd2_y>=0 && value_hd2_x + value_hd2_width<=CAMERA_WIDTH && value_hd2_y + value_hd2_height<=CAMERA_HEIGHT)
                {
                    hdROI.x=value_hd2_x;
                    hdROI.y=value_hd2_y;
                    hdROI.width=value_hd2_width;
                    hdROI.height=value_hd2_height;
                }
                else
                {
//                    hdROI.x=0;
//                    hdROI.y=0;
//                    hdROI.width=CAMERA_WIDTH;
//                    hdROI.height=CAMERA_HEIGHT;
                    value_hd2_x=hdROI.x;
                    value_hd2_y=hdROI.y;
                    value_hd2_width=hdROI.width;
                    value_hd2_height=hdROI.height;
//                    std::cout<<"else"<<std::endl;
                    emit para_error();
                    emit para_error();
                }

                if(value_hd2_p1_out_x >hdROI.width || value_hd2_p1_out_x <0)
                {
                    value_hd2_p1_out_x = hdROI.width;
                    emit para_error();
                }

                if(value_hd2_p2_out_x >hdROI.width || value_hd2_p2_out_x <0)
                {
                    value_hd2_p2_out_x = hdROI.width;
                    emit para_error();
                }

                if(value_hd2_p3_out_x >hdROI.width || value_hd2_p3_out_x <0)
                {
                    value_hd2_p3_out_x = hdROI.width;
                    emit para_error();
                }

                if(value_hd2_p4_out_x >hdROI.width || value_hd2_p4_out_x <0)
                {
                    value_hd2_p4_out_x = hdROI.width;
                    emit para_error();
                }

                if(value_hd2_p1_in_x >hdROI.width || value_hd2_p1_in_x <0)
                {
                    value_hd2_p1_in_x = hdROI.width;
                    emit para_error();
                }

                if(value_hd2_p2_in_x >hdROI.width || value_hd2_p2_in_x <0)
                {
                    value_hd2_p2_in_x = hdROI.width;
                    emit para_error();
                }

                if(value_hd2_p3_in_x >hdROI.width || value_hd2_p3_in_x <0)
                {
                    value_hd2_p3_in_x = hdROI.width;
                    emit para_error();
                }

                if(value_hd2_p4_in_x >hdROI.width || value_hd2_p4_in_x <0)
                {
                    value_hd2_p4_in_x = hdROI.width;
                    emit para_error();
                }

                if(value_hd2_p1_out_y >hdROI.height || value_hd2_p1_out_y <0)
                {
                    value_hd2_p1_out_y = hdROI.height;
                    emit para_error();
                }

                if(value_hd2_p2_out_y >hdROI.height || value_hd2_p2_out_y <0)
                {
                    value_hd2_p2_out_y = hdROI.height;
                    emit para_error();
                }

                if(value_hd2_p3_out_y >hdROI.height || value_hd2_p3_out_y <0)
                {
                    value_hd2_p3_out_y = hdROI.height;
                    emit para_error();
                }

                if(value_hd2_p4_out_y >hdROI.height || value_hd2_p4_out_y <0)
                {
                    value_hd2_p4_out_y = hdROI.height;
                    emit para_error();
                }

                if(value_hd2_p1_in_y >hdROI.height || value_hd2_p1_in_y <0)
                {
                    value_hd2_p1_in_y = hdROI.height;
                    emit para_error();
                }

                if(value_hd2_p2_in_y >hdROI.height || value_hd2_p2_in_y <0)
                {
                    value_hd2_p2_in_y = hdROI.height;
                    emit para_error();
                }

                if(value_hd2_p3_in_y >hdROI.height || value_hd2_p3_in_y <0)
                {
                    value_hd2_p3_in_y = hdROI.height;
                    emit para_error();
                }

                if(value_hd2_p4_in_y >hdROI.height || value_hd2_p4_in_y <0)
                {
                    value_hd2_p4_in_y = hdROI.height;
                    emit para_error();
                }
                //保存参数
                ofstream outfile("./save_cam4.dat",ios::binary);
                int save_cam4[cam4_paras_num];
                for(int i=0;i<cam4_paras_num;i++)
                    save_cam4[i]=*cam4_paras[i];
                outfile.write((char*)&save_cam4,sizeof(save_cam4));
                outfile.close();

                SetRoi_hd2_flag = false;
            }
            //加载cam4参数
            if(load_cam4_flag)
            {
                load_cam4_flag=false;
                ifstream infile("./save_cam4.dat",ios::binary);
                if(!infile)
                {
                    QMessageBox::about(NULL, "About", "请先保存参数,才能加载!");
                }
                else
                {
                    int load_nums[cam4_paras_num];
                    infile.read((char*)&load_nums,sizeof(load_nums));
                    infile.close();
                    for(int i=0;i<cam4_paras_num;i++)
                        *cam4_paras[i]=load_nums[i];
                    QMessageBox::about(NULL, "About", "加载参数cam4成功!!!");
                }


            }


            rectangle(dst, hdROI, Scalar(0, 255, 255), 1);

            vector<Point> hdROI_out_pt;
            vector<vector<Point> > hdROI_out_contour;
            hdROI_out_mask = Mat::zeros(Size(hdROI.width, hdROI.height), CV_8UC1);
            hdROI_out_pt.push_back(Point(value_hd2_p1_out_x, value_hd2_p1_out_y));
            hdROI_out_pt.push_back(Point(value_hd2_p2_out_x, value_hd2_p2_out_y));
            hdROI_out_pt.push_back(Point(value_hd2_p3_out_x, value_hd2_p3_out_y));
            hdROI_out_pt.push_back(Point(value_hd2_p4_out_x, value_hd2_p4_out_y));
            hdROI_out_contour.push_back(hdROI_out_pt);
            drawContours(hdROI_out_mask, hdROI_out_contour, 0, Scalar::all(255), cv::FILLED, 8, 1, 0, Point());

            vector<Point> hdROI_in_pt;
            vector<vector<Point> > hdROI_in_contour;
            hdROI_in_mask = Mat::zeros(Size(hdROI.width, hdROI.height), CV_8UC1);
            hdROI_in_pt.push_back(Point(value_hd2_p1_in_x, value_hd2_p1_in_y));
            hdROI_in_pt.push_back(Point(value_hd2_p2_in_x, value_hd2_p2_in_y));
            hdROI_in_pt.push_back(Point(value_hd2_p3_in_x, value_hd2_p3_in_y));
            hdROI_in_pt.push_back(Point(value_hd2_p4_in_x, value_hd2_p4_in_y));
            hdROI_in_contour.push_back(hdROI_in_pt);
            drawContours(hdROI_in_mask, hdROI_in_contour, 0, Scalar::all(255), cv::FILLED, 8, 1, 0, Point());

            drawContours(dst, hdROI_out_contour, 0, Scalar(255, 0, 255), 1, 8, 1, 0, Point(hdROI.x, hdROI.y));
            drawContours(dst, hdROI_in_contour, 0, Scalar(0, 255, 255), 1, 8, 1, 0, Point(hdROI.x, hdROI.y));

            /************************/
            /*  Handrail Monitoring	*/
            /************************/
            //Foreground
            mog->apply(src2(hdROI), dstFg, learning_rate);
            mog->getBackgroundImage(dstBg);
//            mog(src2(hdROI), dstFg, 0.01);
//            mog.getBackgroundImage(dstBg);
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
            cam4_hdDrail=false;
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
                    cam4_hdDrail=true;
                }
            }

            for (int i = 0; i < hdDrailRect.size(); i++)
                rectangle(dst, Rect(hdDrailRect[i].x + hdROI.x, hdDrailRect[i].y + hdROI.y, hdDrailRect[i].width, hdDrailRect[i].height), Scalar(0, 0, 255), 2);

            src2Ready = false;

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
            disPos.x = 550;
            disPos.y = 20;
            string cam4_location;
            cam4_location = "left drail";
            putText(dst, cam4_location, disPos, FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 0, 0), 1);

            //dst
            const uchar *pSrc = (const uchar*)dst.data;
            QImage temp(pSrc, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
            qtImage = temp.copy();
            qtImage = qtImage.rgbSwapped();
            emit getImage4(qtImage);

            //Next Frame
            fmCurrent++;
        }

        //Video Control
        msleep(1);

    }
    cerr << "Camera " << CAMERA_NUM << " Stop!!" << endl;
    return;

}
