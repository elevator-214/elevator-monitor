/*
 * @Description: load the trt engine model
 * @Author: HLG
 * @Date: 2020年6月25日21:48:46

 */
#pragma once
#include <NvInfer.h>
#include <vector>
#include <iostream>
#include <string>
using namespace std;
class TrtLogger : public nvinfer1::ILogger {
    void log(Severity severity, const char* msg) override
    {
        // suppress info-level messages
        if (severity != Severity::kVERBOSE)
            std::cout << msg << std::endl;
    }
};


class My_trt
{
public:
    My_trt(){}
    ~My_trt();
    void CopyFromHostToDevice(const std::vector<float>& input, int bindIndex);
    void CopyFromDeviceToHost(std::vector<float>& output, int bindIndex);
    size_t GetBindingSize(int bindIndex) const;
    bool DeserializeEngine(const std::string& engineFile);
    void InitEngine();
    void DoInference(std::vector<float>& inputData, std::vector<float>& result);
    void Forward();
    
    TrtLogger mLogger;
    nvinfer1::IRuntime* mRuntime = nullptr;
    nvinfer1::ICudaEngine* mEngine = nullptr;
    nvinfer1::IExecutionContext* mContext = nullptr;
    std::vector<void*> mBinding;
    std::vector<size_t> mBindingSize;
    std::vector<std::string> mBindingName;
    std::vector<nvinfer1::Dims> mBindingDims;
    std::vector<nvinfer1::DataType> mBindingDataType;
    // batch size
    int mBatchSize; 
    int mInputSize = 0;
};
