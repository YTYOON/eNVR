#pragma once
#include "resource.h"
#include "VideoThin.h"
#include "SnapshotViewer.h"

// CDlgSnapshot dialog
class CDlgSnapshot : public CDialog
{
	DECLARE_DYNAMIC(CDlgSnapshot)

public:
	CDlgSnapshot(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSnapshot();

// Dialog Data
	enum { IDD = IDD_DLG_SNAPSHOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedOpenDirectory(UINT nCmdID);
    uint8_t* m_picture_buf;
	BITMAPINFOHEADER m_bih;
	POINT m_PickPoint;
	CRect m_rectDraw;
	CRect m_rectDrawOrg;
	CRect m_rcImg;
	CRect m_rectDlg;
	CSnapshotViewer* m_snapshotViewer;
	DWORD m_biSizeImage;
	bool m_bOriginalRect;
	int m_zDelta;	
	CBitmapButton m_pBtnOpenDirectory ;
	CBitmapButton m_pBtnClose ;
	CString m_csFileName;
	void vGetBMP(uint8_t* picture_buf,	BITMAPINFOHEADER bih, DWORD biSizeImage, CString csFileName);
	vector <CRect> m_vctRectBtn;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	CToolTipCtrl* m_ToolTip;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//afx_msg void OnBnClickedButton1();
	//afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	void vGetBackGround();
	CBitmap m_MemBitmap; 
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void DrawAlphaBlend (HWND hWnd, HDC hdcwnd);
	CDC m_MemDC; 
};

