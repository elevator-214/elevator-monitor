/*
 * @Description: load the trt engine model
 * @Author: HLG
 * @Date: 2020年6月25日21:48:46

 */
#include<fstream>
#include <cstring>
#include <NvInfer.h>
#include <cuda.h>
#include<NvInferPlugin.h>
#include "cuda_runtime.h"
#include "spdlog/spdlog.h"
#include"my_gcn.hpp"
using namespace std;
#ifndef CUDA_CHECK
#define CUDA_CHECK(callstr)                                                                    \
    {                                                                                          \
        cudaError_t error_code = callstr;                                                      \
        if (error_code != cudaSuccess) {                                                       \
            std::cerr << "CUDA error " << error_code << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
            exit(0);                                                                         \
        }                                                                                      \
    }
#endif
inline void* safeCudaMalloc(size_t memSize) {
    void* deviceMem;
    CUDA_CHECK(cudaMalloc(&deviceMem, memSize));
    if (deviceMem == nullptr) {
        std::cerr << "Out of memory" << std::endl;
        exit(1);
    }
    return deviceMem;
}
inline int64_t volume(const nvinfer1::Dims& d)
{
    return std::accumulate(d.d, d.d + d.nbDims, 1, std::multiplies<int64_t>());
}

inline unsigned int getElementSize(nvinfer1::DataType t)
{
    switch (t)
    {
        case nvinfer1::DataType::kINT32: return 4;
        case nvinfer1::DataType::kFLOAT: return 4;
        case nvinfer1::DataType::kHALF: return 2;
        case nvinfer1::DataType::kINT8: return 1;
        default: throw std::runtime_error("Invalid DataType.");
    }
}
inline void safeCudaFree(void* deviceMem) {
    CUDA_CHECK(cudaFree(deviceMem));
}

void My_trt::CopyFromHostToDevice(const std::vector<float>& input, int bindIndex) {
    CUDA_CHECK(cudaMemcpy(mBinding[bindIndex], input.data(), mBindingSize[bindIndex], cudaMemcpyHostToDevice));
}
void My_trt::CopyFromDeviceToHost(std::vector<float>& output, int bindIndex) {
    CUDA_CHECK(cudaMemcpy(output.data(), mBinding[bindIndex], mBindingSize[bindIndex], cudaMemcpyDeviceToHost));
}
My_trt::~My_trt() {
    // spdlog::info("hello!!");
    if(mContext != nullptr) {
        mContext->destroy();
        mContext = nullptr;
    }
    // spdlog::info("hello!!");
    if(mEngine !=nullptr) {
        mEngine->destroy();
        mEngine = nullptr;
    }
    // spdlog::info("hello!!");
    for(size_t i=0;i<mBinding.size();i++) {
        safeCudaFree(mBinding[i]);
    }

}
size_t My_trt::GetBindingSize(int bindIndex) const {
    return mBindingSize[bindIndex];
}
bool My_trt::DeserializeEngine(const std::string& engineFile) {
    std::ifstream in(engineFile.c_str(), std::ifstream::binary);
    if(in.is_open()) {
        spdlog::info("deserialize engine from {}",engineFile);
        auto const start_pos = in.tellg();
        in.ignore(std::numeric_limits<std::streamsize>::max());
        size_t bufCount = in.gcount();
        in.seekg(start_pos);
        std::unique_ptr<char[]> engineBuf(new char[bufCount]);
        in.read(engineBuf.get(), bufCount);
        initLibNvInferPlugins(&mLogger, "");
        mRuntime = nvinfer1::createInferRuntime(mLogger);
        mEngine = mRuntime->deserializeCudaEngine((void*)engineBuf.get(), bufCount, nullptr);
        assert(mEngine != nullptr);
        mBatchSize = mEngine->getMaxBatchSize();
        spdlog::info("max batch size of deserialized engine: {}",mEngine->getMaxBatchSize());
        // spdlog::info("DeserializeEngine!!!!");
        mRuntime->destroy();
        // spdlog::info("DeserializeEngine!!!!");
        return true;
    }
    return false;
}

void My_trt::InitEngine() {
    spdlog::info("init engine...");
    mContext = mEngine->createExecutionContext();
    assert(mContext != nullptr);

    spdlog::info("malloc device memory");
    int nbBindings = mEngine->getNbBindings();//engine.getNbBindings()是为了获取与这个engine相关的输入输出tensor的数量。//这个地方，输入+输出 总共就2个
    std::cout << "nbBingdings: " << nbBindings << std::endl;
    mBinding.resize(nbBindings);
    mBindingSize.resize(nbBindings);
    mBindingName.resize(nbBindings);
    mBindingDims.resize(nbBindings);
    mBindingDataType.resize(nbBindings);
    for(int i=0; i< nbBindings; i++) {
        nvinfer1::Dims dims = mEngine->getBindingDimensions(i);
        nvinfer1::DataType dtype = mEngine->getBindingDataType(i);
        const char* name = mEngine->getBindingName(i);
        int64_t totalSize = volume(dims) * mBatchSize * getElementSize(dtype);
        mBindingSize[i] = totalSize;
        mBindingName[i] = name;
        mBindingDims[i] = dims;
        mBindingDataType[i] = dtype;
        if(mEngine->bindingIsInput(i)) {
            spdlog::info("input: ");
        } else {
            spdlog::info("output: ");
        }
        spdlog::info("binding bindIndex: {}, name: {}, size in byte: {}",i,name,totalSize);
        spdlog::info("binding dims with {} dimemsion",dims.nbDims);
        for(int j=0;j<dims.nbDims;j++) {
            std::cout << dims.d[j] << " x ";
        }
        std::cout << "\b\b  "<< std::endl;
        mBinding[i] = safeCudaMalloc(totalSize);
        if(mEngine->bindingIsInput(i)) {
            mInputSize++;
        }
    }

}
void My_trt::DoInference(std::vector<float>& inputData, std::vector<float>& result)
{
    this->CopyFromHostToDevice(inputData, 0);//把inputData 复制到mBinding[0]
    this->Forward();
    result.resize(7); 
    this->CopyFromDeviceToHost(result,1);//把结果从mBinding[1]中取出来，放到net_output中
}
void My_trt::Forward() {
    cudaEvent_t start,stop;
    float elapsedTime;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);
    cudaEventRecord(start, 0);
    mContext->execute(mBatchSize, &mBinding[0]);
    cudaEventRecord(stop, 0);
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&elapsedTime, start, stop);
    spdlog::info("net forward takes {} ms", elapsedTime);
}
