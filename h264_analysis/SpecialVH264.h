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
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSpecialVH264App:
// �йش����ʵ�֣������ SpecialVH264.cpp
//

class CSpecialVH264App : public CWinApp
{
public:
	CSpecialVH264App();

// ��д
public:
	virtual BOOL InitInstance();
	//������������
	void LoadLaguage();
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSpecialVH264App theApp;