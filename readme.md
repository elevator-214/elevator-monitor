cmake版本代码，抛开了qt-creator,之后的移植考虑在这个版本上进行
## 依赖库
需要事先安装好 *TensorRT6* *QT5* *OpenCV4.2*

## 编译
mkdir build
cd build
cmake ..
make

## 2020.02.27
完成了大件物品滞留算法的重新设计以及移植

## 2020.02.29
修改了线程1的Bug，如kalman坐标错误等等

## 2020.03.01
在线程2上完成了基于tensorRT的关键点提取
