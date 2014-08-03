#if !defined(AFX_EMAPVIEW_H__9ADAC351_E1AD_420D_A3FB_8BB45EE48724__INCLUDED_)
#define AFX_EMAPVIEW_H__9ADAC351_E1AD_420D_A3FB_8BB45EE48724__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EMapView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEMapView view

class CEMapView : public CScrollView
{
protected:
	CEMapView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CEMapView)

// Attributes
public:

// Operations
public:
	void vSetEMap(CString csFile);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEMapView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	CRect m_rect;
	BYTE* pData ;
	HENHMETAFILE hMetaHandle;
	virtual ~CEMapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEMapView)
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EMAPVIEW_H__9ADAC351_E1AD_420D_A3FB_8BB45EE48724__INCLUDED_)
