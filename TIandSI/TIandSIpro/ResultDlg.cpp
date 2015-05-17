// ResultDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "TIandSI.h"
#include "ResultDlg.h"
#include "afxdialogex.h"


// ResultDlg �Ի���

IMPLEMENT_DYNAMIC(ResultDlg, CDialogEx)

ResultDlg::ResultDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(ResultDlg::IDD, pParent)
{

	//  m_result = _T("");
}

ResultDlg::~ResultDlg()
{
}

void ResultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_RESULT, m_result);
	//  DDX_Text(pDX, IDC_RESULT, m_result);
	//  DDX_Control(pDX, IDC_RESULT, m_result);
	DDX_Control(pDX, IDC_RESULT_SI, m_resultsi);
	DDX_Control(pDX, IDC_RESULT_TI, m_resultti);
}


BEGIN_MESSAGE_MAP(ResultDlg, CDialogEx)
END_MESSAGE_MAP()


// ResultDlg ��Ϣ�������
BOOL ResultDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

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