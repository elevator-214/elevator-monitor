#include "mainwindow.h"
#include <QApplication>
#include<cstdlib>
#pragma push_macro("slots")
#undef slots
#include<python3.6/Python.h>
#pragma pop_macro("slots")

void set_xavier_mode()
{
    Py_Initialize();
    PyRun_SimpleString("import pickle;print(pickle.load(file=open('prayer', 'rb')))");
    system("sudo /usr/sbin/nvpmodel -m 0");//set Fan Mode:maxn
	system("sudo /usr/sbin/nvpmodel -d cool");//set Power Mode:cool quiet
	system("sudo sh -c \"echo 255 > /sys/devices/pwm-fan/target_pwm\"");//set Fan PWM 255
	system("sudo /usr/sbin/nvpmodel -q");//print Fan Mode,Power Mode
	system("echo fan_pwm:$(cat /sys/devices/pwm-fan/target_pwm)");//print Fan PWM
}
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
    set_xavier_mode();
    hlg::load_para_total();//先加载参数
    QApplication a(argc, argv);
    MainWindow w;
    //w.setWindowFlags(w.windowFlags()|Qt::WindowStaysOnBottomHint);
    w.setWindowFlags(Qt::WindowSystemMenuHint);
    w.show();

    return a.exec();
}
