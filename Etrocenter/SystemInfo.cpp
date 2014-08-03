// BarPtz.cpp: implementation of the CBarPtz class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DlgFlyAway.h"
#include "SystemInfo.h"
#include "processinfo.h"
//#include "psapi.h" 
//#include <stdio.h>
//#include <windows.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern 	LARGE_INTEGER   liOldIdleTime   =   {0,0};   
extern  LARGE_INTEGER   liOldSystemTime   =   {0,0};
#define LOGORECT CRect(50,5, 250,50)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSystemInfo::CSystemInfo()
:m_pNetsession(NULL)
{
 	/*CBrush m_brush(GLOBALBKCOLOR);*/
	m_brush.CreateSolidBrush(GLOBALBKCOLOR);
}

CSystemInfo::~CSystemInfo()
{

}

BEGIN_MESSAGE_MAP(CSystemInfo, CDockablePane)
	//{{AFX_MSG_MAP(CSystemInfo)
	ON_WM_CREATE()
//	ON_MESSAGE(MM_JOY1MOVE, OnJoyStickMove)
	//}}AFX_MSG_MAP
//	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
//	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_CONTROL(BN_CLICKED, IDB_LOGO, &CSystemInfo::vOnClickLogo)
	ON_CONTROL(WM_SETCURSOR, IDB_LOGO, &CSystemInfo::vOnClickLogo)
    ON_WM_SETCURSOR()
END_MESSAGE_MAP()


int CSystemInfo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)	return -1;

	// TODO: Add extra initialization here
 
	//Create Time Static to display current time
 
	
	m_curTime.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(140, 85, 330, 100), this, IDC_TIME);
	StartTimer(1000);

	//vGetLocal();

	/***** System Info ********/
	m_cpuProgress.Create( WS_CHILD | WS_VISIBLE, CRect(140, 70, 330, 80), this, IDC_CPUCTRL);
	m_cpuProgress.SetRange(0, 100);

	m_cpu.Create(_T(""), WS_CHILD | WS_VISIBLE, CRect(20, 65, 130, 85), this, IDC_CPU);
	m_BitmapLogo.Create(_T(""), WS_CHILD|WS_VISIBLE|SS_BITMAP|SS_CENTERIMAGE|SS_NOTIFY, LOGORECT, this, IDB_LOGO);
    HMODULE hResOem = ::LoadLibrary(_T("Manufacturer.DLL"));
    HMODULE hRes = hResOem ? hResOem : AfxGetResourceHandle();
    HBITMAP hbitmap = m_BitmapLogo.SetBitmap(::LoadBitmap(hRes, MAKEINTRESOURCE(IDB_LOGO))); 
    if (hResOem) FreeLibrary(hResOem);
	//m_BitmapLogo.LoadBitmaps(IDB_LOGO, IDB_LOGO, IDB_LOGO, IDB_LOGO);
	//m_BitmapLogo.SetIndex(300);
#ifndef GENERIC
	DWORD dwStyle = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW;
	//Help Bitmap Button, displayed for etrovision only
	m_help.Create(_T(""), dwStyle, CRect(319, 1, 339, 21), this, IDB_HELP);
	m_help.LoadBitmaps(IDB_HELP, IDB_HELP, IDB_HELP, IDB_HELP); 
	m_help.SetIndex(200);
#endif	 
	//Logo
	//CDC *pdc = GetDC();
	//CIniReader s_ini;
	//s_ini.setINIFullFileName(_T("\ec.ini"));

	//CString csLogo = s_ini.getKeyValue(_T("logo"),_T("system"));
	//
	////Defined custom logo
	//if (csLogo!=_T(""))	
	//{
	//	m_hBmpCustom =	 (HBITMAP) LoadImage(
	//		AfxGetInstanceHandle(),   // handle to instance
	//		csLogo,  // name or identifier of the image (root is where project is)
	//		IMAGE_BITMAP,        // image types
	//		0,     // desired width
	//		0,     // desired height
	//		LR_LOADFROMFILE); 
	//	m_CDCLogo.CreateCompatibleDC(pdc);
	//	m_CDCLogo.SelectObject(m_hBmpCustom);
	//}
	//else
	//{
	//	m_Logo.LoadBitmapW(IDB_LOGO);
	//	m_CDCLogo.CreateCompatibleDC(pdc);
	//	m_CDCLogo.SelectObject(&m_Logo);
	//}

	return 0;
}

 


void CSystemInfo::DoDataExchange(CDataExchange* pDX)
{
	//CDialog::DoDataExchange(pDX);
	////{{AFX_DATA_MAP(CDlgGroupName)
	//DDX_Text(pDX, IDC_EDIT_GROUPNAME, m_csGroupName);
	//DDV_MaxChars(pDX, m_csGroupName, 31);
	//}}AFX_DATA_MAP
}

void CSystemInfo::vOpenHelp()
{		
	ShellExecute(NULL,_T("open"),_T("etrocenter_help.chm"),NULL,NULL,SW_SHOWNORMAL);		
}

void CSystemInfo::vOnClickLogo()
{
    if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))// if Press Control then Open Etrocenter Folder
    {
        TCHAR apppath[MAX_PATH]={0};
		GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
		CString csAppPath(apppath);
        int nPos = csAppPath.ReverseFind('\\');
		csAppPath = csAppPath.Left(nPos + 1);
		ShellExecute(NULL,_T("open"),csAppPath,NULL,NULL,SW_SHOWNORMAL);        
    }else
    {
	    CAboutDlg aboutDlg;
	    UINT re = aboutDlg.DoModal();
    }
	
}

void CSystemInfo::vSetNetSession(CNetSession* pNetsession)
{
	m_pNetsession = pNetsession;
}

UINT  CSystemInfo::StartTimer (UINT TimerDuration)

    {
        UINT    TimerVal;
        TimerVal = SetTimer (IDT_TIMER, TimerDuration, NULL);  // Starting the Timer

        return TimerVal;
    }// end StartTimer


BOOL CSystemInfo::OnEraseBkgnd(CDC* pDC)
{
	CBrush brush(GLOBALBKCOLOR);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &brush);
	return true;
}

void CSystemInfo::OnPaint()
{
	CPaintDC dc(this); 
	int ll_ygap=20, ll_h=48;

	//dc.BitBlt(50,5, 250,50,&m_CDCLogo,0,0, SRCCOPY);		
}


void CSystemInfo::OnTimer(UINT_PTR nIDEvent)
{
	//CTime ct= CTime::GetCurrentTime();
    if (nIDEvent != IDT_TIMER) return;
	UpdateData();
	//m_sCurrtime.Format(_T("%02d - %02d - %04d     %02d : %02d : %02d "),  ct.GetMonth(), ct.GetDay(), ct.GetYear(), ct.GetHour(),ct.GetMinute(), ct.GetSecond());
	
	//m_curTime.SetWindowTextW(m_sCurrtime);
	vGetLocal();

	//CPU info
	int CPU = usageTotal.GetCpuUsage();

	static CString lstr;
	static CString strCPU;
	LOADSTRING(strCPU, IDC_CPU);
	lstr.Format(_T("%s %d%%"), strCPU,CPU);
	m_cpu.SetWindowText(lstr);
	 
	m_cpuProgress.SetPos(CPU);
	UpdateData(FALSE);			
}


HBRUSH CSystemInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
 
	// Call the base class implementation first! Otherwise, it may
   // undo what we're trying to accomplish here.
   HBRUSH hbr =  CDockablePane::OnCtlColor(pDC, pWnd, nCtlColor);

   //  We can use CWnd::GetDlgCtrlID() to perform the most efficient test.
   int ll_id = pWnd->GetDlgCtrlID();
   if (ll_id == IDC_TIME || ll_id == IDC_CPU)
   {
      // Set the text color to red
      pDC->SetTextColor(RGB(3,7,61));

      // Set the background mode for text to transparent so background will show thru.
      pDC->SetBkMode(TRANSPARENT);

      // Return handle to our CBrush object
      hbr = m_brush;
   }

   return hbr;

}

 

void CSystemInfo::OnSize(UINT nType, int cx, int cy)
{
	CRect rc;
	GetClientRect(rc);
	
	CDockablePane::OnSize(nType, cx, cy);
}


//
double CSystemInfo::CPU_Usage()
{
	SYSTEM_PERFORMANCE_INFORMATION   SysPerfInfo;   
	SYSTEM_TIME_INFORMATION   SysTimeInfo;   
	SYSTEM_BASIC_INFORMATION   SysBaseInfo;   
	double   dbIdleTime;   
	double   dbSystemTime;
	LONG   status;   

	dbIdleTime=0;
	PROCNTQSI   NtQuerySystemInformation; 
	NtQuerySystemInformation   =   (PROCNTQSI)GetProcAddress(GetModuleHandle( _T("ntdll") ), "NtQuerySystemInformation" );   
    
	if   (!NtQuerySystemInformation)   
	return 0;   
    
	//   get   number   of   processors   in   the   system   
	status   =   NtQuerySystemInformation(SystemBasicInformation,&SysBaseInfo,sizeof(SysBaseInfo),NULL);   
	if   (status   !=   NO_ERROR)   
	return 0; 

	//   get   new   system   time   
	status   =  NtQuerySystemInformation(SystemTimeInformation,&SysTimeInfo,sizeof(SysTimeInfo),0);   
	if   (status!=NO_ERROR)   
	return 0;   
    
	//   get   new   CPU's   idle   time   
	status   =	NtQuerySystemInformation(SystemPerformanceInformation,&SysPerfInfo,sizeof(SysPerfInfo),NULL);   
	if   (status   !=   NO_ERROR)   
	return 0;   
    
	//   if   it's   a   first   call   -   skip   it   
	if   (liOldIdleTime.QuadPart   !=   0)   
	{   
		//   CurrentValue   =   NewValue   -   OldValue   
		dbIdleTime   =   Li2Double(SysPerfInfo.liIdleTime)   -   Li2Double(liOldIdleTime);   
		dbSystemTime   =   Li2Double(SysTimeInfo.liKeSystemTime)   -     
    
		Li2Double(liOldSystemTime);   
    
		//   CurrentCpuIdle   =   IdleTime   /   SystemTime   
		dbIdleTime   =   dbIdleTime   /   dbSystemTime;   
    
		//   CurrentCpuUsage%   =   100   -   (CurrentCpuIdle   *   100)   /   NumberOfProcessors   
		dbIdleTime   =   100.0   -   dbIdleTime   *   100.0   /  (double)SysBaseInfo.bKeNumberProcessors   +   0.5;   
	}   
    
	//   store   new   CPU's   idle   and   system   time   
	liOldIdleTime   =   SysPerfInfo.liIdleTime;   
	liOldSystemTime   =   SysTimeInfo.liKeSystemTime;

	return dbIdleTime;
}



void CSystemInfo::vGetLocal()
{
// TODO: Add extra initialization here
	const int STR_SZ = 1024;
	TCHAR szBuf[STR_SZ] = { 0 };
	TCHAR szLocale[STR_SZ] = { 0 };

	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGLANGUAGE, szBuf, STR_SZ);
	_tcscpy(szLocale, szBuf);
	::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SENGCOUNTRY, szBuf, STR_SZ);
	if (_tcsclen(szBuf) != 0){
		_tcscat(szLocale, _T("_"));
		_tcscat(szLocale, szBuf);
	}
	::GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_IDEFAULTANSICODEPAGE, szBuf, STR_SZ);
	if (_tcsclen(szBuf) != 0){
		_tcscat(szLocale, _T("."));
		_tcscat(szLocale, szBuf);
	}

	{
		_tsetlocale(LC_ALL, szLocale); // e.g. szLocale = "English_United States.1252
	
		TCHAR strDate[STR_SZ];
		TCHAR strTime[STR_SZ];
		TCHAR str[STR_SZ];

		time_t lt;
		time (&lt);
		tm* timeptr = localtime (&lt);
		_tcsftime(strDate, STR_SZ, _T("%x"), timeptr); // Date representation for current locale
		_tcsftime(strTime, STR_SZ, _T("%X"), timeptr); // Time representation for current locale

		_tcscpy (str, strDate);
		_tcscat (str, _T("   "));
		_tcscat (str, strTime);
		
	m_curTime.SetWindowTextW(str);
	//	::SetDlgItemText (m_hWnd, IDC_STATIC_WIN32, str);
	}
}

BOOL CSystemInfo::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    
    if (pWnd == &m_BitmapLogo)
    {
        ::SetCursor(LoadCursor(NULL,IDC_HAND));
    }
    else
    {
        CDockablePane::OnSetCursor(pWnd, nHitTest, message);
    }
    return true;
}
