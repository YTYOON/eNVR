#pragma once

// CMyTasksPane

class  CMainFrame;

class CMyTasksPane : public CMFCTasksPane
{
	DECLARE_DYNAMIC(CMyTasksPane)

public:
	CMyTasksPane(CMainFrame* pMainFrame);
	virtual ~CMyTasksPane();


public:
	CMainFrame* m_pMainFrame;
protected:
    CMFCTasksPaneTaskGroup* m_pFullGroupCaption;
    map <int, int> m_mapGroupHeight;
    map <int, HWND> m_mapTaskHwnd;
    map <CMFCTasksPaneTaskGroup*, bool> m_mapCollapse;
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnShowControlBarMenu(CPoint point);
    int AddWindow(int nGroup, HWND hwndTask, int nWndHeight, BOOL bAutoDestroyWindow = FALSE, DWORD dwUserData = 0);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    //afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
protected:
    void vRestoreStatus(void);
};


