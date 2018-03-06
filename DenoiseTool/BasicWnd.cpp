#include "StdAfx.h"
#include "BasicWnd.h"

CBasicWnd::CBasicWnd(void)
{
}

CBasicWnd::~CBasicWnd(void)
{
}



LRESULT CBasicWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CBasicWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CBasicWnd::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
	::ScreenToClient(*this, &pt);

	RECT rcClient;
	::GetClientRect(*this, &rcClient);

	if( !::IsZoomed(*this) ) {
		RECT rcSizeBox = m_pm.GetSizeBox();
		if( pt.y < rcClient.top + rcSizeBox.top ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
			return HTTOP;
		}
		else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
			if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
			if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
			return HTBOTTOM;
		}
		if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
		if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
	}

	RECT rcCaption = m_pm.GetCaptionRect();
	if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
		&& pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
			CControlUI* pControl = static_cast<CControlUI*>(m_pm.FindControl(pt));
			if( pControl && _tcsicmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
				_tcsicmp(pControl->GetClass(), _T("OptionUI")) != 0 )
				return HTCAPTION;
	}

	return HTCLIENT;
}

LRESULT CBasicWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	SIZE szRoundCorner = m_pm.GetRoundCorner();
	if( !::IsIconic(*this)/* && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) */) {
		RECT rcClient;
		::GetClientRect(*this, &rcClient);
		RECT rc = { rcClient.left, rcClient.top + szRoundCorner.cx, rcClient.right, rcClient.bottom };
		HRGN hRgn1 = ::CreateRectRgnIndirect( &rc );
		HRGN hRgn2 = ::CreateRoundRectRgn(rcClient.left, rcClient.top, rcClient.right + 1, rcClient.bottom - szRoundCorner.cx, szRoundCorner.cx, szRoundCorner.cy);
		::CombineRgn( hRgn1, hRgn1, hRgn2, RGN_OR );
		::SetWindowRgn(*this, hRgn1, TRUE);
		::DeleteObject(hRgn1);
		::DeleteObject(hRgn2);
	}

	bHandled = FALSE;
	return 0;
}

LRESULT CBasicWnd::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	MONITORINFO oMonitor = {};
	oMonitor.cbSize = sizeof(oMonitor);
	::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
	::CRect rcWork = oMonitor.rcWork;
	LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
	lpMMI->ptMaxPosition.x = rcWork.left;
	lpMMI->ptMaxPosition.y = rcWork.top;
	lpMMI->ptMaxSize.x = rcWork.right - rcWork.left;
	lpMMI->ptMaxSize.y = rcWork.bottom - rcWork.top;
	bHandled = FALSE;
	return 0;
}

LRESULT CBasicWnd::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
	if( wParam == SC_CLOSE ) {
		 ::PostQuitMessage(0L);
		 bHandled = TRUE;
		return 0;
	}
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CBasicWnd::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRes;
}

LRESULT CBasicWnd::OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}



LRESULT CBasicWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
// 	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
// 	styleValue &= ~WS_CAPTION;
// 	//styleValue &= ~WS_THICKFRAME;
// 
// 	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
// 	m_WndShadow.Create(m_hWnd);
// 	m_WndShadow.SetSize(15);
// 	m_WndShadow.SetPosition(0, 0);

//	::SetWindowLong(*this, GWL_STYLE, styleValue );



	LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
	styleValue &= ~WS_CAPTION;
	styleValue &= ~WS_THICKFRAME;
	::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	m_WndShadow.Create(m_hWnd);
	OnSetShadowSize();
	m_WndShadow.SetPosition(0, 0);
	Init();
	return 0;
}


LRESULT CBasicWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0;
	BOOL bHandled = TRUE;

	switch( uMsg ) {
	case WM_CREATE:        lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
	case WM_CLOSE:         lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:       lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
//	case WM_NCACTIVATE:    lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
	case WM_NCCALCSIZE:    lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
	case WM_NCPAINT:       lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
	case WM_NCHITTEST:     lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
	case WM_SIZE:          lRes = OnSize(uMsg, wParam, lParam, bHandled); break;
	case WM_GETMINMAXINFO: lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
 	case WM_SYSCOMMAND:    lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
	case WM_KEYDOWN:       lRes = OnKeyDown(uMsg,wParam,lParam,bHandled);break;
	case WM_MOUSEHOVER:    lRes = OnMouseHover(uMsg, wParam, lParam, bHandled); break;
	default:
		bHandled = FALSE;
	}

	if( bHandled ) return lRes;
	if( m_pm.MessageHandler(uMsg, wParam, lParam, lRes) ) return lRes;

	return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

void CBasicWnd::OnSetShadowSize()
{
	m_WndShadow.SetSize(4);

}
