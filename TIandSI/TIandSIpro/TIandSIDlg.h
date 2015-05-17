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
//ʹ��STL
#include <vector>
using std::vector;
#include <math.h>
#include "ResultDlg.h"

extern "C"
{
#include "sdl/SDL.h"
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
};


typedef struct SDLParam{
	SDL_Surface *screen; 
	SDL_Overlay *bmp; 
	SDL_Rect rect;
	bool graphically;
}SDLParam;

typedef enum Systemstate{
	SYSTEM_PREPARE,
	SYSTEM_PAUSE,
	SYSTEM_PROCESS
};

// CTIandSIDlg �Ի���
class CTIandSIDlg : public CDialogEx
{
// ����
public:
	CTIandSIDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TIANDSI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedUrllistAdd();
	afx_msg void OnBnClickedUrllistDelete();
	CListBox m_urllist;
	CMFCEditBrowseCtrl m_inputurl;

	vector<CString> urllist;
	void RefreshUrllist();
	afx_msg void OnBnClickedUrllistDeleteall();

	int TIandSI(CString urllist,LPVOID dlg);
	int TIandSICal(LPVOID lparam,char* ydata,char* prev_ydata,int width,int height,SDLParam sdlparam,float &ti,float &si);

//	CButton m_outputsi;
//	CButton m_outputti;
	afx_msg void OnBnClickedAbout();
	CWinThread *pThreadProcess;
	CProgressCtrl m_progressall;
	CEdit m_progressalltext;
	//SDL
	SDLParam sdlparam;
	CProgressCtrl m_progresscur;
	CEdit m_progresscurtext;
//	afx_msg void OnClickedRadioDraw();
	//
	ResultDlg *resultdlg;
	//
	void SystemClear();
	afx_msg void OnBnClickedResult();
	CButton m_output;

	void SetState(Systemstate state);
	Systemstate sysstate;

	CComboBox m_draw;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	CEdit m_curval;
	afx_msg void OnWebsite();
	afx_msg void OnUrllistQuickadd();

	afx_msg void OnCancel();
};
