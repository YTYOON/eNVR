#pragma once
#include "resource.h"
#include "BtnST.h"
#include "afxwin.h"
// CDlgMatrixTag dialog
class CMyCheckBox : public CButton
{
	DECLARE_DYNAMIC(CMyCheckBox)
public:
	//DECLARE_DYNCREATE(CMyCheckBox) 
   // DECLARE_SERIAL(CSzjsbList)	
	CMyCheckBox();
	virtual ~CMyCheckBox();
    bool m_bCheckedLiveview;
	bool m_bCheckedIPwall;

	UINT  m_nMyCBoxID;

protected:
    CBrush m_BrushBg;
		
public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//{{AFX_MSG(CMyCheckBox)
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT nCtlColor);
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnPaint();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
   	virtual void PreSubclassWindow();

};


class CDlgMatrixTag : public CDialog
{
	DECLARE_DYNAMIC(CDlgMatrixTag)

public:
	CDlgMatrixTag(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgMatrixTag();

// Dialog Data
	enum { IDD = IDD_DLG_MATRIX };

protected:
    CMyCheckBox m_ButtonAutoScan;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

///////////Larry add 102-11-20/////////////
public:
	void vSetMyCBoxID(UINT nMyCBoxID);
	void vStartDisplayAutoPlay(int nAutoPlayTime);
	int	m_nAutoScanTime;
	int	m_nAutoScanCount;
	CRect m_rcDispAutoText;
/////////////////////////////////////////////

	int m_lRet;			//Larry Add 103-03-28 for Setting - Multiple Monitor //
	int vBitmapIDToMartixCode(int nBitmapID);
	int vMartixCodeToBitmapID(int nMartixCode);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnAutoScanSetting();
    //afx_msg void OnBnAutoScan();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMatrixBnClicked(UINT uid);
	
	CButtonST *btnAutoScanSetting;
	CButtonST *btnAutoScanSettingGroup;
	
    CBrush m_Brushbk;
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	bool m_bSetStyle;
    afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	afx_msg void OnDestroy();
	//bool m_bAutoScan;
protected:
    BOOL CreateMatrixs();
	vector <CButtonST*> m_vtBtnMatrixs;
public:
    afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	CButtonST m_btnAutioScanSetting;
	afx_msg void OnBnClickedBtnAutoscanSetting();
	void vSetMatrixBtnHot(UINT nId);
	map <int, int> mapTable;
	
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

protected:
	virtual void OnCancel();
};
