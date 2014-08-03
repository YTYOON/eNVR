 // DlgAc.cpp : implementation file

#include "stdafx.h"
#include "DlgAc.h"
#include "MainFrm.h"
#include "BaseRadioButton.h"
#include "resource.h"
#include "DlgGate.h"
#include "DlgEmap.h"
#include "IntraEvent.h"

#define	ACTIME	1000 
#define	ACTIMEPE	 5000
#define	REFRESHID	2033
#define	CAMBUTID	234
#define BMPKEY 0
#define BMPRED 1
#define BMPKEYRED 2
#define BMPBLANK 3
IMPLEMENT_DYNAMIC(CDlgAc, CBaseDialog)

const UINT ID_TIMER_AC0 = 0x1000;
const UINT ID_TIMER_AC1 = 0x1001;
const UINT ID_TIMER_AC2 = 0x1002;
const UINT ID_TIMER_AC3 = 0x1003;
const UINT ID_TIMER_AC4 = 0x1004;
const UINT ID_TIMER_AC5 = 0x1005;
const UINT ID_TIMER_AC6 = 0x1006;
const UINT ID_TIMER_AC7 = 0x1007;
const UINT ID_TIMER_AC8 = 0x1008;
const UINT ID_TIMER_AC9 = 0x1009;
const UINT ID_TIMER_AC10 = 0x1010;
const UINT ID_TIMER_AC11 = 0x1011;
const UINT ID_TIMER_AC12 = 0x1012;
const UINT ID_TIMER_AC13 = 0x1013;
const UINT ID_TIMER_AC14 = 0x1014;
const UINT ID_TIMER_AC15 = 0x1015;
const UINT ID_TIMER_AC16 = 0x1016;
const UINT ID_TIMER_AC17 = 0x1017;
const UINT ID_TIMER_AC18 = 0x1018;
const UINT ID_TIMER_AC19 = 0x1019;
const UINT ID_TIMER_AC20 = 0x1020;


const UINT ID_TIMER_SECUREP = 0x1088;

static CDatabase*		pDB;
static vector<CEmapAccessCam*> m_vpcam; 
static HANDLE hThreadAccess;
static CString HexToAscii(CString s);
static void vParseSegment(TCHAR* tszBuffer, map <CString, eVERSETS>::iterator pos);
//static CURL* g_curl;

static bool m_EventReceiveFlag;
//static CString DLTBRAND
int	m_securecnt=0;

CDlgAc::CDlgAc(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgAc::IDD, pParent)
{

}

CDlgAc::~CDlgAc()
{
	fnFreeMemory(false);	
}

void CDlgAc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTSET, m_butset);
	DDX_Control(pDX, IDC_BUTSEARCH, m_butsch);
	DDX_Control(pDX,IDC_BUTPLAYBK, m_butpbk);
	DDX_Control(pDX, IDC_BUTREP, m_butrep);
	DDX_Control(pDX, IDC_COMBOGROUP, m_comboGroup);
	DDX_Control(pDX, IDC_EDITFOLDER, m_editFolder);
	DDX_Control(pDX, IDC_EDITMAP, m_editMap);

	DDX_Control(pDX, IDC_MAP, m_map1);

	//DDX_Control(pDX, IDC_BUTBROWSE, m_butBrowse);
	//DDX_Control(pDX, IDC_BUTMAP, m_butMap);
	DDX_Control(pDX, IDC_CHECKREC, m_checkRec);
	DDX_Control(pDX, IDC_LISTCAM, m_listCam);
	DDX_Control(pDX, IDC_LISTAC, m_listAc);

	DDX_Control(pDX, IDC_STAC, m_stac);
	//DDX_Control(pDX, IDC_BUTBROWSE, m_butBrowse);
	//DDX_Control(pDX, IDC_BUTMAP, m_butMap);
	DDX_Control(pDX, IDC_STGDETAIL, m_gbDetail);
	DDX_Control(pDX, IDC_BUTSWITCH, m_butSwitch);
	DDX_Control(pDX, IDC_BUTOPEN, m_butOpen);
	DDX_Control(pDX, IDC_BUTRED, m_butRed);
	DDX_Control(pDX, IDC_CHECKWARN, m_ckAlarm);
	//DDX_Control(pDX, IDC_LSPLITV, m_lsplitv);
	DDX_Control(pDX, IDC_LISTCHAIN, m_listchain);
}


BEGIN_MESSAGE_MAP(CDlgAc, CBaseDialog)
 
	ON_BN_CLICKED(IDC_BUTSET, &CDlgAc::OnBnClickedButset)
	ON_BN_CLICKED(IDC_BUTREP, &CDlgAc::OnBnClickedButrep)
	ON_BN_CLICKED(IDC_BUTBROWSE, &CDlgAc::OnBnClickedButbrowse)
	ON_BN_CLICKED(IDC_BUTMAP, &CDlgAc::OnBnClickedButmap)
	ON_BN_CLICKED(REFRESHID,  &CDlgAc::OnBnClickedButRefresh)
	ON_CBN_SELCHANGE(IDC_COMBOGROUP, &CDlgAc::OnCbnSelchangeCombogroup)
	//ON_NOTIFY(LVN_BEGINDRAG, IDC_LISTCAM, &CDlgAc::OnLvnBegindragListcam)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LISTCAM, &CDlgAc::OnLvnBegindragListcam)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTSEARCH, &CDlgAc::OnBnClickedButsearch)
	ON_NOTIFY(NM_RCLICK, IDC_LISTAC, &CDlgAc::OnNMRClickListac)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MODIFY_EMPLOYEE, OnModifyEmployee)
 
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTAC, &CDlgAc::OnLvnItemchangedListac)
	ON_BN_CLICKED(IDC_BUTOPEN, &CDlgAc::OnBnClickedButopen)
	ON_BN_CLICKED(IDC_BUTRED, &CDlgAc::OnBnClickedButred)
	ON_BN_CLICKED(IDC_CHECKWARN, &CDlgAc::OnBnClickedCheckwarn)
	ON_BN_CLICKED(IDC_BUTSWITCH, &CDlgAc::OnBnClickedButswitch)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCHAIN, &CDlgAc::OnLvnItemchangedListchain)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDlgAc message handlers

void CDlgAc::fnInitControl(void)
{
	CRect llrect;
	//Split Control
	int ll_posx = 0, ll_posy  = 76, ll_gap = 10, ll_iconwh = 64;
	int ll_topgap = 8, ll_stposx;
	
	CMainFrame* lMainFrame;
	lMainFrame = (CMainFrame*)AfxGetMainWnd();
	pDB = &( lMainFrame->m_DB);

	nScreenW = ::GetSystemMetrics(SM_CXSCREEN);
	nScreenH = ::GetSystemMetrics(SM_CYSCREEN);

	m_maxMapW = nScreenW * 4/7;
	m_maxMapH = nScreenH * 13/18;


	m_bDragging = false;
	mb_winMin = false;

	// Left V Split 
	int ll_chainw;
	
	if ( nScreenW * 15/144> CHAINW)
		ll_chainw = CHAINW;
	else
		ll_chainw = nScreenW * 15/144;

	m_chainw = ll_chainw;
	GetDlgItem(IDC_BUTCLEAR)->ShowWindow(SW_HIDE);

	m_lsplitv.Create(WS_CHILD | WS_VISIBLE, CRect(ll_chainw , ll_posy, ll_chainw  , m_rect.bottom), this, IDC_LSPLITV);
	m_lsplitv.SetRange(ll_chainw-50, ll_chainw+50, -1);
	 
	/*CRect	rcWnd;*/
	CWnd* pParent = m_map1.GetParent();		
	if (::IsWindow(pParent->m_hWnd))
	{	
			m_map1.GetWindowRect(m_maprcWnd);	
			pParent->ScreenToClient(m_maprcWnd); 
	}
	
	//Right Side Panel pos
	int	ll_panelx;
	
 	if ( nScreenW * 38/144 < PANELW)
			ll_panelx = m_chainw + 5 + m_maxMapW;
	else
			ll_panelx = m_rect.right - PANELW;

	m_rpanelx = ll_panelx;

	m_splitV.Create(WS_CHILD | WS_VISIBLE, CRect(ll_panelx  , m_rect.top, ll_panelx   , m_rect.bottom), this, IDC_SPLITV);
	m_splitV.SetRange(0, 0, -1);

	//Top H Split
	m_splitTopH.Create(WS_CHILD | WS_VISIBLE, CRect( ll_posx  , ll_posy  , ll_panelx  , ll_posy ), this, IDC_SPLITTOPH);
	m_splitTopH.SetRange( ll_posy,  ll_posy, 1);

	//Tool Buttons
	ll_posx += ll_gap;
	m_butsch.LoadBitmaps(IDB_SEARCH_C, IDB_SEARCHSEL_C, IDB_SEARCH_C, IDB_SEARCH_C);
	m_butsch.MoveWindow(ll_posx , 4, ll_iconwh, ll_iconwh, true);

	m_butpbk.ShowWindow(SW_HIDE);

	ll_posx += ll_gap +ll_iconwh ;
	m_butrep.LoadBitmaps(IDB_REPORT_C, IDB_REPORTSEL_C, IDB_REPORT_C, IDB_REPORT_C);
	m_butrep.MoveWindow(ll_posx , 4, ll_iconwh, ll_iconwh, true);

	ll_posx += ll_gap +ll_iconwh ;
	m_butset.LoadBitmaps(IDB_SETUP_C, IDB_SETUPSEL_C, IDB_SETUP_C, IDB_SETUP_C);
	m_butset.MoveWindow( ll_posx , 4, ll_iconwh, ll_iconwh, true);
	ll_posx = m_rect.right - PANELW + 20;

	/*======= Left Side Bar ==================*/
	//Chain combobox
	m_comboGroup.MoveWindow(3, ll_iconwh + 35, ll_chainw-5 , 15, true);
	ll_posy = ll_iconwh + 80;

	m_listchain.MoveWindow(3, TOPY + 45, ll_chainw - 5, m_rect.bottom -TOPY - 80 );
	m_listchain.ShowWindow(true);
	m_listchain.ShowScrollBar(SB_VERT, TRUE); 

	//Map
 	//m_map1.MoveWindow(ll_chainw+ 5, TOPY, m_maxMapW, m_maxMapH - TOPY, true);
 
	//===========  Right Control Panel ===============
	ll_posy = ll_topgap;
	ll_posx = ll_panelx + 50;
	ll_stposx = ll_panelx + 15;
	
	//Combobox - group
	m_editFolder.GetClientRect(llrect);	
	
	//Folder
	((CStatic*)GetDlgItem(IDC_STGROUP2)) ->MoveWindow( ll_stposx , ll_posy, 40, 15, true);
	m_editFolder.MoveWindow(ll_posx, ll_posy, nScreenW*1/6, 24);
	m_butBrowse.AutoLoad( IDC_BUTBROWSE, this);
	m_butBrowse.MoveWindow(ll_posx + nScreenW*1/6 + 5, ll_posy, 24,24);
	
	//Map Row
	ll_posy += 30;
	((CStatic*)GetDlgItem(IDC_STGROUP3)) ->MoveWindow(ll_stposx, ll_posy, 20, 15, true);
	m_editMap.MoveWindow(ll_posx, ll_posy, nScreenW*1/6, 24);	
	m_butMap.AutoLoad(IDC_BUTMAP, this);
	m_butMap.MoveWindow(ll_posx + nScreenW*1/6 + 5, ll_posy, 24,24);
 
	ll_posy += 30;
	//m_checkRec.MoveWindow(	 ll_posx + 80, ll_posy, 240, llrect.Height());
	m_checkRec.ShowWindow(SW_HIDE);

	//Refresh Button
	mp_butRefresh = new CBitmapButton;
	DWORD dwStyle =WS_CHILD | WS_VISIBLE | BS_OWNERDRAW; 
	mp_butRefresh->Create(_T(""),dwStyle, CRect(0,0,24,24), this, REFRESHID);
	mp_butRefresh->LoadBitmaps(IDB_REFRESH, IDB_REFRESH, IDB_REFRESH, IDB_REFRESH);
	mp_butRefresh->MoveWindow( ll_posx , ll_posy , 24,24);
 
	((CButton*)GetDlgItem(IDC_RADIOMAP))->ShowWindow(SW_HIDE);
	((CButton*)GetDlgItem(IDC_RADIOMAT))->ShowWindow(SW_HIDE);	 

	//Camera
	ll_posy += 30;
	((CStatic*)GetDlgItem(IDC_STCAM)) ->MoveWindow(ll_stposx, ll_posy, 60, 15, true);
	m_listCam.MoveWindow( ll_posx , ll_posy + 15, m_rect.right - ll_posx -  12, 260);

	//Right Panel H Split
	ll_posy += 275;  
	m_splitH.Create(WS_CHILD | WS_VISIBLE, CRect( m_rpanelx ,  ll_posy  , m_rect.right  , ll_posy), this, IDC_SPLITH);
	m_splitH.SetRange(200, 200, -1);

	//Access Control
	ll_posy += 10;
	GetDlgItem(IDC_BUTCLEAR) ->ShowWindow(true);

	((CStatic*)GetDlgItem(IDC_STAC)) ->MoveWindow(ll_stposx, ll_posy, 110, 15, true);
	m_ckAlarm.MoveWindow(ll_stposx + 111, ll_posy, 130, 15, true);
	ll_posy +=30;
	 
	//Access Control Log
	//m_listAc.MoveWindow( ll_stposx , ll_posy, m_rect.right - ll_posx +58, m_rect.bottom - ll_posy - 30,  true);
	m_listAc.MoveWindow( ll_stposx , ll_posy, m_rect.right - m_rpanelx - 20, m_rect.bottom - ll_posy - 30,  true);

	//Build AC List Control

	//Localize
	CString csLoc1(_T(""));//date
	LOADSTRING(csLoc1, IDS_ACDATE);			

	CString csLoc2(_T(""));//time
	LOADSTRING(csLoc2, IDS_EMAPTIME);			

	CString csLoc3(_T(""));//status
	LOADSTRING(csLoc3, IDS_ACSTATUS);			

	CString csLoc4(_T(""));//dept
	LOADSTRING(csLoc4, IDS_ACDEPT);			

	CString csLoc5(_T(""));//name
	LOADSTRING(csLoc5, IDS_ACNAME);			

	CString csLoc6(_T(""));//id
	LOADSTRING(csLoc6, IDS_EMAPID);			
 
	LV_COLUMN ListColumn;
	WCHAR * ListTitles1[11] = {_T(""),csLoc1.GetBuffer(), csLoc2.GetBuffer(), csLoc3.GetBuffer(), csLoc4.GetBuffer(), csLoc5.GetBuffer(), csLoc6.GetBuffer(), csLoc6.GetBuffer(), csLoc6.GetBuffer(), csLoc6.GetBuffer(), csLoc6.GetBuffer() };
	for(int i = 0; i < 11; i++)
		{
			ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			ListColumn.fmt = LVCFMT_LEFT;
			ListColumn.iSubItem = i;

			if (i==0)  ListColumn.cx = 22; //Icon
			else if (i==1)  ListColumn.cx = 40; //Date
			else if (i==2)  ListColumn.cx = 55; //Time
			else if (i==3)  ListColumn.cx = 50; // Status - OK - Warning
			else if (i==4)  ListColumn.cx = 67; //Dept
			else if (i==5)  ListColumn.cx = 95; //Name
			else if (i==6)  ListColumn.cx = 25; //Camera ID
			else if (i==7)  ListColumn.cx = 0; //Card
			else if (i==8)  ListColumn.cx =0 ; //Cam Name
			else if (i==9)  ListColumn.cx =0 ; //Group Name
			else if (i==10)  ListColumn.cx =0 ; //recordid

			ListColumn.pszText = ListTitles1[i];
			m_listAc.InsertColumn(i, &ListColumn);
		}

		dwStyle = m_listAc.GetStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		m_listAc.SetExtendedStyle(dwStyle);

		//Bitmap in the list control
		m_smImg.Create(16, 16, ILC_COLOR | ILC_MASK, 0, 1);

		//List Event Bitmap Stuff
		CBitmap bIcon;
		bIcon.LoadBitmapW(IDB_REDCIRCLE);
		m_smImg.Add(&bIcon, RGB(0, 0, 0));
		m_listAc.SetImageList(&m_smImg, LVSIL_SMALL);

		//===== Event =====
		m_EventReceiveFlag = true;
 		m_EmapEventRec = new CEventReceive();

		UINT nRetEvent(0);
		hThreadEventRec = (HANDLE)_beginthreadex(NULL, 0,  GetEventFromService, (LPVOID)this, 0, &nRetEvent);
		mb_filter = false;	//Filter log

		fnInitControlDetail();
		fnTooltip();
}

//For Details Sections
BOOL CDlgAc::fnInitControlDetail()
{
			int	ll_detailh = 200, ll_edith=24 ;	//height
			int	ll_top = m_rect.bottom - ll_detailh;
			int	ll_x = m_chainw;

			ll_top =  nScreenH * 5/7 + 10;
			//m_splitBotH.Create(WS_CHILD | WS_VISIBLE, CRect(CHAINW+5   , ll_top , m_rect.right - PANELW  , ll_top ), this, IDC_SPLITBOTH);
			m_splitBotH.Create(WS_CHILD | WS_VISIBLE, CRect(m_chainw+5 , ll_top , m_rpanelx  , ll_top ), this, IDC_SPLITBOTH);
			m_splitBotH.SetRange( ll_top -100,  ll_top+100, 1);

			//Groupbox
			int ll_y = ll_top + 15;
			m_gbDetail.MoveWindow(ll_x + 10 ,  ll_y, nScreenW*80/144, nScreenH*2/9, 1);
			m_gbDetail.ShowWindow(SW_SHOW);

			//1st line
			int  ll_textl = 15, ll_textw = 110, ll_editw=140, ll_editx = 290;
			ll_y += 25;
			ll_x = m_chainw + ll_textl;
			GetDlgItem(IDC_STGROUPCAM)->MoveWindow(ll_x, ll_y, ll_textw,  ll_edith);
			ll_x += ll_textw + 5;
			GetDlgItem(IDC_EDITGROUP)->MoveWindow(ll_x, ll_y, ll_editw,  ll_edith);
			ll_x += ll_editw + 5;
			GetDlgItem(IDC_EDITCAM)->MoveWindow(ll_x, ll_y, ll_editw,  ll_edith);
			ll_x += ll_editw + 5;

			//m_butSwitch.ShowWindow(SW_HIDE);
			m_butSwitch.MoveWindow(ll_x, ll_y, ll_edith,  ll_edith);
			m_butSwitch.LoadBitmaps(IDB_GOTO, IDB_GOTO, IDB_GOTO, IDB_GOTO);

			//2nd line
			ll_y += 30;
			ll_x = m_chainw+ ll_textl;
			GetDlgItem(IDC_STDATETIME)->MoveWindow(ll_x, ll_y, ll_textw,  ll_edith);
			ll_x += ll_textw + 5;
			GetDlgItem(IDC_EDITDATE)->MoveWindow(ll_x, ll_y, ll_editw,  ll_edith);
			ll_x += ll_editw + 5;
			GetDlgItem(IDC_EDITTIME)->MoveWindow(ll_x, ll_y, ll_editw,  ll_edith);

			//3RD line - status
			ll_y += 30;
			ll_x =m_chainw +  ll_textl;

			GetDlgItem(IDC_STSTATUS)->MoveWindow(ll_x, ll_y, ll_textw - 35,  ll_edith);
			ll_x += ll_textw + 5;

			m_butRed.MoveWindow(ll_x - 30, ll_y, ll_edith, ll_edith);	
			m_butRed.LoadBitmaps(IDB_REDCIRCLE, IDB_REDCIRCLE, IDB_REDCIRCLE, IDB_REDCIRCLE);

			GetDlgItem( IDC_EDITSTATUS)->MoveWindow(ll_x, ll_y, ll_editw,  ll_edith);
			ll_x += ll_editw + 5;
			//GetDlgItem( IDC_BUTOPEN)->MoveWindow(ll_x, ll_y, ll_edith, ll_edith);
			m_butOpen.MoveWindow(ll_x, ll_y, ll_edith, ll_edith);
			m_butOpen.LoadBitmaps(IDB_OPENGATE24,IDB_OPENGATE24SEL, IDB_OPENGATE24, IDB_OPENGATE24);

			//4th line
			ll_y += 30;
			ll_x = m_chainw + ll_textl;
			if (nScreenW < 801)
				ll_editw = 90;
			GetDlgItem( IDC_STCDN)->MoveWindow(ll_x, ll_y, ll_textw,  ll_edith);
			ll_x += ll_textw + 5;
			GetDlgItem( IDC_EDITCARD)->MoveWindow(ll_x, ll_y, ll_editw,  ll_edith);
			ll_x += ll_editw + 5;
			GetDlgItem( IDC_EDITDEPT)->MoveWindow(ll_x, ll_y, ll_editw,  ll_edith);
			ll_x += ll_editw + 5;
			GetDlgItem( IDC_EDITNAME)->MoveWindow(ll_x, ll_y, ll_editw,  ll_edith);
			return true;
}


BOOL CDlgAc::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	mb_startup =true;
	fnInitControl();
	fnBuildCamGroup();

	fnInitFolder();
	fnBuildList();
	fnBuildAc();
	fnLoadCamInfo();
	fnInitGroup(); //Fill group combobox
	fnInitChain();
	fnInitAccess(); //start Access Control	
	fnInitLocalize();
	//fnStartupDisplayVideo();
	mb_startup =false;
	
	//Get Chainid
	int ll_minChainid = fnGetMinChainid();
	if (ll_minChainid>0)
		fnChainChanged(ll_minChainid);
	fnGroupChanged();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

 
void CDlgAc::fnBuildCamGroup(void)
{
	int ll_cnt=0;	
	CString	csChainDesc, csDefaultValue, csMapfile, csVideoOn;
	char	cMD, cVL, cDI;

	DWORD		dwStyle = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | CBRS_TOP;

	CString csSQL_DefaultGroup("SELECT chainid, chain_desc FROM ecac_chain ORDER BY 1");
	CRecordset	rsChain(pDB);
	rsChain.Open(CRecordset::forwardOnly, csSQL_DefaultGroup);
	
	CDBVariant  varID;
	STRCHAIN	lChain;

	while (!rsChain.IsEOF()) { 

			rsChain.GetFieldValue( _T("chainid"), varID);	
			lChain.nid = varID.m_iVal;
			if (ll_cnt==0)  m_chainid =  varID.m_iVal;

			rsChain.GetFieldValue(_T("chain_desc"), csChainDesc);
			lChain.Desc = csChainDesc.Trim();

			//rsGroup.GetFieldValue(_T("gatemap_file"), csMapfile);
			//lGroup.mapfile = csMapfile.Trim();
 
			//rsGroup.GetFieldValue(_T("default_"), csDefaultValue);
			//lGroup.Default_ = false;
			//if (csDefaultValue.Trim()==_T("Y")) lGroup.Default_ = true;

			m_comboGroup.InsertString(ll_cnt,  csChainDesc);	
			//m_pvtChain
			m_vtChain.push_back(lChain);
			rsChain.MoveNext();
			ll_cnt++;
	}
	m_comboGroup.SetCurSel(0);
	//fnGroupChanged();
	rsChain.Close();
	return;
}


LRESULT CDlgAc::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
 	if (message == WM_NOTIFY)
	{
		if (wParam == IDC_SPLITH || wParam == IDC_SPLITBOTH || wParam == IDC_LSPLITV)
		{	
			SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;
			//DoResizeH(pHdr->delta);
			DoResizeH(wParam, pHdr->delta);
		}
	}
	if (message == WM_NCLBUTTONDBLCLK)
	{	 
				STR_CAM* pCam = (STR_CAM*) lParam;
				int lid = pCam->camidx;
				
				if (lid>0 && lid<1000)
					fnDisplayVideo(lid);
			//STR_CAM* pCam = (STR_CAM*) lParam;
			//int lid = pCam->camidx;
			//CPoint* lpoint;
			//lpoint = pCam->cpoint;
			//fnDropWithin(pCam->cpoint);
			//fnAfterMove(pCam->xdiff, pCam->ydiff, m_groupid, pCam->camidx);
		
	} //if (message == WM_NCLBUTTONDBLCLK)

	if (message == WM_KEYDOWN)
	{
		if ( wParam ==VK_DELETE)
		{
				STR_CAM* pCam = (STR_CAM*) lParam;
				fnRemoveCam(pCam->camidx);	
		}
 	}

	if (message == WM_NCLBUTTONUP)
	{
			STR_CAM* pCam = (STR_CAM*) lParam;
			CPoint* lcpt = pCam->cpoint;
			bool lb_drop = fnDropWithin(lcpt);
			if (lb_drop)
			{
					fnAfterMove(pCam->xdiff, pCam->ydiff, m_groupid,pCam->camidx);
		
					//Save into DB
					try{
							CString ls_sql;
							ls_sql.Format(_T("UPDATE ec_camera_group_camera SET acmap_x = %d,  acmap_y=%d  WHERE cameragroupid=%d AND camera_idx=%d"), lcpt->x,  lcpt->y, m_groupid, pCam->camidx );				 	
							TRACE(ls_sql);
							pDB->ExecuteSQL(ls_sql);
					}
					catch(CDBException * p) 	{
						AfxMessageBox(p->m_strError);
						p->Delete();
					}
			}
	}

	return CBaseDialog::DefWindowProc(message, wParam, lParam);
}

void CDlgAc::DoResizeH(WPARAM wParam, int delta)
{
		CRect rc;

		//resize tab control and tab page
			if (wParam == IDC_SPLITH )
			{
				CSplitterControl::ChangeHeight(&m_listAc, -delta, CW_BOTTOMALIGN);
				CSplitterControl::ChangeHeight(&m_listCam, delta, CW_TOPALIGN);
				CSplitterControl::ChangePos(&m_stac, 0 , delta);
				CSplitterControl::ChangePos( GetDlgItem(IDC_CHECKWARN), 0, delta);
			}
			else if (wParam ==IDC_LSPLITV)
			{
				CSplitterControl::ChangeWidth(&m_listchain, delta, CW_LEFTALIGN);
			}
			else if (wParam ==  IDC_SPLITBOTH)
			{
				CSplitterControl::ChangeHeight(&m_map1, delta, CW_TOPALIGN);
				CSplitterControl::ChangePos(GetDlgItem( IDC_STGDETAIL), 0 , delta);

				CSplitterControl::ChangePos( &m_butSwitch, 0 , delta);
				
					CSplitterControl::ChangePos(GetDlgItem( IDC_STGROUPCAM ), 0 , delta);
					
						
				CSplitterControl::ChangePos(GetDlgItem( IDC_EDITGROUP ), 0 , delta);
				CSplitterControl::ChangePos(GetDlgItem( IDC_EDITCAM ), 0 , delta);
				CSplitterControl::ChangePos(&m_butOpen , 0 , delta);
				
				CSplitterControl::ChangePos(GetDlgItem( IDC_STDATETIME ), 0 , delta);
				CSplitterControl::ChangePos(GetDlgItem( IDC_EDITDATE ), 0 , delta);
				CSplitterControl::ChangePos(GetDlgItem(  IDC_EDITTIME), 0 , delta);
				CSplitterControl::ChangePos(GetDlgItem(  IDC_STSTATUS), 0 , delta);
				CSplitterControl::ChangePos(GetDlgItem( IDC_EDITSTATUS ), 0 , delta);
				CSplitterControl::ChangePos(GetDlgItem( IDC_STCDN ), 0 , delta);
				CSplitterControl::ChangePos(GetDlgItem(  IDC_EDITCARD), 0 , delta);
				CSplitterControl::ChangePos(GetDlgItem(  IDC_EDITDEPT), 0 , delta);
				CSplitterControl::ChangePos(GetDlgItem( IDC_EDITNAME ), 0 , delta);
				
			}
		Invalidate();
		UpdateWindow();
}

void CDlgAc::fnInitGroup(void)
{
		int ll_cnt=0;	
		CString	csGroupDesc, csDefaultValue, csVideoOn, ls_map;
		char	cMD, cVL, cDI;

		DWORD		dwStyle = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | CBRS_TOP;

		CString ls_sql = _T("SELECT ec_camera_group.cameragroupid, camera_group_desc,  default_, ecac_chainac.gatemap_file FROM ec_camera_group, ecac_chainac WHERE  ec_camera_group.cameragroupid = ecac_chainac.cameragroupid AND ecac_chainac.chainid>0 ORDER BY default_ DESC");
		CRecordset	rsGroup(pDB);
		rsGroup.Open(CRecordset::forwardOnly, ls_sql);
	
		CDBVariant  varID;
		STRGROUP lGroup;

		while (!rsGroup.IsEOF()) { 

				rsGroup.GetFieldValue( _T("cameragroupid"), varID);	
				lGroup.nid = varID.m_iVal;
				if (ll_cnt==0)  m_groupid =  varID.m_iVal;

			rsGroup.GetFieldValue(_T("camera_group_desc"), csGroupDesc);
			lGroup.Desc = csGroupDesc.Trim();

			rsGroup.GetFieldValue(_T("default_"), csDefaultValue);
			lGroup.Default_ = false;
			if (csDefaultValue.Trim()==_T("Y")) lGroup.Default_ = true;

			rsGroup.GetFieldValue(_T("gatemap_file"), ls_map);			
			lGroup.mapfile = ls_map;

			m_vtGroup.push_back(lGroup);
			rsGroup.MoveNext();
			ll_cnt++;
	}
	//m_comboGroup.SetCurSel(0);
	//fnGroupChanged();
	rsGroup.Close();
	return;

	//	int ll_cnt=0;	
	//CString	csGroupDesc, csDefaultValue, csMapfile, csVideoOn;
	//char	cMD, cVL, cDI;

	//DWORD		dwStyle = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | CBRS_TOP;

	//CString csSQL_DefaultGroup("SELECT cameragroupid, camera_group_desc, acmap_file,   default_ FROM ec_camera_group ORDER BY default_ DESC");
	//CRecordset	rsGroup(pDB);
	//rsGroup.Open(CRecordset::forwardOnly, csSQL_DefaultGroup);
	//
	//CDBVariant  varID;
	//STRGROUP lGroup;

	//while (!rsGroup.IsEOF()) { 

	//		rsGroup.GetFieldValue( _T("cameragroupid"), varID);	
	//		lGroup.nid = varID.m_iVal;
	//		if (ll_cnt==0)  m_groupid =  varID.m_iVal;

	//		rsGroup.GetFieldValue(_T("camera_group_desc"), csGroupDesc);
	//		lGroup.Desc = csGroupDesc.Trim();

	//		rsGroup.GetFieldValue(_T("acmap_file"), csMapfile);
	//		lGroup.mapfile = csMapfile.Trim();
 //
	//		rsGroup.GetFieldValue(_T("default_"), csDefaultValue);
	//		lGroup.Default_ = false;
	//		if (csDefaultValue.Trim()==_T("Y")) lGroup.Default_ = true;

	//		m_comboGroup.InsertString(ll_cnt,  csGroupDesc);	

	//		m_pvtGroup.push_back(lGroup);
	//		rsGroup.MoveNext();
	//		ll_cnt++;
	//}
	//m_comboGroup.SetCurSel(0);
	//fnGroupChanged();
	//rsGroup.Close();
	//return;
}



bool CDlgAc::fnGroupChanged(void)
{
		int nIndex=-1;
		CString strGroup,strWFile, wholeFile, ls_folder ;

		if (m_vtGate.size()<=0) return false;

// 		m_groupid = m_vtGateCam[oIndex].camgroupid;
 
		fnDeleteAllCam();
		fnCloseVideo();
		ib_dirty = false;  //prevent resave 


		//Get selected Group info from m_vtGroup
		for (int i=0; i<m_vtGroup.size(); i++)
		{
			if (m_vtGroup[i].nid == m_groupid)
			{
				//m_groupid = m_vtGroup[i].nid;
				nIndex = i;
				break;
			}
		}

		if (nIndex<0) return false;
 		
		m_editMap.SetWindowTextW(m_vtGroup[nIndex].mapfile);   //put file into

		if (m_groupid >0) fnPopulateCam(m_groupid); //display cameras in the list control
		m_editFolder.GetWindowTextW(ls_folder);
		wholeFile.Format(_T("%s\\%s"), ls_folder, m_vtGroup[nIndex].mapfile.Trim());

		// Temp comment out
		HBITMAP hbm = (HBITMAP) m_map1.GetBitmap();
		BITMAP bm;
		SIZE szBitmap;
		GetObject(hbm, sizeof(bm), &bm);
		CBitmap hbmp;
		 HBITMAP hbitmap;

		if (!fnFileExist(wholeFile)) //File NOT exist
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
	
		fnMaximizeBmp( mi_orgwidth, mi_orgheight);
		m_map1.MoveWindow( m_chainw+5, TOPY,  mi_orgwidth,  mi_orgheight - TOPY , true);
		m_map1.Load(wholeFile);

		//Repaint Cameras in map
		fnDisplayCamAc(m_groupid);
 		fnStartupDisplayVideo();
		
		return false;
}


void CDlgAc::fnDeleteAllCam(void)
{
	//Delete Camera icons
	for (int ci=0; ci < m_vpcam.size() ; ci++)
	{
				CEmapAccessCam* lpcam =  m_vpcam[ci];
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

//Populate Cameras into camera list
void CDlgAc::fnPopulateCam(int o_groupid)
{
	try
		{		
			if (&pDB==NULL)  return;
			CDBVariant varCamidx, varid, varNull;
			CString ls_sql1, ls_sql2, ls_sql3, ls_sql, ls_name, ls_ip, ls_full, ls_control;
			CRecordset  rs(pDB); 
			ls_sql1 = _T("SELECT ec_camera.camera_idx, ec_camera.ipaddress, ec_camera.cameraname, ec_camera_group_camera.cameragroupcameraid, ec_camera_group_camera.acmap_x, ec_camera_group_camera.acmap_y, ecac_camera.ac_cameraid, ecac_camera.access_brand,ecac_camera.ac_control ");
			ls_sql2 = _T("  FROM ec_camera INNER JOIN ec_camera_group_camera ON ec_camera.cameraid=ec_camera_group_camera.cameraid AND ec_camera_group_camera.cameragroupid=");
			ls_sql3 = _T(" LEFT OUTER JOIN ecac_camera ON ec_camera.camera_idx = ecac_camera.camera_idx ORDER BY ec_camera.cameraid ASC");
			ls_sql.Format(_T("%s%s%d%s"),ls_sql1, ls_sql2, o_groupid, ls_sql3);
			TRACE(ls_sql);
			rs.Open(CRecordset::forwardOnly, ls_sql);
			int cnt=0;	

			m_listCam.DeleteAllItems();
			//m_listcam.SetImageList(&m_cImageListSmall, LVSIL_SMALL);

			while (!rs.IsEOF()) { 
					CDBVariant  varx, vary, varaccamid;
					CString	ls_brand;
					varx = varNull;
					varaccamid =varNull;
					rs.GetFieldValue( _T("camera_idx"), varCamidx,  SQL_C_SSHORT );
					rs.GetFieldValue(_T("ipaddress"), ls_ip);	
					rs.GetFieldValue(_T("cameraname"), ls_name);
					rs.GetFieldValue( _T("cameragroupcameraid"), varid,  SQL_C_SSHORT );
					rs.GetFieldValue( _T("acmap_x"), varx,  SQL_C_SSHORT);
					rs.GetFieldValue( _T("acmap_y"), vary,  SQL_C_SSHORT );
					rs.GetFieldValue( _T("ac_cameraid"), varaccamid,  SQL_C_SSHORT );
					rs.GetFieldValue( _T("access_brand"), ls_brand );
					rs.GetFieldValue( _T("ac_control"), ls_control );
					
					ls_full.Format( _T("(%d) %s - %s"), varCamidx.m_iVal, ls_name, ls_ip);
					LVITEMW* pLitem = new LVITEM;
					pLitem->mask = LVIF_PARAM | LVIF_TEXT|LVIF_IMAGE;
					pLitem->iItem = cnt;
	
					int nItem;
					if (varx.m_iVal>0 && vary.m_iVal>0 && varx.m_iVal<2000 && vary.m_iVal<2000 && varaccamid.m_iVal > 0 )
					{	//In the map
								if (ls_control == _T("Y"))	//mark as access control
									nItem = m_listCam.InsertItem(cnt, _T(""), BMPKEYRED);  //check with key					
								else
									nItem = m_listCam.InsertItem(cnt, _T(""), BMPRED);  //check with key					
					}
					else	//Not in the map
					{
								if (ls_control == _T("Y"))	//mark as access control
									nItem = m_listCam.InsertItem(cnt, _T(""), BMPKEY);  //  key					
								else
									nItem = m_listCam.InsertItem(cnt, _T(""), BMPBLANK);  //BLANK				
					}

					m_listCam.SetItemText(nItem, 1, ls_full);
					CString ls_gcamid;
					//ls_gcamid.Format(_T("%d"),varid.m_iVal);
					ls_gcamid.Format(_T("%d"),varCamidx.m_iVal);
					m_listCam.SetItemText(nItem, 2, ls_gcamid);
					varx.m_iVal = 0;
					cnt++;
					rs.MoveNext();
			} //While
			rs.Close();
	} //try
		catch(CDBException * p) 	{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}	
}


bool CDlgAc::fnFileExist(CString o_file)
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

void CDlgAc::fnMaximizeBmp(int &o_maxW, int &o_maxH)
{
	//Return the Max size allowed	
	if (o_maxW > m_maxMapW && m_maxMapW>0) o_maxW=m_maxMapW;
	if (o_maxH > m_maxMapH && m_maxMapH>0) o_maxH = m_maxMapH;
}

bool CDlgAc::fnBuildList(void)
{
		//Camera List
		LV_COLUMN ListColumn;

		CString csLoc1(_T(""));//ipcam
		LOADSTRING(csLoc1, IDS_EMAPIPCAM);			

		CString csLoc2(_T(""));//id
		LOADSTRING(csLoc2, IDS_EMAPID);

		//WCHAR * ListTitles[3] = {_T(""), _T("IP Cameras"),_T("ID")};
		WCHAR * ListTitles[3] = {_T(""), csLoc1.GetBuffer(),  csLoc2.GetBuffer()};

		for(int i = 0; i < 3; i++)
		{
			ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
			ListColumn.fmt = LVCFMT_LEFT;
			ListColumn.iSubItem = i;

			if(i == 0)
				{ListColumn.cx = 18;}
			else if ( i == 1 )
				{ListColumn.cx = 240;}
			else if(i == 2)
				{ListColumn.cx = 0;}
			
			ListColumn.pszText = ListTitles[i];
			m_listCam.InsertColumn(i, &ListColumn);
		}

		DWORD dwStyle = m_listCam.GetStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		m_listCam.SetExtendedStyle(dwStyle);
		m_listCam.ModifyStyle(0, LVS_SINGLESEL);

		//CBitmap bIcon, bred;
		//m_SmallImg.Create(16, 16, ILC_COLOR | ILC_MASK, 0, 1);
		//bIcon.LoadBitmapW(IDB_KEY);
		//m_SmallImg.Add(&bIcon, RGB(0, 0, 0));
		//bred.LoadBitmapW( IDB_CHECKREDKEY);
		//m_SmallImg.Add(&bred, RGB(0, 0, 0));
		
		CBitmap bkey, bred, bkeyred;
		//Key 0; Checkred 1; checkred&key 2; empty 3
		m_SmallImg.Create(16, 16, ILC_COLOR | ILC_MASK, 0, 1);
		bkey.LoadBitmapW(IDB_KEY);
		m_SmallImg.Add(&bkey, RGB(0, 0, 0));
		
		bred.LoadBitmapW(IDB_CHECKRED);
		m_SmallImg.Add(&bred, RGB(0, 0, 0));
		
		bkeyred.LoadBitmapW( IDB_CHECKREDKEY);
		m_SmallImg.Add(&bkeyred, RGB(0, 0, 0));

		m_listCam.SetImageList(&m_SmallImg, LVSIL_SMALL);
	return false;
}

//Open Settings AC 
void CDlgAc::OnBnClickedButset()
{
	fnOpenSetting(true);
	//CDlgSettingAC lDlg;
	//lDlg.DoModal();
	//if (lDlg.mb_camChanged)
	//{
	//	//if changed in the settings, need to refresh the camera list
	//	fnGroupChanged();
	//}
	/*if (lDlg.mb_holChanged)
		fnRefreshVector(*/
}

void CDlgAc::OnBnClickedButrep()
{

// Run Report
	 TCHAR apppath[MAX_PATH]={0};
	GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
	CString csAppPath(apppath);
	int nPos = csAppPath.ReverseFind('\\');
	csAppPath = csAppPath.Left(nPos + 1);
	CString csReport = csAppPath + _T("\\EcReport.exe");

	ShellExecuteW(NULL,_T(""),csReport,NULL,NULL,SW_SHOWNORMAL);
}

void CDlgAc::fnTooltip(void)
{		
		CString	ls_st;
		m_pToolTipCtrl = new CToolTipCtrl;
		
		if (!m_pToolTipCtrl->Create(this))
			TRACE(_T("Unable To create ToolTip\n"));
		
		LOADSTRING(ls_st, IDS_ACSET);		
		m_pToolTipCtrl->AddTool(&m_butset, ls_st);

		LOADSTRING(ls_st, IDS_ACPLAYBACK);		
		m_pToolTipCtrl->AddTool(&m_butsch, ls_st);

		LOADSTRING(ls_st, IDS_SELECTFOLDER);		
		m_pToolTipCtrl->AddTool(&m_butBrowse, ls_st);

		LOADSTRING(ls_st, IDS_SELECTBMP);		
		m_pToolTipCtrl->AddTool(&m_butMap, ls_st);

		LOADSTRING(ls_st, IDS_ACREPORT);		
		m_pToolTipCtrl->AddTool(&m_butrep, ls_st);

		LOADSTRING(ls_st, IDS_ACREFRESH);		
		m_pToolTipCtrl->AddTool(mp_butRefresh, ls_st);
 }

BOOL CDlgAc::PreTranslateMessage(MSG* pMsg)
{
	if ( m_pToolTipCtrl != NULL)
		m_pToolTipCtrl->RelayEvent(pMsg);

	return CBaseDialog::PreTranslateMessage(pMsg);
}

bool CDlgAc::fnInitFolder(void)
{
	//If there's no row in the DB ec_storage of EMAP, put the working folder as the store loaction
	CString	ls_loc;
	CString ls_sql = (_T("SELECT store_location FROM ec_storage  WHERE storage_type='AC'"));
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
	else	//Empty storage in DB
	{
		TCHAR apppath[MAX_PATH]={0};
		GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
		
		CString csAppPath(apppath);
		int nPos = csAppPath.ReverseFind('\\');
		csAppPath = csAppPath.Left(nPos);
		m_editFolder.SetWindowTextW(csAppPath);
		m_Folder = csAppPath;
		mc_folderMode = 'E';	//empty

		ls_sql.Format(_T("INSERT INTO ec_storage (storage_type, store_location) VALUES ('AC', '%s') "), csAppPath.Trim() ); 
		TRACE(ls_sql);
		pDB->ExecuteSQL(ls_sql);

	}
 
	rsGroup.Close();
	return false;
}

void CDlgAc::OnBnClickedButbrowse()
{
	LPMALLOC pMalloc = NULL;
	LPITEMIDLIST pidl = NULL;
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));

	// set the bi's default values
	bi.hwndOwner = m_hWnd;
	bi.lpszTitle = _T("Modify access control map storage folder:");
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT;
 
	pidl = SHBrowseForFolder(&bi);
	if(pidl != NULL)
	{
		 CString pszPath;
		// free memory
		if(SUCCEEDED(SHGetMalloc(&pMalloc)) && pMalloc)
		{
			SHGetPathFromIDListW(pidl, pszPath.GetBuffer(256));
			m_editFolder.SetWindowTextW(pszPath.Trim());
			ib_dirty = true;
			if (mc_folderMode=='E')
			{}
			else	//Change folder
				mc_folderMode='C';
			
			pMalloc->Free(pidl);  
			pMalloc->Release(); 
	

		//Save into DB
		try{
		CString ls_sql;
		ls_sql.Format(_T("DELETE FROM ec_storage WHERE storage_type='AC' "));
		pDB->ExecuteSQL(ls_sql);

		ls_sql.Format(_T("INSERT INTO ec_storage (storage_type, store_location) VALUES ('AC', '%s') "), pszPath.Trim() ); 
			TRACE(ls_sql);
			pDB->ExecuteSQL(ls_sql);
		}
		catch(CDBException * p) 	{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
	}
	} //	if(pidl != NULL)
}

void CDlgAc::OnBnClickedButRefresh()
{
	//fnReSecurity();
	fnCloseVideo();
	fnGroupChanged();
}

void CDlgAc::OnBnClickedButmap()
{
		CString	ls_tfolder =_T("");
 
		CFileDialog fOpenDlg(TRUE, _T("bmp"), _T("Access Control Map File"), OFN_HIDEREADONLY|OFN_FILEMUSTEXIST, 	_T("Select Bitmap File (*.bmp)|*.bmp|"), this);
	  
		fOpenDlg.m_pOFN->lpstrTitle=_T("Select Bitmap File");
		CString ls_appPath = ((CMainFrame*)AfxGetMainWnd())->m_csAppPath;
		if (ls_appPath !=_T(""))
				fOpenDlg.m_pOFN->lpstrInitialDir = ls_appPath;
		else
				fOpenDlg.m_pOFN->lpstrInitialDir=_T("c:");
	  
		if(fOpenDlg.DoModal()==IDOK)
		{
				CString ls_path = fOpenDlg.GetPathName();
				CString ls_name = fOpenDlg.GetFileName();
				CString ls_sfile, ls_tfile;
	
				((CEdit*)GetDlgItem(IDC_EDITMAP))->SetWindowTextW(ls_name.Trim());

				fnLoadBmp(ls_path.Trim());
				//m_curFile = ls_path.Trim();

				m_editFolder.GetWindowTextW(ls_tfolder);
				ls_sfile.Format(_T("%s\\%s"),ls_tfolder,ls_name);		
				bool ls_ret = CopyFile(ls_path,ls_sfile, false); 
				//mc_fileMode = 'C';
				ib_dirty = true;

		try{
				CString	ls_sql;
				ls_sql.Format(_T("UPDATE ec_camera_group SET acmap_file = '%s' WHERE cameragroupid = %d "), ls_name.Trim(), m_groupid ); 
				TRACE(ls_sql);
				pDB->ExecuteSQL(ls_sql);
				
				//Update m_pvtGroup
				for (int i=0; i< m_vtGroup.size(); i++)
				{
					if (m_vtGroup[i].nid == m_groupid)
						m_vtGroup[i].mapfile = ls_name.Trim();
				}

			}
			catch(CDBException * p) 	{
				AfxMessageBox(p->m_strError);
				p->Delete();
		}
		}
}

//change camera group
void CDlgAc::OnCbnSelchangeCombogroup()
{
	int ll_sel = m_comboGroup.GetCurSel();

	for (int i=0; i< m_vtChain.size(); i++)
	{
		if (i==ll_sel)
		{			
			fnChainChanged(m_vtChain[i].nid);
			m_preitem = -1;	//force it to rebuild video...
		}
	}
	//fnCloseVideo();
	//fnGroupChanged();
}


void CDlgAc::OnLvnBegindragListcam(NMHDR *pNMHDR, LRESULT *pResult)
{
 
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  	
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
	m_dragTxt= m_listCam.GetItemText(m_nDragIndex,1);
	m_currGCamid = _ttoi(m_listCam.GetItemText(m_nDragIndex,2));

	//Return if camera already exist in the 
	if (fnCheckCamExist(m_currGCamid , m_nDragIndex))
		return;

	//We will call delete later (in LButtonUp) to clean this up
	m_pDragImage = m_listCam.CreateDragImage(m_nDragIndex, &pt);
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

void CDlgAc::OnLButtonUp(UINT nFlags, CPoint point)
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
			
			CString  cs;
			LOADSTRING(cs, IDS_ADDACCAM);
			//	= _T("Are you sure to add the camera as access control?") ;
			if ( AfxMessageBox(cs, MB_OKCANCEL,0 )== IDCANCEL)  return;

			CEmapAccessCam* lpCam= new CEmapAccessCam;	
			lpCam->Create(_T(""), WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(0, 0, CAMSIZEW, CAMSIZEH), this, 1);
			lpCam->LoadBitmaps(IDB_CAM37, IDB_CAM37SEL, IDB_CAM37SEL, IDB_CAM37);
			lpCam->mb_ac = true; //Access Control as parent
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

			m_listCam.SetItem(m_nDragIndex, 0, LVIF_PARAM | LVIF_TEXT|LVIF_IMAGE, _T(""), 1,0,0, NULL);
			fnSaveNewLoc(m_groupid, lpCam->m_idx , point.x, point.y);
			fnLoadCamInfo(lpCam->m_idx);
			fnInsertCamera(lpCam->m_idx);
			fnDisplayVideo(lpCam->m_idx);
 
		}
	}

	m_bDragging = false;
	m_dragTxt = _T("");
	ib_dirty = true;
	CDialog::OnLButtonUp(nFlags, point);
 
}

void CDlgAc::OnMouseMove(UINT nFlags, CPoint point)
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
		CBaseDialog::OnMouseMove(nFlags, point);
}

bool CDlgAc::fnDropWithin(LPPOINT mpt)
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

bool CDlgAc::fnCheckMapExist(void)
{
	CString		csMap;
	((CEdit*)GetDlgItem(IDC_EDITMAP))->GetWindowTextW(csMap);

	if (csMap==_T("")) return false;

	return true;
}

//true - camera already in the map, will abort
bool CDlgAc::fnCheckCamExist(int o_camGroupid, int o_camidx)
{
	for (int i=0; i<m_vpcam.size(); i++)
	{
		if (m_vpcam[i]->m_camgroupid==o_camGroupid &&  m_vpcam[i]->m_camidx == o_camidx && m_vpcam[i]->mc_mode !='D')		
			return true;
		if (m_vpcam[i]->m_mapx>0 && m_vpcam[i]->m_camidx == o_camidx)
			return true;
	}

	return false;
	//if camera idx is not in the ac camera idx list, return true;
	//int ll_camidx = _ttoi(m_listCam.GetItemText(o_camidx,2));
	//bool	lb_found = false;
	//for (int i=0; i<m_pvtAc.size(); i++)
	//{
	//	if (m_pvtAc[i].camidx == ll_camidx) lb_found = true;
	//}

	//return (!lb_found);
	//if (lb_found)	return false;
	//else return true;	//not found, abort
}

// Build ecac_camera into STRAC & vector
void CDlgAc::fnBuildAc(void)
{
	CString	ls_brand;
	CString ls_sql = (_T("SELECT ac_cameraid, camera_idx, access_brand FROM ecac_camera "));
	CRecordset	rsAc(pDB);
	rsAc.Open(CRecordset::forwardOnly, ls_sql);
	
	CDBVariant  varID, varCamidx;
	STRAC lAc;

	while (!rsAc.IsEOF()) { 
			
			rsAc.GetFieldValue( _T("ac_cameraid"),  varID, SQL_C_SSHORT );	
			lAc.nid = varID.m_iVal;

			rsAc.GetFieldValue( _T("camera_idx"),  varCamidx, SQL_C_SSHORT );
			rsAc.GetFieldValue( _T("access_brand"),  ls_brand );	

			lAc.camidx = varCamidx.m_iVal;
			lAc.csBrand = ls_brand.Trim().MakeUpper();
			m_pvtAc.push_back(lAc);
			rsAc.MoveNext();
	}
	rsAc.Close();
}


void CDlgAc::fnInitAccess(void)
{
	mb_AC = false;
 	is_glue = _T("");
	//Check if  Schema support access control
	CString	ls_loc;
	CDBVariant	varAC, varNull;
	CString ls_sql = (_T("select * from sys.tables where name = 'ecac_record'"));
	CRecordset	rsAC(pDB);
	rsAC.Open(CRecordset::forwardOnly, ls_sql);
	
	if (rsAC.GetRecordCount() <=0)
	{
		AfxMessageBox(_T("Access Control Schema Not Valid, Please Get Help From Admin."));
		return;
	}

	rsAC.Close();
 	//fnStartAccessService();
	fnRefreshHolVec();
	fnRefreshPriVec();

	//fnHolTimeStr();
}


void CDlgAc::fnSaveNewLoc(int oGroupid, int oCamidx, int posx, int o_posy)
{
	try{
		CString ls_sql;
		ls_sql.Format(_T("UPDATE ec_camera_group_camera SET acmap_x = %d,  acmap_y=%d  WHERE cameragroupid=%d AND camera_idx=%d"), posx,  o_posy, oGroupid,  oCamidx  );
 	
			TRACE(ls_sql);
			pDB->ExecuteSQL(ls_sql);
	}
	catch(CDBException * p) 	
	{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}	
}

//Display Camera icons in the map
bool CDlgAc::fnDisplayCamAc(int oGroupid)
{
	//Display camera icons into the bitmap
	CString cssql, cssql1, cssql2, cssql3;
	int emapx, emapy, ll_cnt=0;
 
	//Loop into ec_camera_group_camera and display the camer icons
 	cssql1 = _T("SELECT ec_camera_group_camera.acmap_x, ec_camera_group_camera.acmap_y, ec_camera_group_camera.cameragroupcameraid, ec_camera.cameraname, ec_camera.camera_idx,ecac_camera.ac_cameraid ");
	cssql2 =  _T(" FROM ec_camera_group_camera INNER JOIN ec_camera ON ec_camera.cameraid=ec_camera_group_camera.cameraid  LEFT OUTER JOIN ecac_camera ON ec_camera_group_camera.camera_idx = ecac_camera.camera_idx WHERE cameragroupid=");
	//cssql3 = _T("  AND acmap_x>0 AND acmap_y>0 ORDER BY ec_camera_group_camera.cameraid ");
	cssql3 = _T("  ORDER BY ec_camera_group_camera.cameraid ");

	cssql.Format( _T("%s%s%d%s"), cssql1, cssql2, oGroupid, cssql3);
	CRecordset	rsCam(pDB);
	rsCam.Open(CRecordset::forwardOnly, cssql);
	
	CDBVariant  varEmapx, varEmapy, varNull, varid, varidx, varaccameraid;
	STRGROUP lGroup;

	while (!rsCam.IsEOF()) { 
			varEmapx = varNull;

			rsCam.GetFieldValue( _T("acmap_x"), varEmapx, SQL_C_SSHORT);	
			emapx = varEmapx.m_iVal;

			rsCam.GetFieldValue( _T("acmap_y"), varEmapy, SQL_C_SSHORT);	
			emapy= varEmapy.m_iVal;
	
			//rsCam.GetFieldValue( _T("camera_idx"), varidx, SQL_C_SSHORT);
			
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

					CEmapAccessCam* lpCam= new CEmapAccessCam;
					lpCam->Create(_T(""), WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(emapx, emapy, CAMSIZEW, CAMSIZEH), this, CAMBUTID);
					lpCam->LoadBitmaps(IDB_CAM37, IDB_CAM37SEL, IDB_CAM37SEL, IDB_CAM37);
//					lpCam->vSetParent(_T("SETTING"));
					lpCam->MoveWindow(emapx, emapy, CAMSIZEW,CAMSIZEH,true);
					//	lpCam->BringWindowToTop();

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
					/*	lpCam->m_accameraid = varaccameraid.m_iVal;
						CEmapAccessCam*	lpAcCam = new CEmapAccessCam;

						lpAcCam->m_camidx = varidx.m_iVal;
						lpAcCam->Create(_T(""), WS_CHILD|WS_VISIBLE|BS_OWNERDRAW, CRect(emapx, emapy-CAMSIZEH, CAMSIZEH, CAMSIZEH), this, ll_cnt);
						lpAcCam->LoadBitmaps(IDB_KEYB, IDB_KEYB,IDB_KEYB,IDB_KEYB);
						lpAcCam->MoveWindow(emapx, emapy - CAMSIZEH, CAMSIZEH,CAMSIZEH,true);
						m_vpAccessCam.push_back(lpAcCam);*/
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
	rsCam.Close();
	return false;
}

bool CDlgAc::fnAfterMove(int o_newx, int o_newy, int o_camgroupid, int o_camidx)
{
		int	ll_basex=0;
		CPoint pt(o_newx, o_newy);
		bool lb_drop = fnDropWithin(&pt);

		CRect		lrect;
		m_map1.GetWindowRect(&lrect);
		ll_basex = lrect.left;

	for (int i=0; i<m_vpcam.size(); i++)
	{
		CEmapAccessCam* lpCam = m_vpcam[i];
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
	//for (int i=0; i<m_vpAccessCam.size(); i++)
	//{
	//	CEmapAccessCam*	lpAccess = m_vpAccessCam[i];
	//	if (lpAccess->m_camidx == o_camidx)
	//	{
	//		lpAccess->MoveWindow(o_newx , o_newy -ACCESSH , ACCESSW, ACCESSH, true);
	//	}
	//}

	//move the static text as well
	for (int i=0; i<m_vpName.size(); i++)
	{
		CBaseStatic*  lpstc = m_vpName[i];
		if (lpstc->m_id == o_camidx)
		{
			lpstc->MoveWindow(o_newx + 40, o_newy, STCW, STCH, true);
			break;
		}
	}
	//Save into DB as new location
	fnSaveNewLoc(m_groupid, o_camidx , o_newx, o_newy);
	return false;
}

void CDlgAc::fnRemoveCam(int o_camidx)
{
	for (int i=0; i<m_vpcam.size(); i++)
	{	
		if (o_camidx == m_vpcam[i]->m_idx)
			{
				CEmapAccessCam* lpCam = m_vpcam[i];
				lpCam->mb_dirty = true;
				lpCam->mc_mode = 'D'; //Delete			
				lpCam->DestroyWindow();		
				m_vpcam[i]->DestroyWindow();	
				ib_dirty = true;
				break;
			}
	}//for (int i=0; i<m_vpcam.size(); i++)		
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
	//m_vpEventVideo
		for (int i=0; i<m_vpEventVideo.size(); i++) 
		{
			if (m_vpEventVideo[i]->mc_mode =='P' && m_vpEventVideo[i]->m_camidx == o_camidx)
			{
				CVideoThin* lpVideo = ((CVideoThin*)&(m_vpEventVideo[i]->m_videovv));
				lpVideo->vStopPlay();
				lpVideo->vStopRecv();
				m_vpEventVideo[i]->CloseWindow();
				m_vpEventVideo[i]->DestroyWindow();
				m_vpEventVideo[i]->mc_mode = 'C';

				//Update UPDATE ec_camera_group_camera
				if (m_groupid>0)
				{
					try{
						CString ls_sql;
						ls_sql.Format(_T("UPDATE ec_camera_group_camera SET acmap_x = NULL,  acmap_y=NULL  WHERE cameragroupid=%d AND camera_idx=%d"), m_groupid, o_camidx  );	 	
						TRACE(ls_sql);
						pDB->ExecuteSQL(ls_sql);		
 
						fnPopulateCam(m_groupid); //display cameras in the list control
					}
					catch(CDBException * p) 	{
						AfxMessageBox(p->m_strError);
						p->Delete();
					}
				}
				break;
			}
		}
}


UINT CDlgAc::ReceiveAccess(LPVOID lpParam)
{
		CameraInfo* pCameraInfo = (CameraInfo*)lpParam;
		CDlgAc*	pDlgAc = (CDlgAc*)pCameraInfo->pWnd;

		CString csURL = _T("http://") + pCameraInfo->csIP  + _T("/config/get_serial.cgi");
		char *chUrl = new char[csURL.GetLength() + 1];
		wcstombs_s(NULL, chUrl, csURL.GetLength() + 1, csURL, _TRUNCATE);

		CString csUserPwd = pCameraInfo->csUser + _T(":") + pCameraInfo->csPwd;
		char *chUserPwd = new char[csUserPwd.GetLength() + 1];
		wcstombs_s(NULL, chUserPwd, csUserPwd.GetLength() + 1, csUserPwd, _TRUNCATE);

		CURLcode gCode = curl_global_init(CURL_GLOBAL_ALL);
		
		CURL* g_curl = curl_easy_init();
 
	  		CURLINFO	m_curlInfo;
		//	pCameraInfo->cDB = pDB;
			curl_easy_setopt(g_curl, CURLOPT_URL, chUrl);
			curl_easy_setopt(g_curl, CURLOPT_USERPWD, chUserPwd);
			curl_easy_setopt(g_curl, CURLOPT_READDATA, pCameraInfo);
			
			TRACE(_T("Curl easy call Notify \r\n"));

			curl_easy_setopt(g_curl, CURLOPT_WRITEFUNCTION, notifyWriter);
			curl_easy_setopt(g_curl, CURLOPT_WRITEDATA, pCameraInfo);
			curl_easy_setopt(g_curl, CURLOPT_CONNECTTIMEOUT, 20);		//Connection Time out
	 
			CURLcode pCode = curl_easy_perform(g_curl); 

			if(pCode == CURLE_COULDNT_CONNECT)
			{		
				  curl_easy_cleanup(g_curl);
				  delete chUrl;
				  delete chUserPwd;
				  CString ls_str, ls_err;
				  LOADSTRING(ls_str, IDS_NOTFUN)
				  ls_err.Format(_T("%s %s"),  pCameraInfo->csIP, ls_str );
				  pDlgAc->fnCntconnect(pCameraInfo->csCamIDX);
				  //AfxMessageBox(ls_err);
				  return -1;
			}
			else
			{				
					int	icnt=0;
					while(pCode != CURLE_COULDNT_CONNECT)
					{				
						//pCameraInfo->bFirstFlag = true;
						//control while loop...
						//icnt++;
						//if (icnt>10) return -1;
						pCode = curl_easy_perform(g_curl);  
						if	(pCode == CURLE_OK)
							return 0;
					}//while
			} // else - connect 

	/*		curl_easy_cleanup(g_curl);
			curl_global_cleanup();*/
			delete chUrl;
			delete chUserPwd;
			  
		return 1;
}

int CDlgAc::notifyWriter(char* data, size_t size, size_t nmemb,void *lParam)
{
 	if(lParam == NULL)	return 0;
	CameraInfo* pCameraInfo = (CameraInfo*)lParam;
	CDlgAc* pDlgAc = (CDlgAc*) (pCameraInfo->pWnd);
	
	TRACE(_T("Notify Writer \r\n"));

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
			{ vParseSegment(tzBuffer, m_pos);}

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
						lsAll.Format(_T("%s%s"),lsAll,ls_ad);
					}
				}
		}

		TRACE(_T("ALL:   ") + lsAll);
		if (pDlgAc!=NULL)	 
		{
				pDlgAc-> fnModifyAccess(lParam, lsAll, pCameraInfo->csCamIDX);
		}
	}
 
	return len;
}



static CString HexToAscii(CString s)
{
	CString lsAll;
	lsAll.GetBuffer(256);
	for(int i = 0; i < s.GetLength(); i+=2)
	{
		CString	lstmp=_T("");
		CString	ls_ret=_T("");
		lstmp = s.Mid(i, 2).MakeLower();

		if (lstmp==_T("0a")) ls_ret = _T( "F");
		else if (lstmp==_T("03")) ls_ret = _T( "T");	
		else if (lstmp==_T("0d")) ls_ret = _T( "R");
		else if (lstmp==_T("30")) ls_ret = _T( "0");
		else if (lstmp==_T("31")) ls_ret = _T( "1");
		else if (lstmp==_T("32")) ls_ret = _T( "2");
		else if (lstmp==_T("33")) ls_ret = _T( "3");
		else if (lstmp==_T("34")) ls_ret = _T( "4");
		else if (lstmp==_T("35")) ls_ret = _T( "5");
		else if (lstmp==_T("36")) ls_ret = _T( "6");
		else if (lstmp==_T("37")) ls_ret = _T( "7");
		else if (lstmp==_T("38")) ls_ret = _T( "8");
		else if (lstmp==_T("39")) ls_ret = _T( "9");
		else if (lstmp==_T("3a")) ls_ret = _T( ":");
		else if (lstmp==_T("2a")) ls_ret = _T( "*");
		else if (lstmp==_T("2b")) ls_ret = _T( "+");
		else if (lstmp==_T("2c")) ls_ret = _T( "'");
		else if (lstmp==_T("2d")) ls_ret = _T( "-");
		else if (lstmp==_T("03")) ls_ret = _T( "E");
		else if (lstmp==_T("44")) ls_ret = _T( "D");
		else if (lstmp==_T("47")) ls_ret = _T( "G");
		else if (lstmp==_T("4b")) ls_ret = _T( "K");
		else if (lstmp==_T("")) ls_ret = _T( "-");
		else ls_ret = lstmp.Mid(1,1);

		//ls_new = fnDavinciCode(lstmp);
		lsAll.Format(_T("%s%s"),lsAll,ls_ret);
	}

	return lsAll;
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


// Timer being triggerred, call send command CGI here
//Obsolete
void CDlgAc::fnAccessTimer(void)
{ 
		if ( m_vcCamInfo.size()<=0) return;

 		CameraInfo* pCameraInfo = m_vcCamInfo[0];

		// Pegasus
		CString csURL = _T("http://") + pCameraInfo->csIP  + _T("/config/send_cmd.cgi?cmd=05444f3030&");
		
		//Soyal
		//CString csURL = _T("http://") + pCameraInfo->csIP  + _T("/config/send_cmd.cgi?cmd=7e040125db01&");
		//CString csURL = _T("http://") + pCameraInfo->csIP + _T("/config/send_cmd.cgi?cmd=7e040118e6ff&");

		char *chUrl = new char[csURL.GetLength() + 1];
		wcstombs_s(NULL, chUrl, csURL.GetLength() + 1, csURL, _TRUNCATE);

		CString csUserPwd = pCameraInfo->csUser + _T(":") + pCameraInfo->csPwd;
		char *chUserPwd = new char[csUserPwd.GetLength() + 1];
		wcstombs_s(NULL, chUserPwd, csUserPwd.GetLength() + 1, csUserPwd, _TRUNCATE);

		CURLcode gCode = curl_global_init(CURL_GLOBAL_ALL);
		CURL* ms_curl = curl_easy_init();

		if(ms_curl == NULL)
			return ;

		CURLINFO	m_curlInfo;

		curl_easy_setopt(ms_curl, CURLOPT_URL, chUrl);
		curl_easy_setopt(ms_curl, CURLOPT_USERPWD, chUserPwd);
		CURLcode pCode = curl_easy_perform(ms_curl); 

		curl_easy_cleanup(ms_curl);
		
		delete chUrl;
		delete chUserPwd;
		return ;
}


void CDlgAc::fnAccessTimer(int oTimerid)
{ 
		if ( m_vcCamInfo.size()<=0) return;
		if (m_pvtAc.size()<=0) return;
		bool lb_found = false;

		CameraInfo* pCameraInfo;
		CString	ls_brand, ls_camIdx;
		CString csURL;
	
		for (int i=0; i<m_pvtAc.size(); i++)
		{
			if (m_pvtAc[i]._timer == oTimerid)		//find the right timer id
			{
				ls_brand = m_pvtAc[i].csBrand.Trim().MakeLower();
				for (int j=0; j<m_vcCamInfo.size(); j++)
				{
					ls_camIdx.Format(_T("%d"),m_pvtAc[i].camidx);
					m_timerIdx = m_pvtAc[i].camidx;
					if (m_vcCamInfo[j]->csCamIDX == ls_camIdx)
					{
						lb_found = true;
						pCameraInfo = m_vcCamInfo[j];
					}
				}
			}
		}

		if (!lb_found) return;	//make sure it find the right info
 	//	 = m_vcCamInfo[0];

		//// Pegasus
		//if (ls_brand == _T("pegasus"))
		//		csURL = _T("http://") + pCameraInfo->csIP  + _T("/config/send_cmd.cgi?cmd=05444f3030&");
		//else	// Soyal as default		
				csURL = _T("http://") + pCameraInfo->csIP  + _T("/config/send_cmd.cgi?cmd=7e040125db01&");//Soyal 
		//CString csURL = _T("http://") + pCameraInfo->csIP + _T("/config/send_cmd.cgi?cmd=7e040118e6ff&");

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
		curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 20);	
		CURLcode pCode = curl_easy_perform(m_curl); 

		if(pCode == CURLE_COULDNT_CONNECT)
			{		
				  curl_easy_cleanup(m_curl);
				  delete chUrl;
				  delete chUserPwd;
				  CString ls_str, ls_err;
				  LOADSTRING(ls_str, IDS_NOTFUN)
				  ls_err.Format(_T("%s %s"),  pCameraInfo->csIP, ls_str );
				  fnCntconnect(pCameraInfo->csCamIDX);
				  TRACE(_T("Error in Connect \r\n"));

				  int ll_ret = AfxMessageBox(ls_err);
				  return;
			}

		curl_easy_cleanup(m_curl);
		delete chUrl;
		delete chUserPwd;
		return ;
		//CURLINFO	m_curlInfo;

		//curl_easy_setopt(m_curl, CURLOPT_URL, chUrl);
		//curl_easy_setopt(m_curl, CURLOPT_USERPWD, chUserPwd);
		//CURLcode pCode = curl_easy_perform(m_curl); 

		//curl_easy_cleanup(m_curl);
		//
		//delete chUrl;
		//delete chUserPwd;
		//return ;
}


void CDlgAc::fnStartAccessService(void)
{
		USES_CONVERSION;
	 
		try
	{
		CString csIP, csUser, csPwd, csWebPort, csCamIDX, csCamID, csProtocol;
		CRecordset m_rs(pDB);
		
		//m_rs.Open(CRecordset::forwardOnly, _T("SELECT ec_camera.cameraid, ec_camera.camera_idx,  ec_camera.mac_address, ec_camera.ipaddress, ec_camera.httpport, ec_camera.username, ec_camera.password, ec_stream.stream_protocol, ec_stream.stream_name FROM ec_camera INNER JOIN ec_stream ON ec_camera.cameraid = ec_stream.cameraid  INNER JOIN ecgt_camera ON ec_camera.camera_idx = ecgt_camera.camera_idx  WHERE ecgt_camera.gate_control='Y' AND ec_stream.stream_name = 'stream 1'"));
		m_rs.Open(CRecordset::forwardOnly, _T("SELECT ec_camera.cameraid, ec_camera.camera_idx,  ec_camera.mac_address, ec_camera.ipaddress, ec_camera.httpport, ec_camera.username, ec_camera.password, ec_stream.stream_protocol, ec_stream.stream_name FROM ec_camera INNER JOIN ec_stream ON ec_camera.cameraid = ec_stream.cameraid  INNER JOIN ecac_camera ON ec_camera.camera_idx = ecac_camera.camera_idx  WHERE ecac_camera.ac_control='Y'  AND ec_stream.stream_name = 'stream 1'"));
		
		while(!m_rs.IsEOF())
		{
				m_rs.GetFieldValue(_T("cameraid"), csCamID);
				m_rs.GetFieldValue(_T("camera_idx"), csCamIDX);
				m_rs.GetFieldValue(_T("ipaddress"), csIP);
				m_rs.GetFieldValue(_T("httpport"), csWebPort);
				m_rs.GetFieldValue(_T("username"), csUser);
				m_rs.GetFieldValue(_T("password"), csPwd);
				m_rs.GetFieldValue(_T("stream_protocol"), csProtocol);
				
				csCamID.TrimRight();
				csCamIDX.TrimRight();
				csIP.TrimRight();
				csWebPort.TrimRight();
				csUser.TrimRight();
				csPwd.TrimRight();
				csProtocol.TrimRight();

				CameraInfo* o_CameraInfo = new CameraInfo();

				o_CameraInfo->csCamIDX = csCamIDX;
				o_CameraInfo->csIP = csIP;
				o_CameraInfo->csPort = csWebPort;
				o_CameraInfo->csProtocol = csProtocol;
				o_CameraInfo->csPwd = csPwd;
				o_CameraInfo->csUser = csUser;
				o_CameraInfo->pWnd = (CDlgEmap*)this;

				m_vcCamInfo.push_back(o_CameraInfo);
				m_rs.MoveNext();
		}
		m_rs.Close();
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	m_ini.setINIFullFileName(_T("\\ec.ini"));
	CString csAC = m_ini.getKeyValue(_T("count"),_T("access control"));

	//Use ini file timer as new timer, otherwise use default ACTIMER
	CString csTimer = m_ini.getKeyValue(_T("timer"),_T("access control"));
	int	mi_timer;
	if (csTimer!=_T(""))
		mi_timer = _ttoi(csTimer );
	else
		mi_timer = ACTIME;
 
	for(int iTemp = 0; iTemp < m_vcCamInfo.size(); iTemp++)
	{
			UINT nOldRet(0);
			m_vcCamInfo[iTemp]->cDB = pDB;
			m_vcCamInfo[iTemp]->pListAc = &m_listAc;

			//
			if (fnCheckiscontrol(m_vcCamInfo[iTemp]->csCamIDX) == false)
				continue;
			m_hAcThread = (HANDLE)::_beginthreadex(NULL, 0, ReceiveAccess, (LPVOID)m_vcCamInfo[iTemp], 0, &nOldRet);
		
			int m_timerid;
			if (iTemp ==0) m_timerid =  ID_TIMER_AC0;
			else if (iTemp ==1) m_timerid =  ID_TIMER_AC1;
			else if (iTemp ==2) m_timerid =  ID_TIMER_AC2;
			else if (iTemp ==3) m_timerid =  ID_TIMER_AC3;
			else if (iTemp ==4) m_timerid =  ID_TIMER_AC4;
			else if (iTemp ==5) m_timerid =  ID_TIMER_AC5;
			else if (iTemp ==6) m_timerid =  ID_TIMER_AC6;
			else if (iTemp ==7) m_timerid =  ID_TIMER_AC7;
			else if (iTemp ==8) m_timerid =  ID_TIMER_AC8;
			else if (iTemp ==9) m_timerid =  ID_TIMER_AC9;
			else if (iTemp ==10) m_timerid =  ID_TIMER_AC10;
			else if (iTemp ==11) m_timerid =  ID_TIMER_AC11;
			else if (iTemp ==12) m_timerid =  ID_TIMER_AC12;
			else if (iTemp ==13) m_timerid =  ID_TIMER_AC13;
			else if (iTemp ==14) m_timerid =  ID_TIMER_AC14;
			else if (iTemp ==15) m_timerid =  ID_TIMER_AC15;
			else if (iTemp ==16) m_timerid =  ID_TIMER_AC16;
			else if (iTemp ==17) m_timerid =  ID_TIMER_AC17;
			else if (iTemp ==18) m_timerid =  ID_TIMER_AC18;
			else if (iTemp ==19) m_timerid =  ID_TIMER_AC19;
			else if (iTemp ==20) m_timerid =  ID_TIMER_AC20;

			SetTimer(m_timerid, ACTIME, 0 );
			//SetTimer(m_timerid, ACTIME, 0 );
			fnTimertoVector(_ttoi(m_vcCamInfo[iTemp]->csCamIDX), ACDFTBRAND, m_timerid);
	 
	} // for loop...	

	SetTimer(ID_TIMER_SECUREP, 3000, 0);
	
}


void CDlgAc::OnTimer(UINT_PTR nIDEvent)
{
 	bool lb_auto = false;
	int	ltime = 0;
	int	iTimerid=0;
	m_timerIdx = 0;

	//For display security info once
	if (nIDEvent == ID_TIMER_SECUREP)
	{
		fnReSecurity();
		m_securecnt++;
		if (m_securecnt==6) KillTimer(ID_TIMER_SECUREP);
	}

	fnAccessTimer(nIDEvent);

	if (m_timerIdx>0)
	{
		for (int i=0; i<m_vpEventVideo.size(); i++)
		{
			if (m_vpEventVideo[i]->m_camidx == m_timerIdx && m_vpEventVideo[i]->mb_displayVideo== true)
			fnDisplayVideo(m_timerIdx);
		}
	}

	CBaseDialog::OnTimer(nIDEvent);
}


bool CDlgAc::fnModifyAccess(LPVOID param, CString oData, CString oCamIdx)
{
		CameraInfo* pCameraInfo = (CameraInfo*)param;

		return fnModifyAccess(param, oData, oCamIdx, _T("etrovision"));
}

bool CDlgAc::fnModifyAccess(LPVOID param, CString oData, CString oCamIdx, CString oBrand)
{
		CString	ls_event=_T(""), ls_rtype=_T("OK");
		bool okflag = false;
		CString	ls_id, ls_date, ls_hhmm;

		oBrand = oBrand.MakeLower().Trim();
		pDB = ((CameraInfo*)param)->cDB;
 
		if (oData.GetLength() <40) 
		{
			ls_event = oData.Mid(6,2).MakeLower();
			CString ls_secure = oData.Mid(30,2).MakeLower();

			if (ls_event == _T("03") && ls_secure == _T("45")) // Let 03 passed, security check
			{
					for (int i=0; i<m_pvtAc.size(); i++)
					{
						if (m_pvtAc[i].camidx == _ttoi(oCamIdx))
						{
							m_pvtAc[i].csecurity = 'Y';
							break;
						}
					}
					//fnSetCurrentTime(_ttoi(oCamIdx));	
					return true;
			}
			else
				return false;
		}

		fnSetVideoFlag( _ttoi(oCamIdx), false);
		CString	ls_ss, ls_mm, ls_hh, ls_dd, ls_MM, ls_yy;

		if (oBrand.MakeUpper() == ACDFTBRAND)	//Soyal Access Control
		{
			if (oData.Left(2)==_T("7e"))	//Start code
			{
				ls_event = oData.Mid(6,2).MakeLower();
				if (ls_event == _T("0b") || ls_event  == _T("03") ) //0b- normal; 03-Invalid Card
				{				 
					if (ls_event  == _T("03")) ls_rtype = _T("WARN");
					
					ls_ss = fnHexToDec(oData.Mid(10,2));
					ls_mm = fnHexToDec(oData.Mid(12,2));
					ls_hh = fnHexToDec(oData.Mid(14,2));

					ls_dd = fnHexToDec(oData.Mid(18,2));
					ls_MM = fnHexToDec(oData.Mid(20,2));
					ls_yy = fnHexToDec(oData.Mid(22,2));
	
					ls_date.Format(_T("20%s/%s/%s"),ls_yy, ls_MM, ls_dd);
					ls_hhmm.Format(_T("%s:%s:%s"),ls_hh,ls_mm, ls_ss);
					//ls_id.Format(_T("%s%s%s%s"), oData.Mid(38,2),  oData.Mid(40,2),  oData.Mid(46,2), oData.Mid(48,2));
					ls_id.Format(_T("%s%s%s"), oData.Mid(38,2),  oData.Mid(40,2),  oData.Mid(46,2));

					//string had been cut off
					if (ls_id==_T("") && oData.GetLength()<40)
					{
							mb_toglue = true;
							is_glue = oData;
							return false;
					}
 				}
				else if (ls_event == _T("04"))
					{
						fnClearRec(_ttoi(oCamIdx));
						return false;
					}
			}
			else
				return false;
		} //if (oBrand == _T("ETROVISION"))	//Soyal Access Control
		//else if  (oBrand == _T("pegasus"))	//Pegasus
		//{
		//	//CString	ls_id=_T(""), ls_date=_T(""), ls_hhmm=_T("");
		//	bool lb_ret = fnPegasusDecode(oData, ls_id, ls_yy, ls_MM, ls_dd, ls_hh, ls_mm, ls_ss);
		//
		//	if (ls_id == _T("")) return false;
		//		ls_date.Format(_T("20%s/%s/%s"),ls_yy, ls_MM, ls_dd);
		//		ls_hhmm.Format(_T("%s:%s"),ls_hh,ls_mm);
		//}

		//Get Camera Name & camera group name

		CString	ls_select, ls_camname=_T(""), ls_groupdesc=_T("");
		int		accessID=0;


		try{	
	/*			CRecordset m_rscam(pDB);
				ls_select.Format(_T("SELECT ec_camera.cameraname, ec_camera_group.camera_group_desc FROM ec_camera_group_camera LEFT OUTER JOIN ec_camera ON  ec_camera.camera_idx = ec_camera_group_camera.camera_idx  LEFT OUTER JOIN ec_camera_group ON ec_camera_group.cameragroupid = ec_camera_group_camera.cameragroupid WHERE ec_camera_group_camera.camera_idx = %s "),oCamIdx);
				TRACE(ls_select);
				m_rscam.Open(CRecordset::dynaset, ls_select);
				
				while(!m_rscam.IsEOF()   ) 
				{ 	  
					m_rscam.GetFieldValue( _T("cameraname"), ls_camname); 
					m_rscam.GetFieldValue( _T("camera_group_desc"),ls_groupdesc); 
					break;
				}*/
				fnGetcamgroupname(_ttoi(oCamIdx), ls_camname, ls_groupdesc);

				//Get Employee Name and Department
				CString ls_sql1, ls_sql2, ls_sql3, ls_sql, csDept, csGCode, csEmpName;
				CDBVariant		varAccessid;
		 
				CRecordset m_rs(pDB);

				ls_sql1 = _T("SELECT ecac_camera_group.access_group_code, ecac_camera_group.access_group_desc, ecac_access_employee.access_name, ecac_access_employee.accessid FROM ecac_access_employee ");
				ls_sql2 = _T(" , ecac_camera_group WHERE ecac_access_employee.access_group_code = ecac_camera_group.access_group_code");
				ls_sql3 = _T(" AND ecac_access_employee.access_masterid LIKE '"  );
				ls_sql1 = ls_sql1 + ls_sql2 + ls_sql3;

				ls_sql.Format( _T("%s%s"), ls_sql1 , ls_id );
				ls_sql += _T("%'");

				m_rs.Open(CRecordset::dynaset, ls_sql);

			if (m_rs.GetRecordCount() >0)
			{
  				m_rs.MoveFirst();

				while(!m_rs.IsEOF()   ) 
				{ 	  
					okflag = true;

					m_rs.GetFieldValue( _T("access_group_code"), csGCode); 
					m_rs.GetFieldValue( _T("access_group_desc"), csDept); 
					m_rs.GetFieldValue( _T("access_name"), csEmpName); 
					m_rs.GetFieldValue( _T("accessid"),  varAccessid, SQL_C_SLONG); 
					accessID = varAccessid.m_iVal;
				
					fnInsertRow(param, ls_event, ls_date, ls_hhmm, oCamIdx , csDept, csEmpName, csGCode, ls_id, ls_camname, ls_groupdesc  );
					break;
				}
			}
			m_rs.Close();
			//In case no related data in the DB
			if (!okflag)
			{
				fnInsertRow(param, ls_event, ls_date, ls_hhmm, oCamIdx,  _T("Unknown"), ls_id, csGCode, ls_id, ls_camname, ls_groupdesc );
				ls_rtype = _T("WARN");
			}
		}
		catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}

		//Don't save data into unauthrized data into DB
		if ( fnCheckAuthorize(_ttoi(oCamIdx)))
 			fnAccessInsertDB(ls_rtype, ls_date, ls_hhmm, ls_id, accessID, _ttoi(oCamIdx));

		if (oBrand.MakeUpper() == ACDFTBRAND)	fnClearRec(_ttoi(oCamIdx));

		//For display video
		m_EventTimer.iTimer = 7;
 		m_EventTimer.iIndex = _ttoi(oCamIdx);
		m_EventTimer.cMode = 'O'; //Open Video
		m_vpEventTimer.push_back(m_EventTimer);

		return true;
}

//Warn - Time is not within 
//No Priv. No privilege in the camera/group
bool CDlgAc::fnInsertRow(LPVOID param, CString oStatus, CString oDate, CString oTime, CString oCamidx, CString oDept, CString oName, CString oGCode,  CString oCard, CString oCamname, CString oGroup)
{
	LVITEM lvi;
	lvi.mask = LVIF_IMAGE | LVIF_TEXT;
 	CString ls_status;
	TCHAR szItem[256];
	lvi.iItem = 0;
	lvi.iSubItem = 0;
 	
	bool	lb_priv = false;
	bool	lb_within = false;



	lvi.pszText =szItem;

	if (oStatus == _T("0b")) //Normal
	{
		fnSetVideoFlag( _ttoi(oCamidx), false);

		//Check if date within holiday and time located within
		lb_priv =  fnCheckPriv(_ttoi(oCamidx), oGCode);
		lb_within = fnTimeWithin(oCamidx, oDate, oTime, oGCode);

		if (lb_priv)	{ //Pass Privilege
			if (lb_within) // Time within the group time OK, normal
			{
					//if Filter display warning only, return here...
					if (mb_filter)
						return false;
					lvi.iImage = 1;
			}
			else
			{
 					lvi.iImage = 0;
					ls_status = _T("Warn");
			}
		}
		else
		{
				lvi.iImage = 0;
				ls_status = _T("No Priv.");
		}
	}
	else if (oDate==_T("") && oTime==_T("") && oStatus==_T("04")) //Might happen...skip the 04 status
		return false;
	else	//Warning
	{
		fnSetVideoFlag( _ttoi(oCamidx), true);
		lvi.iImage = 0;
	}

	lvi.pszText = _T("");
	int nItem = m_listAc.InsertItem(&lvi);

	
	//Date
	lvi.iSubItem =1;	
	_stprintf(szItem, _T("%s"), oDate.Mid(5));
	lvi.pszText =szItem;
	m_listAc.SetItem(&lvi);

	//Time
	lvi.iSubItem =2;	
	CString csTime;
	csTime.Format(_T("%s:%s"), oTime.Mid(0,2), oTime.Mid(2,2));
	lvi.pszText = (LPTSTR)(LPCTSTR)(oTime);
	m_listAc.SetItem(&lvi);

	//Status
	lvi.iSubItem =3; 
	//Check if the camera is pass security
	//if ( fnCheckAuthorize(_ttoi(oCamidx)))   
	//{
			if (oStatus == _T("0b"))
			{
				if (oDept ==_T("Unknown"))// Unknown card, no need to check privilege & time within				
					ls_status = _T("Warn");
				else
				{
						ls_status = _T("OK");
						if (!lb_priv) ls_status = _T("No Priv.");
						if (!lb_within) ls_status = _T("Warn");
				}
			}
			else
				ls_status = _T("Warn");

			lvi.pszText = (LPTSTR)(LPCTSTR)(ls_status.MakeUpper());
			m_listAc.SetItem(&lvi);
			
			//Department
			lvi.iSubItem =4; 
			lvi.pszText = (LPTSTR)(LPCTSTR)(oDept.MakeUpper());
			m_listAc.SetItem(&lvi);

			//Name
			lvi.iSubItem =5;  
			lvi.pszText = (LPTSTR)(LPCTSTR)(oName);
			m_listAc.SetItem(&lvi);

			//Camera Index
			lvi.iSubItem =6;
			lvi.pszText = (LPTSTR)(LPCTSTR)(oCamidx);
			m_listAc.SetItem(&lvi);

			if (ls_status == _T("Warn"))
			{
				for (int i=0; i<m_vpEventVideo.size(); i++)
				{
					if (m_vpEventVideo[i]->m_camidx == _ttoi(oCamidx))
					{
							m_vpEventVideo[i]->mb_displayVideo= true;
							break;
					}
				}
			}

			//Cardcode
			lvi.iSubItem =7;  
			lvi.pszText = (LPTSTR)(LPCTSTR)(oCard);
			m_listAc.SetItem(&lvi);

			//Cam Name
			lvi.iSubItem =8;  
			lvi.pszText = (LPTSTR)(LPCTSTR)(oCamname);
			m_listAc.SetItem(&lvi);

			//Group Name
			lvi.iSubItem =9;  
			lvi.pszText = (LPTSTR)(LPCTSTR)(oGroup);
			m_listAc.SetItem(&lvi);

			//recordid
			//pDB->CommitTrans();
			lvi.iSubItem =10;
			CString	ls_sql;
			ls_sql =  _T("SELECT access_recordid FROM ecac_record WHERE access_recordid IN (SELECT MAX(access_recordid) FROM ecac_record)");

			CRecordset	rs(pDB);
			rs.Open(CRecordset::forwardOnly, ls_sql);
			CDBVariant  varID;
 			CString ls_recordid;

			while (!rs.IsEOF()) { 
				rs.GetFieldValue( _T("access_recordid"), varID,  SQL_C_SSHORT);	
 				ls_recordid.Format(_T("%d"), varID.m_iVal+1);
				if (varID.m_iVal>0)
				{
						lvi.pszText = (LPTSTR)(LPCTSTR)(ls_recordid);
						m_listAc.SetItem(&lvi);
				}
				break;
			}
	return false;
}


void CDlgAc::fnAccessInsertDB(CString oType, CString oDate, CString oTime, CString oID, int oAccessid, int oCamIdx)
{
	try{
 			CString	ls_sql, ls_dt;
			ls_dt.Format(_T("%s %s"),oDate, oTime);
		
			ls_sql.Format(_T("INSERT INTO ecac_record (record_daytime, record_type, access_masterid, accessid, camera_idx ) VALUES ( CONVERT(datetime, '%s'), '%s', '%s', %d, %d )"), ls_dt, oType, oID, oAccessid, oCamIdx  );
	 		TRACE(ls_sql);
			pDB->ExecuteSQL(ls_sql);
	}
	catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
}


// Free Memory
//obClose - true: to close dialog / false: to destroy dialog
void CDlgAc::fnFreeMemory(bool obClose)
{
			if (obClose) //Close dialog, might re-open
			{
 					if (m_pToolTipCtrl !=NULL )
					{
						delete m_pToolTipCtrl ;
						m_pToolTipCtrl = NULL;
					}

					for (int i=0; i<m_vpEventVideo.size(); i++)
					{
						if (m_vpEventVideo[i]) 
						{
							CEventVideo* lpemapVideo = 	m_vpEventVideo[i];
							delete m_vpEventVideo[i];
						}
					}
					m_vpEventVideo.clear(); 
			}  //if (obClose)

			if (!obClose) //Destroy
			{
				for (int i=0; i<m_pvtAc.size(); i++)
				{
					if (m_pvtAc[i]._timer>0)
						KillTimer(m_pvtAc[i]._timer);
				}
					m_pvtAc.clear();

					curl_global_cleanup();	
					

				for (int i=0; i<m_vctpAllCam.size(); i++)
				{
					if (m_vctpAllCam[i]) delete m_vctpAllCam[i];
				}
				m_vctpAllCam.clear(); 

				curl_global_cleanup();
			}	// if (!obClose)
  
}


void CDlgAc::fnDisplayVideo(int o_camidx)
{
	//Loop through m_vctpAllCam and find the camidx
	int ll_camx=0, ll_camy=0;
	for (int i=0; i<m_vpEventVideo.size(); i++)
	{
		if (m_vpEventVideo[i]->m_camidx == o_camidx)
		{
			m_vpEventVideo[i]->mb_displayVideo = false; //Prevent to be reuse
			if (m_vpEventVideo[i]->mc_mode == 'P')
				return;		//Abort if video is playing
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

	for (int i=0; i<	 m_vctpAllCam.size(); i++)
	{
		if ( m_vctpAllCam[i]->camera_idx == o_camidx)
		{
			try{
                    USES_CONVERSION;
					NODEITEM* lpCam = m_vctpAllCam[i];

					CEventVideo* m_EventVideo= new CEventVideo; 
					m_EventVideo->fnAssignNode(lpCam);
					m_EventVideo->fnAssignTitle(A2W(lpCam->camera_name));
					m_EventVideo->mc_mode = 'P'; //Play mode

					m_EventVideo->mc_parent = 'A';  //AC
					m_EventVideo->m_camidx = o_camidx;
					m_EventVideo->Create(IDD_DLGEMAPVIEW1, this);
		 
					CRect lrect, llrect;
					GetWindowRect(&lrect);
					GetClientRect(&llrect);
					if (mb_startup)
						ll_camx = ll_camx + llrect.left;
					else
						ll_camx = ll_camx + lrect.left;
					ll_camy = ll_camy + lrect.top;
					m_EventVideo->MoveWindow(CRect(ll_camx, ll_camy+40, ll_camx + ACVIDEOW,  ll_camy+50 + ACVIDEOH), true);

					m_EventVideo->ShowWindow(SW_SHOW);
					m_EventVideo->BringWindowToTop();

					m_EventVideo->mb_door = fnVideoDoor(o_camidx);			
					m_EventVideo->fnShowDoor();
					m_vpEventVideo.push_back(m_EventVideo);
					return;
			}
			catch(CException* p)
			{
						p->Delete();
			}
		}
 
	} //for loop
}

// Load related camera info into noteitem
bool CDlgAc::fnLoadCamInfo(void)
{
	USES_CONVERSION;
	HTREEITEM lprvItem;

	char cIp[16];
	CDBVariant varCamidx, varStreamPort, varCamIdx, varCamid,varWebPort;
	CString ls_sql, ls_desc, ls_camname, ls_ipaddress, ls_macaddress, ls_idx, ls_username, ls_password, ls_sprotocol,ls_webPort;
	CString csStreamName;
	
	NODEITEM lNode={0};
	NODEITEM* lpNode;
	lpNode = &lNode;


	CRecordset m_rs(pDB);

	//if ( m_vtAc.size()>0)
	//{
	//	CString ls_in = _T(" in ( ");

	//	for (int i=0; i<	m_vtGate.size(); i++)
	//	{
	//		CString ls_cell;
	//		ls_cell.Format(_T(" %d,"), m_vtGate[i].camgroupid);
	//		ls_in += ls_cell; 
	//	}
	//	
	//	ls_in = ls_in.Mid(0, ls_in.GetLength()-1) + _T(")  ");
	//	
	//		ls_sql = _T("SELECT stream_name, ec_camera.cameraid, ec_camera.camera_idx,  ec_camera.cameraname, ec_camera.ipaddress, ec_camera.mac_address,ec_camera.httpport, ec_camera.username, ec_camera.password, ec_stream.stream_port, ec_stream.stream_protocol ");
	//		ls_sql += _T(" FROM ec_camera INNER JOIN ec_camera_group_camera ON ec_camera_group_camera.camera_idx =ec_camera.camera_idx  LEFT OUTER JOIN ec_stream ON ec_stream.cameraid = ec_camera.cameraid WHERE ec_camera_group_camera.cameragroupid ");			
	//		ls_sql = ls_sql + ls_in + _T(" AND stream_type like '%LV%'  AND ec_camera.active_='Y' ORDER BY ec_camera.camera_idx");			 
	//}
	//else
	//{
	//		ls_sql = _T("SELECT stream_name, ec_camera.cameraid, ec_camera.camera_idx,  ec_camera.cameraname, ec_camera.ipaddress, ec_camera.mac_address,ec_camera.httpport, ec_camera.username, ec_camera.password, ec_stream.stream_port, ec_stream.stream_protocol ");
	//		ls_sql += _T(" FROM ec_camera LEFT OUTER JOIN ecgt_camera ON ecgt_camera.camera_idx =ec_camera.camera_idx  LEFT OUTER JOIN ec_stream ON ec_stream.cameraid = ec_camera.cameraid");
	//		ls_sql +=	_T(" WHERE stream_type like '%LV%'  AND ec_camera.active_='Y' ORDER BY ec_camera.camera_idx");
	//}


	ls_sql = _T("SELECT stream_name, ec_camera.cameraid, ec_camera.camera_idx,  ec_camera.cameraname, ec_camera.ipaddress, ec_camera.mac_address,ec_camera.httpport, ec_camera.username, ec_camera.password, ec_stream.stream_port, ec_stream.stream_protocol ");
	ls_sql += _T(" FROM ec_camera LEFT OUTER JOIN ecac_camera ON ecac_camera.camera_idx =ec_camera.camera_idx  LEFT OUTER JOIN ec_stream ON ec_stream.cameraid = ec_camera.cameraid");
	ls_sql +=	_T(" WHERE stream_type like '%LV%'  AND ec_camera.active_='Y' ORDER BY ec_camera.camera_idx");

	bool lb_ret = m_rs.Open(CRecordset::dynamic, ls_sql);

while(!m_rs.IsEOF()   ) 
	{ 	  
		NODEITEM* m_camInfo = new NODEITEM;
		::ZeroMemory(m_camInfo, sizeof(NODEITEM));
		TVINSERTSTRUCTW itemStru={0};
		//Get Streaming ID
		m_rs.GetFieldValue( _T("stream_name"), csStreamName); 
		csStreamName = csStreamName.Right(1);
		m_camInfo->nStreamID = _wtol(csStreamName.GetBuffer());	
		if (m_camInfo->nStreamID <=0)
		{
			m_camInfo->nStreamID = 1;
		}

		m_rs.GetFieldValue( _T("cameraid"), varCamid,  SQL_C_SLONG );
		if (varCamid.m_iVal>=0) 
			m_camInfo->cameraid = varCamid.m_iVal;

		m_rs.GetFieldValue( _T("camera_idx"), varCamidx,  SQL_C_SSHORT );
		//lNode.camera_idx = 	varField.m_iVal;
		m_camInfo->camera_idx = varCamidx.m_iVal;

		//Camera Name to wcAlias
		m_rs.GetFieldValue(_T("cameraname"), ls_camname);
		ls_camname = ls_camname.Trim()+'\0';
	
		strncpy(m_camInfo->camera_name , W2A(ls_camname), ls_camname.GetLength());
		//lpNode->camera_name = ls_camname.Trim();
		_tcscpy(m_camInfo->wcAlias, ls_camname.GetBuffer(32));		
	
		//ipAddress
		m_rs.GetFieldValue(_T("ipaddress"), ls_ipaddress);
		ls_ipaddress = ls_ipaddress.Trim() +'\0';
		strncpy(m_camInfo->ip , W2A(ls_ipaddress), ls_ipaddress.GetLength());
	
		//Mac Address
		m_rs.GetFieldValue(_T("mac_address"), ls_macaddress);
		ls_macaddress = ls_macaddress.Trim();
		char szMac[13]={0};
		strncpy(szMac, W2A(ls_macaddress), 12); 
		for (int ci=0; ci < 6; ci++)
		{
			char szTmp[3]={0};
			memcpy(szTmp, szMac+ci*2, 2);
			int szBuf;
			sscanf_s(szTmp, "%02X", &szBuf);
			m_camInfo->mac[ci] = (BYTE)szBuf;
		}

		// WebPort		
		m_rs.GetFieldValue(_T("httpport"),varWebPort, SQL_C_SLONG);
		m_camInfo->nWebPort = varWebPort.m_iVal;

		//Username
		m_rs.GetFieldValue(_T("username"), ls_username);
		ls_username = ls_username.Trim() + '\0';
		strncpy(m_camInfo->szUser, W2A(ls_username), ls_username.GetLength());

		//password
		m_rs.GetFieldValue(_T("password"), ls_password);
		ls_password = ls_password.Trim()+'\0';
		strncpy(m_camInfo->szPass, W2A(ls_password), ls_password.GetLength());
	 		 
		m_rs.GetFieldValue(_T("stream_port"), varStreamPort, SQL_C_SLONG);
		if (varStreamPort.m_iVal < 0)
			m_camInfo->nAPPort = DEFAULTSPORT;
		else
			m_camInfo->nAPPort = varStreamPort.m_iVal;

		//stream protocol
		m_rs.GetFieldValue(_T("stream_protocol"), ls_sprotocol);
		ls_sprotocol.Trim();
		if  (ls_sprotocol=="AVIFILE")
			m_camInfo->nProtocol=8;
		else if (ls_sprotocol=="RTSP")
			m_camInfo->nProtocol=4;
		else if (ls_sprotocol=="CGI")
			m_camInfo->nProtocol=2;
		else
			m_camInfo->nProtocol=1;

		//ls_desc.Format( _T("(%d) %s - %s"),varCamidx.m_iVal, ls_camname, ls_ipaddress);

		//lpNode = &lNode;

		m_vctpAllCam.push_back(m_camInfo);
	 
      m_rs.MoveNext(   ); 
	}

	m_rs.Close();
	return false;
}


bool CDlgAc::fnLoadCamInfo(int oCamidx)
{
	USES_CONVERSION;
	HTREEITEM lprvItem;

	char cIp[16];
	CDBVariant varCamidx, varStreamPort, varCamIdx, varCamid,varWebPort;
	CString ls_sql, ls_desc, ls_camname, ls_ipaddress, ls_macaddress, ls_idx, ls_username, ls_password, ls_sprotocol,ls_webPort;
	CString csStreamName;
	
	NODEITEM lNode={0};
	NODEITEM* lpNode;
	lpNode = &lNode;


	CRecordset m_rs(pDB);

	ls_sql = _T("SELECT stream_name, ec_camera.cameraid, ec_camera.camera_idx,  ec_camera.cameraname, ec_camera.ipaddress, ec_camera.mac_address,ec_camera.httpport, ec_camera.username, ec_camera.password, ec_stream.stream_port, ec_stream.stream_protocol ");
	ls_sql += _T(" FROM ec_camera INNER JOIN ecac_camera ON ecac_camera.camera_idx =ec_camera.camera_idx  LEFT OUTER JOIN ec_stream ON ec_stream.cameraid = ec_camera.cameraid");
	ls_sql +=	_T(" WHERE stream_type like '%LV%'  AND ec_camera.active_='Y'  and ec_camera.camera_idx  = ");

	CString ls_sqlM;
	ls_sqlM.Format(_T("%s%d"), ls_sql, oCamidx);

	bool lb_ret = m_rs.Open(CRecordset::dynamic, ls_sqlM);

while(!m_rs.IsEOF()   ) 
	{ 	  
		NODEITEM* m_camInfo = new NODEITEM;
		::ZeroMemory(m_camInfo, sizeof(NODEITEM));
		TVINSERTSTRUCTW itemStru={0};
		//Get Streaming ID
		m_rs.GetFieldValue( _T("stream_name"), csStreamName); 
		csStreamName = csStreamName.Right(1);
		m_camInfo->nStreamID = _wtol(csStreamName.GetBuffer());	
		if (m_camInfo->nStreamID <=0)
		{
			m_camInfo->nStreamID = 1;
		}

		m_rs.GetFieldValue( _T("cameraid"), varCamid,  SQL_C_SLONG );
		if (varCamid.m_iVal>=0) 
			m_camInfo->cameraid = varCamid.m_iVal;

		m_rs.GetFieldValue( _T("camera_idx"), varCamidx,  SQL_C_SSHORT );
		//lNode.camera_idx = 	varField.m_iVal;
		m_camInfo->camera_idx = varCamidx.m_iVal;

		//Camera Name to wcAlias
		m_rs.GetFieldValue(_T("cameraname"), ls_camname);
		ls_camname = ls_camname.Trim()+'\0';
	
		strncpy(m_camInfo->camera_name , W2A(ls_camname), ls_camname.GetLength());
		//lpNode->camera_name = ls_camname.Trim();
		_tcscpy(m_camInfo->wcAlias, ls_camname.GetBuffer(32));		
	
		//ipAddress
		m_rs.GetFieldValue(_T("ipaddress"), ls_ipaddress);
		ls_ipaddress = ls_ipaddress.Trim() +'\0';
		strncpy(m_camInfo->ip , W2A(ls_ipaddress), ls_ipaddress.GetLength());
	
		//Mac Address
		m_rs.GetFieldValue(_T("mac_address"), ls_macaddress);
		ls_macaddress = ls_macaddress.Trim();
		char szMac[13]={0};
		strncpy(szMac, W2A(ls_macaddress), 12); 
		for (int ci=0; ci < 6; ci++)
		{
			char szTmp[3]={0};
			memcpy(szTmp, szMac+ci*2, 2);
			int szBuf;
			sscanf_s(szTmp, "%02X", &szBuf);
			m_camInfo->mac[ci] = (BYTE)szBuf;
		}

		// WebPort		
		m_rs.GetFieldValue(_T("httpport"),varWebPort, SQL_C_SLONG);
		m_camInfo->nWebPort = varWebPort.m_iVal;

		//Username
		m_rs.GetFieldValue(_T("username"), ls_username);
		ls_username = ls_username.Trim() + '\0';
		strncpy(m_camInfo->szUser, W2A(ls_username), ls_username.GetLength());

		//password
		m_rs.GetFieldValue(_T("password"), ls_password);
		ls_password = ls_password.Trim()+'\0';
		strncpy(m_camInfo->szPass, W2A(ls_password), ls_password.GetLength());
	 		 
		m_rs.GetFieldValue(_T("stream_port"), varStreamPort, SQL_C_SLONG);
		if (varStreamPort.m_iVal < 0)
			m_camInfo->nAPPort = DEFAULTSPORT;
		else
			m_camInfo->nAPPort = varStreamPort.m_iVal;

		//stream protocol
		m_rs.GetFieldValue(_T("stream_protocol"), ls_sprotocol);
		ls_sprotocol.Trim();
		if  (ls_sprotocol=="AVIFILE")
			m_camInfo->nProtocol=8;
		else if (ls_sprotocol=="RTSP")
			m_camInfo->nProtocol=4;
		else if (ls_sprotocol=="CGI")
			m_camInfo->nProtocol=2;
		else
			m_camInfo->nProtocol=1;

		m_vctpAllCam.push_back(m_camInfo);
	 
      m_rs.MoveNext(   ); 
	}

	m_rs.Close();
	return false;
}

void CDlgAc::fnAfterVideoClose(int o_camidx)
{
	//Called from video closed, mark m_vpEventVideo mc_mode as close
	for (int i=0; i<m_vpEventVideo.size(); i++)
	{
		if (m_vpEventVideo[i]->m_camidx == o_camidx)
		{
			m_vpEventVideo[i]->mc_mode = 'C';
		}
	}
	 
 }


//For open 2nd time of Access Control
void CDlgAc::fnStartupDisplayVideo()
{

	for (int i=0; i<m_vctpAllCam.size(); i++)
	{
		if (m_vctpAllCam[i]->camera_idx >0)
		{
			if (fnCamWithinGroup(m_groupid, m_vctpAllCam[i]->camera_idx ))
				fnDisplayVideo(m_vctpAllCam[i]->camera_idx);
		}
	}

}

bool CDlgAc::fnCamWithinGroup(int oGroupid, int oCamidx)
{
	try{
		if (oGroupid<=0) oGroupid = m_groupid;
		CDBVariant  varEmapx, varEmapy;
		CString ls_sql;
		ls_sql.Format(_T("SELECT ec_camera_group_camera.cameragroupcameraid, acmap_x, acmap_y  FROM ec_camera_group_camera  INNER JOIN ec_camera ON ec_camera_group_camera.cameraid = ec_camera.cameraid WHERE ec_camera_group_camera.camera_idx = %d and cameragroupid=%d and ec_camera.active_='Y' and ec_camera_group_camera.acmap_x>0 and ec_camera_group_camera.acmap_y>0"), oCamidx, oGroupid);
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

bool CDlgAc::fnCloseVideo(void)
{
		for (int i=0; i<m_vpEventVideo.size(); i++) 
		{
		if (m_vpEventVideo[i]->mc_mode == 'P') //Play
			{
				CVideoThin* lpVideo = ((CVideoThin*)&(m_vpEventVideo[i]->m_videovv));
				lpVideo->vStopPlay();
				lpVideo->vStopRecv();
				m_vpEventVideo[i]->CloseWindow();
				m_vpEventVideo[i]->DestroyWindow();
				m_vpEventVideo[i]->mc_mode = 'C';

			}
		}
		m_vpEventVideo.clear();
		return false;
}

void CDlgAc::OnClose()
{
	fnCloseVideo();
	fnFreeMemory(true);
 
 	CBaseDialog::OnClose();	
}

//For reference....
UINT CDlgAc::fnReceiveNotifyStream(LPVOID lpParam)
{
	/*
		 CameraInfo* pCameraInfo = (CameraInfo*)lpParam;
		 CString csURL = _T("http://") + pCameraInfo->csIP + _T(":") + pCameraInfo->csPort + _T("/config/notify_stream.cgi");
		 char *chUrl = new char[csURL.GetLength() + 1];
		 wcstombs_s(NULL, chUrl, csURL.GetLength() + 1, csURL, _TRUNCATE);
		 CString csUserPwd = pCameraInfo->csUser + _T(":") + pCameraInfo->csPwd;
		 char *chUserPwd = new char[csUserPwd.GetLength() + 1];
		 wcstombs_s(NULL, chUserPwd, csUserPwd.GetLength() + 1, csUserPwd, _TRUNCATE);
		 CURLcode gCode = curl_global_init(CURL_GLOBAL_ALL);
		 if(gCode != CURLE_OK)
		 {
			LogEvent(_T("curl_init failed." )); 
		 }
		 CURL* m_curl = curl_easy_init();
		 if(m_curl == NULL)
		 {LogEvent(_T("curl_easy_init failed."));} 
		 curl_easy_setopt(m_curl, CURLOPT_URL, chUrl);
		 curl_easy_setopt(m_curl, CURLOPT_USERPWD, chUserPwd);
		 curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, notifyWriter);
		 curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, pCameraInfo);
		 //curl_easy_setopt(m_curl, CURLOPT_CONNECTTIMEOUT, 10); 
		 curl_easy_setopt(m_curl, CURLOPT_TIMEOUT, 60); 
		 pCameraInfo->bFirstFlag = true;
		 CURLcode pCode = curl_easy_perform(m_curl); 
		 if(pCode == CURLE_COULDNT_CONNECT)
		 {
		  //LogEvent(_T("IP: %s, CURLcode: %d"), pCameraInfo->csIP, pCode);
		  curl_easy_cleanup(m_curl);
		  curl_global_cleanup();
		  delete chUrl;
		  delete chUserPwd;
		  return 0;
		 }
		 else
		 {
		  while(pCode != CURLE_COULDNT_CONNECT)
		  {
		   //LogEvent(_T("IP: %s, CURLcode: %d"), pCameraInfo->csIP, pCode);  
		   pCameraInfo->bFirstFlag = true;
		   pCode = curl_easy_perform(m_curl);  
		   if(pCode == CURLE_OK)
		   {
			map<CString, CameraInfo>::iterator iPos = mapCameraInfo.begin();
			iPos = mapCameraInfo.find(pCameraInfo->csIP);
			mapCameraInfo.erase(iPos);
			curl_easy_cleanup(m_curl);
			curl_global_cleanup();
			delete chUrl;
			delete chUserPwd;
			::Sleep(60000);
			HANDLE hRestartEvent = ::CreateEvent(NULL, false, false, _T("Global\\ETROCENTER_EVENT_RESTART"));
			if(hRestartEvent == NULL) LogEvent(_T("Create Restart Event Error Code: %d"), ::GetLastError());
			SetEvent(hRestartEvent);
			CloseHandle(hRestartEvent);
			return 0;
		   }
		  }
		 }
		 curl_easy_cleanup(m_curl);
		 curl_global_cleanup();
		 delete chUrl;
		 delete chUserPwd;
		 //LogEvent(_T("IP: %s Curl terminated."), pCameraInfo->csIP);*/
		 return 0;
}

bool CDlgAc::fnLoadBmp(CString o_file)
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

		//m_map1.MoveWindow(0, TOPY, mi_orgwidth,  mi_orgheight ,true);
		m_map1.Load(o_file);
		m_map1.ShowWindow(SW_SHOW);

	return false;
}
//Open Dialog Search Screen
void CDlgAc::OnBnClickedButsearch()
{
			TCHAR apppath[MAX_PATH]={0};
			GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
			CString	csAppPath(apppath);
	        int nPos = csAppPath.ReverseFind('\\');
			csAppPath = csAppPath.Left(nPos + 1);
 
			csAppPath += _T("accessplay.exe");
			ShellExecute(NULL,_T("open"), csAppPath, NULL,NULL,SW_SHOWNORMAL);		
  
}

void CDlgAc::OnModifyEmployee()
{
	fnOpenSetting(false); //Not normal open , open default of 
	/*CDlgSettingAC lDlg;
	lDlg.mb_openNormal = false;
	lDlg.DoModal();*/

}


CString CDlgAc::fnHexToDec(CString oSource)
{

	TCHAR *pEnd; 
	CStringA oSourceA;

	unsigned long ulNumber = _tcstoul((LPCTSTR)oSource, &pEnd, 16); 
	CString str, str1; 
	str.Format(_T("%x"), ulNumber); 
	str1.Format(_T("%d"), ulNumber); 
	if (str1.GetLength()==1)
		str1.Format(_T("0%d"), ulNumber);

	return str1;
}

//For Soyal need to clear previous info
bool CDlgAc::fnClearRec(int oCamIdx)
{
		bool lb_found = false;
		CString	ls_camIdx;
		if ( m_vcCamInfo.size()<=0) return false;
		if ( m_pvtAc.size()<=0) return false;
		CameraInfo* pCameraInfo;
 		//CameraInfo* pCameraInfo = m_vcCamInfo[0];
	for (int i=0; i<m_pvtAc.size(); i++)
		{
			if (m_pvtAc[i].camidx == oCamIdx && m_pvtAc[i].csBrand.Trim().MakeUpper()==ACDFTBRAND )		//find the right timer id
			{
 
				for (int j=0; j<m_vcCamInfo.size(); j++)
				{
					ls_camIdx.Format(_T("%d"),oCamIdx);
					if (m_vcCamInfo[j]->csCamIDX == ls_camIdx)
					{
						lb_found = true;
						pCameraInfo = m_vcCamInfo[j];
					}
				}
			}
		}

		if (!lb_found) return false;	//make sure it find the right info

		
		//Soyal - clear record cgi
		CString csURL = _T("http://") + pCameraInfo->csIP  + _T("/config/send_cmd.cgi?cmd=7e040137c901&");

		char *chUrl = new char[csURL.GetLength() + 1];
		wcstombs_s(NULL, chUrl, csURL.GetLength() + 1, csURL, _TRUNCATE);

		CString csUserPwd = pCameraInfo->csUser + _T(":") + pCameraInfo->csPwd;
		char *chUserPwd = new char[csUserPwd.GetLength() + 1];
		wcstombs_s(NULL, chUserPwd, csUserPwd.GetLength() + 1, csUserPwd, _TRUNCATE);

		CURLcode gCode = curl_global_init(CURL_GLOBAL_ALL);
		CURL* l_curl = curl_easy_init();

		if(l_curl == NULL)
			return false;

		CURLINFO	m_curlInfo;

		curl_easy_setopt(l_curl, CURLOPT_URL, chUrl);
		curl_easy_setopt(l_curl, CURLOPT_USERPWD, chUserPwd);
 
		CURLcode pCode = curl_easy_perform(l_curl); 
		curl_easy_cleanup(l_curl);
		
		delete chUrl;
		delete chUserPwd;
 
	return false;
}

// cgi command to open relay door - for ETROVISION
void CDlgAc::fnOpenDoor(int o_idx)
{
	bool	lb_found=false;
	CameraInfo* pCameraInfo;
	for (int i=0; i< m_vcCamInfo.size(); i++)
	{
		if ( _ttoi(m_vcCamInfo[i]->csCamIDX) == o_idx)
		{
			pCameraInfo = m_vcCamInfo[i];
			lb_found = true;
			break;
		}
	}

		if (!lb_found) return; 
		CString csURL = _T("http://") + pCameraInfo->csIP  + _T("/config/send_cmd.cgi?cmd=7e040104faff&");

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
 
}

void CDlgAc::fnResumeCam(int o_camidx)
{
		if (o_camidx<1) return;
	m_currGCamid =  o_camidx;
	
	for (int ci=0; ci < m_vpcam.size() ; ci++)
	{
				CEmapAccessCam* lpcam =  m_vpcam[ci];
				if (lpcam->m_idx == o_camidx)				
				{	
						lpcam->LoadBitmaps(IDB_CAM37, IDB_CAM37SEL,IDB_CAM37SEL,IDB_CAM37);
						return;
				}
	}
}

CString CDlgAc::fnGetBrand(int o_camidx)
{
	CString ls_ret=_T("");
	for (int i=0; i<m_pvtAc.size(); i++)
	{
		if (m_pvtAc[i].camidx == o_camidx)
			return m_pvtAc[i].csBrand.Trim().MakeUpper();
	}
	return ls_ret;
}

//m_pvtAc
void CDlgAc::fnTimertoVector(int oCamIdx, CString oBrand, int oTimer)
{
	for (int i=0; i<m_pvtAc.size(); i++)
	{
		if (m_pvtAc[i].camidx == oCamIdx && m_pvtAc[i].csBrand == oBrand)
			m_pvtAc[i]._timer = oTimer;
	}
}

// Force event recording if defined in ec.ini
void CDlgAc::fnForceRecording(void)
{
}

//Start Access Process
void CDlgAc::fnStartAccessProcess(int oCamIdx)
{
}

// Stop Access Process
void CDlgAc::fnStopAccessProcess(int oCamIdx)
{
}

LRESULT CDlgAc::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CBaseDialog::WindowProc(message, wParam, lParam);
}

void CDlgAc::fnSetVideoFlag(int oIndex, bool oFlag)
{
	if (m_vpEventVideo.size()>50) return;

	for (int i=0; i<m_vpEventVideo.size(); i++)
	{
		if (m_vpEventVideo[i]->m_camidx == oIndex)
			m_vpEventVideo[i]->mb_displayVideo = oFlag;
	}
}

void CDlgAc::fnInsertCamera(int oCamidx)
{
	CString	ls_sql;
	bool	lb_found = false;

	try{
				//prevent duplicate in ecac_camera
					CRecordset m_rs(pDB);
					ls_sql.Format( _T("SELECT camera_idx FROM ecac_camera WHERE  camera_idx = %d"), oCamidx);
			 
					m_rs.Open(CRecordset::dynamic, ls_sql);
					while(!m_rs.IsEOF() ) 
					{
						lb_found = true;
						return;
					}
					m_rs.Close();

 				ls_sql.Format(_T("INSERT INTO ecac_camera(active_,camera_idx, access_brand, access_model)  VALUES('Y', %d, '%s', '%s')"), oCamidx, ACDFTBRAND, ACDFTMODEL);							
				pDB->ExecuteSQL(ls_sql);
			}
 
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
		return ;
	}
	
}

void CDlgAc::fnInitLocalize(void)
{
	CString csLoc(_T(""));
	//csLoc.LoadStringW(CGS::g_hResStr, IDS_EMAPGROUP);
	LOADSTRING(csLoc, IDS_EMAPGROUP);
	((CStatic*)GetDlgItem(IDC_STGROUP))->SetWindowTextW(csLoc);

	//csLoc.LoadStringW(CGS::g_hResStr, IDS_EMAPFOLDER);
	LOADSTRING(csLoc,   IDS_EMAPFOLDER);
	((CStatic*)GetDlgItem(IDC_STGROUP2))->SetWindowTextW(csLoc);

	//csLoc.LoadStringW(CGS::g_hResStr, IDS_EMAPMAP);
		LOADSTRING(csLoc,  IDS_EMAPMAP);
	((CStatic*)GetDlgItem(IDC_STGROUP3))->SetWindowTextW(csLoc);

	//Cameras
	//csLoc.LoadStringW(CGS::g_hResStr, IDS_EMAPCAMERA);
	LOADSTRING(csLoc,  IDS_EMAPCAMERA);
	((CStatic*)GetDlgItem(IDC_STCAM))->SetWindowTextW( csLoc);

	//csLoc.LoadStringW(CGS::g_hResStr, IDS_ACLOG);
	LOADSTRING(csLoc,  IDS_ACLOG);
	((CStatic*)GetDlgItem(IDC_STAC))->SetWindowTextW( csLoc);

	//Detail Section
	LOADSTRING(csLoc, IDS_STGDETAIL);
	GetDlgItem( IDC_STGDETAIL  )->SetWindowTextW( csLoc);
	
	LOADSTRING(csLoc, IDS_STGROUPCAM);
	GetDlgItem(IDC_STGROUPCAM  )->SetWindowTextW( csLoc);

	LOADSTRING(csLoc, IDS_STDATETIME);
	GetDlgItem( IDC_STDATETIME  )->SetWindowTextW( csLoc);

	LOADSTRING(csLoc, IDS_STSTATUS);
	GetDlgItem(  IDC_STSTATUS )->SetWindowTextW( csLoc);

	LOADSTRING(csLoc, IDS_STCDN );
	GetDlgItem( IDC_STCDN )->SetWindowTextW( csLoc);

	LOADSTRING(csLoc, IDS_CHECKWARN  );
	GetDlgItem( 	IDC_CHECKWARN )->SetWindowTextW( csLoc);

}

bool CDlgAc::fnPegasusDecode(CString& oData, CString& oID, CString& oyy, CString& oMM, CString& odd, CString& ohh, CString& omm, CString& oss)
{
	if (oData.GetLength()<74) return false;
	
	if (oData.Mid(0,6) == _T("023030"))
	{	
		oID = oData.Mid(26, 16);
		oID = HexToAscii(oID);

		oyy = oData.Mid(50, 4);
		oyy = HexToAscii(oyy);

		oMM = oData.Mid(54, 4);
		oMM = HexToAscii(oMM);

		odd = oData.Mid(58, 4);
		odd = HexToAscii(odd);

		if ( oData.Mid(64, 2) !=_T("3a")) return false;

		ohh = oData.Mid(66, 4);
		ohh =HexToAscii(ohh) ;

		omm = oData.Mid(70, 4);
		omm =HexToAscii(omm) ;

		oss = oData.Mid(74, 4);
		oss =HexToAscii(oss) ;
	}
	else
		return false;

	return true;
}

void CDlgAc::OnNMRClickListac(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CMenu Menu;
	Menu.LoadMenu(IDR_MENU_ACPOP);

	CPoint Point;
	GetCursorPos(&Point);

	CMenu *pMenu = Menu.GetSubMenu(0);
	if (pMenu)
	pMenu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, Point.x,Point.y, this);
	*pResult = 0;
}


// check security
bool CDlgAc::fnSecurity(int o_idx)
{

		return true;
 		bool	lb_found=false;
		CameraInfo* pCameraInfo;

		//Get camera info
		for (int i=0; i< m_vcCamInfo.size(); i++)
		{
			if ( _ttoi(m_vcCamInfo[i]->csCamIDX) == o_idx)
			{
				pCameraInfo = m_vcCamInfo[i];
				lb_found = true;
				break;
			}
		}

		if (!lb_found) return false; 
		//command for check security
	   // CString csURL= _T("http://") + pCameraInfo->csIP  + _T("/config/send_cmd.cgi?cmd=7E0C01230000000501011B458611&");
		 CString csURL= _T("http://") + pCameraInfo->csIP  + _T("/config/send_cmd.cgi?cmd=7E040124daff&");

		char *chUrl = new char[csURL.GetLength() + 1];
		wcstombs_s(NULL, chUrl, csURL.GetLength() + 1, csURL, _TRUNCATE);

		CString csUserPwd = pCameraInfo->csUser + _T(":") + pCameraInfo->csPwd;
		char *chUserPwd = new char[csUserPwd.GetLength() + 1];
		wcstombs_s(NULL, chUserPwd, csUserPwd.GetLength() + 1, csUserPwd, _TRUNCATE);

		//CURLcode gCode = curl_global_init(CURL_GLOBAL_ALL);
		CURL* l_curl = curl_easy_init();

		if(l_curl == NULL)
			return false;

		CURLINFO	m_curlInfo;

		curl_easy_setopt(l_curl, CURLOPT_URL, chUrl);
		curl_easy_setopt(l_curl, CURLOPT_USERPWD, chUserPwd);
		curl_easy_setopt(l_curl, CURLOPT_WRITEFUNCTION, &CDlgAc::notifyWriter);
		//curl_easy_setopt(g_curl, CURLOPT_WRITEDATA, pCameraInfo);

		CURLcode pCode = curl_easy_perform(l_curl); 
		//curl_easy_cleanup(l_curl);
		
		delete chUrl;
		delete chUserPwd;

	return false;
}


bool CDlgAc::fnSetCurrentTime(int o_idx)
{
 		bool	lb_found=false;
		CameraInfo* pCameraInfo;

		for (int i=0; i< m_vcCamInfo.size(); i++)
		{
			if ( _ttoi(m_vcCamInfo[i]->csCamIDX) == o_idx)
			{
				pCameraInfo = m_vcCamInfo[i];
				lb_found = true;
				break;
			}
		}

		if (!lb_found) return false; 

 		//Get Current Time
		CTime nowTime = CTime::GetCurrentTime();
		int i_sec = nowTime.GetSecond();
		int i_min = nowTime.GetMinute();
		int i_hr = nowTime.GetHour();
		int i_wk = nowTime.GetDayOfWeek();
		int i_day = nowTime.GetDay();
		int i_mon = nowTime.GetMonth();
		int i_yr = nowTime.GetYear();

	  	CString ls_sec = fnToHex(i_sec);
		CString ls_min = fnToHex(i_min);
		CString ls_hr = fnToHex(i_hr);
		CString ls_wk = fnToHex(i_wk);
		CString ls_day = fnToHex(i_day);
		CString ls_mon = fnToHex(i_mon);

		CString ls_year;
		ls_year.Format(_T("%d"), i_yr);
		ls_year = ls_year.Mid(2,2);
		i_yr = _ttoi(ls_year);
		CString ls_yr = fnToHex(i_yr);

		int i_or = 255^01^35^i_sec^i_min^i_hr^i_wk^i_day^i_mon^i_yr;
		CString ls_or = fnToHex(i_or);

		int i_sum = 36+ i_sec+i_min+i_hr+i_wk+i_day+i_mon+i_yr+i_or;
		CString ls_sum =fnToHex(i_sum);

		 CString csURL1 = _T("http://") + pCameraInfo->csIP  + _T("/config/send_cmd.cgi?cmd=7E0B0123");
		CString csURL;
		csURL.Format(_T("%s%s%s%s%s%s%s%s%s%s&"), csURL1, ls_sec, ls_min, ls_hr, ls_wk, ls_day, ls_mon, ls_yr, ls_or, ls_sum);
	 
		char *chUrl = new char[csURL.GetLength() + 1];
		wcstombs_s(NULL, chUrl, csURL.GetLength() + 1, csURL, _TRUNCATE);

		CString csUserPwd = pCameraInfo->csUser + _T(":") + pCameraInfo->csPwd;
		char *chUserPwd = new char[csUserPwd.GetLength() + 1];
		wcstombs_s(NULL, chUserPwd, csUserPwd.GetLength() + 1, csUserPwd, _TRUNCATE);

		CURLcode gCode = curl_global_init(CURL_GLOBAL_ALL);
		CURL* l_curl = curl_easy_init();

		if(l_curl == NULL)
			return false;

		CURLINFO	m_curlInfo;

		curl_easy_setopt(l_curl, CURLOPT_URL, chUrl);
		curl_easy_setopt(l_curl, CURLOPT_USERPWD, chUserPwd);
 //		curl_easy_setopt(g_curl, CURLOPT_WRITEFUNCTION, &CDlgAc::notifyWriter);
//		curl_easy_setopt(g_curl, CURLOPT_WRITEDATA, pCameraInfo);

		CURLcode pCode = curl_easy_perform(l_curl); 
		curl_easy_cleanup(l_curl);
		
		delete chUrl;
		delete chUserPwd;

		return false;
}

CString CDlgAc::fnToHex(int oDec)
{
	CString ls_ret = _T("");
	char buffer [4];
	itoa(oDec, buffer, 16);
	

	if (buffer[1]==NULL)
		ls_ret.Format(_T("0%c"), buffer[0]);
	else
		ls_ret.Format(_T("%c%c"),buffer[0], buffer[1]);

	if (buffer[2]>0)
		ls_ret.Format(_T("%c%c"), buffer[1], buffer[2]);

	return ls_ret;
}

void CDlgAc::fnReSecurity(void)
{
		for (int i=0; i<m_pvtAc.size(); i++)
		{
			if (m_pvtAc[i].csecurity != 'Y') 
 					fnSecurity(m_pvtAc[i].camidx);	 
		}
}

bool CDlgAc::fnCheckAuthorize(int oCamidx)
{
	//Temp allow
	return true;	
	for (int i=0; i<m_pvtAc.size(); i++)
	{
		if (m_pvtAc[i].camidx == oCamidx) // get the right camera
		{
			if (m_pvtAc[i].csecurity == 'Y')
				return true;
			else
				return false;
		}
	}
	return false;
}

//Return true if time within work hour, return false if time located in holiday or off hours
bool CDlgAc::fnTimeWithin(CString oCamidx, CString oDate, CString oTime, CString oDept)
{
	//Check Date within holiday
	for (int i=0; i<m_vtHol.size(); i++)
	{
		if (m_vtHol[i].ctDate.Mid(0,4) == oDate.Mid(0,4) && m_vtHol[i].ctDate.Mid(5,2) == oDate.Mid(5,2) && m_vtHol[i].ctDate.Mid(8,2) == oDate.Mid(8,2))
			return false; //in holiday
	}

	for (int i=0; i<m_vtWork.size(); i++)
	{
		//if (m_vtWork[i].camera_idx == _ttoi(oCamidx) && oDept == m_vtWork[i].csGroupCode)
		if (  oDept == m_vtWork[i].csGroupCode)
		{
			int i_wkfhr = _ttoi(m_vtWork[i].csWorkFrom.Mid(0,2));
			int i_wkfmin = _ttoi(m_vtWork[i].csWorkFrom.Mid(3,2));
			int i_wkthr = _ttoi(m_vtWork[i].csWorkTo.Mid(0,2));
			int i_wktmin = _ttoi(m_vtWork[i].csWorkTo.Mid(3,2));

			int i_achr = _ttoi(oTime.Mid(0,2));
			int i_acmin = _ttoi(oTime.Mid(3,2));

			if ( i_wkfhr > i_achr  || i_wkthr <  i_achr) return false;
			
			if (i_wkfhr == i_achr) //same hour before work min
					if (i_wkfmin > i_acmin) return false;			

			if (i_wkthr == i_achr)  // same hour, later than min			
				if (i_acmin > i_wktmin ) return false;
			
				return true;
		}
	}
	return true;
}


bool CDlgAc::fnCheckPriv(int oCamidx, CString oDeptCode)
{
	//loop into m_vtPriv check for the privilege
	if (oDeptCode==_T("")) return false;
	for( int i=0; i<m_vtPriv.size(); i++)
	{
		if (m_vtPriv[i].camera_idx == oCamidx && m_vtPriv[i].csGroupCode==oDeptCode)
		{
			if (m_vtPriv[i].cspriv == _T("N"))
				return false;
			else
				return true;
		}
	}
		return false;
}


void CDlgAc::fnOpenSetting(bool oNormal)
{
		CDlgSettingAC lDlg;
		lDlg.mb_openNormal = oNormal;
		if (lDlg.DoModal()==IDOK)
		{
			if (lDlg.mb_camChanged)		//Original Codes
 				fnGroupChanged();
	
			if (lDlg.mb_holChanged)		//Rebuild holiday structure
				fnRefreshHolVec();

			if (lDlg.mb_priChanged)	//Refresh vector of work hour and privilege
				fnRefreshPriVec();
				
		}
}

//Refresh Holiday Vector
void CDlgAc::fnRefreshHolVec(void)
{
	CString csDate;
	CString csSQL("SELECT date_ FROM ecac_hholiday ");
	CRecordset	rsHol(pDB);
	rsHol.Open(CRecordset::forwardOnly, csSQL);
	
	STRHOL lHol;

	m_vtHol.clear();
	while (!rsHol.IsEOF()) { 

			rsHol.GetFieldValue(_T("date_"), csDate);
			lHol.ctDate = csDate;
 			m_vtHol.push_back(lHol);
			rsHol.MoveNext();			
	}
 
	//Build work hour vector
	rsHol.Close();
}

// Refresh vector for work hour and privilege
void CDlgAc::fnRefreshPriVec(void)
{
		CDBVariant	varCamidx;
		CString	ls_gcode, ls_workfrom, ls_workto, ls_sql;
		CRecordset	rs(pDB);

		ls_sql= _T("SELECT access_group_code, work_from, work_to FROM ecac_camera_group ");
		rs.Open(CRecordset::forwardOnly, ls_sql);

		STRWORK  lWork;
		m_vtWork.clear();

		while (!rs.IsEOF()) { 
				rs.GetFieldValue(_T("access_group_code"), ls_gcode  );
				rs.GetFieldValue(_T("work_from"), ls_workfrom  );
				rs.GetFieldValue(_T("work_to"), ls_workto  );

				//lWork.camera_idx = varCamidx.m_iVal;
				lWork.csGroupCode = ls_gcode + '\0';
				lWork.csWorkFrom = ls_workfrom + '\0';
				lWork.csWorkTo = ls_workto + '\0';
				m_vtWork.push_back(lWork);
				rs.MoveNext();			
		}
		rs.Close();

		//Privilege 
		CDBVariant	varidx;
		CString	ls_priv;
	
		ls_sql.Format( _T("SELECT camera_idx, access_group_code, privilege_ FROM ecac_privilege  ") );	
		rs.Open(CRecordset::forwardOnly, ls_sql);
		m_vtPriv.clear();

		while (!rs.IsEOF()) { 
				STRPRIV lstrpriv;
				rs.GetFieldValue(_T("camera_idx"), varidx,  SQL_C_SSHORT  );
				rs.GetFieldValue(_T("access_group_code"), ls_gcode );
				rs.GetFieldValue(_T("privilege_"), ls_priv  );
				
				lstrpriv.camera_idx = varidx.m_iVal;
				lstrpriv.csGroupCode = ls_gcode.Trim() + '\0';
				lstrpriv.cspriv = ls_priv.Trim() + '\0';
				m_vtPriv.push_back(lstrpriv);

				rs.MoveNext();
		 }
		rs.Close();
		return;
	
}

/*=========         Event Stuffs     ===============*/
UINT _stdcall CDlgAc::GetEventFromService(LPVOID lpParam)
{
	CDlgAc* pThis = (CDlgAc*) lpParam;

	pThis->m_EmapEventRec->vSetCallBack(&pThis->fnEmapCallBack, pThis);
	pThis->m_EmapEventRec->vStartReceive();
	
	return 0;

}

void CDlgAc::fnEmapCallBack(EventInfo *m_EventInfo, void *lpParam)
{
	CDlgAc* pThis = (CDlgAc*)lpParam;
	pThis->m_CS.Lock();

	CString osIP(_T("")), osEventType(_T("")), csEventStreamID(_T("")), osIndex(_T("")), csCurHMS(_T(""));
	osIP.Format(_T("%s"), m_EventInfo->tcIP);
	
	//Change Type from DWORD to String.
 	if (m_EventInfo->nEventType == EVENTTYPE::DIGITAL_INPUT)
				osEventType = _T("digital input");
	else
		return;
 
	//osEventType.Format(_T("%s %s"), m_EventInfo->tcEventType, m_EventInfo->tcStream);
	osIndex.Format(_T("%d"), m_EventInfo->nCamIdx);

	CTime timeTime = 0;
	timeTime = CTime::GetCurrentTime();
	csCurHMS.Format(_T("%02d:%02d:%02d"), timeTime.GetHour(), timeTime.GetMinute(), timeTime.GetSecond());

	if (m_EventReceiveFlag == false) return;
 	
	//Do rest of the stuff...display video timer...etc
	pThis->fnEventDisplay( osIndex, osIP, osEventType, csCurHMS	);	
	pThis->m_CS.Unlock();
}

//Event  received, check if camera in the chain, then save into ecgt_record and display PANIC int the list control
bool CDlgAc::fnEventDisplay(CString oIndex, CString osIP, CString osEventType, CString osTime)
{
		//Check if camera in the chain
		int	ll_camidx, ll_acid=0;
		bool	lb_found = false;
		ll_camidx = _ttoi(oIndex);

		//for (int i=0; i<m_vpEventVideo.size(); i++)
		//{
		//	if (m_vpEventVideo[i].camidx == ll_camidx)
		//	{
		//		ll_acid = m_vpEventVideo[i].acid;
		//		lb_found = true;
		//		break;
		//	}
		//}
		//if (!lb_found) return false;

		//Save into DB 
		CTime t = CTime::GetCurrentTime(); 	 
		CString ls_today;		
		CString ls_sql, ls_desc=_T("");
		ls_today.Format(_T("%02d/%02d/%02d"), t.GetMonth(), t.GetDay(), t.GetYear());

		//Get ac desc
		if (ll_acid>0)
		{
				CRecordset m_rs(pDB);

				ls_sql.Format(_T("SELECT ac_desc FROM ecac_ac WHERE acid = %d"), ll_acid);
				m_rs.Open(CRecordset::dynaset, ls_sql);
				if (m_rs.GetRecordCount() >0)
				{
					m_rs.MoveFirst();
					while(!m_rs.IsEOF()   ) 
					{ 	  
						m_rs.GetFieldValue( _T("ac_desc"), ls_desc); 
						break;
					}
				}
		}

		fnInsertDB(_T("PANIC"), ls_today, osTime, _T(""), 0, ll_camidx,_T(""), _T(""), ll_acid , ls_desc);
		fnInsertRowPanic(ls_today, osTime, oIndex, ll_acid, ls_desc );
		for (int i=0; i<m_vpEventVideo.size(); i++)
			{
				if (m_vpEventVideo[i]->m_camidx == ll_camidx)
				{
						m_vpEventVideo[i]->mb_displayVideo= true;
						break;
				}
		}
		return false;
}


bool CDlgAc::fnInsertRowPanic( CString oDate, CString oTime, CString oCamidx, int oParklot, CString oPark )
{															
			LVITEM lvi;
			lvi.mask = LVIF_IMAGE | LVIF_TEXT;
 			CString ls_status;
			TCHAR szItem[256];
			lvi.iItem = 0;
			lvi.iSubItem = 0;
		 	
			bool	lb_priv = false;
			bool	lb_within = false;

			lvi.pszText =szItem;
			lvi.iImage = 0;
			ls_status = _T("PANIC");
 
 			lvi.pszText = _T("");
			int nItem = m_listAc.InsertItem(&lvi);

			//Date
			lvi.iSubItem =1;	
			_stprintf(szItem, _T("%s"), oDate.Left(5));
			lvi.pszText =szItem;
			m_listAc.SetItem(&lvi);

			//Time
			lvi.iSubItem =2;	
			CString csTime;
			csTime.Format(_T("%s:%s"), oTime.Mid(0,2), oTime.Mid(2,2));
			lvi.pszText = (LPTSTR)(LPCTSTR)(oTime);
			m_listAc.SetItem(&lvi);

			//Status
			lvi.iSubItem =3; 
			lvi.pszText = (LPTSTR)(LPCTSTR)(ls_status);
			m_listAc.SetItem(&lvi);
			
			//Parking Lot Desc
	
			//lvi.iSubItem =4; 
			//lvi.pszText = (LPTSTR)(LPCTSTR)(oPark);
			//m_listAc.SetItem(&lvi);

			//Cameraidx
			lvi.iSubItem =6;  
			lvi.pszText = (LPTSTR)(LPCTSTR)(oCamidx);
			m_listAc.SetItem(&lvi);

			//Get Camera Group and camera name
			CString	 ls_camname, ls_groupdesc;
			fnGetcamgroupname(_ttoi(oCamidx), ls_camname, ls_groupdesc);

			//Cam Name
			lvi.iSubItem =8;  
			lvi.pszText = (LPTSTR)(LPCTSTR)(ls_camname);
			m_listAc.SetItem(&lvi);

			//Group Name
			lvi.iSubItem =9;  
			lvi.pszText = (LPTSTR)(LPCTSTR)(ls_groupdesc);
			m_listAc.SetItem(&lvi);

 			return false;
}

void CDlgAc::fnInsertDB(CString oStatus, CString oDate, CString oTime, CString oCardCode, int oCardid, int oCamIdx, CString oName, CString oPlate, int oParklotid, CString oPark)
{
	try{
 			CString	ls_sql, ls_dt;
			ls_dt.Format(_T("%s %s"),oDate, oTime);
		
			ls_sql.Format(_T("INSERT INTO ecac_record (record_daytime, record_type,  camera_idx ) VALUES ( CONVERT(datetime, '%s'), '%s', %d  )"), ls_dt, oStatus,  oCamIdx   );
	 		TRACE(ls_sql);
			pDB->ExecuteSQL(ls_sql);
	}
	catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
}

void CDlgAc::OnLvnItemchangedListac(NMHDR *pNMHDR, LRESULT *pResult)
{
			LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
 
			int ll_item = pNMLV->iItem;

			m_logitem = ll_item;
			TCHAR szBuffer[60];
			DWORD cchBuf(60);
			LVITEM lvi;
			lvi.iItem = ll_item;

			lvi.mask = LVIF_TEXT;
			lvi.pszText = szBuffer;
			lvi.cchTextMax = cchBuf;
			CString ls_data, ls_camgroup;

			for(int i=1; i<11; i++)
			{
				lvi.iSubItem = i;	
				m_listAc.GetItem(&lvi);			
				ls_data = szBuffer;
				
				if (i==1)		//Date
					GetDlgItem(IDC_EDITDATE)->SetWindowTextW(ls_data); 
				else if (i==2)		//Time
					GetDlgItem(IDC_EDITTIME)->SetWindowTextW(ls_data);
				else if (i==3)		//Status
				{
					GetDlgItem(IDC_EDITSTATUS)->SetWindowTextW(ls_data);
					//show, hide red circle based on status
					if (ls_data.Trim().MakeUpper()==_T("OK"))
						GetDlgItem(IDC_BUTRED)->ShowWindow(false);
					else
						GetDlgItem(IDC_BUTRED)->ShowWindow(true);
				}
				else if (i==4)		//Department
						GetDlgItem(IDC_EDITDEPT)->SetWindowTextW(ls_data);		
				else if (i==5)		//Name
						GetDlgItem(IDC_EDITNAME)->SetWindowTextW(ls_data);
				else if (i==6)		//Camera id										
						GetDlgItem(IDC_EDITCAMID)->SetWindowTextW(ls_data);									
				else if (i==7)		//card code
					GetDlgItem(IDC_EDITCARD)->SetWindowTextW(ls_data);
				else if (i==8)		//camera name
						GetDlgItem(IDC_EDITCAM)->SetWindowTextW(ls_data);		
				else if (i==9)		//group name
						GetDlgItem(IDC_EDITGROUP)->SetWindowTextW(ls_data);
				else if (i==10)		//record id
				{
					if (_ttoi(ls_data)>0)
						m_currecordid =  _ttoi(ls_data);
					GetDlgItem(IDC_EDITRECORDID)->SetWindowTextW(ls_data);
				}
			}
  
			*pResult = 0;
}


void CDlgAc::OnBnClickedButopen()
{
 	CString ls_idx;
	GetDlgItem(IDC_EDITCAMID)->GetWindowTextW(ls_idx);
	if (_ttoi(ls_idx)>0 && _ttoi(ls_idx)<1000)
		fnOpenDoor(_ttoi(ls_idx));
}

// Set STATUS OK and update ecac_record
void CDlgAc::OnBnClickedButred()
{
		LVITEM lvItem;
		lvItem.mask = LVIF_IMAGE ;
		lvItem.iItem = m_logitem;
		lvItem.pszText = _T("");
		lvItem.iSubItem = 0;
		lvItem.iImage = 2; //blank
		int ll_ret = m_listAc.SetItem(&lvItem);

		//Get Original status
		CString ls_orgStatus;		
		GetDlgItem(IDC_EDITSTATUS)->GetWindowTextW(ls_orgStatus);

		lvItem.mask = LVIF_TEXT ;
		lvItem.iItem = m_logitem;
		lvItem.pszText = _T("OK");
		lvItem.iSubItem = 3;
		ll_ret = m_listAc.SetItem(&lvItem);
		GetDlgItem(IDC_EDITSTATUS)->SetWindowTextW(_T("OK"));

		//update record
		if (m_currecordid>0)
		{
				CString	ls_sql;
				ls_sql.Format(_T("UPDATE ecac_record SET record_type = 'OK', org_status='%s', modify='Y' WHERE access_recordid = %d "), ls_orgStatus, m_currecordid ); 
				TRACE(ls_sql);
				pDB->ExecuteSQL(ls_sql);
		}

		GetDlgItem(IDC_BUTRED)->ShowWindow(false);
		
}

void CDlgAc::OnBnClickedCheckwarn()
{
	int ll_check = m_ckAlarm.GetCheck();
	if (ll_check==1)
		mb_filter = true;
	else
		mb_filter = false;
}

// From camera index to get camera name and group name from ec_camera_group_camera table
bool CDlgAc::fnGetcamgroupname(int oCamidx, CString& oCamname, CString& oGroupname)
{

				CRecordset m_rscam(pDB);
				CString ls_select;
				ls_select.Format(_T("SELECT ec_camera.cameraname, ec_camera_group.camera_group_desc FROM ec_camera_group_camera LEFT OUTER JOIN ec_camera ON  ec_camera.camera_idx = ec_camera_group_camera.camera_idx  LEFT OUTER JOIN ec_camera_group ON ec_camera_group.cameragroupid = ec_camera_group_camera.cameragroupid WHERE ec_camera_group_camera.camera_idx = %d "), oCamidx);
				TRACE(ls_select);
				m_rscam.Open(CRecordset::dynaset, ls_select);
				
				while(!m_rscam.IsEOF()   ) 
				{ 	  
					m_rscam.GetFieldValue( _T("cameraname"), oCamname); 
					m_rscam.GetFieldValue( _T("camera_group_desc"), oGroupname); 
					break;
				}

				m_rscam.Close();
	return false;
}

void CDlgAc::OnBnClickedButswitch()
{
	CString	ls_curGroup, ls_toGroup;
	m_comboGroup.GetWindowTextW(ls_curGroup);
	GetDlgItem(IDC_EDITGROUP)->GetWindowTextW(ls_toGroup);
	
	if (ls_curGroup.Trim()== ls_toGroup.Trim())
		return;

	for (int i=0; i<m_vtGroup.size(); i++)
	{		
		if (m_vtGroup[i].Desc == ls_toGroup)
		{
			m_comboGroup.SetCurSel(i);
			fnCloseVideo();
			fnGroupChanged();
		}

	}


}

void CDlgAc::fnInitChain(void)
{
	LVITEM* litem = new LVITEM;
 
	CBitmap bIcon, bred;
 
		m_chainimg.Create(96, 96, ILC_COLOR16, 3, 1);
		bred.LoadBitmapW(IDB_AC96);
		m_chainimg.Add(&bred, RGB(0, 0, 0));
		m_listchain.SetImageList(&m_chainimg, LVSIL_NORMAL  );
}

void CDlgAc::fnChainChanged(int oChainid)
{
	m_listchain.DeleteAllItems();
	m_vtGate.clear();
	m_vtGateCam.clear();

	CString ls_sql, ls_sql1, ls_sql2, ls_desc;

	//ls_sql1 = _T("SELECT ecac_chainac.chainid, ecac_chainac.cameragroupid, ecac_ac.acid,  ec_camera_group_camera.camera_idx , ecac_ac.ac_desc ");
	//ls_sql2 = _T("FROM ecac_chainac, ecac_ac, ec_camera_group_camera WHERE ecac_chainac.acid = ecac_ac.acid AND ec_camera_group_camera.cameragroupid = ecac_chainac.cameragroupid AND ");
	//ls_sql.Format(_T("%s%s ecac_chainac.chainid=%d ORDER BY ecac_ac.acid"), ls_sql1, ls_sql2, oChainid);

	ls_sql1 = _T("SELECT ecac_chainac.chainid, ecac_chainac.cameragroupid, ecac_ac.acid,  ec_camera_group_camera.camera_idx , ecac_ac.ac_desc ");
	ls_sql2 = _T("FROM ecac_chainac INNER JOIN ecac_ac ON  ecac_chainac.acid = ecac_ac.acid LEFT OUTER JOIN ec_camera_group_camera ON ec_camera_group_camera.cameragroupid = ecac_chainac.cameragroupid WHERE ");
	ls_sql.Format(_T("%s%s ecac_chainac.chainid=%d ORDER BY ecac_ac.acid"), ls_sql1, ls_sql2, oChainid);


	//ls_sql1 = _T("SELECT ecac_chainac.chainid, ecac_chainac.cameragroupid, ecac_ac.acid, ecac_ac.ac_desc   ");
	//ls_sql2 = _T("FROM ecac_chainac LEFT OUTER JOIN ecac_ac ON  ecac_ac.acid = ecac_chainac.acid  WHERE ");
	//ls_sql.Format(_T("%s%s ecac_chainac.chainid=%d ORDER BY ecac_ac.acid"), ls_sql1, ls_sql2, oChainid);
	CRecordset	rsChain(pDB);
	rsChain.Open(CRecordset::forwardOnly, ls_sql);
	
	CDBVariant  varID, varGID, varChainID, varCamidx;
	STRCHAIN	lChain;
	int ll_cnt=0, ll_preacid=0 ;
	STRGATE	lGate;

	while (!rsChain.IsEOF()) { 

			rsChain.GetFieldValue( _T("chainid"), varChainID,  SQL_C_SSHORT);	
			rsChain.GetFieldValue( _T("cameragroupid"), varGID);	
			rsChain.GetFieldValue( _T("acid"), varID,  SQL_C_SSHORT);	
			rsChain.GetFieldValue( _T("camera_idx"), varCamidx,  SQL_C_SSHORT);	
			rsChain.GetFieldValue(_T("ac_desc"), ls_desc);
			//rsChain.GetFieldValue(_T("camera_idx"), varCamidx,  SQL_C_SSHORT);

			lGate.camgroupid = varGID.m_iVal; 
			lGate.acid = varID.m_iVal;
			lGate.chainid = varChainID.m_iVal;
			lGate.camidx = varCamidx.m_iVal;
			lGate.cMode = 'D';  //Mark delete as default

			if (ll_preacid != varID.m_iVal)
			{
					int nItem = m_listchain.InsertItem( ll_cnt, ls_desc, 0 );
					m_vtGateCam.push_back(lGate);
			}			

			ll_cnt++;
			m_vtGate.push_back(lGate);

			ll_preacid = varID.m_iVal;
			rsChain.MoveNext();
	}//while loop
	
	//Build Chain Camera		
	fnGateCam();
	fnCleanupThread();
	fnStartAccessService();

}

void CDlgAc::fnGateCam(void)
{
		int ll_groupid;
		CString ls_sql;		
		CDBVariant  varCamIdx;

		//Mark camera in ecgt_camera as Active
		ls_sql = (_T("SELECT ecac_camera.camera_idx  FROM ecac_camera WHERE ac_control='Y' "));
		CRecordset	rsGroup(pDB);
		rsGroup.Open(CRecordset::forwardOnly, ls_sql);
								
		while (!rsGroup.IsEOF()) { 

				rsGroup.GetFieldValue( _T("camera_idx"), varCamIdx, SQL_C_SSHORT);	
				for (int i=0; i<m_vtGate.size(); i++)
				{
					if (m_vtGate[i].camidx == varCamIdx.m_iVal)
					{
						m_vtGate[i].cMode = 'A';
						break;
					}
				}
				rsGroup.MoveNext();
		}
}


void CDlgAc::OnLvnItemchangedListchain(NMHDR *pNMHDR, LRESULT *pResult)
{
 
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

		if (m_preitem == pNMLV->iItem) return;
		m_preitem = pNMLV->iItem;
	 
		if (m_vtGate.size()<=0 || m_preitem<0) return;

 		m_groupid = m_vtGateCam[m_preitem].camgroupid;
		fnGroupChanged();
		*pResult = 0;
}


int CDlgAc::fnGetMinChainid(void)
{
		CDBVariant	varid;	
		CRecordset m_rs(pDB);
		CString	ls_sql;
		
		ls_sql =_T("SELECT chainid FROM ecac_chain ORDER BY chainid ");
		m_rs.Open(CRecordset::dynaset, ls_sql);
		
		while (!m_rs.IsEOF()) { 		 
				m_rs.GetFieldValue(_T("chainid"), varid, SQL_C_SSHORT);
				return varid.m_iVal;			
		}
	return -1;
}

void CDlgAc::fnCntconnect(CString oCamidx)
{
	for (int i=0; i<m_pvtAc.size(); i++)
	{
		if (m_pvtAc[i].camidx == _ttoi(oCamidx))
		{
			if (m_pvtAc[i]._timer >0)
				KillTimer(m_pvtAc[i]._timer);
		}
	}
}

//// check if the camera attached Access Controller
//bool CDlgAc::fnIscontroller(int oCamidx)
//{
//	bool lb_ret = false;
//
//	for (int i=0; i<m_vtGate.size(); i++)
//	{
//		if (m_vtGate[i].camgroupid == m_groupid)
//		{
//			if (m_vtGate[i].camidx == oCamidx && m_vtGate[i].cMode=='A')
//				return true;		
//		}
//	}
//	return lb_ret;
//}

void CDlgAc::fnCleanupThread(void)
{
	curl_global_cleanup();
	for(int i=0; i<m_pvtAc.size(); i++)
	{
		if (m_pvtAc[i]._timer>0)
				KillTimer(m_pvtAc[i]._timer);
	}
}
bool CDlgAc::fnCheckiscontrol(CString oCamidx)
{
		for(int i=0; i<m_vtGate.size(); i++)
		{

			if (m_vtGate[i].camidx == _ttoi(oCamidx))
			{
				return true;
			}
		}
		return false;
}

void CDlgAc::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);
}

//Check if the video display door
bool CDlgAc::fnVideoDoor(int o_camidx)
{
		CString ls_sql, ls_control;
		ls_sql.Format(_T("SELECT ac_control FROM ecac_camera WHERE camera_idx=%d"), o_camidx);
 
			try{
			TRACE(ls_sql);
			CRecordset m_rs(pDB);	 
			m_rs.Open(CRecordset::dynamic, ls_sql);
			while(!m_rs.IsEOF() ) 
			{
					m_rs.GetFieldValue(_T("ac_control"), ls_control);
					if (ls_control==_T("Y")) 
							return true;
					else
						return false;
					break;
			}
			}
		catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
		return false;

}
