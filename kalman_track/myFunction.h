#ifndef LIFUNCTION_H
#define LIFUNCTION_H

/************************************************************/
/*  File Name: liFunction.h									*/
/*  Description: Functions used in the program.				*/
/*  Author: Haozheng Li										*/
/*  EMail: 466739850@qq.com									*/
/************************************************************/

#include <iostream>
#include <sstream>
#include <iomanip>
#include<algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include<munkres.h>
#include<adapters/adapter.h>
#include<adapters/std2dvectordapter.h>
#include <set>
using namespace std;
using namespace cv;
typedef float BoxDistanceType;
//calculate distance p2p
float liDistance(Point2f pt1, Point2f pt2);

//calculate mean p2p
Point2f liMean(Point2f pt1, Point2f pt2);

//correct detected point, which should be in ROI and not too close
vector<Point2f> liCorrectDetection(vector<Point2f> pt, Rect roi, float dist_T);

//correct people detected point, which should be in ROI and not too close p2p
vector<Point2f> liCorrectPpDetection(vector<Point2f> pt, Rect roi, float dist_T);

//correct thing detected point, which should be in ROI and not too close both t2t or t2p
vector<Point2f> liCorrectThDetection(vector<Point2f> pt_th, vector<Point2f> pt_pp, Rect roi, float dist_t2t_T, float dist_t2p_T);

//contour detection function with connected component
vector<vector<Point> > liSegmentFGMask(Mat src);

//transfer Point2f to Mat
Mat liPointToMat(Point2f pt);

//transfet Mat to Point2f
Point2f liMatToPoint(Mat pt);

//判断两个数是否相等
bool isequal(BoxDistanceType num,BoxDistanceType target);
//计算iou
BoxDistanceType calculate_iou(cv::Rect rect1,cv::Rect rect2);
//计算距离矩阵
void calculate_Distance_matrix(std::vector<cv::Rect>ppDetectionRect,std::vector<cv::Rect>tracking_boxes,vector<vector<BoxDistanceType>>&Distance_Matrix,vector<vector<BoxDistanceType>>&Distance_Matrix_Reverse);
//根据距离矩阵中的值对矩阵进行滤波
void Distance_matrix_Filter(vector<vector<BoxDistanceType>>&Distance_Matrix,vector<vector<BoxDistanceType>>&Distance_Matrix_Reverse,
                            BoxDistanceType threshold,vector<int>&choose_rows,vector<int>&choose_cols);
//得到滤波之后的距离矩阵
vector<vector<BoxDistanceType>> get_filtered_Matrix(vector<vector<BoxDistanceType>>Matrix_origin,vector<int>choose_rows,vector<int>choose_cols);
#endif // LIFUNCTION_H
