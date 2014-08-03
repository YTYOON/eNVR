#pragma once
#include "resource.h"
#include "BaseDialog.h"
#include "INI.h"

// CDlgModifyStream dialog

struct MoStreamInfo
{
	TCHAR tcVideoType[20];
	TCHAR tcProtocol[50];
	TCHAR tcResolution[20];
	TCHAR tcPort[10];
	TCHAR tcLV_RE[10];
	//TCHAR tcRecording[10];
};

class CDlgModifyStream : public CDialog
{
	DECLARE_DYNAMIC(CDlgModifyStream)

public:
	CDlgModifyStream(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgModifyStream();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CBrush	m_SilverBrush;
	CBrush	m_WhiteBrush;

// Dialog Data
	enum { IDD = IDD_DLG_MODIFY_STREAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	MoStreamInfo m_MoStreamInfo;
	void vSetMoStreamInfo(MoStreamInfo o_MoStreamInfo);
	void vProtocolInit();
	void vResolutionInit();
	
	/*--DB--*/
	//CDatabase m_DB;
	//bool vDBConnect(void);
	
	MoStreamInfo mGetMoStreamInfo();
};
