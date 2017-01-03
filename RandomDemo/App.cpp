#include "pch.h"
#include "DemoWindow.h"

class App final : CWinApp
{
	virtual BOOL InitInstance() override
	{
		m_pMainWnd = new DemoWindow;
		m_pMainWnd->ShowWindow(m_nCmdShow);
		m_pMainWnd->UpdateWindow();
		return true;
	}

	virtual BOOL OnIdle(LONG lCount) override
	{
		m_pMainWnd->InvalidateRect(nullptr, false);
		return true;
	}
};

App app;