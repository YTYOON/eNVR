// EtroExplorerView.h : interface of the CEtroExplorerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EtroExplorerView_H__0BDFC1DC_577B_42F3_B85C_9C16EF51D087__INCLUDED_)
#define AFX_EtroExplorerView_H__0BDFC1DC_577B_42F3_B85C_9C16EF51D087__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Video.h"
#include "GS.h"
//#include "CDMedia.h"
#include "EtroExplorerDoc.h"
#include "INI.h"
#include <afxole.h> 
#include "LiveView.h"

class CEtroExplorerView : public CView
{
public: // create from serialization only
	CEtroExplorerView();
	DECLARE_DYNCREATE(CEtroExplorerView)


// Attributes
public:
	CEtroExplorerDoc* GetDocument();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEtroExplorerView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	void vSetWindowSize(int nWidth, int nHeight);
	void vSetGrids(int nGrid);
	bool bPresentIPCam(char *szHostName, WORD nPort, char *szUserID, char *szUserPassword);
	void vAttach(vector <NODEITEM*> pNode);
	vector <CRect> vGetRect(){return m_rects;};
	virtual ~CEtroExplorerView();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void vFreeVideoSet();
	void vFreeVideoSet(int o_index);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BYTE nFocusVideo;
	vector <CVideo*> m_videos;
	vector <CRect> m_rects;
	BYTE	m_grids;
	long	m_GlobalScreenW;
	int		nCamIndex;
	int     m_nLayout;
	vector <NODEITEM*> m_vctpNode;

protected:

	//{{AFX_MSG(CEtroExplorerView)
	afx_msg void OnZoomWindows();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	CGS m_gs;
	CRect bGetRect();
	//CDMedia m_mediaCtrl;
	CIniReader m_ini;
	CImageList*	m_pDragImage;
	COleDropTarget m_dropTarget;
	bool m_bDragging;
	bool m_bOutOfRange;
	bool bIsOutOfRange(int nNodeCount);
    bool m_bDisplayOSD ;
	bool ib_SingleDisplay;
	bool m_bSwap;
	bool m_bOnFlyAway;
	
	
	void vNewVideoSet(NODEITEM* pNode);
	void vIsDragging(bool m_bLDragging);
	LRESULT vSetOSD(WPARAM wParam, LPARAM lParam);
	
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	bool CheckBackupFile();
	void GetDatabaseFileName(CString& strFileName);
	void ImportDatabase(const CString& strFilePath);
	void ImportReg(const CString& strFilePath);
	void RestartService(const CString& str);
public:

	CLiveView* m_pLiveView;
	void vScreenToViewClientRect(POINT *pt);
	void vGetCurrentGroupNodeItems(vector<NODEITEM*> &vctpNode);
	void vStartWizard();
	void vResetOSD(void);
	void SetRegFlag(bool bflag){ m_pLiveView->SetRegFlag(bflag); }
};

#ifndef _DEBUG  // debug version in EtroExplorerView.cpp
inline CEtroExplorerDoc* CEtroExplorerView::GetDocument()
   { return (CEtroExplorerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EtroExplorerView_H__0BDFC1DC_577B_42F3_B85C_9C16EF51D087__INCLUDED_)
