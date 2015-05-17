/**
 * ��򵥵Ļ���libVLC�Ĳ�����-ͼ�ν����
 * Simplest libVLC Player GUI
 *
 * ������ Lei Xiaohua
 * leixiaohua1020@126.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * ��������һ����򵥵Ļ���libVLC��ͼ�ν��沥������
 * �ʺϳ�ѧ��ѧϰlibVLC��
 *
 * This example is the simplest Player based on libVLC.
 * Suitable for the beginner of libVLC.
 */

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CplayerGUIApp:
// �йش����ʵ�֣������ playerGUI.cpp
//

class CplayerGUIApp : public CWinApp
{
public:
	CplayerGUIApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CplayerGUIApp theApp;