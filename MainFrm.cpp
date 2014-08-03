// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "WorkSpaceToolBar.h"
#include "MainFrm.h"
#include "MSMVisualManager.h"
#include "NVR.h"
#include "EtroExplorerDoc.h"
#include "EtroExplorerView.h"
#include "EventLogView.h"
#include <math.h>
#include "DlgFlyAway.h"
#include "Winsvc.h"
#include "DlgUserLogin.h"
#include "DlgAddManageCam.h"
#include "DlgMassAssignIP.h"
#include "DlgSettingCamGroup.h"
#include "DlgSettingRecording.h"
#include "DlgSettingMulMonitor.h"
#include "DlgFtp.h"
#include "IdeDiskInfo.h"
#include "DlgAppTag.h"
#include "EnTabCtrl.h"
#include "AddDevice.h"
#include "DlgExitGate.h"
#include "OSFacade.h"
#include "VirtualKeyboard.h"
enum enumStartUpType{MAIN=0,START_LIVEVIEW,START_EMAP,START_EVENTMONITOR,START_PLAYBACK,START_ACCESSCONTROL,START_GATECONTROL};
//enum enumServiceType{STREAM=0,RECORD,EVENT,HEALTH,REMOTE};


#define numOfService 5 // SERVICE+1
#define MYWM_NOTIFYICON					(WM_APP + 1111)	

#define TIMER_DOCKWND_REPAINT		1

//#include "EtroDB.h"

//--Dll Function Type
typedef int (iMakeSureRootExist)(CString &csRegPath);
const UINT  MsgDeleteFlyAway = RegisterWindowMessage(_T("MsgDeleteFlyAway") );
 

#define SYSINFOH 120
bool mb_emapExist = false;
bool mb_acExist = false;
bool mb_gateExist = false;
bool mb_lprExist = false;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)


BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_RESETTOOLBAR, OnToolbarReset)
	ON_REGISTERED_MESSAGE(AFX_WM_TOOLBARMENU, OnToolbarContextMenu)
    ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_COMMAND(IDC_LIVE_VIEW, OnLiveView)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_COMMAND(ID_FIRMWARE_UPGRADE, OnFirmwareUpgrade)
	ON_COMMAND(ID_SET_NETWORK, OnSetNetwork)
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, OnToolbarDropDown)
	ON_MESSAGE(MYWM_NOTIFYICON, OnDockingIcon)
	ON_WM_COPYDATA()
	ON_COMMAND(IDR_MENU_NOTIFY_EXIT, &CMainFrame::OnMenuNotifyExit)
	ON_COMMAND(ID_RECORD_CLOSE, &CMainFrame::OnRecordClose)
	ON_COMMAND(ID_RECORD_RESTART, &CMainFrame::OnRecordRestart)
	ON_COMMAND(ID_EVENT_RESTART, &CMainFrame::OnEventRestart)
	ON_COMMAND(ID_EVENT_CLOSE, &CMainFrame::OnEventClose)
	ON_COMMAND(ID_STREAM_RESTART, &CMainFrame::OnStreamRestart)
	ON_COMMAND(ID_STREAM_CLOSE, &CMainFrame::OnStreamClose)
	ON_COMMAND(ID_HEALTH_RESTART, &CMainFrame::OnHealthRestart)
	ON_COMMAND(ID_HEALTH_CLOSE, &CMainFrame::OnHealthClose)
    ON_COMMAND(ID_MENU_RELOGIN, &CMainFrame::OnMenuReLogin)
	ON_COMMAND(ID_REMOTE_RESTART, &CMainFrame::OnRemoteRestart)
	ON_COMMAND(ID_REMOTE_CLOSE, &CMainFrame::OnRemoteClose)
    ON_COMMAND(ID_SYSTEM_SETTING, &CMainFrame::OnSystemSetting)
    ON_COMMAND(SC_MAXIMIZE, CMainFrame::OnMax)
    ON_COMMAND(SC_RESTORE, CMainFrame::OnMax)
    ON_COMMAND(SC_MINIMIZE, CMainFrame::OnMin) 
    ON_COMMAND(ID_MENU_FULLSCREEN,  CMainFrame::RunFullScreen) 
	ON_COMMAND_RANGE(ID_TOOLBAR_DATETIME, ID_TOOLBAR_PLAYBACK, &CMainFrame::OnRangeCmds)
	ON_WM_SYSCOMMAND()
	ON_REGISTERED_MESSAGE(MsgDeleteFlyAway, OnDeleteFlyAway)
	ON_WM_TIMER()
    ON_WM_SETCURSOR() 
	ON_COMMAND(ID_JOYSTICK, &CMainFrame::OnJoystick)
END_MESSAGE_MAP()
	

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

void CMainFrame::OnMax()
{
    ShowWindow(SW_MAXIMIZE);
}
void CMainFrame::OnMin()
{
    ShowWindow(SW_MINIMIZE);
}
BOOL CMainFrame::bCustomizeModeCheck()
{
    BOOL bAero(false);
    // dwOSVersionInfoSize must big one byte of sizeof struct on 64bits OS, 
    // other wise cause a "Run-Time Check Failure #2 - Stack around the variable 'osver' was corrupted." eroor 
    // why ??? It's must Microsoft bug, I don't know why.---- by javan
    char szBuffer[sizeof(OSVERSIONINFOEX)+1]={};
    OSVERSIONINFO& osver= *(OSVERSIONINFO*)&szBuffer;//{};
    osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);// -1; 
    BOOL bRet = ::GetVersionEx(&osver);
    if (osver.dwMajorVersion >= 6)
    {
        HMODULE hModule = ::LoadLibrary(_T("Dwmapi.dll"));
        if (hModule)
        {
            typedef HRESULT  (WINAPI* DWMISCOMPOSITIONENABLED)(BOOL*);
            DWMISCOMPOSITIONENABLED func;
            func = (DWMISCOMPOSITIONENABLED)GetProcAddress(hModule, "DwmIsCompositionEnabled"); 
            if (func)
            {
                func(&bAero);
            }
        } 
    }
    return bAero;
}
void CMainFrame::vSetWindowStyle()
{
    if (bCustomizeModeCheck())
    {
        ModifyStyle (WS_CAPTION| FWS_ADDTOTITLE, 0); //
        ModifyStyleEx (WS_EX_CLIENTEDGE, 0);
        ::SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE)|WS_EX_TOPMOST);
        m_wndCaptionBar.m_bCustomizeMode = true;
    }
	else
    {
        m_wndCaptionBar.m_bCustomizeMode = false;
    }
	CMFCVisualManager::SetDefaultManager (RUNTIME_CLASS (CMSMVisualManager));
    
	//---------------------------------
	// Set toolbar and menu image size:
	//---------------------------------
	CMFCToolBarButton::m_bWrapText = FALSE;
	CMFCToolBar::SetMenuSizes (CSize (22, 22), CSize (16, 16));

	CMFCToolBarComboBoxButton::SetFlatMode ();
	CMFCToolBarComboBoxButton::SetCenterVert ();

	if (!m_wndCaptionBar.Create(this, IDR_MAINFRAME))
	{
		TRACE0("Failed to create captionbar\n");
		return ;
	}

	if (afxGlobalData.fontRegular.GetSafeHandle () != NULL)
	{
		LOGFONT lf;
		ZeroMemory (&lf, sizeof (LOGFONT));
		afxGlobalData.fontRegular.GetLogFont (&lf);

		m_wndCaptionBar.SetCaptionFont (lf);
	}

	CClientDC dc (this);
	m_bIsHighColor = true;//dc.GetDeviceCaps (BITSPIXEL) > 16;

	UINT uiToolbarHotID = 0, uiToolbarColdID = 0, uiToolbarDisID = 0, uiToolbarPresID = 0, uiMenuID = 0;
	GetToolbarID( uiToolbarHotID,  uiToolbarColdID,  uiToolbarDisID,  uiToolbarPresID,  uiMenuID);

	m_wndToolBar.SetAuthorize(m_iAuth);
	if (!m_wndToolBar.CreateEx (this, TBSTYLE_FLAT,
		AFX_DEFAULT_TOOLBAR_STYLE, CRect(0, 0, 0, 0)) ||
		!m_wndToolBar.LoadToolBar (IDR_MAINFRAME, uiToolbarColdID, uiMenuID, 
		FALSE /* Not locked */, uiToolbarDisID, 0,  uiToolbarHotID, uiToolbarPresID))
	{
		TRACE0("Failed to create links bar\n");
	}
	DockPane (&m_wndToolBar);
	EnableDocking(CBRS_ALIGN_ANY);
}
void CMainFrame::GetToolbarID(UINT& uiToolbarHotID, UINT& uiToolbarColdID, UINT& uiToolbarDisID, UINT& uiToolbarPresID, UINT& uiMenuID)
{
	bool bNVR = m_pbk->isNVRType();
	if (m_bIsHighColor)
	{
		if (bNVR)
		{
			uiToolbarHotID = IDB_HOTTOOLBAR_NVR;
			uiToolbarColdID = IDB_COLDTOOLBAR_NVR;
			uiToolbarDisID = IDB_DISABLEDTOOLBAR_NVR;
			uiToolbarPresID = IDB_PRESSEDTOOLBAR_NVR;
		} 
		else
		{
			uiToolbarHotID = IDB_HOTTOOLBAR_CMS;
			uiToolbarColdID = IDB_COLDTOOLBAR_CMS;
			uiToolbarDisID = IDB_DISABLEDTOOLBAR_CMS;
			uiToolbarPresID = IDB_PRESSEDTOOLBAR_CMS;
		}
		uiMenuID = IDB_MENU_IMAGES;
	}
	else
	{
		uiMenuID = IDB_MENU_IMAGES_16;
	}
}

int CMainFrame::OnPreCreate()
{
    vSetWindowStyle();
	if (!m_wndMenuBar.Create (this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}
    m_wndMenuBar.ShowWindow(SW_HIDE);

	BOOL bValidString;
	CString strMainToolbarTitle;
	bValidString = strMainToolbarTitle.LoadString (IDS_MAIN_TOOLBAR);
	
	EnableDocking(CBRS_ALIGN_ANY);
	EnableWindowsDialog (ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);
	m_dwMenuBarVisibility=AFX_MBS_HIDDEN;
    m_dwMenuBarState = AFX_MBS_HIDDEN;
    CWnd::SetMenu(NULL);
	CreateTaskPane();

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnViewCustomize()
{
	//------------------------------------
	// Create a customize toolbars dialog:
	//------------------------------------
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog (this,
		TRUE /* Automatic menus scaning */
		);

	pDlgCust->Create ();
}

LRESULT CMainFrame::OnToolbarContextMenu(WPARAM,LPARAM lp)
{

	return 0;
}

afx_msg LRESULT CMainFrame::OnToolbarReset(WPARAM /*wp*/,LPARAM)
{
	// TODO: reset toolbar with id = (UINT) wp to its initial state:
	//
	// UINT uiToolBarId = (UINT) wp;
	// if (uiToolBarId == IDR_MAINFRAME)
	// {
	//		do something with m_wndToolBar
	// }

	return 0;
}

BOOL CMainFrame::OnShowPopupMenu (CMFCPopupMenu* pMenuPopup)
{
	//---------------------------------------------------------
	// Replace ID_VIEW_TOOLBARS menu item to the toolbars list:
	//---------------------------------------------------------
	//CMDIFrameWnd::OnShowPopupMenu (pMenuPopup);

	//if (pMenuPopup != NULL &&
	//	pMenuPopup->GetMenuBar ()->CommandToIndex (ID_VIEW_TOOLBARS) >= 0)
	//{
	//	if (CMFCToolBar::IsCustomizeMode ())
	//	{
	//		//----------------------------------------------------
	//		// Don't show toolbars list in the cuztomization mode!
	//		//----------------------------------------------------
	//		return FALSE;
	//	}

	//	pMenuPopup->RemoveAllItems ();

	//	CMenu menu;
	//	VERIFY(menu.LoadMenu (IDR_POPUP_TOOLBAR));

	//	CMenu* pPopup = menu.GetSubMenu(0);
	//	ASSERT(pPopup != NULL);

	//	if (pPopup)
	//	{
	//		pMenuPopup->GetMenuBar ()->ImportFromMenu (*pPopup, TRUE);
	//	}
	//}

	return TRUE;
}

void CMainFrame::OnWindowManager() 
{
	ShowWindowsDialog ();
}

void CMainFrame::SetupMemoryBitmapSize (const CSize& sz)
{
	CMSMVisualManager* pManager = DYNAMIC_DOWNCAST (CMSMVisualManager,
		CMFCVisualManager::GetInstance ());

	if (pManager != NULL)
	{
        //m_wndCaptionBar.SetCaptionHeight(0);
		CRect rtRes (0, 0, sz.cx, 0);

		CRect rt;

		CPane* bars[] = 
        {&m_wndCaptionBar, &m_wndToolBar};//, &m_wndCategoryBar}; //&m_wndMenuBar,

		for (long i = 0; i < 2; i++)
		{
			CPane* pBar = bars [i];

			if (pBar != NULL && pBar->GetSafeHwnd () != NULL)
			{
				pBar->GetWindowRect (rt);
				rtRes.bottom += rt.Height ();
			}
		}

		CDC* pDC = GetDC ();

		pManager->SetupMemoryBitmapSize (pDC, rtRes.Size ());

		ReleaseDC (pDC);
	}
}


void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
    SetupMemoryBitmapSize (CSize (cx, cy));

	if (m_wndCaptionBar.GetSafeHwnd () != NULL)
	{
		m_wndCaptionBar.SetParentMaximize (nType == SIZE_MAXIMIZED);
	}

    CMDIFrameWndEx::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
}

void CMainFrame::CreateTreePane(const CRect& rect)
{
	CString csString(_T(""));
	LOADSTRING(csString, IDC_IPCAM);
	int nPage3Gr1 =  m_wndTaskPane.AddGroup ( csString);
	m_pTreeCtrlTab = new CTreeTabCtrl;
	m_pTreeCtrlTab->CreateTreeTab(AFX_CBRS_FLOAT | AFX_CBRS_RESIZE , rect, &m_wndTaskPane, AFX_IDW_CONTROLBAR_FIRST+35);
	m_wndTaskPane.AddWindow (nPage3Gr1, m_pTreeCtrlTab->GetSafeHwnd(), 350);
}

void CMainFrame::CreateMatrixPane()
{
	CString csString(_T(""));
	LOADSTRING(csString, IDC_MATRIX);
	int nPage3Gr1 =  m_wndTaskPane.AddGroup (csString);
	m_wndMatrix.Create(IDD_DLG_MATRIX_TAB, &m_wndTaskPane);
	m_wndTaskPane.AddWindow (nPage3Gr1, m_wndMatrix.GetSafeHwnd (), 210);
	m_wndTaskPane.CollapseGroup(nPage3Gr1,true);
}

void CMainFrame::CreatePTZPane()
{
	int nPage3Gr1 =  m_wndTaskPane.AddGroup ( _T("PTZ"));
	if (!m_PTZ.Create(IDD_DLG_PTZ, &m_wndTaskPane))//, sizeDummy, true, AFX_IDW_CONTROLBAR_FIRST+33, WS_CHILD | WS_VISIBLE | CBRS_RIGHT))
	{
		TRACE0("Failed to System Info\n");
		//return -1;      
	}
	m_wndTaskPane.AddWindow (nPage3Gr1, m_PTZ.GetSafeHwnd (), 210);
	if (m_PTZ)
	{
		m_pbk->RegistWnd(&m_PTZ);
		if (_UseDirectInput)
		{
			m_PTZ.InitJoyStick(GetSafeHwnd());
		}
	}
}

void CMainFrame::CreateEventListPane(const CRect& rect)
{
	CString csString(_T(""));
	LOADSTRING(csString, IDC_EVENT);
	int nPage3Gr1 =  m_wndTaskPane.AddGroup ( csString);
	DWORD dwStyle2 =  LVS_REPORT | WS_BORDER | WS_TABSTOP | LVS_SHOWSELALWAYS |LVS_SINGLESEL;

	if (!m_wndEventList.Create(dwStyle2,rect, &m_wndTaskPane, AFX_IDW_CONTROLBAR_FIRST+34))
	{
		TRACE0("Failed to System Info\n");
		//return -1;      
	}
	m_wndTaskPane.AddWindow (nPage3Gr1, m_wndEventList.GetSafeHwnd (), 350);
}

BOOL CMainFrame::CreateTaskPane()
{
	CRect rectDummy(0, 0, 400, 400);
	if (!m_wndTaskPane.Create(_T("Tasks Pane"), this, rectDummy, false, 32001,
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT, 0))
	{
		TRACE0("Failed to create task pane\n");
		return FALSE;      // fail to create
	}

	m_wndTaskPane.EnableDocking (CBRS_ALIGN_RIGHT | CBRS_ALIGN_LEFT);
	DockPane(&m_wndTaskPane);

	CreateTreePane(rectDummy);
	CreateMatrixPane();
	CreatePTZPane();
	CreateEventListPane(rectDummy);

    m_wndTaskPane.SetTasksHorzOffset(0);
    m_wndTaskPane.SetTasksIconVertOffset(0);
    m_wndTaskPane.SetHorzMargin(0);
    m_wndTaskPane.SetVertMargin(0);
    m_wndTaskPane.SetGroupCaptionHeight(25);
   
	return true;
}

/////////////////////////////////////////////////////////////////////////
///////////// begin merge from etrocenter ///////////////////////////////
/////////////////////////////////////////////////////////////////////////

CMainFrame::CMainFrame():m_bOnFullScreen(false),m_nOriginalStyle(0),m_bDisplayOSD(true),m_nMainStartIdx(0),m_OrgLayoutIdx(1)
,m_bAutoScan(false),m_bAutoScanIPWall(false),m_nAutoScanTime(10),m_bEventLV(false),m_bRestartByHealth(false),m_bIPWallSelected(false)
,m_bWizardEnable(true),m_bStartupFullScreen(false),m_iAuth(0),m_bGroupAutoPlay(false),m_bGroupAutoScanPause(false)
,m_bGroupAutoScanResume(false),m_nCurrentPlayGroupId(-1),m_bDBOpen(true),m_wndTaskPane(this)
,m_emap(NULL),m_ac(NULL),m_pTreeCtrlTab(NULL)

{
	int	li_matrix;

	m_ini.setINIFullFileName(_T("\\ec.ini"));
	
	
	CString csMatrix = m_ini.getKeyValue(_T("matrix"),_T("video"));
	if (csMatrix == _T(""))
	{
		m_OrgLayoutIdx=3;
	}
	else
	{
		m_OrgLayoutIdx = _ttoi(csMatrix);
	}

	CString csAutoScan = m_ini.getKeyValue(_T("AutoScan"),_T("video"));
	if(csAutoScan == _T("1"))
		m_bAutoScan = true;

	CString csGroupAutoScan = m_ini.getKeyValue(_T("GroupAutoScan"),_T("video"));
	if(csGroupAutoScan == _T("1"))
		m_bGroupAutoPlay = true;

	CString csFullScreen = m_ini.getKeyValue(_T("RunFullScreen"),_T("system"));
	if(csFullScreen == _T("1"))
		m_bStartupFullScreen = true;

	CString csWiz;
	csWiz = m_ini.getKeyValue(_T("EnableWizard"),_T("system"));
	if( csWiz == _T("0") )
	{
		m_bWizardEnable = false;		
	}
	CGS::bGetGroupAutoScanIdx(m_vctnGroupAutoPlayIdx);
	
	m_Crypto.DeriveKey(_T("Encrypt!@#$%^&*()_+-~`"));
}

CMainFrame::~CMainFrame()
{
	 m_DB.Close();

	m_vctDlgFlyAwayRecycleBin.clear();
//#ifdef PLUGIN
	if (m_emap != NULL) delete m_emap;
	if (m_ac != NULL) delete m_ac;
//#endif

	SECURITY_DESCRIPTOR secutityDese; 
	::InitializeSecurityDescriptor(&secutityDese, SECURITY_DESCRIPTOR_REVISION); 
	::SetSecurityDescriptorDacl(&secutityDese,TRUE,NULL,FALSE); 

	SECURITY_ATTRIBUTES securityAttr; 

	// set SECURITY_ATTRIBUTES 
	securityAttr.nLength = sizeof SECURITY_ATTRIBUTES; 
	securityAttr.bInheritHandle = FALSE; 
	securityAttr.lpSecurityDescriptor = &secutityDese;

	HANDLE hCloseThisThread = ::CreateEvent(&securityAttr, false, false, ETROCENTER_CLOSE_NAS_THREAD);
	::SetEvent(hCloseThisThread);

}

void CMainFrame::vGetPathInfo()
{
	g_csaSavePath.RemoveAll();
	try
	{
		int iPathID = 1;
		CString csSQL_Path(_T("SELECT * FROM ec_storage WHERE storage_type='RECORD'"));
		CString csPath, csPathID, csBufferSize;
		CRecordset rPath(&m_DB);
		rPath.Open(CRecordset::forwardOnly, csSQL_Path);
		while (!rPath.IsEOF()) 
		{ 
			rPath.GetFieldValue(_T("store_location"), csPath);
			rPath.GetFieldValue(_T("buffer_size"), csBufferSize);

			csPathID.Format(_T("%d"), iPathID);
			csBufferSize.TrimRight();
			g_csaSavePath.Add(csPath);
			iPathID++;

			rPath.MoveNext();
		}
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CMainFrame::CheckCmdline(CString& csAuto, const CString& csUser, const CString& csEnPass)
{
	CWinApp* pApp = AfxGetApp();
	CString str;
	str.Format(_T("%s"),pApp->m_lpCmdLine);

	if (str.Right(1) == "R" && !csUser.IsEmpty() && !csEnPass.IsEmpty())
	{
		csAuto = _T("1");
	}
}

int CMainFrame::nAuthorize()
{

	if(m_bDBOpen)	
		vDatabaseCheck();
	else
    {
        PostMessage(WM_DESTROY,0,0);
        return -1;
		//this->OnDestroy();
    }

	if(this->m_iAuth == CANCEL)
    {
        PostMessage(WM_DESTROY,0,0);
        return -1;
		//this->OnDestroy();
    }

	CRegKey regKey;
	//szCurrentUser={0};
	::ZeroMemory(&szCurrentUser[0], sizeof(szCurrentUser));
	const DWORD len = 1024;
	DWORD nActualLen(len);
    CString csForder(CProductDefine::m_tzRegistSubFolder);

	//p_iMakeSureRootExist(csForder);

	LONG nRet = regKey.Open(HKEY_LOCAL_MACHINE, csForder.GetBuffer());
	if(nRet != ERROR_SUCCESS)
	{
		if( regKey.Create(HKEY_LOCAL_MACHINE, csForder.GetBuffer()) != 0)
		{
			TRACE(_T("Create Registry Fail."));
		}
	}
	
	nRet = regKey.QueryValue(szCurrentUser, _T("USERPWD"), &nActualLen);
	if(nRet != ERROR_SUCCESS)
	{
		//regKey.SetValue(_T("0;root;pass;"), _T("USERPWD"));
		TRACE(_T("Registry Query Fail!\r\n"));
		//return 0;
	}
	regKey.Close();

	CString csAuto, csUser, csEnPass, csPass;

	AfxExtractSubString(csAuto, szCurrentUser, 0, ';');
	AfxExtractSubString(csUser, szCurrentUser, 1, ';');
	AfxExtractSubString(csEnPass, szCurrentUser, 2, ';'); 

	csPass = csDecryptAuthPassword(csEnPass);
	CheckCmdline(csAuto, csUser, csEnPass);

	CDlgUserLogin dlgUserLogin(&m_DB, this);
    dlgUserLogin.vSetOption(CDlgUserLogin::LOGIN);

	int iLoginCount = 0;
	while(iLoginCount < 3)
	{
		if(m_bLoginInfoReady && m_iAuth != LOGIN_UNAUTHORIZED)	break;

		if(!m_bDBOpen)	break;

		if(::_ttoi(csAuto) == 1 && iLoginCount < 1)
		{
			if( dlgUserLogin.iCheck(csUser, csPass) != -1)
			{
				this->m_iAuth = dlgUserLogin.m_iChecksum;
			}
			else
			{
				this->m_iAuth = LOGIN_UNAUTHORIZED;
			}
		}
		else
		{
            dlgUserLogin.bAttach(csUser, csPass);
			if( dlgUserLogin.DoModal() == IDOK)
			{
				this->m_iAuth = dlgUserLogin.m_iChecksum;
				::_tcscpy(szCurrentUser, dlgUserLogin.m_csRegValue.GetBuffer());

			}
			else if(dlgUserLogin.DoModal() == IDCANCEL)
			{
				this->m_iAuth = LOGIN_UNAUTHORIZED;
				//--Shut down
			}
		}

		if( this->m_iAuth != LOGIN_UNAUTHORIZED)	break; 
		iLoginCount++;
		CString csString;
		//csString.LoadString(CGS::g_hResStr, IDS_UNAUTHORIZED);
		LOADSTRING(csString,IDS_UNAUTHORIZED);
		AfxMessageBox(csString);
	}
	if(this->m_iAuth < 0)	
    {
        //this->OnDestroy();	
        if (dlgUserLogin.m_csUserName == _T("admin") && dlgUserLogin.m_csPassword == _T("admin"))
        {
            //Easter egg
        }else
        {
            PostMessage(WM_DESTROY,0,0);
            return -1;
        }
    }

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
	return -1;
	bDBConnect();
    vSetMonitorStartupEvent();
	this->m_bLoginInfoReady = false;
    vGetAppPath();
    HMODULE hResOem = ::LoadLibrary(_T("Manufacturer.DLL"));
    if (hResOem)
    {
        SetIcon(::LoadIcon(hResOem, MAKEINTRESOURCE(IDR_MAINFRAME)), false);
        FreeLibrary(hResOem);
    }
	else
	{
		AfxMessageBox(_T("Invalid Manufacturer.DLL!"));
		PostMessage(WM_DESTROY,0,0);
		return -1;
	}

	if (-1 == nAuthorize())
		return -1;
	
	OnPreCreate();
	SetMenu(NULL);

	SetWindowText(  CProductDefine::m_tzProductName );	 
	m_nOriginalStyle =  GetWindowLong(m_hWnd, GWL_STYLE);
   // ShowWindow(SW_MAXIMIZE);

	vDocking(); 
	vSetMainFrmToMonitor();
    BOOL bResult=::SetForegroundWindow(m_hWnd);
    bResult = ::BringWindowToTop((m_hWnd));

	if(m_bStartupFullScreen)
	{
        m_bOnFullScreen = TRUE;
		OnFullScreen(m_bStartupFullScreen);
	}

	if (_SaveNewOSDItem)
	{
		//enum OSDBUTTONS {CAPTION=0,OSDSNAPSHOT,FIXRATIO,SENDAUDIO,RECVAUDIO,MORE,RECONNECT,DOTRIGGER,INSTANTPLAYBACK,RECVIDEO};//
		CGS::m_vctnOSDBtnConfig.clear();
		CGS::m_vctnOSDBtnConfig.push_back(RECVIDEO);
		CGS::m_vctnOSDBtnConfig.push_back(CAPTION);
		CGS::m_vctnOSDBtnConfig.push_back(FIXRATIO);
		CGS::m_vctnOSDBtnConfig.push_back(RECVAUDIO);
		CGS::m_vctnOSDBtnConfig.push_back(SENDAUDIO);
		CGS::m_vctnOSDBtnConfig.push_back(OSDSNAPSHOT);
		CGS::m_vctnOSDBtnConfig.push_back(MORE);
		CGS::bSetOSDConfigToRegistry();
	}
	else
		CGS::bGetOSDConfigFromRegistry();

	vGetPathInfo();
    CheckRecordPathAvailable();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

void CMainFrame::vVideoPresent(vector <NODEITEM *>pNodeItem)
{
	CView* pView;
    if (MDIGetActive() == NULL) return;
 
	try
	{
		while (pView = MDIGetActive()->GetActiveView())
		{
			if (pView->IsKindOf( RUNTIME_CLASS(CEtroExplorerView)))
			{
				((CEtroExplorerView*)pView)->vAttach(pNodeItem);
				break;
			}
			MDINext();
			MDIMaximize(pView);
		}
			
	}
	catch(exception* e)
	{
		TRACE(_T("Exception - vVideoPresent"));
	}

}
bool CMainFrame::bOutOfRange(int nNodeCount)
{
	CView* pView;
 
	try{

		while (pView = MDIGetActive()->GetActiveView())
		{
			if (pView->IsKindOf( RUNTIME_CLASS(CEtroExplorerView)))
			{
				return ((CEtroExplorerView*)pView)->bIsOutOfRange(nNodeCount);
				break;
			}
			MDINext();
			MDIMaximize(pView);
		}
	}
	catch(exception* e)
	{
		TRACE(_T("Exception - bOutOfRange"));
	}
}



void CMainFrame::vAlocateViewCell(UINT nSize)
{
	CView* pView;
	TRACE(_T(" CMainFrame::vAllocateViewCell - Start"));
    CMDIChildWnd*  pMdiChild = MDIGetActive();
    if (pMdiChild == NULL) return;
	while (pView = MDIGetActive()->GetActiveView())
	{
		if (pView->IsKindOf( RUNTIME_CLASS(CEtroExplorerView)))
		{   
			
			CString ls_matrix;
			ls_matrix.Format(_T("%d"),m_OrgLayoutIdx);
			if(nSize == 1)
			{
				m_ini.setKey(_T("1"), _T("matrix"),_T("video"));
				m_OrgLayoutIdx = 1;
				((CEtroExplorerView*)pView)->vSetGrids(1);
			}
			else
			{
				m_ini.setKey(ls_matrix, _T("matrix"),_T("video"));
				((CEtroExplorerView*)pView)->vSetGrids(m_OrgLayoutIdx);
			}
			break;
		}
		MDINext();
	}

	TRACE(_T(" CMainFrame::vAlocateViewCell - End "));
	return;
}//CMainFrame::vAlocateViewCell
void CMainFrame::vGetCurrentAllNodeItems(vector<NODEITEM*> &vctpNode)
{
	CView* pView;
	while (pView = MDIGetActive()->GetActiveView())
	{
		if (pView->IsKindOf( RUNTIME_CLASS(CEtroExplorerView)))
		{   			
			((CEtroExplorerView*)pView)->vGetCurrentGroupNodeItems(vctpNode);
			break;
		}
		MDINext();
	}	
	return;
}

void CMainFrame::OnLiveView() 
{
	GetTreeCtrlType()->vLiveView();
}

void CMainFrame::RunLiveView() 
{
	 OnLiveView();
}

void CMainFrame::RunFullScreen()
{
	m_bOnFullScreen ^= 1;
    MDIGetActive()->GetActiveView()->ShowWindow(SW_HIDE);
	OnFullScreen(m_bOnFullScreen);
    MDIGetActive()->GetActiveView()->ShowWindow(SW_SHOW);
}
//#ifdef PLUGIN
void CMainFrame::OnEmapView(CRect rect)
{	 	
	m_emap = new CDlgEmap;
	m_emap->m_rect = rect;
	m_emap->Create(CDlgEmap::IDD);
	m_emap->SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);
	//m_emap->ShowWindow(SW_SHOWMAXIMIZED);
	mb_emapExist = true;	
}
void CMainFrame::OnEmapView(CRect rect,int nGroupId)
{	 	
	m_emap = new CDlgEmap;
	m_emap->m_rect = rect;
	m_emap->m_nGroupIdx = (nGroupId);
	m_emap->Create(CDlgEmap::IDD);
	m_emap->SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);
	//m_emap->ShowWindow(SW_SHOWMAXIMIZED);
	mb_emapExist = true;	
}

void CMainFrame::fnOpenAc()
{
		CRect rect;

		GetDesktopWindow()->GetWindowRect(&rect);
		m_ac = new CDlgAc;
		m_ac->m_rect = rect;
		m_ac->Create(CDlgAc::IDD);
		m_ac->SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);

		//Localize AC Title
		CString	ls_title;
		LOADSTRING(ls_title,  IDS_AC_TAB_AC );
		m_ac->SetWindowTextW(ls_title);
		mb_acExist = true;	
}
// open ac with specific monitor rectangle
void CMainFrame::fnOpenAc(CRect rect, CRect rect2)
{
		m_ac = new CDlgAc;
		m_ac->m_rect = rect2;
		m_ac->Create(CDlgAc::IDD);
		m_ac->SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);
		mb_acExist = true;	
}

//Open Gate Dialog Window
void CMainFrame::fnOpenGate()
{
		CRect rect;

		GetDesktopWindow()->GetWindowRect(&rect);
		m_gate = new CDlgGate;
		m_gate->m_rect = rect;
		m_gate->Create(CDlgGate::IDD);
		m_gate->SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);

		//Localize Gate Title
		CString	ls_title;
		LOADSTRING(ls_title,  IDS_GATE_CONTROL );
		m_gate->SetWindowTextW(ls_title);

		mb_gateExist = true;	
}

void CMainFrame::fnOpenLPR()
{
		CRect rect;

		GetDesktopWindow()->GetWindowRect(&rect);
		m_lpr = new CDlgLPR;
		m_lpr->m_rect = rect;
		m_lpr->Create(CDlgLPR::IDD);
		m_lpr->SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);

		mb_gateExist = true;	
}
// open gate dialog with specific monitor rect
void CMainFrame::fnOpenGate(CRect rect, int nChainId, CRect rect2)
{
		m_gate = new CDlgGate;
		m_gate->m_rect = rect2;
		m_gate->m_nChainId = nChainId;
		m_gate->Create(CDlgGate::IDD);
		m_gate->SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);

		mb_gateExist = true;	
}

bool CMainFrame::vOpenEmapView()
{
	CRect rect;
	GetDesktopWindow()->GetWindowRect(&rect);
	OnEmapView(rect);

	return false;
}
//#endif


void CMainFrame::vItemChanged(NODEITEM *pNode)
{
	CView* pView;
	pView = MDIGetActive()->GetActiveView();
	::PostMessage(pView->m_hWnd, ID_NODEITEM_CHANGED,NULL, (unsigned int)pNode);
}
void CMainFrame::OnClose()
{
    int nActionCode(IDOK);

	if (m_pbk->isNVRType() && !m_bRestartByHealth)
    {
        CDlgTransparent* pTransdlg= new CDlgTransparent(this) ;
  	    pTransdlg->Create(IDD_SPLASH);
        CRect rect;
        GetClientRect(rect);
        pTransdlg->MoveWindow(0, 0, rect.Width(), rect.Height());
	    pTransdlg->SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);
        CDlgExitGate* pdlg = new CDlgExitGate(this);
        nActionCode = pdlg->DoModal();
        ::SendMessage(pTransdlg->m_hWnd, WM_CLOSE, NULL, NULL); //pTransdlg->m_hWnd, 
        delete pdlg;
        delete pTransdlg;
        if (nActionCode == IDCANCEL) return;
        COSFacade osfacade;
        CDlgUserLogin dlgUserLogin(&m_DB, this);
        if (nActionCode == IDC_LOGOUT)
        {
            dlgUserLogin.vSetOption(CDlgUserLogin::LOGOUT);
        }else
        {
            dlgUserLogin.vSetOption(CDlgUserLogin::RESTART);
        }
        
        dlgUserLogin.DoModal();
        bool bAdminAction(false);
        if( dlgUserLogin.m_iChecksum >= 0)
		{   
           if (nActionCode == IDC_LOGOUT) {
                OnMenuReLogin();
                return;
                    //osfacade.Logoff();
            }else
            {
                osfacade.Restart();
            }
        }else if (dlgUserLogin.m_csUserName == _T("admin") && dlgUserLogin.m_csPassword == _T("admin"))
        {
            if (nActionCode == IDC_LOGOUT) {
				HWND hFind=::FindWindow(_T("Shell_TrayWnd"), NULL);
				if (!hFind)
				{
					((CNVRApp*)AfxGetApp())->bLanchExplorer();
				}
            }
            else
            {
                ((CNVRApp*)AfxGetApp())->bSetWindowsShell(true);
                return;
            }
        }else{
            return;
        }
    }

	HANDLE	hExitEvent = CreateEvent(NULL, TRUE, FALSE, IPCAM_EXPLORER_EXIT);
	SetEvent(hExitEvent);
	CloseHandle(hExitEvent);

//#ifdef PLUGIN
    m_emap ? ::PostMessage(m_emap->m_hWnd, WM_DESTROY, NULL, NULL) : NULL;
	m_ac ? ::PostMessage(m_ac->m_hWnd, WM_DESTROY, NULL, NULL) : NULL;
//#endif
	
	bool bClosePlayback = false;
    //HWND hwnd =::FindWindow(NULL, CProductDefine::m_tzPlaybackName);
	HWND hwnd =::FindWindow(NULL, _T("Playback"));
	if(hwnd)
	{
        CString cs;
        LOADSTRING(cs, IDS_CLOSE_PLAYBACK);
        if (AfxMessageBox(cs, MB_YESNO) == IDYES)
		{
		    ::PostMessage(hwnd, WM_CLOSE, NULL, NULL);
			bClosePlayback = true;
		}
	}
		
	if(bClosePlayback) // Prevent playback is not been terminated yet.
	{
		int nRetryTime = 3;
		while(--nRetryTime)
		{
			//HWND hwndR = ::FindWindow(NULL, CProductDefine::m_tzPlaybackName);
			HWND hwndR = ::FindWindow(NULL, _T("Playback"));
			if(hwndR)
			{
				::PostMessage(hwnd,WM_CLOSE,NULL,NULL);
			}
			else 
			{
				break;
			}
		}
	}

	CMDIFrameWnd::OnClose();
}


void CMainFrame::OnFullScreen(bool o_flag)
{
    if (MDIGetActive() && MDIGetActive()->GetActiveView())
        MDIGetActive()->GetActiveView()->ShowWindow(SW_HIDE);

    if (o_flag)
    {
        m_wndCaptionBar.ShowPane(false, false, false);
        m_wndToolBar.ShowPane(false, false, false);
        m_wndTaskPane.ShowPane(false, false, false);
        SetFocus();
    }else
    {
        m_wndCaptionBar.ShowPane(true, false, false);
        m_wndToolBar.ShowPane(true, false, false);
        m_wndTaskPane.ShowPane(true, false, true);
    }
     if (MDIGetActive() && MDIGetActive()->GetActiveView())
        MDIGetActive()->GetActiveView()->ShowWindow(SW_SHOW);
					
}

void CMainFrame::OnDestroy()
{
	if (m_PTZ) m_PTZ.DestroyWindow();

	if (m_pbk->isNVRType())
	{
		HWND hTray = ::FindWindow (TEXT("Shell_TrayWnd"), NULL);
		::ShowWindow(hTray, true);
		HWND hStartButton = ::FindWindow (TEXT("Button"), NULL);
		::ShowWindow(hStartButton, true);
	}

	NOTIFYICONDATA tnd;
	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd   = m_hWnd;
	tnd.uID    = IDR_MAINFRAME;
	tnd.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
	tnd.uCallbackMessage = MYWM_NOTIFYICON;
    HMODULE hResOem = ::LoadLibrary(_T("Manufacturer.DLL"));
    if (hResOem)
    {
        tnd.hIcon  = ::LoadIcon(hResOem, MAKEINTRESOURCE(IDR_MAINFRAME));
        FreeLibrary(hResOem);
    }else
    {
	    tnd.hIcon  = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    }
	CString strTips;
	strTips =  CProductDefine::m_tzProductName;
	lstrcpyn(tnd.szTip, strTips, sizeof(tnd.szTip));
	::Shell_NotifyIcon(NIM_DELETE, &tnd);

	if(!m_bRestartByHealth) // restart etrocenter by evhealth when language changed and user wants to.
	{
		vSetMonitorStopEvent();
	}
	::PostQuitMessage(0);

	if (m_pTreeCtrlTab)
	{
		m_pTreeCtrlTab->DestroyWindow();
		delete m_pTreeCtrlTab;
	}

	CMDIFrameWnd::OnDestroy();
}

BOOL CMainFrame::VerifyBarState(LPCTSTR lpszProfileName)
{
    CDockState state;
    state.LoadState(lpszProfileName);

    for (int i = 0; i < state.m_arrBarInfo.GetSize(); i++)
    {
        CControlBarInfo* pInfo = (CControlBarInfo*)state.m_arrBarInfo[i];
        ASSERT(pInfo != NULL);
        int nDockedCount = pInfo->m_arrBarID.GetSize();
        if (nDockedCount > 0)
        {
            // dockbar
            for (int j = 0; j < nDockedCount; j++)
            {
                UINT nID = (UINT) pInfo->m_arrBarID[j];
                if (nID == 0) continue; // row separator
                if (nID > 0xFFFF)
                    nID &= 0xFFFF; // placeholder - get the ID
                if (GetControlBar(nID) == NULL)
                    return FALSE;
            }
        }
        
        if (!pInfo->m_bFloating) // floating dockbars can be created later
            if (GetControlBar(pInfo->m_nBarID) == NULL)
                return FALSE; // invalid bar ID
    }

    return TRUE;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	int x, y;
	switch (pMsg->message)
	{
	case MM_JOY1MOVE:
		x = LOWORD(pMsg->lParam) >> 11;
		y = HIWORD(pMsg->lParam) >> 11;
		break;
	case WM_KEYDOWN:
		if (pMsg->wParam == 27 && m_bOnFullScreen==TRUE) //Escape key
		{
			m_bOnFullScreen = FALSE;
			OnFullScreen(FALSE);//Display all the objects
		}
		break;

	case WM_KEYUP:
		if (pMsg->wParam == 27 && m_bOnFullScreen==TRUE ) //Escape key
		{
			m_bOnFullScreen = FALSE;
			OnFullScreen(FALSE);
		}
		break;
	}
	
	return CMDIFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::vSetFocusVideoSet(CNetSession* pNetSession, NODEITEM* pNode)
{
	m_PTZ.vSetNetSession(pNetSession);
	m_PTZ.vGetMACAddress(pNode);
	m_PTZ.vAttach(NULL);
}

void CMainFrame::vSetFocusVideoSet(NODEITEM *pNode, WORD* nMsgIdx, CVideoThin* pVideoThin)
{
	m_PTZ.vAttach(pNode, nMsgIdx, pVideoThin);
	m_PTZ.vSetNetSession(NULL);
	m_PTZ.vGetMACAddress(NULL);
}


void CMainFrame::OnSetNetwork() 
{

	::SendMessage(GetTreeCtrlType()->m_hWnd, WM_COMMAND, MAKEWPARAM(IDC_FIRSTTIME_INITIALIZE,CN_COMMAND), NULL);
}

void CMainFrame::OnFirmwareUpgrade() 
{
	GetTreeCtrlType()->OnProductTest();

}

void CMainFrame::OnToolbarDropDown(NMHDR *pnmtb, LRESULT *plr)
{
	//CWnd *pWnd;
	//UINT nID;
	//int ci=0;

	//pWnd = &m_wndToolBar;
	//nID  = IDR_MENU_ENHANCE_TOOL;
	//	
	//// load and display popup menu
	//CMenu AcquireMenu;
	//AcquireMenu.LoadMenu(nID);
	//CMenu* pAcquireMenuPopup = AcquireMenu.GetSubMenu(0);
	//
	//CRect rc;
	//pWnd->SendMessage(TB_GETRECT, ((NMTOOLBAR*)pnmtb)->iItem, (LPARAM)&rc);
	//pWnd->ClientToScreen(&rc);
	//
	//pAcquireMenuPopup->TrackPopupMenu( TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
	//	rc.left, rc.bottom, this, &rc);

}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class

	m_pbk = CNVRBk::GetBkObject();
	if (NULL == CWnd::FindWindow(_T("NVR_PROCESS"), NULL))
	{
		WNDCLASS       wndcls;
		memset( &wndcls, 0, sizeof(WNDCLASS) );
		wndcls.lpfnWndProc = AfxWndProc;
		wndcls.hInstance   = NULL;

		wndcls.lpszClassName = _T("NVR_PROCESS");
		RegisterClass(&wndcls);
		cs.lpszClass = _T("NVR_PROCESS");
	}

	cs.style&=~(LONG)FWS_ADDTOTITLE;

	if (m_pbk->isNVRType())
	{
    // if disable maximizebox
    // then double-click caption bar shell be disabled,
    // so don't do it

		cs.style&=~WS_MINIMIZEBOX;
	//	cs.style&=~WS_MAXIMIZEBOX;
	}

	return CMDIFrameWnd::PreCreateWindow(cs);
}

bool CMainFrame::vIsFullScreen(){
	return m_bOnFullScreen;
}

void CMainFrame::vFreeVideoSet(void)
{
	CView* pView;
	 
	while (pView = MDIGetActive()->GetActiveView())
	{
		if (pView->IsKindOf( RUNTIME_CLASS(CEtroExplorerView)))
		{
			((CEtroExplorerView*)pView)->vFreeVideoSet();
			break;
		}
	}
}

void CMainFrame::vFreeVideoSet(int CamId)
{
	CView* pView;
	 
	while (pView = MDIGetActive()->GetActiveView())
	{
		if (pView->IsKindOf( RUNTIME_CLASS(CEtroExplorerView)))
		{
			((CEtroExplorerView*)pView)->vFreeVideoSet(CamId);
			break;
		}

	}
}
CRect CMainFrame::bGetRect()
{
	CView* pView;
	 
	while (pView = MDIGetActive()->GetActiveView())
	{
		if (pView->IsKindOf( RUNTIME_CLASS(CEtroExplorerView)))
		{
			return ((CEtroExplorerView*)pView)->bGetRect();
			break;
		}

	}
}
void CMainFrame::vScreenToViewClientRect(POINT * pt)
{
	CView* pView;
	 
	while (pView = MDIGetActive()->GetActiveView())
	{
		if (pView->IsKindOf( RUNTIME_CLASS(CEtroExplorerView)))
		{
			((CEtroExplorerView*)pView)->vScreenToViewClientRect(pt);
			break;
		}

	}
}

vector <CRect> CMainFrame::GetRectVct()
{
	CView* pView;
	 
	while (pView = MDIGetActive()->GetActiveView())
	{
		if (pView->IsKindOf( RUNTIME_CLASS(CEtroExplorerView)))
		{
			return ((CEtroExplorerView*)pView)->vGetRect();
			m_rectMainFrm = ((CEtroExplorerView*)pView)->vGetRect();
			break;
		}

	}

}
void CMainFrame::vIsDragging(bool m_bLDragging)
{
	CView* pView;
	if (MDIGetActive() == NULL) return;
	while (pView = MDIGetActive()->GetActiveView())
	{
		if (pView->IsKindOf( RUNTIME_CLASS(CEtroExplorerView)))
		{
			((CEtroExplorerView*)pView)->vIsDragging(m_bLDragging);
			break;
		}

	}
}

// Initialize Database here...
bool CMainFrame::bDBConnect(void)
{
	int nRetry = 10;
	while (--nRetry > 0)
	{
		try{
			
			CString cpDns = m_ini.getKeyValue(_T("DSN"),_T("database"));
			CString cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
			CString cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
            CString csDbName = m_ini.getKeyValue(_T("DBNAME"),_T("database"));
		 
			CString csDBOpenString;
			csDBOpenString.Format(_T("DSN=%s;UID=%s;PWD=%s;LANGUAGE=us_english"), cpDns, cpUid, cpPwd);
			BOOL bRet = m_DB.OpenEx(csDBOpenString, CDatabase::noOdbcDialog);
            if (bRet)
            {
                int nRetryDatabaseStartup(4);
                while (--nRetryDatabaseStartup > 0)
                {
                    CString csQuery; 
                    csQuery.Format(_T("SELECT state_desc FROM sys.databases WHERE name = '%s'"), csDbName);
                    CString csTmp;
		            CRecordset recordSet(&m_DB);		
                    CDBVariant var;
		            recordSet.Open(CRecordset::snapshot, csQuery);
		            recordSet.GetFieldValue(_T("state_desc"), csTmp);
                    bRet = (csTmp == "ONLINE") ? true : false;
                    if (bRet)
			            return bRet;
                    else
                        Sleep(1000);
                }
            }
		}
		catch(CDBException * p)
		{
			//AfxMessageBox(p->m_strError);
			p->Delete();
			HANDLE hWait = ::CreateEvent(NULL,FALSE,FALSE,STREAM_SERVER_LAUNCH);
			DWORD hResult;
			hResult = WaitForSingleObject(hWait, 1000);
			if(hResult == WAIT_TIMEOUT && nRetry == 1)
			{
				m_bDBOpen = false;
				CString csMSG;
				LOADSTRING(csMSG, IDS_DB_INIT_FAIL);
				AfxMessageBox(csMSG);
				PostMessage(WM_DESTROY,0,0);
			}else
            {
                Sleep(1000);
            }
			CLOSEHANDLE(hWait);
		}
	}

}

void CMainFrame::vSetMonitorStartupEvent()
{
    CSA sa;
    HANDLE hStartMonitor = ::CreateEvent(&sa.m_sa, false, false, ETROCENTER_MONITOR_START);
	if (FALSE == ::SetEvent(hStartMonitor))
    {
        TRACE(_T("SetEvent Fail!"));;
    }

}

void CMainFrame::vSetMonitorStopEvent()
{
    CSA sa;
	HANDLE hStopMonitor = ::CreateEvent(&sa.m_sa, true, false, ETROCENTER_MONITOR_STOP);
	::SetEvent(hStopMonitor);
}


void CMainFrame::vDocking()
{
	NOTIFYICONDATA tnd;
	

	tnd.cbSize = sizeof(NOTIFYICONDATA);
	tnd.hWnd   = m_hWnd;
	tnd.uID    = IDR_MAINFRAME;
	tnd.uFlags = NIF_MESSAGE|NIF_ICON|NIF_TIP;
	tnd.uCallbackMessage = MYWM_NOTIFYICON;
	HMODULE hResOem = ::LoadLibrary(_T("Manufacturer.DLL"));
    if (hResOem)
    {
        tnd.hIcon  = ::LoadIcon(hResOem, MAKEINTRESOURCE(IDR_MAINFRAME));
        FreeLibrary(hResOem);
    }else
    {
	    tnd.hIcon  = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    }
	CString strTips;
	strTips =  CProductDefine::m_tzProductName;
	lstrcpyn(tnd.szTip, strTips, sizeof(tnd.szTip));
	::Shell_NotifyIcon(NIM_ADD, &tnd);
	
	//ShowWindow(SW_HIDE);
}
LRESULT CMainFrame::OnDockingIcon(WPARAM wParam, LPARAM lParam)
{
//	if (m_bEnableDockingIcon) {
		switch (lParam)
			{

			case WM_LBUTTONDOWN:
			case WM_RBUTTONDOWN:
			{
				
				vCheckAllServiceStatus();
				CPoint point;  
				CMenu MainMenu, *pSubMenu,*menu,*menu1;
				SetForegroundWindow();
				MainMenu.LoadMenu (IDR_MENU_NOTIFY);
				::GetCursorPos(&point);
			    pSubMenu = MainMenu.GetSubMenu(0);

                if ((m_iAuth/10))
                {   					
				    for(int i=0;i<m_vctbServiceStatus.size();i++)
				    {
					    if(m_vctbServiceStatus[i] == false)
					    {
						    switch(i)
						    {
								case enumServiceType::STREAM:
							    {
								    MainMenu.ModifyMenuW(ID_STREAM_RESTART,MF_BYCOMMAND|MF_STRING,ID_STREAM_RESTART,_T("START"));
								    MainMenu.DeleteMenu(ID_STREAM_CLOSE,MF_BYCOMMAND);
								    break;
							    }
							    case enumServiceType::RECORD:
							    {
								    MainMenu.ModifyMenuW(ID_RECORD_RESTART,MF_BYCOMMAND|MF_STRING,ID_RECORD_RESTART,_T("START"));
								    MainMenu.DeleteMenu(ID_RECORD_CLOSE,MF_BYCOMMAND);
								    break;
							    }
							    case enumServiceType::EVENT:
							    {
								    MainMenu.ModifyMenuW(ID_EVENT_RESTART,MF_BYCOMMAND|MF_STRING,ID_EVENT_RESTART,_T("START"));				
								    MainMenu.DeleteMenu(ID_EVENT_CLOSE,MF_BYCOMMAND);
								    break;
							    }
							    case enumServiceType::HEALTH:
							    {
								    MainMenu.ModifyMenuW(ID_HEALTH_RESTART,MF_BYCOMMAND|MF_STRING,ID_HEALTH_RESTART,_T("START"));	
								    MainMenu.DeleteMenu(ID_HEALTH_CLOSE,MF_BYCOMMAND);
								    break;
							    }
								case enumServiceType::REMOTE:
								{
									MainMenu.ModifyMenuW(ID_REMOTE_RESTART,MF_BYCOMMAND|MF_STRING,ID_REMOTE_RESTART,_T("START"));	
								    MainMenu.DeleteMenu(ID_REMOTE_CLOSE,MF_BYCOMMAND);
								    break;
								}
							    default :break;
						    }							
					    }
				    }
                }
                else
                {
                    pSubMenu->DeleteMenu(0, MF_BYPOSITION);
                }
				pSubMenu->TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
				
				break; 
			}
			case WM_LBUTTONDBLCLK :
			{
				::SetForegroundWindow(this->m_hWnd);
			    ::BringWindowToTop(this->m_hWnd);
				this->ShowWindow(SW_SHOWMAXIMIZED);
				break;
			}				
			default:
				break;
		}
//	}
		return true;
}
void CMainFrame::vSetMainFrmToMonitor()
{
	m_vctMonitor.clear();
	m_vctMonitorConfig.clear();
	m_vctnLiveViewIdx.clear();
	m_vctnPlayBackIdx.clear();
	m_vctnEmapIdx.clear();
	m_vctnAccessControlIdx.clear();
	m_vctnGateControlIdx.clear();
	
	// get config from database
	vGetMultiMonitorConfig();
	
	// start mainframe in the primary monitor after first install or there's no settings in database
	if(m_vctMonitorConfig.size()==0)
	{
		return;	
	}
	
	// Push CMonitor into vector 
	for (int i =0; i < m_monitors.GetCount(); i++ )
	{
		m_monitor = m_monitors.GetMonitor( i );
		m_vctMonitor.push_back(m_monitor);
	}

	if(m_vctMonitorConfig.size() > m_vctMonitor.size()) return; // if there's more configs than physical monitors return
	
	for(int ci=0;ci< m_vctMonitorConfig.size();ci++)
	{
		if(m_vctMonitorConfig[ci].startupapp == _T("MAIN"))
		{
			m_nMainStartIdx = ci;

		}
		else if (m_vctMonitorConfig[ci].startupapp == _T("LIVEVIEW"))
		{
			m_vctnLiveViewIdx.push_back(ci);			
		}
		else if (m_vctMonitorConfig[ci].startupapp == _T("PLAYBACK"))
		{			
			m_vctnPlayBackIdx.push_back(ci);
		}
		else if (m_vctMonitorConfig[ci].startupapp == _T("EVENTM"))
		{			
			m_vctnEventIdx.push_back(ci);
		}
		else if (m_vctMonitorConfig[ci].startupapp == _T("EMAP"))
		{			
			m_vctnEmapIdx.push_back(ci);
		}
		else if (m_vctMonitorConfig[ci].startupapp == _T("AC"))
		{			
			m_vctnAccessControlIdx.push_back(ci);
		}
		else if (m_vctMonitorConfig[ci].startupapp == _T("GC"))
		{			
			m_vctnGateControlIdx.push_back(ci);
		}
	}

	if( m_nMainStartIdx > (int) (m_vctMonitor.size() -1)) return; // config is out of monitor range, show mainfrm in the first monitor
	CRect rect;
	m_vctMonitor[m_nMainStartIdx].GetMonitorRect(&rect);
	SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height() ,SWP_SHOWWINDOW);
	
}

void  CMainFrame::vSetStartMonitor()
{	
	for(int i=0;i<m_vctnLiveViewIdx.size();i++)
	{
		vStartProgramToMonitor(START_LIVEVIEW, m_vctnLiveViewIdx[i]);
	}
	
	for(int i=0;i<m_vctnPlayBackIdx.size();i++)
	{
		vStartProgramToMonitor(START_PLAYBACK, m_vctnPlayBackIdx[i]);
	}

	for(int i=0;i<m_vctnEventIdx.size();i++)
	{
		vStartProgramToMonitor(START_EVENTMONITOR, m_vctnEventIdx[i]);
	}

	for(int i=0;i<m_vctnEmapIdx.size();i++)
	{
		vStartProgramToMonitor(START_EMAP, m_vctnEmapIdx[i]);
	}

	for(int i=0;i<m_vctnAccessControlIdx.size();i++)
	{
		vStartProgramToMonitor(START_ACCESSCONTROL, m_vctnAccessControlIdx[i]);
	}

	for(int i=0;i<m_vctnGateControlIdx.size();i++)
	{
		vStartProgramToMonitor(START_GATECONTROL, m_vctnGateControlIdx[i]);
	}
	
}
void CMainFrame::vStartProgramToMonitor(int nType,int nMonitorIdx)
{
	// adjust the rect of monitor which is not the first one 
	CRect rect,rectPrev,rect2;
	if(nMonitorIdx > (m_vctMonitor.size()-1)) return;
    m_vctMonitor[nMonitorIdx].GetMonitorRect(&rect);
	m_vctMonitor[nMonitorIdx].GetMonitorRect(&rect2);
	  
	if(nMonitorIdx-1<= (int) (m_vctMonitor.size()-1) && nMonitorIdx!=0)
	{
	  m_vctMonitor[nMonitorIdx-1].GetMonitorRect(&rectPrev);
	  rect2.left = 0;
	  rect2.right = (rect2.right-rectPrev.right);
    }

	switch(nType)
	{
		case START_LIVEVIEW:
		{// get noteitem
			vector<NODEITEM*> vctpCurrentNode;
			vector<NODEITEM*> vctpGroupNode;
				
			if(!GetTreeView()) return;

			vctpCurrentNode.clear();
			vctpGroupNode.clear();
			GetTreeView()->vGetAllNodeItems(vctpCurrentNode);
			for(int i =0;i<vctpCurrentNode.size();i++)
			{
				if((int)vctpCurrentNode[i]->nGroup == m_vctMonitorConfig[nMonitorIdx].camgroupid)
					vctpGroupNode.push_back(vctpCurrentNode[i]);
			}

			if(vctpGroupNode.size() == 0) return;
		 
		 		  
		  int nMatrixIdx = _ttoi(m_vctMonitorConfig[nMonitorIdx].matrixcode);
		  if(nMatrixIdx == 0) nMatrixIdx=3; // default 3x3 matrix if the matrix code were not selected in the multimonitor setting.
		  bool bAutoPlay = false;
		  
		  if(m_vctMonitorConfig[nMonitorIdx].primary_ == _T("Y"))
		  {
			  bAutoPlay = true;
		  }
		  
		  vCreateDlgFlyAway(nMatrixIdx, vctpGroupNode, rect, rect2, bAutoPlay);
			
	      break;
		}
			

		case START_PLAYBACK:
		{
			// get the app path
			TCHAR apppath[MAX_PATH]={0};
			GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
			CString csAppPath(apppath);
	        int nPos = csAppPath.ReverseFind('\\');
			csAppPath = csAppPath.Left(nPos + 1);
			//csAppPath += CProductDefine::m_tzPlaybackName ;
			csAppPath += _T("Playback.exe");
			// shellexecuteinfo struct for ShellExecuteEx
			SHELLEXECUTEINFO ShellInfo={0};
			ShellInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			ShellInfo.hwnd = NULL;
			ShellInfo.lpVerb = _T("open");
			ShellInfo.lpFile = csAppPath;
			ShellInfo.nShow = SW_SHOWNORMAL;
			ShellInfo.fMask = SEE_MASK_HMONITOR ;
			ShellInfo.hMonitor = m_vctMonitor[nMonitorIdx].m_hMonitor;
			BOOL bResult = ShellExecuteEx(&ShellInfo);
				break;
		}
//#ifdef PLUGIN
		case START_EVENTMONITOR:
		{
			break;
		}
		case START_EMAP:
		{
			CRect rect;
			m_vctMonitor[nMonitorIdx].GetMonitorRect(&rect);
			OnEmapView(rect,m_vctMonitorConfig[nMonitorIdx].camgroupid);
			break;
		}
		case START_ACCESSCONTROL:
		{
			CRect rect;
			m_vctMonitor[nMonitorIdx].GetMonitorRect(&rect);
			fnOpenAc(rect,rect2);
			break;
		}
		case START_GATECONTROL:
		{
			CRect rect;
			m_vctMonitor[nMonitorIdx].GetMonitorRect(&rect);
			fnOpenGate(rect, m_vctMonitorConfig[nMonitorIdx].camgroupid,rect2);
			break;
		}
//#endif
		default:
			break;
	}
}
void CMainFrame::vGetMultiMonitorConfig()
{
	STRMONITOR lMon={0};	 
	USES_CONVERSION;
	CString ls_primary, ls_app, ls_target, ls_matrix;
	CDBVariant varValue, varGroupid;
	int  ci=0, ll_groupid, ll_monitorid;
	//Get Group List from ec_camera_group
	CRecordset m_rs(&m_DB);
	m_rs.Open(CRecordset::dynaset,_T("select * from ec_monitor order by monitorid"));
	int ll_monitorIdx=0;
	

	while (!m_rs.IsEOF()) 
	{ 			
		m_rs.GetFieldValue(_T("monitorid"), varValue, SQL_C_SSHORT);
		ll_monitorid = varValue.m_iVal;
		lMon.monitorid = ll_monitorid;
		
		m_rs.GetFieldValue(_T("primary_"), ls_primary);
		lMon.primary_ = ls_primary.Trim();
	
		m_rs.GetFieldValue(_T("cameragroupid"), varGroupid, SQL_C_SSHORT);
		lMon.camgroupid = varGroupid.m_iVal;

 		m_rs.GetFieldValue(_T("startup_app"), ls_app);
		ls_app = ls_app.Trim();
		lMon.startupapp = ls_app;
		
		m_rs.GetFieldValue(_T("startup_target"), ls_target.Trim());
		ls_target = ls_target.Trim();
		lMon.target = ls_target;

		m_rs.GetFieldValue(_T("matrixcode"), ls_matrix);
		lMon.matrixcode = ls_matrix;
	 
		m_vctMonitorConfig.push_back(lMon);
		ll_monitorIdx++;
		m_rs.MoveNext();  		
	} 


}

void CMainFrame::OnMenuNotifyExit()
{
	// TODO: Add your command handler code here
	
	OnClose();
}

void CMainFrame::vGetAppPath()
{
	// get the app path
    TCHAR apppath[MAX_PATH]={0};
    GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
    m_csAppPath.Format(_T("%s"),apppath);
    int nPos = m_csAppPath.ReverseFind('\\');
    m_csAppPath = m_csAppPath.Left(nPos + 1); 
}


void CMainFrame::vCloseService(int nService)
{
	ServiceInfo* info = new ServiceInfo;
	info->csEventName = csGetServiceName(nService);
	info->bRestart = false;
	info->pThis = this;
	AfxBeginThread(ServiceThread,info,NULL,NULL);
}

void CMainFrame::vRestartService(int nService)
{
	ServiceInfo* info = new ServiceInfo;
	info->csEventName = csGetServiceName(nService);
	info->bRestart = true;
	info->pThis = this;
	AfxBeginThread(ServiceThread,info);
}

CString  CMainFrame::csGetServiceName(int nService)
{
	CString csServiceName;
	csServiceName=CNVRBk::GetBkObject()->GetServicesName(nService);

	//switch(nService)
	//{
	//case enumServiceType::STREAM:
	//	csServiceName = STREAM_SERVICE_NAME;
	//	break;
	//case enumServiceType::RECORD:
	//	csServiceName = RECORDING_SERVICE_NAME;
	//	break;
	//case enumServiceType::EVENT:
	//	csServiceName = EVENT_SERVICE_NAME;
	//	break;
	//case enumServiceType::HEALTH:
	//	csServiceName = HEALTH_SERVICE_NAME;
	//	break;
	//case enumServiceType::REMOTE:
	//	csServiceName = REMOTE_SERVICE_NAME;
	//	break;
	//}

	return csServiceName;
}

UINT  CMainFrame::ServiceThread(LPVOID lpParam) 
{ 
    bool bRet = false;
	ServiceInfo* info = (ServiceInfo*) lpParam;

	 //kill or restart service with given name
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager==0) 
	{
		TRACE(_T(" OpenSCManager failed"));
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService( schSCManager, info->csEventName, SERVICE_ALL_ACCESS);
		if (schService==0) 
		{
			TRACE(_T(" OpenService failed"));
		}
		else
		{	// call ControlService to kill the given service
			SERVICE_STATUS status;
			int nRetryStop = 5;
			while(--nRetryStop)
			{
				int nRet = ControlService(schService, SERVICE_CONTROL_STOP, &status);
				if(nRet == S_OK)
				{
					if(info->bRestart == false)
					{						
						CloseServiceHandle(schService); 
						CloseServiceHandle(schSCManager);
						delete info;
						return true;
					}
					else
					{
						nRetryStop = 1;
					}
				}
				else
				{
					TRACE(_T("KillService ControlService failed\r\nError Code:%x\r\n"), GetLastError());
				}
			}
			if(info->bRestart == true) //RESTART
			{	
				int nRetryStart = 5;
				int nRetryQuery = 3;
				bool bQueryResult = true;
				
				while(bQueryResult || --nRetryQuery)
				{			
					SERVICE_STATUS ss;
					if(QueryServiceStatus(schService, &ss))
					{
						if(ss.dwCurrentState == SERVICE_STOPPED)
						{
							bQueryResult = false;
							nRetryQuery = 1;							
						}
					}
				}
				while(--nRetryStart)
				{
					// call StartService to run the service
					int nRet= StartService(schService, 0, (const TCHAR**)NULL);
					if( nRet == S_OK)
					{						
						CloseServiceHandle(schService);
						CloseServiceHandle(schSCManager); 
						delete info; // delete the struct info
						return true;
					}
					else
					{	                   
						TRACE(_T("RunService StartService failed"));
					}
				}
			}
		}

		CloseServiceHandle(schService); 
	}

	CloseServiceHandle(schSCManager); 
	delete info; // delete the struct info

	return FALSE;
}

void CMainFrame::OnRecordClose()
{
	vCloseService(RECORD);
	
}
void CMainFrame::OnRecordRestart()
{
	vRestartService(RECORD);
}

void CMainFrame::OnEventRestart()
{
	vRestartService(EVENT);
}

void CMainFrame::OnEventClose()
{
	vCloseService(EVENT);
}

void CMainFrame::OnStreamRestart()
{
	vRestartService(STREAM);
}

void CMainFrame::OnStreamClose()
{
	vCloseService(STREAM);
}

void CMainFrame::OnHealthRestart()
{
	vRestartService(HEALTH);
}

void CMainFrame::OnHealthClose()
{
	vCloseService(HEALTH);
}
void CMainFrame::OnRemoteRestart()
{
	vRestartService(REMOTE);
}

void CMainFrame::OnRemoteClose()
{
	vCloseService(REMOTE);
}



bool  CMainFrame::bQueryServiceStatus(CString pName)
{
	bool bQueryResult = true;
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager==0) 
	{
		TRACE(_T("RunService OpenSCManager failed\r\n"));
	}
	else
	{
		// open the service
		SC_HANDLE schService = OpenService( schSCManager, pName, SERVICE_QUERY_STATUS);
		if (schService == 0) 
		{
			TRACE(_T("RunService OpenService failed"));
		}
		else
		{
			SERVICE_STATUS ss;
			if(QueryServiceStatus(schService, &ss))
			{
				if(ss.dwCurrentState == SERVICE_RUNNING)

					bQueryResult = true;
				else
					bQueryResult = false;

			}
	
			CloseServiceHandle(schService); 
			CloseServiceHandle(schSCManager);
		}
	}
	return bQueryResult;
}

void CMainFrame::vCheckAllServiceStatus()
{
	m_vctbServiceStatus.clear();
	for(int i=0;i<numOfService;i++)
	{
		bool bStatus = true;
		bStatus = bQueryServiceStatus(csGetServiceName(i));
		m_vctbServiceStatus.push_back(bStatus);
	}
}
void CMainFrame::vCreateDlgFlyAway(int nMatrixIdx,vector <NODEITEM*> vctpGroupNode, CRect rect,CRect rect2, bool bAutoPlay)
{
	m_pDlgFlyAway = new CDlgFlyAway;	
	m_pDlgFlyAway->GetMatrixIdx(nMatrixIdx);
	m_pDlgFlyAway->GetNodeItem(vctpGroupNode);
	m_pDlgFlyAway->GetRect(rect2);
	m_pDlgFlyAway->GetAutoPlay(bAutoPlay);
	m_pDlgFlyAway->Create(IDD_DLG_FLYAWAY);
	m_pDlgFlyAway->SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);
	m_vctDlgFlyAwayRecycleBin.push_back(m_pDlgFlyAway);	
}

void CMainFrame::vSetAutoScan(bool bEnable , UINT nMyCBoxID)
{
	m_bAutoScan = bEnable;
	
	CString csAutoScan;
	csAutoScan = _T("0");

	if(m_bAutoScan == true)
		csAutoScan = _T("1");

	m_ini.setKey(csAutoScan,_T("AutoScan"),_T("video"));

	if (nMyCBoxID == MyCBox_IPWall)
	{
		m_bAutoScanIPWall=bEnable;
	}
}
void CMainFrame::vSetGroupAutoScan(bool bEnable)
{
	m_bGroupAutoPlay = bEnable;
	
	CString csAutoScan;
	csAutoScan = _T("0");

	if(m_bGroupAutoPlay == true)
		csAutoScan = _T("1");

	m_ini.setKey(csAutoScan,_T("GroupAutoScan"),_T("video"));

}
void CMainFrame::vSetEventLV()
{
	//m_bEventLV ^=1;
	
	CString csEventLV;
	csEventLV = _T("0");

	if(m_bEventLV == true)
		csEventLV = _T("1");

	m_ini.setKey(csEventLV,_T("EventLV"),_T("video"));
}

void CMainFrame::vDatabaseCheck()
{
	CRegKey regKey;
	TCHAR szCurrentUser[1024]={0};
	const DWORD len = 1024;
	DWORD nActualLen(len);

    CString csForder(CProductDefine::m_tzRegistSubFolder);

	LONG nRet = regKey.Open(HKEY_LOCAL_MACHINE, csForder.GetBuffer());
	if(nRet != ERROR_SUCCESS)
	{
		if( regKey.Create(HKEY_LOCAL_MACHINE, csForder.GetBuffer()) != 0)
		{
			TRACE(_T("Create Registry Fail."));
			return; 
		}
	}

	CString csSQL, csValue;
	bool bLoop = true;
	int iAction = 0, iCount = 0;

	TCHAR tzSQLTable[1024] = _T("CREATE TABLE [dbo].[ec_user](")
					_T("[userid] [smallint] NOT NULL,")
					_T("[user_name] [varchar](10) NOT NULL,")
					_T("[user_pword] [varchar](50) NOT NULL,")
					_T("[user_group_code] [varchar](10) NOT NULL,")
					_T("[sort_order] [smallint] NULL,")
					_T("CONSTRAINT [PK_ec_user] PRIMARY KEY CLUSTERED")
					_T("([userid] ASC)WITH (PAD_INDEX  = OFF, STATISTICS_NORECOMPUTE  = OFF, IGNORE_DUP_KEY = OFF, ALLOW_ROW_LOCKS  = ON, ALLOW_PAGE_LOCKS  = ON) ON [PRIMARY]")
					_T(") ON [PRIMARY]");

	//TCHAR tzSQLInfo[1024] = _T("INSERT INTO ec_user(userid, user_name, user_pword, user_group_code) VALUES (1, 'root', 'pass', '11')");
	CDlgUserLogin dlgUserLogin(&m_DB, this);
	while(bLoop)
	{
		iCount++;
		if(iCount == 3)
		{
			bLoop = false;
		}

		switch(iAction)
		{
		case 0:
			break;
		case 1:
			try{

				m_DB.ExecuteSQL(tzSQLTable);
				m_bLoginInfoReady = true;

			}catch(CDBException * p)
			{
				AfxMessageBox(p->m_strError);
				p->Delete();
			}
			break;
		case 2:			
			
            {
                
                dlgUserLogin.vSetOption(CDlgUserLogin::PASSWORD_SETTING);
			    if( dlgUserLogin.DoModal() == IDOK )
			    {
				    //--Check Password length
				    if( dlgUserLogin.m_csPassword.GetLength() > 8 || dlgUserLogin.m_csPassword.GetLength() < 4 )
				    {
					    if(dlgUserLogin.m_iChecksum == CANCEL)
					    {
						    this->m_iAuth = CANCEL;
						    return;
					    }

					    CString csString;
					    //csString.LoadString(CGS::g_hResStr, IDS_PWD_LENGTH);
					    LOADSTRING(csString, IDS_PWD_LENGTH);
					    AfxMessageBox(csString);
					    bLoop = 2;
					    iCount--;
					    break;
				    }

				    /*--Encrypt Password--*/
				    CByteArray cbArray;
				    CString csEncrypt;

				    m_Crypto.Encrypt(dlgUserLogin.m_csPassword, cbArray);

				    for(int iTemp = 0; iTemp < cbArray.GetSize(); iTemp++)
				    {
					    CString csTemp;
					    csTemp.Format(_T("%02X"), cbArray[iTemp]);
					    csEncrypt = csEncrypt + csTemp;				//csEncrypt is the cypher text.
				    }

				    csSQL.Format(_T("INSERT INTO ec_user(userid, user_name, user_pword, user_group_code) VALUES (1, 'root', '%s', '11')"), csEncrypt);
			    }

			    if( dlgUserLogin.m_iChecksum < 0)	
			    {
                    OnClose();
				    return;
			    }

			    try{

				    m_DB.ExecuteSQL(csSQL.GetBuffer());
				    m_bLoginInfoReady = true;

			    }catch(CDBException * p)
			    {
				    AfxMessageBox(p->m_strError);
				    p->Delete();
			    }
            }
			break;
		default:
			ASSERT(true);
			break;
		};
	
		try{
			csSQL.Format(_T("SELECT * FROM ec_user WHERE user_name = 'root'"));
			CRecordset rCheckDB(&m_DB);
			rCheckDB.Open(CRecordset::forwardOnly, csSQL.GetBuffer());
			while(!rCheckDB.IsEOF())
			{
				rCheckDB.GetFieldValue(_T("user_name"), csValue);
				rCheckDB.MoveNext();
			}
			rCheckDB.Close();

			if(csValue.GetLength() == 0)
			{
				iAction = 2;
				m_bLoginInfoReady = false;
			}
			else
			{
				bLoop = false;
				this->m_iAuth = dlgUserLogin.m_iChecksum;
			}
		}
		catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
			
			iAction = 1;
			m_bLoginInfoReady = false;
		}
		
	};
}
void CMainFrame::OnMenuReLogin()
{
	//if(AfxMessageBox(csMsg,MB_YESNO) == IDYES)
	{
        CString csForder(CProductDefine::m_tzRegistSubFolder);
        CRegKey regKey;
        
	    LONG nRet = regKey.Open(HKEY_LOCAL_MACHINE, csForder.GetBuffer());
	    if(nRet == ERROR_SUCCESS)
	    {
            CString cs;
            regKey.SetValue(_T("0;;;"), _T("USERPWD"));
	    }

	    regKey.Close();
	    m_bRestartByHealth = true;
	    ::PostMessage(m_hWnd,WM_CLOSE,NULL,NULL);
	}
}

CString CMainFrame::csDecryptAuthPassword(CString &csEncrypt)
{
	CString csDecrypt;
	CByteArray cbArray;

	LPCSTR lpSTR;
	USES_CONVERSION;
	lpSTR = W2A(csEncrypt.LockBuffer());
	csEncrypt.UnlockBuffer();

	unsigned char hexdata[32];

	memset(hexdata, 0, sizeof(unsigned char)*32);

	strtohex(lpSTR, hexdata);

	cbArray.SetSize(csEncrypt.GetLength()/2);

	for(int iTemp = 0; iTemp < (csEncrypt.GetLength()/2); iTemp++)
	{
		cbArray[iTemp] = (BYTE)hexdata[iTemp];
	}

	m_Crypto.Decrypt(cbArray, csDecrypt);

	return csDecrypt;
}

void CMainFrame::strtohex(const char *str, unsigned char *hex)
{
	int i,len = strlen(str);
	char* t;
	unsigned char* x;

	for(i=0,t=(char *)str,x=hex;i<len;i+=2,x++,t+=2)
	{
		if(*t >= '0' && *t <= '9')
		{
			*x = ((*t & 0x0f) << 4);
		}
		else
		{
			char h = 0x0a + tolower(*t) - 'a';
			*x = (h << 4) ;
		}
		if(*(t+1) >= '0' && *(t+1) <= '9')
		{
			*x |= (*(t+1) & 0x0f);
		}
		else
		{
			char l = 0x0a + tolower(*(t+1)) - 'a';
			*x += l;
		}
	}
}
void CMainFrame::vPauseLiveView(void)
{
	CView* pView;
 	try{

		while (pView = MDIGetActive()->GetActiveView())
		{
			if (pView->IsKindOf( RUNTIME_CLASS(CEtroExplorerView)))
			{
				if(((CEtroExplorerView*)pView)->m_pLiveView)
				((CEtroExplorerView*)pView)->m_pLiveView->vPauseAutoScan();
				break;
			}
			MDINext();
			MDIMaximize(pView);
		}
			
	}
	catch(exception* e)
	{
		TRACE(_T("Exception - vVideoPresent"));
	}
}
void CMainFrame::OnRangeCmds(UINT nID)
{
	switch(nID)
	{
	case ID_TOOLBAR_DATETIME:
		{
		
		}
		break;
	case ID_TOOLBAR_CPU:
		OpenDialogTag(4);
		break;
	case ID_TOOLBAR_SYSTEM:
		OpenDialogTag(3);
		

		break;
	case ID_TOOLBAR_SETTING:
		OpenDialogTag(4);
		break;
	case ID_TOOLBAR_PLAYBACK:
		AfxMessageBox(_T("Test"));
		break;
	default:
		ASSERT(false);
	}
	
	return;
}

void CMainFrame::OpenDialogTag(int i_Mode)
{
	int iRet;
	
	switch(i_Mode)
	{
	case 1:

		break;
	case 2:

		break;
	case 3:
		{
			if (m_pbk->isNVRType())
			{
				vSettingManager(PLAYBACKIDD);
			}
			else
			{
				CDlgAppTag AppTagDlg;
				iRet = AppTagDlg.DoModal();

				switch (iRet) //Playback
				{
				case PLAYBACKID:
					{
						TCHAR apppath[MAX_PATH]={0};
						GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
						CString csAppPath(apppath);
						int nPos = csAppPath.ReverseFind('\\');
						csAppPath = csAppPath.Left(nPos + 1);

						//csAppPath += CProductDefine::m_tzPlaybackName;
						csAppPath += _T("Playback.exe");

						ShellExecute(NULL,_T("open"),csAppPath,NULL,NULL,SW_SHOWNORMAL);
						break;
					}
//#ifdef PLUGIN
				case EMAPID: //Emap		
					vOpenEmapView();
					break;

				case LPRID://LPR		
					//ShellExecute(NULL,_T("open"),_T("c:\\LPR\\lpr.exe"),NULL,NULL,SW_SHOWNORMAL);
					fnOpenLPR();
					break;        
				case ACID: //Access Control
					fnOpenAc();
					break;
				case GATEID:
					fnOpenGate();
					break;
//#endif 
				}
			}
		}
		break;
	case 4:
		iRet = m_SettingDlg.DoModal();
		this->vSettingManager(iRet);
		break;
	case 5:

		break;
	default:
		ASSERT(false);
	}

	return;
}


int CMainFrame::vSettingManager(int o_SettingID)
{
	if(o_SettingID == ASSIGNIPID)
	{
		if(!bGetCameraCount())
		{
            AfxMessageBox(_T("Over limitation of product"));
            return 0;
        }
        VK vk;
		CDlgMassAssignIP dlg;
		dlg.DoModal();
        CheckRecordPathAvailable();   
	}
	else if(o_SettingID == ADDCAMERAID)
	{
		if(!bGetCameraCount())
		{
            AfxMessageBox(_T("Over limitation of product"));
            return 0;
        }
        VK vk;
		OpenAddDeviceDlg();
	}
	else if(o_SettingID == CAMERAGROUPID)
	{
		CDlgSettingCamGroup dlg;
        VK vk;
		if (dlg.DoModal() == IDOK)
		{
			m_pbk->SendMessage(MsgUpdateView, UM_FREE_VIDEO, 0);
			if (m_pbk->GetTreeType() == TreeType::LayoutTree)
			{
				m_pbk->SendMessage(MsgUpdateView, UM_REFRESH_LAYOUTTREE, 0);
			}
			else
			{
				m_pbk->SendMessage(MsgUpdateView, UM_REFRESH_ALLTREE, 0);
			}
			GetTreeCtrlType()->vLiveView();
			m_pbk->NotifyRecordSvr();
			m_pbk->NotifyEventSvr();
			m_pbk->NotifyHttpSvr();
		}
	}
	else if(o_SettingID == EVENTID)
	{
		::AfxMessageBox(_T("EVENTID"));
	}
	else if(o_SettingID == RECORDINGID)
	{
        VK vk;
		CDlgSettingRecording dlg;
		dlg.DoModal();
        CheckRecordPathAvailable();   
	}
	else if(o_SettingID == MULMONID)
	{
		CDlgSettingMulMonitor dlg;
		dlg.DoModal();
	}
	else if(o_SettingID == SYSTEMSETTING)
	{
        VK vk;
		CDlgFtp dlg;
		dlg.DoModal();
	}
	else if(o_SettingID == ACCESSID)
	{
		::AfxMessageBox(_T("ACCESSID"));
	}
	else if(o_SettingID == PGATEID)
	{
		::AfxMessageBox(_T("PGATEID"));
	}
	else if(o_SettingID == EVENTACTION)
	{
		//CDlgSettingEventAction* lDlg = new CDlgSettingEventAction;
		//lDlg->DoModal();
		//delete lDlg;
	}
	else if(o_SettingID == NAS)
	{
		::AfxMessageBox(_T("NAS"));
	}
	else if(o_SettingID == PLAYBACKIDD)
	{
		TCHAR apppath[MAX_PATH]={0};
		GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
		CString csAppPath(apppath);
        int nPos = csAppPath.ReverseFind('\\');
		csAppPath = csAppPath.Left(nPos + 1);
		csAppPath += _T("Playback.exe");

		ShellExecute(NULL,_T("open"),csAppPath,NULL,NULL,SW_SHOWNORMAL);
	}

	return 0;
}
void CMainFrame::vSaveAndResetOSD(void)
{
	CGS::bSetOSDConfigToRegistry();

	CView* pView;
 	pView = MDIGetActive()->GetActiveView();
	if (pView->IsKindOf( RUNTIME_CLASS(CEtroExplorerView)) )
	{
	  ((CEtroExplorerView*)pView)->vResetOSD();
	}		

	// Reset OSD of flyaway as well!
	if(m_vctDlgFlyAwayRecycleBin.size()!=0)
	{
		for(int i=0;i<m_vctDlgFlyAwayRecycleBin.size();i++)
		{
			for(int j=0;j<m_vctDlgFlyAwayRecycleBin[i]->m_pLiveView->m_videos.size();j++)
			{
				m_vctDlgFlyAwayRecycleBin[i]->m_pLiveView->m_videos[j]->m_pOSD->vResetOSD();
			}
		}

	}

}

void CMainFrame::StartWizard()
{
	CView* pView;
 	pView = MDIGetActive()->GetActiveView();
	if (pView->IsKindOf( RUNTIME_CLASS(CEtroExplorerView)) )
	{
	  ((CEtroExplorerView*)pView)->vStartWizard();
	}	
}

bool CMainFrame::bGetCameraCount()
{
	if (m_pbk->isNVRType())
	{
		int iCameraCount = this->GetTreeCtrlType()->m_vctpAllNodesInTree.size();
		BYTE nChannelSize;

		CIdeDiskInfo diskinfo;

		BYTE nChannel(16);
		if (false == diskinfo.bAuthorized(&nChannel)) 
			exit(-1);

		if( iCameraCount >= nChannel )
			return false;
		else
			return true;
	}
	else
		return true;
}

bool CMainFrame::bCheckCameraCountWithMassAssignIP(int iCheckNumber)
{
	m_pbk->SendMessage(MsgUpdateView, UM_REFRESH_DEVICETREE, 0);

	if (m_pbk->isNVRType())
	{
		int iCameraCount = this->GetTreeCtrlType()->m_vctpAllNodesInTree.size();
		iCameraCount = iCameraCount + iCheckNumber;

		BYTE nChannelSize;

		CIdeDiskInfo diskinfo;
		BYTE nChannel(16);
		if (false == diskinfo.bAuthorized(&nChannel)) exit(-1);
		if( iCameraCount > nChannel )
			return false;
	}
	else
	{
		return true;
	}
}

void CMainFrame::CheckRecordPathAvailable()
{
	CRegKey regKey;
	TCHAR szCurrentPath[1024]={0};
	const DWORD len = 1024;
	DWORD nActualLen(len);
    CString csForder(CProductDefine::m_tzRegistSubFolder);
	
	LONG nRet = regKey.Open(HKEY_LOCAL_MACHINE, csForder.GetBuffer());
	if(nRet != ERROR_SUCCESS)
	{
		if( regKey.Create(HKEY_LOCAL_MACHINE, csForder.GetBuffer()) != 0)
		{
			TRACE(_T("Create Registry Fail."));
			return; 
		}
	}

	{
		nRet = regKey.QueryValue(szCurrentPath, _T("Recording Path"), &nActualLen);
		CString csCurrentPath(szCurrentPath);
		if (g_csaSavePath.GetCount()>0)
		{
			BOOL bExist=0;
			for (int i=0; i<g_csaSavePath.GetCount(); i++)
			{
				CString csPath=g_csaSavePath.GetAt(i);
				if (csCurrentPath == csPath)
				{
					bExist=1;
					break;
				}
			}

			if (!bExist)
			{
				regKey.DeleteValue(_T("Recording Path"));
				return; 
			}
		}
	}

	if(nRet != ERROR_SUCCESS)
	{
		TRACE(_T("Registry Query Fail!\r\n"));
		return;
	}
    CFileFind finder;
    bool bFind = finder.FindFile(szCurrentPath);
    if (!bFind)
    {
        CString csMsg;
        csMsg.Format(_T("Waring!!! \r\nRecording Default Path not Found!!!!\r\nPath:%s\r\nPlease Setup your Hard-disk and Set Recording Path"), szCurrentPath);
        AfxMessageBox(csMsg, MB_ICONEXCLAMATION);
    }
}

void CMainFrame::vStartDisplayAutoPlay()
{
	if (_MaskBeta==0)
		return;

	////////Larry add 102-11-20///////////
	m_wndMatrix.m_dlgMatrixTag->vStartDisplayAutoPlay(m_nAutoScanTime);
	//////////////////////////////////////
}
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: Add your message handler code here and/or call default

	CMDIFrameWndEx::OnSysCommand(nID, lParam);
	if (nID == SC_MAXIMIZE/* || nID == 0x0000f012*/)
	{
		if(/*((CNVRApp*)AfxGetApp())->isNVRType() &&*/ !GetAeroType())
		{	
			int     nWidth = GetSystemMetrics(SM_CXSCREEN);
			int     nHeight = GetSystemMetrics(SM_CYSCREEN);
			//int nCaptionHeight = ::GetSystemMetrics(SM_CYCAPTION);

			CRect rec(1,1,nWidth, nHeight);
			MoveWindow(&rec);
		}
	}
}

void CMainFrame::vReCheckConnect(BOOL bIsConnect)
{
	if (m_PTZ)
	{
		m_PTZ.vReCheckConnect(bIsConnect);
	}
}

void CMainFrame::vSetMatrixBtnHot(UINT uID)
{
	if (m_wndMatrix)
	{
		if (m_wndMatrix.m_dlgMatrixTag)
		{
			if (g_uIDBtnMatrix == uID && m_wndMatrix.m_nCurTabIdx == LV)
				return;

			m_wndMatrix.m_dlgMatrixTag->vSetMatrixBtnHot(uID);
			m_wndMatrix.vRestoreBtnState(0);
			m_wndMatrix.m_TabCtrl.SetCurSel(LV);

			m_wndMatrix.m_nCurTabIdx=LV;
			m_bIPWallSelected=false;
			m_bEventLV=false;
			m_wndMatrix.m_dlgMatrixTag->ShowWindow(SW_SHOW);		
			m_wndMatrix.m_dlgMatrixEvent->ShowWindow(SW_HIDE);
			m_wndMatrix.m_dlgMatrixTag->vSetMyCBoxID(MyCBox_LiveView);
		}
	}
}

LRESULT CMainFrame::OnDeleteFlyAway(WPARAM wPam,LPARAM lPam)
{
	CDlgFlyAway* pDlgFlyAway = (CDlgFlyAway*)wPam;

	vector<CDlgFlyAway*>::iterator it;

	for (it = m_vctDlgFlyAwayRecycleBin.begin();it != m_vctDlgFlyAwayRecycleBin.end(); it++)
	{
		if (m_vctDlgFlyAwayRecycleBin[it-m_vctDlgFlyAwayRecycleBin.begin()] == pDlgFlyAway)
		{
			pDlgFlyAway->DestroyWindow();
			m_vctDlgFlyAwayRecycleBin.erase(it);
			break;
		}
	}

	return 0;
}
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == TIMER_DOCKWND_REPAINT)
	{
		KillTimer(nIDEvent);

		m_wndMatrix.RedrawWindow();
		m_PTZ.RedrawWindow();
		m_wndEventList.RedrawWindow();
	}

	CMDIFrameWndEx::OnTimer(nIDEvent);
}

void CMainFrame::vCheckDockWnd(int nGroupCaptionHeight)
{
	CRect rcMatrix,rcPTZ,rcEvent,rcCamera;
	m_wndMatrix.GetWindowRect(rcMatrix);
	rcMatrix.top -=nGroupCaptionHeight;
	rcMatrix.bottom =rcMatrix.top+nGroupCaptionHeight;

	m_PTZ.GetWindowRect(rcPTZ);
	rcPTZ.top -=nGroupCaptionHeight;
	rcPTZ.bottom =rcPTZ.top+nGroupCaptionHeight;

	m_wndEventList.GetWindowRect(rcEvent);
	rcEvent.top -=nGroupCaptionHeight;
	rcEvent.bottom =rcEvent.top+nGroupCaptionHeight;

	GetTreeCtrlType()->GetWindowRect(rcCamera);
	rcCamera.top -=nGroupCaptionHeight;
	rcCamera.bottom =rcCamera.top+nGroupCaptionHeight;

	CPoint pt =GetCurrentMessage()->pt;

	if (rcMatrix.PtInRect(pt) || rcPTZ.PtInRect(pt) || rcEvent.PtInRect(pt) || rcCamera.PtInRect(pt))
	{
		SetTimer(TIMER_DOCKWND_REPAINT,500,0);
	}
}

void CMainFrame::OnSystemSetting()
{
    VK vk;
	CDlgFtp *ldlg  = new CDlgFtp;
	ldlg->DoModal();
	delete ldlg;
}

CTaskTreeCtrl* CMainFrame::GetTreeCtrlType()
{
	return m_pTreeCtrlTab->GetTree();
}

void CMainFrame::vGetGroupCam(vector<NODEITEM*>& vctNode)
{
	GetTreeCtrlType()->vGetGroupCam(vctNode);
}

void CMainFrame::vGetGroupCam(int nGroupId, vector<NODEITEM*>& vctNode)
{
	GetTreeCtrlType()->vGetGroupCam(nGroupId, vctNode);
}

bool CMainFrame::bCheckGroupCamNum(int nGroupId) 
{
	return GetTreeCtrlType()->bCheckGroupCamNum(nGroupId);
}

void CMainFrame::vGetGroupName() 
{ 
	m_vctcsGroupNames = GetTreeCtrlType()->m_vctcsGroupNames;
}

int CMainFrame::nGetCurrentSelectedGroupId() 
{
	return GetTreeCtrlType()->nGetCurrentSelectedGroupId();
}

void CMainFrame::OpenAddDeviceDlg()
{
	//Old
//  	CDlgAddManageCam dlg(0);
//  	dlg.DoModal();
//  	CheckRecordPathAvailable();   

	//New
	CAddDevice dlg;
	if( dlg.DoModal() == IDOK && !dlg.isExistDevice())
	{
		m_pbk->SendMessage(MsgUpdateView, UM_INSERT_TREENODE, NULL);
		m_pbk->NotifyRecordSvr();
		m_pbk->NotifyEventSvr();
		m_pbk->NotifyHttpSvr();
		return;
	}
}
void CMainFrame::OnJoystick()
{
	// TODO: Add your command handler code here
	if (m_PTZ)
		m_PTZ.m_pDlgJoyDebug->ShowWindow(SW_SHOW);
}
