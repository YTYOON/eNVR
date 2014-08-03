#pragma once
#include "resource.h"
#include "BaseDialog.h"

#pragma comment(lib, "mpr.lib")
#include <Winnetwk.h>

// CDlgNASSetting dialog

struct NASInfo{

	TCHAR	tcServerType[16];
	TCHAR	tcServerAddr[MAX_PATH];
//    TCHAR   tcServerPath[MAX_PATH];
	TCHAR	tcServerUser[64];
	TCHAR	tcServerPass[128];
};



class CDlgNASSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgNASSetting)

public:
	CDlgNASSetting(CWnd* pParent = NULL);   // standard constructor
	CDlgNASSetting(NASInfo m_NASInfo, CWnd* pParent = NULL);
	virtual ~CDlgNASSetting();

// Dialog Data
	enum { IDD = IDD_DLG_NAS_DETAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnNasLan();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	CComboBox m_comboServerType;

	CCrypto m_CCrypto;
	void strtohex(const char* str, unsigned char* hex);
	CString DecryptPassword(CString csEncrypt);

	NASInfo m_NASInfo;
	//void vAttachNASInfo(NASInfo &m_NASInfo);
	bool bModFlag;
	afx_msg void OnBnClickedCancel();
	int GetIpAddress(const CString &sHostName, CStringArray &sIpAddress);
};
