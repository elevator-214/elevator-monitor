/*
 * @Description: load the trt engine model
 * @Author: HLG
 * @Date: 2020年6月25日21:48:46

 */
#include"my_gcn.hpp"
#include <iostream>
#include <string>
using namespace std;


int main()
{
	string trt_file="./gcn.trt";
	My_trt gcn;
	gcn.DeserializeEngine(trt_file);
	gcn.InitEngine();

	const int N=1;
	const int C=3;
	const int H=14;
	const int W=1;
	std::vector<float> inputData;
	std::vector<float> outputData;
    inputData.resize(N * C * H * W);
    for(int n=0; n<N;n++) {
        for(int c=0;c<C;c++) {
            for(int h=0;h<H;h++) {
            	for(int w=0;w<W;w++) {
                	inputData[n*(C*H*W)+c*(H*W)+h*W+w] = (float)(50);
            	}
            }
        }
    }
    gcn.DoInference(inputData,outputData);
    for(int i=0;i<outputData.size();i++)
    {
    	cout <<outputData[i]<<endl;
    }
    return 0;
}
