#pragma once

// CDlgSnapshotDummy dialog
#include "resource.h"
typedef unsigned __int8   uint8_t;

class CDlgSnapshotDummy : public CDialog
{
	DECLARE_DYNAMIC(CDlgSnapshotDummy)

public:
	CDlgSnapshotDummy(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSnapshotDummy();

// Dialog Data
	enum { IDD = IDD_DLGSNAPSHOTDUMMY };

protected:
   // CDlgSnapshot m_Snapshot;
    
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
     virtual BOOL OnInitDialog();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	//afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnPaint();
	//afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	POINT m_PickPoint;
	CRect m_rectDraw;
	CRect m_rcImg;
	uint8_t* m_picture_buf;
	BITMAPINFOHEADER m_bih;
	int m_zDelta;
	bool m_bOriginalRect;
	void vGetBMP(uint8_t* picture_buf,	BITMAPINFOHEADER bih, DWORD biSizeImage, CString csFileName);
};
