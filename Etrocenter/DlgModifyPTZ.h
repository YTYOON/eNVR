#pragma once
#include "resource.h"
#include "BaseDialog.h"
#include "INI.h"

// CDlgModifyPTZ dialog

struct MoPTZInfo
{
	TCHAR tcProtocol[20];
	TCHAR tcEnable[20];
	TCHAR tcDI1[20];
	TCHAR tcDI2[20];
	TCHAR tcDO[20];
};

class CDlgModifyPTZ : public CDialog
{
	DECLARE_DYNAMIC(CDlgModifyPTZ)

public:
	CDlgModifyPTZ(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgModifyPTZ();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBrush	m_SilverBrush;
	CBrush	m_WhiteBrush;

// Dialog Data
	enum { IDD = IDD_DLG_MODIFY_PTZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	MoPTZInfo m_MoPTZInfo;
	void vSetMoPTZInfo(MoPTZInfo o_MoPTZInfo);

	/*--DB--*/
	//CDatabase m_DB;
	//bool vDBConnect(void);

	void vProtocolInit();
	MoPTZInfo mGetMoPTZInfo();
};
