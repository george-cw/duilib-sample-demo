// DenoiseTool.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DenoiseTool.h"
#include "shlobj.h"
#include <Shellapi.h>
#include "MainFrameUI.h"

#include <dbghelp.h>//dump file
#pragma comment ( lib, "dbghelp.lib" )
//添加的标识只运行一次的属性名
LPCTSTR g_szPropName =  _T("change dfdf");
HANDLE g_hValue = (HANDLE)1; //全局变量

// 定义枚举窗口回调函数
BOOL CALLBACK EnumWndProc(HWND hwnd,LPARAM lParam)
{
	HANDLE h = GetProp(hwnd,g_szPropName);
	if( h == g_hValue)
	{
		*(HWND*)lParam = hwnd;
		return false;
	}

	return true;
}

BOOL IsRunning()
{
	//查找是否有本程序的前一个实例运行
	HWND oldHWnd = NULL;
	EnumWindows(EnumWndProc,(LPARAM)&oldHWnd); //枚举所有运行的窗口
	if(oldHWnd != NULL)
	{
		::ShowWindow(oldHWnd,SW_SHOWNORMAL);    //激活找到的前一个程序
		::SetForegroundWindow(oldHWnd);   //把它设为前景窗口

		return TRUE;        //退出本次运行
	}

	return FALSE;
}

LONG CallBackCrashHandler(EXCEPTION_POINTERS *pException);


int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	if (IsRunning())
 		return 0;

	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() + _T("skin"));

	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) return 0;

	// Initiation of the shadow
	CWndShadow::Initialize(hInstance);

	GetLanguage();
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CallBackCrashHandler);//捕获异常

	CMainFrameUI *pMainFrame = new CMainFrameUI();
	if (pMainFrame == NULL )return 0;
	pMainFrame->Create(NULL,_T("DenoiseTool"), UI_WNDSTYLE_DIALOG /*| WS_MINIMIZEBOX | WS_MAXIMIZEBOX*/ ,0);//WS_EX_WINDOWEDGE使得双击不能最大化
	//pMainFrame->SetIcon(IDI_MODIFY);
	pMainFrame->CenterWindow();
	pMainFrame->ShowWindow(true);
	CPaintManagerUI::MessageLoop();

	::CoUninitialize();


	return 0;
}

tstring GetPresentTime()  
{  
	SYSTEMTIME time;  
	GetLocalTime(&time);   

	TCHAR wszTime[128];  
	_stprintf_s(wszTime, _T("%04d-%02d-%02d %02d-%02d-%02d-%03d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);  

	return tstring(wszTime);  
}  

void CreateDumpFile(LPCTSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)  
{  
	// 创建Dump文件  
	//  
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  

	// Dump信息  
	//  
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;  
	dumpInfo.ExceptionPointers = pException;  
	dumpInfo.ThreadId = GetCurrentThreadId();  
	dumpInfo.ClientPointers = TRUE;  

	// 写入Dump文件内容  
	//  
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);  

	CloseHandle(hDumpFile);  
}  

LONG CallBackCrashHandler(EXCEPTION_POINTERS *pException)  
{   
	MessageBox(NULL,GetStrFrmKey(_T("ID33008")),GetStrFrmKey(_T("titleError")),MB_OK);
	
	// 以当前时间为文件名  
	//  
	TCHAR szModuleDir[128];
	DWORD dwLength=GetModuleFileName(NULL,szModuleDir,128);

	for(DWORD dw=dwLength-1;dw>0;dw--)
	{
		if(szModuleDir[dw]=='\\')
		{
			szModuleDir[dw+1]=0;
			break;
		}
	}

	tstring strDumpFileName = szModuleDir + GetPresentTime() +_T(".dmp");  

	// 创建Dump文件  
	//  
	CreateDumpFile(strDumpFileName.data(), pException); 

	return EXCEPTION_EXECUTE_HANDLER;  
}