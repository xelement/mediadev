/** 
 * TIandSIcmd
 *
 * ���ʺ磬�����裬����
 * nonmarking2010@gmail.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 *
 * ����YUV�ļ�·�����ֱ��ʣ����ظ�ʽ
 * ���TI,SIֵ
 */
#include "stdafx.h"
#include "TIandSIcmd.h"

//��ȡ�ļ���С
long filesize( FILE *fp )
{
    long int save_pos;
    long size_of_file;

    /* Save the current position. */
    save_pos = ftell( fp );

    /* Jump to the end of the file. */
    fseek( fp, 0L, SEEK_END );

    /* Get the end position. */
    size_of_file = ftell( fp );

    /* Jump back to the original position. */
    fseek( fp, save_pos, SEEK_SET );

    return( size_of_file );
}

//�Զ���vector������  
bool SortFunc( float v1, float v2)//ע�⣺�������Ĳ���������һ��Ҫ��vector��Ԫ�ص�����һ��  
{  
    return v1 > v2;//��������  
}  

//����TI��SI
int TIandSI(const char* strPath,int width,int height,int pixfmt)
{
	//----------------
	//CFile cfYUV;
	FILE* fYUV;
	FILE* fout;
	CString out_filename;
	out_filename.Format("%s_TISI.csv",strPath);
	fout=fopen(out_filename,"wb+");

	int nHeight, nWidth;
	int nFrameNum;
	int nYSize;	
	int nUVSize;
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

	vector<float> si_f;
	vector<float> ti_f;
	CString si_char,ti_char;
	/****************��ʼ������**********************/
	nHeight = height;
	nWidth = width;
	nYSize = nHeight*nWidth;
	switch (pixfmt) {
    case 400: nUVSize = 0; break;
    case 422: nUVSize = nYSize; break;
    case 444: nUVSize = nYSize*2; break;
    default :
    case 420: nUVSize = nYSize*0.5; break;
	}
	nFrameSize = nYSize + nUVSize;

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
	NewUVBuffer = new unsigned char [nUVSize];
	memset(NewUVBuffer, 0x80, nUVSize);

	NewSIBuffer = new float [nYSize];
	NewTIBuffer = new float [nYSize];

	//cfYUV.Open(strPath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeRead); 
	fYUV=fopen(strPath,"rb");
	if(fYUV==NULL)
	{
		printf("cant open file\n");
		return FALSE;
	}
	nFrameNum = filesize(fYUV)/nFrameSize;

	fprintf(fout,"----------------SI-------------------\n");
		/********************����SI***********************/
		for(k = 0;k < nFrameNum; k++)//���n֡��ѭ��n-1��
		{
			//cfYUV.Seek(k*nFrameSize, CFile::begin);
			fseek(fYUV,k*nFrameSize,SEEK_SET);
			//cfYUV.Read(pFrame, nYSize);
			fread(pFrame,1,nYSize,fYUV);

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
				printf("SI(%d) : %.3f\n",k+1,flSI_S);
				fprintf(fout,"%.3f\n",flSI_S);
				si_f.push_back(flSI_S);
		}

		fprintf(fout,"\n----------------TI-------------------\n");
		/*******************************����TI*********************************/
		for(k = 0;k < nFrameNum-1; k++)//���n֡��ѭ��n-1��
		{
			fseek(fYUV,k*nFrameSize,SEEK_SET);
			fread(pFrame,1,nYSize,fYUV);

			//cfYUV.Seek((k+1)*nFrameSize, CFile::begin);
			//cfYUV.Read(pNextFrame, nYSize);

			fseek(fYUV,(k+1)*nFrameSize,SEEK_SET);
			fread(pNextFrame,1,nYSize,fYUV);

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
			printf("TI(%d) : %.3f\n",k+1,flTI_S);
			fprintf(fout,"%.3f\n",flTI_S);
			ti_f.push_back(flTI_S);
		}

		fprintf(fout,"\n----------------Summary-------------------\n");
		std::sort(ti_f.begin(),ti_f.end(),SortFunc);  
		float max_ti=ti_f[0];
		fprintf(fout,"max_TI:%.3f\n",max_ti);
		float sum=0;int num=0;
		for(vector<float>::iterator it = ti_f.begin();it!=ti_f.end();it++) ///�ڶ��ֵ��÷���  
		{  
			sum=sum+*it;  num++;
		}
		float avg_ti=sum/num;
		fprintf(fout,"avg_TI:%.3f\n",avg_ti);

		std::sort(si_f.begin(),si_f.end(),SortFunc);
		float max_si=si_f[0];
		fprintf(fout,"max_SI:%.3f\n",max_si);
		for(vector<float>::iterator it = si_f.begin();it!=si_f.end();it++) ///�ڶ��ֵ��÷���  
		{  
			sum=sum+*it;  num++;
		}
		float avg_si=sum/num;
		fprintf(fout,"avg_SI:%.3f\n",avg_si);


	//���
	si_f.clear();
	ti_f.clear();
	//--------
	//cfYUV.Close();
	fclose(fYUV);
	fclose(fout);

	delete []pFrame;
	delete []pFrame2D;
	delete []pNextFrame;
	delete []pNextFrame2D;
	delete []NewYBuffer;
	delete []NewUVBuffer;
	delete []NewTIBuffer;
	delete []NewSIBuffer;

	return 1;
}

//usage
void tisi_usage(){
	printf("---------------------help------------------------\n");
	printf("TISI:calculate the TI and SI values for YUV\n");
	printf("options\n");
	printf("-i <string> the input file name\n");
	printf("-x <int> width\n");
	printf("-y <int> height\n");
	printf("-f <string> format:420,422,444,400\n");
	printf("-h help\n");
	printf("-------------------------------------------------\n");
}

int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	int ret;
	CString filename="",width_s,height_s,pixfmt_s; 
	int width=-1,height=-1,pixfmt=420;


	extern _TCHAR *optarg;
	int opt;
	if(argc==1){
		printf("parameters are not enough!\n");
		tisi_usage();
		return FALSE;
	}
	while ((opt =getopt(argc, argv,_T("i:x:y:f:h"))) != -1)
    {
      switch (opt)
		{
		case 'h':
		  tisi_usage();
		  return FALSE;
		case 'i':
			filename.Format("%s",optarg);
			break;
		case 'x':
			width_s.Format("%s",optarg);
            width=atoi(width_s);
			break;
		case 'y':
			height_s.Format("%s",optarg);
            height=atoi(height_s);
			break;
		case 'f':
			pixfmt_s.Format("%s",optarg);
            pixfmt=atoi(pixfmt_s);
			break;
		default:
		  printf("unkown: %c\n", opt);
		  tisi_usage();
		  return FALSE;
		}
    }
    if(filename==""){
        fprintf(stderr,"please input the filename\n");
        return -1;
    }
    if(width==-1||height==-1){
        fprintf(stderr,"please input width and height\n");
        return -1;
    }

	ret=TIandSI(filename,width,height,pixfmt);
	if(!ret)
		return -1;

	return 0;
}

