#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "resource.h"
#include "DlgRecRecord.h"
#include "DlgRecSchedule.h"
#include "INI.h"
#include "DlgRecDuplicate.h"
#include "BaseDialog.h"
#include "TimeLine.h"
#include <bitset>

enum INITIAL_TYPE {RECORDING_DEFAULT = 0, NEW_CAMERA, MODIFY_CAMERA};

// CDlgSettingRecording dialog

struct RecCameraList
{
	CString	csCameraIDX;
	CString	csCameraName;
	CString csIPnName;
	BYTE byteTimeArray[43];
	int iTreePos;
};

//enum RECORDING_TYPE {CONTINOUS = 0, SCHEDULING_EVENT_TRIGGER};
class CNVRBk;
class CDlgSettingRecording : public CDialog
{
	DECLARE_DYNAMIC(CDlgSettingRecording)

public:
	CDlgSettingRecording(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettingRecording();

// Dialog Data
	enum { IDD = IDD_DLG_RECORDING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int iCameraIDX, iCameraTreeIDX;
	int m_iInitFlag;

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl pPathList;
	CTreeCtrl pRECCameraTreeCtrl;
	CEdit pRECCameraNameEdit;
	CTabCtrl pRecordingTab;

	CDatabase* pDB;
	CEdit m_edit;
	int hitRow, hitCol;
	bool bShowFlag;
	enum RECORDING_TYPE {CONTINOUS = 0, SCHEDULING_EVENT_TRIGGER};

	//CRecordTimeLine* m_CRecordTimeLine;
	//CDlgRecSchedule* m_DlgRecSchedule;
	CDlgRecRecord* m_DlgRecRecord;
	//CDlgRecDuplicate* m_CDlgRecDuplicate;
	vector<CString> m_veDuplicateCamera;

	CBitmapButton m_cbRecPathDelete;
	CBitmapButton m_cbRecPathAdd;
	CBitmapButton m_cbRecPathEdit;
	CToolTipCtrl *m_ToolTip;

	CBitmapButton m_cbRecEvent;
	CBitmapButton m_cbRecContinue;

	RecCameraList m_RecCameraList;
	vector<RecCameraList> veRecCameraList;
	//CDatabase DBQuery;
	CString csDSN, csDBUser, csDBPass, csODBC;	//For Database
	CString m_IP;
	CNVRBk *m_pbk;

	BOOL OnInitDialog();
	void vGetPathInfo();
	void vInitRecCameraTreeCtrl();
	//void vDBConnect();
	void vSQLRecCameraList();
	CString csSQLRecordingID(CString csCameraIDX, CString csRecordingID);
	CString csSQLRecording(CString csCameraIDX, CString csRecordingFlag);
	int iSQLScheduleNum(CString csRecordingID, int csScheduleNum);
	int iGetCameraIDXVector(CString csCameraName);
	void vSQLInsertScheduleTime(CString csRecordingID, HTREEITEM hScheduleParent);
	HTREEITEM hGetCameraSelectRoot(HTREEITEM hSelect);
	void vDeleteStoragePath();
	void vDeleteRecordingInfo(int iCameraIDX);
	void vSaveStoragePath();
	void vSaveRecordingInfo(int iCameraIDX);
	void vSaveRecordingInfo(vector<CString> m_veDuplicateCamera);
	void vGetCurrentRecordingPath();
	bool bCheckPathBuffer( CString csPath, int iBuffer);
	void vSetInitFlag(int m_iInitFlag, CString csCameraName, int iCameraIdx);
	void vShowCameraInfo(int iCameraidx, CString csCameraName);

	afx_msg void OnBnClickedButtonAddPath();
	afx_msg void OnBnClickedButtonDeletePath();
	afx_msg void OnBnClickedOk();
	afx_msg void OnTcnSelchangeTabRecording(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeCameraList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDuplicate();
	afx_msg void OnBnClickedButtonRecordingSave();
	afx_msg void OnBnClickedButtonModifyPath();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush	m_SilverBrush;
	CBrush	m_WhiteBrush;

	bool DirtyFlag;
	bool m_bBufferFlag;
//	afx_msg void OnDestroy();
	afx_msg void OnClose();

	afx_msg void OnNMDblclkListPath(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListPath(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnKillfocusEditPathList();
	//CString csStoragePath;
	//bool bCheckStoragePathLength();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadioContinuous();
	afx_msg void OnBnClickedRadioEvent();
	afx_msg void OnBnClickedRadioScheduling();

	CTimeLine m_TimeLine;

	CString csGetRecordingType(int m_iCameraIDX);
	void vShowSchedule(bool bShow);
	afx_msg void OnBnClickedBtnMon();
	afx_msg void OnBnClickedBtnTue();
	afx_msg void OnBnClickedBtnWed();
	afx_msg void OnBnClickedBtnThu();
	afx_msg void OnBnClickedBtnFri();
	afx_msg void OnBnClickedBtnSat();
	afx_msg void OnBnClickedBtnSun();

	bool bMon;
	bool bTue;
	bool bWed;
	bool bThu;
	bool bFri;
	bool bSat;
	bool bSun;

	bool bTimeArrayConvert2Storage(BYTE* btSrc, DWORD nsrcSize, BYTE* btDesc, DWORD nDescSize);
	bool bStorageConvert2TimeArray(BYTE* btSrc, DWORD nSrcSize, BYTE* btDesc, DWORD nDescSize);

	bool bDeleteCameraRecordingInfo(int iCameraIDX);
	bool bSaveCameraBinaryTime(int iCameraIDX, BYTE* bySQLByte, CString csRecordingType);
	bool bGetRecoringTypeAndBinaryTime(int iCameraIDX);
	afx_msg void OnBnClickedRadioScheduleEvent();

	bool bExistECAC(int iCameraIdx);

	BYTE byFromTimeLine[168];
	BYTE bySQLByte[43];
	BYTE byConvertByte[337];

	CButton m_GroupBox;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
