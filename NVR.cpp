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
#include "ChildFrm.h"
#include "EtroExplorerDoc.h"
#include "EtroExplorerView.h"
#include "ProductDefine.h"
#include "ini.h"
#include "ResourceMgr.h"
#include "DlgMatrixTab.h"
#include "GetCpuID.h"
#include "IdeDiskInfo.h"
#include "NVR.h"
#include "OSFacade.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
    
HHOOK		hHook(NULL);
/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarApp

BEGIN_MESSAGE_MAP(CNVRApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNVRApp construction

CNVRApp::CNVRApp() :
	CWinAppEx (TRUE /* m_bResourceSmartUpdate */)
{
    //_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG | _CRTDBG_MODE_WNDW );
    _CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
   _CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
   _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
   _CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
   _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG );
   _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );
	m_bSaveState = FALSE;
	m_bLoadWindowPlacement = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWorkSpaceToolBarApp object

CNVRApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarApp initialization
void CNVRApp::InitOEM()
{
	HMODULE hResOem = ::LoadLibrary(_T("Manufacturer.dll"));
    if (hResOem)
    {
        LOADOEMSTRING(hResOem, IDS_BRAND, CProductDefine::m_tzBrand);
        LOADOEMSTRING(hResOem, IDS_PRODUCTNAME, CProductDefine::m_tzProductName);

		if (CNVRBk::GetBkObject()->isNVRType())
        {
			CIdeDiskInfo diskinfo;
			BYTE nChannel(0);
			if (false == diskinfo.bAuthorized(&nChannel))  exit(-1);
			bSetWindowsShell(false);

            //CIdeDiskInfo diskinfo;
            //BYTE nChannel(0);
            //if (false == diskinfo.bAuthorized(&nChannel)) exit(-1);
			_stprintf(CProductDefine::m_tzProductName, _T("%s%d"), _T("NVR"), nChannel);
            HWND hFind=::FindWindow(_T("Shell_TrayWnd"), NULL);
			if (NULL != hFind)
            {
                COSFacade osfacade;
                osfacade.Logoff();
                exit(-1);
            }
        }

        LOADOEMSTRING(hResOem, IDS_PLAYBACKNAME, CProductDefine::m_tzPlaybackName);
        LOADOEMSTRING(hResOem, IDS_PREFIX, CProductDefine::m_tzPrefix);
        LOADOEMSTRING(hResOem, IDS_REGISTSUBFOLDER, CProductDefine::m_tzRegistSubFolder);
        FreeLibrary(hResOem);
    }
}

bool  CNVRApp::bInitProduct()
{
    ::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOALIGNMENTFAULTEXCEPT | SEM_NOOPENFILEERRORBOX);	
	HANDLE h = ::CreateMutex(NULL, TRUE, APMUTEXID);
    DWORD dwRet = ::GetLastError();
	if (ERROR_ALREADY_EXISTS == dwRet || ERROR_ACCESS_DENIED == dwRet)
	{
		HWND hwnd =FindWindow(NULL, CProductDefine::m_tzProductName);
		if (hwnd)
		{
			::ShowWindow(hwnd, SW_NORMAL);
			BOOL bResult=::SetForegroundWindow(hwnd);
			bResult = ::BringWindowToTop((hwnd));
		}
		return false;
	}

	InitOEM();
	CIniReader iniFile;
	iniFile.setINIFullFileName(_T("\\ec.ini"));
	CString csPath = iniFile.csGetInIPath();
	
	//Check if ec.ini exist
	if (csPath.GetLength() == 0) 
	{
//		AfxMessageBox(IDS_INI_NOT_FOUND);
		return  false;
	}
	
    
	////////////////////////////////
	CResourceMgr resMgr;
	resMgr.vQuerynGetDLL();
	 if (NULL == resMgr.g_hResStr) 
	 {
		 CResourceMgr::g_hResStr = AfxGetResourceHandle();
		 CResourceMgr::m_nLocaleID = ENUS_LCID; // set default language to English
		 resMgr.bSetLocaleID(ENUS_LCID); 
	 }
	
}
void CNVRApp::vNVRConduction()
{   
    PBYTE lpKeyState(NULL);
    GetKeyboardState(lpKeyState);
    if (lpKeyState == NULL)
    {    
        TCHAR szParm[MAX_PATH]={};
        _stprintf(szParm, _T("osk.exe"));
        STARTUPINFO ssi={};
        PROCESS_INFORMATION ppi={};
        //ssi.cb = sizeof(ssi);
        //ssi.wShowWindow = SW_HIDE;
        //ssi.dwX = 0;
        //ssi.dwY = 0;
        //ssi.dwXSize = 500;
        //ssi.dwYSize = 250;

        CreateProcess(NULL, szParm, NULL, NULL, FALSE, 0, NULL, NULL, &ssi, &ppi );
        HWND hwnd(NULL);
        while (!hwnd)
        {
            hwnd = ::FindWindow(_T("OSKMainClass"), NULL);
        }
        ::ShowWindow(hwnd, SW_MINIMIZE);
        ::Sleep(1000);
        ::ShowWindow(hwnd, SW_MINIMIZE);
    }
}
void CNVRApp::GetCursorResource()
{
	int nIdx(-1);
	m_hCursor[++nIdx] = LoadCursor(IDC_CURHAND);
	m_hCursor[++nIdx] = LoadCursor(IDC_CURHANDGRAP);	
	m_hCursor[++nIdx] = LoadCursor(IDC_CURSORMOVE);
	m_hCursor[++nIdx] = LoadCursor(IDC_CURSOR_TVSET);
	m_hCursor[++nIdx] = LoadCursor(IDC_CURSOR_BLUEPEN);
	m_hCursor[++nIdx] = LoadCursor(IDC_CURSOR_REDPEN);
	ASSERT(nIdx < sizeof(m_hCursor) / sizeof(HCURSOR));
}
#define DESKTOPNAME _T("EVDESKTOP")
int  WINAPI Thread_Desktop()//LPTHREAD_START_ROUTINE ThreadFunc, THREAD_DATA *td)
{
	HDESK	hOriginalThread;
	HDESK	hOriginalInput;
	HDESK	hNewDesktop;

	// Save original ...
	hOriginalThread = GetThreadDesktop(GetCurrentThreadId());
	hOriginalInput = OpenInputDesktop(0, FALSE, DESKTOP_SWITCHDESKTOP);

	// Create a new Desktop and switch to it
	hNewDesktop = CreateDesktop(DESKTOPNAME, NULL, NULL, 0, GENERIC_ALL, NULL);
	SetThreadDesktop(hNewDesktop);
	SwitchDesktop(hNewDesktop);

	// Execute thread in new desktop
	//td->hDesk = hNewDesktop;
	//StartThread(ThreadFunc, td);
    //CreateThread
	// Restore original ...
	//SwitchDesktop(hOriginalInput);
	//SetThreadDesktop(hOriginalThread);

	// Close the Desktop
	//CloseDesktop(hNewDesktop);

	return 0;
}




BOOL CNVRApp::InitInstance()
{
	CWnd *pWndPrev, *pWndChild;
	pWndPrev = CWnd::FindWindow(_T("NVR_PROCESS"), NULL);
	if (NULL != pWndPrev)
	{
		pWndChild = pWndPrev->GetLastActivePopup();
		pWndPrev->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
		pWndChild->SetForegroundWindow();
	}
	else
	{
		::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOALIGNMENTFAULTEXCEPT | SEM_NOOPENFILEERRORBOX);	

		if (bInitProduct() == false) exit(-1);
		using namespace Gdiplus;
		Gdiplus::GdiplusStartupInput m_gdiplusStartupInput;
		ULONG_PTR m_gdiplusToken;
		Gdiplus::GdiplusStartup(&m_gdiplusToken, &m_gdiplusStartupInput, NULL);

		if (!AfxOleInit())
		{
			AfxMessageBox(IDP_OLE_INIT_FAILED);
			return FALSE;
		}

		AfxEnableControlContainer();
        //Thread_Desktop();
		/*if (CNVRBk::GetBkObject()->isNVRType())
		{
			CIdeDiskInfo diskinfo;
			BYTE nChannel(0);
			if (false == diskinfo.bAuthorized(&nChannel)) return -1;
            bSetWindowsShell(false);
		}*/
        //bDivingWindows(false);
		SetRegistryKey(STYLE_KEY);

		LoadStdProfileSettings();  // Load standard INI file options (including MRU)

		SetRegistryBase (_T("Settings"));

		// Initialize all Managers for usage. They are automatically constructed
		// if not yet present
		InitContextMenuManager();
		InitKeyboardManager();
		//InitOEM();
		GetCursorResource();

		// Register the application's document templates.  Document templates
		//  serve as the connection between documents, frame windows and views.

		CMultiDocTemplate* pDocTemplate;
		pDocTemplate = new CMultiDocTemplate(
			IDR_WORKSPTYPE,
			RUNTIME_CLASS(CEtroExplorerDoc),
			RUNTIME_CLASS(CChildFrame), // custom MDI child frame
			RUNTIME_CLASS(CEtroExplorerView));
		AddDocTemplate(pDocTemplate);

		// create main MDI Frame window
        //vNVRConduction();
		CMainFrame* pMainFrame = new CMainFrame;
		if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
			return FALSE;
		m_pMainWnd = pMainFrame;

		// The main window has been initialized, so show and update it.
		//pMainFrame->ShowWindow(m_nCmdShow);
		//pMainFrame->UpdateWindow();

		//pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
		pMainFrame->BringWindowToTop();
		pMainFrame->SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE,0);
		//   // Parse command line for standard shell commands, DDE, file open
		//CCommandLineInfo cmdInfo;
		//ParseCommandLine(cmdInfo);

		//// Dispatch commands specified on the command line
		//if (!ProcessShellCommand(cmdInfo))
		//	return FALSE;
		pDocTemplate->OpenDocumentFile(NULL);

		pMainFrame->StartWizard();
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarApp message handlers

int CNVRApp::ExitInstance() 
{
	CMFCVisualManager::DestroyInstance( TRUE ); 
    bDivingWindows(true);
	return CWinAppEx::ExitInstance();
}

void CNVRApp::PreLoadState ()
{

	//GetContextMenuManager()->AddMenu (_T("My menu"), IDR_CONTEXT_MENU);

	// TODO: add another context menus here
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


CAboutDlg::CAboutDlg(): CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
    ON_NOTIFY(NM_CLICK, IDC_SYSLINK1, &CAboutDlg::OnNMClickSyslink1)
END_MESSAGE_MAP()

// App command to run the dialog
void CNVRApp::OnAppAbout()
{
    if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))// if Press Control then Open Etrocenter Folder
    {
        TCHAR apppath[MAX_PATH]={0};
		GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
		CString csAppPath(apppath);
        int nPos = csAppPath.ReverseFind('\\');
		csAppPath = csAppPath.Left(nPos + 1);
		ShellExecute(NULL,_T("open"),csAppPath,NULL,NULL,SW_SHOWNORMAL);        
    }else if (GetKeyState(VK_SHIFT)  & 0x8000)// if Press Control then Open Etrocenter Folder
    {
	    CRegKey regKey;
	    TCHAR szCurrentPath[1024]={0};
	    const DWORD len = 1024;
	    DWORD nActualLen(len);
        CString csForder(CProductDefine::m_tzRegistSubFolder);

        LONG nRet = regKey.Open(HKEY_LOCAL_MACHINE, csForder.GetBuffer());
	    if(nRet != ERROR_SUCCESS)
	    {
			    return; 
	    }

	    nRet = regKey.QueryValue(szCurrentPath, _T("Recording Path"), &nActualLen);
	    if(nRet != ERROR_SUCCESS)
	    {
		    return;
	    }
        ShellExecute(NULL,_T("open"), szCurrentPath, NULL, NULL, SW_SHOWNORMAL);        

    }
    else
    {
	    CAboutDlg aboutDlg;
	    UINT re = aboutDlg.DoModal();
    }
	//CAboutDlg aboutDlg;
	//aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CWorkSpaceToolBarApp message handlers

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    CStatic* pIconWnd = (CStatic*)GetDlgItem(IDC_STATIC_LOGO);
    HMODULE hResOem = ::LoadLibrary(_T("Manufacturer.DLL"));
    if (hResOem)
    {
        pIconWnd->SetIcon(::LoadIcon(hResOem, MAKEINTRESOURCE(IDR_MAINFRAME)));
        FreeLibrary(hResOem);
    }

    CString cs;
    cs.Format(_T("About %s"), CProductDefine::m_tzProductName);
    SetWindowText(cs);

//#ifdef GENERIC
    if (_tcscmp(CProductDefine::m_tzBrand, _T("Etrovision")) != 0)
        GetDlgItem(IDC_SYSLINK1)->ShowWindow(SW_HIDE);
//#endif	

    m_csVersion = _T("Version: ");
	vGetVersion();
	this->GetDlgItem(IDC_STATIC_ABOUT_VERSION)->SetWindowTextW(m_csVersion);
    this->CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnNMClickSyslink1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	ShellExecute(NULL, _T("open"), _T("http://www.etrovision.com"), NULL, NULL, SW_SHOWNORMAL);
}
void CAboutDlg::vGetVersion()
{
	// get the app path
	TCHAR apppath[MAX_PATH]={0};
	GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
	CString csAppPath(apppath);
  	
	// get version

	DWORD dwLen = 0;
	TCHAR* lpData=NULL;
    BOOL bSuccess = FALSE;
	dwLen = GetFileVersionInfoSize(csAppPath, 0);
 
   if (0 == dwLen)
	{
	  return ;
	}

    lpData =new TCHAR [dwLen+1]();

	GetFileVersionInfo(csAppPath, 0, dwLen, lpData);
	
	LPVOID lpBuffer = NULL;
    UINT uLen = 0;

   VerQueryValue(lpData,
              _T("\\StringFileInfo\\040904b0\\FileVersion"), //040904b0 can be found in resource view->version-> blockheader
              
     /*  properties for query 
    CompanyName
    FileDescription
    FileVersion
    InternalName
    LegalCopyright
    OriginalFilename
    ProductName
    ProductVersion
    Comments
    LegalTrademarks
    PrivateBuild
    SpecialBuild
     */             
              &lpBuffer,
              &uLen); 
	
   CString csVersion;
   csVersion.Format(_T("%s"),(TCHAR*)lpBuffer);

   m_csVersion += csVersion;
  
    delete [] lpData;

}


bool CNVRApp::bDivingWindows(bool bShowHide)
{
    int nShowWindowFlag = bShowHide ? SW_SHOWNORMAL : SW_HIDE;
    HWND hTray = FindWindow (TEXT("Shell_TrayWnd"), NULL);
	ShowWindow(hTray, nShowWindowFlag);
	HWND hStartButton = FindWindow (TEXT("Button"), NULL);
	ShowWindow(hStartButton, nShowWindowFlag);
    Desktop_Show_Hide(bShowHide);
    return true;
}
/*************************
 * Mouse Hook procedure. *
 * (Win 9x)              *
 *************************/
LRESULT CALLBACK MouseHookProc(int nCode, WORD wParam, DWORD lParam) 
{
	if(nCode >= 0)
	{
        if (wParam == WM_LBUTTONDBLCLK)
        {
            if (((MOUSEHOOKSTRUCT *)lParam)->hwnd == GetDesktopWindow())
            {
                return 1;
            }
        }
	}

    return CallNextHookEx(hHook, nCode, wParam, lParam); 
} 


/***********************************
 * Low Level Mouse Hook procedure. *
 * (Win NT4SP3+)                   *
 ***********************************/
LRESULT CALLBACK LowLevelMouseHookProc(int nCode, WORD wParam, DWORD lParam) 
{
	PMSLLHOOKSTRUCT p = (PMSLLHOOKSTRUCT)lParam;
	HWND hWnd = WindowFromPoint(p->pt);

	if(nCode >= 0)
	{
        if ((wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN) && hWnd == GetDesktopWindow())
        {
			return 1;
        }
	}

    return CallNextHookEx(hHook, nCode, wParam, lParam); 
} 


bool CNVRApp::Desktop_Show_Hide(bool bShowHide)	
{
    OSVERSIONINFO	osvi;
	BOOL			bIsWindowsNT4SP3orLater;
	int				iServicePack;
	TCHAR           *p;

	// Determine the current windows version
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	for (p = osvi.szCSDVersion; *p  && !isdigit(*p); *p++);
	iServicePack = _ttoi(p); 
	bIsWindowsNT4SP3orLater = (osvi.dwPlatformId == VER_PLATFORM_WIN32_NT) && 
		                      (((osvi.dwMajorVersion == 4) && (iServicePack >= 3)) || 
						      (osvi.dwMajorVersion > 4));

    if (!bShowHide)
    {
        if (!hHook)
		{
            hHook  = SetWindowsHookEx(bIsWindowsNT4SP3orLater ? WH_MOUSE_LL : WH_MOUSE, 
									  bIsWindowsNT4SP3orLater ? (HOOKPROC)LowLevelMouseHookProc : (HOOKPROC)MouseHookProc, 
                                      this->m_hInstance, 
                                      0);
			if (!hHook)
				return 0;
		}
    }
    else
    {
        UnhookWindowsHookEx(hHook);
        hHook = NULL;
    }
#define     PROGRAM_MANAGER _T("Program Manager")	// Program manager window name

    return SetWindowPos(FindWindow(NULL, PROGRAM_MANAGER),
                        NULL,
                        0, 0, 0, 0,
                        bShowHide ? SWP_SHOWWINDOW : SWP_HIDEWINDOW | SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER);
}
bool CNVRApp::bWindowsHotkey(bool bEnable)
{
    CRegKey regkey;
    LONG nRet = regkey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\Explorer"));
	if(nRet != ERROR_SUCCESS)
	{
		return false;
	}
    regkey.SetDWORDValue(_T("NoWinKeys"), bEnable);
    regkey.Close();
    nRet = regkey.Open(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Policies\\System"));
	if(nRet != ERROR_SUCCESS)
	{
		return false;
	}
    regkey.SetDWORDValue(_T("DisableLockWorkstation"), bEnable);
    regkey.Close();
        
}
bool CNVRApp::bLanchExplorer()
{
    TCHAR szParm[MAX_PATH]={};
    _stprintf(szParm, _T("explorer"));
    STARTUPINFO ssi={};
    PROCESS_INFORMATION ppi={};
    ZeroMemory( &ssi, sizeof(ssi) );
    ssi.cb = sizeof(ssi);
    return CreateProcess(NULL, szParm,NULL, NULL, FALSE, 0, NULL, NULL, &ssi, &ppi ) ;
}
bool CNVRApp::bSetWindowsShell(bool bExplorer)
{
    CRegKey regkey;
	CString csRegFolder=_T("Software\\Microsoft\\Windows NT\\CurrentVersion\\Winlogon");
	HKEY hkFolder;
	DWORD dwDisposition = REG_CREATED_NEW_KEY;

    LONG nRet = regkey.Open(HKEY_CURRENT_USER, csRegFolder);
	if(nRet != ERROR_SUCCESS)
	{
		return false;
	}
    if (bExplorer)
    {
        TCHAR szParm[MAX_PATH]={};
        _stprintf(szParm, _T("explorer"));
        STARTUPINFO ssi={};
        PROCESS_INFORMATION ppi={};
        ZeroMemory( &ssi, sizeof(ssi) );
        ssi.cb = sizeof(ssi);
	    CreateProcess(NULL, szParm,NULL, NULL, FALSE, 0, NULL, NULL, &ssi, &ppi ) ;

       // LONG nRet = regkey.DeleteValue(_T("Shell"));
        //bWindowsHotkey(true);
    }else
    {
        TCHAR apppath[MAX_PATH]={};
        GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);

		/*if(ERROR_SUCCESS ==RegCreateKeyEx(HKEY_CURRENT_USER, csRegFolder, NULL, _T("Shell"),
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hkFolder, &dwDisposition))
		{				
			const BYTE* lpData = (const BYTE*)(LPCTSTR) apppath;
			RegSetValueEx(hkFolder, _T("Shell"), NULL, REG_SZ, (BYTE*)apppath ,  (_tcslen(apppath)+1)*2 );
		}*/

        int nRet=regkey.SetStringValue(_T("Shell"), apppath);
        bWindowsHotkey(false);
    }
    regkey.Close();
}