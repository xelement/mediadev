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

#include "stdafx.h"
#include "TIandSI.h"
#include "TIandSIDlg.h"
#include "afxdialogex.h"
#include <atlconv.h>

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
public:
//	afx_msg void OnDropFiles(HDROP hDropInfo);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CTIandSIDlg �Ի���




CTIandSIDlg::CTIandSIDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTIandSIDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTIandSIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_URLLIST, m_urllist);
	DDX_Control(pDX, IDC_INPUTURL, m_inputurl);
	DDX_Control(pDX, IDC_PROGRESS_ALL, m_progressall);
	DDX_Control(pDX, IDC_PROGRESS_ALL_TEXT, m_progressalltext);
	DDX_Control(pDX, IDC_PROGRESS_CUR, m_progresscur);
	DDX_Control(pDX, IDC_PROGRESS_CUR_TEXT, m_progresscurtext);
	DDX_Control(pDX, IDC_OUTPUT, m_output);
	DDX_Control(pDX, IDC_DRAW, m_draw);
	DDX_Control(pDX, IDC_CURVAL, m_curval);
}

BEGIN_MESSAGE_MAP(CTIandSIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTIandSIDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_URLLIST_ADD, &CTIandSIDlg::OnBnClickedUrllistAdd)
	ON_BN_CLICKED(IDC_URLLIST_DELETE, &CTIandSIDlg::OnBnClickedUrllistDelete)
	ON_BN_CLICKED(IDC_URLLIST_DELETEALL, &CTIandSIDlg::OnBnClickedUrllistDeleteall)
	ON_BN_CLICKED(IDABOUT, &CTIandSIDlg::OnBnClickedAbout)
//	ON_BN_CLICKED(IDC_CHART_EDITOR, &CTIandSIDlg::OnBnClickedChartEditor)
//ON_BN_CLICKED(IDC_RADIO_DRAW, &CTIandSIDlg::OnClickedRadioDraw)
ON_BN_CLICKED(IDRESULT, &CTIandSIDlg::OnBnClickedResult)
ON_WM_DROPFILES()
ON_BN_CLICKED(IDPAUSE, &CTIandSIDlg::OnBnClickedPause)
ON_BN_CLICKED(IDSTOP, &CTIandSIDlg::OnBnClickedStop)
ON_COMMAND(IDWEBSITE, &CTIandSIDlg::OnWebsite)
ON_COMMAND(ID_URLLIST_QUICKADD, &CTIandSIDlg::OnUrllistQuickadd)
END_MESSAGE_MAP()


// CTIandSIDlg ��Ϣ�������

BOOL CTIandSIDlg::OnInitDialog()
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
	//---------------
	
	m_output.SetCheck(TRUE);

	UpdateData(FALSE);
	//��ʼ��
	m_progressalltext.SetWindowText(L"0%");
	m_progresscurtext.SetWindowText(L"0%");
	m_progressall.SetRange(0,100);
	m_progresscur.SetRange(0,100);

	//SDL==========================
	sdlparam.graphically==true;
	//SDL_putenv()����ǰ��
	char variable[256];   
	CWnd* pWnd = GetDlgItem(IDC_SCREEN);  //��ȡͼƬ�ؼ��Ĵ���ָ��   
	sprintf(variable,"SDL_WINDOWID=0x%1x",pWnd->GetSafeHwnd()); // ��ʽ���ַ���      
	SDL_putenv(variable); 

	if(SDL_Init(SDL_INIT_VIDEO)) {
		AfxMessageBox(L"Could not initialize SDL"); 
		return 0;
	} 


	CRect screenrect;
	GetDlgItem(IDC_SCREEN)->GetWindowRect(screenrect);
	sdlparam.screen = SDL_SetVideoMode(screenrect.Width(), screenrect.Height(), 0, 0);
	if(!sdlparam.screen) {  
		AfxMessageBox(L"SDL: could not set video mode");  
		return 0;
	}
	sdlparam.rect.x=0;
	sdlparam.rect.y=0;
	sdlparam.rect.w=screenrect.Width();
	sdlparam.rect.h=screenrect.Height();

	//-------------
	resultdlg=new ResultDlg;
	resultdlg->Create(IDD_RESULT_DIALOG);

	m_draw.InsertString(0,L"SI");
	m_draw.InsertString(1,L"TI");
	m_draw.InsertString(2,L"����ʾ");
	m_draw.SetCurSel(0);

	SetState(SYSTEM_PREPARE);
	//=============================

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CTIandSIDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTIandSIDlg::OnPaint()
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
HCURSOR CTIandSIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//������Ƶ���Ҽ���
int CTIandSIDlg::TIandSI(CString url,LPVOID lparam)
{
	CTIandSIDlg *dlg=(CTIandSIDlg *)lparam;;
	//----------------
	AVFormatContext	*pFormatCtx;
	int				i, video_stream,audio_stream;
	AVCodecContext	*pCodecCtx,*pCodecCtx_au;
	AVCodec			*pCodec,*pCodec_au;
	char in_url[500]={0};
	char out_url[500]={0};
	USES_CONVERSION;
	sprintf(in_url,"%s",W2A(url));
	char *suffix=(char *)strrchr(in_url, '.');//
	*suffix='\0';
	strcpy(out_url,in_url);
	*suffix='.';
	sprintf(out_url,"%s_tisi.csv",out_url);


	av_register_all();
	pFormatCtx = avformat_alloc_context();
	if(avformat_open_input(&pFormatCtx,in_url,NULL,NULL)!=0){
		AfxMessageBox(L"Couldn't open file.\n");
		return FALSE;
	}
	if(av_find_stream_info(pFormatCtx)<0)
	{
		AfxMessageBox(L"Couldn't find stream information.\n");
		return FALSE;
	}
	video_stream=-1;
	audio_stream=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++) {
		if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_VIDEO){
			video_stream=i;
		}if(pFormatCtx->streams[i]->codec->codec_type==AVMEDIA_TYPE_AUDIO){
			audio_stream=i;
		}
	}
	if(video_stream==-1)
	{
		AfxMessageBox(L"Didn't find a video stream.\n");
		return FALSE;
	}
	if(video_stream!=-1){

		pCodecCtx=pFormatCtx->streams[video_stream]->codec;
		pCodec=avcodec_find_decoder(pCodecCtx->codec_id);
		if(pCodec==NULL)
		{
			AfxMessageBox(L"Codec not found.\n");
			return FALSE;
		}
		if(avcodec_open2(pCodecCtx, pCodec,NULL)<0)
		{
			AfxMessageBox(L"Could not open codec.\n");
			return FALSE;
		}

		//vtype=VE_TIL_SIL;
		std::vector<float> silist;
		std::vector<float> tilist;

		AVFrame	*pFrame,*pFrameYUV;
		pFrame=avcodec_alloc_frame();
		pFrameYUV=avcodec_alloc_frame();
		uint8_t *out_buffer;
		out_buffer=(uint8_t *)av_malloc(avpicture_get_size(PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height));
		avpicture_fill((AVPicture *)pFrameYUV, out_buffer, PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height);

		int ret, got_picture;
		static struct SwsContext *img_convert_ctx;
		int y_size = pCodecCtx->width * pCodecCtx->height;

		AVPacket *packet=(AVPacket *)av_malloc(sizeof(AVPacket));
		av_new_packet(packet, y_size);
		//����TI��ʱ��ʹ��
		int prev_has=0;
		uint8_t *prev_ydata=(uint8_t *)av_malloc(pCodecCtx->width*pCodecCtx->height);

		img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, PIX_FMT_YUV420P, SWS_BILINEAR, NULL, NULL, NULL); 
		//������ļ�
		FILE *fp=NULL;
		if(dlg->m_output.GetCheck()==TRUE){
			fp=fopen(out_url,"wb+");
			fprintf(fp,"TI,SI\n");
		}

		//Draw
		dlg->sdlparam.bmp = SDL_CreateYUVOverlay(pCodecCtx->width, pCodecCtx->height,SDL_YV12_OVERLAY, dlg->sdlparam.screen); 
		//Bar
		dlg->m_progresscur.SetPos(0);
		dlg->m_progresscurtext.SetWindowText(L" 0%");
		//�Ǹ���
		int framecnt=0;
		//΢��
		int ptime=0;
		while(av_read_frame(pFormatCtx, packet)>=0)
		{
			if(packet->stream_index==video_stream)
			{
				ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
				if(ret < 0)
				{
					printf("Decode Error.\n");
					return -1;
				}
				if(got_picture)
				{
					sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
					//��ǰ���֡�����ܼ���TI
					if(prev_has==1){
						float ti=0,si=0;
						int retval=TIandSICal(dlg,(char *)pFrameYUV->data[0],(char *)prev_ydata,pCodecCtx->width,pCodecCtx->height,sdlparam,ti,si);
						if(retval==-1){
							return -1;
						}
						CString curvalstr;
						curvalstr.Format(L"SI:%.3f TI:%.3f",si,ti);
						dlg->m_curval.SetWindowText(curvalstr);

						tilist.push_back(ti);
						silist.push_back(si);
						TRACE("%f,%f\n",ti,si);
						if(dlg->m_output.GetCheck()==TRUE)
							fprintf(fp,"%f,%f\n",ti,si);
						framecnt++;
						//Bar
						ptime=((float)pFrame->pkt_pts)*((float)pFormatCtx->streams[video_stream]->time_base.num)/((float)pFormatCtx->streams[video_stream]->time_base.den);
						int progress=(ptime*100)/(pFormatCtx->duration/1000000);
						if(progress<0||progress>100){
							progress=0;
						}
						dlg->m_progresscur.SetPos(progress);
						CString progressstr;
						progressstr.Format(L"%2d%%",progress);
						dlg->m_progresscurtext.SetWindowText(progressstr);
					}else{
						prev_has=1;
					}
					//������������
					memcpy(prev_ydata,pFrameYUV->data[0],pCodecCtx->width*pCodecCtx->height);
				}
			}
			av_free_packet(packet);
		}
		sws_freeContext(img_convert_ctx);


		//Free
		SDL_FreeYUVOverlay(dlg->sdlparam.bmp);
		//Bar
		dlg->m_progresscur.SetPos(100);
		dlg->m_progresscurtext.SetWindowText(L"100%");

		//�ļ���
		CString in_url1,filename;
		in_url1.Format(L"%s",A2W(in_url));
		int nPos=in_url1.ReverseFind('\\');   
		filename=in_url1.Mid(nPos+1);

		CString resulttistr(filename);
		CString resultsistr(filename);
		resulttistr.AppendFormat(L"\r\n=======\r\n");
		resultsistr.AppendFormat(L"\r\n=======\r\n");

		//����ƽ��ֵ
		int num=0;
		float sum=0;
		num=silist.size();
		for (int i=0;i<num;i++){
			sum +=silist[i];
			resultsistr.AppendFormat(L"%.3f\r\n",silist[i]);
		}
		float siavg=sum/num;
		num=0;
		sum=0;
		num=tilist.size();

		for (int i=0;i<num;i++){
			sum +=tilist[i];
			resulttistr.AppendFormat(L"%.3f\r\n",tilist[i]);
		}
		float tiavg=sum/num;

		if(dlg->m_output.GetCheck()==TRUE){
			fprintf(fp,"TI_AVG,SI_AVG\n");
			fprintf(fp,"%f,%f\n",tiavg,siavg);
		}
		
		resultsistr.AppendFormat(L"SI_AVG\r\n%.3f\r\n=======\r\n",siavg);
		resulttistr.AppendFormat(L"TI_AVG\r\n%.3f\r\n=======\r\n",tiavg);

		dlg->resultdlg->AppendTIStr(resulttistr);
		dlg->resultdlg->AppendSIStr(resultsistr);

		fcloseall();

		av_free(out_buffer);
		av_free(pFrameYUV);
		avcodec_close(pCodecCtx);
	}
	avformat_close_input(&pFormatCtx);

	return 0;
}


//����TI��SI
int CTIandSIDlg::TIandSICal(LPVOID lparam,char* ydata,char* prev_ydata,int width,int height,SDLParam sdlparam,float &ti,float &si)
{
	CTIandSIDlg *dlg=(CTIandSIDlg *)lparam;;
	

	int nHeight, nWidth;
	int nFrameNum;
	int nYSize;	
	int nFrameSize;

	unsigned char *pFrame;
	unsigned char **pFrame2D;
	unsigned char *pNextFrame;
	unsigned char **pNextFrame2D;
	unsigned char *NewYBuffer;
	unsigned char *NewUVBuffer;

	float *NewSIBuffer;
	float *NewTIBuffer;
	float flSI_A, flSI_S, flTI_A, flTI_S;

	int i, j, k, m, n;
	//Check State-----
	while(dlg->sysstate==SYSTEM_PAUSE){
		Sleep(1000);
	}
	if(dlg->sysstate==SYSTEM_PREPARE){
		return -1;
	}
	/****************��ʼ������**********************/
	nHeight = height;
	nWidth = width;
	nYSize = nHeight*nWidth;

	i = 0;
	j = 0;
	k = 0;
	m = n = 0;
	flSI_A = flSI_S = flTI_A = flTI_S = 0;

	pFrame = new unsigned char [nYSize];
	pFrame2D = new unsigned char *[nHeight];
	for(i = 0; i < nHeight; i++)
		pFrame2D[i] = pFrame + i*nWidth;

	pNextFrame = new unsigned char [nYSize];
	pNextFrame2D = new unsigned char *[nHeight];
	for(i = 0; i < nHeight; i++)
		pNextFrame2D[i] = pNextFrame + i*nWidth;

	NewYBuffer = new unsigned char [nYSize];

	int nUVSize=nYSize/2;
	
	//UV����ͼ��
	NewUVBuffer= new unsigned char[nUVSize];
	memset(NewUVBuffer, 0x80, nUVSize);


	NewSIBuffer = new float [nYSize];
	NewTIBuffer = new float [nYSize];

	/********************����SI***********************/
	memcpy(pFrame,ydata,nYSize);

	for(j = 0; j < nHeight; j++)
		for(i = 0; i < nWidth; i++)
		{
			float XSobel, YSobel, SobelValue;

			if((j == 0) && (i == 0))//(0, 0)
			{
				XSobel = -3*pFrame2D[0][0] + 3*pFrame2D[0][i+1]				
				-pFrame2D[j+1][0] + pFrame2D[j+1][i+1];

				YSobel = 3*pFrame2D[0][0] + pFrame2D[0][i+1]
				-3*pFrame2D[j+1][0] - pFrame2D[j+1][i+1];
			}
			else if((j == 0) && (i == (nWidth - 1)))//(nWidth - 1, 0)
			{
				XSobel = -3*pFrame2D[0][i-1] + 3*pFrame2D[0][i]
				-pFrame2D[j+1][i-1] + pFrame2D[j+1][i];

				YSobel = pFrame2D[0][i-1] + 3*pFrame2D[0][i]
				-pFrame2D[j+1][i-1] - 3*pFrame2D[j+1][i];
			}
			else if((j == (nHeight - 1)) && (i == 0))//(0, nHeight - 1)
			{
				XSobel = -pFrame2D[j-1][0] + pFrame2D[j-1][i+1]
				-3*pFrame2D[j][0] + 3*pFrame2D[j][i+1];				

				YSobel = 3*pFrame2D[j-1][0] + pFrame2D[j-1][i+1]
				-3*pFrame2D[j][0] - pFrame2D[j][i+1];
			}
			else if((j == (nHeight - 1)) && (i == (nWidth - 1)))//(nWidth - 1, nHeight - 1)
			{
				XSobel = -pFrame2D[j-1][i-1] + pFrame2D[j-1][i]
				-3*pFrame2D[j][i-1] + 3*pFrame2D[j][i];				

				YSobel = pFrame2D[j-1][i-1] + 3*pFrame2D[j-1][i]
				-pFrame2D[j][i-1] - 3*pFrame2D[j][i];
			}
			else if(j == 0)//����һ��
			{
				XSobel = -3*pFrame2D[0][i-1] + 3*pFrame2D[0][i+1]
				-pFrame2D[j+1][i-1] + pFrame2D[j+1][i+1];

				YSobel = pFrame2D[0][i-1] + 2*pFrame2D[0][i] + pFrame2D[0][i+1]
				-pFrame2D[j+1][i-1] - 2*pFrame2D[j+1][i] - pFrame2D[j+1][i+1];
			}
			else if(j == (nHeight - 1))//����һ��
			{
				XSobel = -pFrame2D[j-1][i-1] + pFrame2D[j-1][i+1]
				-3*pFrame2D[j][i-1] + 3*pFrame2D[j][i+1];

				YSobel = pFrame2D[j-1][i-1] + 2*pFrame2D[j-1][i] + pFrame2D[j-1][i+1]
				-pFrame2D[j][i-1] - 2*pFrame2D[j][i] - pFrame2D[j][i+1];
			}
			else if(i == 0)//���һ��
			{
				XSobel = -pFrame2D[j-1][0] + pFrame2D[j-1][i+1]
				-2*pFrame2D[j][0] + 2*pFrame2D[j][i+1]				
				-pFrame2D[j+1][0] + pFrame2D[j+1][i+1];

				YSobel = 3*pFrame2D[j-1][0] + pFrame2D[j-1][i+1]
				-3*pFrame2D[j+1][0] - pFrame2D[j+1][i+1];
			}
			else if(i == (nWidth - 1))//����һ��
			{
				XSobel = -pFrame2D[j-1][i-1] + pFrame2D[j-1][i]
				-2*pFrame2D[j][i-1] + 2*pFrame2D[j][i]				
				-pFrame2D[j+1][i-1] + pFrame2D[j+1][i];

				YSobel = pFrame2D[j-1][i-1] + 3*pFrame2D[j-1][i]
				-pFrame2D[j+1][i-1] - 3*pFrame2D[j+1][i];
			}
			else
			{
				XSobel = -pFrame2D[j-1][i-1] + pFrame2D[j-1][i+1]
				-2*pFrame2D[j][i-1] + 2*pFrame2D[j][i+1]				
				-pFrame2D[j+1][i-1] + pFrame2D[j+1][i+1];

				YSobel = pFrame2D[j-1][i-1] + 2*pFrame2D[j-1][i] + pFrame2D[j-1][i+1]
				-pFrame2D[j+1][i-1] - 2*pFrame2D[j+1][i] - pFrame2D[j+1][i+1];
			}

			SobelValue = sqrt(pow(XSobel, 2) + pow(YSobel, 2));

			NewYBuffer[j*nWidth + i] = (unsigned char)SobelValue;
			NewSIBuffer[j*nWidth + i] = SobelValue;
		}

		for(m = 1; m < (nHeight - 1); m++)
			for(n = 1; n < (nWidth - 1); n++)
			{
				flSI_A = flSI_A + NewSIBuffer[m*nWidth + n];
			}
		flSI_A = flSI_A/((nWidth - 2)*(nHeight - 2));

		for(m = 1; m < (nHeight - 1); m++)
			for(n = 1; n < (nWidth - 1); n++)
			{
				flSI_S = flSI_S + pow((NewSIBuffer[m*nWidth+n]-flSI_A), 2);
			}
		flSI_S = sqrt(flSI_S/((nWidth - 2)*(nHeight - 2) - 1));
		//���SI
		//printf("SI(%d) : %.3f\n",k+1,flSI_S);
		si=flSI_S;
		//��ͼ
		if(dlg->m_draw.GetCurSel()==0){

			SDL_LockYUVOverlay(dlg->sdlparam.bmp);
			dlg->sdlparam.bmp->pixels[0]=NewYBuffer;
			dlg->sdlparam.bmp->pixels[2]=NewUVBuffer;
			dlg->sdlparam.bmp->pixels[1]=NewUVBuffer+nUVSize/2;     
			dlg->sdlparam.bmp->pitches[0]=nWidth;
			dlg->sdlparam.bmp->pitches[2]=nWidth/2;   
			dlg->sdlparam.bmp->pitches[1]=nWidth/2;
			SDL_UnlockYUVOverlay(dlg->sdlparam.bmp);  
			SDL_DisplayYUVOverlay(dlg->sdlparam.bmp, &dlg->sdlparam.rect); 
		}

		memcpy(pFrame,prev_ydata,nYSize);
		memcpy(pNextFrame,ydata,nYSize);

		for(j = 0; j < nHeight; j++)
			for(i = 0; i < nWidth; i++)
			{
				NewTIBuffer[j*nWidth + i] = pNextFrame2D[j][i] - pFrame2D[j][i];
				NewYBuffer[j*nWidth + i] = abs(pNextFrame2D[j][i] - pFrame2D[j][i]);

				flTI_A = flTI_A + NewTIBuffer[j*nWidth + i];
			}

		flTI_A = flTI_A/nYSize;

		for(j = 0; j < nHeight; j++)
			for(i = 0; i < nWidth; i++)
			{
				flTI_S = flTI_S + pow((NewTIBuffer[j*nWidth + i]-flTI_A), 2);
			}

		flTI_S = sqrt(flTI_S/(nYSize - 1));
		//���TI
		ti=flTI_S;

		//��ͼ
		if(dlg->m_draw.GetCurSel()==1){

			SDL_LockYUVOverlay(dlg->sdlparam.bmp);
			dlg->sdlparam.bmp->pixels[0]=NewYBuffer;
			dlg->sdlparam.bmp->pixels[2]=NewUVBuffer;
			dlg->sdlparam.bmp->pixels[1]=NewUVBuffer+nUVSize/2;     
			dlg->sdlparam.bmp->pitches[0]=nWidth;
			dlg->sdlparam.bmp->pitches[2]=nWidth/2;   
			dlg->sdlparam.bmp->pitches[1]=nWidth/2;
			SDL_UnlockYUVOverlay(dlg->sdlparam.bmp);  
			SDL_DisplayYUVOverlay(dlg->sdlparam.bmp, &dlg->sdlparam.rect); 
		}

		delete []pFrame;
		delete []pFrame2D;
		delete []pNextFrame;
		delete []pNextFrame2D;
		delete []NewYBuffer;
		delete []NewTIBuffer;
		delete []NewSIBuffer;

		delete []NewUVBuffer;

		return 0;
}



UINT Thread_Process(LPVOID lpParam){
	CTIandSIDlg *dlg=(CTIandSIDlg *)lpParam;

	dlg->m_progressall.SetPos(0);
	dlg->m_progressalltext.SetWindowText(L"0%");
	//����

	dlg->SetState(SYSTEM_PROCESS);

	int i,j;
	for(i=0;i<dlg->urllist.size();i++){
		dlg->m_urllist.SetCurSel(i);

		int progress=i/dlg->urllist.size();
		dlg->m_progressall.SetPos(progress);
		CString progresstext;
		progresstext.Format(L"%d%%",progress);
		dlg->m_progressalltext.SetWindowText(progresstext);
		//------------------------------------
		int retval=dlg->TIandSI(dlg->urllist[i],dlg);
		if(retval==-1){
			return -1;
		}
		
	}
	dlg->m_progressall.SetPos(100);
	dlg->m_progressalltext.SetWindowText(L"100%");

	dlg->SetState(SYSTEM_PREPARE);

	AfxMessageBox(L"������ϣ�");
	return 0;
}

void CTIandSIDlg::OnBnClickedOk()
{
	//���²���
	UpdateData(TRUE);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	if(urllist.size()<1){
		AfxMessageBox(L"�����б��������Ƶ���У�");
		return;
	}
	SystemClear();

	pThreadProcess=AfxBeginThread(Thread_Process,this);//�����߳�

}


void CTIandSIDlg::OnBnClickedUrllistAdd()
{
	UpdateData(TRUE);
	//��ȡ��ַ�����б�
	CString url;
	m_inputurl.GetWindowText(url);

	if(url.IsEmpty()==TRUE){
		AfxMessageBox(L"�����ַΪ�գ�");
		return;
	}

	urllist.push_back(url);

	RefreshUrllist();
}


void CTIandSIDlg::OnBnClickedUrllistDelete()
{
	//ɾ��Ԫ��
	int urlindex=m_urllist.GetCurSel();
	if(urlindex!=-1){
	urllist.erase(urllist.begin()+urlindex);
	}else{
		AfxMessageBox(L"û��ѡ���κ�Ԫ�أ�");
	}

	RefreshUrllist();
}


void CTIandSIDlg::OnBnClickedUrllistDeleteall()
{
	if(urllist.size()==0){
		AfxMessageBox(L"�б��Ѿ�Ϊ�գ�");
	}
	//���Ԫ��
	urllist.clear();
	RefreshUrllist();
}

void CTIandSIDlg::RefreshUrllist(){
	//���������б�
	m_urllist.ResetContent();
	int i=0;
	for(i=0;i<urllist.size();i++){
		CString record;
		//�ļ���
		CString filename;
		int nPos=urllist[i].ReverseFind('\\');   
		filename=urllist[i].Mid(nPos+1);
		m_urllist.InsertString(i,filename);
	}
}

void CTIandSIDlg::OnBnClickedAbout()
{
	CAboutDlg dlg;
	dlg.DoModal();
}



//void CTIandSIDlg::OnClickedRadioDraw()
//{
//	UpdateData(TRUE);
//}


void CTIandSIDlg::SystemClear()
{
	resultdlg->SystemClear();
}

void CTIandSIDlg::SetState(Systemstate state)
{
	sysstate=state;
	switch(state){
	case SYSTEM_PREPARE:{
		m_output.EnableWindow(TRUE);
		GetDlgItem(IDC_URLLIST_DELETE)->EnableWindow(TRUE);
		GetDlgItem(IDC_URLLIST_DELETEALL)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDPAUSE)->EnableWindow(FALSE);
		GetDlgItem(IDSTOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_URLLIST_ADD)->EnableWindow(TRUE);
		GetDlgItem(IDPAUSE)->SetWindowText(L"��ͣ");
		break;
				 }
	case SYSTEM_PAUSE:{
		GetDlgItem(IDPAUSE)->SetWindowText(L"����");
		break;
				 }
	case SYSTEM_PROCESS:{
		m_output.EnableWindow(FALSE);
		GetDlgItem(IDC_URLLIST_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_URLLIST_DELETEALL)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDPAUSE)->EnableWindow(TRUE);
		GetDlgItem(IDSTOP)->EnableWindow(TRUE);
		GetDlgItem(IDC_URLLIST_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDPAUSE)->SetWindowText(L"��ͣ");
		state=SYSTEM_PROCESS;
		break;
				 }
	}
}

void CTIandSIDlg::OnBnClickedResult()
{
	resultdlg->ShowWindow(TRUE);
}




void CTIandSIDlg::OnDropFiles(HDROP hDropInfo)
{
	LPTSTR pFilePathName =(LPTSTR)malloc(500);
	::DragQueryFile(hDropInfo, 0, pFilePathName,500);  // ��ȡ�Ϸ��ļ��������ļ�������ؼ���

	m_inputurl.SetWindowText(pFilePathName);

	::DragFinish(hDropInfo);   // ע����������٣��������ͷ�Windows Ϊ�����ļ��ϷŶ�������ڴ�
	free(pFilePathName);

	CDialogEx::OnDropFiles(hDropInfo);
}


void CTIandSIDlg::OnBnClickedPause()
{
	if(sysstate==SYSTEM_PROCESS){
		SetState(SYSTEM_PAUSE);
	}else{
		SetState(SYSTEM_PROCESS);
	}
}


void CTIandSIDlg::OnBnClickedStop()
{
	SetState(SYSTEM_PREPARE);
}


void CTIandSIDlg::OnWebsite()
{
	ShellExecuteA(NULL, "open","http://blog.csdn.net/leixiaohua1020",NULL,NULL,SW_SHOWNORMAL);
}


void CTIandSIDlg::OnUrllistQuickadd()
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
			urllist.push_back(szFileName); 
		} 
		RefreshUrllist();
	} 
}

void CTIandSIDlg::OnCancel()
{
	if(IDOK==AfxMessageBox(L"ȷ���˳���",MB_OKCANCEL|MB_ICONINFORMATION)){
		resultdlg->DestroyWindow();
		delete resultdlg;
		CDialogEx::OnCancel();
	}

}