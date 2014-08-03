 #include "Resource.h"
#include "BaseDialog.h"
#include "BaseStatic.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "PictureCtrl.h"
#include "EmapAccessCam.h"
#include "SystemInfo.h"
#include	 "IPCamTreeCtrl.h"
#include "EventVideo.h"
#include "SplitterControl.h"
#include "INI.h"
#include "ColorBox.h"
#include "afxext.h"
#include "EventManager.h"
#include "starinterface.h"
//#include "EmapAccessCam.h"
#include "DlgSettingAc.h"
#include "DlgAcSearch.h"
#include "EventReceive.h"

#define	PANELW 380
#define	TOPY	85
#define	ACVIDEOW 300
#define	ACVIDEOH 205
#define ACDFTBRAND _T("ETROVISION")
#define ACDFTMODEL _T("AC_EV721H")

static int notifyWriter(char* data, size_t size, size_t nmemb,void* lParam);

#pragma once

// CDlgAc dialog

class CDlgAc : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgAc)

public:
	CDlgAc(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgAc();
	CRect		m_rect;

// Dialog Data
	enum { IDD = IDD_DLGAC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
		static int notifyWriter(char* data, size_t size, size_t nmemb,void* lParam);

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

		struct STRAC
		{
			int    nid;
			int	camidx;
			CString	csBrand;
			int	_timer;
			char	csecurity;
		};

		struct CameraInfo
		{
		CString csIP;
		CString csPort;
		CString csUser;
		CString csPwd;
		CString csCamIDX;
		CString	csProtocol;
		CWnd*	pWnd;
		CDatabase*	cDB;
		CListCtrl*	pListAc;
	};

struct sSETS
{
	enum TYPE {STRING=0, CHECK, INTERGER};
	CString csKey;
	CString csCaption;
	sSETS(TCHAR* A, TCHAR* B){csKey = A; csCaption = B;};
    sSETS(){};	
};

	struct sVERSETS
	{
		map <CString, sSETS> aSet;
		bool flag; 
	};

	struct strEventTimer
	{
		int		iTimer;
		int		iIndex;
		char	cMode;
	};

	struct STRLIST
	{
		CString	sStatus;
		CString	sDate;
		CString	sTime;
		CString	sDept;
		CString	sName;
		int			iIdx;
		char		cMode;
	};

	struct STRHOL
	{
		int		holid;
		CString ctDate;
	};

	struct STRWORK
	{
		int		camera_idx;
		CString csGroupCode;
		CString csWorkFrom;
		CString csWorkTo;
	};

	struct STRPRIV
	{
		int		camera_idx;
		CString csGroupCode;
		CString cspriv;
	};

	struct STRCHAIN		//for access chain
		{
			int	nid;
			int	groupid;
			CString	Desc;
			CString	mapfile;
		};

			struct STRGATE
		{
			int    chainid;
			int	acid;
			int	camgroupid;
			int    nid;
			int	camidx;
			CString	csBrand;
			int	_timer;
			char	csecurity;
			char	cMode;   //D:DeActive; A:Active;
		};

	bool	ib_dirty;
	bool	m_bDragging;
	bool	mb_AC;
	char	mc_folderMode;
	char	mc_viewMode; //Map or Matrix; default map mode

	int		m_groupid;
	int		il_posx, mi_orgwidth, mi_orgheight;
	int		m_maxMapW, m_maxMapH;
	int		m_nDragIndex ;
	int		m_currGCamid;
	int		m_nDropIndex;
	CString		m_Folder;

	CSplitterControl	m_lsplitv, m_splitV;
	CSplitterControl	m_splitTopH, m_splitBotH, m_splitH;

	CDatabase*		pDB;

	// Drag Stuff define here
	CListCtrl*			m_pDragList;		//Which ListCtrl we are dragging FROM
	CPictureCtrl*		m_pDropPic;		//Which ListCtrl we are dropping ON
	CImageList*		m_pDragImage;	//For creating and managing the drag-image
	CWnd*				m_pDropWnd;		//Pointer to window we are dropping on (will be cast to CListCtrl* type)
	CPictureCtrl*		m_pDropPC;
	CString				m_dragTxt;

	void fnInitControl(void);
	virtual BOOL OnInitDialog();
 
	CBitmapButton		m_butset, m_butsch, m_butrep, m_butpbk;	
	CBitmapButton*		mp_butRefresh;
 
	CComboBox m_comboGroup;
	CEdit m_editFolder;
	CEdit m_editMap;
	
	CButton m_checkRec;

	CListCtrl m_listCam;
	CListCtrl m_listAc;

	CStatic m_stac;
	CPictureCtrl	m_map1;
	CToolTipCtrl*  m_pToolTipCtrl;  

	//void fnPopulateCam(void);
	bool fnDisplayCam(int oGroupid);
	strEventTimer		m_EventTimer; 

protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	CImageList			m_SmallImg;

public:
		//Static Function
		CIniReader		m_ini;
		static UINT _stdcall ReceiveAccess(LPVOID lpParam);
		static UINT _stdcall SecureThread(LPVOID lpParam);
  
		HANDLE	m_hAcThread;

		//Functions
		void DoResizeH(WPARAM wParam, int delta);
		void fnInitGroup(void);
		bool fnGroupChanged(void);
		void fnDeleteAllCam(void);
		 void fnTooltip(void);	
		 void fnPopulateCam(int o_groupid);
		 bool fnFileExist(CString o_file);
		 void fnMaximizeBmp(int &o_maxW, int &o_maxH);
		 bool fnInitFolder(void);
		 bool fnAfterMove(int o_newx, int o_newy, int o_camgroupid, int o_camidx);
		void fnRemoveCam(int o_camidx);
		void fnInitAccess(void);
		void fnStartAccessService();
		void fnDisplayVideo(int o_camidx);
		bool fnLoadCamInfo(void);
		bool fnLoadCamInfo(int oCamidx);

		void fnStartupDisplayVideo();
		void fnAfterVideoClose(int o_camidx);
		void fnCntconnect(CString oCamidx);
		bool fnVideoDoor(int o_camidx);

		//vector
		 //vector <STRGROUP>  m_pvtGroup;
		 vector<CBaseStatic*> m_vpName;
		 vector <STRAC> m_pvtAc;
		 vector <CameraInfo*> m_vcCamInfo;
		 vector <NODEITEM*> m_vctpAllCam;		
		 vector <CEventVideo*> m_vpEventVideo;

	 	vector<strEventTimer> m_vpEventTimer;
		vector <STRLIST>  m_vpList;
		vector <STRHOL>  m_vtHol;
		vector <STRWORK>  m_vtWork;
		vector <STRPRIV>  m_vtPriv;

		vector <STRGROUP>  m_vtGroup;
		vector <STRCHAIN> m_vtChain;
		vector <STRGATE>  m_vtGate;
		vector <STRGATE>  m_vtGateCam;

	 bool fnBuildList(void);
	 afx_msg void OnBnClickedButset();
	 afx_msg void OnBnClickedButrep();

	 virtual BOOL PreTranslateMessage(MSG* pMsg);

	 afx_msg void OnBnClickedButbrowse();
	 afx_msg void OnBnClickedButmap();
	 afx_msg void OnBnClickedButRefresh();
	 afx_msg void OnCbnSelchangeCombogroup();
	// afx_msg void OnLvnBegindragListcam(NMHDR *pNMHDR, LRESULT *pResult);
	 afx_msg void OnLvnBegindragListcam(NMHDR *pNMHDR, LRESULT *pResult);
	 afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	// afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	 afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg void OnTimer(UINT_PTR nIDEvent);


	 bool fnDropWithin(LPPOINT mpt);
	 bool fnCheckMapExist(void);
	 bool fnCheckCamExist(int o_camGroupid, int o_camidx);
	 // Build ecac_camera into STRAC & vector
	 void fnBuildAc(void);
	 void fnSaveNewLoc(int oGroupid, int oCamidx, int posx, int o_posy);
	 bool fnDisplayCamAc(int oGroupid);

	void fnAccessTimer(void);	
	void fnAccessTimer(int oTimerid);
 
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	bool fnModifyAccess(LPVOID param, CString oData, CString oCamIdx);
	bool fnModifyAccess(LPVOID param, CString oData, CString oCamIdx, CString oBrand);

	void fnAccessInsertDB(CString orType, CString oDate, CString oTime, CString oID, int oAccessID, int oCamIdx);
	bool fnInsertRow(LPVOID param, CString oStatus, CString oDate, CString oTime, CString oCamidx, CString oDept, CString oName, CString oGCode, CString oCard, CString oCamname, CString oGroup);
	bool fnCamWithinGroup(int oGroupid, int oCamidx);
	void fnFreeMemory(bool obClose);
	bool fnCloseVideo(void);
	bool fnLoadBmp(CString o_file);

	afx_msg void OnClose();
	CBitmapButton m_butBrowse;
 
	CBitmapButton m_butMap;
	afx_msg void OnBnClickedButsearch();
	CString fnHexToDec(CString oSource);
	//bool fnClearRec(void);
	bool fnClearRec(int oCamIdx);
	// cgi command to open relay door - for soyal
	void fnOpenDoor(int oIdx);
	void fnResumeCam(int o_camidx);
	CString fnGetBrand(int o_camidx);
	void fnTimertoVector(int oCamIdx, CString oBrand, int oTimer);
	// Force event recording if defined in ec.ini
	void fnForceRecording(void);
	void fnStartAccessProcess(int oCamIdx);
	// Stop Access Process
	void fnStopAccessProcess(int oCamIdx);

	UINT CDlgAc::fnReceiveNotifyStream(LPVOID lpParam);

	CImageList	m_smImg;
	
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	void fnSetVideoFlag(int oIndex, bool oFlag);
	int	m_timerIdx; //Timer as camera index - for video display
	int	m_logitem;
	int	m_currecordid;		//record id for record log

	bool	mb_startup;	
	bool	mb_toglue;
	bool	mb_winMin;	//Keep window minimized
	bool	mb_filter;
	CString	is_glue;

	void fnInsertCamera(int oCamidx);
	void fnInitLocalize(void);
	bool fnPegasusDecode(CString& oData, CString& oID, CString& oyy, CString& oMM, CString& odd, CString& ohh, CString& omm, CString& oss);
	afx_msg void OnNMRClickListac(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnModifyEmployee();
	
	void fnInsertList(void);
	// Build vector
	int fnIntoVector(CString oStatus, CString oDate, CString oTime, CString oCamidx, CString oDept, CString oName);
	// check security
	bool fnSecurity(int o_idx);
	bool fnSetCurrentTime(int oCamidx);
	CString fnToHex(int oDec);
	//void fnSetTimer(int oCamidx);

	void fnReSecurity(void);
	bool fnCheckAuthorize(int oCamidx);
	bool fnTimeWithin(CString oCamidx, CString oDate, CString oTime, CString oDept);
	bool fnCheckPriv(int oCamidx, CString oDeptCode);

	void fnOpenSetting(bool oNormal);
	void fnRefreshHolVec(void);
	void fnRefreshPriVec(void);
	void fnInitChain(void);

		//Event Receive
 		CCriticalSection m_CS;
		CEventReceive* m_EmapEventRec;
		HANDLE hThreadEventRec;
		bool fnEventDisplay(CString oIndex, CString osIP, CString osEventType, CString osTime);
		bool fnInsertRowPanic( CString oDate, CString oTime, CString oCamidx, int oParklot, CString oPark);

		static UINT _stdcall GetEventFromService(LPVOID lpParam);
		static void fnEmapCallBack(EventInfo *m_EventInfo, void *lpParam);
		void fnInsertDB(CString oStatus, CString oDate, CString oTime, CString oCardCode, int oCardid, int oCamIdx, CString oName, CString oPlate, int oParklotid, CString oPark);

		CStatic m_gbDetail;
 
		CBitmapButton m_butSwitch;
		CBitmapButton m_butOpen;
		CBitmapButton m_butRed;

		BOOL fnInitControlDetail();
		void fnBuildCamGroup(void);
		void fnDetails(int oRecordid);

		afx_msg void OnLvnItemchangedListac(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnBnClickedButopen();
		afx_msg void OnBnClickedButred();
		afx_msg void OnBnClickedCheckwarn();
		CButton m_ckAlarm;
		// From camera index to get camera name and group name from ec_camera_group_camera table
		bool fnGetcamgroupname(int oCamidx, CString& oCamname, CString& oGroupname);
		afx_msg void OnBnClickedButswitch();
 
		CListCtrl m_listchain;
		CImageList		m_chainimg;

		//Chain
		int	m_chainid, m_preitem;
		int	m_chainw;
		long nScreenW, nScreenH;
		int	m_rpanelx;	//Right panel
		CRect		m_maprcWnd;

		void fnChainChanged(int oChainid);
		void fnGateCam(void);
		int fnGetMinChainid(void);
		//bool fnIscontroller(int oCamidx);
		void fnCleanupThread(void);

		afx_msg void OnLvnItemchangedListchain(NMHDR *pNMHDR, LRESULT *pResult);
		bool fnCheckiscontrol(CString oCamidx);
		afx_msg void OnSize(UINT nType, int cx, int cy);
};
