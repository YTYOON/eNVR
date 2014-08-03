#pragma once
#include "resource.h"
typedef unsigned __int8   uint8_t;
// CSnapshotViewer

class CSnapshotViewer : public CStatic
{
	DECLARE_DYNAMIC(CSnapshotViewer)

public:
	CSnapshotViewer();
	virtual ~CSnapshotViewer();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	POINT m_PickPoint;
	CRect m_rectDraw;
	CRect m_rcImg;
	uint8_t* m_picture_buf;
	BITMAPINFOHEADER m_bih;
	int m_zDelta;
	bool m_bOriginalRect;
	void vGetData(uint8_t* picture_buf,	BITMAPINFOHEADER bih, DWORD biSizeImage,CRect rectDraw);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};


