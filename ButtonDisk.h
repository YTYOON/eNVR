#pragma once


#define WM_BUTTONDISK_PRESS		WM_USER + 9428
#define WM_BUTTONDISK_RELEASE		WM_USER + 9429

class CButtonDisk : public CStatic
{
	DECLARE_DYNAMIC(CButtonDisk)

public:
    enum {MOUSE_HOVER = 0, MOUSE_PRESS, NORMAL};
    enum {POS_CENTER = 0, POS_UP, POS_RIGHT, POS_DOWN, POS_LEFT};
	CButtonDisk();
	virtual ~CButtonDisk();

protected:
	DECLARE_MESSAGE_MAP()
    int nAreaHitTest(CPoint& point);
    CDC m_dcEvent[2][5];
    CDC m_dcNormal;
    //CBitmap m_bmpNormal;
    //CBitmap m_bmpFocus[5];
    CBitmap m_bmpNormal;
    CBitmap m_bmpEvent[2][5];
    int m_nStatus;
    int m_nPosStatus;
public:
    afx_msg void OnMouseHover(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


