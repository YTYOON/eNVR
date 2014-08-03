#pragma once


// CTimeLine

struct DateTime
{
	TCHAR  Date[4];
	int		hour;
	bool bEnable;

};

class CTimeLine : public CStatic
{
	DECLARE_DYNAMIC(CTimeLine)

public:
	enum STATUS {CLEAR =0, CONTINUOUS, EVENT};
	CTimeLine();
	virtual ~CTimeLine();

protected:
	DECLARE_MESSAGE_MAP()
	int m_nPaintedIdx;
public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	void vSetGridRect();
	vector <CRect> m_rects;
	vector <bool> m_bColored;
	vector <bool> m_bColoredEvent;
	vector <STATUS> m_ChartStatus;
	vector <DateTime> m_vctDateTime;
	bool m_bDrag;
	int m_nStartIdx;
	int m_nEndIdx;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DateTime RectToDate(int nRectIdx);
	CBrush m_RectBrush;
	CBrush m_RectBrushEvent;
	bool m_bChangeColor, m_bEnable;
	void vReset();
	void vInitial();

	void vFillDayRect(int iDay, bool bFill);
	void vSetChangeColorFlag(bool bFlag);

	void vGetEventByte(BYTE* btSrc);
	void vFillAllRect();

	void vFillScheduleAndEventRect(BYTE* btSrc);
	void vDisable();
	void vEnable();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	
	int m_iArrayFirstByte;
};


