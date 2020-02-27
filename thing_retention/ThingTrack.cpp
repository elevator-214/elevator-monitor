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

    void ThingInterface::ThingTracker::track(const vector<Rect>&Thing_Detected )
    {
        vector<Rect>detected_rects = Thing_Detected;
        

        //����������
        std::vector<cv::Rect>tracking_boxes;
        for (int i = 0; i < tracking_things.size(); ++i)
            tracking_boxes.push_back(tracking_things[i].box);
        const int rows = detected_rects.size();
        const int cols = tracking_boxes.size();
        vector<vector<BoxDistanceType>>Distance_Matrix(rows, vector<BoxDistanceType>(cols));
        vector<vector<BoxDistanceType>>Distance_Matrix_reverse(cols, vector<BoxDistanceType>(rows));
        calculate_Distance_matrix(detected_rects, tracking_boxes, Distance_Matrix, Distance_Matrix_reverse);

        //���ȸ��ݾ������Ծ��ο�����˲�
        vector<int>choose_rows, choose_cols;
        const BoxDistanceType distance_threshold = 0.80;//���볬��0.99�Ľ������⿼�ǣ�����ڼ��������������壬�ڸ��ٶ�����Ҫ�޳�
        for (int i = 0; i<rows; i++)
            choose_rows.push_back(i);
        for (int i = 0; i<cols; i++)
            choose_cols.push_back(i);
        Distance_matrix_Filter(Distance_Matrix, Distance_Matrix_reverse, distance_threshold,
            choose_rows, choose_cols);
        //�����˲��Ľ�����õ��˲�֮��Ĵ�����������ƥ��ľ���
        vector<vector<BoxDistanceType>>filtered_distanceMatrix;
        filtered_distanceMatrix = get_filtered_Matrix(Distance_Matrix, choose_rows, choose_cols);

        //������ƥ��
        vector<vector<BoxDistanceType>>match_result_Matrix;
        vector<pair<int, int>>pairs;//ƥ��ɹ��Ķ�
        set<int>rows_set;//ƥ��ɹ�����
        set<int>cols_set;//ƥ��ɹ�����
        if (filtered_distanceMatrix.size()>0)
        {
            Std2dVectorAdapter<BoxDistanceType>std2matrix;
            Matrix<BoxDistanceType> matrix = std2matrix.convertToMatrix(filtered_distanceMatrix);
            Munkres<BoxDistanceType> m;
            m.solve(matrix);
            std2matrix.convertFromMatrix(match_result_Matrix, matrix);

            //ƥ��֮����Ҫ�ҳ�ƥ��ɹ��Ķ�
            for (int row = 0; row<match_result_Matrix.size(); ++row)
            {
                for (int col = 0; col<match_result_Matrix[0].size(); ++col)
                {
                    if (isequal(match_result_Matrix[row][col], 0.0))//���������=0�Ĵ���ƥ��ɹ�
                    {
                        rows_set.insert(choose_rows[row]);
                        cols_set.insert(choose_cols[col]);
                        pairs.push_back({ choose_rows[row],choose_cols[col] });
                        break;
                    }
                }
            }
        }

        //����������ƥ��ɹ��ĸ���Ŀ��,��Ҫ����������,�Լ����������Ŷ�
        for (auto &iter : pairs)
        {
            tracking_things[iter.second].track_frame++;//����֡������
            tracking_things[iter.second].confidenceIncrease();
            tracking_things[iter.second].box=detected_rects[iter.first];//�ü���������¸��ٽ��
        }
        //���ھ���̫Զ�ĸ���Ŀ��,��Ҫ���������Ŷ�,���Ŷȵ���һ��ֵ,����Ӻ���ǰɾ��
        for (int i = tracking_things.size() - 1; i >= 0; --i)
        {
            if (cols_set.find(i) != cols_set.end())//���������ƥ��ɹ�
                continue;
            else if (!tracking_things[i].confidenceDecrease())//���Ŷȼ���0����,�޳���Ŀ��
            {
                idTabelDelete(tracking_things[i].id);
                tracking_things.erase(tracking_things.begin()+i);
            }
            else//���Ŷ�û����0����,�������ٳɹ���֡��+1
            {
                tracking_things[i].track_frame++;//����֡������
            }
        }
        
        //���ھ���̫Զ�ļ��Ŀ��,������Ϊ��Ŀ��,���뵽�����б���
        for (int i = 0; i<detected_rects.size(); i++)
        {
            if (rows_set.find(i) == rows_set.end())//û��������ƥ��ɹ���Ŀ��
            {
                tracking_things.push_back(Thing(idCreator(), detected_rects[i]));
            }
        }
    }
}