/* 
 * H.264 ������
 * H.264 Analysis
 *
 * ������ Lei Xiaohua
 * leixiaohua1020@126.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 * 
 * H.264������������
 * H.264 Stream Analysis Tools
 *
 */
#include "stdafx.h"
#include "SpecialVH264Dlg.h"

char* probe_nal_unit(char* filename,int data_offset,int data_lenth);
int h264_analyze(int argc, char *argv[]);