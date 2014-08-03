#pragma once
#include "resource.h"
#include "BaseDialog.h"
#include "SplitterControl.h"
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
#include "DlgSettingAc.h"
#include "DlgAcSearch.h"
#include "BtnST.h"
#include "DlgSettingGate.h"
#include "EventReceive.h"

// CDlgLPR dialog
//#define DODOFOLDER _T("\\\\USER-THINK-X200\\SnapShotJPG\\dodo\\")
#define DODOFOLDER _T("c:\\LPR\\dodo\\")
#define DODOFOLDER1 _T("c:\\LPR\\dodo1\\")
#define	FILETIMER 5000

class CDlgLPR : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgLPR)

public:
	CDlgLPR(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgLPR();

// Dialog Data
	enum { IDD = IDD_DLGLPR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//void CDlgLPR::fnInitControl(void);


/*========================================================*/
	public:
		CToolTipCtrl*	m_pToolTipCtrl;
		CRect		m_rect;
		CBitmapButton 	m_butset, m_butsch, m_butrep, m_butBrowse, m_butMap, m_butOpengate;	
		//CBitmapButton		m_butstatus;
		afx_msg void OnBnClickedButbrowse();
		afx_msg void OnBnClickedButmap();

	
		//CBitmapButton m_butSet;

		CSplitterControl	m_LsplitV, m_splitV, m_splitTopH, m_splitH, m_splitBotH;
		int		m_maxMapW, m_maxMapH;
		int		m_groupid;
		int		m_chainid;
		int		m_botHPos;
		bool	ib_dirty;
		bool	m_bDragging;
		bool	mb_AC;
		char	mc_folderMode;
		char	mc_viewMode; //Map or Matrix; default map mode

		int		il_posx, mi_orgwidth, mi_orgheight;
		int		m_nDragIndex ;
		int		m_currGCamid;
		int		m_nDropIndex;
		CString		m_Folder;
 
		CDatabase*		pDB;

		CComboBox		m_comboGroup;
		CEdit		m_editFolder, m_editMap;
		CPictureCtrl	m_map;
		CListCtrl		m_listCam; //, m_listGate;
			
		//Functions
		virtual BOOL OnInitDialog();
		void fnInitControl(void);		
		void fnBuildCamGroup();
		bool fnFileExist(CString o_file);
		void fnMaximizeBmp(int &o_maxW, int &o_maxH);
		bool fnInitFolder(void);
		void fnBuildGate(void);
		bool fnCheckMapExist(void);

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

		struct STRGATE
		{
			int    chainid;
			int	parklotid;
			int	camgroupid;
			int    nid;
			int	camidx;
			CString	csBrand;
			int	_timer;
			char	csecurity;
			char	cMode;   //D:DeActive; A:Active;
		};

		struct STRCHAIN		//for parking chain
		{
			int	nid;
			int	groupid;
			CString	Desc;
			CString	mapfile;
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
				CString csFolder;
		};

		struct TIMESTAMP_STRUCT {
			   SQLSMALLINT year;
			   SQLUSMALLINT month;
			   SQLUSMALLINT day;
			   SQLUSMALLINT hour;
			   SQLUSMALLINT minute;
			   SQLUSMALLINT second;
			   SQLUINTEGER fraction;
		} TIMESTAMP_STRUCT;

		struct STRLPRJPG {
			int			camidx;
			CString	filename;
		}LPRJPG;

	struct ENGINE_PARM {
			int	camera_idx;
			int	contrast_level;
			int	max_char;
			int	min_char;
			int	dangle;
			int	rangle;
		} ;

		//Vectors
		 vector<CBaseStatic*> m_vpName;
		vector <STRCHAIN> m_vtChain;
		vector <STRGATE>  m_vtGate;
		vector <STRGATE>  m_vtGateCam;
		vector <STRGROUP>  m_vtGroup;
		vector <NODEITEM*> m_vctpAllCam;
		vector <CEventVideo*> m_vpEventVideo;
		vector <CameraInfo*> m_vcCamInfo;
		vector <STRLPRJPG> m_vcLPRjpg;
		vector	<ENGINE_PARM*> m_vcENGINE;

		afx_msg void OnCbnSelchangeCombogroup();
		
		//bool fnGroupChanged(void);		
		bool fnGroupChanged();

		void fnPopulateCam(int o_groupid);
		bool fnBuildList(void);
		bool fnCheckCamExist(int o_camGroupid, int o_camidx);
		bool fnCheckCamActive( int o_camidx);

		void fnRemoveCam(int o_camidx);
		void fnAfterVideoClose(int o_camidx);
		void		fnOpenDoor(int o_idx);
		bool	fnCloseVideo(void);
		void		fnTimertoVector(int oCamIdx, CString oBrand, int oTimer);

		//bool	fnCheckCamExist(int oCamidx);
		void		fnGateTimer(int oTimerid);
public:
		static int notifyWriter(char* data, size_t size, size_t nmemb,void* lParam);
		//static int SnapshotNotifyWriter(char* data, size_t size, size_t nmemb,void *lParam);

		protected:
		//virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		CImageList			m_SmallImg;
		CImageList			m_smImg;
		CImageList			m_chainimg;

public:
		CToolTipCtrl*	m_pToolTipCtr;
		//afx_msg void OnLvnBeginrdragListcam(NMHDR *pNMHDR, LRESULT *pResult);
		CIniReader		m_ini;
		char					mc_view;		//view mode; M:map L:Live view
		HANDLE			m_hGateThread;
		static UINT _stdcall ReceiveGate(LPVOID lpParam);
		static UINT _stdcall ReceiveLPR(LPVOID lpParam);

		// Drag Stuff define here
		CListCtrl*			m_pDragList;		//Which ListCtrl we are dragging FROM
		CPictureCtrl*		m_pDropPic;		//Which ListCtrl we are dropping ON
		CImageList*		m_pDragImage;	//For creating and managing the drag-image
		CWnd*				m_pDropWnd;		//Pointer to window we are dropping on (will be cast to CListCtrl* type)
		CPictureCtrl*		m_pDropPC;
		CString				m_dragTxt;
		CButtonST *		btncloseChain;
		CStatic				m_stac;

		bool fnLoadBmp(CString o_file);
		bool fnDropWithin(LPPOINT mpt);
		bool fnLoadCamInfo(void);
		bool fnLoadCamInfo(int oCamidx);
		bool fnDisplayCamGate(int oGroupid);
		void fnDeleteAllCam(void);

		CListCtrl m_listChain;
 
		// Initialize Parking Chain and list into m_listChain
		void fnInitChain(void);
		void fnInitGroup(void);
		void fnSaveNewLoc(int oGroupid, int oCamidx, int posx, int o_posy);
		void fnInsertCamera(int oCamidx);
		void fnDisplayVideo(int o_camidx);
		void fnStartupDisplayVideo();
		void fnStartGateService();

		bool fnCamWithinGroup(int oGroupid, int oCamidx);

		afx_msg void OnLvnItemchangedListchain(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnBnClickedClose();	
		//afx_msg void OnBnDblClickedMap();	
		afx_msg void OnBnDoubleClicked();

		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		void fnChainChanged(int oChainid);
		afx_msg void OnNMDblclkGlistcam(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnLvnBegindragGlistcam(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnBnDoubleclicked();

		virtual BOOL PreTranslateMessage(MSG* pMsg);
		void fnHideChain(bool ob_toHide);
		void DoResizeH(int delta);
		void DoResizeH(WPARAM wParam, int delta);

		//void fnSaveNewLoc(int oGroupid, int oCamidx, int posx, int o_posy);
		bool fnAfterMove(int o_newx, int o_newy, int o_camgroupid, int o_camidx);
		bool fnModifyAccess(LPVOID param, CString oData, CString oCamIdx);
		bool fnInsertRow(LPVOID param, CString oStatus, CString oDate, CString oTime, CString oCamidx, CString oDept, CString oName, CString oGCode, CString* opRet);
		void fnInsertDB(CString oStatus, CString oDate, CString oTime, CString oCardCode, int oCardid, int oCamIdx, CString oName, CString oPlate, int oParklotid, CString oPark);
		bool fnClearRec(int oCamIdx);

		CString fnHexToDec(CString oSource);

	protected:
		virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		int	m_preitem;
		bool	mb_toglue;
		CString	is_glue;


public:
		CStatic m_staclog;
		bool	mb_filter;
		int	m_timerIdx; //Timer as camera index - for video display

		// Build Gate Cameras with attached, used for threaex
		void fnGateCam(void);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		CStatic m_stGateInfo;
		CStatic m_stStatus;
		CEdit m_editStatus;
		CStatic m_gbDetail;
//		afx_msg void OnLvnItemchangedListac(NMHDR *pNMHDR, LRESULT *pResult);		
		void fnFreeMemory();
		int m_nChainId;

		void fnDetails(int oRecordid);
		void fnOpenSetting(bool oNormal);
		CEdit m_editTime;
		CEdit m_editDate;

		afx_msg void OnClose();
		afx_msg void OnBnClickedButset();
		afx_msg void OnBnClickedButChangeP();
		afx_msg void OnBnClickedButsearch();
		afx_msg void OnBnClickedButrep();
		afx_msg void OnNMClickListac(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnLvnItemchangedLgate(NMHDR *pNMHDR, LRESULT *pResult);

		afx_msg void OnBnClickedButstatusrmbutstatus();
		afx_msg void OnBnClickedButstatusr();

		CListCtrl m_listgate;

		CBitmapButton		m_butstatusr, m_butstatusg, m_butNext;
		int	m_logitem, m_currecordid;
		// Check if date value valid 
		bool fnDataValid(CString oyy, CString omm, CString odd, CString ohh, CString omin, CString oss);
		int fnGetMinChainid(void);
		void fnInitLocalize(void);
		
		CButton m_ckAlarm;
		afx_msg void OnBnClickedCkfilter();
		afx_msg void OnBnClickedButgoto();

		//Event Receive
 		CCriticalSection m_CS;
		CEventReceive* m_EmapEventRec;
		HANDLE hThreadEventRec;
		bool fnEventDisplay(CString oIndex, CString osIP, CString osEventType, CString osTime);
		bool fnInsertRowPanic( CString oDate, CString oTime, CString oCamidx, int oParklot, CString oPark);

		static UINT _stdcall GetEventFromService(LPVOID lpParam);
		static void fnEmapCallBack(EventInfo *m_EventInfo, void *lpParam);


		void fnCleanupThread(void);
		// check if the camera attached Gate Controller
		bool fnIscontroller(int oCamidx);
		void fnCntconnect(CString oCamidx);
		void fnInitLPR(void);
		// Load Selected Image
		void fnLoadImg(CString ofile);
		CPictureCtrl m_pic;

		//===============  LPR
		bool fnANPRFile(  CString oFile);

		int	m_lprcnt;
		CString m_lprFolder, m_lprFolder1;
		CString m_folder[2];
		void fnLPRfeednext(void);
		CBitmapButton m_butPrv;
		afx_msg void OnBnClickedButprv();
		bool fnInsertRowLPR( CString oStatus,  CString oCamidx, CString oParklot, CString oCardcode, CString oName );

		void fnStartLPRService();
		size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) ;
		//UINT  GetSnapshotFromCGI(LPVOID lpParam);
		//static int SnapshotNotifyWriter(char* data, size_t size, size_t nmemb,void *lParam);
		static UINT  GetSnapshotFromCGI(LPVOID lpParam);
		void fnLPRBeginANPR(void);
		void fnUpdateicon(void);
		void fnLPRDetail(int o_camidx);

		void fnLPRLoadlist(void);
		CBitmapButton m_butplay;
		CBitmapButton m_butstop;
		afx_msg void OnBnClickedButplay();
		afx_msg void OnBnClickedButstop();
		afx_msg void OnBnClickedButplaybk();
		void fnLPRSnapShot();
		int fnLPRgetidx(CString oFile);
		CStatic m_notify;
		void fnReleaseLPR(void);
		// Manually change LPR
		void fnLPRChange(void);
		bool fnLPRCompare(CString oOrgplate, CString oNewplate);
		int	m_filetimer;

		//For Demo
		void fnDemoPopulateCam();
		bool fnDemoDisplayCamGate();
		bool fnLPRLoopCompare(CString oOrgplate, CString oNewplate);
		bool	mb_demo;
};
