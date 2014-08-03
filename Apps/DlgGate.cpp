// DlgGate.cpp : implementation file
//

#include "stdafx.h"
#include "DlgGate.h"
#include "MainFrm.h"
#include "BaseRadioButton.h"
#include "resource.h"
#include "DlgAc.h"
#include "DlgEmap.h"
#include "IntraEvent.h"

#define	CAMBUTID	233
#define	CHAINW	150
static vector<CEmapAccessCam*> m_vpcam; 
//static HANDLE hThreadAccess;
//static CString HexToAscii(CString s);
//static void vParseSegment(TCHAR* tszBuffer, map <CString, eVERSETS>::iterator pos);
//static CURL* g_curl;
////static CString DLTBRAND
//int	m_securecnt=0;
// CDlgGate dialog
#define GATETIME 1000
#define BMPKEY 0
#define BMPRED 1
#define BMPKEYRED 2
#define BMPBLANK 3

static bool m_EventReceiveFlag;
const UINT ID_TIMER_GT0 = 0x1100;
const UINT ID_TIMER_GT1 = 0x1101;
const UINT ID_TIMER_GT2 = 0x1102;
const UINT ID_TIMER_GT3 = 0x1103;
const UINT ID_TIMER_GT4 = 0x1104;
const UINT ID_TIMER_GT5 = 0x1105;
const UINT ID_TIMER_GT6 = 0x1106;
const UINT ID_TIMER_GT7 = 0x1107;
const UINT ID_TIMER_GT8 = 0x1108;
const UINT ID_TIMER_GT9 = 0x1109;
const UINT ID_TIMER_GT10 = 0x1110;
const UINT ID_TIMER_GT11 = 0x1111;
const UINT ID_TIMER_GT12 = 0x1112;
const UINT ID_TIMER_GT13 = 0x1113;
const UINT ID_TIMER_GT14 = 0x1114;
const UINT ID_TIMER_GT15 = 0x1115;
const UINT ID_TIMER_GT16 = 0x1116;
const UINT ID_TIMER_GT17 = 0x1117;
const UINT ID_TIMER_GT18 = 0x1118;
const UINT ID_TIMER_GT19 = 0x1119;
const UINT ID_TIMER_GT20 = 0x1120;

const UINT ID_TIMER_SECUREP = 0x1000;

IMPLEMENT_DYNAMIC(CDlgGate, CBaseDialog)

static void vParseSegment(TCHAR* tszBuffer, map <CString, eVERSETS>::iterator pos);

CDlgGate::CDlgGate(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgGate::IDD, pParent)
{

}

CDlgGate::~CDlgGate()
{
	fnFreeMemory();
}

void CDlgGate::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTSEARCH, m_butsch);
	DDX_Control(pDX, IDC_BUTSET, m_butset);
	DDX_Control(pDX, IDC_BUTREP, m_butrep);
	DDX_Control(pDX, IDC_COMBOGROUP, m_comboGroup);
	DDX_Control(pDX, IDC_EDITFOLDER, m_editFolder);
	DDX_Control(pDX, IDC_EDITMAP, m_editMap);
	DDX_Control(pDX, IDC_MAP, m_map);
	DDX_Control(pDX, IDC_GLISTCAM, m_listCam);
	//DDX_Control(pDX, IDC_LISTGATELOG, m_listGate);
	//DDX_Control(pDX, IDC_LISTGATE, m_listGate);

	DDX_Control(pDX, IDC_LISTCHAIN, m_listChain);
	DDX_Control(pDX, IDC_STAC, m_staclog);
	DDX_Control(pDX, IDC_STGATEINFO, m_stGateInfo);
	DDX_Control(pDX, IDC_STSTATUS, m_stStatus);
	DDX_Control(pDX, IDC_EDITSTATUS, m_editStatus);
	DDX_Control(pDX, IDC_STGBDETAIL, m_gbDetail);
	DDX_Control(pDX, IDC_EDITTIME, m_editTime);
	DDX_Control(pDX, IDC_EDITDATE, m_editDate);
	DDX_Control(pDX, IDC_BUTOPENGATE, m_butOpengate);
	DDX_Control(pDX, IDC_LGATE, m_listgate);
	//	DDX_Control(pDX, IDC_BUTSTATUS, m_butstatus);
	DDX_Control(pDX, IDC_BUTSTATUSR, m_butstatusr);
	DDX_Control(pDX, IDC_BUTSTATUSG, m_butstatusg);
	DDX_Control(pDX, IDC_BUTGOTO, m_butGoto);
	DDX_Control(pDX, IDC_CKFILTER, m_ckAlarm);
}


BEGIN_MESSAGE_MAP(CDlgGate, CBaseDialog)
	ON_BN_CLICKED(IDC_BUTBROWSE, &CDlgGate::OnBnClickedButbrowse)
	ON_BN_CLICKED(IDC_BUTMAP, &CDlgGate::OnBnClickedButmap)
	ON_CBN_SELCHANGE(IDC_COMBOGROUP, &CDlgGate::OnCbnSelchangeCombogroup)
//	ON_NOTIFY(LVN_BEGINRDRAG, IDC_LISTCAM, &CDlgGate::OnLvnBeginrdragListcam)
	ON_WM_MOUSEMOVE()
//	ON_NOTIFY(LVN_BEGINRDRAG, IDC_GLISTCAM, &CDlgGate::OnLvnBeginrdragGlistcam)
//	ON_NOTIFY(LVN_ENDSCROLL, IDC_LISTCHAIN, &CDlgGate::OnLvnEndScrollListchain)
	ON_BN_CLICKED(IDB_CLOSE, &CDlgGate::OnBnClickedClose)

	ON_BN_DOUBLECLICKED(IDB_CAM37SEL,  OnBnDoubleClicked)

	//ON_BN_CLICKED(IDB_CLOSE, &CDlgGate::OnBnClickedClose)

	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCHAIN, &CDlgGate::OnLvnItemchangedListchain)
	ON_NOTIFY(NM_DBLCLK, IDC_GLISTCAM, &CDlgGate::OnNMDblclkGlistcam)
//	ON_NOTIFY(HDN_BEGINDRAG, 0, &CDlgGate::OnHdnBegindragGlistcam)
//	ON_NOTIFY(LVN_BEGINRDRAG, IDC_GLISTCAM, &CDlgGate::OnLvnBeginrdragGlistcam)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_GLISTCAM, &CDlgGate::OnLvnBegindragGlistcam)
	ON_WM_LBUTTONUP()
	ON_WM_CLOSE()
	ON_WM_TIMER()
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTAC, &CDlgGate::OnLvnItemchangedListac)
	ON_BN_CLICKED(IDC_BUTSET, &CDlgGate::OnBnClickedButset)
//	ON_EN_CHANGE(IDC_EDITDATE, &CDlgGate::OnEnChangeEditdate)
ON_BN_CLICKED(IDC_BUTOPENGATE, &CDlgGate::OnBnClickedButopengate)
ON_BN_CLICKED(IDC_BUTSEARCH, &CDlgGate::OnBnClickedButsearch)
ON_BN_CLICKED(IDC_BUTREP, &CDlgGate::OnBnClickedButrep)
 
//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTGATELOG, &CDlgGate::OnLvnItemchangedListgatelog)
//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTGATE, &CDlgGate::OnLvnItemchangedListgatelog)
//ON_NOTIFY(NM_CLICK, IDC_LISTGATE, &CDlgGate::OnNMClickListgate)
ON_NOTIFY(LVN_ITEMCHANGED, IDC_LGATE, &CDlgGate::OnLvnItemchangedLgate)
 
ON_BN_CLICKED(IDC_BUTSTATUSR, &CDlgGate::OnBnClickedButstatusr)
ON_BN_CLICKED(IDC_CKFILTER, &CDlgGate::OnBnClickedCkfilter)
ON_BN_CLICKED(IDC_BUTGOTO, &CDlgGate::OnBnClickedButgoto)
END_MESSAGE_MAP()


// CDlgGate message handlers

void CDlgGate::OnBnClickedButbrowse()
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

		ls_sql.Format(_T("DELETE FROM ec_storage WHERE storage_type='GATE' ; INSERT INTO ec_storage (storage_type, store_location) VALUES ('GATE', '%s') "), pszPath.Trim() ); 
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

void CDlgGate::OnBnClickedButmap()
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

				//Check if chain is hide
				if (m_listChain.IsWindowVisible())
				{
					CRect lrect;
					m_map.GetClientRect(&lrect);
					//m_map.MoveWindow(m_rect.right - LPANELW + 10, TOPY, lrect.right, lrect.bottom, true);
					//m_map.MoveWindow(0, TOPY, mi_orgwidth,  mi_orgheight ,true);
					//m_map.MoveWindow(m_rect.right - LPANELW + 10, TOPY,  mi_orgwidth,  mi_orgheight, true);
					
				}

				m_editFolder.GetWindowTextW(ls_tfolder);
				ls_sfile.Format(_T("%s\\%s"),ls_tfolder,ls_name);		
				bool ls_ret = CopyFile(ls_path,ls_sfile, false); 
				//mc_fileMode = 'C';
				ib_dirty = true;

		try{
				CString	ls_sql;				
				ls_sql.Format(_T("UPDATE ecgt_chainparklot SET gatemap_file = '%s' WHERE cameragroupid = %d AND chainid>0 and parklotid>0 "), ls_name.Trim(), m_groupid ); 
				TRACE(ls_sql);
				pDB->ExecuteSQL(ls_sql);
				
				//Update m_ptGroup
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
		}	//		if(fOpenDlg.DoModal()==IDOK)
}


BOOL CDlgGate::OnInitDialog()
{
	CBaseDialog::OnInitDialog();
	m_pbk = CNVRBk::GetBkObject();
	m_ini.setINIFullFileName(_T("\\ec.ini"));
	fnInitControl();
	fnInitLocalize();
	fnBuildCamGroup();

	fnInitFolder();
	fnBuildList();		//Build Camera List header

	fnInitGroup();
	fnInitChain();

	fnLoadCamInfo(); //Load all the cameras info within chain / Group		

	//Get Chainid
	int ll_minChainid = fnGetMinChainid();
	if (ll_minChainid>0)
		fnChainChanged(ll_minChainid);
	fnGroupChanged();
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CDlgGate::fnInitControl(void)
{			
			CRect llrect;
			DWORD dwStyle;
			int ll_posx = 0, ll_posy  = 76, ll_gap = 10, ll_iconwh = 64;
			int	ll_gapx = 80;
			int	ll_topgap = 8, ll_stposx;
			
			nScreenW = ::GetSystemMetrics(SM_CXSCREEN);
			nScreenH = ::GetSystemMetrics(SM_CYSCREEN);

			mb_filter = false;		//Default as NOT filter
			CMainFrame* lMainFrame;
			lMainFrame = (CMainFrame*)AfxGetMainWnd();
			pDB = &( lMainFrame->m_DB);

			m_bDragging = false;
			int ll_vx=150;
			GetDlgItem(IDC_BUTCLEAR)->ShowWindow(SW_HIDE);
			m_LsplitV.Create(WS_CHILD | WS_VISIBLE, CRect(ll_vx , ll_posy, ll_vx  , m_rect.bottom), this, IDC_LSPLITV);
			//m_LsplitV.Create(WS_CHILD | WS_VISIBLE, CRect(m_rect.right - LPANELW , ll_posy, m_rect.right - LPANELW  , m_rect.bottom), this, IDC_LSPLITV);
			m_LsplitV.SetRange(ll_vx, ll_vx, -1);
		 
			m_splitV.Create(WS_CHILD | WS_VISIBLE, CRect(m_rect.right - GPANELW , ll_posy, m_rect.right - GPANELW  , m_rect.bottom), this, IDC_SPLITV);
			m_splitV.SetRange(200, 200, -1);

			m_splitTopH.Create(WS_CHILD | WS_VISIBLE, CRect( ll_posx  , ll_posy  , m_rect.right  , ll_posy ), this, IDC_SPLITTOPH);
			m_splitTopH.SetRange( ll_posy,  ll_posy, 1);

			//Buttons
			ll_posx += ll_gap ;
			m_butsch.LoadBitmaps(IDB_SEARCH_C, IDB_SEARCHSEL_C, IDB_SEARCH_C, IDB_SEARCH_C);
			m_butsch.MoveWindow(ll_posx , 4, ll_iconwh, ll_iconwh, true);

			ll_posx += ll_gap +ll_iconwh ;
			m_butrep.LoadBitmaps(IDB_REPORT_C, IDB_REPORTSEL_C, IDB_REPORT_C, IDB_REPORT_C);
			m_butrep.MoveWindow(ll_posx , 4, ll_iconwh, ll_iconwh, true);

			ll_posx += ll_gap +ll_iconwh ;
			m_butset.LoadBitmaps(IDB_SETUP_C, IDB_SETUPSEL_C, IDB_SETUP_C, IDB_SETUP_C);
			m_butset.MoveWindow( ll_posx , 4, ll_iconwh, ll_iconwh, true);
			ll_posx = m_rect.right - GPANELW + 20;

			//Map
			m_maxMapW =  m_rect.right - GPANELW - CHAINW - 10	;
			m_maxMapH = m_rect.bottom -  ll_posy;
			//m_map.MoveWindow(m_rect.right - LPANELW + ll_topgap, TOPY, m_maxMapW, m_maxMapH - TOPY, true);
			//m_map.MoveWindow(ll_vx + 5, TOPY, m_maxMapW, m_maxMapH - TOPY, true);

			//Control Panel
			ll_posy = TOPY + ll_topgap;
			ll_posx += 60;
			ll_stposx = ll_posx - 60;

			//Radio Button	Map View & live view - hide now
			((CButton*)GetDlgItem(IDC_RADIOMAP)) ->ShowWindow(false);
			((CButton*)GetDlgItem(IDC_RADIOLIVE)) ->ShowWindow(false);
			mc_view = 'M';		//Map
			
			//Combobox - Chain Select
			m_comboGroup.GetClientRect(llrect);
			m_comboGroup.ShowWindow(true);
			m_comboGroup.MoveWindow(5, TOPY + 20, 140, llrect.Height(),  true);

			(CButton*)GetDlgItem(IDC_CHECKREC)->ShowWindow(false);
			(CButton*)GetDlgItem(IDC_BUTPLAYBK)->ShowWindow(false);

			//============ Right hand side Panel ===============
			//Folder
			//ll_posy += 25;
			((CStatic*)GetDlgItem(IDC_STGROUP2)) ->MoveWindow(ll_stposx, ll_posy, 60, 15, true);
			m_editFolder.MoveWindow(ll_posx - 20, ll_posy, 200, llrect.Height());

			m_butBrowse.AutoLoad( IDC_BUTBROWSE, this);
			//m_butBrowse.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL);
			m_butBrowse.MoveWindow(ll_posx + 185, ll_posy, 24,24);
			
			//Create Close Chain Button
			btncloseChain = new CButtonST;
			CRect rc;
			GetClientRect(rc);
			int nHoveOver = (int)BTNST_AUTO_GRAY;
			dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;
			COLORREF clBG = MASKCOLOR;
			btncloseChain->Create(NULL, dwStyle, CRect(rc.Width()-17, 1, rc.Width()-1, 17), this, IDB_CLOSE);
			btncloseChain->SetBitmaps(IDB_CLOSE, clBG ,nHoveOver);
			btncloseChain->MoveWindow(CHAINW-20,TOPY, 16, 16, true);
			
			//ListChain
			ll_posy += 30;
			m_listChain.MoveWindow(0, TOPY + 45, CHAINW, m_rect.bottom -TOPY - 80 );
			m_listChain.ShowWindow(true);
			m_listChain.ShowScrollBar(SB_VERT, TRUE); 

			//Map button
			((CStatic*)GetDlgItem(IDC_STGROUP3)) ->MoveWindow(ll_stposx, ll_posy, 60, 15, true);
			m_editMap.MoveWindow(ll_posx - 20, ll_posy, 200, llrect.Height());	
			m_butMap.AutoLoad(IDC_BUTMAP, this);
			m_butMap.MoveWindow(ll_posx + 185, ll_posy, 24,24);
		 
			dwStyle =WS_CHILD | WS_VISIBLE | BS_OWNERDRAW; 
		 
			//Camera
			ll_posy += 30;
			((CStatic*)GetDlgItem(IDC_STCAM)) ->MoveWindow(ll_stposx, ll_posy, 60, 15, true);
			
			//camera List
			ll_posy += 20;
			m_listCam.MoveWindow( ll_posx - 70 , ll_posy, m_rect.right - ll_posx +58, 205);

			ll_posy += 210;  
			m_splitH.Create(WS_CHILD | WS_VISIBLE, CRect( m_rect.right - GPANELW + 5 ,  ll_posy  , m_rect.right  , ll_posy), this, IDC_SPLITH);
			m_splitH.SetRange(200, 200, -1);

			ll_posy += 10;
			((CStatic*)GetDlgItem(IDC_STAC)) ->MoveWindow(ll_stposx, ll_posy, 150, 15, true);
			m_ckAlarm.MoveWindow(ll_stposx + 150, ll_posy, 150, 15, true);
			ll_posy +=20;

 			m_listgate.MoveWindow( ll_posx -70, ll_posy, m_rect.right - ll_posx +58, m_rect.bottom - ll_posy - 30,  true);
			
			m_maph = nScreenH *3/4;
			if (ll_posy - TOPY < nScreenH *3/4)
				m_maph = ll_posy - TOPY;
				
			m_map.MoveWindow(ll_vx + 5, TOPY, m_maxMapW, m_maph, true);

			//================Details Section==========
			int	ll_editgap = 150, ll_textw = 110, ll_texth=20, ll_editw=120, ll_edith=20; 			
			ll_posy +=270;
			CRect  lrectst, lrect;			
			m_botHPos = ll_posy;			

			CWnd* pParent = m_map.GetParent();		
			if (::IsWindow(pParent->m_hWnd))
			{	
				m_map.GetWindowRect(m_maprcWnd);	
				pParent->ScreenToClient(m_maprcWnd); 
			}

			//ll_posy = TOPY + m_maph+35;
			ll_posy =  nScreenH * 5/7 + 30;

			m_splitBotH.Create(WS_CHILD | WS_VISIBLE, CRect( ll_vx + 5  ,  ll_posy  , m_rect.right - GPANELW ,  ll_posy  ), this, IDC_SPLITBOTH);
			m_splitBotH.SetRange( ll_posy-100, ll_posy+100);
		
			//Groupbox
			int	ll_detailx = ll_vx + 10 ;   //350
			//((CStatic*)GetDlgItem(IDC_STGBDETAIL))->MoveWindow( m_rect.right - LPANELW + 15, ll_posy + 15,  LPANELW  - GPANELW - 50, m_rect.bottom - ll_posy - 50, 1);
			((CStatic*)GetDlgItem(IDC_STGBDETAIL))->MoveWindow( ll_detailx, ll_posy + 15,  nScreenW*82/144, nScreenH*1/5, 1);

			//1st line - Gate info
			int	ll_curx;
			ll_posx = ll_detailx + 20;
			ll_posy +=40;
			m_stGateInfo.MoveWindow(ll_posx - 10, ll_posy, ll_textw, ll_texth);
			ll_curx = ll_posx + ll_editgap;
			((CStatic*) GetDlgItem(IDC_EDITPARK))->MoveWindow( ll_curx, ll_posy,  ll_editw, ll_edith);
			ll_curx += ll_editw + 3;
			((CStatic*) GetDlgItem(IDC_EDITCAM))->MoveWindow(ll_curx, ll_posy, ll_editw, ll_edith);
			ll_curx += ll_editw + 3;
			m_butGoto.LoadBitmaps(IDB_GOTO, IDB_GOTO, IDB_GOTO, IDB_GOTO);
			m_butGoto.MoveWindow(ll_curx, ll_posy, 24,24);

			//2nd line
			ll_posy +=28;
			((CStatic*) GetDlgItem(IDC_STDATETIME))->MoveWindow(ll_posx - 10, ll_posy, ll_textw, ll_texth);
			m_editDate.MoveWindow(ll_posx + ll_editgap, ll_posy,  ll_editw, ll_edith);
			m_editTime.MoveWindow(ll_posx + ll_editgap + ll_editw+ 3 , ll_posy,  ll_editw, ll_edith);			

			//3rd line - Status
			ll_posy +=28;
			m_stStatus.GetClientRect(&lrectst);
			m_stStatus.MoveWindow(ll_posx - 10, ll_posy,  ll_textw, ll_texth);			
					
			m_butstatusr.LoadBitmaps(IDB_REDCIRCLE, IDB_REDCIRCLE, IDB_REDCIRCLE, IDB_REDCIRCLE);
			m_butstatusr.MoveWindow(ll_posx + ll_editgap - 26, ll_posy, 16, 16, true);
			m_butstatusr.ShowWindow(false);

			m_butstatusg.LoadBitmaps(IDB_YELLOWCIRCLE, IDB_YELLOWCIRCLE, IDB_YELLOWCIRCLE, IDB_YELLOWCIRCLE);
			m_butstatusg.MoveWindow(ll_posx + ll_editgap - 26, ll_posy, 16, 16, true);
			m_butstatusg.ShowWindow(false);

			//Status editbox
			m_editStatus.GetClientRect(&lrect);
			m_editStatus.MoveWindow(ll_posx + ll_editgap, ll_posy,  ll_editw , ll_edith);

			m_butOpengate.LoadBitmaps(IDB_OPENGATE24,IDB_OPENGATE24SEL, IDB_OPENGATE24, IDB_OPENGATE24);
			m_butOpengate.MoveWindow( ll_posx + ll_editgap + ll_editw + 3 ,  ll_posy, 24,24, true);

			//License Plate Line
			ll_posy +=28;
			((CStatic*)GetDlgItem(IDC_STLP))->GetClientRect(&lrectst);
			((CStatic*)GetDlgItem(IDC_STLP))->MoveWindow(ll_posx - 10, ll_posy,  lrectst.Width(), lrect.Height());
			((CStatic*)GetDlgItem(IDC_EDITCARD))->GetClientRect(&lrect);
			if (nScreenW < 801)
				ll_editw = 90;
			((CStatic*)GetDlgItem(IDC_EDITCARD))->MoveWindow(ll_posx + ll_editgap, ll_posy,  ll_editw, lrect.Height());
			ll_posx += ll_editgap + ll_editw+ 3; 
			((CStatic*)GetDlgItem(IDC_EDITNAME))->MoveWindow(ll_posx , ll_posy,  ll_editw, lrect.Height());
			ll_posx +=  ll_editw+ 3; 
			((CStatic*)GetDlgItem(IDC_EDITLP))->MoveWindow(ll_posx , ll_posy,  ll_editw, lrect.Height());

			//================  For Gate Log List Control =================================
			CString csLoc1(_T(""));//date
			LOADSTRING(csLoc1, IDS_ACDATE);			

			CString csLoc2(_T(""));//time
			LOADSTRING(csLoc2, IDS_EMAPTIME);			

			CString csLoc3(_T(""));//status
			LOADSTRING(csLoc3, IDS_ACSTATUS);			

			CString csLoc4(_T("Gate"));//Gate
			LOADSTRING(csLoc4, IDS_HINT_GATE	);						
			
			CString csLoc5(_T("Cam Idx"));  //record id
			LOADSTRING(csLoc5, IDS_EMAPID );			

			CString csLoc6(_T("ID"));  //record idx
			LOADSTRING(csLoc6, IDS_EMAPID );			
 
			LV_COLUMN ListColumn; 			 
			WCHAR * ListTitles1[7] = {_T(""), csLoc1.GetBuffer(), csLoc2.GetBuffer(), csLoc3.GetBuffer(), csLoc4.GetBuffer() ,csLoc5.GetBuffer(), csLoc6.GetBuffer()};

			for(int i = 0; i < 7; i++)
			{
				ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
				ListColumn.fmt = LVCFMT_LEFT;
				ListColumn.iSubItem = i;

				if (i==0)  ListColumn.cx = 22; //Icon
				else if (i==1)  ListColumn.cx = 42; //Date
				else if (i==2)  ListColumn.cx = 55; //Time
				else if (i==3)  ListColumn.cx = 50; // Status - OK - Warning
				else if (i==4)  ListColumn.cx = 80; //Gate
				else if (i==5)  ListColumn.cx = 20; //camera idx
				else if (i==6)  ListColumn.cx = 0; //record id

				ListColumn.pszText = ListTitles1[i];
				m_listgate.InsertColumn(i, &ListColumn);
			}

			dwStyle = m_listgate.GetStyle();
			dwStyle |= LVS_EX_FULLROWSELECT;
			m_listgate.SetExtendedStyle(dwStyle);

			//Bitmap in the list control
			m_smImg.Create(16, 16, ILC_COLOR | ILC_MASK, 0, 1);

			//List Event Bitmap Stuff
			CBitmap bIcon,yIcon;
			bIcon.LoadBitmapW(IDB_REDCIRCLE);
			yIcon.LoadBitmapW(IDB_YELLOWCIRCLE);
			m_smImg.Add(&bIcon, RGB(0, 0, 0));
			m_smImg.Add(&yIcon, RGB(0, 0, 0));
			m_listgate.SetImageList(&m_smImg, LVSIL_SMALL);


			//===== Event =====
			m_EventReceiveFlag = true;
 			m_EmapEventRec = new CEventReceive();

			UINT nRetEvent(0);
			hThreadEventRec = (HANDLE)_beginthreadex(NULL, 0,  GetEventFromService, (LPVOID)this, 0, &nRetEvent);

			//============Tooltips ==========
			CString ls_tooltip;
			
			m_pToolTipCtrl = new CToolTipCtrl;
			if (!m_pToolTipCtrl->Create(this))		
				TRACE(_T("Unable To create ToolTip\n"));

			LOADSTRING(ls_tooltip, IDS_GTSEARCH);//playback
			m_pToolTipCtrl->AddTool(&m_butsch, ls_tooltip);

			LOADSTRING(ls_tooltip, IDS_GTREPORT);	//report
			m_pToolTipCtrl->AddTool(&m_butrep, ls_tooltip);

			LOADSTRING(ls_tooltip,IDS_GTSET	);//setting
			m_pToolTipCtrl->AddTool(&m_butset, ls_tooltip);

			LOADSTRING(ls_tooltip,  IDS_SELECTFOLDER );
			m_pToolTipCtrl->AddTool( (CBitmapButton*)GetDlgItem(IDC_BUTBROWSE), ls_tooltip);

			LOADSTRING(ls_tooltip, IDS_EMAPMAP  );
			m_pToolTipCtrl->AddTool( (CBitmapButton*)GetDlgItem(IDC_BUTMAP), ls_tooltip);    

} //fnInitControl

//Build Chain combobox
void CDlgGate::fnBuildCamGroup(void)
{
	int ll_cnt=0;	
	CString	csChainDesc, csDefaultValue, csMapfile, csVideoOn;
	char	cMD, cVL, cDI;

	DWORD		dwStyle = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | CBRS_TOP;

	//CString csSQL_DefaultGroup("SELECT cameragroupid, camera_group_desc, gatemap_file,   default_ FROM ec_camera_group ORDER BY default_ DESC");
	CString csSQL_DefaultGroup("SELECT chainid, chain_desc FROM ecgt_chain ORDER BY 1");
	CRecordset	rsChain(pDB);
	rsChain.Open(CRecordset::forwardOnly, csSQL_DefaultGroup);
	
	CDBVariant  varID;
	STRCHAIN	lChain;
	//STRGROUP lGroup;

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

void CDlgGate::OnCbnSelchangeCombogroup()
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
		 
}

//bool CDlgGate::fnGroupChanged(void)
//{
//		int nIndex = m_comboGroup.GetCurSel();
//		CString strGroup,strWFile, wholeFile, ls_folder ;
//		m_comboGroup.GetLBText(nIndex, strGroup);
//
////		fnDeleteAllCam();
//		ib_dirty = false;  //prevent resave 
//		//Get selected Group info from m_pvtGroup
//		m_groupid = m_vtGroup[nIndex].nid;
//		m_editMap.SetWindowTextW(m_vtGroup[nIndex].mapfile);   //put file into
//
//		if (m_groupid >0) fnPopulateCam(m_groupid); //display cameras in the list control
//		m_editFolder.GetWindowTextW(ls_folder);
//		wholeFile.Format(_T("%s\\%s"), ls_folder, m_vtGroup[nIndex].mapfile.Trim());
//
//		// Temp comment out
//		HBITMAP hbm = (HBITMAP) m_map.GetBitmap();
//		BITMAP bm;
//		SIZE szBitmap;
//		GetObject(hbm, sizeof(bm), &bm);
//		CBitmap hbmp;
//		 HBITMAP hbitmap;
//
//		if (!fnFileExist(wholeFile)) //File NOT exist
//		{
//			m_map.ShowWindow(SW_HIDE);
//			return false;
//		}
//		m_map.ShowWindow(SW_SHOW);
//		 hbitmap=(HBITMAP)::LoadImage(::AfxGetInstanceHandle(),wholeFile, IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
//
//		 hbmp.Attach(hbitmap);
//		 hbmp.GetBitmap(&bm);
//
//		 mi_orgwidth = bm.bmWidth;
//		 mi_orgheight = bm.bmHeight;
//	
//		  //if (mi_orgwidth > 
//		fnMaximizeBmp( mi_orgwidth, mi_orgheight);
//		m_map.MoveWindow(CHAINW + 13, TOPY,  mi_orgwidth,  mi_orgheight - TOPY , true);
//		m_map.Load(wholeFile);
//
//		//Repaint Cameras in map
////		fnDisplayCamAc(m_groupid);
//// 		fnStartupDisplayVideo();
//		
//		return false;
//
//}

// Click on different Parking lot
bool CDlgGate::fnGroupChanged()
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
		HBITMAP hbm = (HBITMAP) m_map.GetBitmap();
		BITMAP bm;
		SIZE szBitmap;
		GetObject(hbm, sizeof(bm), &bm);
		CBitmap hbmp;
		 HBITMAP hbitmap;

		if (!fnFileExist(wholeFile)) //File NOT exist
		{
			m_map.ShowWindow(SW_HIDE);
			return false;
		}
		m_map.ShowWindow(SW_SHOW);
		 hbitmap=(HBITMAP)::LoadImage(::AfxGetInstanceHandle(),wholeFile, IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);

		 hbmp.Attach(hbitmap);
		 hbmp.GetBitmap(&bm);

		 mi_orgwidth = bm.bmWidth;
		 mi_orgheight = bm.bmHeight;
	
		  //if (mi_orgwidth > 
		fnMaximizeBmp( mi_orgwidth, mi_orgheight);
		m_map.MoveWindow(CHAINW + 13, TOPY,  mi_orgwidth,  mi_orgheight - TOPY , true);
		m_map.Load(wholeFile);

		//Repaint Cameras in map
		fnDisplayCamGate(m_groupid);

 		fnStartupDisplayVideo();

		return false;
}


bool CDlgGate::fnFileExist(CString o_file)
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

void CDlgGate::fnMaximizeBmp(int &o_maxW, int &o_maxH)
{
	//Return the Max size allowed	
	if (o_maxW > m_maxMapW) 
		o_maxW=m_maxMapW;
	
	if (o_maxH > m_maxMapH) 
		o_maxH = m_maxMapH;

	if (o_maxH > nScreenH*3/4)
		o_maxH =  nScreenH*3/4;

}

bool CDlgGate::fnInitFolder(void)
{
	//If there's no row in the DB ec_storage of EMAP, put the working folder as the store loaction
	CString	ls_loc;
	CString ls_sql = (_T("SELECT store_location FROM ec_storage  WHERE storage_type='GATE'"));
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

		ls_sql.Format(_T("INSERT INTO ec_storage (storage_type, store_location) VALUES ('GATE', '%s') "), csAppPath.Trim() ); 
		TRACE(ls_sql);
		pDB->ExecuteSQL(ls_sql);

	}
 
	rsGroup.Close();
	return false;
}


void CDlgGate::fnInitGroup(void)
{
		int ll_cnt=0;	
	CString	csGroupDesc, csDefaultValue, csVideoOn, ls_map;
	char	cMD, cVL, cDI;

	DWORD		dwStyle = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW | CBRS_TOP;

	CString ls_sql = _T("SELECT ec_camera_group.cameragroupid, camera_group_desc,  default_, ecgt_chainparklot.gatemap_file FROM ec_camera_group, ecgt_chainparklot WHERE  ec_camera_group.cameragroupid = ecgt_chainparklot.cameragroupid AND ecgt_chainparklot.chainid>0 ORDER BY default_ DESC");
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
}


// Build all the camera info into m_vtGate from ecgtt_camera 
void CDlgGate::fnBuildGate(void)
{
	CString	ls_brand;
	CString ls_sql = (_T("SELECT gt_cameraid, camera_idx, access_brand FROM ecgt_camera"));
	CRecordset	rsAc(pDB);
	rsAc.Open(CRecordset::forwardOnly, ls_sql);
	
	CDBVariant  varID, varCamidx;
	STRGATE lAc;

	while (!rsAc.IsEOF()) { 
			
			rsAc.GetFieldValue( _T("ac_cameraid"),  varID, SQL_C_SSHORT );
			lAc.nid = varID.m_iVal;

			rsAc.GetFieldValue( _T("camera_idx"),  varCamidx, SQL_C_SSHORT );
			rsAc.GetFieldValue( _T("access_brand"),  ls_brand );	

			lAc.camidx = varCamidx.m_iVal;
			lAc.csBrand = ls_brand.Trim().MakeLower();
			m_vtGate.push_back(lAc);
			rsAc.MoveNext();
	}
	rsAc.Close();
}

//Display cameras into m_listCam
void CDlgGate::fnPopulateCam(int o_groupid)
{
	try
		{		
			if (&pDB==NULL)  return;
			CDBVariant varCamidx, varid, varNull;
			CString ls_sql1, ls_sql2, ls_sql3, ls_sql, ls_name, ls_ip, ls_full, ls_control;
			CRecordset  rs(pDB); 
			ls_sql1 = _T("SELECT ec_camera.video_format, ec_camera.camera_idx, ec_camera.ipaddress, ec_camera.cameraname, ec_camera_group_camera.cameragroupcameraid, ec_camera_group_camera.gtmap_x, ec_camera_group_camera.gtmap_y, ecgt_camera.gt_cameraid, ecgt_camera.access_brand, ecgt_camera.gate_control ");
			ls_sql2 = _T("  FROM ec_camera INNER JOIN ec_camera_group_camera ON ec_camera.cameraid=ec_camera_group_camera.cameraid AND ec_camera_group_camera.cameragroupid=");
			ls_sql3 = _T(" LEFT OUTER JOIN ecgt_camera ON ec_camera.camera_idx = ecgt_camera.camera_idx ORDER BY ec_camera.cameraid ASC");
			ls_sql.Format(_T("%s%s%d%s"),ls_sql1, ls_sql2, o_groupid, ls_sql3);
			TRACE(ls_sql);

			rs.Open(CRecordset::forwardOnly, ls_sql);
			int cnt=0;	

			m_listCam.DeleteAllItems();
			//m_listcam.SetImageList(&m_cImageListSmall, LVSIL_SMALL);

			while (!rs.IsEOF()) { 
					CDBVariant  varx, vary, varaccamid;
					CString	ls_brand,str;
					varx = varNull;
					varaccamid =varNull;

					rs.GetFieldValue(_T("video_format"), str);
					if (m_pbk->GetDevice(str) == eNVR || m_pbk->GetDevice(str) == IPVSTATION)
					{
						if (!m_pbk->IsChannel(str))
						{
							rs.MoveNext();
							continue;
						}
					}

					rs.GetFieldValue( _T("camera_idx"), varCamidx,  SQL_C_SSHORT );
					rs.GetFieldValue(_T("ipaddress"), ls_ip);	
					rs.GetFieldValue(_T("cameraname"), ls_name);
					rs.GetFieldValue( _T("cameragroupcameraid"), varid,  SQL_C_SSHORT );
					rs.GetFieldValue( _T("gtmap_x"), varx,  SQL_C_SSHORT);
					rs.GetFieldValue( _T("gtmap_y"), vary,  SQL_C_SSHORT );
					rs.GetFieldValue( _T("gt_cameraid"), varaccamid,  SQL_C_SSHORT );
					rs.GetFieldValue( _T("access_brand"), ls_brand );
					rs.GetFieldValue( _T("gate_control"), ls_control );
					
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

//Build Camera List header
bool CDlgGate::fnBuildList(void)
{
		//Camera List
		LV_COLUMN ListColumn;

		CString csLoc1(_T(""));//ipcam
		LOADSTRING(csLoc1, IDS_EMAPIPCAM);			

		CString csLoc2(_T(""));//id
		LOADSTRING(csLoc2, IDS_EMAPID);

		WCHAR * ListTitles[3] = {_T(""), csLoc1.GetBuffer(),  csLoc2.GetBuffer()};
		//WCHAR * ListTitles[3] = {_T(""), _T("IP Cameras"),_T("ID")};

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


/*
void CDlgGate::OnLvnBeginrdragListcam(NMHDR *pNMHDR, LRESULT *pResult)
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
*/

bool CDlgGate::fnCheckMapExist(void)
{
	CString		csMap;
	((CEdit*)GetDlgItem(IDC_EDITMAP))->GetWindowTextW(csMap);

	if (csMap==_T("")) return false;

	return true;
}

//true - camera already in the map, will abort
bool CDlgGate::fnCheckCamExist(int o_camGroupid, int o_camidx)
{
	for (int i=0; i<m_vpcam.size(); i++)
	{
		if (m_vpcam[i]->m_camgroupid==o_camGroupid &&  m_vpcam[i]->m_camidx == o_camidx && m_vpcam[i]->mc_mode !='D')		
			return true;
		if (m_vpcam[i]->m_mapx>0 && m_vpcam[i]->m_camidx == o_camidx)
			return true;
	}

	return false;
}

void CDlgGate::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		TRACE(_T("mouse move"));
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

bool CDlgGate::fnLoadBmp(CString o_file)
{
		HBITMAP hbm = (HBITMAP) m_map.GetBitmap();
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

//		m_map.MoveWindow(0, TOPY, mi_orgwidth,  mi_orgheight ,true);
		m_map.Load(o_file);
		m_map.ShowWindow(SW_SHOW);

	return false;
}


void CDlgGate::OnBnClickedClose()
{
	if (m_LsplitV.IsWindowVisible()==true)  //Hide it
		fnHideChain(true);
	else //Show Chain
		fnHideChain(false);
}

void CDlgGate::OnBnDoubleClicked()
{
	AfxMessageBox(_T("double click here"));
}

// Initialize Parking Chain and list into m_listChain
void CDlgGate::fnInitChain(void)
{
	LVITEM* litem = new LVITEM;
 
	CBitmap bIcon, bred;
 
		m_chainimg.Create(96, 96, ILC_COLOR16, 3, 1);
		bred.LoadBitmapW(IDB_PARKCHAIN);
		m_chainimg.Add(&bred, RGB(0, 0, 0));
		m_listChain.SetImageList(&m_chainimg, LVSIL_NORMAL  );
}

void CDlgGate::OnLvnItemchangedListchain(NMHDR *pNMHDR, LRESULT *pResult)
{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

		if (m_preitem == pNMLV->iItem) return;
		m_preitem = pNMLV->iItem;
	 
		if (m_vtGate.size()<=0 || m_preitem<0) return;

 		m_groupid = m_vtGateCam[m_preitem].camgroupid;
		fnGroupChanged();
		*pResult = 0;
}

void CDlgGate::fnChainChanged(int oChainid)
{
	m_listChain.DeleteAllItems();
	m_vtGate.clear();
	m_vtGateCam.clear();

	CString ls_sql, ls_sql1, ls_sql2, ls_desc;
	//ls_sql1 = _T("SELECT ecgt_chainparklot.chainid, ecgt_chainparklot.cameragroupid, ecgt_parklot.parklotid, ecgt_parklot.parklot_desc,  ec_camera_group_camera.camera_idx  ");
	//ls_sql2 = _T("FROM ecgt_chainparklot, ecgt_parklot, ec_camera_group_camera WHERE ecgt_chainparklot.parklotid = ecgt_parklot.parklotid AND ec_camera_group_camera.cameragroupid = ecgt_chainparklot.cameragroupid AND ");
	//ls_sql.Format(_T("%s%s ecgt_chainparklot.chainid=%d ORDER BY ecgt_parklot.parklotid"), ls_sql1, ls_sql2, oChainid);

	ls_sql1 = _T("SELECT ecgt_chainparklot.chainid, ecgt_chainparklot.cameragroupid, ecgt_parklot.parklotid, ecgt_parklot.parklot_desc,  ec_camera_group_camera.camera_idx  ");
	ls_sql2 = _T("FROM ecgt_chainparklot INNER JOIN ecgt_parklot ON ecgt_chainparklot.parklotid = ecgt_parklot.parklotid LEFT OUTER JOIN ec_camera_group_camera ON ec_camera_group_camera.cameragroupid = ecgt_chainparklot.cameragroupid WHERE ");
	ls_sql.Format(_T("%s%s ecgt_chainparklot.chainid=%d ORDER BY ecgt_parklot.parklotid"), ls_sql1, ls_sql2, oChainid);
	
	CRecordset	rsChain(pDB);
	rsChain.Open(CRecordset::forwardOnly, ls_sql);
	
	CDBVariant  varID, varGID, varChainID, varCamidx;
	STRCHAIN	lChain;
	int ll_cnt=0, ll_preParklotid=0 ;
	STRGATE	lGate;

	while (!rsChain.IsEOF()) { 

			rsChain.GetFieldValue( _T("chainid"), varChainID,  SQL_C_SSHORT);	
			rsChain.GetFieldValue( _T("cameragroupid"), varGID);	
			rsChain.GetFieldValue( _T("parklotid"), varID,  SQL_C_SSHORT);	
			rsChain.GetFieldValue(_T("parklot_desc"), ls_desc);
			rsChain.GetFieldValue(_T("camera_idx"), varCamidx,  SQL_C_SSHORT);

			lGate.camgroupid = varGID.m_iVal; 
			lGate.parklotid = varID.m_iVal;
			lGate.chainid = varChainID.m_iVal;
			lGate.camidx = varCamidx.m_iVal;
			lGate.cMode = 'D';  //Mark delete as default

			if (ll_preParklotid != varID.m_iVal)
			{
					int nItem = m_listChain.InsertItem( ll_cnt, ls_desc, 0 );
					m_vtGateCam.push_back(lGate);
			}			

			ll_cnt++;
			m_vtGate.push_back(lGate);

			ll_preParklotid = varID.m_iVal;
			rsChain.MoveNext();
	}//while loop
	
	//Build Chain Camera		
	fnGateCam();
	fnCleanupThread();		//Cleanup different thread
	fnStartGateService();

}

void CDlgGate::OnNMDblclkGlistcam(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

//void CDlgGate::OnHdnBegindragGlistcam(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

//void CDlgGate::OnLvnBeginrdragGlistcam(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

void CDlgGate::OnLvnBegindragGlistcam(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  	
	//Don't let it drag if map not exist...
 	if (!fnCheckMapExist()) return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	// Save the index of the item being dragged in m_nDragIndex
	//  This will be used later for retrieving the info dragged
	m_nDragIndex = pNMListView->iItem;
	CBitmap bitmap;
	// Create a drag image
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

void CDlgGate::OnLButtonUp(UINT nFlags, CPoint point)
{
	TRACE(_T("Left button up"));
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
			m_map.GetWindowRect(&lrect);
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
//	CBaseDialog::OnLButtonUp(nFlags, point);
}


bool CDlgGate::fnDropWithin(LPPOINT mpt)
{
		//if mpt is within the map1, return true, otherwise return false
	CRect rect; 	
	m_map.GetWindowRect(&rect);
 
	ScreenToClient(&rect); //optional step - see below  //position:  rect.left, rect.top //size: rect.Width(), rect.Height() 
	if (mpt->x < rect.right && mpt->x > rect.left && mpt->y >rect.top && mpt->y<rect.bottom)
		return true;
	else
		return false;
}

bool CDlgGate::fnLoadCamInfo(void)
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

	if ( m_vtGate.size()>0)
	{
		CString ls_in = _T(" in ( ");

		for (int i=0; i<	m_vtGate.size(); i++)
		{
			CString ls_cell;
			ls_cell.Format(_T(" %d,"), m_vtGate[i].camgroupid);
			ls_in += ls_cell; 
		}
		
		ls_in = ls_in.Mid(0, ls_in.GetLength()-1) + _T(")  ");
		
			ls_sql = _T("SELECT stream_name, ec_camera.cameraid, ec_camera.camera_idx,  ec_camera.cameraname, ec_camera.ipaddress, ec_camera.mac_address,ec_camera.httpport, ec_camera.username, ec_camera.password, ec_stream.stream_port, ec_stream.stream_protocol ");
			ls_sql += _T(" FROM ec_camera INNER JOIN ec_camera_group_camera ON ec_camera_group_camera.camera_idx =ec_camera.camera_idx  LEFT OUTER JOIN ec_stream ON ec_stream.cameraid = ec_camera.cameraid WHERE ec_camera_group_camera.cameragroupid ");			
			ls_sql = ls_sql + ls_in + _T(" AND stream_type like '%LV%'  AND ec_camera.active_='Y' ORDER BY ec_camera.camera_idx");			 
	}
	else
	{
			ls_sql = _T("SELECT stream_name, ec_camera.cameraid, ec_camera.camera_idx,  ec_camera.cameraname, ec_camera.ipaddress, ec_camera.mac_address,ec_camera.httpport, ec_camera.username, ec_camera.password, ec_stream.stream_port, ec_stream.stream_protocol ");
			ls_sql += _T(" FROM ec_camera LEFT OUTER JOIN ecgt_camera ON ecgt_camera.camera_idx =ec_camera.camera_idx  LEFT OUTER JOIN ec_stream ON ec_stream.cameraid = ec_camera.cameraid");
			ls_sql +=	_T(" WHERE stream_type like '%LV%'  AND ec_camera.active_='Y' ORDER BY ec_camera.camera_idx");
	}

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

bool CDlgGate::fnLoadCamInfo(int oCamidx)
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
	ls_sql += _T(" FROM ec_camera INNER JOIN ecgt_camera ON ecgt_camera.camera_idx =ec_camera.camera_idx  LEFT OUTER JOIN ec_stream ON ec_stream.cameraid = ec_camera.cameraid");
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


void CDlgGate::fnSaveNewLoc(int oGroupid, int oCamidx, int posx, int o_posy)
{
	try{
		CString ls_sql;
		ls_sql.Format(_T("UPDATE ec_camera_group_camera SET gtmap_x = %d,  gtmap_y=%d  WHERE cameragroupid=%d AND camera_idx=%d"), posx,  o_posy, oGroupid,  oCamidx  );
		//ls_sql.Format(_T("UPDATE ec_camera_group_camera SET acmap_x = %d,  acmap_y=%d  WHERE cameragroupid=%d AND camera_idx=%d"), posx,  o_posy, oGroupid,  oCamidx  );
 	
			TRACE(ls_sql);
			pDB->ExecuteSQL(ls_sql);
	}
	catch(CDBException * p) 	
	{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}	
}

void CDlgGate::fnInsertCamera(int oCamidx)
{
	CString	ls_sql;
	bool	lb_found = false;

	try{
				//prevent duplicate in ecgt_camera
					CRecordset m_rs(pDB);
					ls_sql.Format( _T("SELECT camera_idx FROM ecgt_camera WHERE  camera_idx = %d"), oCamidx);
			 
					m_rs.Open(CRecordset::dynamic, ls_sql);
					while(!m_rs.IsEOF() ) 
					{
						lb_found = true;
						return;
					}
					m_rs.Close();

 				//ls_sql.Format(_T("INSERT INTO ecac_camera(active_,camera_idx, access_brand, access_model)  VALUES('Y', %d, '%s', '%s')"), oCamidx, ACDFTBRAND, ACDFTMODEL);							
					ls_sql.Format(_T("INSERT INTO ecgt_camera(active_,camera_idx, access_brand, access_model)  VALUES('Y', %d, '%s', '%s')"), oCamidx, ACDFTBRAND, ACDFTMODEL);
				pDB->ExecuteSQL(ls_sql);
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
		return ;
	}
	
}


void CDlgGate::fnDisplayVideo(int o_camidx)
{
	//Loop through m_vctpAllCam and find the camidx
	int ll_camx=0, ll_camy=0;

	//Check video already playing
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
					m_EventVideo->mc_parent = 'G'; //Gate					

					m_EventVideo->m_camidx = o_camidx;					

					m_EventVideo->Create(IDD_DLGEMAPVIEW1, this);
		 
					CRect lrect, llrect;
					GetWindowRect(&lrect);
					GetClientRect(&llrect);
	 
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


	//for (int i=0; i<	 m_vctpAllCam.size(); i++)
	//{
	//	if ( m_vctpAllCam[i]->camera_idx == o_camidx)
	//	{
	//		try{
	//				NODEITEM* lpCam = m_vctpAllCam[i];

	//				CEventVideo* m_EventVideo= new CEventVideo; 
	//				m_EventVideo->fnAssignNode(lpCam);
	//				m_EventVideo->fnAssignTitle(lpCam->camera_name);
	//				m_EventVideo->mc_mode = 'P'; //Play mode
	//				//m_EventVideo->mb_ac = true;
	//				m_EventVideo->mc_parent = 'G'; //Gate
	//				m_EventVideo->m_camidx = o_camidx;
	//				m_EventVideo->Create(IDD_DLGEMAPVIEW1, this);
	//	 
	//				CRect lrect, llrect;
	//				GetWindowRect(&lrect);
	//				GetClientRect(&llrect);
	//				//if (mb_startup)
	//				//	ll_camx = ll_camx + llrect.left;
	//				//else
	//				//	ll_camx = ll_camx + lrect.left;
	//				ll_camy = ll_camy + lrect.top;
	//				m_EventVideo->MoveWindow(CRect(ll_camx, ll_camy+40, ll_camx + ACVIDEOW,  ll_camy+50 + ACVIDEOH), true);

	//				m_EventVideo->ShowWindow(SW_SHOW);
	//				m_EventVideo->BringWindowToTop();
	//				m_vpEventVideo.push_back(m_EventVideo);
	//				return;
	//		}
	//		catch(CException* p)
	//		{
	//					p->Delete();
	//		}
	//	}
 
	} //for loop
}

	//Display camera icons into the bitmap
bool CDlgGate::fnDisplayCamGate(int oGroupid)
{
	CString cssql, cssql1, cssql2, cssql3;
	int emapx, emapy, ll_cnt=0;
 
	//Loop into ec_camera_group_camera and display the camer icons
 	cssql1 = _T("SELECT ec_camera_group_camera.gtmap_x, ec_camera_group_camera.gtmap_y, ec_camera_group_camera.cameragroupcameraid, ec_camera.cameraname, ec_camera.camera_idx,ecgt_camera.gt_cameraid ");
	cssql2 =  _T(" FROM ec_camera_group_camera INNER JOIN ec_camera ON ec_camera.cameraid=ec_camera_group_camera.cameraid  LEFT OUTER JOIN ecgt_camera ON ec_camera_group_camera.camera_idx = ecgt_camera.camera_idx WHERE cameragroupid=");
	//cssql3 = _T("  AND acmap_x>0 AND acmap_y>0 ORDER BY ec_camera_group_camera.cameraid ");
	cssql3 = _T("  ORDER BY ec_camera_group_camera.cameraid ");

	cssql.Format( _T("%s%s%d%s"), cssql1, cssql2, oGroupid, cssql3);
	CRecordset	rsCam(pDB);
	rsCam.Open(CRecordset::forwardOnly, cssql);
	
	CDBVariant  varEmapx, varEmapy, varNull, varid, varidx, varaccameraid;
	STRGROUP lGroup;

	while (!rsCam.IsEOF()) { 
			varEmapx = varNull;

			rsCam.GetFieldValue( _T("gtmap_x"), varEmapx, SQL_C_SSHORT);	
			emapx = varEmapx.m_iVal;

			rsCam.GetFieldValue( _T("gtmap_y"), varEmapy, SQL_C_SSHORT);	
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

					m_map.GetWindowRect(&lrect);
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
					rsCam.GetFieldValue( _T("gt_cameraid"), varaccameraid, SQL_C_SSHORT);	
					lid = varid.m_iVal;
					
					//Assign camera info into CEmapCam
					lpCam->m_camgroupid = lid;
					lpCam->m_idx = varidx.m_iVal;
					lpCam->m_mapx = emapx;
					lpCam->m_mapy = emapy;
					lpCam->m_title = ls_name.Trim();
				
					if (varaccameraid.m_iVal>0){
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
					lpTxt->ShowWindow(SW_SHOW);
					m_vpName.push_back(lpTxt);
			}

			rsCam.MoveNext();
	
	} //while loop
	rsCam.Close();
	return false;
}


//For open 2nd time of Access Control
void CDlgGate::fnStartupDisplayVideo()
{
	for (int i=0; i<m_vtGate.size(); i++)
	{
		if (m_vtGate[i].camidx >0 && m_vtGate[i].camgroupid == m_groupid)
		{
			if (fnCamWithinGroup(m_groupid, m_vtGate[i].camidx ))
			fnDisplayVideo(m_vtGate[i].camidx);
		}
	}
}

void CDlgGate::fnStartGateService()
{ 
		USES_CONVERSION;
	 
		try
		{
			CString csIP, csUser, csPwd, csWebPort, csCamIDX, csCamID, csProtocol;
			CRecordset m_rs(pDB);

			m_rs.Open(CRecordset::forwardOnly, _T("SELECT ec_camera.cameraid, ec_camera.camera_idx,  ec_camera.mac_address, ec_camera.ipaddress, ec_camera.httpport, ec_camera.username, ec_camera.password, ec_stream.stream_protocol, ec_stream.stream_name FROM ec_camera INNER JOIN ec_stream ON ec_camera.cameraid = ec_stream.cameraid  INNER JOIN ecgt_camera ON ec_camera.camera_idx = ecgt_camera.camera_idx  WHERE ecgt_camera.gate_control='Y' AND ec_stream.stream_name = 'stream 1'"));
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
	CString csTimer = m_ini.getKeyValue(_T("timer"),_T("parking gate"));
	int	mi_timer;
	if (csTimer!=_T(""))
		mi_timer = _ttoi(csTimer );
	else
		mi_timer = GATETIME;

	for(int iTemp = 0; iTemp < m_vcCamInfo.size(); iTemp++)
	{
			UINT nOldRet(0);
			m_vcCamInfo[iTemp]->cDB = pDB;
			m_vcCamInfo[iTemp]->pListAc = &m_listgate;
			
			if (fnCheckCamActive(_ttoi(m_vcCamInfo[iTemp]->csCamIDX)))		
				m_hGateThread = (HANDLE)::_beginthreadex(NULL, 0, CDlgGate::ReceiveGate, (LPVOID)m_vcCamInfo[iTemp], 0, &nOldRet);
		
			int m_timerid;
			if (iTemp ==0) m_timerid =  ID_TIMER_GT0;
			else if (iTemp ==1) m_timerid =  ID_TIMER_GT1;
			else if (iTemp ==2) m_timerid =  ID_TIMER_GT2;
			else if (iTemp ==3) m_timerid =  ID_TIMER_GT3;
			else if (iTemp ==4) m_timerid =  ID_TIMER_GT4;
			else if (iTemp ==5) m_timerid =  ID_TIMER_GT5;
			else if (iTemp ==6) m_timerid =  ID_TIMER_GT6;
			else if (iTemp ==7) m_timerid =  ID_TIMER_GT7;
			else if (iTemp ==8) m_timerid =  ID_TIMER_GT8;
			else if (iTemp ==9) m_timerid =  ID_TIMER_GT9;
			else if (iTemp ==10) m_timerid =  ID_TIMER_GT10;
			else if (iTemp ==11) m_timerid =  ID_TIMER_GT11;
			else if (iTemp ==12) m_timerid =  ID_TIMER_GT12;
			else if (iTemp ==13) m_timerid =  ID_TIMER_GT13;
			else if (iTemp ==14) m_timerid =  ID_TIMER_GT14;
			else if (iTemp ==15) m_timerid =  ID_TIMER_GT15;
			else if (iTemp ==16) m_timerid =  ID_TIMER_GT16;
			else if (iTemp ==17) m_timerid =  ID_TIMER_GT17;
			else if (iTemp ==18) m_timerid =  ID_TIMER_GT18;
			else if (iTemp ==19) m_timerid =  ID_TIMER_GT19;
			else if (iTemp ==20) m_timerid =  ID_TIMER_GT20;
 			
			SetTimer(m_timerid, GATETIME, 0 );
			fnTimertoVector(_ttoi(m_vcCamInfo[iTemp]->csCamIDX), _T("ETROVISION"), m_timerid);
	 
	} // for loop...	

	//SetTimer(ID_TIMER_SECUREP, 3000, 0);
}

bool CDlgGate::fnCamWithinGroup(int oGroupid, int oCamidx)
{
	try{
		if (oGroupid<=0) oGroupid = m_groupid;
		CDBVariant  varEmapx, varEmapy;
		CString ls_sql;
		ls_sql.Format(_T("SELECT ec_camera_group_camera.cameragroupcameraid, gtmap_x, gtmap_y  FROM ec_camera_group_camera  INNER JOIN ec_camera ON ec_camera_group_camera.cameraid = ec_camera.cameraid WHERE ec_camera_group_camera.camera_idx = %d and cameragroupid=%d and ec_camera.active_='Y' and ec_camera_group_camera.gtmap_x>0 and ec_camera_group_camera.gtmap_y>0"), oCamidx, oGroupid);
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

BOOL CDlgGate::PreTranslateMessage(MSG* pMsg)
{
	//Tooltips
	if ( m_pToolTipCtrl != NULL)
		m_pToolTipCtrl->RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgGate::OnBnDoubleclicked()
{
	AfxMessageBox(_T("double clicked"));
}

void CDlgGate::fnHideChain(bool ob_toHide)
{
	bool bShow = true;
	const	int BUTH=16;
	int	ll_movex=0;
	CRect lrect, lwrect, lvrect, llvrect;
	
	m_map.GetClientRect(&lrect);
	m_map.GetWindowRect(&lwrect);

	
	if (ob_toHide == true) // Hide Chain
	{
			bShow = false;
			m_map.MoveWindow(0, TOPY, lrect.right, lrect.bottom, true);			
	
			btncloseChain->MoveWindow(0, TOPY - 13, BUTH, BUTH, false);
			btncloseChain->SetBitmaps(IDB_TREE_CLOSED, MASKCOLOR, (int)BTNST_AUTO_GRAY);
			//CRect	 lhrect,lvrect;
			m_splitV.GetWindowRect(&lvrect);
			m_splitBotH.MoveWindow(0, m_botHPos , lvrect.left, 5, 1);
			ll_movex = - m_rect.right + LPANELW;
 
	}
	else // Show Chain
	{
			bShow = true;
			int ll_posx = 160;
			//m_map.MoveWindow(m_rect.right - LPANELW, TOPY, lrect.right, lrect.bottom, true);
			m_map.MoveWindow(ll_posx, TOPY, lrect.right, lrect.bottom, true);
			
			btncloseChain->SetBitmaps(IDB_CLOSE, MASKCOLOR, (int)BTNST_AUTO_GRAY);
			btncloseChain->MoveWindow(CHAINW-20,TOPY, BUTH, BUTH, true);
			m_LsplitV.GetWindowRect(&llvrect);
			m_splitV.GetWindowRect(&lvrect);
			m_splitBotH.MoveWindow(llvrect.right, m_botHPos, lvrect.left - llvrect.left, 5, 1); 
			ll_movex = m_rect.right - LPANELW;
	}

	for (int ci=0; ci < m_vpcam.size() ; ci++)
	{
				CEmapAccessCam* lpcam =  m_vpcam[ci];					
				int ll_camx;
				if (bShow)
					ll_camx = lpcam->m_mapx;
				else
					ll_camx = lpcam->m_mapx + (ll_movex);
				lpcam->MoveWindow(ll_camx, lpcam->m_mapy,CAMSIZEW, CAMSIZEH, true);

				//Move Cam Static text
				if ( m_vpName.size()> ci)
				{
					CBaseStatic* lpName =  m_vpName[ci];	
					if (!lpName) continue;
					lpName->MoveWindow(ll_camx+40, lpcam->m_mapy, STCW, STCH, true);
					//lpName->MoveWindow(lpName->m_mapx + (ll_movex), lpName->->m_mapy,	STCW, STCH, true);
				}

	}

	m_comboGroup.ShowWindow(bShow);
	m_listChain.ShowWindow(bShow);
	m_LsplitV.ShowWindow(bShow);	
}


LRESULT CDlgGate::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
 	if (message == WM_NOTIFY)
	{
		if (wParam == IDC_SPLITH || wParam == IDC_SPLITV ||  wParam == IDC_SPLITBOTH ||  wParam ==  IDC_LSPLITV)
		{	
			SPC_NMHDR* pHdr = (SPC_NMHDR*) lParam;
			DoResizeH(wParam, pHdr->delta);
		}	
	}

	if (message == WM_NCLBUTTONDBLCLK)
	{
			STR_CAM* pCam = (STR_CAM*) lParam;
			int lid = pCam->camidx;
			if (lid>0 && lid<1000)	
				fnDisplayVideo(pCam->camidx);		
	}
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
							ls_sql.Format(_T("UPDATE ec_camera_group_camera SET gtmap_x = %d,  gtmap_y=%d  WHERE cameragroupid=%d AND camera_idx=%d"), lcpt->x,  lcpt->y, m_groupid, pCam->camidx );				 	
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

//void CDlgGate::DoResizeH(  int delta)
//{
		//CRect rc;
		//if (wParam == IDC_SPLITV )
		//	{
		//		//resize tab control and tab page
		//		CSplitterControl::ChangeHeight(&m_listgate, -delta, CW_BOTTOMALIGN);
		//		CSplitterControl::ChangeHeight(&m_listCam, delta, CW_TOPALIGN);
		//		CSplitterControl::ChangePos(&m_staclog, 0 , delta);
		//		CSplitterControl::ChangePos( GetDlgItem(IDC_STGROUP2), 0 , delta);
		//		CSplitterControl::ChangePos( GetDlgItem(IDC_STGROUP3), 0 , delta);
		//}
		//Invalidate();
		//UpdateWindow();
//}


void CDlgGate::DoResizeH(WPARAM wParam, int delta)
{
		if (wParam ==  IDC_SPLITH)
		{
				CSplitterControl::ChangeHeight(&m_listgate, -delta, CW_BOTTOMALIGN);
				CSplitterControl::ChangeHeight(&m_listCam, delta, CW_TOPALIGN);
				CSplitterControl::ChangePos(&m_staclog, 0 , delta);
				CSplitterControl::ChangePos(GetDlgItem(IDC_CKFILTER), 0 , delta);
		}
		else if (  wParam == IDC_SPLITV)
		{
				CSplitterControl::ChangePos(&m_listCam, -delta , 0);
				CSplitterControl::ChangeWidth(&m_listCam, -delta, CW_LEFTALIGN);

				CSplitterControl::ChangePos(&m_listgate, -delta , 0);
				CSplitterControl::ChangeWidth(&m_listgate, -delta, CW_LEFTALIGN);

				CSplitterControl::ChangePos(&m_splitH, -delta , 0);
				CSplitterControl::ChangeWidth(&m_splitH, -delta, CW_LEFTALIGN);

				CSplitterControl::ChangePos(GetDlgItem(IDC_STCAM), -delta, 0);
				CSplitterControl::ChangePos(GetDlgItem(IDC_STGROUP2), -delta, 0);
				CSplitterControl::ChangePos(GetDlgItem(IDC_STGROUP3), -delta, 0);
				CSplitterControl::ChangePos(GetDlgItem(IDC_RADIOMAP), -delta, 0);
				CSplitterControl::ChangePos(GetDlgItem(IDC_RADIOLIVE), -delta, 0);
				CSplitterControl::ChangePos(GetDlgItem(IDC_STAC), -delta, 0);

				CSplitterControl::ChangeWidth(&m_splitBotH, delta, CW_LEFTALIGN);
				//CSplitterControl::ChangeWidth(&m_gbDetail, delta, CW_LEFTALIGN);	
				CSplitterControl::ChangeWidth(&m_editFolder, -delta, CW_RIGHTALIGN);
				CSplitterControl::ChangeWidth(&m_editMap, -delta, CW_RIGHTALIGN);

				CSplitterControl::ChangeWidth(&m_map, delta, CW_LEFTALIGN);
		}
		else if (  wParam == IDC_SPLITBOTH)
		{
			CSplitterControl::ChangePos(GetDlgItem(IDC_STGBDETAIL ), 0, delta);
			CSplitterControl::ChangePos(GetDlgItem( IDC_STSTATUS ), 0, delta );
			CSplitterControl::ChangePos(GetDlgItem( IDC_STGATEINFO  ), 0, delta );

			CSplitterControl::ChangePos(GetDlgItem(  IDC_STDATETIME ), 0, delta );

			CSplitterControl::ChangePos(GetDlgItem(  IDC_STLP ), 0, delta );
			CSplitterControl::ChangePos(GetDlgItem( IDC_EDITSTATUS), 0, delta );
			CSplitterControl::ChangePos(GetDlgItem( IDC_EDITPARK), 0, delta );
			CSplitterControl::ChangePos(GetDlgItem( IDC_EDITCAM ), 0, delta );
			CSplitterControl::ChangePos(GetDlgItem( IDC_EDITDATE ), 0, delta );
			CSplitterControl::ChangePos(GetDlgItem( IDC_EDITTIME ), 0, delta );
			CSplitterControl::ChangePos(GetDlgItem( IDC_EDITCARD ), 0, delta );
			CSplitterControl::ChangePos(GetDlgItem( IDC_EDITNAME ), 0, delta );
			CSplitterControl::ChangePos(GetDlgItem( IDC_EDITLP ), 0, delta );
			CSplitterControl::ChangePos(GetDlgItem( IDC_BUTOPENGATE ), 0, delta );
			CSplitterControl::ChangePos(GetDlgItem(IDC_BUTGOTO ), 0, delta );
			
			CSplitterControl::ChangeHeight(&m_map, delta, CW_TOPALIGN);
		}
		else if (  wParam == IDC_LSPLITV)
		{
			CSplitterControl::ChangePos(GetDlgItem(IDC_STGBDETAIL ), -delta, 0);
			CSplitterControl::ChangePos(GetDlgItem(IDC_STSTATUS ), -delta, 0);
			CSplitterControl::ChangePos(GetDlgItem(IDC_EDITSTATUS ), -delta, 0);
			CSplitterControl::ChangePos(GetDlgItem(  IDC_BUTGOTO ), -delta, 0);
			
			CSplitterControl::ChangePos(&m_butOpengate, -delta, 0);

			CSplitterControl::ChangePos(GetDlgItem(IDC_STGATEINFO ), -delta, 0);
			CSplitterControl::ChangePos(GetDlgItem(IDC_EDITPARK ), -delta, 0);
			CSplitterControl::ChangePos(GetDlgItem(IDC_EDITCAM ), -delta, 0);
	 
			CSplitterControl::ChangePos(GetDlgItem(IDC_STDATETIME   ), -delta, 0);
			CSplitterControl::ChangePos(GetDlgItem( IDC_EDITDATE   ), -delta, 0);
			CSplitterControl::ChangePos(GetDlgItem( IDC_EDITTIME   ), -delta, 0);

			CSplitterControl::ChangePos(GetDlgItem( IDB_CLOSE  ), -delta, 0);
			CSplitterControl::ChangeWidth(&m_listChain, delta, CW_LEFTALIGN);
			CSplitterControl::ChangeWidth(&m_comboGroup, delta, CW_LEFTALIGN);

			CSplitterControl::ChangePos(GetDlgItem(   IDC_STLP ), -delta, 0);
			CSplitterControl::ChangePos(GetDlgItem(  IDC_EDITCARD ), -delta, 0);
			CSplitterControl::ChangePos(GetDlgItem( IDC_EDITNAME    ), -delta, 0);
			CSplitterControl::ChangePos(GetDlgItem(   IDC_EDITLP  ), -delta, 0);
			
			CSplitterControl::ChangeWidth(&m_map, -delta, CW_RIGHTALIGN);
			CSplitterControl::ChangeWidth(&m_splitBotH, -delta, CW_RIGHTALIGN);
		}

		Invalidate();
		UpdateWindow();
}

void CDlgGate::fnDeleteAllCam(void)
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

bool CDlgGate::fnAfterMove(int o_newx, int o_newy, int o_camgroupid, int o_camidx)
{
		int	ll_basex=0;
		CPoint pt(o_newx, o_newy);
		bool lb_drop = fnDropWithin(&pt);

		CRect		lrect;
		m_map.GetWindowRect(&lrect);
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

void CDlgGate::fnRemoveCam(int o_camidx)
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
						ls_sql.Format(_T("UPDATE ec_camera_group_camera SET gtmap_x = NULL,  gtmap_y=NULL  WHERE cameragroupid=%d AND camera_idx=%d"), m_groupid, o_camidx  );	 	
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


void CDlgGate::fnAfterVideoClose(int o_camidx)
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

void CDlgGate::fnOpenDoor(int o_idx)
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
		//CGI for Open door
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


bool CDlgGate::fnCloseVideo(void)
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
void CDlgGate::OnClose()
{
		fnCloseVideo();
		fnFreeMemory();
		CBaseDialog::OnClose();

}

void CDlgGate::fnTimertoVector(int oCamIdx, CString oBrand, int oTimer)
{
	for (int i=0; i<m_vtGate.size(); i++)
	{
		if (m_vtGate[i].camidx == oCamIdx )
			m_vtGate[i]._timer = oTimer;
	}
}

UINT CDlgGate::ReceiveGate(LPVOID lpParam)
{
			CameraInfo* pCameraInfo = (CameraInfo*)lpParam;
			CDlgGate*	pDlgGate = (CDlgGate*)pCameraInfo->pWnd;

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

			curl_easy_setopt(g_curl, CURLOPT_WRITEFUNCTION, &CDlgGate::notifyWriter);
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
				  pDlgGate->fnCntconnect(pCameraInfo->csCamIDX);
				  //AfxMessageBox(ls_err);
				  return -1;
			}
			else
			{
					while(pCode != CURLE_COULDNT_CONNECT)
					{				
						//pCameraInfo->bFirstFlag = true;
						//pCode = curl_easy_perform(g_curl);  
						if	(pCode == CURLE_OK) {
								//map<CString, CameraInfo>::iterator iPos = mapCameraInfo.begin();
								//iPos = mapCameraInfo.find(pCameraInfo->csIP);
								//mapCameraInfo.erase(iPos);
								//curl_easy_cleanup(g_curl);
								//curl_global_cleanup();
								//delete chUrl;
								//delete chUserPwd;
								//::Sleep(10000);
							return 0;
						} //if 
					}//while
			} // else - connect 
				curl_easy_cleanup(g_curl);
			delete chUrl;
			delete chUserPwd;
			  
		return 1;
}

void CDlgGate::fnFreeMemory()
{
 			//Kill Timer
			for (int i=0; i<m_vtGate.size(); i++)
				{
					if (m_vtGate[i]._timer>0)
						KillTimer(m_vtGate[i]._timer);
				}

			m_vtGate.clear();
			curl_global_cleanup();	
					  
}



// Build Gate Cameras with attached, used for threaex
void CDlgGate::fnGateCam(void)
{
		int ll_groupid;
		CString ls_sql;		
		CDBVariant  varCamIdx;

		//Mark camera in ecgt_camera as Active
				ls_sql = (_T("SELECT ecgt_camera.camera_idx  FROM ecgt_camera WHERE gate_control='Y'"));
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

void CDlgGate::fnGateTimer(int oTimerid)
{ 
	if ( m_vcCamInfo.size()<=0) return;
		if (m_vtGate.size()<=0) return;
		bool lb_found = false;

		CameraInfo* pCameraInfo;
		CString	ls_brand, ls_camIdx;
		CString csURL;
	
		for (int i=0; i<m_vtGate.size(); i++)
		{
			if (m_vtGate[i]._timer == oTimerid)		//find the right timer id
			{
				ls_brand = m_vtGate[i].csBrand.Trim().MakeLower();
				for (int j=0; j<m_vcCamInfo.size(); j++)
				{
					ls_camIdx.Format(_T("%d"),m_vtGate[i].camidx);
					m_timerIdx = m_vtGate[i].camidx;
					if (m_vcCamInfo[j]->csCamIDX == ls_camIdx)
					{
						lb_found = true;
						pCameraInfo = m_vcCamInfo[j];
					}
				}
			}
		}

		if (!lb_found) return;	//make sure it find the right info
 

		//CGI send command request data
		csURL = _T("http://") + pCameraInfo->csIP  + _T("/config/send_cmd.cgi?cmd=7e040125db01&");//Soyal 
 

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
}


// Check if camera exist in Chain Cam vector
bool CDlgGate::fnCheckCamActive(int oCamidx)
{
	bool lb_ret = false;
	
	for (int i=0; i<m_vtGate.size(); i++)
	{
		if (m_vtGate[i].camidx == oCamidx)
		{
			if ( m_vtGate[i].cMode == 'A') //active
				return true;
			else
				return false;
		}

	}
	return lb_ret;

}


int CDlgGate::notifyWriter(char* data, size_t size, size_t nmemb,void *lParam)
{
 
 	if(lParam == NULL)	return 0;
	CameraInfo* pCameraInfo = (CameraInfo*)lParam;
	CDlgGate* pDlgGate = (CDlgGate*) (pCameraInfo->pWnd);

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
		
		//temp
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

		if (pDlgGate!=NULL)	 
		{
				pDlgGate-> fnModifyAccess(lParam, lsAll, pCameraInfo->csCamIDX);
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

void CDlgGate::OnTimer(UINT_PTR nIDEvent)
{
 	bool lb_auto = false;
	int	ltime = 0;
	int	iTimerid=0;
	m_timerIdx = 0;

	//For display security info once
	//if (nIDEvent == ID_TIMER_SECUREP)
	//{
	//	fnReSecurity();
	//	m_securecnt++;
	//	if (m_securecnt==6) KillTimer(ID_TIMER_SECUREP);
	//}

	fnGateTimer(nIDEvent);

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

bool CDlgGate::fnModifyAccess(LPVOID param, CString oData, CString oCamIdx)
{
		CString	ls_event=_T(""), ls_rtype=_T("OK");
		bool okflag = false;
		CString	ls_id, ls_date, ls_hhmm;
 
		pDB = ((CameraInfo*)param)->cDB;
 
		if (oData.GetLength() <40) 
		{
			ls_event = oData.Mid(6,2).MakeLower();
			CString ls_secure = oData.Mid(30,2).MakeLower();

			if (ls_event == _T("03") && ls_secure == _T("45")) // Let 03 passed, security check
			{
					for (int i=0; i<m_vtGate.size(); i++)
					{
						if (m_vtGate[i].camidx == _ttoi(oCamIdx))
						{
							m_vtGate[i].csecurity = 'Y';
							break;
						}
					}
					//fnSetCurrentTime(_ttoi(oCamIdx));	
					return true;
			}
			else
				return false;
		}

//		fnSetVideoFlag( _ttoi(oCamIdx), false);
		CString	ls_ss, ls_mm, ls_hh, ls_dd, ls_MM, ls_yy;
		CString ls_trace;
 
			if (oData.Left(2)==_T("7e"))	//Start code
			{

				ls_event = oData.Mid(6,2).MakeLower();
				if (ls_event == _T("0b") || ls_event  == _T("03") ) //0b- normal; 03-Invalid Card
				{				 
					ls_trace.Format(_T("Data:    %s\r\n"), oData);
					TRACE(ls_trace);

					if (ls_event  == _T("03")) ls_rtype = _T("WARN");
					
					ls_ss = fnHexToDec(oData.Mid(10,2));
					ls_mm = fnHexToDec(oData.Mid(12,2));
					ls_hh = fnHexToDec(oData.Mid(14,2));

					ls_dd = fnHexToDec(oData.Mid(18,2));
					ls_MM = fnHexToDec(oData.Mid(20,2));
					ls_yy = fnHexToDec(oData.Mid(22,2));
					
					if (fnDataValid(ls_yy, ls_MM, ls_dd, ls_hh, ls_mm, ls_ss)  == false)
					{
						fnClearRec(_ttoi(oCamIdx));
					}

					ls_date.Format(_T("20%s/%s/%s"),ls_yy, ls_MM, ls_dd);
					ls_hhmm.Format(_T("%s:%s:%s"),ls_hh,ls_mm, ls_ss);
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
					//	fnClearRec(_ttoi(oCamIdx));
						return false;
					}
			}
			else
				return false;
 
	 
		int		accessID=0;
		try{
			//Get Employee Name and Department
			CString ls_sql1, ls_sql2, ls_sql3, ls_sql, csDept, csGCode, csEmpName, csPark;
			CDBVariant		varParklotid;
	 
			CRecordset m_rs(pDB);
			//Get parklot_desc
			ls_sql1 = _T("SELECT ecgt_camera.camera_idx,  ecgt_parklot.parklotid, ecgt_parklot.parklot_desc  FROM ecgt_camera, ec_camera_group_camera, ecgt_chainparklot, ecgt_parklot  ");
			ls_sql2 = _T(" WHERE ecgt_parklot.parklotid = ecgt_chainparklot.parklotid AND ecgt_chainparklot.cameragroupid = ec_camera_group_camera.cameragroupid AND ");
			ls_sql3 = _T(" ec_camera_group_camera.camera_idx = ecgt_camera.camera_idx AND gate_control='Y'  AND ecgt_camera.camera_idx =  ");
 			ls_sql.Format( _T("%s%s%s%s"), ls_sql1, ls_sql2, ls_sql3, oCamIdx );
			
			m_rs.Open(CRecordset::dynaset, ls_sql);
			if (m_rs.GetRecordCount() >0)
			{
  				m_rs.MoveFirst();
				while(!m_rs.IsEOF()   ) 
				{ 	  
					okflag = true;
					m_rs.GetFieldValue( _T("parklotid"), varParklotid, SQL_C_SSHORT);	
					m_rs.GetFieldValue( _T("parklot_desc"), csPark); 
					break;
				}
			}
				
			//Get  card/name/LP info from ecgt_card
			CString	csName=_T(""), csPlate=_T("");
			CDBVariant		varCardid;
			m_rs.Close();
			ls_sql.Format(_T("SELECT cardid, name, lplate1 FROM ecgt_card WHERE cardcode = '%s'"), ls_id);
			m_rs.Open(CRecordset::dynaset, ls_sql);
			if (m_rs.GetRecordCount() >0)
			{
  				m_rs.MoveFirst();
				while(!m_rs.IsEOF()   ) 
				{ 	  
					m_rs.GetFieldValue( _T("cardid"), varCardid); 
					m_rs.GetFieldValue( _T("name"), csName); 
					m_rs.GetFieldValue( _T("lplate1"), csPlate); 
					break;
				}
			}

			//Insert row into Gate Log List Control
			CString ls_status=_T("");
			CString* lp_retStatus = &ls_status;


			ls_trace.Format(_T("Date: %s;    Time:%s \r\n"), ls_date, ls_hhmm);
			TRACE(ls_trace);
			//Insert row into ecgt_record
			fnInsertDB( ls_rtype , ls_date, ls_hhmm, ls_id,  varCardid.m_iVal, _ttoi( oCamIdx), csName, csPlate, varParklotid.m_iVal, csPark);

			fnInsertRow(param, ls_event, ls_date, ls_hhmm, oCamIdx , csPark, ls_id, csName,  lp_retStatus);
			m_rs.Close();

			//In case no related data in the DB
			if (!okflag)
			{
				fnInsertRow(param, ls_event, ls_date, ls_hhmm, oCamIdx,  _T("Unknown"), ls_id, csGCode,  lp_retStatus);
				ls_rtype = _T("WARN");
			}


		}
		catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}

		//Don't save data into unauthrized data into DB
		//if ( fnCheckAuthorize(_ttoi(oCamIdx)))
 	//		fnAccessInsertDB(ls_rtype, ls_date, ls_hhmm, ls_id, accessID, _ttoi(oCamIdx));

	fnClearRec(_ttoi(oCamIdx));

		//For display video
		//m_EventTimer.iTimer = 7;
 	//	m_EventTimer.iIndex = _ttoi(oCamIdx);
		//m_EventTimer.cMode = 'O'; //Open Video
		//m_vpEventTimer.push_back(m_EventTimer);

		return true;
}

CString CDlgGate::fnHexToDec(CString oSource)
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

//Date, Time, Status, Gate, ID, Card
bool CDlgGate::fnInsertRow(LPVOID param, CString oStatus, CString oDate, CString oTime, CString oCamidx, CString oParklot, CString oCardcode, CString oName, CString* opRet)
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

			if (oStatus == _T("0b")) //Normal for Controller
			{
					lvi.iImage = 3;
					ls_status = _T("OK");
					//if filtering warning, pass OK status
					if (mb_filter)
						return true;
			}
			else if (oStatus == _T("03")) //No Passed for Controller
			{
					lvi.iImage = 0;
					ls_status = _T("Warn");
			}
			else if (oDate==_T("") && oTime==_T("") && oStatus==_T("04")) //Might happen...skip the 04 status
				return false;

			 *opRet = ls_status ;
			lvi.pszText = _T("");
			int nItem = m_listgate.InsertItem(&lvi);

			//Date
			lvi.iSubItem =1;	
			_stprintf(szItem, _T("%s"), oDate.Mid(5));
			lvi.pszText =szItem;
			m_listgate.SetItem(&lvi);

			//Time
			lvi.iSubItem =2;	
			CString csTime;
			csTime.Format(_T("%s:%s"), oTime.Mid(0,2), oTime.Mid(2,2));
			lvi.pszText = (LPTSTR)(LPCTSTR)(oTime);
			m_listgate.SetItem(&lvi);

			//Status
			lvi.iSubItem =3; 
			lvi.pszText = (LPTSTR)(LPCTSTR)(ls_status.MakeUpper());
			m_listgate.SetItem(&lvi);
			
			//Parking Lot Desc
			lvi.iSubItem =4; 
			lvi.pszText = (LPTSTR)(LPCTSTR)(oParklot);
			m_listgate.SetItem(&lvi);

			//Cameraidx
			lvi.iSubItem =5;  
			lvi.pszText = (LPTSTR)(LPCTSTR)(oCamidx);
			m_listgate.SetItem(&lvi);

			//Put the record id into last column
			lvi.iSubItem =6;
			CString	ls_sql;
			ls_sql =  _T("SELECT gate_recordid FROM ecgt_record WHERE gate_recordid IN (SELECT MAX(gate_recordid) FROM ecgt_record)");

			CRecordset	rs(pDB);
			rs.Open(CRecordset::forwardOnly, ls_sql);
			CDBVariant  varID;
 
			while (!rs.IsEOF()) { 
				rs.GetFieldValue( _T("gate_recordid"), varID,  SQL_C_SSHORT);	
				CString ls_recordid;
				ls_recordid.Format(_T("%d"), varID.m_iVal);
				lvi.pszText = (LPTSTR)(LPCTSTR)(ls_recordid);
				m_listgate.SetItem(&lvi);
				break;
			}

			if (ls_status.MakeLower() == _T("warn"))
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

			return false;
}


bool CDlgGate::fnInsertRowPanic( CString oDate, CString oTime, CString oCamidx, int oParklot, CString oPark )
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
			ls_status = _T("Panic");
 
 			lvi.pszText = _T("");
			int nItem = m_listgate.InsertItem(&lvi);

			//Date
			lvi.iSubItem =1;	
			_stprintf(szItem, _T("%s"), oDate.Left(5));
			lvi.pszText =szItem;
			m_listgate.SetItem(&lvi);

			//Time
			lvi.iSubItem =2;	
			CString csTime;
			csTime.Format(_T("%s:%s"), oTime.Mid(0,2), oTime.Mid(2,2));
			lvi.pszText = (LPTSTR)(LPCTSTR)(oTime);
			m_listgate.SetItem(&lvi);

			//Status
			lvi.iSubItem =3; 
			lvi.pszText = (LPTSTR)(LPCTSTR)(ls_status);
			m_listgate.SetItem(&lvi);
			
			//Parking Lot Desc
	
			lvi.iSubItem =4; 
			lvi.pszText = (LPTSTR)(LPCTSTR)(oPark);
			m_listgate.SetItem(&lvi);

			//Cameraidx
			lvi.iSubItem =5;  
			lvi.pszText = (LPTSTR)(LPCTSTR)(oCamidx);
			m_listgate.SetItem(&lvi);

			//Put the record id into last column
			lvi.iSubItem =6;
			CString ls_sql;
			ls_sql =  _T("SELECT gate_recordid FROM ecgt_record WHERE gate_recordid IN (SELECT MAX(gate_recordid) FROM ecgt_record)");

			CRecordset	rs(pDB);
			rs.Open(CRecordset::forwardOnly, ls_sql);
			CDBVariant  varID;
 
			while (!rs.IsEOF()) { 
				rs.GetFieldValue( _T("gate_recordid"), varID,  SQL_C_SSHORT);	
				CString ls_recordid;
				ls_recordid.Format(_T("%d"), varID.m_iVal);
				lvi.pszText = (LPTSTR)(LPCTSTR)(ls_recordid);
				m_listgate.SetItem(&lvi);
				break;
			}
 
			return false;
}


void CDlgGate::fnInsertDB(CString oStatus, CString oDate, CString oTime, CString oCardCode, int oCardid, int oCamIdx, CString oName, CString oPlate, int oParklotid, CString oPark)
{
	try{
 			CString	ls_sql, ls_dt;
			ls_dt.Format(_T("%s %s"),oDate, oTime);
		
			ls_sql.Format(_T("INSERT INTO ecgt_record (record_daytime, status_, cardcode, cardid, camera_idx, name, lplate, parklotid, parklot_desc ) VALUES ( CONVERT(datetime, '%s'), '%s', '%s', %d, %d, '%s', '%s', %d, '%s' )"), ls_dt, oStatus, oCardCode, oCardid, oCamIdx, oName, oPlate, oParklotid, oPark  );
	 		TRACE(ls_sql);
			pDB->ExecuteSQL(ls_sql);
	}
	catch(CDBException * p)
		{
			AfxMessageBox(p->m_strError);
			p->Delete();
		}
}

//For Soyal need to clear previous info
bool CDlgGate::fnClearRec(int oCamIdx)
{
		bool lb_found = false;
		CString	ls_camIdx;
		if ( m_vcCamInfo.size()<=0) return false;
		if ( m_vtGate.size()<=0) return false;
		CameraInfo* pCameraInfo;
 		//CameraInfo* pCameraInfo = m_vcCamInfo[0];
	for (int i=0; i<m_vtGate.size(); i++)
		{
			if (m_vtGate[i].camidx == oCamIdx  )		//find the right timer id
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

//void CDlgGate::OnLvnItemchangedListac(NMHDR *pNMHDR, LRESULT *pResult)
//{
//		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//		
//		int ll_item = pNMLV->iItem;
//		TCHAR szBuffer[12];
//		DWORD cchBuf(12);
//		LVITEM lvi;
//		lvi.iItem = ll_item;
//		lvi.iSubItem = 7; //record id
//		lvi.mask = LVIF_TEXT;
//		lvi.pszText = szBuffer;
//		lvi.cchTextMax = cchBuf;
//		m_listGate.GetItem(&lvi);
//		CString ls_recordid = szBuffer;
//
// 		fnDetails(_ttoi(ls_recordid));
//		*pResult = 0;
//}

//Load Details Info
void CDlgGate::fnDetails(int oRecordid)
{
		CString	ls_sql, csStatus, csCardcode, csDateTime, csName, csPlate, csPark, csCam;

		ls_sql.Format(_T("SELECT status_, parklotid, ecgt_record.camera_idx, cardid, cardcode, record_daytime, name, lplate, parklot_desc, ec_camera.cameraname  FROM ecgt_record  LEFT OUTER JOIN ec_camera ON ec_camera.camera_idx =ecgt_record.camera_idx WHERE gate_recordid = %d"), oRecordid);
		TRACE(ls_sql);

			CRecordset	rs(pDB);
			rs.Open(CRecordset::forwardOnly, ls_sql);
			CDBVariant  varCamidx, varDateTime, varParklotid;
 
			while (!rs.IsEOF()) { 
				rs.GetFieldValue( _T("status_"), csStatus);	
				rs.GetFieldValue( _T("parklotid"), varParklotid,  SQL_C_SSHORT);
				rs.GetFieldValue( _T("camera_idx"), varCamidx,  SQL_C_SSHORT);
				rs.GetFieldValue( _T("cardcode"), csCardcode);					
				rs.GetFieldValue( _T("record_daytime"), varDateTime, SQL_C_TIMESTAMP );
				rs.GetFieldValue( _T("name"), csName);	
				rs.GetFieldValue( _T("lplate"), csPlate);	
				rs.GetFieldValue( _T("parklot_desc"), csPark);	
				rs.GetFieldValue( _T("cameraname"), csCam);	
				break;
			}
 
			if (varDateTime.m_pdate == NULL) return;
			if (varDateTime.m_pdate->year < 2010) return; //To prevent crash

			CTime ltime(varDateTime.m_pdate->year,varDateTime.m_pdate->month,varDateTime.m_pdate->day, varDateTime.m_pdate->hour, varDateTime.m_pdate->minute, varDateTime.m_pdate->second);
		
			CString ls_date, ls_time, ls_parklotid;
			ls_parklotid.Format(_T("%d"), varParklotid.m_iVal);
			ls_date.Format(_T("%d//%d//%d"), varDateTime.m_pdate->year, varDateTime.m_pdate->month, varDateTime.m_pdate->day);			
			m_editDate.SetWindowTextW(ls_date);
						
			ls_time.Format(_T("%d:%d:%d"), varDateTime.m_pdate->hour, varDateTime.m_pdate->minute, varDateTime.m_pdate->second);			
			m_editTime.SetWindowTextW(ls_time);

			m_editStatus.SetWindowTextW((LPCTSTR)csStatus);
			((CEdit*)GetDlgItem(IDC_EDITPARKLOT))->SetWindowTextW( ls_parklotid);
			((CEdit*)GetDlgItem(IDC_EDITCARD))->SetWindowTextW(csCardcode);
			((CEdit*)GetDlgItem(IDC_EDITNAME))->SetWindowTextW(csName);
			((CEdit*)GetDlgItem(IDC_EDITLP))->SetWindowTextW(csPlate);
			((CEdit*)GetDlgItem(IDC_EDITPARK))->SetWindowTextW(csPark);
			((CEdit*)GetDlgItem(IDC_EDITCAM))->SetWindowTextW(csCam);
			CString ls_idx;
			ls_idx.Format(_T("%d"),varCamidx.m_iVal);
			((CEdit*)GetDlgItem(IDC_EDITIDX))->SetWindowTextW(ls_idx);
			
			bool lb_showred = true;
			if (csStatus.MakeUpper().Trim()==_T("OK")) 
				m_butstatusr.ShowWindow(!lb_showred);
			else
				m_butstatusr.ShowWindow(lb_showred);
			//m_butstatusr.ShowWindow(lb_showred);
			//m_butstatusg.ShowWindow(!lb_showred);			 
}

void CDlgGate::OnBnClickedButset()
{
	fnOpenSetting(true);
}

void CDlgGate::fnOpenSetting(bool oNormal)
{
		CDlgSettingGate lDlg;
//		lDlg.mb_openNormal = oNormal;
		if (lDlg.DoModal()==IDOK)
		{
			//if (lDlg.mb_camChanged)		//Original Codes
 		//		fnGroupChanged();
	
			//if (lDlg.mb_holChanged)		//Rebuild holiday structure
			//	fnRefreshHolVec();

			//if (lDlg.mb_priChanged)	//Refresh vector of work hour and privilege
			//	fnRefreshPriVec();
				
		}
}

void CDlgGate::OnBnClickedButopengate()
{
	// 	CString ls_status=_T("");
	//		CString* lp_retStatus = &ls_status;

	//fnInsertRow(NULL,_T("WARN"),_T("2012-02-21"), _T("15:22,22"),_T("21"),_T("DEPT"),_T("Peter"), _T("WARN"),lp_retStatus);
	//return;

	CString ls_idx;
	((CStatic*)GetDlgItem(IDC_EDITIDX))->GetWindowTextW(ls_idx);
	if (_ttoi(ls_idx)>0 && _ttoi(ls_idx)<1000)
		fnOpenDoor(_ttoi(ls_idx));
}

void CDlgGate::OnBnClickedButsearch()
{
			TCHAR apppath[MAX_PATH]={0};
			GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
			CString	csAppPath(apppath);
	        int nPos = csAppPath.ReverseFind('\\');
			csAppPath = csAppPath.Left(nPos + 1);
 
			csAppPath += _T("accessplay.exe");
			ShellExecute(NULL,_T("open"), csAppPath, _T("g"),NULL,SW_SHOWNORMAL);		
}

void CDlgGate::OnBnClickedButrep()
{
// Run Report
	 TCHAR apppath[MAX_PATH]={0};
	GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
	CString csAppPath(apppath);
	int nPos = csAppPath.ReverseFind('\\');
	csAppPath = csAppPath.Left(nPos + 1);
	CString csReport = csAppPath + _T("\\EcReport.exe");

	ShellExecuteW(NULL,_T(""),csReport, _T("g"),NULL,SW_SHOWNORMAL);
}


void CDlgGate::OnNMClickListac(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CDlgGate::OnLvnItemchangedLgate(NMHDR *pNMHDR, LRESULT *pResult)
{
			LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
			int ll_item = pNMLV->iItem;
			m_logitem = ll_item;
			TCHAR szBuffer[12];
			DWORD cchBuf(12);
			LVITEM lvi;
			lvi.iItem = ll_item;
			lvi.iSubItem = 6;	//record id
			lvi.mask = LVIF_TEXT;
			lvi.pszText = szBuffer;
			lvi.cchTextMax = cchBuf;
			m_listgate.GetItem(&lvi);
			
			//Get Recordid for log
			CString ls_recordid = szBuffer;
			m_currecordid = _ttoi(ls_recordid);
 			fnDetails(m_currecordid);
			*pResult = 0;
}

//Modify Status here...
void CDlgGate::OnBnClickedButstatusr()
{
 		// Set gate item as OK
		LVITEM lvItem;
		lvItem.mask = LVIF_IMAGE ;
		lvItem.iItem = m_logitem;
		lvItem.pszText = _T("");
		lvItem.iSubItem = 0;
		lvItem.iImage = 2; //blank
		int ll_ret = m_listgate.SetItem(&lvItem);

		//Get Original status
		CString ls_orgStatus;
		m_editStatus.GetWindowTextW(ls_orgStatus);

		lvItem.mask = LVIF_TEXT ;
		lvItem.iItem = m_logitem;
		lvItem.pszText = _T("OK");
		lvItem.iSubItem = 3;
		ll_ret = m_listgate.SetItem(&lvItem);
		m_editStatus.SetWindowTextW(_T("OK"));

		//update record
		if (m_currecordid>0)
		{
				CString	ls_sql;
				ls_sql.Format(_T("UPDATE ecgt_record SET status_ = 'OK', org_status='%s', modify='Y' WHERE gate_recordid = %d "), ls_orgStatus, m_currecordid ); 
				TRACE(ls_sql);
				pDB->ExecuteSQL(ls_sql);
		}

		m_butstatusr.ShowWindow(false);
		m_butstatusg.ShowWindow(false);
 }

// Check if date value valid 
bool CDlgGate::fnDataValid(CString oyy, CString omm, CString odd, CString ohh, CString omin, CString oss)
{
	if (oyy.GetLength()>2 || omm.GetLength()>2 || odd.GetLength()>2 || ohh.GetLength()>2 || omin.GetLength()>2 || oss.GetLength()>2 ) return false;
	if (_ttoi(oyy) > 20 || _ttoi(oyy) < 8) return false;
	
	if (_ttoi(omm) > 12 || _ttoi(omm) < 1) return false;
	if (_ttoi(odd) > 31 || _ttoi(odd) < 1) return false;
	if (_ttoi(ohh) > 24 || _ttoi(ohh) < 0) return false;
	if (_ttoi(omin) > 60 || _ttoi(omin) < 0) return false;
	if (_ttoi(oss) > 60 || _ttoi(oss) < 0) return false;

	return true;
}

int CDlgGate::fnGetMinChainid(void)
{
		CDBVariant	varid;	
		CRecordset m_rs(pDB);
		CString	ls_sql;
		
		ls_sql =_T("SELECT chainid FROM ecgt_chain ORDER BY chainid ");
		m_rs.Open(CRecordset::dynaset, ls_sql);
		
		while (!m_rs.IsEOF()) { 		 
				m_rs.GetFieldValue(_T("chainid"), varid, SQL_C_SSHORT);
				return varid.m_iVal;			
		}
	return -1;
}

void CDlgGate::fnInitLocalize(void)
{
		CString csLoc(_T(""));
		//LOADSTRING(csLoc, IDC_STAC);
		//((CStatic*)GetDlgItem(IDC_STAC))->SetWindowTextW(csLoc);

		LOADSTRING(csLoc, IDS_EMAPIPCAM);
		((CStatic*)GetDlgItem(IDC_STCAM))->SetWindowTextW(csLoc);

		LOADSTRING(csLoc,   IDS_EMAPFOLDER);
		GetDlgItem(IDC_STGROUP2 )->SetWindowTextW(csLoc);

		//LOADSTRING(csLoc, IDC_STGROUP3 );
		LOADSTRING(csLoc,  IDS_EMAPMAP);
		GetDlgItem(IDC_STGROUP3 )->SetWindowTextW(csLoc);

		//LOADSTRING(csLoc, IDC_CKFILTER);
		LOADSTRING(csLoc, IDS_CHECKWARN  );
		GetDlgItem(IDC_CKFILTER )->SetWindowTextW(csLoc);

		//LOADSTRING(csLoc, IDC_STSTATUS);
		//GetDlgItem(IDC_STSTATUS )->SetWindowTextW(csLoc);

		//LOADSTRING(csLoc, IDC_STGATEINFO);
		//GetDlgItem(IDC_STGATEINFO )->SetWindowTextW(csLoc);

		//============== Details ============
		GetDlgItem( IDC_STGBDETAIL )->SetWindowTextW(csLoc);	//GroupBox

		LOADSTRING(csLoc, IDS_STSTATUS);
		GetDlgItem( IDC_STSTATUS )->SetWindowTextW(csLoc);	//Status		
				
		LOADSTRING(csLoc, IDS_STGATEINFO);
		GetDlgItem( IDC_STGATEINFO )->SetWindowTextW( csLoc); //parking lot - Camera

		LOADSTRING(csLoc, IDS_STDATETIME);
		GetDlgItem( IDC_STDATETIME )->SetWindowTextW( csLoc); //datetime

		LOADSTRING(csLoc, IDS_STLP);
		GetDlgItem( IDC_STLP )->SetWindowTextW( csLoc);		//Card Name, Plate
 		
}

void CDlgGate::OnBnClickedCkfilter()
{
	int ll_check = m_ckAlarm.GetCheck();
	if (ll_check==1)
		mb_filter = true;
	else
		mb_filter = false;
}

void CDlgGate::OnBnClickedButgoto()
{
	//Get  ls_parklotid
	CString	ls_parklotid;
	((CEdit*)GetDlgItem(IDC_EDITPARKLOT))->GetWindowTextW(ls_parklotid);
	if (_ttoi(ls_parklotid)<=0) return;
	int ll_parklotid = _ttoi(ls_parklotid);

	if (m_vtGateCam.size()<=0) return;
	for(int i=0; i<m_vtGateCam.size(); i++)
	{
		if (m_vtGateCam[i].parklotid == ll_parklotid)
		{
			if (m_vtGateCam[i].camgroupid >0)
			{
				if (m_vtGateCam[i].camgroupid == m_groupid)
					return;

				m_groupid = m_vtGateCam[i].camgroupid;
				fnGroupChanged();
				return;
			}
		}
	} // for loop

}

UINT _stdcall CDlgGate::GetEventFromService(LPVOID lpParam)
{
	CDlgGate* pThis = (CDlgGate*) lpParam;

	pThis->m_EmapEventRec->vSetCallBack(&pThis->fnEmapCallBack, pThis);
	pThis->m_EmapEventRec->vStartReceive();
	
	return 0;

}

void CDlgGate::fnEmapCallBack(EventInfo *m_EventInfo, void *lpParam)
{
	CDlgGate* pThis = (CDlgGate*)lpParam;
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
bool CDlgGate::fnEventDisplay(CString oIndex, CString osIP, CString osEventType, CString osTime)
{
		//Check if camera in the chain
		int	ll_camidx, ll_parklotid=0;
		bool	lb_found = false;
		ll_camidx = _ttoi(oIndex);

		for (int i=0; i<m_vtGate.size(); i++)
		{
			if (m_vtGate[i].camidx == ll_camidx)
			{
				ll_parklotid = m_vtGate[i].parklotid;
				lb_found = true;
				break;
			}
		}
		if (!lb_found) return false;

		//Save into DB 
		CTime t = CTime::GetCurrentTime(); 	 
		CString ls_today;		
		CString ls_sql, ls_desc=_T("");
		ls_today.Format(_T("%02d/%02d/%02d"), t.GetMonth(), t.GetDay(), t.GetYear());

		//Get parklot desc
		if (ll_parklotid>0)
		{
				CRecordset m_rs(pDB);

				ls_sql.Format(_T("SELECT parklot_desc FROM ecgt_parklot WHERE parklotid = %d"), ll_parklotid);
				m_rs.Open(CRecordset::dynaset, ls_sql);
				if (m_rs.GetRecordCount() >0)
				{
					m_rs.MoveFirst();
					while(!m_rs.IsEOF()   ) 
					{ 	  
						m_rs.GetFieldValue( _T("parklot_desc"), ls_desc); 
						break;
					}
				}
		}

		fnInsertDB(_T("PANIC"), ls_today, osTime, _T(""), 0, ll_camidx,_T(""), _T(""), ll_parklotid , ls_desc);
		fnInsertRowPanic(ls_today, osTime, oIndex, ll_parklotid, ls_desc );
 
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

void CDlgGate::fnCleanupThread(void)
{
	curl_global_cleanup();
	for(int i=0; i<m_vtGate.size(); i++)
	{
		if (m_vtGate[i]._timer>0)
				KillTimer(m_vtGate[i]._timer);
	}
}

// check if the camera attached Gate Controller
bool CDlgGate::fnIscontroller(int oCamidx)
{
	bool lb_ret = false;

	for (int i=0; i<m_vtGate.size(); i++)
	{
		if (m_vtGate[i].camgroupid == m_groupid)
		{
			if (m_vtGate[i].camidx == oCamidx && m_vtGate[i].cMode=='A')
				return true;		
		}
	}
	return lb_ret;
}

void CDlgGate::fnCntconnect(CString oCamidx)
{
	for (int i=0; i<m_vtGate.size(); i++)
	{
		if (m_vtGate[i].camidx == _ttoi(oCamidx))
		{
			if (m_vtGate[i]._timer >0)
				KillTimer(m_vtGate[i]._timer);
		}
	}
}

//Check if the video display door
bool CDlgGate::fnVideoDoor(int o_camidx)
{
		CString ls_sql, ls_control;
		ls_sql.Format(_T("SELECT gate_control FROM ecgt_camera WHERE camera_idx=%d"), o_camidx);
 
			try{
			TRACE(ls_sql);
			CRecordset m_rs(pDB);	 
			m_rs.Open(CRecordset::dynamic, ls_sql);
			while(!m_rs.IsEOF() ) 
			{
					m_rs.GetFieldValue(_T("gate_control"), ls_control);
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
