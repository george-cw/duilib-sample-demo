#include "StdAfx.h"
#include "IniFile.h"

#define MAX_ALLSECTIONS 2048 //ȫ���Ķ���
#define MAX_SECTION 260 //һ����������
#define MAX_ALLKEYS 6000 //ȫ���ļ���
#define MAX_KEY 260 //һ����������

namespace IniFile
{

BOOL CIni::SetFileName(LPCTSTR lpFileName)
{
	m_strFileName=lpFileName;
	return FALSE;
}

_tstring CIni::GetFileName(void)
{
	return m_strFileName;
}

BOOL CIni::SetValue(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpValue,bool bCreate)
{
	TCHAR lpTemp[MAX_PATH] ={0};

	//����if����ʾ�������bCreateΪfalseʱ����û���������ʱ�򷵻�TRUE����ʾ����
	//!*&*none-value*&!* ��Щ�ַ�û���ر����壬������д�Ƿ�ֹ������ͬ��
	if (!bCreate)
	{
		GetPrivateProfileString(lpSection,lpKey,_T("!*&*none-value*&!*"),lpTemp,MAX_PATH,m_strFileName.c_str());
		if(lstrcmp(lpTemp,_T("!*&*none-value*&!*"))==0)
			return TRUE;
	}

	if(WritePrivateProfileString(lpSection,lpKey,lpValue,m_strFileName.c_str()))
		return FALSE;
	else
		return GetLastError();
}

_tstring CIni::GetValue(LPCTSTR lpSection, LPCTSTR lpKey)
{
	DWORD dValue;
	TCHAR lpValue[MAX_PATH] ={0};

	dValue=GetPrivateProfileString(lpSection,lpKey,_T(""),lpValue,MAX_PATH,m_strFileName.c_str());
	return lpValue;
}

BOOL CIni::DelSection(LPCTSTR lpSection)
{
	if(WritePrivateProfileString(lpSection,NULL,NULL,m_strFileName.c_str()))
		return FALSE;
	else
		return GetLastError();
}

BOOL CIni::DelKey(LPCTSTR lpSection, LPCTSTR lpKey)
{
	if(WritePrivateProfileString(lpSection,lpKey,NULL,m_strFileName.c_str()))
		return FALSE;
	else
		return GetLastError();
}


int CIni::GetSections(vector <_tstring> & arrSection)
{
	/*
	������������
	GetPrivateProfileSectionNames - �� ini �ļ��л�� Section ������
	��� ini �������� Section: [sec1] �� [sec2]���򷵻ص��� 'sec1',0,'sec2',0,0 �����㲻֪�� 
	ini ������Щ section ��ʱ���������� api ����ȡ���� 
	*/
	int i; 
	int iPos=0; 
	int iMaxCount;
	TCHAR chSectionNames[MAX_ALLSECTIONS]={0}; //�ܵ���������ַ���
	TCHAR chSection[MAX_SECTION]={0}; //���һ��������
	GetPrivateProfileSectionNames(chSectionNames,MAX_ALLSECTIONS,m_strFileName.c_str());

	//����ѭ�����ضϵ�����������0
	for(i=0;i<MAX_ALLSECTIONS;i++)
	{
		if (chSectionNames[i]==0)
		{
			if (chSectionNames[i]==chSectionNames[i+1])
				break;
		}
	}

	iMaxCount=i+1; //Ҫ��һ��0��Ԫ�ء����ҳ�ȫ���ַ����Ľ������֡�
	arrSection.clear();//���ԭ����

	for(i=0;i<iMaxCount;i++)
	{
		chSection[iPos++]=chSectionNames[i];
		if(chSectionNames[i]==0)
		{ 
			arrSection.push_back(chSection);
			memset(chSection,0,MAX_SECTION);
			iPos=0;
		}

	}

	return (int)arrSection.size();
}

int CIni::GetKeyValues(vector <_tstring>& arrKey,vector <_tstring>& arrValue,LPCTSTR lpSection)
{
	/*
	������������
	GetPrivateProfileSection- �� ini �ļ��л��һ��Section��ȫ��������ֵ��
	���ini����һ���Σ������� "��1=ֵ1" "��2=ֵ2"���򷵻ص��� '��1=ֵ1',0,'��2=ֵ2',0,0 �����㲻֪�� 
	���һ�����е����м���ֵ����������� 
	*/
	int i; 
	int iPos=0;
	_tstring strKeyValue;
	int iMaxCount;
	TCHAR chKeyNames[MAX_ALLKEYS]={0}; //�ܵ���������ַ���
	TCHAR chKey[MAX_KEY]={0}; //�������һ������

	GetPrivateProfileSection(lpSection,chKeyNames,MAX_ALLKEYS,m_strFileName.c_str());

	for(i=0;i<MAX_ALLKEYS;i++)
	{
		if (chKeyNames[i]==0)
		{
			if(chKeyNames[i+1]==0)
				break;
		}

	}

	iMaxCount=i+1; //Ҫ��һ��0��Ԫ�ء����ҳ�ȫ���ַ����Ľ������֡�
	arrKey.clear();//���ԭ����
	arrValue.clear();

	for(i=0;i<iMaxCount;i++)
	{
		chKey[iPos++]=chKeyNames[i];
		if(chKeyNames[i]==0)
		{
			strKeyValue=chKey;
			//arrKey.Add(strKeyValue.Left(strKeyValue.Find(_T("="))));
			//arrValue.Add(strKeyValue.Mid(strKeyValue.Find(_T("="))+1,1));
			_tstring subKey,subValue;
			subKey = subValue =_T("");

			int nIndex = strKeyValue.find('=');
			subKey = strKeyValue.substr(0,nIndex);
			subValue = strKeyValue.substr(nIndex+1,strKeyValue.length() - (nIndex+1));
 			arrKey.push_back(subKey);
			arrValue.push_back(subValue);
			memset(chKey,0,MAX_KEY);
			iPos=0;
		}

	}

	return (int)arrKey.size();
}

BOOL CIni::DelAllSections()
{
	int nSection;
	vector <_tstring> arrSection;
	nSection= GetSections(arrSection);
	for(int i=0;i<nSection;i++)
	{
		if(DelSection(arrSection[i].c_str()))
			return GetLastError();
	}
	return FALSE;
}

}
