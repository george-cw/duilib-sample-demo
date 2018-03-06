// DenoiseTool.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "DenoiseTool.h"
#include "shlobj.h"
#include <Shellapi.h>
#include "MainFrameUI.h"

#include <dbghelp.h>//dump file
#pragma comment ( lib, "dbghelp.lib" )
//��ӵı�ʶֻ����һ�ε�������
LPCTSTR g_szPropName =  _T("change dfdf");
HANDLE g_hValue = (HANDLE)1; //ȫ�ֱ���

// ����ö�ٴ��ڻص�����
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
	//�����Ƿ��б������ǰһ��ʵ������
	HWND oldHWnd = NULL;
	EnumWindows(EnumWndProc,(LPARAM)&oldHWnd); //ö���������еĴ���
	if(oldHWnd != NULL)
	{
		::ShowWindow(oldHWnd,SW_SHOWNORMAL);    //�����ҵ���ǰһ������
		::SetForegroundWindow(oldHWnd);   //������Ϊǰ������

		return TRUE;        //�˳���������
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
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CallBackCrashHandler);//�����쳣

	CMainFrameUI *pMainFrame = new CMainFrameUI();
	if (pMainFrame == NULL )return 0;
	pMainFrame->Create(NULL,_T("DenoiseTool"), UI_WNDSTYLE_DIALOG /*| WS_MINIMIZEBOX | WS_MAXIMIZEBOX*/ ,0);//WS_EX_WINDOWEDGEʹ��˫���������
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
	// ����Dump�ļ�  
	//  
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);  

	// Dump��Ϣ  
	//  
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;  
	dumpInfo.ExceptionPointers = pException;  
	dumpInfo.ThreadId = GetCurrentThreadId();  
	dumpInfo.ClientPointers = TRUE;  

	// д��Dump�ļ�����  
	//  
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);  

	CloseHandle(hDumpFile);  
}  

LONG CallBackCrashHandler(EXCEPTION_POINTERS *pException)  
{   
	MessageBox(NULL,GetStrFrmKey(_T("ID33008")),GetStrFrmKey(_T("titleError")),MB_OK);
	
	// �Ե�ǰʱ��Ϊ�ļ���  
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

	// ����Dump�ļ�  
	//  
	CreateDumpFile(strDumpFileName.data(), pException); 

	return EXCEPTION_EXECUTE_HANDLER;  
}