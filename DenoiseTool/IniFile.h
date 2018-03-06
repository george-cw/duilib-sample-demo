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
//һ���Բ�����
BOOL SetFileName(LPCTSTR lpFileName); //�����ļ���
_tstring GetFileName(void); //����ļ���
BOOL SetValue(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpValue,bool bCreate=true); //���ü�ֵ��bCreate��ָ����������δ����ʱ���Ƿ񴴽���
_tstring GetValue(LPCTSTR lpSection, LPCTSTR lpKey); //�õ���ֵ.
BOOL DelSection(LPCTSTR strSection); //ɾ������
BOOL DelKey(LPCTSTR lpSection, LPCTSTR lpKey); //ɾ������


public:
//�߼�������
int GetSections(vector <_tstring> & arrSection); //ö�ٳ�ȫ���Ķ���
int GetKeyValues(vector <_tstring>& arrKey,vector <_tstring>& arrValue,LPCTSTR lpSection); //ö�ٳ�һ���ڵ�ȫ��������ֵ

BOOL DelAllSections();

};

}
