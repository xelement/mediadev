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
#include "resource.h"
#include "stdafx.h"
#include "NALParse.h"
#include "h264_analyze.h"
#include <vector>
using std::vector;
// CSpecialVH264Dlg �Ի���
class CSpecialVH264Dlg : public CDialogEx
{
// ����
public:
	CSpecialVH264Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SPECIAL_V_H264 };

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
	CMFCEditBrowseCtrl m_vh264inputurl;
	CListCtrl m_vh264nallist;
	CButton m_vh264nallistmaxnum;
	afx_msg void OnBnClickedVH264InputurlOpen();
	void OnCustomdrawMyList ( NMHDR* pNMHDR, LRESULT* pResult );
	int nl_index;
	int AppendNLInfo(int nal_reference_idc,int nal_unit_type,int len,int data_lenth,int data_offset);
	char fileurl[MAX_URL_LENGTH];
	void SystemClear();
	afx_msg void OnBnClickedVH264About();
	CEdit m_vh264nalinfo;
	//һ��Packet��¼
	typedef struct NALInfo{
		int data_offset;
		int data_lenth;
	}NALInfo;
	vector<NALInfo> nl_infovector;
	//-----
	afx_msg void OnDropFiles(HDROP hDropInfo);

	CString text;
	CComboBox m_vh264lang;
	afx_msg void OnSelchangeVH264Lang();
	afx_msg void OnItemchangedVH264Nallist(NMHDR *pNMHDR, LRESULT *pResult);
};
