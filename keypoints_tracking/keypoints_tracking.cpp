/* 
Author:HLG
Date:2020年6月5日
huang.liguang@qq.com
*/
//#define _GLIBCXX_USE_CXX11_ABI 0
#include<iostream>
#include<math.h>
#include<unordered_map>
#include<munkres.h>
#include<adapters/adapter.h>
#include<adapters/std2dvectordapter.h>
#include<keypoints_tracking.h>
#include<my_tracking_function.h>
// using namespace std;
namespace hlg{
static bool isequal(double num,double target)
{
    if(abs(num-target)<0.000001)
        return true;
    else
        return false;
}
static double oks_max=DBL_MIN;
static double calculate_OKS(Keypoints skeleton1,Keypoints skeleton2,std::vector<int>bones_info)
{
    //计算oks，但由于是跟踪用，并没有真实值，因此对OKS进行了自定义，用另一种距离方式来计算两个骨架之间的距离，详情见小论文
    //当两个骨架越不相似时，自定义的oks越大 oks最大时为keypoints_num*keypoint_oks_max+box_oks_max=14×1.0+5.0=19.0
    int keypoints_num=skeleton1.keypoints_num;
    double distance_square_thres=60*60;//距离平方阈值，如果两个骨架的相同关键点欧式距离平方超过这个阈值，则oks+=1
    double confidence_thres=0.2;//置信度阈值，如果两个骨架的相同关键点中的某一个置信度小于这个阈值，则oks+=1
    double keypoint_oks_max=1.0;
    double box_square_thres=200*200;//矩形框中心距离阈值，如果超过这个阈值，则oks+=box_oks_max
    double box_oks_max=5.0;
    double oks=20.0;//初始给20
    oks_max=keypoints_num*keypoint_oks_max+box_oks_max+oks;
    
    for(int i=0;i<keypoints_num;i++)
    {
        if((skeleton1.C)[i]>confidence_thres&&(skeleton2.C)[i]>confidence_thres)//一直检测到 高置信度点
        {
            double euclidean_distance=((skeleton1.X)[i]-(skeleton2.X)[i])*((skeleton1.X)[i]-(skeleton2.X)[i])+
                                      ((skeleton1.Y)[i]-(skeleton2.Y)[i])*((skeleton1.Y)[i]-(skeleton2.Y)[i]);
            oks+=(keypoint_oks_max*std::min(euclidean_distance,distance_square_thres)/distance_square_thres);//即此时每个关键点距离最多使得oks增加keypoint_oks_max，在distance_square_thres取得keypoint_oks_max。距离越远，增加越快，要增加得更快可以使用三次方      
        }
        else if((skeleton1.C)[i]<confidence_thres&&(skeleton1.C)[i]>0.0001&&(skeleton2.C)[i]<confidence_thres&&(skeleton2.C)[i]>0.0001)//低置信度点 为了oks尽量可靠，置信度低的点感觉可以尽量少参与其中
        {
            oks+=0.5;
        }
        else if((skeleton1.C)[i]<0.0001&&(skeleton2.C)[i]>confidence_thres)//突然没检测到 遮挡变严重
        {
            oks+=0.0;
        }
        else if((skeleton1.C)[i]>confidence_thres&&(skeleton2.C)[i]<0.0001)//突然检测到 遮挡减缓
        {
            oks+=0.0;//oks不变
        }
        else if((skeleton1.C)[i]<0.0001&&(skeleton2.C)[i]<0.0001)//一直没检测到
        {
            oks+=0.0;//oks不变
        }
                   
    }
    //std::cout<<std::endl<<"keypoint_oks:"<<oks<<std::endl;
    //使用二次函数，这样可以满足外接矩形框距离远的时候，oks增加地快一些，距离近的时候oks增加地慢一些
    double center_distance_square=(skeleton1.center_point.x-skeleton2.center_point.x)*(skeleton1.center_point.x-skeleton2.center_point.x)+
                           (skeleton1.center_point.y-skeleton2.center_point.y)*(skeleton1.center_point.y-skeleton2.center_point.y);
    oks+=(box_oks_max*std::min(center_distance_square,box_square_thres)/box_square_thres);//即此时矩形框中心最多使得oks增加box_oks_max，距离越远，增加越快，要增加得更快可以使用三次方
    //std::cout<<"center_distance_square:"<<center_distance_square<<"center_oks"<<oks<<std::endl;
    //计算骨架枝干距离
    int bones_num=bones_info.size()/2;
    double keypoint_square_thres=55*55;
    double line_distance_thres=45;
    double oks_bones=2.0;//如果有一条枝干相似，则oks-=oks_bones
    for(int i=0;i<bones_num;i++)
    {
        const int keypoint1_id=bones_info[2*i];
        const int keypoint2_id=bones_info[2*i+1];
        double person1_keypoint1_c=(skeleton1.C)[keypoint1_id];
        double person1_keypoint2_c=(skeleton1.C)[keypoint2_id];
        double person2_keypoint1_c=(skeleton2.C)[keypoint1_id];
        double person2_keypoint2_c=(skeleton2.C)[keypoint2_id]; 
        if(person1_keypoint1_c>confidence_thres&&person1_keypoint2_c>confidence_thres&&person2_keypoint1_c>confidence_thres&&person2_keypoint2_c>confidence_thres)
        {
            double person1_keypoint1_x=(skeleton1.X)[keypoint1_id];
            double person1_keypoint1_y=(skeleton1.Y)[keypoint1_id];
            double person1_keypoint2_x=(skeleton1.X)[keypoint2_id];
            double person1_keypoint2_y=(skeleton1.Y)[keypoint2_id]; 
            
            double person2_keypoint1_x=(skeleton2.X)[keypoint1_id];
            double person2_keypoint1_y=(skeleton2.Y)[keypoint1_id];
            double person2_keypoint2_x=(skeleton2.X)[keypoint2_id];
            double person2_keypoint2_y=(skeleton2.Y)[keypoint2_id];
            
            double line_distance=Min_Line_Distance(cv::Point(person1_keypoint1_x,person1_keypoint1_y),cv::Point(person1_keypoint2_x,person1_keypoint2_y),
                                                   cv::Point(person2_keypoint1_x,person2_keypoint1_y),cv::Point(person2_keypoint2_x,person2_keypoint2_y));
            double keypoint1_square_distance=(person1_keypoint1_x-person2_keypoint1_x)*(person1_keypoint1_x-person2_keypoint1_x)+
                                            (person1_keypoint1_y-person2_keypoint1_y)*(person1_keypoint1_y-person2_keypoint1_y);
            double keypoint2_square_distance=(person1_keypoint2_x-person2_keypoint2_x)*(person1_keypoint2_x-person2_keypoint2_x)+
                                            (person1_keypoint2_y-person2_keypoint2_y)*(person1_keypoint2_y-person2_keypoint2_y);

            if(line_distance<line_distance_thres&&std::max(keypoint1_square_distance,keypoint2_square_distance)<keypoint_square_thres)
            {              
                //计算直线枝干直线夹角
                double k1=double(person1_keypoint2_y-person1_keypoint1_y)/double(person1_keypoint2_x-person1_keypoint1_x);
                double k2=(person2_keypoint2_y-person2_keypoint1_y)/(person2_keypoint2_x-person2_keypoint1_x);
                double theta=acos(abs((1+k1*k2)/(sqrt(1+k1*k1)*sqrt(1+k2*k2))));
                theta=theta*180/M_PI;
                if(theta<30)
                {       
                    oks-=oks_bones;
    //                if(i==6)
    //                {
    //                    std::cout<<"line_distance:"<<line_distance<<std::endl;
    //                    std::cout<<"theta:"<<theta<<std::endl; 
    //                    std::cout<<"oks:"<<oks<<std::endl;                          
    //                }
                }
               // std::cout<<"bones_num"<<bones_num<<" line_distance:"<<line_distance<<" theta"<<theta<<std::endl;
            }
           // std::cout<<std::endl;  
            
        }
        
        
    }
    
    //std::cout<<"final_oks:"<<oks<<std::endl;
    std::max(0.0,oks);
    
    return oks;
}
static void calculate_Distance_matrix(std::vector<Keypoints>detected_keypoints,std::vector<Keypoints>tracking_keypoints,std::vector<int>bones,std::vector<std::vector<double>>&Distance_Matrix,std::vector<std::vector<double>>&Distance_Matrix_Reverse)
{
    //std::cout<<"rows"<<Distance_Matrix.size()<<"cols"<<Distance_Matrix[0].size()<<std::endl;
    //std::cout<<std::endl<<"next frame "<<"det"<<detected_keypoints.size()<<" track"<<tracking_keypoints.size()<<std::endl;
    
    for(int row=0;row<Distance_Matrix.size();row++)
    {
        for(int col=0;col<Distance_Matrix[0].size();col++)
        {
            //std::cout<<"row:"<<row<<"col"<<col<<std::endl; 
            Distance_Matrix[row][col]=calculate_OKS(detected_keypoints[row],tracking_keypoints[col],bones);//oks越小,距离越大
            //std::cout<<"oks:"<<Distance_Matrix[row][col]<<std::endl; 
            //std::cout<<Distance_Matrix[row][col]<<" ";
            Distance_Matrix_Reverse[col][row]=Distance_Matrix[row][col];
        }
        //std::cout<<std::endl;
    }

}
void Distance_matrix_Filter(std::vector<std::vector<double>>&Distance_Matrix,std::vector<std::vector<double>>&Distance_Matrix_Reverse,
                            double threshold,std::vector<int>&choose_rows,std::vector<int>&choose_cols)
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
std::vector<std::vector<double>> get_filtered_Matrix(std::vector<std::vector<double>>Matrix_origin,std::vector<int>choose_rows,std::vector<int>choose_cols)
{
    int rows=0,cols=0;
    if(choose_rows.size()>0&&choose_cols.size()>0)
    {
        rows=choose_rows.size();
        cols=choose_cols.size();
    }
    std::vector<std::vector<double>>filtered_Matrix(rows,std::vector<double>(cols));
    for(int row=0;row<rows;row++)
    {
        for(int col=0;col<cols;col++)
        {
            filtered_Matrix[row][col]=Matrix_origin[choose_rows[row]][choose_cols[col]];
        }
    }
    return filtered_Matrix;
}
int count_keypoints_num(){
    //根据骨架点连线，来数一下有多少个关键点,顺便将关键点的序号进行重排，写来一开始调试用的。。后来并没有用上 
    const std::vector<int>bones={  1,2,   1,5,   2,3,   3,4,   5,6,   6,7,   2,9,   5,12,     9,10,    10,11,    12,13, 13,14,  1,0  };
    std::unordered_map<int,bool>m;
    for(int i=0;i<bones.size();++i)
    {
        if(m.find(bones[i])==m.end())
            m.insert(std::pair<int, bool>(bones[i], true));
    }

    //重排顺序
    std::vector<int>bones_resort;
    std::unordered_map<int,bool> ::iterator it;
    it=m.begin();
    while(it != m.end())
    {
        bones_resort.push_back(it->first);
        // cout<<"first->:"<<it->first<<endl;
        // cout<<"second->:"<<it->second<<endl;
        it++;
    } 
    sort(bones_resort.begin(),bones_resort.end());
    std::unordered_map<int,int>remap;
    for(int i=0;i<bones_resort.size();i++)
    {
        if(remap.find(bones_resort[i])==remap.end())
        {

            remap.insert(std::pair<int, int>(bones_resort[i], i));
            // printf("%d,%d,%d\n",bones_resort[i], i,remap[bones_resort[i]] );
        }
    }
   
    for(int i=0;i<bones.size();i++)
    {
        
        std::cout<<remap[bones[i]]<<',';
    }
    std::cout<<std::endl;


    std::cout<<"size"<<m.size()<<std::endl;
    return m.size();
    // cout<<"test in  keypoints_tracking.cpp"<<endl;
}
void Single_Skeleton::add_trajectory(cv::Point point)
{
    if(trajectory.size()>=max_trajectory_size)
    {
        trajectory.erase(trajectory.begin());
    }
    trajectory.push_back(point);
}
Single_Skeleton::Single_Skeleton(int id,int keypoints_num,Keypoints keypoints,double kalman_confidence_thres):person_keypoints(keypoints)
{
    this->keypoints_num=keypoints_num;
    this->id=id;
    this->color=color_list[id%color_list.size()];
    // this->max_trajectory_size=50;
    //置信度参数初始化
    confidence = 15;//初始置信度给15
    confidence_inc = 0;
    confidence_dec = 0;
    confidence_linear_decrease_frame=10;
    //关键点坐标置信度初始化
    //卡尔曼跟踪参数初始化
    for (int i = 0; i < keypoints_num; ++i)
    {
        this->keypoints_kalmanfilter.push_back(cv::KalmanFilter(4,2,0));//状态变量、观测变量、控制变量的维度
        track_keypoint_confidence.push_back(0);
    }
    
    for(int i=0;i<keypoints_num;i++)
    {
        keypoints_kalmanfilter[i].statePre = cv::Mat::zeros(4, 1, CV_32F);
        keypoints_kalmanfilter[i].statePost = cv::Mat::zeros(4, 1, CV_32F);   //x
        if((person_keypoints.C)[i]==0.0)//x y c =0
        {
            keypoints_kalmanfilter[i].statePost.at<float>(0) = 0.0;
            keypoints_kalmanfilter[i].statePost.at<float>(1) = 0.0;
        }
        else
        {
            keypoints_kalmanfilter[i].statePost.at<float>(0) = (person_keypoints.X)[i];
            keypoints_kalmanfilter[i].statePost.at<float>(1) = (person_keypoints.Y)[i];
        }
        

    //    transitionMatrix = *(Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);//opencv2
        keypoints_kalmanfilter[i].transitionMatrix=(cv::Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1);//opencv4
        keypoints_kalmanfilter[i].controlMatrix.release();    //B
        keypoints_kalmanfilter[i].measurementMatrix = cv::Mat::zeros(2, 4, CV_32F);   //H
        setIdentity(keypoints_kalmanfilter[i].measurementMatrix);
        keypoints_kalmanfilter[i].gain = cv::Mat::zeros(4, 2, CV_32F);    //K

        keypoints_kalmanfilter[i].errorCovPre = cv::Mat::zeros(4, 4, CV_32F);
        keypoints_kalmanfilter[i].errorCovPost = cv::Mat::zeros(4, 4, CV_32F);    //P
        setIdentity(keypoints_kalmanfilter[i].errorCovPost, cv::Scalar::all(1));
        keypoints_kalmanfilter[i].processNoiseCov = cv::Mat::eye(4, 4, CV_32F);   //Q
        setIdentity(keypoints_kalmanfilter[i].processNoiseCov, cv::Scalar::all(1e-3));
        keypoints_kalmanfilter[i].measurementNoiseCov = cv::Mat::eye(2, 2, CV_32F);   //R
        setIdentity(keypoints_kalmanfilter[i].measurementNoiseCov, cv::Scalar::all(1e-6));

        keypoints_kalmanfilter[i].temp1.create(4, 4, CV_32F);
        keypoints_kalmanfilter[i].temp2.create(2, 4, CV_32F);
        keypoints_kalmanfilter[i].temp3.create(2, 2, CV_32F);
        keypoints_kalmanfilter[i].temp4.create(2, 4, CV_32F);
        keypoints_kalmanfilter[i].temp5.create(2, 1, CV_32F);
    }

 
    
}
void Single_Skeleton::person_predict()
{
    for(int i=0;i<this->keypoints_kalmanfilter.size();++i)
    {//对每个关键点进行卡尔曼预测
        if((person_keypoints.C)[i]>kalman_confidence_thres)
        {
            this->keypoints_kalmanfilter[i].predict();
                 
            (person_keypoints.X)[i]=double(this->keypoints_kalmanfilter[i].statePost.at<float>(0));
            (person_keypoints.Y)[i]=double(this->keypoints_kalmanfilter[i].statePost.at<float>(1));
            x_y_limit((person_keypoints.X)[i],(person_keypoints.Y)[i]);
            keypoints_kalmanfilter[i].statePost.at<float>(0)=(person_keypoints.X)[i];  
            keypoints_kalmanfilter[i].statePost.at<float>(1)=(person_keypoints.Y)[i];         
        }
        
      
    } 
}

void Single_Skeleton::person_correct(Keypoints detected_keypoint)
{
    //匹配成功时，使用检测的关键点对跟踪关键点进行更新
    
    for(int i=0;i<keypoints_num;i++)
    {
        if((detected_keypoint.C)[i]>kalman_confidence_thres)//如果该关键点检测成功使用检测结果来对跟踪结果更新
        {  
            x_y_limit((detected_keypoint.X)[i],(detected_keypoint.Y)[i]);
            if((person_keypoints.C)[i]==0.0)//之前该关键点置信度为0 先对kalman坐标以及一些参数初始化
            {
                keypoints_kalmanfilter[i].statePost.at<float>(0) = (detected_keypoint.X)[i];
                keypoints_kalmanfilter[i].statePost.at<float>(1) = (detected_keypoint.Y)[i];
                
                keypoints_kalmanfilter[i].errorCovPre = cv::Mat::zeros(4, 4, CV_32F);
                keypoints_kalmanfilter[i].errorCovPost = cv::Mat::zeros(4, 4, CV_32F);    //P
                setIdentity(keypoints_kalmanfilter[i].errorCovPost, cv::Scalar::all(1));
                keypoints_kalmanfilter[i].processNoiseCov = cv::Mat::eye(4, 4, CV_32F);   //Q
                setIdentity(keypoints_kalmanfilter[i].processNoiseCov, cv::Scalar::all(1e-3));
                keypoints_kalmanfilter[i].measurementNoiseCov = cv::Mat::eye(2, 2, CV_32F);   //R
                setIdentity(keypoints_kalmanfilter[i].measurementNoiseCov, cv::Scalar::all(1e-6));

                keypoints_kalmanfilter[i].temp1.create(4, 4, CV_32F);
                keypoints_kalmanfilter[i].temp2.create(2, 4, CV_32F);
                keypoints_kalmanfilter[i].temp3.create(2, 2, CV_32F);
                keypoints_kalmanfilter[i].temp4.create(2, 4, CV_32F);
                keypoints_kalmanfilter[i].temp5.create(2, 1, CV_32F);
            }
            else
            {
                cv::Mat point_mat = (cv::Mat_<float>(2, 1) << (detected_keypoint.X)[i], (detected_keypoint.Y)[i]);
                keypoints_kalmanfilter[i].correct(point_mat);
            }
            (person_keypoints.X)[i]=(detected_keypoint.X)[i];
            (person_keypoints.Y)[i]=(detected_keypoint.Y)[i];
        }   
        (person_keypoints.C)[i]=(detected_keypoint.C)[i];        
    }
    add_trajectory(person_keypoints.center_point_update());//对骨架外界矩形进行更新
}

void Skeleton_Tracking::idTabelUpdate(int id)
{
    idTabel[id] = false;

    while (!idTabel[idTabel.size() - 1])
        idTabel.pop_back();
}

int Skeleton_Tracking::idCreator()
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

void Skeleton_Tracking::skeletons_track(std::vector<std::vector<double>>detected_skeletons)
{
    const int detected_num=detected_skeletons.size();
    const int tracker_num=people_skeletons.size();
    for(int i=0;i<tracker_num;i++)
    {//对每个人进行卡尔曼预测
        this->people_skeletons[i].person_predict();
    }

    
    std::vector<Keypoints> detected_keypoints;
    std::vector<Keypoints>tracking_keypoints;
    for(int i=0;i<detected_num;i++)
    {
        //将二维vector变成Keypoints类型，方便后面求取oks
        detected_keypoints.push_back(Keypoints(this->keypoints_num,detected_skeletons[i]));
    }
    for(int i=0;i<tracker_num;i++)
    {
        //将二维vector变成Keypoints类型，方便后面求取oks
        tracking_keypoints.push_back(people_skeletons[i].person_keypoints);
    }
    //距离矩阵计算及滤波
    const int rows=detected_num;
    const int cols=tracker_num;
    std::vector<std::vector<double>>Distance_Matrix(rows,std::vector<double>(cols));//骨架距离矩阵
    std::vector<std::vector<double>>Distance_Matrix_reverse(cols,std::vector<double>(rows));//骨架距离矩阵转置，以空间换时间
    //std::cout<<"rows"<<rows<<"cols"<<cols<<std::endl;
    calculate_Distance_matrix(detected_keypoints,tracking_keypoints,bones_info,Distance_Matrix,Distance_Matrix_reverse);
    //首先根据距离来对矩形框进行滤波,如果骨架距离超过阈值，需要另外计算，如果在检测结果中，则当作新检测目标加入，如果在跟踪列表中，则当作离开的目标进行去除
    std::vector<int>choose_rows,choose_cols;
    const double distance_threshold=oks_max*9.9/10.0;//距离超过distance_threshold的将其另外考虑
    for(int i=0;i<rows;i++)//先把所有检测结果的序号都放到choose_rows中，后续再进行剔除
        choose_rows.push_back(i);
    for(int i=0;i<cols;i++)//先把所有跟踪目标的序号都放到choose_cols中，后续再进行剔除
        choose_cols.push_back(i);
    Distance_matrix_Filter(Distance_Matrix,Distance_Matrix_reverse,distance_threshold,
                           choose_rows,choose_cols);
    std::vector<std::vector<double>>filtered_distanceMatrix;//得到剔除之后的矩阵
    filtered_distanceMatrix=get_filtered_Matrix(Distance_Matrix,choose_rows,choose_cols);
    //匈牙利匹配
    std::vector<std::vector<double>>match_result_Matrix;
    std::vector<std::pair<int,int>>pairs;//匹配成功的对
    std::set<int>rows_set;//匹配成功的行
    std::set<int>cols_set;//匹配成功的列
    if(filtered_distanceMatrix.size()>0)
    {
        Std2dVectorAdapter<double>std2matrix;
        Matrix<double> matrix=std2matrix.convertToMatrix(filtered_distanceMatrix);
        Munkres<double> m;
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

    //对于匹配成功的乘客骨架,需要更新其坐标,以及提升其置信度  
    for(auto &iter:pairs)
    {
        this->people_skeletons[iter.second].track_frame++;//跟踪帧数增加
        this->people_skeletons[iter.second].confidenceIncrease();
        this->people_skeletons[iter.second].person_correct(detected_keypoints[iter.first]);//对卡尔曼滤波的坐标进行更新
    }
    //对于距离太远或者匈牙利匹配失败的跟踪目标,需要降低其置信度,置信度低于一定值,将其从后往前删除
    for (std::vector<Single_Skeleton>::iterator k = people_skeletons.end()-1; k != people_skeletons.begin()-1; k--)
    {
        if(cols_set.find(std::distance(people_skeletons.begin(),k))!=cols_set.end())//如果匈牙利匹配成功
            continue;
        else if (!(*k).confidenceDecrease())//置信度减到0以下,剔除该目标
        {
            idTabelUpdate((*k).id);
            people_skeletons.erase(k);
        }
        else//置信度没减到0以下,用上次的跟踪结果更新卡尔曼滤波器
        {
            (*k).track_frame++;//跟踪帧数增加
            (*k).person_correct((*k).person_keypoints);// 使用预测结果来进行更新
        }

    }

    // //对于距离太远的乘客骨架,将其作为新目标,加入到跟踪列表中
    for(int i=0;i<detected_num;i++)
    {   
        // std::cout<<"detected_num:"<<detected_num<<std::endl;
        #ifdef DEBUG
        cout<<"产生新目标"<<endl;
        #endif
        if(rows_set.find(i)==rows_set.end())//没有匈牙利匹配成功的目标
        {

            // std::cout<<id<<" "<<this->keypoints_num<<std::endl;
            // printf("%d,%d\n",id, this->keypoints_num);
            people_skeletons.push_back(Single_Skeleton(idCreator(),this->keypoints_num, detected_keypoints[i]));
        }

    }
}
void Skeleton_Tracking::draw_skeletons(cv::Mat img,double confidence_thres,bool draw_trajectory)
{//画出置信度超过阈值的关键点所在的骨架
    int people_num=people_skeletons.size();
    for(int i=0;i<people_num;++i) {
        cv::Scalar color=people_skeletons[i].color;
        for(int j=0;j<bones_num;++j)
        {
            const int index1=bones_info[j*2];
            const int index2=bones_info[j*2+1];
            
            const double confidence1=((people_skeletons[i].person_keypoints).C)[index1];//第i个人的第index1个关键点的置信度
            const double confidence2=((people_skeletons[i].person_keypoints).C)[index2];//第i个人的第index2个关键点的置信度
            // std::cout<<confidence1<<" "<<confidence2<<std::endl;
            if(std::min(confidence1,confidence2)>confidence_thres)
            {
                double point1_x=((people_skeletons[i].person_keypoints).X)[index1];
                double point1_y=((people_skeletons[i].person_keypoints).Y)[index1];
                double point2_x=((people_skeletons[i].person_keypoints).X)[index2];
                double point2_y=((people_skeletons[i].person_keypoints).Y)[index2];
                x_y_limit(point1_x,point1_y);
                x_y_limit(point2_x,point2_y);
                if(point1_x<1&&point1_y<1||point2_x<1&&point2_y<1)
                {
                    std::cout<<"person:"<<i<<"color:"<<color<<" "<<"error confidence:"<<confidence1<<" "<<confidence2<<std::endl;
                    std::cout<<"point1_x:"<<point1_x<<" "<<point1_y<<" "<<" point2:"<<point2_x<<" "<<point2_y<<std::endl;
                }
                    
                const cv::Point point1(point1_x,point1_y);                       
                const cv::Point point2(point2_x,point2_y);
                cv::line(img,point1,point2, color, 2 );
            }
            
            // cv::circle(dst,cv::Point(result[i*3],result[i*3+1]),2,cv::Scalar(0,255,0),-1);
            // putText(dst, to_string(i), cv::Point(result[i*3],result[i*3+1]), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 1);
        }
        //如果需要画出行人中心轨迹
        if(draw_trajectory)
        {
            if(people_skeletons[i].trajectory.size()>0)
            {
        
                cv::circle(img,people_skeletons[i].trajectory.back(),1,color,-1);
                for(int k=people_skeletons[i].trajectory.size()-1;k>0;--k)
                {
                    cv::Point point1=people_skeletons[i].trajectory[k];
                    cv::Point point2=people_skeletons[i].trajectory[k-1];
                    cv::line(img,point1,point2,color,1);
                }
            }

        }
        //画出乘客的id和置信度，用来调试用
        //cv::Point center_point=people_skeletons[i].person_keypoints.center_point;
//        cv::putText(img, std::to_string(i), center_point, cv::FONT_HERSHEY_SIMPLEX, 0.6, color, 1);
  //      center_point.y+=20;
    //    double confidence=people_skeletons[i].confidence;
      //  cv::putText(img, std::to_string(confidence), center_point, cv::FONT_HERSHEY_SIMPLEX, 0.6, color, 1);
    }
    
}
}

