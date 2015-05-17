/**
 * ��򵥵�Direct3D������Ƶ�����ӣ�Direct3D����RGB/YUV��[Surface]
 * Simplest Video Play Direct3D (Direct3D play RGB/YUV)[Surface]
 *
 * ������ Lei Xiaohua
 * leixiaohua1020@126.com
 * �й���ý��ѧ/���ֵ��Ӽ���
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * ������ʹ��Direct3D����RGB/YUV��Ƶ�������ݡ�ʹ��D3D�е�Surface��Ⱦ���ݡ�
 * ʹ��Surface��Ⱦ��Ƶ�������һ�ַ�����ʹ��Texture����˵�����Ӽ򵥣��ʺ�
 * ����ѧϰ��
 * �������ò�������: 
 *
 * [��ʼ��]
 * Direct3DCreate9(): ���IDirect3D9
 * IDirect3D9->CreateDevice(): ͨ��IDirect3D9����Device���豸����
 * IDirect3DDevice9->CreateOffscreenPlainSurface(): ͨ��Device����һ��Surface���������棩��
 *
 * [ѭ����Ⱦ����]
 * IDirect3DSurface9->LockRect(): �����������档
 * memcpy(): �������
 * IDirect3DSurface9->UnLockRect(): �����������档
 * IDirect3DDevice9->BeginScene(): ��ʼ���ơ�
 * IDirect3DDevice9->GetBackBuffer(): ��ú󱸻��塣
 * IDirect3DDevice9->StretchRect(): ����Surface�������󱸻��塣
 * IDirect3DDevice9->EndScene(): �������ơ�
 * IDirect3DDevice9->Present(): ��ʾ������
 *
 * This software play RGB/YUV raw video data using Direct3D. It uses Surface 
 * in D3D to render the pixel data. Compared to another method (use Texture), 
 * it is more simple and suitable for the beginner of Direct3D.
 * The process is shown as follows:
 *
 * [Init]
 * Direct3DCreate9(): Get IDirect3D9.
 * IDirect3D9->CreateDevice(): Create a Device.
 * IDirect3DDevice9->CreateOffscreenPlainSurface(): Create a Offscreen Surface.
 *
 * [Loop to Render data]
 * IDirect3DSurface9->LockRect(): Lock the Offscreen Surface.
 * memcpy(): Fill pixel data...
 * IDirect3DSurface9->UnLockRect(): UnLock the Offscreen Surface.
 * IDirect3DDevice9->BeginScene(): Begin drawing.
 * IDirect3DDevice9->GetBackBuffer(): Get BackBuffer.
 * IDirect3DDevice9->StretchRect(): Copy Surface data to BackBuffer.
 * IDirect3DDevice9->EndScene(): End drawing.
 * IDirect3DDevice9->Present(): Show on the screen.
 */

#include <stdio.h>
#include <tchar.h>
#include <d3d9.h>

CRITICAL_SECTION  m_critial;

IDirect3D9 *m_pDirect3D9= NULL;
IDirect3DDevice9 *m_pDirect3DDevice= NULL;
IDirect3DSurface9 *m_pDirect3DSurfaceRender= NULL;

RECT m_rtViewport;

//set '1' to choose a type of file to play
//Read BGRA data
#define LOAD_BGRA    0
//Read YUV420P data
#define LOAD_YUV420P 1


//Width, Height
const int screen_w=500,screen_h=500;
const int pixel_w=320,pixel_h=180;
FILE *fp=NULL;

//Bit per Pixel
#if LOAD_BGRA
const int bpp=32;
#elif LOAD_YUV420P
const int bpp=12;
#endif

unsigned char buffer[pixel_w*pixel_h*bpp/8];


void Cleanup()
{
	EnterCriticalSection(&m_critial);
	if(m_pDirect3DSurfaceRender)
		m_pDirect3DSurfaceRender->Release();
	if(m_pDirect3DDevice)
		m_pDirect3DDevice->Release();
	if(m_pDirect3D9)
		m_pDirect3D9->Release();
	LeaveCriticalSection(&m_critial);
}


int InitD3D( HWND hwnd, unsigned long lWidth, unsigned long lHeight )
{
	HRESULT lRet;
	InitializeCriticalSection(&m_critial);
	Cleanup();

	m_pDirect3D9 = Direct3DCreate9( D3D_SDK_VERSION );
	if( m_pDirect3D9 == NULL )
		return -1;

	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

	GetClientRect(hwnd,&m_rtViewport);

	lRet=m_pDirect3D9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp, &m_pDirect3DDevice );
	if(FAILED(lRet))
		return -1;

#if LOAD_BGRA
	lRet=m_pDirect3DDevice->CreateOffscreenPlainSurface(
		lWidth,lHeight,
		D3DFMT_X8R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pDirect3DSurfaceRender,
		NULL);
#elif LOAD_YUV420P
	lRet=m_pDirect3DDevice->CreateOffscreenPlainSurface(
		lWidth,lHeight,
		(D3DFORMAT)MAKEFOURCC('Y', 'V', '1', '2'),
		D3DPOOL_DEFAULT,
		&m_pDirect3DSurfaceRender,
		NULL);
#endif


	if(FAILED(lRet))
		return -1;

	return 0;
}


bool Render()
{
	HRESULT lRet;
	//Read Data
	//RGB
	if (fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp) != pixel_w*pixel_h*bpp/8){
		// Loop
		fseek(fp, 0, SEEK_SET);
		fread(buffer, 1, pixel_w*pixel_h*bpp/8, fp);
	}
	
	if(m_pDirect3DSurfaceRender == NULL)
		return -1;
	D3DLOCKED_RECT d3d_rect;
	lRet=m_pDirect3DSurfaceRender->LockRect(&d3d_rect,NULL,D3DLOCK_DONOTWAIT);
	if(FAILED(lRet))
		return -1;

	byte *pSrc = buffer;
	byte * pDest = (BYTE *)d3d_rect.pBits;
	int stride = d3d_rect.Pitch;
	unsigned long i = 0;

	//Copy Data
#if LOAD_BGRA
	int pixel_w_size=pixel_w*4;
	for(i=0; i< pixel_h; i++){
		memcpy( pDest, pSrc, pixel_w_size );
		pDest += stride;
		pSrc += pixel_w_size;
	}
#elif LOAD_YUV420P
	for(i = 0;i < pixel_h;i ++){
		memcpy(pDest + i * stride,pSrc + i * pixel_w, pixel_w);
	}
	for(i = 0;i < pixel_h/2;i ++){
		memcpy(pDest + stride * pixel_h + i * stride / 2,pSrc + pixel_w * pixel_h + pixel_w * pixel_h / 4 + i * pixel_w / 2, pixel_w / 2);
	}
	for(i = 0;i < pixel_h/2;i ++){
		memcpy(pDest + stride * pixel_h + stride * pixel_h / 4 + i * stride / 2,pSrc + pixel_w * pixel_h + i * pixel_w / 2, pixel_w / 2);
	}
#endif

	lRet=m_pDirect3DSurfaceRender->UnlockRect();
	if(FAILED(lRet))
		return -1;

	if (m_pDirect3DDevice == NULL)
		return -1;

	m_pDirect3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
	m_pDirect3DDevice->BeginScene();
	IDirect3DSurface9 * pBackBuffer = NULL;

	m_pDirect3DDevice->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&pBackBuffer);
	m_pDirect3DDevice->StretchRect(m_pDirect3DSurfaceRender,NULL,pBackBuffer,&m_rtViewport,D3DTEXF_LINEAR);
	m_pDirect3DDevice->EndScene();
	m_pDirect3DDevice->Present( NULL, NULL, NULL, NULL );
	pBackBuffer->Release();

	return true;
}


LRESULT WINAPI MyWndProc(HWND hwnd, UINT msg, WPARAM wparma, LPARAM lparam)
{
	switch(msg){
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparma, lparam);
}

int WINAPI WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize = sizeof(wc);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpfnWndProc = (WNDPROC)MyWndProc;
	wc.lpszClassName = L"D3D";
	wc.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&wc);

	HWND hwnd = NULL;
	hwnd = CreateWindow(L"D3D", L"Simplest Video Play Direct3D (Surface)", WS_OVERLAPPEDWINDOW, 100, 100, screen_w, screen_h, NULL, NULL, hInstance, NULL);
	if (hwnd==NULL){
		return -1;
	}
	
	if(InitD3D( hwnd, pixel_w, pixel_h)==E_FAIL){
		return -1;
	}

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);

#if LOAD_BGRA
	fp=fopen("../test_bgra_320x180.rgb","rb+");
#elif LOAD_YUV420P
	fp=fopen("../test_yuv420p_320x180.yuv","rb+");
#endif
	if(fp==NULL){
		printf("Cannot open this file.\n");
		return -1;
	}

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT){
		//PeekMessage, not GetMessage
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			Sleep(40);
			Render();
		}
	}


	UnregisterClass(L"D3D", hInstance);
	return 0;
}

