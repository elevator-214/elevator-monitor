#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <QThread>
#include <QImage>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;



extern bool cam1_ppCount;
extern bool cam1_ppRetent;
extern bool cam1_ppCrowded;
extern bool cam1_thRetent;
extern int cam1_ppFlow30s;

extern bool cam2_ppCount;

extern bool cam3_hdDrail;

extern bool cam4_hdDrail;

extern bool no_camera1;
extern bool no_camera2;
extern bool no_camera3;


class Communication : public QThread
{
    Q_OBJECT
public:
    Communication();
    ~Communication();
    int set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop); 
private:
	int fd; //设备描述符


    uint64_t resultTXD;
    int resultTXD_size;
    int nPipeID;


signals:



protected:
    void run();
};

#endif // Communication_H
