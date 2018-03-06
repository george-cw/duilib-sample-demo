#pragma once
#include "WndShadow.h"

class CBasicWnd:public CWindowWnd
{
public:
	CBasicWnd(void);
	~CBasicWnd(void);


	LPCTSTR GetWindowClassName() const { return _T("LoginWnd"); };
	UINT GetClassStyle() const { return CS_DBLCLKS; };
 	virtual void Init()= 0;


	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	 
	LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){ return (wParam == 0) ? TRUE : FALSE; }
	LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){ return 0; }
	LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){ return 0;}
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);	 
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    virtual void OnSetShadowSize();

public:
	CPaintManagerUI m_pm;

	CWndShadow m_WndShadow;

};
