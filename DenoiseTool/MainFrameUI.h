#include "stdafx.h"
#include "BasicWnd.h"

class CMainFrameUI: public CBasicWnd,public INotifyUI,public IDialogBuilderCallback
{
public:
	CMainFrameUI(void);
	~CMainFrameUI(void);

protected:
	virtual void Init();
	void InitWidgets();
	void InitUiText();
	void Release();

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void Notify(TNotifyUI& msg);
	void OnClick(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);


	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:

	CLabelUI *m_pVersionLabel;
};