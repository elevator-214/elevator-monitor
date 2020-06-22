/* 
Author:HLG
Date:2020年6月5日
huang.liguang@qq.com
*/
//#define _GLIBCXX_USE_CXX11_ABI 0
#include"my_tracking_function.h"
#include<math.h>
#include<algorithm>
// using namespace std;
namespace hlg{
double cross(cv::Point2f A, cv::Point2f B, cv::Point2f P)      //向量AB与向量AP的外积
{
    cv::Point2f AB(B.x - A.x, B.y - A.y);
    cv::Point2f AP(P.x - A.x, P.y - A.y);
    return AB.x*AP.y - AB.y*AP.x;
}
double dot(cv::Point2f A, cv::Point2f B, cv::Point2f P)         //向量AB与向量AP的外积
{
    cv::Point2f AB( B.x - A.x, B.y - A.y);
    cv::Point2f AP( P.x - A.x, P.y - A.y);
    return AB.x*AP.x + AB.y*AP.y;
}
double dis2(cv::Point2f a, cv::Point2f b)                //点a、b距离的平方
{
    return double(a.x - b.x)*double(a.x - b.x) + double(a.y - b.y)*double(a.y - b.y);
}
int dir(cv::Point2f A, cv::Point2f B, cv::Point2f P)            //点P与线段AB位置关系
{
    if (cross(A, B, P) < 0) return -1;     //逆时针
    else if (cross(A, B, P)>0) return 1;   //顺时针
    else if (dot(A, B, P) < 0) return -2;  //反延长线
    else if (dot(A, B, P) >= 0 && dis2(A, B) >= dis2(A, P))
    {
        if (dis2(A, B) < dis2(A, P)) return 2;  //延长线
        return 0;                          //在线上
    }
}
double disMin(cv::Point2f A, cv::Point2f B, cv::Point2f P)      //点P到线段AB的最短距离
{
    double r = (double(P.x - A.x)*double(B.x - A.x) + double(P.y - A.y)*double(B.y - A.y)) / dis2(A, B);
    if (r <= 0) return sqrt(dis2(A, P));
    else if (r >= 1) return sqrt(dis2(B, P));
    else
    {
        double AC = r*sqrt(dis2(A, B));
        return sqrt(dis2(A, P) - AC*AC);
    }
}
double Min_Line_Distance(cv::Point2f A1,cv::Point2f A2,cv::Point2f B1,cv::Point2f B2)
{
    if (dir(A1, A2, B1)*dir(A1, A2, B2) <= 0 && dir(B1, B2, A1)*dir(B1, B2, A2) <= 0)  //两线段相交, 距离为0
        return 0.0; 
    else
        return std::min(std::min(std::min(disMin(A1, A2, B1), disMin(A1, A2, B2)), disMin(B1, B2, A1)), disMin(B1, B2, A2));
}
}
