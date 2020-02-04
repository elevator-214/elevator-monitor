#include "camthread2.h"
#include <boost/asio.hpp>//gai
#include <ctime>
Mat src2;
bool src1Ready = false;//cam2标志位
bool src2Ready = false;//cam4标志位

bool cam2_ppCount=false;
bool no_camera2=false;

extern bool draw_trajectory_flag;//是否画行人轨迹标志位
extern bool drawSkeleton_flag;//是否画骨架标志位
extern bool drawID_flag;//是否画id标志位
extern bool drawBox_flag;//是否画检测框标志位
extern bool cam2_start_flag;
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


    while(true);
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
    Mat dst;
    /************************/
    /*  socket Patameters	*/
    /************************/
    boost::asio::io_service io_service;
    using boost::asio::local::stream_protocol;
    stream_protocol::socket sock(io_service);
    boost::system::error_code error_code;
    std::vector<char> error_image_buf(1000000);//如果出错，则读光缓冲区
    sock.connect("./uds_server.sock",error_code);


    //Judgement
    int ppCount_inc = 0;
    double fps = (double)getTickCount();
    /*开三个线程，第一个线程读取摄像头,并发送给cam4,第二个线程发送图片给python的行人检测,第三个线程负责接收处理好的图片*/
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


        vector<uchar> image_buff;
        vector<uchar> image_size_buff(4);//存放图片大小的数组
        vector<uchar> bool_flag_buff(1);//用来存放bool型变量的数组

        int image_size;
        uchar bool_char;
        vector<int> param= vector<int>(2);
        param[0]=cv::IMWRITE_JPEG_QUALITY;
        param[1]=70;//default(95) 0-100 图片质量

        while(!CAMERA_STOP)
        {
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
                    cout<<"hello"<<endl;
                }

            }
            if(src1Ready)
            {
                cout<<"hi"<<endl;
                cv::imencode(".jpg", src,image_buff,param);
    //            end   = clock();
    //            cout << "花费了" << (double)(end - start) / CLOCKS_PER_SEC << "秒" << endl;
                image_size=image_buff.size();
                std::cout<<"image_size"<<image_size<<std::endl;
                image_size_buff[3]=image_size;//将图片大小存到buff中,发送出去
                image_size_buff[2]=image_size>>8;
                image_size_buff[1]=image_size>>16;
                image_size_buff[0]=image_size>>24;

                //发送标志位信息
                bool_char=draw_trajectory_flag;
                bool_char=(bool_char<<1)+drawSkeleton_flag;
                bool_char=(bool_char<<1)+drawID_flag;
                bool_char=(bool_char<<1)+drawBox_flag;
                bool_char=(bool_char<<4)+bool_char;
                bool_flag_buff[0]=bool_char;



        //        int check=image_size_buff[0]+image_size_buff[1]*256+image_size_buff[2]*256*256;
        //        cout<<check<<endl;

    //            cout<<"1111!!!!"<<endl;
                sock.write_some(boost::asio::buffer(image_size_buff, image_size_buff.size()));
                sock.write_some(boost::asio::buffer(image_size_buff, image_size_buff.size()));
                sock.write_some(boost::asio::buffer(bool_flag_buff, bool_flag_buff.size()));
    //            cout<<"1111!!!!"<<endl;
                sock.write_some(boost::asio::buffer(image_buff, image_buff.size()));
                src1Ready = false;
    //            dst = src.clone();


            }

        }


    }));

    /*开三个线程，第三个线程负责接收行人检测的结果*/
    ths.push_back(std::thread([&](){

        int datalenth1;
        int datalenth2;
        vector<char> buf(8);
        while(true)
        {
            fps = (double)getTickCount();


//            size_t len=sock.read_some(boost::asio::buffer(buf,8), error_code);
            boost::asio::read(sock,boost::asio::buffer(buf,8));

            datalenth1 = (unsigned char)(buf[3])*256*256*256 + (unsigned char)(buf[2]) *256*256 + (unsigned char)(buf[1])*256+ (unsigned char)(buf[0]);
            datalenth2 = (unsigned char)(buf[7])*256*256*256 + (unsigned char)(buf[6]) *256*256 + (unsigned char)(buf[5])*256+ (unsigned char)(buf[4]);
            cout<<datalenth1<<endl;
            cout<<datalenth2<<endl;
            if(datalenth1==datalenth2&&datalenth1>0&&datalenth1<200000)
            {
                std::vector<char> image_buf(datalenth1);
//                sock.read_some(boost::asio::buffer(image_buf,datalenth1), error_code);
                boost::asio::read(sock,boost::asio::buffer(image_buf,datalenth1));
                dst=cv::imdecode(cv::Mat(image_buf),cv::IMREAD_COLOR);
                fps = (double)getTickFrequency() / ((double)getTickCount() - fps);


                if(!cam2_start_flag)
                {
                    putText(dst, "STOP!!!", cv::Point2d(100, 250), FONT_HERSHEY_SIMPLEX, 5, Scalar(0, 0, 255), 5);
                }
                //dst
                const uchar *pSrc = (const uchar*)dst.data;
                QImage temp(pSrc, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);
                qtImage = temp.copy();
                qtImage = qtImage.rgbSwapped();
                emit getImage2(qtImage);

                //Next Frame
                fmCurrent++;
                fmMinue = fps * 60;

                //Video Control
                msleep(1);


            }
            else
            {
                sock.read_some(boost::asio::buffer(error_image_buf), error_code);
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
