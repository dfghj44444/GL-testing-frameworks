/************************************************************************/
/* Copyright (C) 2012-20112 OneDouble(Xingmin)
/* This file is part of the "My GL DEMO".
/* dfghj77777@gmail.com                                                 
/************************************************************************/

#include "stdafx.h"
#include "MyGLDemo.h"
#include "Triangle.h"
#include "Square.h"
#include "C3DButton.h"
#include "Ultility.h"

#define MAX_LOADSTRING 100

// Global Variable
TCHAR szTitle[MAX_LOADSTRING];
TCHAR szWindowClass[MAX_LOADSTRING];

HGLRC g_hRC = NULL;										// Permanent Rendering Context
HDC   g_hDC = NULL;
HWND  g_hWnd = NULL;										
HINSTANCE g_hInst = NULL;								
						
bool g_bActive=TRUE;									

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

DWORD g_dwCurTime = 0;
DWORD g_dwLastTime= 0;
DWORD g_dwEslapeTime = 0;
DWORD g_dwCreateTimer = 0;

int	mouse_x, mouse_y;	
using namespace std;

std::vector<IObject*> g_vecObjects;

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

void CALLBACK AddBtnToScence()
{
	IObject* aObject; 
	if (RANDOM_RANGE(0,1) > 0.5)
	{	
		aObject = new CTriangle();
	}
	else
	{
		aObject = new CSquare();
	}
	aObject->SetPos(vector3df(RANDOM_RANGE(-2,2),RANDOM_RANGE(-2,2),RANDOM_RANGE(3,10)));

	g_vecObjects.push_back(aObject);
}

//logic loop
void FrameMove(   ) 
{
	g_dwCurTime     = timeGetTime();
	g_dwEslapeTime = g_dwCurTime - g_dwLastTime;
	g_dwLastTime = g_dwCurTime;
	//g_dwCreateTimer += g_dwEslapeTime;
	//if (g_dwCreateTimer > 20000)
	//{
	//	AddBtnToScence();
	//	g_dwCreateTimer = 0;
	//}

	for (vector<IObject*>::iterator it=g_vecObjects.begin();it!=g_vecObjects.end();++it)
	{
		(*it)->FrameMove(g_dwEslapeTime);
	}
}
//void RenderOject(IObject* x)
//{
//	x->Render();
//}

//render loop
void Render( )	
{	
	//calc fps
	int nFps		=  1000/static_cast<int>(g_dwEslapeTime > 1 ? g_dwEslapeTime : 1);
	char strFps[25]= {0,};
	sprintf(strFps,"MyGLDemo --fps:%d",nFps);
	DWORD dwResult;
	SendMessageTimeout(g_hWnd, WM_SETTEXT, 0,
		reinterpret_cast<LPARAM>(strFps),
		SMTO_ABORTIFHUNG, 10000, &dwResult);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	glLoadIdentity();	

	//start
	glTranslatef(-1.5f,0.0f,-6.0f);
	
	int loop = 0;
	for (vector<IObject*>::iterator it=g_vecObjects.begin();it!=g_vecObjects.end();++it)
	{
		glLoadName(loop);										// Assign Object A Name (ID)
		glPushMatrix();	

		(*it)->Render();

		glPopMatrix();	
		loop++;
	}	
	//gluLookAt(0.0,0.0,5.0,0.0,0.0,0.0,0.0,1.0,0.0);

}

void ReSizeGLScene(int width, int height)
{
	if (height==0)	// Prevent A Divide By Zero By
		height=1;

	glViewport(0, 0, width, height);	
	glMatrixMode(GL_PROJECTION);	
	glLoadIdentity();				

	gluPerspective(45.0f,(float)width/(float)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);			
	glLoadIdentity();					
}

char *TextFileRead(char *fn) {


	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {

			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

unsigned int g_fragShader,g_PixelShader;
void SetShaders() {

	char *fs = NULL;

	g_fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	fs = TextFileRead("pixel.frag");

	const char * ff = fs;


	glShaderSource(g_fragShader, 1, &ff,NULL);

	free(fs);

	glCompileShader(g_fragShader);

	g_PixelShader = glCreateProgram();
	glAttachShader(g_PixelShader,g_fragShader);
	glLinkProgram(g_PixelShader);


}

int InitGL(GLvoid)
{
	glShadeModel(GL_SMOOTH);						
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);			
	glClearDepth(1.0f);								
	glEnable(GL_DEPTH_TEST);						
	glDepthFunc(GL_LEQUAL);							
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glColor4f(1.0f, 1.0f, 1.0f, 0.5);				
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);				
	glEnable(GL_BLEND);

	IObject* aBtn = new C3DButton();
	g_vecObjects.clear();
	g_vecObjects.push_back(aBtn);
 
	glewInit();

	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else
	{
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}
	SetShaders();


	return TRUE;	
}

void ExitWindow(void)	
{
	if (g_hRC)	
	{
		if (!wglMakeCurrent(NULL,NULL))	
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

		if (!wglDeleteContext(g_hRC))
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

		g_hRC=NULL;	
	}
	for (vector<IObject*>::iterator it=g_vecObjects.begin();it!=g_vecObjects.end();++it)
	{
		_DELETE(*it);
	}
	g_vecObjects.erase(g_vecObjects.begin(),g_vecObjects.end());

}


BOOL CreateGLWindow( int width, int height, int bits)
{
	GLuint		PixelFormat;			

	static	PIXELFORMATDESCRIPTOR pfd=		
	{
		sizeof(PIXELFORMATDESCRIPTOR),1,									
		PFD_DRAW_TO_WINDOW |				
		PFD_SUPPORT_OPENGL |				
		PFD_DOUBLEBUFFER,					
		PFD_TYPE_RGBA,						
		bits,								
		0, 0, 0, 0, 0, 0,					
		0,									
		0,									
		0,									
		0, 0, 0, 0,							
		16,									
		0,									
		0,									
		PFD_MAIN_PLANE,						
		0,									
		0, 0, 0								
	};

	if (!(g_hDC=GetDC(g_hWnd)))						
	{
		ExitWindow();								
		MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	if (!(PixelFormat=ChoosePixelFormat(g_hDC,&pfd)))	
	{
		ExitWindow();								
		MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	if(!SetPixelFormat(g_hDC,PixelFormat,&pfd))		
	{
		ExitWindow();							
		MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	if (!(g_hRC=wglCreateContext(g_hDC)))			
	{
		ExitWindow();							
		MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	if(!wglMakeCurrent(g_hDC,g_hRC))					
	{
		ExitWindow();								
		MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;							
	}

	ShowWindow(g_hWnd,SW_SHOW);					
	SetForegroundWindow(g_hWnd);				
	SetFocus(g_hWnd);						
	ReSizeGLScene(width, height);				

	if (!InitGL())									
	{
		ExitWindow();								
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								
	}

	return TRUE;		
}

// object pick
void MouseEvent(UINT e)											
{
	unsigned int	buffer[512];										
	int	hits;												

	if (g_vecObjects.size()<1)			return;													

	int	viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(512, buffer);							

	(void) glRenderMode(GL_SELECT);

	glInitNames();												
	glPushName(0);												

	glMatrixMode(GL_PROJECTION);								
	glPushMatrix();												
	glLoadIdentity();											

	gluPickMatrix((double) mouse_x, (double) (viewport[3]-mouse_y), 1.0f, 1.0f, viewport);

	gluPerspective(45.0f, (float) (viewport[2]-viewport[0])/(float) (viewport[3]-viewport[1]), 0.1f, 100.0f);
	glMatrixMode(GL_MODELVIEW);							
	Render();											
	glMatrixMode(GL_PROJECTION);						
	glPopMatrix();										
	glMatrixMode(GL_MODELVIEW);	

	hits=glRenderMode(GL_RENDER);						

	if (hits > 0)//if hit anyone										
	{
		int	choose = buffer[3];									
		int depth = buffer[1];									

		for (int loop = 1; loop < hits; loop++)					
		{
			if (buffer[loop*4+1] < unsigned int(depth))
			{
				choose = buffer[loop*4+3];						
				depth = buffer[loop*4+1];						
			}       
		}

		if (!g_vecObjects[choose]->IsBtn() && e == WM_LBUTTONDOWN)								// if is not a btn,delete it
		{
			_DELETE(g_vecObjects[choose]);
			g_vecObjects.erase(g_vecObjects.begin() + choose);//delete it

			_ASSERT(g_vecObjects[0]->IsBtn());
			g_vecObjects[0]->SetFocus(FALSE);
		}
		else//add or make btn status changed
		{
			g_vecObjects[choose]->HandleMouse(e);
		}
	}
	else
	{
		_ASSERT(g_vecObjects[0]->IsBtn());
		g_vecObjects[0]->SetFocus(FALSE);
	}
}


//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW ;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYGLDEMO));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYGLDEMO);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   g_hInst = hInstance; // 将实例句柄存储在全局变量中

   g_hWnd = CreateWindow(szWindowClass, szTitle, WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
      CW_USEDEFAULT, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!g_hWnd)
      return FALSE;

   ShowWindow(g_hWnd, nCmdShow);
   UpdateWindow(g_hWnd);
   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_ACTIVATE:	
		if (!HIWORD(wParam))
			g_bActive=TRUE;
		else
			g_bActive=FALSE;
		return 0;		
	case WM_SIZE:								
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam)); 
			return 0;								
		}
	case WM_ERASEBKGND:
		return TRUE;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MOUSEMOVE:
		{
			mouse_x = LOWORD(lParam);          
			mouse_y = HIWORD(lParam);
			MouseEvent(message);
		}
		break;
		break;
	case WM_DESTROY:
		ExitWindow();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MYGLDEMO, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
		return FALSE;

	if (!CreateGLWindow (SCREEN_WIDTH,SCREEN_HEIGHT,16))
		return FALSE;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MYGLDEMO));

	// 主消息循环:
	while( TRUE )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if( 0 == GetMessage(&msg, NULL, 0, 0 ) )
			{
				return (int)msg.wParam;
			}

			if( 0 == TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
			{
				TranslateMessage( &msg ); 
				DispatchMessage( &msg );
			}
		}
		else
		{
			if( g_bActive )
			{
				FrameMove();
				Render();
				SwapBuffers(g_hDC);
			}
			else
			{
				WaitMessage();
			}
		}
	}

	return (int) msg.wParam;
}



