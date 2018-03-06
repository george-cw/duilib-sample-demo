#pragma once

#include <string>
#include <vector>
#include <string>


using namespace std;

#ifdef _UNICODE
typedef std::wstring _tstring;
#else 
typedef std::string _tstring;
#endif

namespace IniFile
{
class CIni
{
private:
_tstring m_strFileName;
public:
CIni()
{
}
public:
//一般性操作：
BOOL SetFileName(LPCTSTR lpFileName); //设置文件名
_tstring GetFileName(void); //获得文件名
BOOL SetValue(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpValue,bool bCreate=true); //设置键值，bCreate是指段名及键名未存在时，是否创建。
_tstring GetValue(LPCTSTR lpSection, LPCTSTR lpKey); //得到键值.
BOOL DelSection(LPCTSTR strSection); //删除段名
BOOL DelKey(LPCTSTR lpSection, LPCTSTR lpKey); //删除键名


public:
//高级操作：
int GetSections(vector <_tstring> & arrSection); //枚举出全部的段名
int GetKeyValues(vector <_tstring>& arrKey,vector <_tstring>& arrValue,LPCTSTR lpSection); //枚举出一段内的全部键名及值

BOOL DelAllSections();

};

}
