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
#include "AddDlg.h"
#include "afxdialogex.h"


// AddDlg �Ի���

IMPLEMENT_DYNAMIC(AddDlg, CDialogEx)

AddDlg::AddDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(AddDlg::IDD, pParent)
{

	//  m_inputyuvwidth = _T("");
	//  m_inputyuvheight = _T("");
	m_inputyuvheight = 0;
	m_inputyuvwidth = 0;
}

AddDlg::~AddDlg()
{
}

void AddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INPUT_YUV_PIXFMT, m_inputyuvpixfmt);
	DDX_Control(pDX, IDC_YUVURLS, m_yuvurls);
	DDX_Text(pDX, IDC_INPUT_YUV_HEIGHT, m_inputyuvheight);
	DDX_Text(pDX, IDC_INPUT_YUV_WIDTH, m_inputyuvwidth);
}


BEGIN_MESSAGE_MAP(AddDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHOOSEFILE, &AddDlg::OnBnClickedChoosefile)
	ON_BN_CLICKED(IDC_YUVLIST_ADD, &AddDlg::OnBnClickedYuvlistAdd)
END_MESSAGE_MAP()


// AddDlg ��Ϣ�������


BOOL AddDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_inputyuvpixfmt.InsertString(0,L"YUV420P");
	m_inputyuvpixfmt.InsertString(1,L"YUV422P");
	m_inputyuvpixfmt.InsertString(2,L"YUV444P");
	m_inputyuvpixfmt.InsertString(3,L"Y");
	m_inputyuvpixfmt.SetCurSel(0);

	m_inputyuvwidth=1920;
	m_inputyuvheight=1080;
	UpdateData(FALSE);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}



void AddDlg::OnBnClickedChoosefile()
{
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT);
	//����ļ����ַ���������,���Ļ�������С
	TCHAR szLargeBuf[8192]={0}; 
	dlg.m_ofn.lpstrFile =szLargeBuf;
	dlg.m_ofn.nMaxFile=8192;

	if(dlg.DoModal()==IDOK) 
	{ 
		POSITION pos = dlg.GetStartPosition(); 
		while(pos) 
		{ 
			CString szFileName = dlg.GetNextPathName(pos); 
			urls.push_back(szFileName); 
		} 
		CString urlsstr;
		//ˢ��
		for(int i=0;i<urls.size();i++){
			urlsstr.AppendFormat(L"%s\r\n",urls[i]);
		}
		m_yuvurls.SetWindowText(urlsstr);
	} 
}


void AddDlg::OnBnClickedYuvlistAdd()
{
	UpdateData(TRUE);
	for(int i=0;i<urls.size();i++){
		Pixfmt pixfmt=(Pixfmt)m_inputyuvpixfmt.GetCurSel();
		YUVInfo yuvinfo={urls[i],m_inputyuvwidth,m_inputyuvheight,pixfmt};
		maindlg->yuvlistvector.push_back(yuvinfo);
	}
	maindlg->RefreshYuvlist();
	CDialogEx::OnOK();
}
