/* 
Author:HLG
Date:2020年6月5日
huang.liguang@qq.com
*/
#pragma once

#include <opencv2/opencv.hpp>

namespace hlg{


double cross(cv::Point2f A, cv::Point2f B, cv::Point2f P);      //向量AB与向量AP的外积

double dot(cv::Point2f A, cv::Point2f B, cv::Point2f P);         //向量AB与向量AP的外积

double dis2(cv::Point2f a, cv::Point2f b);                //点a、b距离的平方

int dir(cv::Point2f A, cv::Point2f B, cv::Point2f P);            //点P与线段AB位置关系

double disMin(cv::Point2f A, cv::Point2f B, cv::Point2f P);      //点P到线段AB的最短距离

double Min_Line_Distance(cv::Point2f A1,cv::Point2f A2,cv::Point2f B1,cv::Point2f B2);//求两个线段的最短距离


}
