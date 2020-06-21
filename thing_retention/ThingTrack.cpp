#include"ThingTrack.h"
namespace hlg
{
    Thing::Thing(int id, const Rect &detected_box)
    {
        this->track_frame = 0;
        this->confidence = 4;
        this->confidence_inc = 0;
        this->confidence_dec = 0;
        this->id = id;
        this->box = detected_box;
    }
    void Thing::confidenceIncrease()
    {
        confidence_inc++;
        confidence_dec = 0;
        confidence += log(confidence_inc + 1) / log(1.5f);
    }

    bool Thing::confidenceDecrease()
    {
        confidence_dec++;
        confidence_inc = 0;
        confidence -= pow(1.5f, confidence_dec);
        if (confidence < 0)
        {
            confidence = 0;
            return false;
        }

        return true;
    }
    vector<vector<int>> &ThingInterface::ThingTracker::GetThingsInfo()
    {
        ThingsInfo.clear();
        const int size = tracking_things.size();
        ThingsInfo.resize(size);
        for (int i = 0; i < size; ++i)
        {
            ThingsInfo[i] = { tracking_things[i].box.x,tracking_things[i].box.y,
                tracking_things[i].box.x + tracking_things[i].box.width - 1,
                tracking_things[i].box.y + tracking_things[i].box.height - 1,
                int(tracking_things[i].track_frame),tracking_things[i].id};
        }
        return ThingsInfo;
    }
    ThingInterface::ThingTracker::ThingTracker(string thing_name)
    {
        tracking_things.clear();
        name = thing_name;
        idTabel.push_back(true);	//id 0 is always used

    }
    int ThingInterface::ThingTracker::idCreator()
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
    void ThingInterface::ThingTracker::idTabelDelete(int id)
    {
        idTabel[id] = false;

        while (!idTabel[idTabel.size() - 1])
            idTabel.pop_back();
    }
    void ThingInterface::ThingTracker::Filter_People(vector<Rect>&things_boxes,const vector<Rect>&people_boxes)
    {
        static double iou_threshold = 0.01;
        for (int i = things_boxes.size()-1; i >=0 ; --i)
        {
            const Rect&rect_th = things_boxes[i];
            for (int j = 0; j < people_boxes.size(); ++j)
            {
                const Rect&rect_pp = people_boxes[j];
                //const cv::Rect And = rect_th | rect_pp;
                const cv::Rect Union = rect_th & rect_pp;
                const double iou= double(Union.area()*1.0 / rect_pp.area());//去掉与people_box重叠面积比较大的检测things_box
                // cout<<"det iou"<<iou<<endl;
                if (iou > iou_threshold)
                {
                    things_boxes.erase(things_boxes.begin() + i);
                    break;
                }         
                
            }
        }
        for (int i = tracking_things.size()-1; i >=0 ; --i)
        {
            const Rect&rect_th_track = tracking_things[i].box;
            for (int j = 0; j < people_boxes.size(); ++j)
            {
                const Rect&rect_pp = people_boxes[j];
                //const cv::Rect And = rect_th | rect_pp;
                const cv::Rect Union = rect_th_track & rect_pp;
                const double iou= double(Union.area()*1.0 / rect_pp.area());//去掉与people_box重叠面积比较大的跟踪things_box
                //  cout<<"track iou:"<<rect_th_track.area()<<" "<<Union.area()<<" "<<rect_pp.area()<<" "<<iou<<endl;
                if (iou > iou_threshold)
                {
                    idTabelDelete(tracking_things[i].id);
                    tracking_things.erase(tracking_things.begin() + i);
                    break;
                }         
                
            }
        }
    }
    void ThingInterface::ThingTracker::track(const vector<Rect>&Thing_Detected ,const vector<Rect>&people_boxes)
    {
        vector<Rect>detected_rects = Thing_Detected;
        
        Filter_People(detected_rects,people_boxes);//根据人的位置，进行过滤
        //计算距离矩阵
        std::vector<cv::Rect>tracking_boxes;
        for (int i = 0; i < tracking_things.size(); ++i)
            tracking_boxes.push_back(tracking_things[i].box);
        const int rows = detected_rects.size();
        const int cols = tracking_boxes.size();
        vector<vector<BoxDistanceType>>Distance_Matrix(rows, vector<BoxDistanceType>(cols));
        vector<vector<BoxDistanceType>>Distance_Matrix_reverse(cols, vector<BoxDistanceType>(rows));
        calculate_Distance_matrix(detected_rects, tracking_boxes, Distance_Matrix, Distance_Matrix_reverse);

        //首先根据距离来对矩形框进行滤波
        vector<int>choose_rows, choose_cols;
        const BoxDistanceType distance_threshold = 0.80;//距离超过0.99的将其另外考虑，如果在检测对列则当作新物体，在跟踪对列则要剔除
        for (int i = 0; i<rows; i++)
            choose_rows.push_back(i);
        for (int i = 0; i<cols; i++)
            choose_cols.push_back(i);
        Distance_matrix_Filter(Distance_Matrix, Distance_Matrix_reverse, distance_threshold,
            choose_rows, choose_cols);
        //根据滤波的结果，得到滤波之后的待用来匈牙利匹配的矩阵
        vector<vector<BoxDistanceType>>filtered_distanceMatrix;
        filtered_distanceMatrix = get_filtered_Matrix(Distance_Matrix, choose_rows, choose_cols);

        //匈牙利匹配
        vector<vector<BoxDistanceType>>match_result_Matrix;
        vector<pair<int, int>>pairs;//匹配成功的对
        set<int>rows_set;//匹配成功的行
        set<int>cols_set;//匹配成功的列
        if (filtered_distanceMatrix.size()>0)
        {
            Std2dVectorAdapter<BoxDistanceType>std2matrix;
            Matrix<BoxDistanceType> matrix = std2matrix.convertToMatrix(filtered_distanceMatrix);
            Munkres<BoxDistanceType> m;
            m.solve(matrix);
            std2matrix.convertFromMatrix(match_result_Matrix, matrix);

            //匹配之后需要找出匹配成功的对
            for (int row = 0; row<match_result_Matrix.size(); ++row)
            {
                for (int col = 0; col<match_result_Matrix[0].size(); ++col)
                {
                    if (isequal(match_result_Matrix[row][col], 0.0))//结果矩阵中=0的代表匹配成功
                    {
                        rows_set.insert(choose_rows[row]);
                        cols_set.insert(choose_cols[col]);
                        pairs.push_back({ choose_rows[row],choose_cols[col] });
                        break;
                    }
                }
            }
        }

        //对于匈牙利匹配成功的跟踪目标,需要更新其坐标,以及提升其置信度
        for (auto &iter : pairs)
        {
            tracking_things[iter.second].track_frame++;//跟踪帧数增加
            tracking_things[iter.second].confidenceIncrease();
            tracking_things[iter.second].box=detected_rects[iter.first];//用检测结果来更新跟踪结果
        }
        //对于距离太远的跟踪目标,需要降低其置信度,置信度低于一定值,将其从后往前删除
        for (int i = tracking_things.size() - 1; i >= 0; --i)
        {
            if (cols_set.find(i) != cols_set.end())//如果匈牙利匹配成功
                continue;
            else if (!tracking_things[i].confidenceDecrease())//置信度减到0以下,剔除该目标
            {
                idTabelDelete(tracking_things[i].id);
                tracking_things.erase(tracking_things.begin()+i);
            }
            else//置信度没减到0以下,算作跟踪成功，帧数+1
            {
                tracking_things[i].track_frame++;//跟踪帧数增加
            }
        }
        
        //对于距离太远的检测目标,将其作为新目标,加入到跟踪列表中
        for (int i = 0; i<detected_rects.size(); i++)
        {
            if (rows_set.find(i) == rows_set.end())//没有匈牙利匹配成功的目标
            {
                tracking_things.push_back(Thing(idCreator(), detected_rects[i]));
            }
        }
    }
}
