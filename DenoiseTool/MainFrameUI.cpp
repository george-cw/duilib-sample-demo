#include "stdafx.h"
#include "MainFrameUI.h"

extern CStdString g_AppPath;
extern LPCTSTR g_szPropName;
extern HANDLE g_hValue;

CMainFrameUI::CMainFrameUI(void)
{

}
CMainFrameUI::~CMainFrameUI(void)
{
	::RemoveProp(m_hWnd, g_szPropName);
}

void CMainFrameUI::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("windowinit") ) 
	{
		OnPrepare(msg);
		return;
	}
	else if( msg.sType == _T("click") ) 
	{
		OnClick(msg);
	}

}

void CMainFrameUI::Init()
{
	m_pm.Init(m_hWnd);
	
	CDialogBuilder builder;
	CControlUI* pRoot = builder.Create(_T("main_skin.xml"), (UINT)0, this, &m_pm);
	ASSERT(pRoot && "Failed to parse XML");
	m_pm.AttachDialog(pRoot);
	m_pm.AddNotifier(this);

	//…Ë÷√¥∞ø⁄ Ù–‘
	SetProp(m_hWnd,g_szPropName,g_hValue);

	InitAppPath();
	InitWidgets();
	InitUiText();
}

void CMainFrameUI::InitWidgets()
{
	m_pVersionLabel = static_cast<CLabelUI*>(m_pm.FindControl(_T("version_label")));
	
}
void CMainFrameUI::InitUiText()
{
	m_pVersionLabel->SetText(GetStrFrmKey(_T("version")));
}


void CMainFrameUI::Release()
{

}

void CMainFrameUI::OnPrepare(TNotifyUI& msg)
{
	
} 

void CMainFrameUI::OnClick(TNotifyUI& msg)
{
	CStdString sCtrlName = msg.pSender->GetName();
	if( sCtrlName == _T("closebtn") || sCtrlName == _T("quit_btn"))
	{
		SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0); 
		return; 
	}
	else if( sCtrlName == _T("minbtn"))
	{ 
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		return; 
	}
	else if( sCtrlName == _T("maxbtn"))
	{ 
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); 
		return; 
	}
	else if( sCtrlName == _T("restorebtn"))
	{ 
		SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); 
		return; 
	}
}

LRESULT CMainFrameUI::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( wParam == SC_CLOSE ) {
		Release();
		PostQuitMessage(0);
		bHandled = TRUE;
		return 0;
	}

	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(true);
		}
		else 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("maxbtn")));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_pm.FindControl(_T("restorebtn")));
			if( pControl ) pControl->SetVisible(false);
		}
	}

	return lRes;
}

LRESULT CMainFrameUI::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	
	return CBasicWnd::HandleMessage(uMsg,wParam,lParam);
}

CControlUI* CMainFrameUI::CreateControl(LPCTSTR pstrClass)
{

	return NULL;
}