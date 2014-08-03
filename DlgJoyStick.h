#pragma once


// CDlgJoyStick dialog

class CDlgJoyStick : public CDialog
{
	DECLARE_DYNAMIC(CDlgJoyStick)

public:
	CDlgJoyStick(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgJoyStick();

// Dialog Data
	enum { IDD = IDD_JOYST_IMM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonCalibration();
};
