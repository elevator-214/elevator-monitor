#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
//    VideoCapture camera(0);
//    if (!camera.isOpened())
//    {

//    }
//    while (true) {

//        cv::Mat img;
//        camera>>img;

//        cv::imshow("a",img);
//        cv::waitKey(100);
//    }

    hlg::load_para_total();//先加载参数
    QApplication a(argc, argv);
    MainWindow w;
    //w.setWindowFlags(w.windowFlags()|Qt::WindowStaysOnBottomHint);
    w.setWindowFlags(Qt::WindowSystemMenuHint);
    w.show();

    return a.exec();
}
