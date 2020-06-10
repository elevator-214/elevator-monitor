/************************************************************/
/*  File Name: KalmanTracker.cpp							*/
/*  Description: Class for multi-target Kalman tracking.	*/
/*  Author: HLG                                     		*/
/*  EMail: huang.liguang@qq.com								*/
/************************************************************/

#include "KalmanTracker.h"
int ppFlowLine;

MyKalmanFilter::MyKalmanFilter(int id_0, Point2f measurement_0,Rect box)
{
    id = id_0;
    
    confidence = 15;
    trajectory.clear();
    inside_inc = 0;
    outside_inc = 0;
    confidence_inc = 0;
    confidence_dec = 0;
    confidence_linear_decrease_frame=10;
    this->box=box;
    statePre = Mat::zeros(4, 1, CV_32F);
    statePost = Mat::zeros(4, 1, CV_32F);	//x
    statePost.at<float>(0) = measurement_0.x;
    statePost.at<float>(1) = measurement_0.y;

    transitionMatrix = Mat::eye(4, 2, CV_32F);	//A
//    transitionMatrix = *(Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);//opencv2
    transitionMatrix=(Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);//opencv4
    controlMatrix.release();	//B
    measurementMatrix = Mat::zeros(2, 4, CV_32F);	//H
    setIdentity(measurementMatrix);
    gain = Mat::zeros(4, 2, CV_32F);	//K

    errorCovPre = Mat::zeros(4, 4, CV_32F);
    errorCovPost = Mat::zeros(4, 4, CV_32F);	//P
    setIdentity(errorCovPost, Scalar::all(1));
    processNoiseCov = Mat::eye(4, 4, CV_32F);	//Q
    setIdentity(processNoiseCov, Scalar::all(1e-3));
    measurementNoiseCov = Mat::eye(2, 2, CV_32F);	//R
    setIdentity(measurementNoiseCov, Scalar::all(1e-6));

    temp1.create(4, 4, CV_32F);
    temp2.create(2, 4, CV_32F);
    temp3.create(2, 2, CV_32F);
    temp4.create(2, 4, CV_32F);
    temp5.create(2, 1, CV_32F);
}

MyKalmanFilter::~MyKalmanFilter() {}

Point2f MyKalmanFilter::position()
{
    Point2f pt(statePost.at<float>(0), statePost.at<float>(1));

    return pt;
}
void MyKalmanFilter::kalman_correct(const Point2f center_point,const Rect& detected_box)
{
    const Mat measurement=liPointToMat(center_point);
    // printf("before correct:%d,%d\n",int(statePost.at<float>(0)),int(statePost.at<float>(1)));
    this->correct(measurement);
    this->box.width=detected_box.width;
    this->box.height=detected_box.height;
    this->box.x=max(int(statePost.at<float>(0))-(this->box.width)/2,0);
    this->box.y=max(int(statePost.at<float>(1))-(this->box.height)/2,0);
    // printf("after correct:%d,%d\n",int(statePost.at<float>(0)),int(statePost.at<float>(1)));
}
void MyKalmanFilter::kalman_predict()
{
    this->predict();
    this->box.x=int(statePost.at<float>(0))-(this->box.width)/2;
    this->box.y=int(statePost.at<float>(1))-(this->box.height)/2;
}
void MyKalmanFilter::confidenceIncrease()
{
    confidence_inc++;
    confidence_dec = 0;
    confidence +=confidence_inc;
    cout<<"in confidence:"<<confidence<<endl;
    // confidence += log(confidence_inc + 1) / log(1.5f);
}

bool MyKalmanFilter::confidenceDecrease()
{
    confidence_dec++;
    confidence_inc = 0;
    if(confidence_dec<confidence_linear_decrease_frame)//分段递减
        confidence-=confidence_dec;
    else
    {
        confidence -= pow(2.0f, (confidence_dec-6));
    }
    //cout<<"de confidence:"<<confidence<<endl;
    if (confidence < 0)
    {
        confidence = 0;
        return false;
    }

    return true;
}

MyPersonKalmanFilter::MyPersonKalmanFilter(int id_0, Point2f measurement_0,Rect box)
    :MyKalmanFilter(id_0,measurement_0,box)
{
    UpsideFlag=false;
    DownsideFlag=false;
    count_flag=false;
    this->appearside=this->defaut;
    this->disappearside=this->defaut;
    track_frame=0;
    if(measurement_0.y<=ppFlowLine)//检测框在人流线上面
    {
        this->UpsideFlag=true;
        this->appearside=this->up;
    }
    else//检测框在人流线下面
    {
        this->DownsideFlag=true;
        this->appearside=this->down;
    }

}



KalmanTracker::KalmanTracker(float targetSize_0, string targetName_0)
{
    target.clear();
    size = targetSize_0;
    name = targetName_0;
    count = target.size();
    idTabel.push_back(true);	//id 0 is always used
}

KalmanTracker::~KalmanTracker() {}

void KalmanTracker::track(vector<Point2f> measurement)
{
    int measureCount = measurement.size();
    count = target.size();
    if (!measureCount && !count)
        return;

    if (count)
    {
        //too far m2k
        for (int i = 0; i < measureCount; i++)
        {
            float dist_min = liDistance(measurement[i], target[0].position());
            for (int j = 1; j < count; j++)
            {
                float dist = liDistance(measurement[i], target[j].position());
                if (dist < dist_min)
                    dist_min = dist;
            }

            if (dist_min > size)
                target.push_back(MyKalmanFilter(idCreator(), measurement[i]));
        }
    }
    count = target.size();

    if (count < measureCount)
    {
        //match
        for (int i = 0; i < count; i++)
        {
            int label = i;
            float dist_min = liDistance(target[i].position(), measurement[i]);
            for (int j = i + 1; j < measureCount; j++)
            {
                float dist = liDistance(target[i].position(), measurement[j]);
                if (dist < dist_min)
                {
                    dist_min = dist;
                    label = j;
                }
            }
            target[i].confidenceIncrease();
            swap(measurement[label], measurement[i]);

            target[i].predict();
            target[i].correct(liPointToMat(measurement[i]));
        }

        //none match
        for (int i = count; i < measureCount; i++)
        {
            target.push_back(MyKalmanFilter(idCreator(), measurement[i]));

            target[i].predict();
            target[i].correct(liPointToMat(measurement[i]));
        }

        count = target.size();
    }
    else
    {
        //match
        for (int i = 0; i < measureCount; i++)
        {
            int label = i;
            float dist_min = liDistance(measurement[i], target[i].position());
            for (int j = i + 1; j < count; j++)
            {
                float dist = liDistance(measurement[i], target[j].position());
                if (dist < dist_min)
                {
                    dist_min = dist;
                    label = j;
                }
            }
            target[label].confidenceIncrease();
            swap(target[label], target[i]);

            target[i].predict();
            target[i].correct(liPointToMat(measurement[i]));
        }

        //none match
        bool deleteTarget0 = false;
        for (vector<MyKalmanFilter>::iterator k = target.begin() + measureCount; k != target.end(); k++)
        {
            if (!(*k).confidenceDecrease())
            {
                if (k != target.begin())
                {
                    vector<MyKalmanFilter>::iterator kt = k;
                    k--;
                    idTabelUpdate((*kt).id);
                    target.erase(kt);
                }
                else
                    deleteTarget0 = true;

                continue;
            }
            (*k).predict();
            measurement.push_back((*k).position());

            (*k).correct(liPointToMat(measurement[measurement.size() - 1]));
        }
        if (deleteTarget0)
        {
            idTabelUpdate(target[0].id);
            target.erase(target.begin());
        }

        count = target.size();
        //measureCount = measurement.size();
    }

}
vector<Point2f> KalmanTracker::trackment()
{
    vector<Point2f> trackment;
    for (int i = 0; i < target.size(); i++)
        trackment.push_back(target[i].position());

    return trackment;
}

void KalmanTracker::print(int frameCount)
{
    ;
}

Mat KalmanTracker::show(Mat src, int type_0)
{
    Mat dst = src.clone();
    for (int i = 0; i < target.size(); i++)
    {
        target[i].trajectory.push_back(target[i].position());
        if (target[i].trajectory.size() > 500)
            target[i].trajectory.pop_back();

        vector<Point2f>::const_iterator pt = target[i].trajectory.end() - 1;
        Scalar color = colorTabel(target[i].id);
        circle(dst, *pt, 5, color, 2);
        stringstream ssid;
        string sid;
        ssid << target[i].id;
        ssid >> sid;
        stringstream ssconf;
        string sconf;
        ssconf << target[i].confidence;
        ssconf >> sconf;
        string st = name + "[" + sid + "]" + sconf;
        if (!type_0)
            circle(dst, *pt, size / 2, color, 2);
        else
            rectangle(dst, Point((*pt).x - size / 2, (*pt).y - size / 2), Point((*pt).x + size / 2, (*pt).y + size / 2), color, 2);
        putText(dst, st, Point((*pt).x - size / 2, (*pt).y + size * 3 / 4), FONT_HERSHEY_SIMPLEX, 0.5, color, 2);

        while (pt != target[i].trajectory.begin())
        {
            circle(dst, *(pt - 1), 5, color, 2);
            line(dst, *(pt - 1), *pt, color, 2);
            pt--;
        }
    }
//    namedWindow(name, CV_WINDOW_AUTOSIZE);
//    imshow(name, dst);

    return dst;
}

void KalmanTracker::idTabelUpdate(int id)
{
    idTabel[id] = false;

    while (!idTabel[idTabel.size() - 1])
        idTabel.pop_back();
}

int KalmanTracker::idCreator()
{
    unsigned int id = 0;
    while (idTabel[id])
    {
        id++;
        if (id == idTabel.size())
        {
            idTabel.push_back(true);
            break;
        }
    }
    idTabel[id] = true;

    return id;
}

Scalar KalmanTracker::colorTabel(int id_0)
{
    Scalar color(0, 255, 0);
    switch (id_0)
    {
    case 0:
    case 1: break;
    case 2: color = Scalar(0, 255, 255); break;
    case 3: color = Scalar(255, 255, 0); break;
    case 4: color = Scalar(255, 0, 255); break;

    case 5: color = Scalar(18, 153, 255); break;
    case 6: color = Scalar(230, 224, 176); break;
    case 7: color = Scalar(132, 227, 255); break;
    case 8: color = Scalar(240, 32, 160); break;
    case 9: color = Scalar(203, 192, 255); break;

    case 10: color = Scalar(3, 97, 255); break;
    case 11: color = Scalar(42, 42, 128); break;
    case 12: color = Scalar(34, 139, 34); break;
    case 13: color = Scalar(255, 144, 30); break;
    case 14: color = Scalar(80, 127, 255); break;
    case 15: color = Scalar(201, 252, 189); break;

    case 16: color = Scalar(221, 160, 221); break;
    case 17: color = Scalar(0, 215, 255); break;
    case 18: color = Scalar(85, 142, 235); break;
    case 19: color = Scalar(87, 38, 135); break;
    case 20: color = Scalar(140, 180, 210); break;
    default: break;
    }

    return color;
}

PeopleKalmanTracker::PeopleKalmanTracker(float targetSize_0, string targetName_0):KalmanTracker(targetSize_0,targetName_0)
{
    up_to_down_count=0;
    down_to_up_count=0;

}
void PeopleKalmanTracker::PeopleFlow()
{
    for(int i=0;i<target.size();i++)
    {
        if(target[i].position().y>ppFlowLine&&target[i].UpsideFlag&&!target[i].DownsideFlag)
        {
            target[i].DownsideFlag=true;
            up_to_down_count++;
            target[i].count_flag=true;
        }
        else if(target[i].position().y<=ppFlowLine&&!target[i].UpsideFlag&&target[i].DownsideFlag)
        {
            target[i].UpsideFlag=true;
            down_to_up_count++;
            target[i].count_flag=true;
        }
    }
}
void PeopleKalmanTracker::PeopleFlowFix(MyPersonKalmanFilter person)
{
    if(person.position().y<=ppFlowLine)
    {
        person.disappearside=person.up;
    }
    else
    {
        person.disappearside=person.down;
    }
    if(person.disappearside==person.appearside&&person.count_flag)//同方向进出,且该行人已经计数
    {
        if(person.appearside==person.up)
        {
            this->up_to_down_count--;
        }
        else
        {
            this->down_to_up_count--;
        }
    }
}
void PeopleKalmanTracker::track(vector<Point2f> measurement,std::vector<cv::Rect>ppDetectionRect)
{
    /*该函数用来计算检测框与跟踪框的相似度矩阵 匈牙利匹配 以及kalman跟踪*/
    if(measurement.size()!=ppDetectionRect.size())//矩形框中心和矩形框的数量应该要相等
        cerr<<"矩形框中心和矩形框的数量应该要相等!";
    //首先使用卡尔曼滤波预测位置
    for(int i=0;i<target.size();i++)
        target[i].kalman_predict();
    // for(int i=0;i<target.size();i++)
    //     printf("kalman predict:%d,%d\n",target[i].kalman_predict());
    //计算距离矩阵
    std::vector<cv::Rect>tracking_boxes;
    for(int i=0;i<target.size();i++)
        tracking_boxes.push_back(target[i].box);
    const int rows=ppDetectionRect.size();
    const int cols=tracking_boxes.size();
    vector<vector<BoxDistanceType>>Distance_Matrix(rows,vector<BoxDistanceType>(cols));
    vector<vector<BoxDistanceType>>Distance_Matrix_reverse(cols,vector<BoxDistanceType>(rows));
    calculate_Distance_matrix(ppDetectionRect,tracking_boxes,Distance_Matrix,Distance_Matrix_reverse);
    //首先根据距离来对矩形框进行滤波
    vector<int>choose_rows,choose_cols;
    const BoxDistanceType distance_threshold=0.99;//距离超过0.99的将其另外考虑
    for(int i=0;i<rows;i++)
        choose_rows.push_back(i);
    for(int i=0;i<cols;i++)
        choose_cols.push_back(i);

    Distance_matrix_Filter(Distance_Matrix,Distance_Matrix_reverse,distance_threshold,
                           choose_rows,choose_cols);
#ifdef DEBUG
    cout<<"检测数:"<<rows<<"kalman数:"<<cols<<endl;
    cout<<"距离矩阵"<<endl;
    for(int row=0;row<Distance_Matrix.size();row++)
    {
        for(int col=0;col<Distance_Matrix[0].size();col++)
        {
            cout<<Distance_Matrix[row][col]<<" ";
        }
        cout<<endl;
    }
#endif
    vector<vector<BoxDistanceType>>filtered_distanceMatrix;
    filtered_distanceMatrix=get_filtered_Matrix(Distance_Matrix,choose_rows,choose_cols);
#ifdef DEBUG
    cout<<"choose_rows"<<endl;
    for(auto num:choose_rows)
        cout<<num<<" ";
    cout<<endl;
    cout<<"choose_cols"<<endl;
    for(auto num:choose_cols)
        cout<<num<<" ";
    cout<<endl;
#endif
    //匈牙利匹配
    vector<vector<BoxDistanceType>>match_result_Matrix;
    vector<pair<int,int>>pairs;//匹配成功的对
    set<int>rows_set;//匹配成功的行
    set<int>cols_set;//匹配成功的列
    if(filtered_distanceMatrix.size()>0)
    {
        Std2dVectorAdapter<BoxDistanceType>std2matrix;
        Matrix<BoxDistanceType> matrix=std2matrix.convertToMatrix(filtered_distanceMatrix);
        Munkres<BoxDistanceType> m;
        m.solve(matrix);
        std2matrix.convertFromMatrix(match_result_Matrix,matrix);

        //匹配之后需要找出匹配成功的对
        for(int row=0;row<match_result_Matrix.size();++row)
        {
            for(int col=0;col<match_result_Matrix[0].size();++col)
            {
                if(isequal(match_result_Matrix[row][col],0.0))//结果矩阵中=0的代表匹配成功
                {
                    rows_set.insert(choose_rows[row]);
                    cols_set.insert(choose_cols[col]);
                    pairs.push_back({choose_rows[row],choose_cols[col]});
                    break;
                }
            }
        }
    }
#ifdef DEBUG
    cout<<"匹配成功的pairs数:"<<pairs.size()<<endl;
    for(auto pair:pairs)
        cout<<pair.first<<" "<<pair.second<<endl;
#endif
    //对于匹配成功的跟踪目标,需要更新其坐标,以及提升其置信度
    for(auto &iter:pairs)
    {
        target[iter.second].track_frame++;//跟踪帧数增加
        target[iter.second].confidenceIncrease();
        target[iter.second].kalman_correct(measurement[iter.first],ppDetectionRect[iter.first]);
    }
    PeopleFlow();//人流量计数
    //对于距离太远的跟踪目标,需要降低其置信度,置信度低于一定值,将其从后往前删除
    for (vector<MyPersonKalmanFilter>::iterator k = target.end()-1; k != target.begin()-1; k--)
    {
        if(cols_set.find(std::distance(target.begin(),k))!=cols_set.end())//如果匈牙利匹配成功
            continue;
        else if (!(*k).confidenceDecrease())//置信度减到0以下,剔除该目标
        {
            idTabelUpdate((*k).id);
            PeopleFlowFix(*k);//修正人流量
            target.erase(k);
        }
        else//置信度没减到0以下,用上次的跟踪结果更新卡尔曼滤波器
        {
            (*k).track_frame++;//跟踪帧数增加
            (*k).kalman_correct((*k).position(),(*k).box);
        }

    }
    //对于距离太远的检测目标,将其作为新目标,加入到跟踪列表中
    for(int i=0;i<measurement.size();i++)
    {
        #ifdef DEBUG
        cout<<"产生新目标"<<endl;
        #endif
        if(rows_set.find(i)==rows_set.end())//没有匈牙利匹配成功的目标
        {
            target.push_back(MyPersonKalmanFilter(idCreator(), measurement[i],ppDetectionRect[i]));
        }
    }
}



