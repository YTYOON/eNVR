#pragma once
#include "resource.h"
#include "INI.h"
#include "BtnST.h"

#define ASSIGNIPID 9
#define FTPID 8
#define SYSTEMID 7
#define SYSTEMSETTING 7
#define	ACCESSID 23
#define	PGATEID 24
#define EVENTACTION 25
#define NAS 26
#define ADDCAMERAID 1
#define CAMERAGROUPID 4
#define EVENTID 3
#define RECORDINGID 5
#define MULMONID 6
#define PLAYBACKIDD	10

// CDlgSettingTag dialog

class CDlgSettingTag : public CDialog
{
	DECLARE_DYNAMIC(CDlgSettingTag)

public:
	CDlgSettingTag(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettingTag();

// Dialog Data
	enum { IDD = IDD_DLG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
 
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedAddCam();
	afx_msg void OnBnClickedIp();
	afx_msg void OnBnClickedEvent();
	afx_msg void OnBnClickedCamgroup();
	afx_msg void OnBnClickedRecording();
	afx_msg void OnBnClickedSystem();
	afx_msg void OnBnClickedMulmon();
	afx_msg void OnBnClickedAccess();
	afx_msg void OnBnClickedPGate();
	afx_msg void OnBnClickedEventAction();
	afx_msg void OnBnClickedNAS();

	CButtonST *btnclose;
	CButtonST *btnaddcam;
	CButtonST *btnip;
	CButtonST *btnevent;
	CButtonST *btncamgroup;
	CButtonST *btnemap;
	CButtonST *btnrecording;
	CButtonST *btnmulmon;
	CButtonST *btnsystem;
	CButtonST *btnftp;
	CButtonST *btnac;
	CButtonST *btnpgate;
	CButtonST *btneventaction;
	CButtonST *btnnas;

	CStatic *pstMulti;
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	vector<CRect> m_rectButton;

	//Access Control
	CIniReader		m_ini;
	bool mb_AC, mb_gate;
};
