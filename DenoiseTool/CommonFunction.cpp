#include "stdafx.h"
#include "CommonFunction.h"
#include "IniFile.h"
#include "shlobj.h"
#include "shlwapi.h"

CStdString g_OSLangName = _T("");
CStdString g_AppPath = _T("");

void ActDbgPrint(const TCHAR* format, ...)
{

	//return ;

	if (format == NULL || format[0] == '\0')
		return;

	va_list argList;
	va_start( argList, format );
	TCHAR output[8192] = { 0 };
	_vstprintf(output, format, argList);
	va_end( argList );

	OutputDebugString(output);

}

bool InitAppPath()
{
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
	g_AppPath=szModuleDir;

	if (g_AppPath != _T(""))
		return true;
	else
		return false;
}

CStdString GetLanguage()
{
#ifdef _NEW_FACE_
	LANGID LangID = GetThreadLocale();
#else
	LANGID LangID = GetSystemDefaultLangID();
#endif	
	CStdString csLang;
	if(LangID==0x804)
	{
		csLang = _T("CHN_SIM");
	}
	else if ((LangID==0x404)||(LangID==0xc04)||(LangID==0x1004))
	{
		csLang = _T("CHN_TRI");
	}
	else
	{
		csLang = _T("ENG");
	}

	//csLang = _T("ENG");
	g_OSLangName = csLang;
	
	return csLang;
}

/*************************************************
  Description:    // 通过ID获取对应的资源
  Warning:        // 参数1为纯ID数
				  // 与另外一个GetLangTextEx不同
*************************************************/
CStdString GetStrFrmID(DWORD	ResID , CStdString defaultstr)
{
	CStdString csTemp;
	csTemp.Format(_T("ID%d") , ResID);
	
	CStdString ret = _T("");
	
	ret = GetStrFrmKey(csTemp , _T(""));
	
	if (ret == _T("")) ret = defaultstr;
	
	return ret;
}


CStdString GetStrFrmKey(CStdString keyname , CStdString defaultstr , CStdString AppName )
{
	CStdString ret;

	CStdString inipath = g_AppPath + _T("Lang.lng");
	
	TCHAR value[MAX_PATH];
	
	if (AppName != _T(""))
		GetPrivateProfileString(AppName,keyname,defaultstr,value,MAX_PATH,inipath);	
	else
		GetPrivateProfileString(g_OSLangName,keyname,defaultstr,value,MAX_PATH,inipath);
	
	ret = value;
	
	
	return ret;
}