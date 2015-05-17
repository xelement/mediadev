/**
 * TIandSI
 *
 * ������ Lei Xiaohua
 * leixiaohua1020@126.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * ���������ڼ�����Ƶ��ʱ����Ϣ��TI��Ҳ��Ϊʱ�临�Ӷȣ����Լ��ռ�
 * ��Ϣ��SI��Ҳ��Ϊ�ռ临�Ӷȣ�����������ָ��ȡ����ITU-R BT.1788��׼��
 * 
 * This software is used to calculate raw video's TI (Temporal perceptual 
 * Information) and SI (Spatial perceptual Information) defined in 
 * ITU-R BT.1788.
 */

#include "stdafx.h"
#include "TIandSI.h"
#include "ResultDlg.h"
#include "afxdialogex.h"


// ResultDlg �Ի���

IMPLEMENT_DYNAMIC(ResultDlg, CDialogEx)

ResultDlg::ResultDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ResultDlg::IDD, pParent)
{

}

ResultDlg::~ResultDlg()
{
}

void ResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RESULT_SI, m_resultsi);
	DDX_Control(pDX, IDC_RESULT_TI, m_resultti);
}


BEGIN_MESSAGE_MAP(ResultDlg, CDialogEx)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// ResultDlg ��Ϣ�������
void ResultDlg::SystemClear()
{
	resulttistr.Format(L"");
	resultsistr.Format(L"");
	m_resultti.SetWindowText(L"");
	m_resultsi.SetWindowText(L"");
}

void ResultDlg::AppendTIStr(CString str)
{
	resulttistr.AppendFormat(L"%s\r\n",str);
	m_resultti.SetWindowText(resulttistr);
}

void ResultDlg::AppendSIStr(CString str)
{
	resultsistr.AppendFormat(L"%s\r\n",str);
	m_resultsi.SetWindowText(resultsistr);
}

void ResultDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	delete this;
}
