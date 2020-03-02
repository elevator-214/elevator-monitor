This project is implemented in C++ and uses QT for interface interaction. Just a student project made by a total newb ,so feel free to ask and don’t expect too much. Test on Jetson Xavier. It should also work on ubuntu x86/64.

## TO DO LIST
- [✔] Detect the heads by YOLOv3-tiny
- [✔] Tracking the human heads by Kalman Filter and Hungarian match
- [✔] Human retention detection on escalator floor board
- [✔] Passenger flow statistics
- [✔] Large items retention detection by KNN+kd-tree and Hungarian match
- [✔] Detecting object sticking out of handrail by GMM
- [✔] Human keypoints extraction by Openpose+TensorRT
- [✖] Human keypoints tracking



## Pre-installation required
**CUDA**

**cudnn**

**TensorRT6** 

**QT5** 

**OpenCV4.2**

## Install the Project
```
cd elevator-monitor
mkdir build
cd build
cmake ..
make
./test2
```
## Reference
https://github.com/gishi523/kd-tree

https://github.com/saebyn/munkres-cpp

https://github.com/zerollzeng/tensorrt-zoo



## commit log
### 2020.02.27
完成了大件物品滞留算法的重新设计以及移植

### 2020.02.29
修改了线程1的Bug，如kalman坐标错误等等

### 2020.03.01
在线程2上完成了基于tensorRT的关键点提取

### 2020.03.02
把代码文件都整理到文件夹中，分了模块，方便后人阅读。
启动程序自动打开风扇，把xavier调到最大功率模式，防止烧坏。

