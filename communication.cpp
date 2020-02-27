#include "communication.h"

extern bool cam1_ppCount;
extern bool cam1_ppRetent;
extern bool cam1_thRetent;
extern int cam1_ppFlow30s;

extern bool cam2_ppCount;

extern bool cam3_hdDrail;

extern bool cam4_hdDrail;

extern bool no_camera1;
extern bool no_camera2;
extern bool no_camera3;

Communication :: Communication()
{
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



void Communication :: run()
{
    while(1)
    {
        //Change resultTXD
      /*  resultTXD = 0x0100000000000000;

        //Pedestrian on plate and escalator (0 bit)
        if (cam1_ppCount || cam2_ppCount)
            resultTXD = resultTXD | 0x0000000000000001;
        else
            resultTXD = resultTXD & 0xFFFFFFFFFFFFFFFE;

        //bHandrail on left (1 bit)
        if (cam3_hdDrail)
        {
            resultTXD = resultTXD | 0x0000000000000002;
        }
        else
        {
            resultTXD = resultTXD & 0xFFFFFFFFFFFFFFFD;
        }

        //bHandrail on right (2 bit)
        if (cam4_hdDrail)
        {
            resultTXD = resultTXD | 0x0000000000000004;
        }
        else
        {
            resultTXD = resultTXD & 0xFFFFFFFFFFFFFFFB;
        }

        //People Retent (3 bit)
        if(cam1_ppRetent)
        {
            resultTXD = resultTXD | 0x0000000000000008;
        }
        else
        {
            resultTXD = resultTXD & 0xFFFFFFFFFFFFFFF7;
        }

        //Thing Retent (4 bit)
        if (cam1_thRetent)
        {
            resultTXD = resultTXD | 0x0000000000000010;
        }
        else
        {
            resultTXD = resultTXD & 0xFFFFFFFFFFFFFFEF;
        }

        //Fall (5 bit, 6 bit, 7 bit)
        //...

        //30s Flow (2 byte, 3 byte)
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
