#include "communication.h"
#include <stdio.h> 
#include <sys/types.h> 
#include <errno.h> 
#include <sys/stat.h> 
#include <fcntl.h> 
#include <unistd.h> 
#include <termios.h> 
#include <stdlib.h> 
#include <byteswap.h>


Communication :: Communication()
{
    this->fd = open( "/dev/ttyUSB0", O_RDWR|O_NOCTTY|O_NDELAY);//打开描述符 阻塞读
    if(fd<0)
    {
        perror("open error"); 
 
    }
    if( (set_opt(fd,115200,8,'N',1))<0 )//设置串口信息
    { 
        perror("set_opt error"); 
    
    } 
    resultTXD = 0x0100000000000000;//发送结果 初始化
   /* resultTXD = 0x0100000000000000;
    resultTXD_size = -1;
    const char* cFifoName = "/tmp/my_fifo";
    if( access(cFifoName, F_OK) == -1 )
    {
       if( mkfifo(cFifoName, 0777) )
           perror("Make FIFO error!!\n");
    }
    nPipeID = open(cFifoName, O_WRONLY);
    if( nPipeID < 0)
       perror("Open FIFO error!!\n");*/
}


Communication :: ~Communication()
{
    ;
}

int Communication ::set_opt(int fd,int nSpeed, int nBits, char nEvent, int nStop) 
{ 
    struct termios newtio,oldtio; 
    if ( tcgetattr( fd,&oldtio) != 0) { 
        perror("SetupSerial 1"); 
        return -1; 
    } 


    bzero( &newtio, sizeof( newtio ) ); 
    newtio.c_cflag |= CLOCAL | CREAD; 
    newtio.c_cflag &= ~CSIZE; 
    /***********数据位选择****************/ 
    switch( nBits ) 
    { 
    case 7: 
    newtio.c_cflag |= CS7; 
    break; 


    case 8: 
    newtio.c_cflag |= CS8; 
    break; 
    } 

/***********校验位选择****************/ 
    switch( nEvent ) 
    { 
    case 'O': //奇数
    newtio.c_cflag |= PARENB; 
    newtio.c_cflag |= PARODD; 
    newtio.c_iflag |= (INPCK | ISTRIP); 
    break; 


    case 'E': //偶数
    newtio.c_iflag |= (INPCK | ISTRIP); 
    newtio.c_cflag |= PARENB; 
    newtio.c_cflag &= ~PARODD; 
    break; 


    case 'N': //无奇偶校验位
    newtio.c_cflag &= ~PARENB; 
    break; 
    } 

 /*设置波特率*/
    switch( nSpeed ) 
    { 
    case 2400: 
    cfsetispeed(&newtio, B2400); 
    cfsetospeed(&newtio, B2400); 
    break; 


    case 4800: 
    cfsetispeed(&newtio, B4800); 
    cfsetospeed(&newtio, B4800); 
    break; 


    case 9600: 
    cfsetispeed(&newtio, B9600); 
    cfsetospeed(&newtio, B9600); 
    break; 


    case 115200: 
    cfsetispeed(&newtio, B115200); 
    cfsetospeed(&newtio, B115200); 
    break; 


    default: 
    cfsetispeed(&newtio, B9600); 
    cfsetospeed(&newtio, B9600); 
    break; 
    } 

/*设置停止位*/
    if( nStop == 1 ) 
        newtio.c_cflag &= ~CSTOPB; 
    else if ( nStop == 2 ) 
    newtio.c_cflag |= CSTOPB; 
    newtio.c_cc[VTIME] = 0; 
    newtio.c_cc[VMIN] = 0; 
    tcflush(fd,TCIFLUSH); 
    if((tcsetattr(fd,TCSANOW,&newtio))!=0) 
    { 
    perror("com set error"); 
    return -1; 
    } 


    printf("set done!\n"); 
    return 0; 
} 

void Communication :: run()
{
  
    char buff_rs232[10];//用来接收扶梯那边的数据
    int nread=0;//用来存储读了多少个字节 
    int num=0;//每次读满8个字节就发送一次
    while(true)
    {
        num=0;
        int pipe_num=0;
        while(num!=10)//10
        {
           nread=read(this->fd,buff_rs232+num,10-num);//阻塞读
          // printf("resultTXD:%x\n",resultTXD);
           num+=nread;
        }
        //各种标志位的写入
        resultTXD = 0x0100000000000000;
        
        
        if (cam1_ppCount)
            resultTXD = resultTXD | 0x0000000000000001;
        else
            resultTXD = resultTXD & 0xFFFFFFFFFFFFFFFE;
        //bHandrail on left (1 bit)
        if (cam1_ppRetent)
        {
            resultTXD = resultTXD | 0x0000000000000002;
        }
        else
        {
            resultTXD = resultTXD & 0xFFFFFFFFFFFFFFFD;
        }

        //bHandrail on right (2 bit)
        if (cam1_ppCrowded)
        {
            resultTXD = resultTXD | 0x0000000000000004;
        }
        else
        {
            resultTXD = resultTXD & 0xFFFFFFFFFFFFFFFB;
        }

        //People Retent (3 bit)
        if(cam1_thRetent)
        {
            resultTXD = resultTXD | 0x0000000000000008;
        }
        else
        {
            resultTXD = resultTXD & 0xFFFFFFFFFFFFFFF7;
        }

        //Thing Retent (4 bit)
        if (cam2_ppCount)
        {
            resultTXD = resultTXD | 0x0000000000000010;
        }
        else
        {
            resultTXD = resultTXD & 0xFFFFFFFFFFFFFFEF;
        }
        if (cam3_hdDrail)
        {
            resultTXD = resultTXD | 0x0000000000000020;
        }
        else
        {
            resultTXD = resultTXD & 0xFFFFFFFFFFFFFFDF;
        }
        
        
        if (cam4_hdDrail)
            resultTXD = resultTXD | 0x0000000000000040;
        else
            resultTXD = resultTXD & 0xFFFFFFFFFFFFFFBF;
        
        


        //Fall (5 bit, 6 bit, 7 bit)
        //...

        //30s Flow (2nd 3rd byte)
        long long tempNFlowCountPM = 0x0000000000000000;
        tempNFlowCountPM = cam1_ppFlow30s * 16 * 16;
        resultTXD = resultTXD + tempNFlowCountPM;

        //lack of camera?(4byte)
        if(no_camera1)
        {
            resultTXD=resultTXD | 0x0000000000010000;
        }
        else
        {
            resultTXD=resultTXD & 0xFFFFFFFFFFFEFFFF;
        }
        if(no_camera2)
        {
            resultTXD=resultTXD | 0x0000000000020000;
        }
        else
        {
            resultTXD=resultTXD & 0xFFFFFFFFFFFDFFFF;
        }
        if(no_camera3)
        {
            resultTXD=resultTXD | 0x0000000000040000;
        }
        else
        {
            resultTXD=resultTXD & 0xFFFFFFFFFFFBFFFF;
        }
   
        
        resultTXD=bswap_64(resultTXD);//大小端转换
        write(fd,&resultTXD,8);
        //读到8位之后
        //Change resultTXD
      /*  resultTXD = 0x0100000000000000;
        write(fd,resultTXD,8);
        //Pedestrian on plate and escalator (0 bit)
        
        


        //transfer resultTXD
        resultTXD_size = -1;
        resultTXD_size = write(nPipeID, &resultTXD, 8);
        if(resultTXD_size != 8)
            cerr << "Transfer TXD faill!!" << endl;

        //    //display resultTXD
        //    cerr << "size:" << resultTXD_size << ", resultTXD:" << bitset<sizeof(int)*4>(tempNFlowCountPM/16/16) << " " << bitset<sizeof(int)*2>(resultTXD) << endl;
        */

    }
}
