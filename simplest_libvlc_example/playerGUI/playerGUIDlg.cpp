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

#include "stdafx.h"
#include "playerGUI.h"
#include "playerGUIDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CplayerGUIDlg �Ի���




CplayerGUIDlg::CplayerGUIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CplayerGUIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CplayerGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CURTIME, m_curtime);
	DDX_Control(pDX, IDC_DURATION, m_duration);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_URL, m_url);
	//  DDX_Control(pDX, IDC_VOLUME, m_volume);
}

BEGIN_MESSAGE_MAP(CplayerGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_START, &CplayerGUIDlg::OnBnClickedStart)
	ON_BN_CLICKED(ID_PAUSE, &CplayerGUIDlg::OnBnClickedPause)
	ON_BN_CLICKED(ID_STOP, &CplayerGUIDlg::OnBnClickedStop)
	ON_BN_CLICKED(ID_ABOUT, &CplayerGUIDlg::OnBnClickedAbout)
	ON_BN_CLICKED(IDC_FILEBROWSE, &CplayerGUIDlg::OnBnClickedFilebrowse)
	ON_WM_DROPFILES()
	ON_WM_DESTROY()
	ON_COMMAND(ID_WEBSITE, &CplayerGUIDlg::OnWebsite)
	ON_WM_TIMER()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(ID_INFO, &CplayerGUIDlg::OnBnClickedInfo)
END_MESSAGE_MAP()


// CplayerGUIDlg ��Ϣ�������

BOOL CplayerGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_progress.SetRange(0,100);
	SystemClear();
	/* Load the VLC engine */
	libvlc_inst = libvlc_new (0, NULL);


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CplayerGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CplayerGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CplayerGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CplayerGUIDlg::OnBnClickedStart()
{
	CStringW cstr_url;

#ifdef _UNICODE
	m_url.GetWindowText(cstr_url);
#else
	USES_CONVERSION;
	CStringA cstr_urla;
	m_url.GetWindowText(cstr_urla);
	cstr_url.Format(L"%s",A2W(cstr_urla));
#endif

	std::string str_url;
	UNICODE_to_UTF8(cstr_url, str_url);
	const char *char_url=str_url.c_str();

	if(strcmp(char_url,"")==0){
		AfxMessageBox(_T("Input URL is NULL!"));
		return;
	}
	
	HWND screen_hwnd=NULL;
	screen_hwnd = this->GetDlgItem(IDC_SCREEN)->m_hWnd; 

	if(playerstate!=STATE_PREPARE){
		AfxMessageBox(_T("Media is playing now."));
		return;
	}


     /* Create a new item */
     //m = libvlc_media_new_location (libvlc_inst, "http://mycool.movie.com/test.mov");

     libvlc_m = libvlc_media_new_path (libvlc_inst, char_url);

     /* Create a media player playing environement */
     libvlc_mp = libvlc_media_player_new_from_media (libvlc_m);
     
     /* No need to keep the media now */
     libvlc_media_release (libvlc_m);
 
    //on windows
     libvlc_media_player_set_hwnd (libvlc_mp,screen_hwnd);
 
     /* play the media_player */
     int x=libvlc_media_player_play (libvlc_mp);
    
     //_sleep (30000); /* Let it play a bit */
    

	playerstate=STATE_PLAY;
	SetBtn(STATE_PLAY);
	SetTimer(1,1000,NULL);
}


void CplayerGUIDlg::OnBnClickedPause()
{
	if(playerstate==STATE_PLAY){
		libvlc_media_player_set_pause(libvlc_mp,1);
		playerstate=STATE_PAUSE;
		GetDlgItem(ID_PAUSE)->SetWindowText(_T("Resume"));
	}else if(playerstate==STATE_PAUSE){
		libvlc_media_player_set_pause(libvlc_mp,0);
		playerstate=STATE_PLAY;
		GetDlgItem(ID_PAUSE)->SetWindowText(_T("Pause"));
	}
}


void CplayerGUIDlg::OnBnClickedStop()
{
	if(libvlc_mp!=NULL){
		libvlc_media_player_stop (libvlc_mp);
		libvlc_media_player_release (libvlc_mp);
		KillTimer(1);
	}
	
	SystemClear();
}



void CplayerGUIDlg::OnBnClickedAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}


void CplayerGUIDlg::OnBnClickedFilebrowse()
{
	CString FilePathName;
	//Filter String
	CString strfilter;
	strfilter.Append(_T("Common media formats|*.avi;*.wmv;*.wmp;*.wm;*.asf;*.rm;*.ram;*.rmvb;*.ra;*.mpg;*.mpeg;*.mpe;*.m1v;*.m2v;*.mpv2;"));
	strfilter.Append(_T("*.mp2v;*.dat;*.mp4;*.m4v;*.m4p;*.vob;*.ac3;*.dts;*.mov;*.qt;*.mr;*.3gp;*.3gpp;*.3g2;*.3gp2;*.swf;*.ogg;*.wma;*.wav;"));
	strfilter.Append(_T("*.mid;*.midi;*.mpa;*.mp2;*.mp3;*.m1a;*.m2a;*.m4a;*.aac;*.mkv;*.ogm;*.m4b;*.tp;*.ts;*.tpr;*.pva;*.pss;*.wv;*.m2ts;*.evo;"));
	strfilter.Append(_T("*.rpm;*.realpix;*.rt;*.smi;*.smil;*.scm;*.aif;*.aiff;*.aifc;*.amr;*.amv;*.au;*.acc;*.dsa;*.dsm;*.dsv;*.dss;*.pmp;*.smk;*.flic|"));
	strfilter.Append(_T("Windows Media Video(*.avi;*wmv;*wmp;*wm;*asf)|*.avi;*.wmv;*.wmp;*.wm;*.asf|"));
	strfilter.Append(_T("Windows Media Audio(*.wma;*wav;*aif;*aifc;*aiff;*mid;*midi;*rmi)|*.wma;*.wav;*.aif;*.aifc;*.aiff;*.mid;*.midi;*.rmi|"));
	strfilter.Append(_T("Real(*.rm;*ram;*rmvb;*rpm;*ra;*rt;*rp;*smi;*smil;*.scm)|*.rm;*.ram;*.rmvb;*.rpm;*.ra;*.rt;*.rp;*.smi;*.smil;*.scm|"));
	strfilter.Append(_T("MPEG Video(*.mpg;*mpeg;*mpe;*m1v;*m2v;*mpv2;*mp2v;*dat;*mp4;*m4v;*m4p;*m4b;*ts;*tp;*tpr;*pva;*pss;*.wv;)|"));
	strfilter.Append(_T("*.mpg;*.mpeg;*.mpe;*.m1v;*.m2v;*.mpv2;*.mp2v;*.dat;*.mp4;*.m4v;*.m4p;*.m4b;*.ts;*.tp;*.tpr;*.pva;*.pss;*.wv;|"));
	strfilter.Append(_T("MPEG Audio(*.mpa;*mp2;*m1a;*m2a;*m4a;*aac;*.m2ts;*.evo)|*.mpa;*.mp2;*.m1a;*.m2a;*.m4a;*.aac;*.m2ts;*.evo|"));
	strfilter.Append(_T("DVD(*.vob;*ifo;*ac3;*dts)|*.vob;*.ifo;*.ac3;*.dts|MP3(*.mp3)|*.mp3|CD Tracks(*.cda)|*.cda|"));
	strfilter.Append(_T("Quicktime(*.mov;*qt;*mr;*3gp;*3gpp;*3g2;*3gp2)|*.mov;*.qt;*.mr;*.3gp;*.3gpp;*.3g2;*.3gp2|"));
	strfilter.Append(_T("Flash Files(*.flv;*swf;*.f4v)|*.flv;*.swf;*.f4v|Playlist(*.smpl;*.asx;*m3u;*pls;*wvx;*wax;*wmx;*mpcpl)|*.smpl;*.asx;*.m3u;*.pls;*.wvx;*.wax;*.wmx;*.mpcpl|"));
	strfilter.Append(_T("Others(*.ivf;*au;*snd;*ogm;*ogg;*fli;*flc;*flic;*d2v;*mkv;*pmp;*mka;*smk;*bik;*ratdvd;*roq;*drc;*dsm;*dsv;*dsa;*dss;*mpc;*divx;*vp6;*.ape;*.flac;*.tta;*.csf)"));
	strfilter.Append(_T("|*.ivf;*.au;*.snd;*.ogm;*.ogg;*.fli;*.flc;*.flic;*.d2v;*.mkv;*.pmp;*.mka;*.smk;*.bik;*.ratdvd;*.roq;*.drc;*.dsm;*.dsv;*.dsa;*.dss;*.mpc;*.divx;*.vp6;*.ape;*.amr;*.flac;*.tta;*.csf|"));
	strfilter.Append(_T("All Files(*.*)|*.*||"));

	LPCTSTR lpszfilter=strfilter;
	CFileDialog dlg(TRUE,NULL,NULL,NULL,lpszfilter);///TRUEΪOPEN�Ի���FALSEΪSAVE AS�Ի��� 
	if(dlg.DoModal()==IDOK) {
		FilePathName=dlg.GetPathName();
		m_url.SetWindowText(FilePathName);
	}
}


void CplayerGUIDlg::SystemClear()
{
	SetBtn(STATE_PREPARE);
	playerstate=STATE_PREPARE;
	m_curtime.SetWindowText(_T("00:00:00"));
	m_duration.SetWindowText(_T("00:00:00"));
	libvlc_mp=NULL;
	libvlc_m=NULL;
	m_progress.SetPos(0);
}

void CplayerGUIDlg::SetBtn(PlayerState state){
	if(state==STATE_PREPARE){
		GetDlgItem(ID_START)->EnableWindow(TRUE);
		GetDlgItem(ID_PAUSE)->EnableWindow(FALSE);
		GetDlgItem(ID_STOP)->EnableWindow(FALSE);
		GetDlgItem(ID_INFO)->EnableWindow(FALSE);
	}else{
		GetDlgItem(ID_START)->EnableWindow(FALSE);
		GetDlgItem(ID_PAUSE)->EnableWindow(TRUE);
		GetDlgItem(ID_STOP)->EnableWindow(TRUE);
		GetDlgItem(ID_INFO)->EnableWindow(TRUE);
	}
}

void CplayerGUIDlg::UNICODE_to_UTF8(CStringW& unicodeString, std::string& str)
{
	int stringLength = ::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), NULL, 0, NULL, NULL);

	char* buffer = new char[stringLength + 1];
	::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), buffer, stringLength, NULL, NULL);
	buffer[stringLength] = '\0';

	str = buffer;

	delete[] buffer;
}

void CplayerGUIDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnDropFiles(hDropInfo);

	LPTSTR pFilePathName =(LPTSTR)malloc(MAX_URL_LENGTH);
	::DragQueryFile(hDropInfo, 0, pFilePathName,MAX_URL_LENGTH);  // ��ȡ�Ϸ��ļ��������ļ�������ؼ���
	m_url.SetWindowText(pFilePathName);
	::DragFinish(hDropInfo);   // ע����������٣��������ͷ�Windows Ϊ�����ļ��ϷŶ�������ڴ�
	free(pFilePathName);

}


void CplayerGUIDlg::OnDestroy()
{
	/* Free the media_player */
	libvlc_release (libvlc_inst);

	CDialogEx::OnDestroy();
}


void CplayerGUIDlg::OnWebsite()
{
	ShellExecuteA( NULL,"open","http://blog.csdn.net/leixiaohua1020",NULL,NULL,SW_SHOWNORMAL);
}


void CplayerGUIDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1){
		CString curtimestr,durationstr;
		int curtime;
		int duration;
		int tns, thh, tmm, tss;
		int progress;
		//ms
		curtime = libvlc_media_player_get_time(libvlc_mp);
		if(curtime!=0){
			//change to second
			tns = curtime/1000;
			thh  = tns / 3600;
			tmm  = (tns % 3600) / 60;
			tss  = (tns % 60);
			curtimestr.Format(_T("%02d:%02d:%02d"),thh,tmm,tss);
			m_curtime.SetWindowText(curtimestr);
		}
		duration  = libvlc_media_player_get_length(libvlc_mp);
		if(duration!=0){
			tns = duration/1000;
			thh  = tns / 3600;
			tmm  = (tns % 3600) / 60;
			tss  = (tns % 60);
			durationstr.Format(_T("%02d:%02d:%02d"),thh,tmm,tss);
			m_duration.SetWindowText(durationstr);

			progress=curtime*100/duration;
			m_progress.SetPos(progress);
		}
	}

	//Stop in the end
	if(libvlc_media_player_get_state(libvlc_mp)==libvlc_Ended)
		OnBnClickedStop();


	CDialogEx::OnTimer(nIDEvent);
}


void CplayerGUIDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetSafeHwnd() == m_progress.GetSafeHwnd()){
		float posf=0.0;
		if(nSBCode==SB_THUMBPOSITION){
			posf=(float)nPos/100.0;
			libvlc_media_player_set_position(libvlc_mp,posf);
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CplayerGUIDlg::OnBnClickedInfo()
{
	CString infostr;
	infostr.AppendFormat(_T("========Video Information========\r\n"));
	unsigned video_w=0,video_h=0;
	float framerate=0;
	libvlc_video_get_size(libvlc_mp,0,&video_w,&video_h);
	framerate=libvlc_media_player_get_fps(libvlc_mp);
	infostr.AppendFormat(_T("Video Width:%d\r\nVideo Height:%d\r\nVideo Framerate:%f\r\n")
		,video_w,video_h,framerate);

	infostr.AppendFormat(_T("========Audio Information========\r\n"));
	int channel=0;
	channel=libvlc_audio_get_channel(libvlc_mp);
	infostr.AppendFormat(_T("Audio Channels:%d\r\n"),channel);

	AfxMessageBox(infostr,MB_ICONINFORMATION);
}


