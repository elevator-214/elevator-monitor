#pragma once
#include"myFunction.h"
#include"ThingInterface.h"
namespace hlg
{

/*下面为大件物品检测及跟踪的各种类*/
class Contour_Point : public std::array<double, 2>//轮廓点
{
public:
	// dimension of space (or "k" of k-d tree)
	// KDTree class accesses this member
	static const int DIM = 2;
	// the constructors
	Contour_Point() {}
	Contour_Point(double x, double y)
	{
		(*this)[0] = x;
		(*this)[1] = y;
	}
	Contour_Point(double x, double y, int box_id)
	{
		(*this)[0] = x;
		(*this)[1] = y;
		this->box_id = box_id;
	}
	void set_coord(double x, double y)
	{
		(*this)[0] = x;
		(*this)[1] = y;
	}
	int box_id;//对应大件物品thing_boxes的id号
			   // 转到 OpenCV Point2d
	operator cv::Point2d() const { return cv::Point2d((*this)[0], (*this)[1]); }
};
class MyObejctBox
{
	/*@大件物体边框类*/
public:
	MyObejctBox(const std::vector<cv::Point>&points);
	MyObejctBox() {};
	~MyObejctBox() {};
	int x1;//外接矩形左上角
	int y1;
	int x3;//外接矩形右下角
	int y3;
	cv::Rect rect;
	void Update_Box(const std::vector<cv::Point>&points);
	void Update_coordinates(){
		this->x1 = this->rect.x;
		this->y1 = this->rect.y;
		this->x3 = this->rect.x+ this->rect.width-1;
		this->y3 = this->rect.y + this->rect.height - 1;
	}
};
class MyBigObejct
{
	/*@大件物体边框类*/
public:
	vector<Point>contour_points;
	MyObejctBox box;
	MyBigObejct(const std::vector<cv::Point>&points)
	{ 
		this->box = MyObejctBox(points);
		this->contour_points = points;
	};
	~MyBigObejct() {};
	
};
class ThingInterface::ThingDetector
{
private:
	float big_area_threshold;//大物体面积阈值，大于这个阈值的被判断为大件物体
	float big_area_distance;//大物体距离阈值，将距离小于这个阈值的大物体进行合并
	float small_area_threshold;//小物体面积阈值，小于这个阈值的当作噪声滤除
	float distance_threshold;//距离阈值，如果小轮廓距离大轮廓的最小距离小于这个值，则将小物体归到大物体中
	const double intersection_small_rate = 0.4;//最后对大物体合并的阈值
	const double aspect_ratio_threshold = 4.0;//将不符合长宽比要求的检测框剔除
    bool show_flag;//true则显示处理结果
    double ScaleFactor_Width;
    double ScaleFactor_Height;
    std::vector<MyBigObejct>big_obejcts;
    void ThingBox_Filter();//根据一些自定义的规则，如长宽比，交并比等，对检测出来的前景框进行一些合并及删除
    void Filter_People(vector<Rect>&things_boxes, const vector<Rect>&people_boxes);//根据iou把人头滤除掉
public:
    ThingDetector(const float &big_area_threshold = 3000.0, const float& small_area_threshold = 1000.0, const float& distance_threshold = 70.0, const float& big_area_distance = 30.0,bool show_flag=false);
	~ThingDetector() {}
	void ThingsDetect(const Mat &ForemaskImage);//根据前景图来把物体检测出来
    void SetOutputCoordScale(double OriginImage_Height, double OriginImage_Width,Size Current_Size);
    void Get_Thing_Result(vector<Rect>&things_boxes, const vector<Rect>&people_boxes,  const Rect& thROI);//坐标转换，同时根据iou把人头滤除掉,并根据thROI进行滤除
};

}