#pragma once
#include<string>
#include <sstream>
#include<fstream>
#include<iostream>
#include<vector>
#include <iterator>
#include<unordered_map>
using namespace std;

namespace hlg
{
	/*cam1参数*/
	extern int cam1_CAMERA_NUM;
	extern int cam1_CAMERA_WIDTH;
	extern int cam1_CAMERA_HEIGHT;
	extern string cam1_VideoPath ;
	extern unsigned int cam1_yolo_width ;
	extern unsigned int cam1_yolo_height ;
	extern bool cam1_print_yolo_width_height_flag;
	//ppROI
	extern int cam1_ppROI_x ;
	extern int cam1_ppROI_y ;
	extern int cam1_ppROI_width ;
	extern int cam1_ppROI_height ;
	extern int cam1_ppFlowLine ;
	 //thROI
	extern int cam1_thROI_x ;
	extern int cam1_thROI_y ;
	extern int cam1_thROI_width ;
	extern int cam1_thROI_height ;

	/*cam2参数*/
	extern int cam2_CAMERA_WIDTH;
	extern int cam2_CAMERA_HEIGHT ;

	extern int cam2_CAMERA_NUM;
	extern int cam2_fmMinue;
	//ppROI   
	extern int cam2_ppROI_x ;
	extern int cam2_ppROI_y;
	extern int cam2_ppROI_width;
	extern int cam2_ppROI_height;

	/*cam3参数*/
	extern int cam3_CAMERA_WIDTH;
	extern int cam3_CAMERA_HEIGHT ;
	extern int cam3_CAMERA_NUM ;
			   
	extern int cam3_fmMinue ;
	
	 //hdROI
	extern int cam3_hdROI_x ;
	extern int cam3_hdROI_y ;
	extern int cam3_hdROI_width ;
	extern int cam3_hdROI_height ;

	/*cam4参数*/
	extern int cam4_CAMERA_WIDTH;
	extern int cam4_CAMERA_HEIGHT;
	extern int cam4_CAMERA_NUM;
	extern int cam4_fmMinue;
	//hdROI
	extern int cam4_hdROI_x;
	extern int cam4_hdROI_y;
	extern int cam4_hdROI_width;
	extern int cam4_hdROI_height;

	/*ui mainwindow*/
	extern bool ui_mainwindow_load_cam1_flag ;
	extern bool ui_mainwindow_load_cam2_flag ;
	extern bool ui_mainwindow_load_cam3_flag ;
	extern bool ui_mainwindow_load_cam4_flag ;
	extern bool ui_mainwindow_draw_trajectory_flag ;
	extern bool ui_mainwindow_drawSkeleton_flag ;
	extern bool ui_mainwindow_drawID_flag ;
	extern bool ui_mainwindow_drawBox_flag ;
	extern bool ui_mainwindow_cam1_start_flag ;
	extern bool ui_mainwindow_cam2_start_flag ;
	extern bool ui_mainwindow_cam3_start_flag;
	extern bool ui_mainwindow_cam4_start_flag ;

	void load_camera1(const unordered_map<string, string>&maps);
	void load_camera2(const unordered_map<string, string>&maps);
	void load_camera3(const unordered_map<string, string>&maps);
	void load_camera4(const unordered_map<string, string>&maps);
	void load_ui(const unordered_map<string, string>&maps);
	void load_para_total();
}




