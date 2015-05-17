/**
 * ��򵥵�DirectSound������Ƶ�����ӣ�DirectSound����PCM��
 * Simplest Audio Play DirectSound (DirectSound play PCM) 
 *
 * ������ Lei Xiaohua
 * leixiaohua1020@126.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * ������ʹ��DirectSound����PCM��Ƶ�������ݡ�
 * ����򵥵�DirectSound������Ƶ�Ľ̡̳�
 *
 * �������ò�������: 
 *
 * [��ʼ��]
 * DirectSoundCreate8(): ����һ��DirectSound����
 * SetCooperativeLevel(): ����Э��Ȩ�ޣ���Ȼû��������
 * IDirectSound8->CreateSoundBuffer(): ����һ��������������
 * IDirectSoundBuffer->QueryInterface(IID_IDirectSoundBuffer8..): 
 *			����һ�������������������洢Ҫ���ŵ����������ļ���
 * IDirectSoundBuffer8->QueryInterface(IID_IDirectSoundNotify..): 
 *			����֪ͨ����֪ͨӦ�ó���ָ������λ���Ѿ��ﵽ��
 * IDirectSoundNotify8->SetNotificationPositions(): ����֪ͨλ�á�
 * IDirectSoundBuffer8->SetCurrentPosition(): ���ò��ŵ���ʼ�㡣
 * IDirectSoundBuffer8->Play(): ��ʼ���š�
 *
 * [ѭ����������]
 * IDirectSoundBuffer8->Lock(): ��������������׼��д�����ݡ�
 * fread(): ��ȡ���ݡ�
 * IDirectSoundBuffer8->Unlock(): ��������������
 * WaitForMultipleObjects(): �ȴ�������λ���Ѿ��ﵽ����֪ͨ��
 *
 * This software plays PCM raw audio data using DirectSound.
 * It's the simplest tutorial about DirectSound.
 *
 * The process is shown as follows:
 *
 * [Init]
 * DirectSoundCreate8(): Init DirectSound object.
 * SetCooperativeLevel(): Must set, or we won't hear sound.
 * IDirectSound8->CreateSoundBuffer(): Create primary sound buffer.
 * IDirectSoundBuffer->QueryInterface(IID_IDirectSoundBuffer8..): 
 *			Create secondary sound buffer.
 * IDirectSoundBuffer8->QueryInterface(IID_IDirectSoundNotify..): 
 *			Create Notification object.
 * IDirectSoundNotify8->SetNotificationPositions():
 *			Set Notification Positions.
 * IDirectSoundBuffer8->SetCurrentPosition(): Set position to start.
 * IDirectSoundBuffer8->Play(): Begin to play.
 *
 * [Loop to play data]
 * IDirectSoundBuffer8->Lock(): Lock secondary buffer.
 * fread(): get PCM data.
 * IDirectSoundBuffer8->Unlock(): UnLock secondary buffer.
 * WaitForMultipleObjects(): Wait for Notifications.
 */
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dsound.h>


#define MAX_AUDIO_BUF 4 
#define BUFFERNOTIFYSIZE 192000 


int sample_rate=44100;	//PCM sample rate
int channels=2;			//PCM channel number
int bits_per_sample=16;	//bits per sample

BOOL main(int argc,char * argv[])
{
	int i;
	FILE * fp;
	if((fp=fopen("../NocturneNo2inEflat_44.1k_s16le.pcm","rb"))==NULL){
		printf("cannot open this file\n");
		return -1;
	}

	IDirectSound8 *m_pDS=NULL;					
	IDirectSoundBuffer8 *m_pDSBuffer8=NULL;	//used to manage sound buffers.
	IDirectSoundBuffer *m_pDSBuffer=NULL;	
	IDirectSoundNotify8 *m_pDSNotify=NULL;		
	DSBPOSITIONNOTIFY m_pDSPosNotify[MAX_AUDIO_BUF];
	HANDLE m_event[MAX_AUDIO_BUF];

	SetConsoleTitle(TEXT("Simplest Audio Play DirectSound"));//Console Title
	//Init DirectSound
	if(FAILED(DirectSoundCreate8(NULL,&m_pDS,NULL)))
		return FALSE;
	if(FAILED(m_pDS->SetCooperativeLevel(FindWindow(NULL,TEXT("Simplest Audio Play DirectSound")),DSSCL_NORMAL)))
		return FALSE;


	DSBUFFERDESC dsbd;
	memset(&dsbd,0,sizeof(dsbd));
	dsbd.dwSize=sizeof(dsbd);
	dsbd.dwFlags=DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY |DSBCAPS_GETCURRENTPOSITION2;
	dsbd.dwBufferBytes=MAX_AUDIO_BUF*BUFFERNOTIFYSIZE; 
	//WAVE Header
	dsbd.lpwfxFormat=(WAVEFORMATEX*)malloc(sizeof(WAVEFORMATEX));
	dsbd.lpwfxFormat->wFormatTag=WAVE_FORMAT_PCM;   
	/* format type */
	(dsbd.lpwfxFormat)->nChannels=channels;          
	/* number of channels (i.e. mono, stereo...) */
	(dsbd.lpwfxFormat)->nSamplesPerSec=sample_rate;     
	/* sample rate */
	(dsbd.lpwfxFormat)->nAvgBytesPerSec=sample_rate*(bits_per_sample/8)*channels; 
	/* for buffer estimation */
	(dsbd.lpwfxFormat)->nBlockAlign=(bits_per_sample/8)*channels;        
	/* block size of data */
	(dsbd.lpwfxFormat)->wBitsPerSample=bits_per_sample;     
	/* number of bits per sample of mono data */
	(dsbd.lpwfxFormat)->cbSize=0;

	//Creates a sound buffer object to manage audio samples. 
	HRESULT hr1;
	if( FAILED(m_pDS->CreateSoundBuffer(&dsbd,&m_pDSBuffer,NULL))){   
		return FALSE;
	}
	if( FAILED(m_pDSBuffer->QueryInterface(IID_IDirectSoundBuffer8,(LPVOID*)&m_pDSBuffer8))){
		return FALSE ;
	}
	//Get IDirectSoundNotify8
	if(FAILED(m_pDSBuffer8->QueryInterface(IID_IDirectSoundNotify,(LPVOID*)&m_pDSNotify))){
		return FALSE ;
	}
	for(i =0;i<MAX_AUDIO_BUF;i++){
		m_pDSPosNotify[i].dwOffset =i*BUFFERNOTIFYSIZE;
		m_event[i]=::CreateEvent(NULL,false,false,NULL); 
		m_pDSPosNotify[i].hEventNotify=m_event[i];
	}
	m_pDSNotify->SetNotificationPositions(MAX_AUDIO_BUF,m_pDSPosNotify);
	m_pDSNotify->Release();

	//Start Playing
	BOOL isPlaying =TRUE;
	LPVOID buf=NULL;
	DWORD  buf_len=0;
	DWORD res=WAIT_OBJECT_0;
	DWORD offset=BUFFERNOTIFYSIZE;

	m_pDSBuffer8->SetCurrentPosition(0);
	m_pDSBuffer8->Play(0,0,DSBPLAY_LOOPING);
	//Loop
	while(isPlaying){
		if((res >=WAIT_OBJECT_0)&&(res <=WAIT_OBJECT_0+3)){
			m_pDSBuffer8->Lock(offset,BUFFERNOTIFYSIZE,&buf,&buf_len,NULL,NULL,0);
			if(fread(buf,1,buf_len,fp)!=buf_len){
				//File End
				//Loop:
				fseek(fp, 0, SEEK_SET);
				fread(buf,1,buf_len,fp);
				//Close:
				//isPlaying=0;
			}
			m_pDSBuffer8->Unlock(buf,buf_len,NULL,0);
			offset+=buf_len;
			offset %= (BUFFERNOTIFYSIZE * MAX_AUDIO_BUF);
			printf("this is %7d of buffer\n",offset);
		}
		res = WaitForMultipleObjects (MAX_AUDIO_BUF, m_event, FALSE, INFINITE);
	}

	return 0;
}
