/************************************************************/
/*  File Name: MyFunction.cpp								*/
/*  Description: Functions used in the program.				*/
/*  Author: Haozheng Li										*/
/*  EMail: 466739850@qq.com									*/
/************************************************************/

#include "myFunction.h"

float liDistance(Point2f pt1, Point2f pt2)
{
    float s1 = pow(pt1.x - pt2.x, 2);
    float s2 = pow(pt1.y - pt2.y, 2);
    float dist = sqrt(s1 + s2);
    return dist;
}

Point2f liMean(Point2f pt1, Point2f pt2)
{
    Point2f pt(pt1.x / 2 + pt2.x / 2, pt1.y / 2 + pt2.y / 2);

    return pt;
}

vector<Point2f> liCorrectDetection(vector<Point2f> pt, Rect roi, float dist_T)
{
    vector<Point2f> pt_c;
    for (vector<Point2f>::iterator p = pt.begin(); p != pt.end(); p++)
    {
        if (p->x > roi.x && p->x < roi.x + roi.width && p->y > roi.y && p->y < roi.y + roi.height)
        {
            vector<Point2f>::iterator q = p + 1;
            for (; q != pt.end(); q++)
            {
                float dist = liDistance(*p, *q);
                if (dist < dist_T)
                {
                    *q = liMean(*p, *q);
                    break;
                }
            }

            if (q == pt.end())
            {
                pt_c.push_back(*p);
            }
        }
    }

    return pt_c;
}

vector<Point2f> liCorrectPpDetection(vector<Point2f> pt, Rect roi, float dist_T)
{
    vector<Point2f> pt_c;
    for (vector<Point2f>::iterator p = pt.begin(); p != pt.end(); p++)
    {
        if (p->x > roi.x && p->x < roi.x + roi.width && p->y > roi.y && p->y < roi.y + roi.height)
        {
            vector<Point2f>::iterator q = p + 1;
            for (; q != pt.end(); q++)
            {
                float dist = liDistance(*p, *q);
                if (dist < dist_T)
                {
                    *q = liMean(*p, *q);
                    break;
                }
            }

            if (q == pt.end())
            {
                pt_c.push_back(*p);
            }
        }
    }

    return pt_c;
}

vector<Point2f> liCorrectThDetection(vector<Point2f> pt_th, vector<Point2f> pt_pp, Rect roi, float dist_t2t_T, float dist_t2p_T)
{
    vector<Point2f> pt_c;
    for (vector<Point2f>::iterator p = pt_th.begin(); p != pt_th.end(); p++)
    {
        if (p->x > roi.x && p->x < roi.x + roi.width && p->y > roi.y && p->y < roi.y + roi.height)
        {
            vector<Point2f>::iterator q = p + 1;
            for (; q != pt_th.end(); q++)
            {
                float dist_t2t = liDistance(*p, *q);
                if (dist_t2t < dist_t2t_T)
                {
                    *q = liMean(*p, *q);
                    break;
                }
            }

            if (q == pt_th.end())
            {
                int i = 0;
                for (; i < pt_pp.size(); i++)
                {
                    float dist_t2p = liDistance(*p, pt_pp[i]);
                    if (dist_t2p < dist_t2p_T)
                        break;
                }
                if (i == pt_pp.size())
                    pt_c.push_back(*p);
            }
        }
    }

    return pt_c;
}
void calculate_Distance_matrix(std::vector<cv::Rect>ppDetectionRect,std::vector<cv::Rect>tracking_boxes,vector<vector<BoxDistanceType>>&Distance_Matrix,vector<vector<BoxDistanceType>>&Distance_Matrix_Reverse)
{
    for(int row=0;row<Distance_Matrix.size();row++)
    {
        for(int col=0;col<Distance_Matrix[0].size();col++)
        {
            Distance_Matrix[row][col]=1-calculate_iou(ppDetectionRect[row],tracking_boxes[col]);//iou越小,距离越大
            Distance_Matrix_Reverse[col][row]=Distance_Matrix[row][col];
        }
    }

}
void Distance_matrix_Filter(vector<vector<BoxDistanceType>>&Distance_Matrix,vector<vector<BoxDistanceType>>&Distance_Matrix_Reverse,
                            BoxDistanceType threshold,vector<int>&choose_rows,vector<int>&choose_cols)
{


    if(Distance_Matrix.size()==0||Distance_Matrix[0].size()==0)
    {
        choose_cols.clear();
        choose_rows.clear();
    }

    else
    {
        for(int i=Distance_Matrix.size()-1;i>=0;--i)
        {
            if(*std::min_element(Distance_Matrix[i].begin(),Distance_Matrix[i].end())>threshold)
            {
                choose_rows.erase(choose_rows.begin()+i);
            }
        }
        for(int i=Distance_Matrix_Reverse.size()-1;i>=0;--i)
        {
            if(*std::min_element(Distance_Matrix_Reverse[i].begin(),Distance_Matrix_Reverse[i].end())>threshold)
            {
                choose_cols.erase(choose_cols.begin()+i);
            }
        }
    }
}
vector<vector<BoxDistanceType>> get_filtered_Matrix(vector<vector<BoxDistanceType>>Matrix_origin,vector<int>choose_rows,vector<int>choose_cols)
{
    int rows=0,cols=0;
    if(choose_rows.size()>0&&choose_cols.size()>0)
    {
        rows=choose_rows.size();
        cols=choose_cols.size();
    }
    vector<vector<BoxDistanceType>>filtered_Matrix(rows,vector<BoxDistanceType>(cols));
    for(int row=0;row<rows;row++)
    {
        for(int col=0;col<cols;col++)
        {
            filtered_Matrix[row][col]=Matrix_origin[choose_rows[row]][choose_cols[col]];
        }
    }
    return filtered_Matrix;
}
BoxDistanceType calculate_iou(cv::Rect rect1,cv::Rect rect2)
{
    cv::Rect And= rect1|rect2;
    cv::Rect Union = rect1 & rect2;
    return BoxDistanceType(Union.area()*1.0 / And.area());

}
bool isequal(BoxDistanceType num,BoxDistanceType target)
{
    if(abs(num-target)<0.000001)
        return true;
    else
        return false;
}



vector<vector<Point> > liSegmentFGMask(Mat src)
{
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    int nContourCount = 0;

    Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
    morphologyEx(src, src, MORPH_OPEN, element);
    morphologyEx(src, src, MORPH_CLOSE, element);

    findContours(src, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE, Point(0, 0));

    for (int i = 0; i < contours.size(); i++)
    {
        double len = arcLength(contours[i], true);
        double thresh = (src.cols + src.rows) / 4.f;
        if (len < thresh)
        {
            contours.erase(contours.begin() + i);
            i--;
        }
        else
        {
            approxPolyDP(contours[i], contours[i], 2, 0);
            nContourCount++;
        }
    }

    /*Mat dst = Mat::zeros(src.size(), CV_8UC1);
    for (int i = 0; i< contours.size(); i++)
        drawContours(dst, contours, i, Scalar(255, 255, 255), CV_FILLED, 8, hierarchy, 0, Point());
    namedWindow("contours", CV_WINDOW_AUTOSIZE);
    imshow("contours", dst);*/

    return contours;
}

Mat liPointToMat(Point2f pt)
{
//    Mat pt_m = *(Mat_<float>(2, 1) << pt.x, pt.y);//opencv2
    Mat pt_m = (Mat_<float>(2, 1) << pt.x, pt.y);//opencv4

    return pt_m;
}

Point2f liMatToPoint(Mat pt)
{
    Point2f pt_p(0, 0);

    if (pt.rows == 2 && pt.cols == 1)
    {
        pt_p.x = pt.at<float>(0);
        pt_p.y = pt.at<float>(1);
    }

    return pt_p;
}

