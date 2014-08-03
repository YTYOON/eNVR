#pragma once
//#include "EtroExplorerView.h"
#include "resource.h" 
#include "LiveView.h"
#include "Video.h"
//#include "CEtroExplorerView.h"
#include "EtroExplorerView.h"
// CDlgFlyAway dialog

class CDlgFlyAway : public CDialog
{
	DECLARE_DYNAMIC(CDlgFlyAway)

public:
	CDlgFlyAway(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgFlyAway();

// Dialog Data
	enum { IDD = IDD_DLG_FLYAWAY };
	
	//CEtroExplorerView* m_pLiveView;
	CLiveView* m_pLiveView;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	void GetNodeItem(vector <NODEITEM*> vctNode);
	void GetMatrixIdx(int LayOutIdx);
	vector <NODEITEM*> m_vctNode;
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

public:
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int m_nLayOutIdx;
	void GetRect(CRect rect);
	void OnFullScreen();
	void GetAutoPlay(bool bAutoPlay);
	CRect m_rect;
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	long m_nOriginalStyle;
	bool m_bFullScreen;
	bool m_bAutoPlay;

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual void PostNcDestroy();
};
