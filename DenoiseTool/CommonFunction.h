#pragma once

using namespace std;

void ActDbgPrint(const TCHAR* format, ...);

CStdString GetLanguage();
CStdString GetStrFrmID(DWORD	ResID , CStdString defaultstr = _T(""));
CStdString GetStrFrmKey(CStdString keyname , CStdString defaultstr = _T(""), CStdString AppName = _T(""));
bool InitAppPath();