## Introduction

Use three cameras to monitor some behaviors on the walking elevator.

Including:

- Traffic statistics
- Passenger&Large Object Retention Detection  on  escalator floor board
- Human Keypoints Extraction and Tracking in elevator
- Detection of object sticking out of handrail

This project is implemented in C++ and uses QT for interface interaction. Just a student project made by a total newb ,so feel free to ask and don’t expect too much. Test on Jetson Xavier. It should also work on ubuntu x86/64.

## TO DO LIST

- [✔] Detect the heads by YOLOv3-tiny
- [✔] Tracking the human heads by Kalman Filter and Hungarian match
- [✔] Human retention detection on escalator floor board
- [✔] Passenger flow statistics
- [✔] Large items retention detection by KNN+kd-tree and Hungarian match
- [✔] Detecting object sticking out of handrail by GMM
- [✔] Human keypoints extraction by Openpose+TensorRT
- [✔] Human keypoints tracking

## Pre-installation required

**CUDA**

**cudnn**

**TensorRT6** 

**Boost** 

**QT5** 

**OpenCV4.2**

## Install the Project
```
cd elevator-monitor
mkdir build
cd build
cmake ..
make
./elevator_monitor
```
## Some models & files required

**BaiduYunPan url:** uploading

**usage**:Download and move models & files to **build/**

## Reference

https://github.com/gishi523/kd-tree

https://github.com/saebyn/munkres-cpp

https://github.com/zerollzeng/tensorrt-zoo



