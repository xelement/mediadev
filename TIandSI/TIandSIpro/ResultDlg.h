#pragma once


// ResultDlg �Ի���

class ResultDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ResultDlg)

public:
	ResultDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ResultDlg();

// �Ի�������
	enum { IDD = IDD_RESULT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();


	DECLARE_MESSAGE_MAP()
public:
	void SystemClear();
	CString resulttistr;
	CString resultsistr;
	CEdit m_resultsi;
	CEdit m_resultti;

	void AppendTIStr(CString str);
	void AppendSIStr(CString str);
};
