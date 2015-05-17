/**
 * TIandSIpro
 * (TIandSI Professional)
 *
 * ������ Lei Xiaohua
 * leixiaohua1020@126.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 * 
 * �����̿��Լ���һ��ѹ����Ƶ��ʱ����ϢTI��Temporal perceptual Information��
 * Ҳ���Գ�ʱ�临�Ӷȣ��Ϳռ���ϢSI��Spatial perceptual Information��Ҳ����
 * �ƿռ临�Ӷȣ������㷽�����Ա�׼��ITU-R BT.1788
 *
 * Professional: ֧��ѹ���������������������ݱ���YUV��RGB����Ϊ����
 *
 * This software can calculate a video bitstream's TI(Temporal perceptual Information) 
 * and SI(Spatial perceptual Information) according to ITU-R BT.1788.
 *
 * Professional: Support bitstreams (not raw data such as YUV, RGB, etc.) as Input.
 *
 */

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CTIandSIApp:
// �йش����ʵ�֣������ TIandSI.cpp
//

class CTIandSIApp : public CWinApp
{
public:
	CTIandSIApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CTIandSIApp theApp;