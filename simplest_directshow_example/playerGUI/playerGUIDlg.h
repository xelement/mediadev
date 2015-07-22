/**
 * ��򵥵Ļ���DirectShow�Ĳ�����-ͼ�ν����
 * Simplest DirectShow Player GUI
 *
 * ������ Lei Xiaohua
 * leixiaohua1020@126.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * ��������һ����򵥵Ļ���DirectShow��ͼ�ν��沥������
 * �ʺϳ�ѧ��ѧϰDirectShow��
 *
 * This example is the simplest Player based on DirectShow.
 * And it has a simple Graphical User Interface.
 * Suitable for the beginner of DirectShow.
 */

#pragma once
#include <dshow.h>

#define MAX_URL_LENGTH 500

enum PlayerState{
	STATE_PREPARE,
	STATE_PLAY,
	STATE_PAUSE
};

// Filter graph notification to the specified window
#define WM_GRAPHNOTIFY  (WM_USER+20)

// CplayerGUIDlg �Ի���
class CplayerGUIDlg : public CDialogEx
{
// ����
public:
	CplayerGUIDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PLAYERGUI_DIALOG };

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
	IGraphBuilder *pGraph;
	IMediaControl *pControl;
	IMediaEventEx *pEvent; 
	IBasicVideo *pVideo;
	IBasicAudio *pAudio;
	IVideoWindow *pWindow;
	IMediaSeeking *pSeeking;

	PlayerState playerstate;

	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedAbout();
	CEdit m_curtime;
	CEdit m_duration;
	CSliderCtrl m_progress;
	CEdit m_url;

	void SystemClear();
	void SetBtn(PlayerState state);

	afx_msg void OnBnClickedFilebrowse();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnWebsite();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedFullscreen();
	afx_msg void OnBnClickedInfo();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	LRESULT OnGraphNotify(WPARAM inWParam, LPARAM inLParam);
};
