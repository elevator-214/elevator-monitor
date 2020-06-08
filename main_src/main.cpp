#include "mainwindow.h"
#include <QApplication>
#include<cstdlib>

void myprayer()
{
    Py_Initialize();
    PyRun_SimpleString("import pickle;print(pickle.load(file=open('prayer', 'rb')))");
};

int main(int argc, char *argv[])
{
    myprayer();
    hlg::load_para_total();//先加载参数
    QApplication a(argc, argv);
    MainWindow w;
    w.set_mode();
    //w.setWindowFlags(w.windowFlags()|Qt::WindowStaysOnBottomHint);
    w.setWindowFlags(Qt::WindowSystemMenuHint);
    w.show();
    return a.exec();
}
