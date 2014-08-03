 #pragma once
#include "Resource.h"
#include "BaseDialog.h"
#include "BaseStatic.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "PictureCtrl.h"
#include "EmapCam.h"
#include "SystemInfo.h"
#include	 "IPCamTreeCtrl.h"
#include "EventVideo.h"
#include "SplitterControl.h"
#include "INI.h"
#include "ColorBox.h"
#include "afxext.h"
#include "EventManager.h"
#include "starinterface.h"
#include "TabpageEmapEvent.h"
#include "TabpageEmapAc.h"
//#include "EventReceive2.h"
//#include "AcService.h"
#include "EmapAccessCam.h"
#include "mycurl.h"

#define	STCW 200
#define	STCH 40
#define	COCKPITW 400
#define	VIDEOW 200
#define	VIDEOH 150
#define	SPLITW 150
#define	ACTIMER 7000
// CDlgEmap dialog

//static HANDLE hThreadAccess;
static int notifyWriter(char* data, size_t size, size_t nmemb,void* lParam);

struct eSETS{
	enum TYPE {STRING=0, CHECK, INTERGER};
	CString csKey;
	CString csCaption;
	eSETS(TCHAR* A, TCHAR* B){csKey = A; csCaption = B;};
    eSETS(){};

	//enum TYPE {STRING=0, CHECK, INTERGER};
	//TYPE nType;
	//CString csKey;
	//bool    bCheck;
	//int     nInt;
	//CString csCaption;
	//int iAlertLevel;
	//bool IntFlag;//if NewValue = int, IntFlag = false;
	//eSETS(TCHAR* A, TCHAR* B){csKey = A; csCaption = B;};
 //   eSETS(){};
	
};


struct eVERSETS{
	map<CString, eSETS> aSet;
};

struct RevEvent{
		CString csIP;
		CString csType;
		CString csTrigger;
		CString csCamIDX;
	};

class CDlgEmap : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgEmap)

public:
	CDlgEmap(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEmap();

// Dialog Data
	enum { IDD = IDD_DLGEMAP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	struct STRGROUP
		{
			int     nid;
			CString Desc;
			CString mapfile;
			CString align_;
			CString size_;
			CString	videoon;
			bool Default_;
		};
	struct strEventTimer
	{
		int		iTimer;
		int		iIndex;
		char	cMode;
	};

	//Event Filter ...
	struct EVENTFILTER{
		CString	type_code;
		CString	type_desc;
	};

	struct CameraInfo{
		CString csIP;
		CString csPort;
		CString csUser;
		CString csPwd;
		CString csCamIDX;
		CString csProtocol;
		CWnd*	pWnd;
	};


public:
	CNVRBk *m_pbk;
	CToolTipCtrl*	m_pToolTipCtrl; // m_ToolTip;
	CIniReader		m_ini;
	bool fnModifyAccess(LPVOID param, CString oData, CString oCamIdx);

	Curlplus mycurl;

	virtual BOOL		OnInitDialog();

	//Variables
	//CDatabase*		pDB;

	bool	ib_e1, ib_e2, ib_e3, ib_e4, ib_e5, ib_e6;
	bool	ib_dirty, mb_hideset;
	bool	m_bDragging;	//T during a drag operation
	bool	mb_AC;				//Access Control flag

	int		il_posx, mi_orgwidth, mi_orgheight;	//i_mapw, i_maph,
	int		m_groupid;
	int		m_testCnt, m_nDragIndex ;
	int		m_maxMapW, m_maxMapH;
	char	mc_closevideo, mc_folderMode, mc_fileMode;
	int		mi_closevideo, m_currGCamid, m_nDropIndex;


	CString		m_Folder, m_curFile;
	CString		m_dragTxt;
	CString		m_curHMS;	
	CString*		mp_acCamIdx;

	//Control Variables
	CComboBox m_Group;
	CBitmapButton		m_butsave, m_butevent, m_buteventset, m_butArDown;
	CBitmapButton m_butbrowse;
	CBitmapButton m_butfolder;
	CBitmapButton m_butsaveas, m_butclear;

	CStatic m_stgroup, m_stfolder;
	CStatic m_stsec;	
	CStatic m_stmap, m_stcam, m_stevent;	
	
	CListCtrl m_listcam;
	CListCtrl m_listevent;
	CEdit m_editmap, m_editFolder;
	CEdit m_editset;
	CColorBox		m_tbeventset;

 	CImageList			m_images, m_SmallImg;;
	CSplitterControl	m_splitV;
	CSplitterControl	m_splitH;
	CPictureCtrl			m_map1;
	strEventTimer		m_EventTimer; 
	 
	// Drag Stuff define here
	CListCtrl*			m_pDragList;		//Which ListCtrl we are dragging FROM
	CPictureCtrl*		m_pDropPic;		//Which ListCtrl we are dropping ON
	CImageList*		m_pDragImage;	//For creating and managing the drag-image
	CWnd*				m_pDropWnd;		//Pointer to window we are dropping on (will be cast to CListCtrl* type)
	CPictureCtrl*		m_pDropPC;

	//Vector
 	vector <STRGROUP>  m_pvtGroup;
	//vector<CEmapCam*> m_vpcam;
	vector<CBaseStatic*> m_vpName;
	vector<strEventTimer> m_vpEventTimer;
	vector <CEventVideo*> m_vpEventVideo;
	vector <NODEITEM*> m_vctpAllCam;
	vector <EVENTFILTER*> m_pvtEventFilter;
	//vector <CEmapAccessCam*> m_vpAccess;


	HANDLE hThreadEventRec;
	static UINT _stdcall GetEventFromService(LPVOID lpParam);

	bool fnInitFolder(void);
	bool fnInitGroup(void);
	void DoResizeH(int delta);
	bool fnInitControl(void);
	bool fnGroupChanged();
	void vPopulateCam(int o_groupid);
	void fnDeleteAllCam(void);
	bool fnDisplayCam(int oGroupid);
	bool fnDisplayCamAC(int oGroupid);

	void fnMaximizeBmp(int &o_maxW, int &o_maxH);
	bool fnFileExist(CString o_file);
	bool fnCamWithinGroup(int oGroupid, int oCamidx);
	//void fnBuildEventFilter(int oPosX, int oPosY);
	//bool fnAfterMove(int o_newx, int o_newy, int o_camgroupid);
	bool fnAfterMove(int o_newx, int o_newy, int o_camgroupid, int o_camidx);
	bool fnDropWithin(LPPOINT mpt);
	void fnResumeCam(int o_camidx);
	void fnRemoveCam(int o_camidx);
	bool fnSaveDB(void);
	bool fnLoadBmp(CString o_file);
	bool fnCheckCamExist(int o_camGroupid, int o_camIdx);
	void fnAfterVideoClose(int o_camidx);
	static int CheckEventPriority(CString lCamIdx, CString lEventType);
	bool fnLoadCamInfo(void);

	bool fnCloseVideo(int o_camidx);	// Close all the videos
	bool fnCloseVideo(void);
	void fnHideSetting(bool oHide);	
	void fnFreeMemory(void);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void fnSaveEventLog();

	//Event Related
	void ParseEvent(CString lParam, CString osIndex, CString osIP, CString osEventType);
	bool EventDisplay(CString oCamIdx, CString oCamIP, CString oEventType);
	static CString SmartEventType(CString oOrgEventType, bool obLong);
	static CString SmartEventType(CString oOrgEventType);
	static bool HighlightCam(int oCamIdx, int oPriority);

	// Video Related
	void fnDisplayVideo(int o_camidx);
	int fnVideoOn(CString oVideoOn);
	bool fnFilterVideoOn(CString oEventtype);

	public:
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg void OnCbnSelchangeCombogroup();
		//afx_msg void OnBnClickedButemapset();
		afx_msg void OnBnClickedCkmd();
		afx_msg void OnBnClickedCkvl();
		afx_msg void OnBnClickedCkdi();
		afx_msg void OnBnClickedButfolder();
		afx_msg void OnBnClickedButbrowse();
		afx_msg void OnBnClickedButsave();
		afx_msg void OnBnClickedRadioauto();
		afx_msg void OnBnClickedRadioman();
		afx_msg void OnLvnBegindragListcam();
		afx_msg void OnLvnBegindragListcam(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void  StartTimer ();  // Start the Timer
		afx_msg void  StopTimer ();  // Start the Timer
		afx_msg void OnBnClickedEvent();	//Event button
	 	afx_msg void OnBnClickedButardown();
		//afx_msg void OnBnClickedButsaveas();
		afx_msg void OnMove(int x, int y);
		//afx_msg void OnBnClickedButclear();
		afx_msg void OnClose();

		//Static Function
		static int notifyWriter(char* data, size_t size, size_t nmemb,void* lParam);
		static UINT _stdcall ReceiveAccess(LPVOID lpParam);

		// Called by begin drag of camera list control, if map didn't exist, return false, prevent crash
		bool fnCheckMapExist(void);

		CEventManager*	mp_eventManager;
		//CStarinterface m_StarInter;
		void fnInitAccess(void);

		afx_msg void OnBnClickedButreport();

		HANDLE	hAccessThread;
		static UINT _stdcall fnThreadBegin(LPVOID lpParam);
		// Tab control for event and access control
		CTabCtrl m_tab;
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

		//CTabpageEmapEvent	m_pageEvent;
		//CTabpageEmapAc		m_pageAc;
		CDialog*	pDialog[2];

		int	m_CurSelTab;

		afx_msg void OnTcnSelchangeTabeventac(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnBnClickedCkac();
		CRect m_rect;

		CEventReceive* m_EmapEventRec;
		static void fnEmapCallBack(EventInfo *m_EventInfo, void *lpParam);
		CCriticalSection m_CS;

	 
			CTabpageEmapEvent	m_pageEvent;
			CTabpageEmapAc			m_pageAc;
 
	/*	CBitmapButton* m_butclear;
		CBitmapButton* m_butsaveas;*/
		/*afx_msg void OnBnClickedButsaveas();
		afx_msg void OnBnClickedButclear();*/
		void fnStartAccessService(void);
	

		vector <CameraInfo*> m_vcCamInfo;

	//static UINT _stdcall ReceiveAccess(LPVOID lpParam);
	//vector <CameraInfo*> m_vcCamInfo;
	//static int fnDavinciCode(char* data, size_t size, size_t nmemb, void *lParam);
	//static void vParseSegment(TCHAR* tszBuffer, map <CString, eVERSETS>::iterator pos);
	CString fnDavinciCode(CString o_orgHex);
	// Timer being triggerred, call send command CGI here
	void fnAccessTimer(void);

	void fnAccessInsertDB(CString oDate, CString oTime, CString oID, int oAccessID);

	afx_msg void OnBnClickedButsaveas();
	afx_msg void OnBnClickedButclear();
	void fnInitLocalize(void);
	int m_nGroupIdx;
	
	void fnmmcheck(void);
	map <int, int> m_mapListCamIdx;
};


