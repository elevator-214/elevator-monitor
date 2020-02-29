#include"ThingDetect.h"
#include"kd_tree.h"
namespace hlg
{

/*����Ϊ�������Ĵ���*/
bool MyCompare_x(const cv::Point point1, const cv::Point point2) { return point1.x < point2.x; }
bool MyCompare_y(const cv::Point point1, const cv::Point point2) { return point1.y < point2.y; }
float cal_points_distance(const vector<cv::Point>points1_small, const vector<cv::Point>points2_big)
{
	//���������㼯֮�����С���� ʹ��voronoiͼ��kd���ļ��ɣ�ʹ��ʱ�临�Ӷ��½�
	float min_distance = std::numeric_limits<float>::max();
	return min_distance;
}
MyObejctBox::MyObejctBox(const std::vector<cv::Point>&points)
{
	this->x1 = (*std::min_element(points.begin(), points.end(), MyCompare_x)).x;
	this->x3 = (*std::max_element(points.begin(), points.end(), MyCompare_x)).x;
	this->y1 = (*std::min_element(points.begin(), points.end(), MyCompare_y)).y;
	this->y3 = (*std::max_element(points.begin(), points.end(), MyCompare_y)).y;
	this->rect = Rect(this->x1, this->y1, (this->x3) - (this->x1) + 1, (this->y3) - (this->y1) + 1);
}
void MyObejctBox::Update_Box(const std::vector<cv::Point>&points)
{
	/*��С����ͨ�������´����ͨ��*/
	int new_x1 = (*std::min_element(points.begin(), points.end(), MyCompare_x)).x;
	int new_x3 = (*std::max_element(points.begin(), points.end(), MyCompare_x)).x;
	int new_y1 = (*std::min_element(points.begin(), points.end(), MyCompare_y)).y;
	int new_y3 = (*std::max_element(points.begin(), points.end(), MyCompare_y)).y;
	if (new_x1 < this->x1) {
		this->x1 = new_x1;
		this->rect = Rect(this->x1, this->y1, (this->x3) - (this->x1) + 1, (this->y3) - (this->y1) + 1);
	}
	if (new_x3 > this->x3) {
		this->x3 = new_x3;
		this->rect = Rect(this->x1, this->y1, (this->x3) - (this->x1) + 1, (this->y3) - (this->y1) + 1);
	}
	if (new_y1 < this->y1) {
		this->y1 = new_y1;
		this->rect = Rect(this->x1, this->y1, (this->x3) - (this->x1) + 1, (this->y3) - (this->y1) + 1);
	}
	if (new_y3 > this->y3) {
		this->y3 = new_y3;
		this->rect = Rect(this->x1, this->y1, (this->x3) - (this->x1) + 1, (this->y3) - (this->y1) + 1);
	}
}
ThingInterface::ThingDetector::ThingDetector(const float& big_area_threshold, const float& small_area_threshold, const float& distance_threshold, const float& big_area_distance,bool show_flag)
{
	this->big_area_threshold = big_area_threshold;
	this->small_area_threshold = small_area_threshold;
	this->distance_threshold = distance_threshold;
    this->big_area_distance = big_area_distance;
    this->show_flag = show_flag;
}
void ThingInterface::ThingDetector::ThingsDetect(const Mat& ForemaskImage)
{
    
	//���ȳ�ʼ��MyBigObejcts
	this->big_obejcts.clear();
	//clock_t start_time;
	//start_time = clock();
	//��ֵ�������Ǵ���ɫ��244~255��������������Ϊ0 ��Ӱ��ȥ��
	Mat final_foremask;
	cv::threshold(ForemaskImage, final_foremask, 244, 255, cv::THRESH_BINARY);

	// ͨ����̬ѧ�����Զ�ֵ����ͼ�����Ԥ����
	Mat kernel = getStructuringElement(MORPH_ERODE, Size(3, 3));
	cv::morphologyEx(final_foremask, final_foremask, cv::MORPH_CLOSE, kernel, Point(-1, -1), 5);

	//Ѱ������
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(final_foremask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	//����������� �����ĵ����������������� 
	vector<int>small_contour_ids;	//С�������±�
	size_t big_thing_contourpoint_num = 0;
	for (int i = 0; i < contours.size(); i++)
	{
		const vector<Point>& contour = contours[i];
		double area = contourArea(Mat(contour));
		if (area > this->big_area_threshold)//������
		{
			big_obejcts.push_back(MyBigObejct(contour));
			big_thing_contourpoint_num += contour.size();
		}
		else if (area>this->small_area_threshold)//С����
		{
			small_contour_ids.push_back(i);
		}
	}
	/*�ȶ�������������кϲ�*/
	//�ȸ��������������������������ǰ��
	sort(big_obejcts.begin(), big_obejcts.end(),
		[](const MyBigObejct &A, const MyBigObejct &B) {return A.contour_points.size() > B.contour_points.size(); });
	if (big_obejcts.size() >= 2)
	{
        //cout << "start:" << endl;
		for (int i = big_obejcts.size() - 1; i >= 0; --i)
		{
            //cout << "i:"<<i << endl;
			for (int j = i-1; j >= 0; --j)
			{
                //cout << "j:" << j << endl;
                int x_1= max(big_obejcts[i].box.x1, big_obejcts[j].box.x1);//��x_2������x�����޽���

                int x_2 = min(big_obejcts[i].box.x3, big_obejcts[j].box.x3);
				int y_1 = max(big_obejcts[i].box.y1, big_obejcts[j].box.y1);//��y_2������y�����޽���
				int y_2 = min(big_obejcts[i].box.y3, big_obejcts[j].box.y3);
				if ((x_1 - x_2) < int(this->big_area_distance) && (y_1 - y_2) < int(this->big_area_distance))
				{//������Ӿ�����ıȽϽ�������������������kd����ѯ�������ѯ�ľ������Ƚ�С���򽫴��������кϲ�
					const vector<Point> &contour = big_obejcts[j].contour_points;
					vector<Contour_Point>contour_points(contour.size());
					int point_count = 0;
					for (const auto& point : contour)
					{
						contour_points[point_count++] = Contour_Point(double(point.x), double(point.y), 0);//����id�޹ؽ�Ҫ��id����0
					}
					kdt::KDTree<Contour_Point> kdtree(contour_points);//����kd��
					Contour_Point query;
					double min_distance = std::numeric_limits<double>::max();
					for (const auto&point : big_obejcts[i].contour_points)//��ѯ
					{
						query.set_coord(point.x, point.y);
						double distance;
						const int Point_ID = kdtree.nnSearch(query, &distance);
						if (distance < min_distance)
						{
							min_distance = distance;
						}
					}
					if (min_distance < this->big_area_distance) {//kd����ѯ�ľ������ȽϽ�����Ҫ��������������кϲ�
						//cout << "min_distance:" << min_distance << endl;
						big_obejcts[j].contour_points.reserve(big_obejcts[j].contour_points.size()+ big_obejcts[i].contour_points.size());//�����һ���㼯����ǰ��
						big_obejcts[j].contour_points.insert(big_obejcts[j].contour_points.end(), big_obejcts[i].contour_points.begin(), big_obejcts[i].contour_points.end());
						big_obejcts[j].box.rect |= big_obejcts[i].box.rect;//���¾��ο�
						big_obejcts[j].box.Update_coordinates();
						big_obejcts.erase(big_obejcts.begin() + i);//ɾ�����һ���㼯
						sort(big_obejcts.begin(), big_obejcts.end(),
							[](const MyBigObejct &A, const MyBigObejct &B) {return A.contour_points.size() > B.contour_points.size(); });//���¸��ݵ㼯��Ŀ���򣬴ﵽ���ټ�������Ч����emmm�����ɻ��������kd����ʱ�临�Ӷ�
                        break;
                    }
				}		
			}
		}
	}

	//���С������鵽�����������������
	if (small_contour_ids.size() > 0 && big_obejcts.size() > 0)
	{
		/*���ݴ��������������kd��*/
		//���Ƚ�������������ŵ�������
		vector<Contour_Point>BigTH_contour_points(big_thing_contourpoint_num);
		{
			int i = 0;
			for (int j=0;j<big_obejcts.size();++j)
			{
				const vector<Point> &contour = big_obejcts[j].contour_points;
				for (const auto& point : contour)
				{
					BigTH_contour_points[i++] = Contour_Point(double(point.x), double(point.y), j);//iter.second :boxe_id
				}
			}
		}

		kdt::KDTree<Contour_Point> kdtree(BigTH_contour_points);//����kd��

																//��С�������������KD����ѯ����С���������ϲ���������С�Ĵ�����
		Contour_Point query;
		for (const auto&i : small_contour_ids)
		{
			const vector<Point>& small_contour = contours[i];
			double min_distance = std::numeric_limits<double>::max();
			int final_BoxID;
			for (const auto&point : small_contour)
			{
				query.set_coord(point.x, point.y);
				double distance;
				const int Point_ID = kdtree.nnSearch(query, &distance);
				if (distance < min_distance)
				{
					min_distance = distance;
					final_BoxID = BigTH_contour_points[Point_ID].box_id;
				}
			}
			if (min_distance < this->distance_threshold) {
				big_obejcts[final_BoxID].box.Update_Box(small_contour);
			}

		}
		BigTH_contour_points.clear();
	}
	//cout << "before filter:" << this->big_obejcts.size() << endl;
	this->ThingBox_Filter();//����һЩ�Զ���Ĺ����糤���ȣ������ȵȣ��Լ�������ǰ�������һЩ�ϲ���ɾ��
	//cout << "after filter:" << this->big_obejcts.size() << endl;
	/*for (const auto &box : this->thing_boxes)
		cout << "box area:" << box.rect.area() << endl;*/
    if (show_flag)
    {
        //�������Լ������廭����
        RNG g_rng(12345);
        Mat dst = Mat::zeros(ForemaskImage.size(), CV_8UC3);
        for (int i = 0; i< contours.size(); i++)
        {
            //cout << "��������: " << contours[i].size() << endl;
            Scalar color = Scalar(g_rng.uniform(0, 255), g_rng.uniform(0, 255), g_rng.uniform(0, 255));//����ֵ
            const vector<Point>& c = contours[i];
            const double area = contourArea(Mat(c));
            if (area>this->small_area_threshold)
                drawContours(dst, contours, i, color, cv::FILLED, 8, hierarchy);
        }
        for (int i = 0; i < big_obejcts.size(); ++i)
        {
            const int x1 = big_obejcts[i].box.x1;
            const int x3 = big_obejcts[i].box.x3;
            const int y1 = big_obejcts[i].box.y1;
            const int y3 = big_obejcts[i].box.y3;
            cv::rectangle(dst, Point(x1, y1),
                Point(x3, y3),
                cv::Scalar(255, 255, 255));
        }
        namedWindow("kd-tree_couters", 0);
        imshow("kd-tree_couters", dst);
    }
	
	//cout << "spend time:" << double(clock() - start_time) / CLOCKS_PER_SEC << endl;
	
}
void ThingInterface::ThingDetector::ThingBox_Filter()
{
	//����һЩ�Զ���Ĺ����糤���ȣ������ȵȣ��Լ�������ǰ�������һЩ�ϲ���ɾ��
	//���Ƚ����ΰ�������Ӵ�С��������
	sort(this->big_obejcts.begin(), this->big_obejcts.end(),
		[](const MyBigObejct &A, const MyBigObejct &B) {return A.box.rect.area() > B.box.rect.area(); });
	if (this->big_obejcts.size() >= 2)//������������2�����ο����ȸ��ݽ������Σ���С������� ��ֵ���ϲ������С�ľ���
	{
		for (int i = this->big_obejcts.size() - 1; i >= 0; --i)
		{
			double max_inter_small_rate = 0;
			int big_id = i - 1;
			for (int j = i - 1; j >= 0; --j){
				double inter_small_rate = double((this->big_obejcts[i].box.rect& this->big_obejcts[j].box.rect).area())
					/ double(big_obejcts[i].box.rect.area());
				if (inter_small_rate > max_inter_small_rate){
					max_inter_small_rate = inter_small_rate;
					big_id = j;
				}		
			}
			if (max_inter_small_rate > intersection_small_rate){//�����С�ľ��ν��кϲ�
				this->big_obejcts[big_id].box.rect |= this->big_obejcts[i].box.rect;
				this->big_obejcts[big_id].box.Update_coordinates();
				this->big_obejcts.erase(big_obejcts.begin() + i);//ɾ��������С�ļ���
			}		
		}
	}
	//ɾ����������ֵļ���emmmm
	for (int i = this->big_obejcts.size() - 1; i >= 0; --i){
		const double aspect_ratio = double(this->big_obejcts[i].box.rect.width) / double(this->big_obejcts[i].box.rect.height);
		//cout << "aspect_ratio:" << aspect_ratio << endl;
		if (aspect_ratio > aspect_ratio_threshold || aspect_ratio < (1.0 / aspect_ratio_threshold)){
			this->big_obejcts.erase(big_obejcts.begin() + i);
		}
	}
}
void ThingInterface::ThingDetector::SetOutputCoordScale(const double OriginImage_Height, const double OriginImage_Width, Size Current_Size)
{
    static bool Execute_Once_Flag = true;
    if(Execute_Once_Flag)
    ScaleFactor_Height = OriginImage_Height / double(Current_Size.height);
    ScaleFactor_Width = OriginImage_Width / double(Current_Size.width);
}

void ThingInterface::ThingDetector::Get_Thing_Result(vector<Rect>&things_boxes, const Rect& thROI)
{
    things_boxes.clear();
    things_boxes.reserve(this->big_obejcts.size());
    //�߶�ת��
    for (int i = 0; i < this->big_obejcts.size(); ++i)
    {
        const int &x1 = big_obejcts[i].box.x1;
        const int &y1 = big_obejcts[i].box.y1;
        const int &width = big_obejcts[i].box.rect.width;
        const int &height = big_obejcts[i].box.rect.height;
        Rect rect(x1* ScaleFactor_Width, y1*ScaleFactor_Height, width* ScaleFactor_Width, height*ScaleFactor_Height);
        things_boxes.push_back(rect);//�߶�ת��
    }



    //�����趨ROI����һ�����ˣ�ȥ���غ�����Ƚ�С�ģ�����roi����ļ����
    const double area_ratio_threshold = 0.8;//���ROI�ڵ�������������������ֵ��80%������
    for (int i = things_boxes.size() - 1; i >= 0; --i)
    {
        const auto & detected_rect = things_boxes[i];
        const Rect Inter_rect = detected_rect&thROI;
        const double Inter_area = Inter_rect.area()*1.0 ;
        const double aspect_ratio = double(Inter_rect.width) / double(Inter_rect.height);
        if ((Inter_area < area_ratio_threshold*this->big_area_threshold)||( aspect_ratio > this->aspect_ratio_threshold) ||( aspect_ratio < (1.0 / this->aspect_ratio_threshold)))//ɾ��ROI�����ռ�ȱȽ�С���Լ������Ⱥ���ֵ�
        {
            things_boxes.erase(things_boxes.begin() + i);
        }
    }
}
}

