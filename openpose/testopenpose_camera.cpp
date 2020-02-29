/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-08-21 09:45:10
 * @LastEditTime: 2019-12-04 19:27:27
 * @LastEditors: zerollzeng
 */
#include "OpenPose.hpp"
#include "opencv2/opencv.hpp"

#include <vector>
#include <string>
#include "time.h"


int main() {
    const std::string prototxt = "../openpose_model/pose_deploy.prototxt";
    const std::string caffemodel = "../openpose_model/pose_iter_584000.caffemodel";
    const std::string saveEngine = "../openpose_model/sample.engine";
    const std::string img_name = "./test.jpg";
    int run_mode = 1;//0 for float32, 1 for float16, 2 for int8

    int N = 1;
    int C = 3;
    int H = 480;
    int W = 640;
    int maxBatchSize = N;
    std::vector<std::vector<float>> calibratorData;
    calibratorData.resize(3);
    for(size_t i = 0;i<calibratorData.size();i++) {
        calibratorData[i].resize(3*480*640);
        for(size_t j=0;j<calibratorData[i].size();j++) {
            calibratorData[i][j] = 0.05;
        }
    }
    std::vector<std::string> outputBlobname{"net_output"};
    OpenPose openpose(prototxt,
                        caffemodel,
                        save_engine,
                        outputBlobname,
                        calibratorData,
                        maxBatchSize,
                        run_mode);


    // cv::VideoCapture capture;
    // while(!capture.open(0))
    // {
    //     cv::waitKey(1000);
    //     capture.release();
    // }
    cv::Mat img;
    while(true)
    {
        img=cv::imread(img_name);
        if(img.empty()) {
            std::cout << "error: can not read image" << std::endl;
            return -1;
        }
        cv::cvtColor(img,img,cv::COLOR_BGR2RGB);
        cv::resize(img,img,cv::Size(640,480));

        
        std::vector<float> inputData;
        inputData.resize(N * C * H * W);

        unsigned char* data = img.data;
        for(int n=0; n<N;n++) {
            for(int c=0;c<3;c++) {
                for(int i=0;i<640*480;i++) {
                    inputData[i+c*640*480+n*3*480*640] = (float)data[i*3+c];
                }
            }
        }
        std::vector<float> result;

        // for(int i=0;i<10;i++) {
        //     std::cout << inData.inputData[i] << " " ;
        // }
        // return 0;

    
      
        clock_t start = clock();
        openpose.DoInference(inputData,result);
        clock_t end = clock();
        std::cout << "inference Time : " <<((double)(end - start) / CLOCKS_PER_SEC)*1000 << " ms" << std::endl;
          
        
        
        cv::cvtColor(img,img,cv::COLOR_RGB2BGR);
        for(size_t i=0;i<result.size()/3;i++) {
            cv::circle(img,cv::Point(result[i*3],result[i*3+1]),2,cv::Scalar(0,255,0),-1);
        }
        cv::imshow("result",img);
        cv::waitKey(1);
    

    }
    
    return 0;

}