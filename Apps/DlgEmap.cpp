// DlgEmap.cpp : implementation file
//
#include "stdafx.h"
#include "MainFrm.h"
#include "DlgEmap.h"
#include "BaseRadioButton.h"
#include "resource.h"
#include "DlgSettingEvent.h"
#include "IntraEvent.h"

#define	STCW 150
#define	STCH 20
#define	COCKPITW 400
#define	VIDEOW 200
#define	VIDEOH 150
#define	SPLITW 150
#define	CAMSIZEW 38
#define	CAMSIZEH 23
#define GAPH 50


IMPLEMENT_DYNAMIC(CDlgEmap, CDialog)

static bool m_EventReceiveFlag;
const UINT ID_TIMER_EVENT1 = 0x1000;
const UINT ID_TIMER_EVENT2 = 0x2000;
const UINT ID_TIMER_EVENT3 = 0x3000;
const UINT ID_TIMER_EVENT4 = 0x4000;
const UINT ID_TIMER_EVENT5 = 0x5000;
const UINT ID_TIMER_EVENT6 = 0x6000;
const UINT ID_TIMER_TIME = 0x7000;
const UINT ID_TIMER_AC = 0x8000;


//-- Thread Flag
static bool ThreadFlag;

//static CTabpageEmapEvent	m_pageEvent;
//static CTabpageEmapAc		m_pageAc;

static CDatabase*		pDB;
//static CDatabase			m_pDB;
static vector<CEmapCam*> m_vpcam;
static vector<CEmapAccessCam*> m_vpAccessCam;

static HANDLE hThreadAccess;

static void vParseSegment(TCHAR* tszBuffer, map <CString, eVERSETS>::iterator pos);
static CString HexToAscii(CString s);

//static int notifyWriter(char* data, size_t size, size_t nmemb,void *lParam);

CDlgEmap::CDlgEmap(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgEmap::IDD, pParent),m_rect(0,0,0,0),m_nGroupIdx(0)
	//, m_StarInter()
{

}

//static int notifyWriter(char* data, size_t size, size_t nmemb,void* lParam);

CDlgEmap::~CDlgEmap()
{
//	delete m_pAc;
		//m_EventReceiveFlag = false;
		//fnFreeMemory();
	/*ThreadFlag = false;
	::CloseHandle(hStartThread);*/
}

void CDlgEmap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOGROUP, m_Group);
	//DDX_Control(pDX, IDC_BUTSAVE, m_butsave);
	//==============================================
	DDX_Control(pDX, IDC_STGROUP, m_stgroup);
	DDX_Control(pDX, IDC_LISTCAM, m_listcam);
	DDX_Control(pDX, IDC_LISTEVENT, m_listevent);
	DDX_Control(pDX, IDC_EDITFOLDER, m_editFolder);	
	DDX_Control(pDX, IDC_EDITMAP, m_editmap);
	DDX_Control(pDX, IDC_STFOLDER, m_stfolder);
	DDX_Control(pDX, IDC_STMAP, m_stmap);
	DDX_Control(pDX, IDC_STEMAP, m_map1);
	////DDX_Control(pDX, IDC_BUTBROWSE2, m_butfolder);

	DDX_Control(pDX, IDC_STCAM, m_stcam);

	DDX_Control(pDX, IDC_EDITSEC, m_editset);
	DDX_Control(pDX, IDC_STSEC, m_stsec);
	DDX_Control(pDX, IDC_BUTARDOWN, m_butArDown);

	DDX_Control(pDX, IDC_BUTCLEAR, m_butclear);
	DDX_Control(pDX, IDC_STEVENT, m_stevent);
 	DDX_Control(pDX, IDC_TABEVENTAC, m_tab);
}

BEGIN_MESSAGE_MAP(CDlgEmap, CBaseDialog)
	//ON_WM_CTLCOLOR()
	//ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_COMBOGROUP, &CDlgEmap::OnCbnSelchangeCombogroup)
	ON_BN_CLICKED(IDC_BUTFOLDER, &CDlgEmap::OnBnClickedButfolder)	
	ON_BN_CLICKED(IDC_BUTBROWSE, &CDlgEmap::OnBnClickedButbrowse)
	ON_BN_CLICKED(IDC_BUTSAVE, &CDlgEmap::OnBnClickedButsave)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LISTCAM, &CDlgEmap::OnLvnBegindragListcam)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()

	ON_BN_CLICKED(IDC_BUTEVENT, &CDlgEmap::OnBnClickedEvent)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
 	ON_BN_CLICKED(IDC_RADIOAUTO, &CDlgEmap::OnBnClickedRadioauto)
	ON_BN_CLICKED(IDC_RADIOMAN, &CDlgEmap::OnBnClickedRadioman)

	ON_BN_CLICKED(IDC_CKMD, &CDlgEmap::OnBnClickedCkmd)
	ON_BN_CLICKED(IDC_CKVL, &CDlgEmap::OnBnClickedCkvl)
	ON_BN_CLICKED(IDC_CKDI, &CDlgEmap::OnBnClickedCkdi)
	ON_BN_CLICKED(IDC_BUTARDOWN, &CDlgEmap::OnBnClickedButardown)

//	ON_BN_CLICKED(IDC_BUTSAVEAS, &CDlgEmap::OnBnClickedButsaveas)
	//void CDlgEmap::fnSaveEventLog()
	ON_WM_MOVE()
	//ON_BN_CLICKED(IDC_BUTCLEAR, &CDlgEmap::OnBnClickedButclear)
	//ON_BN_CLICKED(IDC_BUTREPORT, &CDlgEmap::OnBnClickedButreport)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TABEVENTAC, &CDlgEmap::OnTcnSelchangeTabeventac)
	ON_BN_CLICKED(IDC_CKAC, &CDlgEmap::OnBnClickedCkac)
	//ON_BN_CLICKED(IDC_BUTSAVEAS, &CDlgEmap::OnBnClickedButsaveas)
//	ON_BN_CLICKED(IDC_BUTCLEAR, &CDlgEmap::OnBnClickedButclear)
//ON_WM_SIZE()
ON_BN_CLICKED(IDC_BUTSAVEAS, &CDlgEmap::OnBnClickedButsaveas)
ON_BN_CLICKED(IDC_BUTCLEAR, &CDlgEmap::OnBnClickedButclear)
END_MESSAGE_MAP()

BOOL CDlgEmap::OnInitDialog()
{
	//mb_AC = true;
	//No more support Access Control in this screen
	mb_AC = false;
	CBaseDialog::OnInitDialog();
	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	m_pbk = CNVRBk::GetBkObject();

 	ib_dirty = false;
	m_bDragging = false;
	mc_folderMode = 'E';  //Empty
 
	ib_e1=false;
	ib_e2=false;
	ib_e3=false;
	ib_e4=false;
	ib_e5=false;
	ib_e6=false;

	 CRect rectTest(0,0,0,0);
 	 if(m_rect != rectTest)
		SetWindowPos(NULL,m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height(),SWP_SHOWWINDOW);

	fnInitControl();	//Initialize controls and positions
	fnInitFolder();
	fnInitGroup();
	fnLoadCamInfo();
	
	fnInitLocalize();
	 //Initialize Access Control and begin Thread
	 if (mb_AC)  fnInitAccess();

	 if (!mb_AC) fnDisplayCam(m_groupid); // No AC
 	return true;  
}

LRESULT CDlgEmap::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_NOTIFY)
	{
		if (wParam == IDC_SPLITH)
		{	
			SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;
			DoResizeH(pHdr->delta);
		}
	}
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

// CDlgEmap message handlers
bool CDlgEmap::fnInitFolder(void)
{
	//If there's no row in the DB ec_storage of EMAP, put the working folder as the store loaction
	CString	ls_loc;
	CString ls_sql = (_T("SELECT store_location FROM ec_storage  WHERE storage_type='EMAP'"));
	CRecordset	rsGroup(pDB);
	rsGroup.Open(CRecordset::forwardOnly, ls_sql);
	
	CDBVariant  varID;
	STRGROUP lGroup;

	while (!rsGroup.IsEOF()) { 
			rsGroup.GetFieldValue( _T("store_location"), ls_loc);	
			break;
	}
	if (ls_loc!=_T(""))  //Data in the ec_storage
	{
		m_editFolder.SetWindowTextW(ls_loc.Trim());
		m_Folder = ls_loc.Trim();
		mc_folderMode = 'A'; //Available
	}
	else
	{
		TCHAR apppath[MAX_PATH]={0};
		GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
		
		CString csAppPath(apppath);
		int nPos = csAppPath.ReverseFind('\\');
		csAppPath = csAppPath.Left(nPos);
		m_editFolder.SetWindowTextW(csAppPath);
		m_Folder = csAppPath;
		mc_folderMode = 'E';	//empty
	}
	return false;
}

bool CDlgEmap::fnInitControl(void)
{
	int ll_posx, ll_posy, ll_w;
	CRect lrect, llrect;
	USES_CONVERSION;
	//ShowWindow(SW_MAXIMIZE);
	mc_closevideo = 'M'; //Set manual

	GetClientRect(lrect);
	//lrect = m_rect;
	//llrect = m_rect;
	StartTimer();

	//MoveWindow(0,0, lrect.right, lrect.bottom);
	m_Group.GetClientRect(llrect);	
	ll_posx = lrect.right - llrect.Width() - 10;
	ll_posy = 30;
	ll_w = llrect.Width();

	//Save Button
	m_butsave.AutoLoad(IDC_BUTSAVE, this);
	m_butsave.MoveWindow( ll_posx, 2, 24, 24, true); 

	m_Group.MoveWindow( ll_posx, ll_posy, ll_w, llrect.Height(),true);
	m_stgroup.MoveWindow( ll_posx - 60, ll_posy, 50, 20, true); 

	DWORD dwStyle =WS_CHILD | WS_VISIBLE | BS_OWNERDRAW; ;

	GetDlgItem(IDC_STDVO)->ShowWindow(SW_HIDE);
	((CBaseRadioButton*)GetDlgItem(IDC_RADIOAUTO))->ShowWindow(SW_HIDE);
	((CBaseRadioButton*)GetDlgItem(IDC_RADIOMAN))->ShowWindow(SW_HIDE);

//	((CStatic*)GetDlgItem(IDC_STCLOSEVIDEO))->SetWindowTextW(_T("Liveview:"));
	((CStatic*)GetDlgItem(IDC_STCLOSEVIDEO))->MoveWindow( ll_posx - 60, ll_posy, 80, 20, true); 
	m_stsec.ShowWindow(SW_HIDE);
	m_editset.ShowWindow(SW_HIDE);

	GetDlgItem(IDC_CKDI)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CKVL)->ShowWindow(SW_HIDE);
	((CStatic*)GetDlgItem(IDC_STCLOSEVIDEO))->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_CKMD)->ShowWindow(SW_SHOW);

	//folder
	ll_posy += 30;
	m_stfolder.MoveWindow( ll_posx - 60, ll_posy, 60, 20, true); 
	m_editFolder.MoveWindow( ll_posx , ll_posy,   ll_w-50, llrect.Height(),true);
	m_butfolder.AutoLoad(IDC_BUTFOLDER,this);
	m_butfolder.MoveWindow( ll_posx + ll_w - 40, ll_posy-3, 24, 24, true); 


	//map file and Browse Button
	ll_posy += 30;
	m_editmap.MoveWindow( ll_posx , ll_posy,  ll_w-50, llrect.Height(),true);
	m_stmap.MoveWindow( ll_posx - 60, ll_posy, 50, 20, true); 

	m_butbrowse.AutoLoad(IDC_BUTBROWSE,this);
	m_butbrowse.MoveWindow( ll_posx + ll_w - 40, ll_posy-3, 24, 24, true); 


	//Close Video Section
	ll_posy += 25;
 
	m_butArDown.ShowWindow(SW_HIDE);
	 
	((CStatic*)GetDlgItem(IDC_STCLOSEVIDEO))->MoveWindow(ll_posx - 60, ll_posy, 70, 20, true);
	GetDlgItem(IDC_CKMD)->MoveWindow(ll_posx , ll_posy, 100, 20, true);
//	GetDlgItem(IDC_CKAC)->MoveWindow(ll_posx + 120, ll_posy, 130, 20, true);
	GetDlgItem(IDC_CKDI)->MoveWindow(ll_posx + 120, ll_posy, 130, 20, true);

	//Liveview Duration - hide for now
	//ll_posy += 25;
	//GetDlgItem(IDC_STDURATION)->MoveWindow(ll_posx , ll_posy, 100, 20, true);
	//GetDlgItem(IDC_COMBODURATION)->MoveWindow(ll_posx + 120 , ll_posy, 130, 20, true);
	GetDlgItem(IDC_STDURATION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBODURATION)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CKDI)->ShowWindow(SW_HIDE);

	m_ini.setINIFullFileName(_T("\ec.ini"));
	CString ls_cvideo = m_ini.getKeyValue(_T("closevideo"), _T("EMAP"));
	CString ls_videon = m_ini.getKeyValue(_T("video_on"), _T("EMAP"));
	fnVideoOn(ls_videon.Trim());
	mb_hideset = true;	

	//Filter Events 
	//fnBuildEventFilter(ll_posx, ll_posy);
	fnHideSetting(mb_hideset );

	ll_posy += 28;
	//Camera List
	m_stcam.MoveWindow( ll_posx-60 , ll_posy,  50, 20,true);
	m_listcam.GetClientRect(llrect);
	m_listcam.MoveWindow(ll_posx, ll_posy, ll_w, llrect.Height() + 60 , true);

	/*****************************  Event & Access  *******************************/
	ll_posy += llrect.Height() + 65;

	int ll_more = 70;

	if (mb_AC)
	{
		m_stevent.ShowWindow(SW_HIDE);
		m_listevent.ShowWindow(SW_HIDE);
		((CBitmapButton*)GetDlgItem(IDC_BUTSAVEAS))->ShowWindow(SW_HIDE);
		((CBitmapButton*)GetDlgItem(IDC_BUTCLEAR))->ShowWindow(SW_HIDE);
 
		m_tab.ShowWindow(SW_SHOW);
		m_tab.MoveWindow(ll_posx-70 , ll_posy + 10, ll_w + ll_more  , lrect.Height() - ll_posy - 20, true);
		m_tab.InsertItem(0, _T("Events"));
		m_tab.InsertItem(1, _T("Access Control"));

		m_pageEvent.Create(IDD_DLGEMAP_TABEVENT, &m_tab);
		m_pageAc.Create(IDD_DLGEMAP_TABAC, &m_tab);   

		   CRect	rc;
		 int ll_gap = 5;
		m_tab.GetClientRect(rc);
		//m_tab.GetWindowRect(rc);
		
		rc.top += 25;
		rc.bottom -= ll_gap;
		rc.left += ll_gap;
		rc.right -= ll_gap;

		m_pageEvent.MoveWindow(&rc);
		m_pageAc.MoveWindow(&rc);
	    
		pDialog[0] = &m_pageEvent;
		pDialog[1] = &m_pageAc;
	 
		pDialog[0]->ShowWindow(SW_SHOW);
		pDialog[1]->ShowWindow(SW_HIDE);
	 
		m_CurSelTab = 0;
	}
	else	//No Access Control
	{
		m_stevent.MoveWindow(ll_posx - ll_more , ll_posy + 12, ll_more,20);
		
		m_butsaveas.AutoLoad(IDC_BUTSAVEAS, this);
		m_butsaveas.MoveWindow(ll_posx, ll_posy + 7, 24,24);

		m_butclear.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL,IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL);
		m_butclear.MoveWindow(ll_posx + 30, ll_posy + 7, 24,24);
		m_butclear.BringWindowToTop();
		
		GetDlgItem(IDC_CKAC)->ShowWindow(SW_HIDE);	//Hide Access Control checkbox
		//GetDlgItem(IDC_CKDI)->ShowWindow(SW_SHOW);	//

		m_listevent.ShowWindow(SW_SHOW); //Old
		m_listevent.MoveWindow(ll_posx-ll_more, ll_posy+ 32, ll_w + ll_more  , lrect.Height() - ll_posy - 40 ,true);		
	}
	
     CRect	rc;
	 int ll_gap = 5;
   
    rc.top += 25;
    rc.bottom -= ll_gap;
    rc.left += ll_gap;
    rc.right -= ll_gap;

	//Split Control
	GetClientRect(rc);
	ll_posx = ll_posx - 80;

	m_splitV.Create(WS_CHILD | WS_VISIBLE, CRect( ll_posx , rc.top, ll_posx+ SPLITW ,rc.bottom), this, IDC_SPLITV);
	m_splitV.SetRange(0, 0, -1);

	m_splitH.Create(WS_CHILD | WS_VISIBLE, CRect( ll_posx + 10 , ll_posy  , rc.right , ll_posy ), this, IDC_SPLITH);
	m_splitH.SetRange(SPLITW, SPLITW, -1);

	LV_COLUMN ListColumn;

		//Get the group desc
		CComboBox*	lpGroup = ((CComboBox*)GetDlgItem(IDC_COMBOGROUP));
		CString	ls_group, ls_title;
		int nIndex;
		nIndex = lpGroup->GetCurSel();
				
		CString csLoc1(_T(""));//ipcam
		LOADSTRING(csLoc1, IDS_EMAPIPCAM);			

		CString csLoc2(_T(""));//id
		LOADSTRING(csLoc2, IDS_EMAPID);			

        WCHAR * ListTitles[3] = {_T(""), csLoc1.GetBuffer(), csLoc2.GetBuffer()};

		for(int i = 0; i < 3; i++)
		{
			ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			ListColumn.fmt = LVCFMT_LEFT;
			ListColumn.iSubItem = i;

			if(i == 1 )
			{
				ListColumn.cx = 240;
			}
			else if(i == 0)
				{
					ListColumn.cx = 16;
			}
			else if(i == 2)
				{ListColumn.cx = 0;
			}
			
			ListColumn.pszText = ListTitles[i];
			m_listcam.InsertColumn(i, &ListColumn);
		}

		dwStyle = m_listcam.GetStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		m_listcam.SetExtendedStyle(dwStyle);
		m_listcam.ModifyStyle(0, LVS_SINGLESEL);

		m_images.Create(16, 16, ILC_COLOR8, 0, 3);

		CBitmap bIconRed, bIconOrange, bIconYellow;
		bIconRed.LoadBitmapW(IDB_REDCIRCLE);
		bIconOrange.LoadBitmapW(IDB_ORANGECIRCLE);
		bIconYellow.LoadBitmapW(IDB_YELLOWCIRCLE);

		m_images.Add(&bIconRed, RGB(255, 255, 255));
		m_images.Add(&bIconOrange, RGB(0, 0, 0));
		m_images.Add(&bIconYellow, RGB(0, 0, 0));
		m_listevent.SetImageList(&m_images, LVSIL_SMALL);

	m_EventReceiveFlag = true;


	m_EmapEventRec = new CEventReceive();

	UINT nRetEvent(0);
	hThreadEventRec = (HANDLE)_beginthreadex(NULL, 0,  GetEventFromService, (LPVOID)this, 0, &nRetEvent);
		
	//Initialize Event List Control
	m_EventReceiveFlag= true;

	//Localize here
	CString csLoc3(_T("")); //Time
		LOADSTRING(csLoc3, IDS_EMAPTIME);			
	 

		CString csLoc4(_T(""));//event type
		LOADSTRING(csLoc4, IDS_EMAPEVENTTYPE);			
	 

        WCHAR * ListTitles1[5] = {_T(""),csLoc3.LockBuffer(), csLoc2.LockBuffer(), csLoc1.LockBuffer(), csLoc4.LockBuffer()};
	//WCHAR * ListTitles1[5] = {_T(""),_T("Time"), _T("ID"),_T("Camera IP"),_T("Event Type")};
	for(int i = 0; i < 5; i++)
		{
			ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			ListColumn.fmt = LVCFMT_LEFT;
			ListColumn.iSubItem = i;

			if (i==0)  ListColumn.cx = 20;
			else if (i==1)  ListColumn.cx = 73; //Time
			else if (i==2)  ListColumn.cx = 30; //ID
			else if (i==3)  ListColumn.cx = 85; // Camera IP
			else if (i==4)  ListColumn.cx = 135; //Event

			ListColumn.pszText = ListTitles1[i];
			m_listevent.InsertColumn(i, &ListColumn);
		}

		dwStyle = m_listevent.GetStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		m_listevent.SetExtendedStyle(dwStyle);
		m_listevent.ModifyStyle(0, LVS_SINGLESEL);

		m_SmallImg.Create(16, 16, ILC_COLOR | ILC_MASK, 0, 1);

		//List Event Bitmap Stuff
		CBitmap bIcon;
		bIcon.LoadBitmapW(IDB_CHECKRED);
		m_SmallImg.Add(&bIcon, RGB(0, 0, 0));
		m_listcam.SetImageList(&m_SmallImg, LVSIL_SMALL);

		m_maxMapW =  ll_posx - 5; // lrect.Width() - COCKPITW;
		m_maxMapH = lrect.Height();
		m_map1.MoveWindow(0, 0, m_maxMapW, m_maxMapH, true);
		
		//ToolTips
		 m_pToolTipCtrl = new CToolTipCtrl;
		if (!m_pToolTipCtrl->Create(this))
		{
			TRACE(_T("Unable To create ToolTip\n"));
		}
	 
		CString ls_st;
		LOADSTRING(ls_st, IDS_SELECTBMP);
		m_pToolTipCtrl->AddTool(&m_butbrowse, ls_st);
		m_pToolTipCtrl->BringWindowToTop();

		LOADSTRING(ls_st, IDS_SELECTFOLDER);
		m_pToolTipCtrl->AddTool(&m_butfolder, ls_st);

		LOADSTRING(ls_st, IDS_SAVE);		
		m_pToolTipCtrl->AddTool(&m_butsave, ls_st);

		LOADSTRING(ls_st, IDS_SETTINGEVENT);
		m_pToolTipCtrl->AddTool(&m_butevent, ls_st);

		LOADSTRING(ls_st, IDS_EMAPSETTING);
		m_pToolTipCtrl->AddTool(GetDlgItem(IDC_BUTARDOWN), ls_st);

		if (!mb_AC) 
		{
			LOADSTRING(ls_st, IDS_CLEAR_EVENT);
			m_pToolTipCtrl->AddTool(GetDlgItem(IDC_BUTCLEAR), ls_st);
			LOADSTRING(ls_st, IDS_EMAPSAVEAS);
			m_pToolTipCtrl->AddTool(GetDlgItem(IDC_BUTSAVEAS), ls_st);
		}
	//	delete wcsLoc1;
	//	//if (wcsLoc2) delete wcsLoc2;
	///*	if (wcsLoc3) delete wcsLoc3;
	//	if (wcsLoc4) delete wcsLoc4;*/
		m_pToolTipCtrl->Activate(TRUE);
		return false;
}

//Get Event From Service
UINT _stdcall CDlgEmap::GetEventFromService(LPVOID lpParam)
{
	CDlgEmap * pThis = (CDlgEmap*) lpParam;

	//pThis->m_EmapEventRec->vSetCallBack(&pThis->fnEmapCallBack, pThis);
	pThis->m_EmapEventRec->vSetCallBack(&pThis->fnEmapCallBack, pThis);
	pThis->m_EmapEventRec->vStartReceive();
	
	//CSA csa;
	//HANDLE hFreeEvent = ::CreateEventW(&csa.m_sa, false, false, _T("Global\\ETROCENTER_EVENT_FREE"));
	//::SetEvent(hFreeEvent);

	//while(m_EventReceiveFlag == true)
	//{
	//	CSA csa;
	//	HANDLE hStartEvent = ::CreateEventW(&csa.m_sa, false, false, _T("Global\\ETROCENTER_EVENT_START"));
	//	if(hStartEvent == NULL)  	TRACE(_T("Error code :%d"), ::GetLastError());

	//	HANDLE hEndEvent = ::CreateEventW(&csa.m_sa, false, false, _T("Global\\ETROCENTER_EVENT_END"));
	//	if(hEndEvent == NULL) 
	//			TRACE(_T("Error code :%d"), ::GetLastError());

	//	HANDLE hStartEndEvent;
	//	HANDLE hEventThread[2] = {hEndEvent, hStartEvent};

	//	DWORD dwResult = ::WaitForMultipleObjects(2, hEventThread, false, INFINITE);
	//	switch(dwResult)
	//	{
	//		case WAIT_OBJECT_0:
	//			//hStartEndEvent = ::CreateEventW(&csa.m_sa, false, false, _T("Global\\ETROCENTER_EVENT_END_GET"));
	//			//::SetEvent(hStartEndEvent);
	//			return 0;
	//			break;

	//		case WAIT_OBJECT_0 + 1:
	//				HANDLE hReceiveMapFile;
	//				LPCTSTR pBuffer;
	//				CString csEvent;

	//				hReceiveMapFile = ::OpenFileMapping(
	//									FILE_MAP_ALL_ACCESS,
	//									FALSE,
	//									_T("Global\\ETROCENTER_EVENT"));

	//				if(hReceiveMapFile == NULL)
	//				{
	//					//TRACE(_T("Error %d"), ::GetLastError());
	//					::CloseHandle(hReceiveMapFile);
	//				}	
	//				else
	//				{
	//					pBuffer = (LPCTSTR)::MapViewOfFile(hReceiveMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 256);
	//					
	//					//TRACE(pBuffer);
	//					TCHAR tcEvent[256];
	//					::_tcscpy(tcEvent, pBuffer);
	//					csEvent.Format((LPCTSTR)pBuffer);

	//					if(csEvent == _T(""))		return 0;
	//				
	//					CString ls_id=_T(""), ls_ip=_T(""), ls_eventType=_T("");
	//					pThis->ParseEvent(csEvent, ls_id, ls_ip, ls_eventType);
	//						::UnmapViewOfFile(pBuffer);
	//					::CloseHandle(hReceiveMapFile);

	//					HANDLE hFreeEvent = ::CreateEventW(NULL, false, false, _T("Global\\ETROCENTER_EVENT_FREE"));
	//					::SetEvent(hFreeEvent);							 
	//				}

	//				break;
	//	} //switch(dwResult)
	//} //while(m_EventReceiveFlag == true)
	return 0;

}

// Load all the camera info into noteitem
bool CDlgEmap::fnLoadCamInfo(void)
{
	USES_CONVERSION;
	HTREEITEM lprvItem;

	char cIp[16];
	CDBVariant varCamidx, varStreamPort, varCamIdx, varCamid,varWebPort;
	CString ls_sql, ls_desc, ls_camname, ls_ipaddress, ls_macaddress, ls_idx, ls_username, ls_password, ls_sprotocol,ls_webPort;
	CString csStreamName;
	
	vector<ec_Camera> vcCam;
	vector<Cam_Group_Cam> vcGroupCam;
	map<int, vector<ec_Stream> > mapStream;

	m_pbk->GetAllCameras(vcCam);
	m_pbk->GetAllStreamByCamid(mapStream);
	
	int nIdx = 0, nCount = vcCam.size();
	BYTE byDevice = 0;

	for (nIdx=0 ; nIdx<nCount ; nIdx++)
	{
		byDevice = m_pbk->GetDevice(vcCam[nIdx].video_format);
		if (byDevice == eNVR || byDevice == IPVSTATION)
		{
			if (m_pbk->IsChannel(vcCam[nIdx].video_format) == false)
			{
				continue;
			}
		}

		vcGroupCam.clear();
		m_pbk->GetGroupCamera(vcCam[nIdx].cameraid,vcGroupCam,false);
		vcCam[nIdx].vcStream = mapStream[vcCam[nIdx].cameraid];
		
		NODEITEM* pcamInfo = new NODEITEM;
		::ZeroMemory(pcamInfo, sizeof(NODEITEM));
		
		if(vcGroupCam.size())
			pcamInfo->nStreamID = vcGroupCam[0].streamid;

		pcamInfo->cameraid = vcCam[nIdx].cameraid;
		pcamInfo->camera_idx = vcCam[nIdx].camera_idx;

		strncpy(pcamInfo->camera_name , W2A(vcCam[nIdx].cameraname), vcCam[nIdx].cameraname.GetLength());
		_tcscpy(pcamInfo->wcAlias, vcCam[nIdx].cameraname.GetBuffer(32));	

		strncpy(pcamInfo->ip , W2A(vcCam[nIdx].ipaddress), vcCam[nIdx].ipaddress.GetLength());
		m_pbk->TransString2Mac(vcCam[nIdx].mac_address, pcamInfo->mac);
		pcamInfo->nWebPort = vcCam[nIdx].httpport;

		strncpy(pcamInfo->szUser, W2A(vcCam[nIdx].username), vcCam[nIdx].username.GetLength());
		strncpy(pcamInfo->szPass, W2A(vcCam[nIdx].password), vcCam[nIdx].password.GetLength());
		
		if(vcCam[nIdx].vcStream.size() >= pcamInfo->nStreamID && pcamInfo->nStreamID)
			pcamInfo->nAPPort = vcCam[nIdx].vcStream[pcamInfo->nStreamID-1].stream_port;
		else
			pcamInfo->nAPPort = DEFAULTSPORT;
		
		pcamInfo->nProtocol = m_pbk->GetStreamProtocol(vcCam[nIdx],pcamInfo->nStreamID-1);
		strncpy(pcamInfo->modelname, W2A(vcCam[nIdx].model_code), sizeof(vcCam[nIdx].model_code));  

		_tcscpy(pcamInfo->stream_url, vcCam[nIdx].stream_url.GetBuffer());	
		m_vctpAllCam.push_back(pcamInfo);
	}
	
	return false;
}



// Populate groups into combobox
bool CDlgEmap::fnInitGroup(void)
{
	STRGROUP lGroup;
	vector<Cam_Group> vcGroup;
	m_pbk->GetGroup(DeviceTree, vcGroup, false);
	int nIdx = 0, nCount = vcGroup.size();
	m_groupid = 0;
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		lGroup.nid = vcGroup[nIdx].cameragroupid;
		lGroup.mapfile = vcGroup[nIdx].map_file;
		lGroup.Desc = vcGroup[nIdx].camera_group_desc;

		m_Group.InsertString(nIdx,  vcGroup[nIdx].camera_group_desc);	
		m_pvtGroup.push_back(lGroup);
	}	

	fnmmcheck();
	//m_Group.SetCurSel(0);
	fnGroupChanged();
	return false;
}

bool CDlgEmap::fnGroupChanged(void)
{
		int nIndex = m_Group.GetCurSel();
		CString strGroup,strWFile, wholeFile;
		m_Group.GetLBText(nIndex, strGroup);

		fnDeleteAllCam();
		ib_dirty = false;  //prevent resave 
		//Get selected Group info from m_pvtGroup
		m_groupid = m_pvtGroup[nIndex].nid;
		m_editmap.SetWindowTextW(m_pvtGroup[nIndex].mapfile);   //put file into

		 //fnVideoOn(m_pvtGroup[nIndex].videoon);	//Check the appopriate checkbox

		if (m_groupid >0) vPopulateCam(m_groupid); //display cameras in the list control

		wholeFile.Format(_T("%s\\%s"),m_Folder, m_pvtGroup[nIndex].mapfile.Trim());

		HBITMAP hbm = (HBITMAP) m_map1.GetBitmap();
		BITMAP bm;
		SIZE szBitmap;
		GetObject(hbm, sizeof(bm), &bm);
		CBitmap hbmp;
		 HBITMAP hbitmap;

		if (!fnFileExist(wholeFile)) 
		{
			m_map1.ShowWindow(SW_HIDE);
			return false;
		}
		m_map1.ShowWindow(SW_SHOW);
		 hbitmap=(HBITMAP)::LoadImage(::AfxGetInstanceHandle(),wholeFile, IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);

		   hbmp.Attach(hbitmap);
		   hbmp.GetBitmap(&bm);

		  mi_orgwidth = bm.bmWidth;
		  mi_orgheight = bm.bmHeight;
	
		  //if (mi_orgwidth > 
		fnMaximizeBmp( mi_orgwidth, mi_orgheight);
		m_map1.MoveWindow(0, 0,  mi_orgwidth,  mi_orgheight , true);
		m_map1.Load(wholeFile);

		//Repaint Cameras in map
		if (mb_AC)
			fnDisplayCamAC(m_groupid);
		else
			fnDisplayCam(m_groupid); // No AC
	
	return false;
}

	//Populate the cameras into list control 
void CDlgEmap::vPopulateCam(int o_groupid)
{
	CString ls_sql1, ls_sql2, ls_sql3, ls_sql, ls_name, ls_ip, ls_full;

	int cnt=0;	
	int nItem;
	CString ls_gcamid;
	byte byDeviceType = 0;

	ec_Camera cam;
	vector<Cam_Group_Cam> vcGroupCam;
	m_listcam.DeleteAllItems();
	m_mapListCamIdx.clear();

	m_pbk->GetGroupCamera(o_groupid,vcGroupCam);
	int nIdx = 0, nCount = vcGroupCam.size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		cam.clear();
		m_pbk->GetCamera(vcGroupCam[nIdx].cameraid,cam);

		byDeviceType = m_pbk->GetDevice(cam.video_format);
		if (byDeviceType == eNVR || byDeviceType == IPVSTATION)
		{
			if (m_pbk->IsChannel(cam.video_format) == FALSE)
			{
				continue;
			}
		}
		ls_full.Format( _T("(%d) %s - %s"), cam.camera_idx, cam.cameraname, cam.ipaddress);

		nItem = m_listcam.InsertItem(nIdx, _T(""), 1);
		m_listcam.SetItemText(nItem, 1, ls_full);

		ls_gcamid.Format(_T("%d"),vcGroupCam[nIdx].cameragroupcameraid);
		m_listcam.SetItemText(nItem, 2, ls_gcamid);

		m_mapListCamIdx[nIdx]=cam.camera_idx;
	}
}





// Delete all cameras icons in map
void CDlgEmap::fnDeleteAllCam(void)
{
	//Delete Camera icons
	for (int ci=0; ci < m_vpcam.size() ; ci++)
	{
				CEmapCam* lpcam =  m_vpcam[ci];
				lpcam->DestroyWindow();
				m_vpcam[ci]->DestroyWindow();
				delete lpcam;
	}
	m_vpcam.clear();//.empty();

	//Delete Static name
	for (int ci=0; ci < m_vpName.size() ; ci++)
	{
				CBaseStatic* lpName =  m_vpName[ci];
				lpName->DestroyWindow();
				m_vpName[ci]->DestroyWindow();
				delete lpName;	
 
	}
	m_vpName.clear();
}

void CDlgEmap::fnMaximizeBmp(int &o_maxW, int &o_maxH)
{
	//Return the Max size allowed	
	if (o_maxW > m_maxMapW) o_maxW=m_maxMapW;
	if (o_maxH > m_maxMapH) o_maxH = m_maxMapH;
}

bool CDlgEmap::fnFileExist(CString o_file)
{	 
	// Data structure for FindFirstFile
		WIN32_FIND_DATA findData;

		// Clear find structure
		ZeroMemory(&findData, sizeof(findData));

		// Search the file
		HANDLE hFind = FindFirstFile( o_file, &findData );
		if ( hFind == INVALID_HANDLE_VALUE )
			return false;		// File not found 			
		
		FindClose( hFind );// Release find handle
		hFind = NULL;

		return true;
}

void CDlgEmap::DoResizeH(int delta)
{
		CRect rc;
		 
		CSplitterControl::ChangeHeight(&m_listcam, delta);
	 
		if (mb_AC)
		{
			//resize tab control and tab page
			CSplitterControl::ChangeHeight(&m_tab, -delta, CW_BOTTOMALIGN);
			CSplitterControl::ChangeHeight(&m_pageEvent, -delta, CW_TOPALIGN);
			CSplitterControl::ChangeHeight(&m_pageAc, -delta, CW_TOPALIGN);
		}
		else
		{
			CSplitterControl::ChangePos(&m_butclear, 0, delta);
			CSplitterControl::ChangePos(&m_butsaveas, 0, delta);
			CSplitterControl::ChangePos(&m_stevent, 0, delta);
			CSplitterControl::ChangeHeight(&m_listevent, -delta, CW_BOTTOMALIGN);
		}

		Invalidate();
		UpdateWindow();
}

// Display camera incons into map - NO AC
bool CDlgEmap::fnDisplayCam(int oGroupid)
{
	//Display camera icons into the bitmap
	CString cssql, cssql1, cssql2, cssql3;
	int emapx, emapy, ll_cnt=0;
 
	//Loop into ec_camera_group_camera and display the camer icons
	// Old cssql.Format(_T("SELECT ec_camera_group_camera.emap_x, ec_camera_group_camera.emap_y, ec_camera_group_camera.cameragroupcameraid, ec_camera.cameraname, ec_camera.camera_idx FROM ec_camera, ec_camera_group_camera WHERE  ec_camera.cameraid=ec_camera_group_camera.cameraid AND cameragroupid=%d AND emap_x>0 AND emap_y>0 ORDER BY ec_camera_group_camera.cameraid"), oGroupid);
	cssql1 = _T("SELECT ec_camera_group_camera.emap_x, ec_camera_group_camera.emap_y, ec_camera_group_camera.cameragroupcameraid, ec_camera.cameraname, ec_camera.camera_idx ");
	cssql2 =  _T(" FROM ec_camera_group_camera INNER JOIN ec_camera ON ec_camera.cameraid=ec_camera_group_camera.cameraid  WHERE cameragroupid=");
	cssql3 = _T("  AND emap_x>0 AND emap_y>0 ORDER BY ec_camera_group_camera.cameraid ");

	cssql.Format( _T("%s%s%d%s"), cssql1, cssql2, oGroupid, cssql3);
	CRecordset	rsCam(pDB);
	rsCam.Open(CRecordset::forwardOnly, cssql);
	
	CDBVariant  varEmapx, varEmapy, varNull, varid, varidx, varaccameraid;
	STRGROUP lGroup;

	while (!rsCam.IsEOF()) { 
			varEmapx = varNull;

			rsCam.GetFieldValue( _T("emap_x"), varEmapx, SQL_C_SSHORT);	
			emapx = varEmapx.m_iVal;

			rsCam.GetFieldValue( _T("emap_y"), varEmapy, SQL_C_SSHORT);	
			emapy= varEmapy.m_iVal;
	
			//Add camera based on the DB setting...
			if (emapx<0 || emapy<0)
			{
			}
			else
			{
					CRect lrect;
					int lid;					
					CString ls_name, ls_label;

					m_map1.GetWindowRect(&lrect);
					//if (lrect.left>0) emapx = emapx + lrect.left;

					CEmapCam* lpCam= new CEmapCam;	
					lpCam->Create(_T(""), WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(emapx, emapy, CAMSIZEW, CAMSIZEH), this, ll_cnt);
					lpCam->LoadBitmaps(IDB_CAM37, IDB_CAM37SEL, IDB_CAM37SEL, IDB_CAM37);
//					lpCam->vSetParent(_T("SETTING"));
					lpCam->MoveWindow(emapx, emapy, CAMSIZEW,CAMSIZEH,true);

					rsCam.GetFieldValue( _T("cameragroupcameraid"), varid, SQL_C_SSHORT);
					rsCam.GetFieldValue( _T("cameraname"), ls_name);
					rsCam.GetFieldValue( _T("camera_idx"), varidx, SQL_C_SSHORT);										lid = varid.m_iVal;
					
					//Assign camera info into CEmapCam
					lpCam->m_camgroupid = lid;
					lpCam->m_idx = varidx.m_iVal;
					lpCam->m_mapx = emapx;
					lpCam->m_mapy = emapy;
					lpCam->m_title = ls_name.Trim();

					for (map<int, int>::iterator it=m_mapListCamIdx.begin(); it != m_mapListCamIdx.end(); it++)
					{
						if (it->second == lpCam->m_idx)
						{
							m_listcam.SetItem(it->first, 0, LVIF_PARAM | LVIF_TEXT|LVIF_IMAGE, _T(""), 0,0,0, NULL);
							break;
						}
					}

			 		lpCam->m_accameraid = 0;
					
					emapy= varEmapy.m_iVal;;
					ll_cnt++;
					m_vpcam.push_back(lpCam);

					CBaseStatic* lpTxt = new CBaseStatic;
					ls_label.Format(_T("(%d) %s"), varidx.m_iVal, ls_name.Trim());

					lpTxt->Create( ls_label, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(0, 0, STCW, STCH), this, 2);
					lpTxt->m_id = lid;
					lpTxt->MoveWindow(emapx+40, emapy, STCW, STCH, true);
					lpTxt->ShowWindow(1);
					m_vpName.push_back(lpTxt);
					fnDisplayVideo( varidx.m_iVal);
			}

			rsCam.MoveNext();
	
	} //while loop

	return false;
}



// Display camera incons into map
bool CDlgEmap::fnDisplayCamAC(int oGroupid)
{
	//Display camera icons into the bitmap
	CString cssql, cssql1, cssql2, cssql3;
	int emapx, emapy, ll_cnt=0;
 
	//Loop into ec_camera_group_camera and display the camer icons
	// Old cssql.Format(_T("SELECT ec_camera_group_camera.emap_x, ec_camera_group_camera.emap_y, ec_camera_group_camera.cameragroupcameraid, ec_camera.cameraname, ec_camera.camera_idx FROM ec_camera, ec_camera_group_camera WHERE  ec_camera.cameraid=ec_camera_group_camera.cameraid AND cameragroupid=%d AND emap_x>0 AND emap_y>0 ORDER BY ec_camera_group_camera.cameraid"), oGroupid);
	cssql1 = _T("SELECT ec_camera_group_camera.emap_x, ec_camera_group_camera.emap_y, ec_camera_group_camera.cameragroupcameraid, ec_camera.cameraname, ec_camera.camera_idx,ecac_camera.ac_cameraid ");
	cssql2 =  _T(" FROM ec_camera_group_camera INNER JOIN ec_camera ON ec_camera.cameraid=ec_camera_group_camera.cameraid  LEFT OUTER JOIN ecac_camera ON ec_camera_group_camera.camera_idx = ecac_camera.camera_idx WHERE cameragroupid=");
	cssql3 = _T("  AND emap_x>0 AND emap_y>0 ORDER BY ec_camera_group_camera.cameraid ");

	cssql.Format( _T("%s%s%d%s"), cssql1, cssql2, oGroupid, cssql3);
	CRecordset	rsCam(pDB);
	rsCam.Open(CRecordset::forwardOnly, cssql);
	
	CDBVariant  varEmapx, varEmapy, varNull, varid, varidx, varaccameraid;
	STRGROUP lGroup;

	while (!rsCam.IsEOF()) { 
			varEmapx = varNull;

			rsCam.GetFieldValue( _T("emap_x"), varEmapx, SQL_C_SSHORT);	
			emapx = varEmapx.m_iVal;

			rsCam.GetFieldValue( _T("emap_y"), varEmapy, SQL_C_SSHORT);	
			emapy= varEmapy.m_iVal;
	
			//Add camera based on the DB setting...
			if (emapx<0 || emapy<0)
			{
			}
			else
			{
					CRect lrect;
					int lid;					
					CString ls_name, ls_label;

					m_map1.GetWindowRect(&lrect);
					//if (lrect.left>0) emapx = emapx + lrect.left;

					CEmapCam* lpCam= new CEmapCam;	
					lpCam->Create(_T(""), WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(emapx, emapy, CAMSIZEW, CAMSIZEH), this, ll_cnt);
					lpCam->LoadBitmaps(IDB_CAM37, IDB_CAM37SEL, IDB_CAM37SEL, IDB_CAM37);
//					lpCam->vSetParent(_T("SETTING"));
					lpCam->MoveWindow(emapx, emapy, CAMSIZEW,CAMSIZEH,true);

					rsCam.GetFieldValue( _T("cameragroupcameraid"), varid, SQL_C_SSHORT);
					rsCam.GetFieldValue( _T("cameraname"), ls_name);
					rsCam.GetFieldValue( _T("camera_idx"), varidx, SQL_C_SSHORT);
					
					//Access Control
					rsCam.GetFieldValue( _T("ac_cameraid"), varaccameraid, SQL_C_SSHORT);	
					lid = varid.m_iVal;
					
					//Assign camera info into CEmapCam
					lpCam->m_camgroupid = lid;
					lpCam->m_idx = varidx.m_iVal;
					lpCam->m_mapx = emapx;
					lpCam->m_mapy = emapy;
					lpCam->m_title = ls_name.Trim();

					if (varaccameraid.m_iVal>0){
						//Create Access Icons here
						lpCam->m_accameraid = varaccameraid.m_iVal;
						CEmapAccessCam*	lpAcCam = new CEmapAccessCam;

						lpAcCam->m_camidx = varidx.m_iVal;
						lpAcCam->Create(_T(""), WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(emapx, emapy-CAMSIZEH, CAMSIZEH, CAMSIZEH), this, ll_cnt);
						lpAcCam->LoadBitmaps(IDB_KEYB, IDB_KEYB,IDB_KEYB,IDB_KEYB);
						lpAcCam->MoveWindow(emapx, emapy - CAMSIZEH, CAMSIZEH,CAMSIZEH,true);
						m_vpAccessCam.push_back(lpAcCam);
					}
					else
						lpCam->m_accameraid = 0;
					
					emapy= varEmapy.m_iVal;;
					ll_cnt++;
					m_vpcam.push_back(lpCam);

					CBaseStatic* lpTxt = new CBaseStatic;
					ls_label.Format(_T("(%d) %s"), varidx.m_iVal, ls_name.Trim());

					lpTxt->Create( ls_label, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(0, 0, STCW, STCH), this, 2);
					lpTxt->m_id = lid;
					lpTxt->MoveWindow(emapx+40, emapy, STCW, STCH, true);
					lpTxt->ShowWindow(1);
					m_vpName.push_back(lpTxt);
			}

			rsCam.MoveNext();
	
	} //while loop

	return false;
}



void CDlgEmap::OnCbnSelchangeCombogroup()
{
	// TODO: Add your control notification handler code here
	fnCloseVideo();
	fnGroupChanged();
}

// Close all the videos
bool CDlgEmap::fnCloseVideo(int o_camidx)
{
	 
	//Loop through m_vctpAllCam and find the camidx
	int ll_camx=0, ll_camy=0;
	//check if the camera within the group, if not, don't need to display video
	if (!fnCamWithinGroup(0, o_camidx)) return false;

	//if that video already p[laying, return 
	for (int i=0; i<m_vpEventVideo.size(); i++)
	{
		if (m_vpEventVideo[i]->m_camidx == o_camidx) //Same Camera
		{
			 CEventVideo*	lpVideo;
			 lpVideo = m_vpEventVideo[i];
			 if (lpVideo->mc_mode == 'P')
			 {				
				 CVideoThin* lpVideo = ((CVideoThin*)&(m_vpEventVideo[i]->m_videovv));
				lpVideo->vStopPlay();
				lpVideo->vStopRecv();
				lpVideo->DestroyWindow();
				m_vpEventVideo[i]->CloseWindow();
				m_vpEventVideo[i]->DestroyWindow();
				m_vpEventVideo[i]->mc_mode = 'C';
			}
		}
	}
	return false;
}

bool CDlgEmap::fnCloseVideo(void)
{
	for (int i=0; i<m_vpEventVideo.size(); i++) 
	{
		if (m_vpEventVideo[i]->mc_mode == 'P') //Play
		{
			CVideoThin* lpVideo = ((CVideoThin*)&(m_vpEventVideo[i]->m_videovv));
			lpVideo->vStopPlay();
			lpVideo->vStopRecv();
			lpVideo->DestroyWindow();
			m_vpEventVideo[i]->CloseWindow();
			m_vpEventVideo[i]->DestroyWindow();
			m_vpEventVideo[i]->mc_mode = 'C';
		}
	}

	for (int i=0; i<m_vpEventVideo.size(); i++)
	{
		if (m_vpEventVideo[i]) 
		{
			delete m_vpEventVideo[i];
		}
	}
	m_vpEventVideo.clear(); 
	return false;
}


// check if the camera in the current selected group, if yes return true, else false
bool CDlgEmap::fnCamWithinGroup(int oGroupid, int oCamidx)
{
	try{
		if (oGroupid<=0) oGroupid = m_groupid;
		CDBVariant  varEmapx, varEmapy;
		CString ls_sql;
		ls_sql.Format(_T("SELECT ec_camera_group_camera.cameragroupcameraid, emap_x, emap_y  FROM ec_camera_group_camera  INNER JOIN ec_camera ON ec_camera_group_camera.cameraid = ec_camera.cameraid WHERE ec_camera_group_camera.camera_idx = %d and cameragroupid=%d and ec_camera.active_='Y' and ec_camera_group_camera.emap_x>0 and ec_camera_group_camera.emap_y>0"), oCamidx, oGroupid);
		CRecordset	rsGroup(pDB);
		rsGroup.Open(CRecordset::forwardOnly, ls_sql);

		while (!rsGroup.IsEOF()) { 
			return true;
		}
		return false;
	}
	catch(CDBException * p) 	{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
		return false;
}

//void CDlgEmap::OnBnClickedButsaveas()
//{
//	fnSaveEventLog();
//}

//Let Arrow button to handle ...
//void CDlgEmap::OnBnClickedButemapset()
//{
//	// TODO: Add your control notification handler code here
//	//CRect	 lrect, rc, src;
//	//ScreenToClient(src);
//	//m_listcam.GetWindowRect(lrect);
//	//m_listcam.GetClientRect(rc);
//
//	//int	iShowHide = SW_SHOW;
//
//	//if (mb_hideset)
//	//		mb_hideset = false;
//	//else
//	//	mb_hideset =true;
//	//
//	//fnHideSetting(mb_hideset);
//}

void CDlgEmap::fnHideSetting(bool oHide)
{		
		return;
		CWnd* pParent = m_listcam.GetParent();
		int dy ;

		if (pParent && ::IsWindow(pParent->m_hWnd))
		{
			CRect rcWnd;
			m_listcam.GetWindowRect(rcWnd);
			pParent->ScreenToClient(rcWnd);
			if (!oHide) dy = -30;
			else dy = 30;
			rcWnd.top -= dy;
			m_listcam.MoveWindow(rcWnd);
			m_stcam.MoveWindow(rcWnd.left - 60, rcWnd.top, 50, 20);
		}

		int ll_showhide =SW_HIDE ;
		if (!oHide)
			ll_showhide = SW_SHOW;

		/*GetDlgItem(IDC_STDVO)->ShowWindow(ll_showhide);*/
		GetDlgItem(IDC_CKMD)->ShowWindow(ll_showhide);
		//GetDlgItem(IDC_CKVL)->ShowWindow(ll_showhide);
		//GetDlgItem(IDC_CKDI)->ShowWindow(ll_showhide);
		//Hide it for the version
		GetDlgItem(IDC_CKAC)->ShowWindow(ll_showhide);
		((CStatic*)GetDlgItem(IDC_STCLOSEVIDEO))->ShowWindow(ll_showhide);
	/*	GetDlgItem(IDC_CKDI)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CKVL)->ShowWindow(SW_HIDE);*/
		//m_stsec.ShowWindow(ll_showhide);
		//GetDlgItem(IDC_STCLOSEVIDEO)->ShowWindow(ll_showhide);
		//GetDlgItem(IDC_RADIOMAN)->ShowWindow(ll_showhide);
		//GetDlgItem(IDC_RADIOAUTO)->ShowWindow(ll_showhide);
		//m_editset.ShowWindow(ll_showhide);
		/*m_stsec.ShowWindow(ll_showhide);*/
}

//Build Event filter from table ec_hevent_type into vector
//void CDlgEmap::fnBuildEventFilter(int oPosX, int oPosY)
//{
//	CString csSQL, csCode, csDesc;
//	int	ll_chw = 100, ll_chh=15;
//	int	ll_posx = oPosX, ll_posy = oPosY-30; 
//
//	//GetDlgItem(IDC_STDVO)->MoveWindow(oPosX, oPosY+20, 270, 63, false); //Groupbox
//	//GetDlgItem(IDC_STCLOSEVIDEO)->MoveWindow(ll_posx - 60, ll_posy, ll_chw, ll_chh, false);
//	//GetDlgItem(IDC_CKMD)->MoveWindow(ll_posx + 7, ll_posy, ll_chw, ll_chh, false);
//	//GetDlgItem(IDC_CKAC)->MoveWindow(ll_posx + 135, ll_posy, ll_chw, ll_chh, false);
//	//GetDlgItem(IDC_CKDI)->MoveWindow(ll_posx + 7, ll_posy+60, ll_chw, ll_chh, false);
//	//GetDlgItem(IDC_CKAC)->MoveWindow(ll_posx + 135, ll_posy+60, ll_chw, ll_chh, false);
//}

void CDlgEmap::OnBnClickedCkmd()
{
	// TODO: Add your control notification handler code here
	ib_dirty = true;
	mc_fileMode = 'C';
}

void CDlgEmap::OnBnClickedCkvl()
{
	// TODO: Add your control notification handler code here
	ib_dirty = true;
	mc_fileMode = 'C';
}

void CDlgEmap::OnBnClickedCkdi()
{
	// TODO: Add your control notification handler code here
	ib_dirty = true;
	mc_fileMode = 'C';
}

 void CDlgEmap::fnResumeCam(int o_camidx)
{
	TRACE (_T("Resume Camera\r\n"));
	if (o_camidx<1) return;
	m_currGCamid =  o_camidx;
	
	for (int ci=0; ci < m_vpcam.size() ; ci++)
	{
		CEmapCam* lpcam =  m_vpcam[ci];
		if (lpcam->m_idx == o_camidx)				
		{	
			lpcam->LoadBitmaps(IDB_CAM37, IDB_CAM37SEL,IDB_CAM37SEL,IDB_CAM37);
			return;
		}
	}
}

void CDlgEmap::fnRemoveCam(int o_camidx)
{
	for (int i=0; i<m_vpcam.size(); i++)
	{	
		if (o_camidx == m_vpcam[i]->m_idx)
			{
				CEmapCam* lpCam = m_vpcam[i];
				lpCam->mb_dirty = true;
				lpCam->mc_mode = 'D'; //Delete			
				lpCam->DestroyWindow();		
				m_vpcam[i]->DestroyWindow();	
				ib_dirty = true;
				break;
			}
	}

	for (int i=0; i<m_vpName.size(); i++)
	{	
		if (o_camidx == m_vpName[i]->m_id)
		{
				CBaseStatic* lpName = m_vpName[i];
				lpName->DestroyWindow();	
				m_vpName[i]->DestroyWindow();
				break;
		}
	}
}

void CDlgEmap::OnBnClickedButsave()
{
	if (ib_dirty) fnSaveDB();
 }

bool CDlgEmap::fnSaveDB(void)
{ 
	try{
		CString ls_sql, ls_folder, ls_file;
				
		//Save Folder
		if (mc_folderMode=='C' || mc_folderMode=='E') //Save ec_storage only if folder is empty
		{
			m_editFolder.GetWindowTextW(ls_folder);
			if (mc_folderMode=='E')
				ls_sql.Format(_T("INSERT INTO ec_storage (storage_type, store_location) VALUES ('EMAP', '%s') "), ls_folder  );
			else	//Changed Mode
				ls_sql.Format(_T("UPDATE ec_storage SET store_location = '%s'  WHERE  storage_type='EMAP'"), ls_folder  );
			//csSQLInsert.Format(_T("INSERT INTO ec_event_log (eventid, event_type_code, alert_level, occur_time, device_id, event_source_code, device_action_code) VALUES ('%s' ,'%s', '%s', '%s', '%s', '%s', '%s')"), m_CameraEvent.csEventID, csDBEventType, csAlertLevel, csDBTime, csCameraIDX, m_CameraEvent.csEventSourceCode, m_CameraEvent.csDeviceActionCode);
	
			TRACE(ls_sql);
			pDB->ExecuteSQL(ls_sql);
			mc_folderMode=='A';	//Available Mode
		}

		//Save File
		if (mc_fileMode=='C') //file changed
		{
		

			((CEdit*)GetDlgItem(IDC_EDITMAP))->GetWindowTextW(ls_file);
			ls_sql.Format(_T("UPDATE ec_camera_group SET map_file='%s'  WHERE cameragroupid = %d "), ls_file.Trim(), m_groupid   );
 
			TRACE(ls_sql);
			pDB->ExecuteSQL(ls_sql);
			//Update m_pvtGroup
			for (int i=0; i<m_pvtGroup.size(); i++)
			{
				if (m_pvtGroup[i].nid == m_groupid)
				{
					m_pvtGroup[i].mapfile = ls_file.Trim();
					//m_pvtGroup[i].videoon = ls_videoon.Trim();
					break;
				}
			}
		}

		//Save Camera Location
		for (int i=0; i<m_vpcam.size(); i++)
		{
			CEmapCam* lpCam = m_vpcam[i];		
			if (lpCam->m_mapx==0 && lpCam->m_mapy==0) continue;
			if (!lpCam->mb_dirty) continue;

			if (lpCam->mc_mode =='A')
				ls_sql.Format(_T("UPDATE ec_camera_group_camera SET emap_x=%d, emap_y=%d  WHERE cameragroupcameraid = %d "), lpCam->m_mapx, lpCam->m_mapy,  lpCam->m_camgroupid   );
			else if (lpCam->mc_mode =='D')
				ls_sql.Format(_T("UPDATE ec_camera_group_camera SET emap_x=NULL, emap_y=NULL  WHERE cameragroupcameraid = %d "), lpCam->m_camgroupid   );
			
			TRACE(ls_sql);
			pDB->ExecuteSQL(ls_sql);
			if (lpCam->mc_mode =='D') fnGroupChanged();
		}

		//	Video On setting into INI file
			CString	ls_videoon=_T("");
			CString	ls_md=_T("");
			CString	ls_vl =_T("");
			CString	ls_di = _T("");
			CString	ls_ac = _T("");
			if (((CButton*)GetDlgItem(IDC_CKMD))->GetCheck()==1) ls_md=_T("M");
			if (((CButton*)GetDlgItem(IDC_CKVL))->GetCheck()==1)  ls_vl =_T("L");
			if (((CButton*)GetDlgItem(IDC_CKDI))->GetCheck()==1)  ls_di =_T("I");
			if (((CButton*)GetDlgItem(IDC_CKAC))->GetCheck()==1)  ls_ac =_T("A");
			ls_videoon.Format(_T("%s%s%s%s"),ls_md, ls_vl, ls_di,ls_ac);
			m_ini.setKey(ls_videoon, _T("Video_On"),_T("EMAP")); 
	}
	catch(CDBException * p)
	{
				AfxMessageBox(p->m_strError);
				p->Delete();
	}

	return false;
}

void CDlgEmap::OnBnClickedRadioauto()
{
	// TODO: Add your control notification handler code here
	// enable the second edit
	GetDlgItem(IDC_EDITSEC)->EnableWindow(1);
	mc_closevideo = 'A';
	ib_dirty = true;
}

void CDlgEmap::OnBnClickedRadioman()
{
	// TODO: Add your control notification handler code here
	// Disable the second edit
	GetDlgItem(IDC_EDITSEC)->EnableWindow(0);
	mc_closevideo = 'M';

	//Make EventVideo Delete
	for (int i=0; i<m_vpEventVideo.size(); i++) 
	{
		m_vpEventVideo[i]->mc_mode = 'D';
	}
	ib_dirty = true;
}

void CDlgEmap::OnBnClickedButbrowse()
{
	// TODO: Add your control notification handler code here
		CString	ls_tfolder =_T("");
 
		CFileDialog fOpenDlg(TRUE, _T("bmp"), _T("Emap File"), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, 	_T("Select Bitmap File (*.bmp)|*.bmp|"), this);
	  
		fOpenDlg.m_pOFN->lpstrTitle=_T("Select Bitmap File");
	  
		CString ls_appPath = ((CMainFrame*)AfxGetMainWnd())->m_csAppPath;
		if (ls_appPath !=_T(""))
				fOpenDlg.m_pOFN->lpstrInitialDir = ls_appPath;
		else
				fOpenDlg.m_pOFN->lpstrInitialDir=_T("c:");
		//fOpenDlg.m_pOFN->lpstrInitialDir=_T("c:");
	  
		if(fOpenDlg.DoModal()==IDOK)
		{
				CString ls_path = fOpenDlg.GetPathName();
				CString ls_name = fOpenDlg.GetFileName();
				CString ls_sfile, ls_tfile;
	
				((CEdit*)GetDlgItem(IDC_EDITMAP))->SetWindowTextW(ls_name.Trim());

				fnLoadBmp(ls_path.Trim());
				m_curFile = ls_path.Trim();

				m_editFolder.GetWindowTextW(ls_tfolder);
				ls_sfile.Format(_T("%s\\%s"),ls_tfolder,ls_name);		
				bool ls_ret = CopyFile(ls_path,ls_sfile, false); 
				mc_fileMode = 'C';
				ib_dirty = true;
		}
}

bool CDlgEmap::fnLoadBmp(CString o_file)
{
		HBITMAP hbm = (HBITMAP) m_map1.GetBitmap();
		BITMAP bm;
		SIZE szBitmap;
		GetObject(hbm, sizeof(bm), &bm);
		CBitmap hbmp;
		 HBITMAP hbitmap;

		 hbitmap=(HBITMAP)::LoadImage(::AfxGetInstanceHandle(), o_file, IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);

		hbmp.Attach(hbitmap);
		hbmp.GetBitmap(&bm);
	
		mi_orgwidth = bm.bmWidth;
		mi_orgheight = bm.bmHeight;
	
		//If the bmp is too big, load the m_map1 into the maximum allowed
		fnMaximizeBmp(mi_orgwidth, mi_orgheight );

		m_map1.MoveWindow(0, 0, mi_orgwidth,  mi_orgheight ,true);
		m_map1.Load(o_file);
		m_map1.ShowWindow(SW_SHOW);

	return false;
}

void CDlgEmap::OnBnClickedButfolder()
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	LPMALLOC pMalloc = NULL;
	LPITEMIDLIST pidl = NULL;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));

	// set the bi's default values
	bi.hwndOwner = m_hWnd;
	bi.lpszTitle = _T("Select Emap storage folder:");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
 
	pidl = SHBrowseForFolder(&bi);
	if(pidl != NULL)
	{
//		SHGetPathFromIDList(pidl, strMDBPath.GetBuffer(strMDBPath.GetLength()));
		 CString pszPath;
		// free memory
		if(SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)
		{
			SHGetPathFromIDListW(pidl, pszPath.GetBuffer(256));
			//((CEdit*)GetDlgItem(IDC_EDITFOLDER))->SetWindowTextW(pszPath.Trim());
			m_editFolder.SetWindowTextW(pszPath.Trim());
			ib_dirty = true;
			if (mc_folderMode=='E')
			{}
			else	//Change folder
				mc_folderMode='C';
			
			pMalloc->Free(pidl);  
			pMalloc->Release(); 
		}
	} //	if(pidl != NULL)
}


void CDlgEmap::OnLvnBegindragListcam(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	//// TODO: Add your control notification handler code here
	//*pResult = 0;

		//This routine sets the parameters for a Drag and Drop operation.
	//It sets some variables to track the Drag/Drop as well
	// as creating the drag image to be shown during the drag.
	
	//Don't let it drag if map not exist...
	if (!fnCheckMapExist()) return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	//// Save the index of the item being dragged in m_nDragIndex
	////  This will be used later for retrieving the info dragged
	m_nDragIndex = pNMListView->iItem;
	CBitmap bitmap;
	//// Create a drag image
	POINT pt;
	int nOffset = -10; //offset in pixels for drag image
	pt.x = nOffset;
	pt.y = nOffset;

	//Get Drag Camera text
	m_dragTxt= m_listcam.GetItemText(m_nDragIndex,1);
	m_currGCamid = _ttoi(m_listcam.GetItemText(m_nDragIndex,2));

	//Return if camera already exist in the map
	if (fnCheckCamExist(m_currGCamid , m_nDragIndex))
		return;

	//We will call delete later (in LButtonUp) to clean this up
	m_pDragImage = m_listcam.CreateDragImage(m_nDragIndex, &pt);
	ASSERT(m_pDragImage); //make sure it was created
	bitmap.LoadBitmap(IDB_CAM16);
	m_pDragImage->Replace(0, &bitmap, &bitmap);

	//// Change the cursor to the drag image
	////	(still must perform DragMove() in OnMouseMove() to show it moving)
	m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);
	
	//// Set dragging flag and others
	CListCtrl	m_listR;
	m_bDragging = TRUE;	//we are in a drag and drop operation
	m_nDropIndex = -1;	//we don't have a drop index yet
	m_pDragList = &m_listR; //make note of which list we are dragging from
	m_pDropWnd = &m_listR;	//at present the drag list is the drop list

	//// Capture all mouse messages
	SetCapture ();

	*pResult = 0;

}

bool CDlgEmap::fnCheckCamExist(int o_camGroupid, int o_camIdx)
{
	CString ls_debug;
	ls_debug.Format(_T("Groupid: %d;  Camidx: %d"), o_camGroupid, o_camIdx);
	//AfxMessageBox( ls_debug);

	for (int i=0; i<m_vpcam.size(); i++)
	{
		if (m_vpcam[i]->m_camgroupid==o_camGroupid)
		{
			//AfxMessageBox(_T(" return true"));
			return true;

		}
	}
	//AfxMessageBox(_T(" return false"));
	return false;
}

void CDlgEmap::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_bDragging)
	{
		//// Move the drag image
		CPoint pt(point);	//get our current mouse coordinates
		ClientToScreen(&pt); //convert to screen coordinates
		m_pDragImage->DragMove(pt); //move the drag image to those coordinates
		// Unlock window updates (this allows the dragging image to be shown smoothly)
		m_pDragImage->DragShowNolock(false);

		//// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT(pDropWnd); //make sure we have a window

		//// If we drag outside current window we need to adjust the highlights displayed
		if (pDropWnd != m_pDropWnd)
		{
			if (m_nDropIndex != -1) //If we drag over the CListCtrl header, turn off the hover highlight
			{
				TRACE("m_nDropIndex is -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				VERIFY (pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED));
				// redraw item
				VERIFY (pList->RedrawItems (m_nDropIndex, m_nDropIndex));
				pList->UpdateWindow ();
				m_nDropIndex = -1;
			}
			else //If we drag out of the CListCtrl altogether
			{
			/*	TRACE("m_nDropIndex is not -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				int i = 0;
				int nCount = pList->GetItemCount();
				for(i = 0; i < nCount; i++)
				{
					pList->SetItemState(i, 0, LVIS_DROPHILITED);
				}
				pList->RedrawItems(0, nCount);
				pList->UpdateWindow();*/
			}
		}

		// Save current window pointer as the CListCtrl we are dropping onto
		m_pDropWnd = pDropWnd;

		// Convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient(&pt);

		//If we are hovering over a CListCtrl we need to adjust the highlights
		//if(pDropWnd->IsKindOf(RUNTIME_CLASS (CPictureCtrl)))
		//{			
		//	//Note that we can drop here
		//	SetCursor(LoadCursor(NULL, IDC_ARROW));
		//	UINT uFlags;
		//	CPictureCtrl* pPC= (CPictureCtrl*)pDropWnd;
		//}
		//else
		{
			//If we are not hovering over a CListCtrl, change the cursor
			// to note that we cannot drop here
			SetCursor(LoadCursor(NULL, IDC_NO));
		}
		// Lock window updates
		m_pDragImage->DragShowNolock(true);
	}
		
}

void CDlgEmap::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//CBaseDialog::OnLButtonUp(nFlags, point);

	//This routine is the end of the drag/drop operation.
	//When the button is released, we are to drop the item.
	//There are a few things we need to do to clean up and
	// finalize the drop:
	//	1) Release the mouse capture
	//	2) Set m_bDragging to false to signify we are not dragging
	//	3) Actually drop the item (we call a separate function to do that)

	//If we are in a drag and drop operation (otherwise we don't do anything)
	if (m_bDragging)
	{
		// Release mouse capture, so that other controls can get control/messages
		ReleaseCapture ();

		// Note that we are NOT in a drag operation
		m_bDragging = FALSE;

		// End dragging image
		m_pDragImage->DragLeave (GetDesktopWindow ());
		m_pDragImage->EndDrag ();
		delete m_pDragImage; //must delete it because it was created at the beginning of the drag

		CPoint pt (point); //Get current mouse coordinates

		ClientToScreen(&pt); //Convert to screen coordinates
		// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd); //make sure we have a window pointer

		bool lb_drop= fnDropWithin(&point);
 
		if (lb_drop)
		{

		//	LVITEM itm;
		//	char szBuff[20];
		//	BOOL result;
	/*		itm.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE ;
			itm.iSubItem = 0;
			itm.cchTextMax = sizeof(szBuff);*/
			//result = m_listcam.GetItem(&itm);
 
			CEmapCam* lpCam= new CEmapCam;	
			lpCam->Create(_T(""), WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(0, 0, CAMSIZEW, CAMSIZEH), this, 1);
			lpCam->LoadBitmaps(IDB_CAM37, IDB_CAM37SEL, IDB_CAM37SEL, IDB_CAM37);
 
			lpCam->MoveWindow(point.x, point.y, CAMSIZEW,CAMSIZEH,true);

			CRect lrect;
			int ll_relx=0;
			m_map1.GetWindowRect(&lrect);
			ll_relx = pt.x;
			if (lrect.left>0) ll_relx = pt.x - lrect.left;

			lpCam->m_mapx = point.x; //ll_relx;// - CAMSIZE; 
			lpCam->m_mapy = point.y; //pt.y; //- CAMSIZE;
			lpCam->m_camgroupid = m_currGCamid;
			lpCam->m_idx = m_currGCamid;
			lpCam->mb_dirty = true;
			lpCam->mc_mode = 'A';
			m_vpcam.push_back(lpCam);

			CBaseStatic* lpTxt = new CBaseStatic;
			lpTxt->Create(m_dragTxt, WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(0, 0, 100, 26), this, 2);
			if (m_currGCamid>0)
				lpTxt->m_id = m_currGCamid;
			lpTxt->MoveWindow(point.x + CAMSIZEW, point.y, 130, CAMSIZEH, true);
			lpTxt->ShowWindow(1);
			m_vpName.push_back(lpTxt);

			//Make List Camera checked
			LVITEMW* pLitem = new LVITEM;
			pLitem->mask = LVIF_PARAM | LVIF_TEXT|LVIF_IMAGE;
			pLitem->iItem = m_nDragIndex;

			m_listcam.SetItem(m_nDragIndex, 0, LVIF_PARAM | LVIF_TEXT|LVIF_IMAGE, _T(""), 0,0,0, NULL);
			fnDisplayVideo(m_currGCamid);
		}
	}

	m_bDragging = false;
	m_dragTxt = _T("");
	ib_dirty = true;
	CDialog::OnLButtonUp(nFlags, point);
}

void CDlgEmap::fnAfterVideoClose(int o_camidx)
{
	for (int i=m_vpEventTimer.size(); i<0; i--)
		{
			if (m_vpEventTimer[i].iIndex ==o_camidx ) 
			{
				m_vpEventTimer.erase(m_vpEventTimer.begin() + i);
			}
	}
 }

//Open Event Setting Screen
void CDlgEmap::OnBnClickedEvent()
{
	DlgSettingEvent *eventDlg = new DlgSettingEvent;
	eventDlg->DoModal();
	delete eventDlg;
}

// Free Memory
void CDlgEmap::fnFreeMemory(void)
{
//	if(m_ToolTip) delete m_ToolTip;
	if (m_pToolTipCtrl ) delete m_pToolTipCtrl ;
	//if (m_pEcBaseTab) delete m_pEcBaseTab;

	for (int i=0; i<m_vctpAllCam.size(); i++)
	{
		if (m_vctpAllCam[i]) delete m_vctpAllCam[i];
	}
	m_vctpAllCam.clear(); 

	for (int i=0; i<m_vpEventVideo.size(); i++)
	{
		if (m_vpEventVideo[i]) 
		{
			delete m_vpEventVideo[i];
		}
	}
	m_vpEventVideo.clear(); 

	//Event Filter
	for (int i=0; i<m_pvtEventFilter.size(); i++)
	{
		if (m_pvtEventFilter[i]) 			
			delete m_pvtEventFilter[i];
	}
	m_pvtEventFilter.clear();

	//Access Cam
	for (int i=0; i<m_vpAccessCam.size(); i++)
	{
		if (m_vpAccessCam[i]) 			
			delete m_vpAccessCam[i];
	}
	m_vpAccessCam.clear();
}

void CDlgEmap::ParseEvent(CString lParam, CString osIndex, CString osIP, CString osEventType)
{
	CString csEvent; //, csEventIP, csEventType, csCamIDX;
	
	TCHAR tcEvent[512]={0};

	::_tcscpy(tcEvent, lParam);

	TCHAR *tcToken={0};

	tcToken = wcstok(tcEvent, _T(","));
	osIndex.Format(_T("%s"), tcToken);
	//csCamIDX.Format(_T("%s"), tcToken);

	tcToken = wcstok(NULL, _T(","));
	osIP.Format(_T("%s"), tcToken);
	//csEventIP.Format(_T("%s"), tcToken);

	tcToken = wcstok(NULL, _T(","));
	osEventType.Format(_T("%s"), tcToken);

	if (m_EventReceiveFlag == false) return;
	//Display into Event Tabpage
	//m_pageEvent.fnDisplayEvent(osIndex, osIP, osEventType, m_curHMS);
	
	//Do rest of the stuff...display video timer...etc
	EventDisplay( osIndex, osIP, osEventType	);
}

bool CDlgEmap::EventDisplay(CString oCamIdx, CString oCamIP, CString oEventType)
{
		
	if (!mb_AC)
	{
		//===========Obsolete =============================================================//
		// Use the LV_ITEM structure to insert the items
		LVITEM lvi;
		CString strItem;
 
		////To prevent event triggered after window closed
		if (m_EventReceiveFlag == false) return false;
		//// Insert the first item
		lvi.mask =  LVIF_IMAGE | LVIF_TEXT;
 
		lvi.iItem = 0;
		lvi.iSubItem = 0;
		lvi.pszText = _T("");

		////If there's no row defined in the DB (priority -1), ignore the event
		int ll_priority=0;
		//ll_priority = CheckEventPriority(oCamIdx, oEventType);

		//ll_priority--; //the imagelist is the priority - 1
		m_listevent.InsertItem(0,_T(""),2); //Use information for now

 	//	// Set subitem 1
		strItem.Format(_T("%s"),m_curHMS );
		lvi.iSubItem = 1;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_listevent.SetItem(&lvi);

		// Set subitem 2
		strItem.Format(_T("%s"), oCamIdx);
		lvi.iSubItem =2;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_listevent.SetItem(&lvi);
		
		// Set subitem 3
		strItem.Format(_T("%s"),  oCamIP );
		lvi.iSubItem =3;
		lvi.pszText = (LPTSTR)(LPCTSTR)(strItem);
		m_listevent.SetItem(&lvi);

		// Set subitem 4
		strItem.Format(_T("%s"),  oEventType );
		lvi.iSubItem =4;
		CString ls_type = SmartEventType(strItem, true);
		lvi.pszText = (LPTSTR)(LPCTSTR)(ls_type);
		m_listevent.SetItem(&lvi);
	}
	else
	{
		m_pageEvent.fnDisplayEvent(oCamIdx, oCamIP, oEventType, m_curHMS);
	}
		//If there's no row defined in the DB (priority -1), ignore the event
		//int ll_priority=0;
		//ll_priority = CheckEventPriority(oCamIdx, oEventType);
		//HighlightCam(_ttoi(oCamIdx), ll_priority);
		
		//Filter video on, if true - keep going; false-return and NOT display
		if (!fnFilterVideoOn(oEventType)) return false;
		
		//Need to find a 0 spot for index
		bool lb_found = false;

		//Use 6 event timer to prevent events crash
		int	ll_timerlen = 200;
		if (!ib_e1)	{
			SetTimer( ID_TIMER_EVENT1, ll_timerlen, 0 );
			m_EventTimer.iTimer = 1;
			ib_e1 = true;
		}
		else if (!ib_e2){
			SetTimer( ID_TIMER_EVENT2, ll_timerlen, 0 );
			m_EventTimer.iTimer = 2;
			ib_e2= true;
		}
		else if (!ib_e3){
			SetTimer( ID_TIMER_EVENT3, ll_timerlen, 0 );
			m_EventTimer.iTimer = 3;
			ib_e3 = true;
		}
		else if (!ib_e4)	{
			SetTimer( ID_TIMER_EVENT4, ll_timerlen, 0 );
			m_EventTimer.iTimer = 4;
			ib_e4=true;
		}
		else if (!ib_e5) {
			SetTimer( ID_TIMER_EVENT5, ll_timerlen, 0 );
			m_EventTimer.iTimer = 5;
			ib_e5 = true;
		}
		else if (!ib_e6){
			SetTimer( ID_TIMER_EVENT6, ll_timerlen, 0 );
			m_EventTimer.iTimer = 6;
			ib_e6 = true;
		}

		m_EventTimer.iIndex = _ttoi(oCamIdx);
		m_EventTimer.cMode = 'O'; //Open Video
		m_vpEventTimer.push_back(m_EventTimer);
		return true;
}

int CDlgEmap::CheckEventPriority(CString lCamIdx, CString lEventType)
{
		//Get Store location from DB
		int ll_retLevel= -1;
		CString ls_eventType=_T("");
		CDBVariant  varLevel;
		ls_eventType  = SmartEventType(lEventType);

		if(ls_eventType.GetLength() <= 0)	ls_eventType = _T("MD");

	try{

		CString csSQL;
		csSQL.Format(_T("SELECT alert_level FROM ec_camera_event  WHERE camera_idx =%s AND event_type_code ='%s'"), lCamIdx, ls_eventType);
		CRecordset	rsLoc(pDB);
		rsLoc.Open(CRecordset::forwardOnly, csSQL);
			 
		while (!rsLoc.IsEOF()) { 
				rsLoc.GetFieldValue( _T("alert_level"), varLevel, SQL_C_SSHORT);
				if (varLevel.m_iVal<0) return -1;
				ll_retLevel = varLevel.m_iVal;
				break;
		}
	
	}catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return ll_retLevel;
}

CString CDlgEmap::SmartEventType(CString oOrgEventType)
{
	return SmartEventType(oOrgEventType, false);
}

//oLong true will return long string ex. Motion Detect
CString CDlgEmap::SmartEventType(CString oOrgEventType, bool obLong)
{
	CString ls_ret=_T("");
	oOrgEventType = oOrgEventType.MakeLower();

	if ( oOrgEventType.Find(_T("input")) != -1)  //Digital Input
	{	
		if (obLong) 
			ls_ret = _T("Digital Input");
		else
			ls_ret = _T("DI");
	}
	else if ( oOrgEventType.Find(_T("signal")) != -1) //Video Loss
	{
		if (obLong) 
			ls_ret = _T("Video Loss");
		else
			ls_ret = _T("VL");
	}
	else if ( oOrgEventType.Find(_T("output")) != -1)  //Digital Output
	{
		if (obLong) 
			ls_ret = _T("Digital Output");
		else
			ls_ret = _T("DO");
	}
	else if (( oOrgEventType.Find(_T("md")) != -1 ) || ( oOrgEventType.Find(_T("motion detect")) != -1 ))  //Motion Detect
	{
		if (obLong) 
			ls_ret = _T("Motion Detect");
		else
			ls_ret = _T("MD");
	}
	else if ( oOrgEventType.Find(_T("video open")) != -1)  //Video Open
	{
		if (obLong) 
			ls_ret = _T("Video Open");
		else
			ls_ret = _T("VO");
	}
else if ( oOrgEventType.Find(_T("video loss")) != -1)  //Video Loss
	{
		if (obLong) 
			ls_ret = _T("Video Loss");
		else
			ls_ret = _T("VL");
	}
	return ls_ret;
}


//Highlight Camera
bool CDlgEmap::HighlightCam(int oCamIdx, int oPriority)
{
	for (int ci=0; ci < m_vpcam.size() ; ci++)
	{
				CEmapCam* lpcam =  m_vpcam[ci];
				if (lpcam->m_idx == oCamIdx)
				{
					//mpActiveCam =  m_vpcam[ci];
					if (oPriority==0)
						lpcam->LoadBitmaps(IDB_CAM37RED, IDB_CAM37RED,IDB_CAM37RED,IDB_CAM37RED);
					else if (oPriority==1) //Warning
						lpcam->LoadBitmaps(IDB_CAM37ORG, IDB_CAM37ORG,IDB_CAM37ORG,IDB_CAM37ORG);
					else
						lpcam->LoadBitmaps(IDB_CAM37YEL, IDB_CAM37YEL,IDB_CAM37YEL,IDB_CAM37YEL);

					lpcam->mbTimerOn = true;
					//m_vpAcam.push_back(lpcam);
				}
	}
	return false;
}

void CDlgEmap::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	bool lb_auto = false;
	int	ltime = 0;
	int iTimerid=0;

	if (mc_closevideo=='A') //Auto
	{
		lb_auto = true;
		if (mi_closevideo>0) 
			ltime = mi_closevideo * 1000;
	}

	//Access Control Timer
	if (nIDEvent== ID_TIMER_AC)
	{
		//AfxMessageBox(_T("Time here:"));
		fnAccessTimer();
	}

	if (nIDEvent== ID_TIMER_TIME)
	{		
		iTimerid=66;
		CTime ct= CTime::GetCurrentTime();
		m_curHMS.Format(_T("%02d : %02d : %02d "), ct.GetHour(),ct.GetMinute(), ct.GetSecond());
	} //Release event timer flag here
	else if (nIDEvent== ID_TIMER_EVENT1)	{
		iTimerid=1;
		ib_e1 = false;	
	}
	else if (nIDEvent== ID_TIMER_EVENT2)	{
		iTimerid=2;
		ib_e2= false;
	}
	else if (nIDEvent== ID_TIMER_EVENT3)	{
		iTimerid=3;
		ib_e3 = false;		
	}
	else if (nIDEvent== ID_TIMER_EVENT4)	{
		iTimerid=4;
		ib_e4= false;		
	}
	else if (nIDEvent== ID_TIMER_EVENT5)	{
		iTimerid=5;
		ib_e5 = false;		
	}
	else if (nIDEvent== ID_TIMER_EVENT6)	{
		iTimerid=6;
		ib_e6= false;
	}
	else if (nIDEvent== ID_TIMER_AC)	{
		iTimerid=7;
	}

	//CString ls_debug;
	//ls_debug.Format(_T("Emap Timer ID:%d; Timer  Idx:%d  \r\n "),iTimerid , nIDEvent);
	//TRACE(ls_debug);

	if ( nIDEvent != ID_TIMER_TIME ) //&&nIDEvent != ID_TIMER_AC )	// For Event timer only
	{
		if (!lb_auto && nIDEvent != ID_TIMER_AC )	//Manual - kill the timer, close it manually
				KillTimer( nIDEvent);
		//else //auto Close
		//{
		//	if (ltime>0)  SetTimer(nIDEvent, ltime, 0 );
		//}

		CString	ls_emapmode=_T("");
		//control different mode 
		for (int i=0; i<m_vpEventTimer.size(); i++)
		{
			if (m_vpEventTimer[i].iTimer==iTimerid && m_vpEventTimer[i].iIndex>0 ) 
			{
					if (m_vpEventTimer[i].cMode == 'O') //Open
					{	
						ls_emapmode=_T("Open");
						 fnDisplayVideo(m_vpEventTimer[i].iIndex);
						if (nIDEvent  == ID_TIMER_AC)
							SetTimer(ID_TIMER_AC, ACTIMER, 0);
						if (!lb_auto) //Manual
						{						
							m_vpEventTimer[i].iIndex=0;
						}
						else // auto
						{
							m_vpEventTimer[i].cMode = 'C'; //Set to close mode
		 					if (ltime>0)  SetTimer(nIDEvent, ltime, 0 );
						}
					}
					else if (m_vpEventTimer[i].cMode == 'C') //Close
					{
						ls_emapmode=_T("Close");
						fnCloseVideo(m_vpEventTimer[i].iIndex);
						m_vpEventTimer[i].cMode == 'D';

						m_vpEventTimer.erase(m_vpEventTimer.begin() +i);
						KillTimer( nIDEvent);
					}
					//else if (m_vpEventTimer[i].cMode == 'D') //Delete
					//{
					//	ls_emapmode=_T("Delete");
					//	m_vpEventTimer.erase(m_vpEventTimer.begin()+i);
					//	KillTimer( nIDEvent);
					//}
					//ls_debug.Format(_T("Emap Timer ID: %d   ; Timer Mode:%s;  \r\n ") , iTimerid, ls_emapmode);
					//TRACE(ls_debug);
			} // 	for (int i=0; i<m_vpEventTimer.size(); i++)
		}

	}
	CBaseDialog::OnTimer(nIDEvent);
}

//Display video triggerred by event
void CDlgEmap::fnDisplayVideo(int o_camidx)
{
	//Loop through m_vctpAllCam and find the camidx
	int ll_camx=0, ll_camy=0;

	//check if the camera within the group, if not, don't need to display video
	//if (!fnCamWithinGroup(0, o_camidx)) return;

	//Filter Event Type

	//if that video already p[laying, return 
	for (int i=0; i<m_vpEventVideo.size(); i++)
	{
		if (m_vpEventVideo[i]->m_camidx == o_camidx) //Same Camera
		{
			if (m_vpEventVideo[i]->mc_mode == 'P') return;
		}
	}


	for (int i=0; i<m_vpcam.size(); i++)
	{
		if (m_vpcam[i]->m_idx == o_camidx) //Found the index
		{
			ll_camx = m_vpcam[i]->m_mapx;
			ll_camy = m_vpcam[i]->m_mapy;
			break;
		}
	}

	for (int i=0; i<m_vctpAllCam.size(); i++)
	{
		if (m_vctpAllCam[i]->camera_idx == o_camidx)
		{
			try{
            USES_CONVERSION;
			NODEITEM* lpCam = m_vctpAllCam[i];

			//CEmapVideo* m_EventVideo= new CEmapVideo; 
			CEventVideo* m_EventVideo= new CEventVideo; 
			//m_EventVideo->mb_ac = false;	
			//m_EventVideo->pDB = pDB;
			m_EventVideo->mc_parent = 'M'; // EMap
			m_EventVideo->fnAssignNode(lpCam);
			m_EventVideo->fnAssignTitle(A2W(lpCam->camera_name));
			m_EventVideo->mc_mode = 'P'; //Play mode
			m_EventVideo->m_camidx = o_camidx;
			m_EventVideo->Create(IDD_DLGEMAPVIEW1, this);
 
			CRect lrect;
			//m_map1.GetWindowRect(&lrect);
			//if (lrect.left>0) ll_camx = ll_camx + lrect.left;
			//if (lrect.top>0) ll_camy = ll_camy + lrect.top;
			GetWindowRect(&lrect);
			 ll_camx = ll_camx + lrect.left;
			ll_camy = ll_camy + lrect.top;
			m_EventVideo->MoveWindow(CRect(ll_camx, ll_camy+45, ll_camx + VIDEOW,  ll_camy+50 + VIDEOH), true);

			m_EventVideo->ShowWindow(SW_SHOW);
			m_EventVideo->BringWindowToTop();
			m_vpEventVideo.push_back(m_EventVideo);
			return;
			}
			catch(CException* p)
			{
						p->Delete();
			}
		}
	}
}

void CDlgEmap::StartTimer()
{
	// Set timer for CPU
   SetTimer( ID_TIMER_TIME, 1000, 0 );
	//SetTimer( ID_TIMER_AC, 10000, 0 );
}

void CDlgEmap::StopTimer()
{
	// Set timer for CPU
    KillTimer( ID_TIMER_EVENT1);
	KillTimer( ID_TIMER_EVENT2);
	KillTimer( ID_TIMER_EVENT3);
	KillTimer( ID_TIMER_EVENT4);
	KillTimer( ID_TIMER_EVENT5);
	KillTimer( ID_TIMER_EVENT6);
	KillTimer(ID_TIMER_TIME);
	KillTimer(ID_TIMER_AC);
}

//Check the  related checbox
int CDlgEmap::fnVideoOn(CString oVideoOn)
{
	int	ll_length,j;
	ll_length = oVideoOn.GetLength();
	
	//Uncheck all first
	((CButton*)GetDlgItem(IDC_CKMD))->SetCheck(0);	//Motion Detect
	 //((CButton*)GetDlgItem(IDC_CKVL))->SetCheck(0);	//Video Loss
	 ((CButton*)GetDlgItem(IDC_CKDI))->SetCheck(0);	//Digital Input
	 ((CButton*)GetDlgItem(IDC_CKAC))->SetCheck(0);	//Access Control

	for (int i=0; i<ll_length; i++)
	{
		char	lc = oVideoOn.GetAt(i);
		if (lc=='M') ((CButton*)GetDlgItem(IDC_CKMD))->SetCheck(1);	//Motion Detect
		//if (lc=='L') ((CButton*)GetDlgItem(IDC_CKVL))->SetCheck(1);		//Video Loss
		if (lc=='I') ((CButton*)GetDlgItem(IDC_CKDI))->SetCheck(1);	//Digital Input
		if (lc=='A') ((CButton*)GetDlgItem(IDC_CKAC))->SetCheck(1);	//Access Control
	}
	 
	return 0;
}


BOOL CDlgEmap::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if ( m_pToolTipCtrl != NULL)
		m_pToolTipCtrl->RelayEvent(pMsg);

	switch ( pMsg->message )
	{
		case WM_KEYDOWN:
		{
			switch( pMsg->wParam )
			{
				case VK_ESCAPE:						
						if(ib_dirty == true) {
							//CString ls_confirm = _T("Data had been changed, do you want to save changes ?");
							int ll_ret = AfxMessageBox(IDS_SAVE_DATA, MB_YESNO|MB_ICONQUESTION);
							if (ll_ret==IDYES) 
							{
								fnSaveDB();								
							}
							else
							{
								CDialog::OnCancel();
							}
							::DispatchMessage( pMsg );
						} //if(ib_dirty == true)
				fnCloseVideo();
				break;
			} //switch
		} // Case WM_KEYDOWN:
	}

	return CBaseDialog::PreTranslateMessage(pMsg);
}


//Check videoon for display video, return false for NOT display
bool CDlgEmap::fnFilterVideoOn(CString oEventtype)
{
	CString ls_type;
	ls_type	= oEventtype.MakeLower().Trim();

	if (ls_type == _T("digital input") || ls_type==_T("di"))	//Digital Input
	{
		if (((CButton*)GetDlgItem(IDC_CKDI))->GetCheck()==1)  
			return true;
		else
			return false;
	}
	else if (ls_type == _T("motion detect 1") || ls_type==_T("md") || ls_type==_T("motion detect"))	//Motion Detect
	{
		if (((CButton*)GetDlgItem(IDC_CKMD))->GetCheck()==1)  
			return true;
		else
			return false;
	}
	else if (ls_type == _T("video loss") || ls_type==_T("vl"))	//Motion Detect
	{
		if (((CButton*)GetDlgItem(IDC_CKVL))->GetCheck()==1)  
			return true;
		else
			return false;
	}

	return false;
}

void CDlgEmap::OnBnClickedButardown()
{
	// TODO: Add your control notification handler code here		
	CRect	 lrect, rc, src;
	ScreenToClient(src);
	m_listcam.GetWindowRect(lrect);
	m_listcam.GetClientRect(rc);

	int	iShowHide = SW_SHOW;

	if (mb_hideset)
	{
			//Show settings
			mb_hideset = false;
			((CBitmapButton*)GetDlgItem(IDC_BUTARDOWN))->LoadBitmaps(IDB_ARROWUP, IDB_ARROWUP, IDB_ARROWUP, IDB_ARROWUP);
	}
	else
	{
		//Hide Settings
		mb_hideset =true;
		((CBitmapButton*)GetDlgItem(IDC_BUTARDOWN))->LoadBitmaps(IDB_ARROWDOWN, IDB_ARROWDOWN, IDB_ARROWDOWN, IDB_ARROWDOWN);
	}
	fnHideSetting(mb_hideset);
}

void CDlgEmap::fnSaveEventLog()
{
	//Under Construction.....................
		CString	ls_tfolder =_T("");
		CFileDialog fOpenDlg(TRUE, _T("txt"), _T("event_log"), OFN_HIDEREADONLY|OFN_CREATEPROMPT, 	_T("Select Bitmap File (*.txt)|*.txt|"), this);
		fOpenDlg.m_pOFN->lpstrTitle=_T("Save Event Log As");	  
		fOpenDlg.m_pOFN->lpstrInitialDir=_T("c:");
	  
		if(fOpenDlg.DoModal()==IDOK)
		{
			CString ls_path = fOpenDlg.GetPathName();
			CFile cLogFile;
			
			cLogFile.Open( ls_path,  CFile::modeCreate|CFile:: modeReadWrite);
			
			//Loop into list Event
			for(int iRow = 0; iRow<  m_listevent.GetItemCount(); iRow++)
			{	
				CString	ls_data, ls_time, ls_cam, ls_event;
				ls_time = m_listevent.GetItemText(iRow, 1);
				//ls_pri = m_listevent.GetItemText(iRow, 2);
				ls_cam= m_listevent.GetItemText(iRow, 3);
				ls_event= m_listevent.GetItemText(iRow, 4);
				ls_data.Format(_T("Time:%s  Camera:%s   Event:%s \r\n"),ls_time.Trim(),ls_cam.Trim(), ls_event.Trim()); 
					
				USES_CONVERSION;		

				char*	lcp_data = new char[80];	
				strncpy(lcp_data , W2A(ls_data), ls_data.GetLength());
 
				cLogFile.Write(lcp_data,ls_data.GetLength());  
				delete lcp_data;
			}
			cLogFile.Close();

			int i=0;
		}

 
}
void CDlgEmap::OnMove(int x, int y)
{
	CBaseDialog::OnMove(x, y);
 
//	ShowWindow(SW_MAXIMIZE);
	// TODO: Add your message handler code here
}

//void CDlgEmap::OnBnClickedButclear()
//{
//	 	m_listevent.DeleteAllItems();
//}

void CDlgEmap::OnClose()
{
	if (mb_AC)
	{
	/*	ThreadFlag = false;
		::CloseHandle(hAccessThread);*/
	}

//	pDB.Close();
	m_EventReceiveFlag = false;
	fnCloseVideo();
	fnFreeMemory();
	this->DestroyWindow();
	delete m_EmapEventRec;
	return CBaseDialog::OnClose();
}

// Called by begin drag of camera list control, if map didn't exist, return false, prevent crash
bool CDlgEmap::fnCheckMapExist(void)
{
	CString		csMap;
	((CEdit*)GetDlgItem(IDC_EDITMAP))->GetWindowTextW(csMap);

	if (csMap==_T("")) return false;

	return true;
}


void CDlgEmap::fnInitAccess(void)
{
	mb_AC = false;
	return;

	//Check if  Schema support access control
	CString	ls_loc;
	CString ls_sql = (_T("select * from sys.tables where name = 'ecac_record'"));
	CRecordset	rsAC(pDB);
	rsAC.Open(CRecordset::forwardOnly, ls_sql);
	
	if (rsAC.GetRecordCount() >0)	
	{	
		rsAC.Close();
		//CRecordset	rsCamera(pDB);
		CString ls_camera = (_T("SELECT  camera_idx  FROM ecac_camera WHERE active_='Y'"));
		rsAC.Open(CRecordset::forwardOnly, ls_camera);
		if (rsAC.GetRecordCount() > 0)
		{
			mb_AC = true;
			rsAC.Close();
			fnStartAccessService();
	/*		UINT nRet(0);
			hThreadAccess = (HANDLE)::_beginthreadex(NULL, 0, AccessService, NULL, 0, &nRet);*/
		}
		//vStartEventReceive();
		CDlgEmap* lpEmap = (CDlgEmap*) GetParent();
	}
}

void CDlgEmap::OnBnClickedButreport()
{
	// TODO: Add your control notification handler code here
	//CDlgReport	
}

UINT CDlgEmap::fnThreadBegin(LPVOID lpParam)
{
	CDlgEmap* pThis = (CDlgEmap*)lpParam;

	while(ThreadFlag)
	{
		
	}

	return 0;
}
BOOL CDlgEmap::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgEmap::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CDlgEmap::OnTcnSelchangeTabeventac(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	int ll_sel = m_tab.GetCurSel();
	if (ll_sel ==0) 	{
		pDialog[0]->ShowWindow(SW_SHOW);
		pDialog[1]->ShowWindow(SW_HIDE);
	}
	else{
		pDialog[0]->ShowWindow(SW_HIDE);
		pDialog[1]->ShowWindow(SW_SHOW);
	}


}

void CDlgEmap::OnBnClickedCkac()
{
	// TODO: Add your control notification handler code here

	ib_dirty = true;
	mc_fileMode = 'A';
}

bool CDlgEmap::fnAfterMove(int o_newx, int o_newy, int o_camgroupid, int o_camidx)
{
		int	ll_basex=0;
		CPoint pt(o_newx, o_newy);
		bool lb_drop = fnDropWithin(&pt);

		CRect		lrect;
		m_map1.GetWindowRect(&lrect);
		ll_basex = lrect.left;

	for (int i=0; i<m_vpcam.size(); i++)
	{
		CEmapCam* lpCam = m_vpcam[i];
		if (lpCam->m_camgroupid == o_camgroupid)
		{
			//lpCam->m_mapx = o_newx - ll_basex;
			lpCam->m_mapx = o_newx;
			lpCam->m_mapy = o_newy;
			lpCam->mb_dirty = true;
			ib_dirty = true;
			break;
		}
	}
	
	//Move the Access icon as well
	for (int i=0; i<m_vpAccessCam.size(); i++)
	{
		CEmapAccessCam*	lpAccess = m_vpAccessCam[i];
		if (lpAccess->m_camidx == o_camidx)
		{
			lpAccess->MoveWindow(o_newx , o_newy -ACCESSH , ACCESSW, ACCESSH, true);
		}
	}

	//move the static text as well
	for (int i=0; i<m_vpName.size(); i++)
	{
		CBaseStatic*  lpstc = m_vpName[i];
		if (lpstc->m_id == o_camgroupid)
		{
			lpstc->MoveWindow(o_newx + 40, o_newy, STCW, STCH, true);
			break;
		}
	}
	return false;
}

bool CDlgEmap::fnDropWithin(LPPOINT mpt)
{
		//if mpt is within the map1, return true, otherwise return false
	CRect rect; 	
	m_map1.GetWindowRect(&rect);
 
	ScreenToClient(&rect); //optional step - see below  //position:  rect.left, rect.top //size: rect.Width(), rect.Height() 
	if (mpt->x < rect.right && mpt->x > rect.left && mpt->y >rect.top && mpt->y<rect.bottom)
		return true;
	else
		return false;
}
void CDlgEmap::fnEmapCallBack(EventInfo *m_EventInfo, void *lpParam)
{
	CDlgEmap* pThis = (CDlgEmap*)lpParam;
	pThis->m_CS.Lock();

	CString osIP(_T("")), osEventType(_T("")), csEventStreamID(_T("")), osIndex(_T("")), csCurHMS(_T(""));
	osIP.Format(_T("%s"), m_EventInfo->tcIP);
	
	switch(m_EventInfo->nEventType)
	{
	case EVENTTYPE::MOTION_DETECTION:
		//csEventContext.Format(_T("%s"), m_EventInfo->tcEventType);
		if(m_EventInfo->nStatus == EVENT_STATUS::OFF)
		{
			osEventType = _T("Motion Detection OFF");
		}
		else if(m_EventInfo->nStatus == EVENT_STATUS::ON)
		{
			osEventType = _T("Motion Detection ON");
		}

		break;
	case EVENTTYPE::DIGITAL_INPUT:
		if(m_EventInfo->nStatus == EVENT_STATUS::OFF)
		{
			osEventType = _T("Digital Input OFF");
		}
		else if(m_EventInfo->nStatus == EVENT_STATUS::ON)
		{
			osEventType = _T("Digital Input ON");
		}
		break;
	case EVENTTYPE::VIDEO_LOST:
		osEventType = _T("Video Lost");
		break;
	case EVENTTYPE::VIDEO_OPEN:
		osEventType = _T("Video Open");
		break;
	case EVENTTYPE::REBOOT:
		osEventType = _T("Reboot");
		break;
	case EVENTTYPE::DIGITAL_OUTPUT:
		osEventType = _T("Digital Output");
		break;
	case EVENTTYPE::MISSING_OBJECT:
		osEventType = _T("Missing Object");
		break;
	case EVENTTYPE::EVENT_TRIGGER:
		osEventType = _T("Event Trigger");
		break;
	case TRANSACTION:
		osEventType = _T("Transaction");
		break;
	case RECORDING_FAIL:
		osEventType = _T("Recording Status Change");
		break;
	case DISK_FULL:
		osEventType = _T("Overwrite file");
		break;
	case LOG_IN:
		osEventType = _T("Login");
		break;
	case LOG_OUT:
		osEventType = _T("Logout");
		break;
	case DISCONNECT:
		osEventType = _T("Disconnect");
		break;
	default:
		osEventType = _T("Unknow");
		//ASSERT(false);
		break;
	}

	//osEventType.Format(_T("%s %s"), m_EventInfo->tcEventType, m_EventInfo->tcStream);
	osIndex.Format(_T("%d"), m_EventInfo->nCamIdx);

	CTime timeTime = 0;
	timeTime = CTime::GetCurrentTime();
	csCurHMS.Format(_T("%02d:%02d:%02d"), timeTime.GetHour(), timeTime.GetMinute(), timeTime.GetSecond());

	if (m_EventReceiveFlag == false) return;
	//Display into Event Tabpage
	//m_pageEvent.fnDisplayEvent(osIndex, osIP, osEventType, csCurHMS);
	//Do rest of the stuff...display video timer...etc
	pThis->EventDisplay( osIndex, osIP, osEventType	);
	pThis->m_CS.Unlock();
}

/*=========== OLD CODES ================*/
//void CDlgEmap::fnEmapCallBack(EventInfo *m_EventInfo, void *lpParam)
//{
//	CDlgEmap* pThis = (CDlgEmap*)lpParam;
//	pThis->m_CS.Lock();
//
//	CString osIP(_T("")), osEventType(_T("")), csEventStreamID(_T("")), osIndex(_T("")), csCurHMS(_T(""));
//	osIP.Format(_T("%s"), m_EventInfo->tcIP);
//	osEventType.Format(_T("%s %s"), m_EventInfo->tcEventType, m_EventInfo->tcStream);
//	osIndex.Format(_T("%s"), m_EventInfo->tcCamIDX);
//
//	CTime timeTime = 0;
//	timeTime = CTime::GetCurrentTime();
//	csCurHMS.Format(_T("%02d:%02d:%02d"), timeTime.GetHour(), timeTime.GetMinute(), timeTime.GetSecond());
//
//	if (m_EventReceiveFlag == false) return;
//	//Display into Event Tabpage
//	//m_pageEvent.fnDisplayEvent(osIndex, osIP, osEventType, csCurHMS);
//	//m_pageEvent fnDisplayEvent(osIndex, osIP, osEventType, csCurHMS);
//	
//	//Do rest of the stuff...display video timer...etc
//	pThis->EventDisplay( osIndex, osIP, osEventType	);
//	
//	pThis->m_CS.Unlock();
//}
//void CDlgEmap::OnBnClickedButsaveas()
//{
//	// TODO: Add your control notification handler code here
//	if (!mb_AC)
//		fnSaveEventLog();
//}
//
//void CDlgEmap::OnBnClickedButclear()
//{
//	// TODO: Add your control notification handler code here
//	if (!mb_AC)
//		m_listevent.DeleteAllItems();
//}

void CDlgEmap::fnStartAccessService(void)
{
		try
	{
		CString csIP, csUser, csPwd, csWebPort, csCamIDX, csCamID, csProtocol;
		CRecordset rCamera(pDB);

		rCamera.Open(CRecordset::forwardOnly, _T("SELECT ec_camera.cameraid, ec_camera.camera_idx, ec_camera.ipaddress, ec_camera.httpport, ec_camera.username, ec_camera.password, ec_stream.stream_protocol FROM ec_camera INNER JOIN ec_stream ON ec_camera.cameraid = ec_stream.cameraid  INNER JOIN ecac_camera ON ec_camera.camera_idx = ecac_camera.camera_idx  WHERE ec_stream.stream_name = 'stream 1'"));
		while(!rCamera.IsEOF())
		{
			rCamera.GetFieldValue(_T("cameraid"), csCamID);
			rCamera.GetFieldValue(_T("camera_idx"), csCamIDX);
			rCamera.GetFieldValue(_T("ipaddress"), csIP);
			rCamera.GetFieldValue(_T("httpport"), csWebPort);
			rCamera.GetFieldValue(_T("username"), csUser);
			rCamera.GetFieldValue(_T("password"), csPwd);
			rCamera.GetFieldValue(_T("stream_protocol"), csProtocol);
			
			csCamID.TrimRight();
			csCamIDX.TrimRight();
			csIP.TrimRight();
			csWebPort.TrimRight();
			csUser.TrimRight();
			csPwd.TrimRight();
			csProtocol.TrimRight();

			//db_CameraInfo.csCamIDX = csCamIDX;
			//db_CameraInfo.csIP = csIP;
			//db_CameraInfo.csPort = csWebPort;
			//db_CameraInfo.csProtocol = csProtocol;
			//db_CameraInfo.csPwd = csPwd;
			//db_CameraInfo.csUser = csUser;

			//if(csProtocol.Find(_T("RTSP")) != -1)	//--New Type
			//{
			//	//LogEvent(_T("IP:%s, Protocol:%s"), csIP, csProtocol);
			//	mapCameraInfo.insert(::pair<CString, CameraInfo>(csIP, db_CameraInfo));

			//	vSettingRemoteLog(csCamIDX, csIP, csWebPort, csUser, csPwd);
			//}
			//else	//--Old Type
			//{
				//mapCameraInfo.insert(::pair<CString, CameraInfo>(csIP, db_CameraInfo));

				CameraInfo* o_CameraInfo = new CameraInfo();

				o_CameraInfo->csCamIDX = csCamIDX;
				o_CameraInfo->csIP = csIP;
				o_CameraInfo->csPort = csWebPort;
				o_CameraInfo->csProtocol = csProtocol;
				o_CameraInfo->csPwd = csPwd;
				o_CameraInfo->csUser = csUser;
				o_CameraInfo->pWnd = (CDlgEmap*)this;

				m_vcCamInfo.push_back(o_CameraInfo);
		//	}
		
			rCamera.MoveNext();
		}

	}
	catch(CDBException * p)
	{
		p->Delete();
	}

	for(int iTemp = 0; iTemp < m_vcCamInfo.size(); iTemp++)
	{
		UINT nOldRet(0);
		
		//AfxBeginThread(
		HANDLE hReceiveOldType = (HANDLE)::_beginthreadex(NULL, 0, ReceiveAccess, (LPVOID)m_vcCamInfo[iTemp], 0, &nOldRet);
		//Set Access Timer
	 
	}

	SetTimer(ID_TIMER_AC, ACTIMER, 0 );
}

UINT CDlgEmap::ReceiveAccess(LPVOID lpParam)
{
	CameraInfo* pCameraInfo = (CameraInfo*)lpParam;
	CDlgEmap*	pDlgEmap = (CDlgEmap*)pCameraInfo->pWnd;

	CString csURL = _T("http://") + pCameraInfo->csIP  + _T("/config/get_serial.cgi");
	char *chUrl = new char[csURL.GetLength() + 1];
	wcstombs_s(NULL, chUrl, csURL.GetLength() + 1, csURL, _TRUNCATE);

	CString csUserPwd = pCameraInfo->csUser + _T(":") + pCameraInfo->csPwd;
	char *chUserPwd = new char[csUserPwd.GetLength() + 1];
	wcstombs_s(NULL, chUserPwd, csUserPwd.GetLength() + 1, csUserPwd, _TRUNCATE);

	CURLcode gCode = curl_global_init(CURL_GLOBAL_ALL);

	CURL* m_curl = curl_easy_init();

	if(m_curl == NULL)
		return -1;

	CURLINFO	m_curlInfo;

	curl_easy_setopt(m_curl, CURLOPT_URL, chUrl);
	curl_easy_setopt(m_curl, CURLOPT_USERPWD, chUserPwd);
	curl_easy_setopt(m_curl, CURLOPT_READDATA, pCameraInfo);
	curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &CDlgEmap::notifyWriter);

	curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, pCameraInfo);

	CURLcode pCode = curl_easy_perform(m_curl); 

	delete chUrl;
	delete chUserPwd;
	return 1;
}

//Access Control to grab 
int CDlgEmap::notifyWriter(char* data, size_t size, size_t nmemb,void *lParam)
{
 	if(lParam == NULL)	return 0;
	CameraInfo* pCameraInfo = (CameraInfo*)lParam;
	CDlgEmap* pDlgEmap = (CDlgEmap*) (pCameraInfo->pWnd);

	int len = size*nmemb;
	int	i_camidx=0;

	TCHAR tzBuffer[4096];
	MultiByteToWideChar(CP_ACP, 0, data, -1, tzBuffer, 4096);

	CString csNotifyBuffer(_T("")), csEventNum(_T("")), csBuffer(_T(""));
	csNotifyBuffer.Format(_T("%s"), tzBuffer);

	map <CString, eVERSETS> EventMap;

	EventMap[_T("config/get_serial.cgi")];

	if ((csNotifyBuffer.Find(_T("keep_alive")) == -1) && (csNotifyBuffer.Find(_T("data=")) > -1))
	{
		bool	lb_threadFlg = false;	

		map<CString, eVERSETS>::iterator m_pos = EventMap.begin();
		
		for(; m_pos != EventMap.end(); m_pos++)
			{vParseSegment(tzBuffer, m_pos);}

		map<CString, eVERSETS>::iterator pos;
		map<CString, eSETS>::iterator pos2;
		CString	lsAll;
		lsAll.GetBuffer(256);

		for(pos = EventMap.begin(); pos != EventMap.end(); pos++)
		{
				for(pos2 = pos->second.aSet.begin(); pos2 != pos->second.aSet.end(); pos2++)
				{
					if(pos2->first == _T("data"))
					{
						CString ls_ad = pos2->second.csKey;
						CString ls_ascii = HexToAscii(ls_ad);
						lsAll.Format(_T("%s%s"),lsAll,ls_ascii);
						lb_threadFlg	 = true;
					}
				}
		}

		if (pDlgEmap!=NULL)
		{
			pDlgEmap->fnModifyAccess(lParam, lsAll, pCameraInfo->csCamIDX);
		}
	}
 
	return len;
}

void vParseSegment(TCHAR* tszBuffer, map <CString, eVERSETS>::iterator pos)
{
	CString ls_name = _T("");
	//CString ls_data = _T("");
	TCHAR* tszLine;
	wchar_t  tszAll[250] = {0};
	tszLine = wcstok(tszBuffer, _T("\r\n"));
	while (tszLine)
	{
		TCHAR* tszName = tszLine;
		TCHAR* tsz = wcschr(tszLine, '=');
		if (tsz)
		{
			*tsz = '\0';
			tsz++;
			ls_name = (TCHAR*)tszName;
			if ( ls_name ==_T("data"))
			{
				//ls_data = (TCHAR*)tsz; 
				wcscat(tszAll, tsz);
				//tsz = ls_data + tsz;
				pos->second.aSet[tszName] =  eSETS(tszAll, tszName);
			}
			else
				pos->second.aSet[tszName] = eSETS(tsz, tszName);
		}
		tszLine = wcstok(NULL, _T("\r\n"));
	}
}

static CString fnDavinciCode(CString oHex)
{
	CString ls_ret = _T("");
	oHex.MakeLower();
	
	if (oHex==_T("0a")) ls_ret = _T( "F");
	else if (oHex==_T("03")) ls_ret = _T( "T");	
	else if (oHex==_T("0d")) ls_ret = _T( "R");
	else if (oHex==_T("30")) ls_ret = _T( "0");
	else if (oHex==_T("31")) ls_ret = _T( "1");
	else if (oHex==_T("32")) ls_ret = _T( "2");
	else if (oHex==_T("33")) ls_ret = _T( "3");
	else if (oHex==_T("34")) ls_ret = _T( "4");
	else if (oHex==_T("35")) ls_ret = _T( "5");
	else if (oHex==_T("36")) ls_ret = _T( "6");
	else if (oHex==_T("37")) ls_ret = _T( "7");
	else if (oHex==_T("38")) ls_ret = _T( "8");
	else if (oHex==_T("39")) ls_ret = _T( "9");
	else if (oHex==_T("3a")) ls_ret = _T( ":");
	else if (oHex==_T("2a")) ls_ret = _T( "*");
	else if (oHex==_T("2b")) ls_ret = _T( "+");
	else if (oHex==_T("2c")) ls_ret = _T( "'");
	else if (oHex==_T("2d")) ls_ret = _T( "-");
	else if (oHex==_T("03")) ls_ret = _T( "E");
	else if (oHex==_T("44")) ls_ret = _T( "D");
	else if (oHex==_T("47")) ls_ret = _T( "G");
	else if (oHex==_T("4b")) ls_ret = _T( "K");
	else if (oHex==_T("")) ls_ret = _T( "-");
	else ls_ret = oHex.Mid(1,1);
	return ls_ret;
}

static CString HexToAscii(CString s)
{
	CString lsAll;
	lsAll.GetBuffer(256);
	for(int i = 0; i < s.GetLength(); i+=2)
	{
		CString	lstmp=_T("");
		CString	ls_new=_T("");
		lstmp = s.Mid(i, 2);
		ls_new = fnDavinciCode(lstmp);
		lsAll.Format(_T("%s%s"),lsAll,ls_new);
	}

	return lsAll;
}

bool CDlgEmap::fnModifyAccess(LPVOID param, CString oData, CString oCamIdx)
{
		bool okflag = false;
		CDlgEmap* lpEmap = (CDlgEmap*) param;		
		if (oData.GetLength() <30) return false;

		int ll_anchor = oData.Find(_T("----"),0);
		if (ll_anchor<=0) return false;
		CString ls_id = oData.Mid(ll_anchor - 8, 8);
		CString ls_date = oData.Mid(ll_anchor +4, 6);
		CString ls_hhmm = oData.Mid(ll_anchor + 12, 4);
			int		accessID=0;
	

		try{
			//Get Employee Name and Department
			CString ls_sql1, ls_sql2, ls_sql3, ls_sql, csDept, csEmpName;
			CDBVariant		varAccessid;

			CRecordset m_rs(pDB);

			ls_sql1 = _T("SELECT ecac_hgroup.access_group_desc, ecac_access_employee.access_name, ecac_access_employee.accessid FROM ecac_access_employee ");
			ls_sql2 = _T(" LEFT OUTER JOIN ecac_hgroup ON ecac_access_employee.access_group_code = ecac_hgroup.access_group_code");
			ls_sql3 = _T(" WHERE ecac_access_employee.access_masterid = ");
			ls_sql.Format( _T("%s%s%s%s"), ls_sql1, ls_sql2, ls_sql3, ls_id);

			m_rs.Open(CRecordset::dynaset, ls_sql);

			if (m_rs.GetRecordCount() >0)
			{
  				m_rs.MoveFirst();

				while(!m_rs.IsEOF()   ) 
				{ 	  
					okflag = true;
					m_rs.GetFieldValue( _T("access_group_desc"), csDept); 
					m_rs.GetFieldValue( _T("access_name"), csEmpName); 
					m_rs.GetFieldValue( _T("accessid"),  varAccessid, SQL_C_SLONG); 
					accessID = varAccessid.m_iVal;
					m_pageAc.fnInsertRow(ls_date, ls_hhmm, oCamIdx , csDept, csEmpName);
	 
					break;
				}
			}
			m_rs.Close();
			//In case no related data in the DB
			if (!okflag)
			{
				m_pageAc.fnInsertRow(ls_date, ls_hhmm, oCamIdx,  _T("Unknown"), ls_id);				
			}
		}
		catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
 		fnAccessInsertDB(ls_date, ls_hhmm, ls_id, accessID);
		m_EventTimer.iTimer = 7;
 		m_EventTimer.iIndex = _ttoi(oCamIdx);
		m_EventTimer.cMode = 'O'; //Open Video
		m_vpEventTimer.push_back(m_EventTimer);
		SetTimer(ID_TIMER_AC, 500, 0 );
		return true;
}


// Timer being triggerred, call send command CGI here
void CDlgEmap::fnAccessTimer(void)
{ 
		if ( m_vcCamInfo.size()<=0) return;

 		CameraInfo* pCameraInfo = m_vcCamInfo[0];

		CString csURL = _T("http://") + pCameraInfo->csIP  + _T("/config/send_cmd.cgi?cmd=05444f3030&");
		
		char *chUrl = new char[csURL.GetLength() + 1];
		wcstombs_s(NULL, chUrl, csURL.GetLength() + 1, csURL, _TRUNCATE);

		CString csUserPwd = pCameraInfo->csUser + _T(":") + pCameraInfo->csPwd;
		char *chUserPwd = new char[csUserPwd.GetLength() + 1];
		wcstombs_s(NULL, chUserPwd, csUserPwd.GetLength() + 1, csUserPwd, _TRUNCATE);

		CURLcode gCode = curl_global_init(CURL_GLOBAL_ALL);
		CURL* m_curl = curl_easy_init();

		if(m_curl == NULL)
			return ;

		CURLINFO	m_curlInfo;

		curl_easy_setopt(m_curl, CURLOPT_URL, chUrl);
		curl_easy_setopt(m_curl, CURLOPT_USERPWD, chUserPwd);
 
		CURLcode pCode = curl_easy_perform(m_curl); 

		curl_easy_cleanup(m_curl);
		
		delete chUrl;
		delete chUserPwd;
 
		return ;

}

void CDlgEmap::fnAccessInsertDB(CString oDate, CString oTime, CString oID, int oAccessid)
{
	try{
 			CString	ls_sql, ls_dt;
			ls_dt.Format(_T("%s %s:%s:00"),oDate, oTime.Mid(0,2), oTime.Mid(2,2));
			// '20070529 00:00:00'   			CONVERT(smalldatetime, '%s')
		
			ls_sql.Format(_T("INSERT INTO ecac_record (record_daytime, access_masterid, accessid ) VALUES ( CONVERT(datetime, '%s'), '%s', %d )"), ls_dt, oID, oAccessid  );
	 		TRACE(ls_sql);
			pDB->ExecuteSQL(ls_sql);
	}
	catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
}



void CDlgEmap::OnBnClickedButsaveas()
{
	fnSaveEventLog();
}

void CDlgEmap::OnBnClickedButclear()
{
	((CListCtrl*)GetDlgItem(IDC_LISTEVENT))->DeleteAllItems();
}

//Localization
void CDlgEmap::fnInitLocalize(void)
{
	CString csLoc(_T(""));
	LOADSTRING(csLoc, IDS_EMAPGROUP);
	((CStatic*)GetDlgItem(IDC_STGROUP))->SetWindowTextW(csLoc);

	LOADSTRING(csLoc, IDS_EMAPFOLDER);
	((CStatic*)GetDlgItem(IDC_STFOLDER))->SetWindowTextW(csLoc);

	LOADSTRING(csLoc, IDS_EMAPMAP);
	((CStatic*)GetDlgItem(IDC_STMAP))->SetWindowTextW(csLoc);

	//live view static
	LOADSTRING(csLoc, IDS_EMAPLIVEVIEW);
	((CStatic*)GetDlgItem(IDC_STCLOSEVIDEO))->SetWindowTextW( csLoc);

	//Motion Detect checkbox
	LOADSTRING(csLoc, IDS_EMAPMOTIONDETECT);
	((CButton*)GetDlgItem(IDC_CKMD))->SetWindowTextW(csLoc);

	//Cameras
	LOADSTRING(csLoc, IDS_EMAPCAMERA);
	((CStatic*)GetDlgItem(IDC_STCAM))->SetWindowTextW( csLoc);

	LOADSTRING(csLoc, IDS_EMAPEVENT);
	((CStatic*)GetDlgItem(IDC_STEVENT))->SetWindowTextW( csLoc);
 
}

//check if multi-monitor defined, if so get the right camera group
void CDlgEmap::fnmmcheck(void)
{
		bool	lb_found = false;
		CDBVariant vargroupid;

		CString ls_sql = (_T("SELECT cameragroupid FROM ec_monitor WHERE startup_app='EMAP' "));
		CRecordset	rsGroup(pDB);
		rsGroup.Open(CRecordset::forwardOnly, ls_sql);
	

		while (!rsGroup.IsEOF()) { 
			lb_found=true;
			rsGroup.GetFieldValue( _T("cameragroupid"), vargroupid,  SQL_C_SSHORT );
			break;
	}

	if (lb_found)	//Defined Multi_monitor
	{
			if (vargroupid.m_iVal <0)
			{
				m_Group.SetCurSel(0);
				return;
			}

			for ( int i=0; i<m_pvtGroup.size(); i++)
			{
				if (m_pvtGroup[i].nid == vargroupid.m_iVal)
				{
					m_Group.SetCurSel(i);
					return;
				}
			}
	}
	else
			m_Group.SetCurSel(0);

}
