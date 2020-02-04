#include"read_para.h"



namespace hlg {
const string parameters_path = "./parameters.txt";


/*cam1_para*/
int cam1_CAMERA_NUM = 0;
int cam1_CAMERA_WIDTH = 1280;
int cam1_CAMERA_HEIGHT = 720;
string cam1_VideoPath = "/media/hlg/6AA80074A8004155/maskter_project/experiment/data/video/floor_board/clip_video/ch03_20170315060019_46.avi";
unsigned int cam1_yolo_width = 352;
unsigned int cam1_yolo_height = 224;
bool cam1_print_yolo_width_height_flag = true;
//ppROI
int cam1_ppROI_x = 105;
int cam1_ppROI_y = 30;
int cam1_ppROI_width = 350;
int cam1_ppROI_height = 330;
int cam1_ppFlowLine = 220;
//thROI
int cam1_thROI_x = 100;
int cam1_thROI_y = 10;
int cam1_thROI_width = 360;
int cam1_thROI_height = 340;


/*cam2_para*/
int cam2_CAMERA_WIDTH = 640;
int cam2_CAMERA_HEIGHT = 480;

int cam2_CAMERA_NUM = 1;
int cam2_fmMinue = 1200;
 //ppROI   
int cam2_ppROI_x = 260;
int cam2_ppROI_y = 10;
int cam2_ppROI_width = 240;
int cam2_ppROI_height = 460;

/*cam3_para*/
int cam3_CAMERA_WIDTH = 640;
int cam3_CAMERA_HEIGHT = 480;
int cam3_CAMERA_NUM = 2;
int cam3_fmMinue = 1200;
 //hdROI
int cam3_hdROI_x = 340;
int cam3_hdROI_y = 0;
int cam3_hdROI_width = 100;
int cam3_hdROI_height = 480;

/*cam4_para*/
int cam4_CAMERA_WIDTH = 640;
int cam4_CAMERA_HEIGHT = 480;
int cam4_CAMERA_NUM = 1;
int cam4_fmMinue = 1200;
//hdROI
int cam4_hdROI_x = 250;
int cam4_hdROI_y = 0;
int cam4_hdROI_width = 100;
int cam4_hdROI_height = 480;

/*ui mainwindow*/
bool ui_mainwindow_load_cam1_flag = false;
bool ui_mainwindow_load_cam2_flag = false;
bool ui_mainwindow_load_cam3_flag = false;
bool ui_mainwindow_load_cam4_flag = false;
bool ui_mainwindow_draw_trajectory_flag = false;
bool ui_mainwindow_drawSkeleton_flag = false;
bool ui_mainwindow_drawID_flag = false;
bool ui_mainwindow_drawBox_flag = false;
bool ui_mainwindow_cam1_start_flag = true;
bool ui_mainwindow_cam2_start_flag = true;
bool ui_mainwindow_cam3_start_flag = true;
bool ui_mainwindow_cam4_start_flag = true;



void read_txt_to_map(const string&para_path, unordered_map<string, string>&maps)
{
	std::ifstream file(para_path);
	if (!file)
	{
		std::cerr << "failed to open " << para_path << endl;
		return;
	}
	string line;
	while (getline(file, line)) // line中不包括每行的换行符
	{
		line.erase(0, line.find_first_not_of(' '));//首先去除首的空格
		line.erase(line.find_last_not_of(' ') + 1);//去除尾的空格
		if (line.size() == 0 || line[0] == '#' || line[0] == '/')
		{
			continue;//如果这一行被注释掉
		}
		else if (line.find_first_of('=') == std::string::npos)
		{
			continue;//如果这一行没有赋值语句
		}
		else
		{


			std::size_t found = line.find_first_of('/');
			std::size_t found_str = line.find_first_of('\"');
			if (found_str != std::string::npos&&found_str < found)
			{
				line.erase(found_str, 1);//去除双引号
				found_str = line.find_first_of('\"', found_str);
				line.erase(found_str, 1);//去除双引号
				found = line.find_first_of('/', found_str);
			}

			if (found != std::string::npos) {
				line.erase(found);//去除尾的注释
			}

			found = line.find_first_of('#');
			if (found != std::string::npos) {
				line.erase(found);//去除尾的注释
			}
			found = line.find_first_of('=');
			string key = line.substr(0, found);
			string value = line.substr(found + 1);

			key.erase(0, key.find_first_not_of(' '));//首先去除首的空格
			key.erase(key.find_last_not_of(' ') + 1);//去除尾的空格
			value.erase(0, value.find_first_not_of(' '));//首先去除首的空格
			value.erase(value.find_last_not_of(' ') + 1);//去除尾的空格
			maps.insert({ key,value });
		}
	}
}

void assign_value(string *s, const string& name, const unordered_map<string, string>&maps)
{
	*s = maps.at(name);
	cout << name << " is set to " << *s << endl;
}
template<typename T>
void assign_value(T* variable, const string& name, const unordered_map<string, string>&maps)
{
	if (maps.find(name) == maps.end())
	{
		cout << name << "没有找到该变量！" << endl;
		return;
	}
	if (typeid(T).name() == typeid(int).name())
	{
		*variable = stoi(maps.at(name));
	}
	else if (typeid(T).name() == typeid(unsigned int).name())
	{
		*variable = stoul(maps.at(name));
	}
	else if (typeid(T).name() == typeid(bool).name())
	{
		std::istringstream is(maps.at(name));
		is >> std::boolalpha >> *variable;
	}
	else if (typeid(T).name() == typeid(float).name())
	{
		*variable = stof(maps.at(name));
	}
	else if (typeid(T).name() == typeid(double).name())
	{
		*variable = stod(maps.at(name));
	}
	else
	{
		cout << name << "还没没有定义这个数据类型" << endl;
		return;
	}
	cout << name << " is set to " << *variable << endl;
}
void load_camera1(const unordered_map<string, string>&maps)//加载cam1参数
{
	cout << "************load cam1 paras!!************" << endl;
	assign_value(&cam1_yolo_width, "cam1_yolo_width", maps);
	assign_value(&cam1_yolo_height, "cam1_yolo_height", maps);
	assign_value(&cam1_print_yolo_width_height_flag, "cam1_print_yolo_width_height_flag", maps);
	assign_value(&cam1_ppROI_x, "cam1_ppROI_x", maps);
	assign_value(&cam1_ppROI_y, "cam1_ppROI_y", maps);
	assign_value(&cam1_ppROI_width, "cam1_ppROI_width", maps);
	assign_value(&cam1_ppROI_height, "cam1_ppROI_height", maps);
	assign_value(&cam1_ppFlowLine, "cam1_ppFlowLine", maps);
	assign_value(&cam1_thROI_x, "cam1_thROI_x", maps);
	assign_value(&cam1_thROI_y, "cam1_thROI_y", maps);
	assign_value(&cam1_thROI_width, "cam1_thROI_width", maps);
	assign_value(&cam1_thROI_height, "cam1_thROI_height", maps);
	assign_value(&cam1_CAMERA_NUM, "cam1_CAMERA_NUM", maps);
	assign_value(&cam1_CAMERA_WIDTH, "cam1_CAMERA_WIDTH", maps);
	assign_value(&cam1_CAMERA_HEIGHT, "cam1_CAMERA_HEIGHT", maps);
	assign_value(&cam1_VideoPath, "cam1_VideoPath", maps);

}
void load_camera2(const unordered_map<string, string>&maps)//加载cam2参数
{
	cout << "************load cam2 paras!!************" << endl;
	assign_value(&cam2_CAMERA_WIDTH, "cam2_CAMERA_WIDTH", maps);
	assign_value(&cam2_CAMERA_HEIGHT, "cam2_CAMERA_HEIGHT", maps);
	assign_value(&cam2_CAMERA_NUM, "cam2_CAMERA_NUM", maps);
	assign_value(&cam2_fmMinue, "cam2_fmMinue", maps);

	assign_value(&cam2_ppROI_x, "cam2_ppROI_x", maps);
	assign_value(&cam2_ppROI_y, "cam2_ppROI_y", maps);
	assign_value(&cam2_ppROI_width, "cam2_ppROI_width", maps);
	assign_value(&cam2_ppROI_height, "cam2_ppROI_height", maps);
}

void load_camera3(const unordered_map<string, string>&maps)//加载cam3参数
{
	cout << "************load cam3 paras!!************" << endl;
	assign_value(&cam3_CAMERA_WIDTH, "cam3_CAMERA_WIDTH", maps);
	assign_value(&cam3_CAMERA_HEIGHT, "cam3_CAMERA_HEIGHT", maps);
	assign_value(&cam3_CAMERA_NUM, "cam3_CAMERA_NUM", maps);
	assign_value(&cam3_fmMinue, "cam3_fmMinue", maps);

	assign_value(&cam3_hdROI_x, "cam3_hdROI_x", maps);
	assign_value(&cam3_hdROI_y , "cam3_hdROI_y", maps);
	assign_value(&cam3_hdROI_width, "cam3_hdROI_width", maps);
	assign_value(&cam3_hdROI_height, "cam3_hdROI_height", maps);
}


void load_camera4(const unordered_map<string, string>&maps)//加载cam4参数
{
	cout << "************load cam4 paras!!************" << endl;
	assign_value(&cam4_CAMERA_WIDTH, "cam4_CAMERA_WIDTH", maps);
	assign_value(&cam4_CAMERA_HEIGHT, "cam4_CAMERA_HEIGHT", maps);
	assign_value(&cam4_CAMERA_NUM, "cam4_CAMERA_NUM", maps);
	assign_value(&cam4_fmMinue, "cam4_fmMinue", maps);

	assign_value(&cam4_hdROI_x, "cam4_hdROI_x", maps);
	assign_value(&cam4_hdROI_y, "cam4_hdROI_y", maps);
	assign_value(&cam4_hdROI_width, "cam4_hdROI_width", maps);
	assign_value(&cam4_hdROI_height, "cam4_hdROI_height", maps);

}


void load_ui(const unordered_map<string, string>&maps)//加载ui界面参数
{
	cout << "************ui mainwindows paras!!************" << endl;
	assign_value(&ui_mainwindow_load_cam1_flag, "ui_mainwindow_load_cam1_flag", maps);
	assign_value(&ui_mainwindow_load_cam2_flag, "ui_mainwindow_load_cam2_flag", maps);
	assign_value(&ui_mainwindow_load_cam3_flag, "ui_mainwindow_load_cam3_flag", maps);
	assign_value(&ui_mainwindow_load_cam4_flag, "ui_mainwindow_load_cam4_flag", maps);

	assign_value(&ui_mainwindow_draw_trajectory_flag, "ui_mainwindow_draw_trajectory_flag", maps);
	assign_value(&ui_mainwindow_drawSkeleton_flag, "ui_mainwindow_drawSkeleton_flag", maps);
	assign_value(&ui_mainwindow_drawID_flag, "ui_mainwindow_drawID_flag", maps);
	assign_value(&ui_mainwindow_drawBox_flag, "ui_mainwindow_drawBox_flag", maps);
	assign_value(&ui_mainwindow_cam1_start_flag, "ui_mainwindow_cam1_start_flag", maps);
	assign_value(&ui_mainwindow_cam2_start_flag, "ui_mainwindow_cam2_start_flag", maps);
	assign_value(&ui_mainwindow_cam3_start_flag, "ui_mainwindow_cam3_start_flag", maps);
	assign_value(&ui_mainwindow_cam4_start_flag, "ui_mainwindow_cam4_start_flag", maps);

}
void load_para_total()
{
	unordered_map<string, string>maps;
	read_txt_to_map(parameters_path, maps);

	load_camera1(maps);
	load_camera2(maps);
	load_camera3(maps);
	load_camera4(maps);
	load_ui(maps);
	//load_ui();
}

}
