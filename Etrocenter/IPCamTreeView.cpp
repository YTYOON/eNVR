#include "stdafx.h"
#include "IPCamTreeView.h"
#include "IPCamTreeCtrl.h"
#include "NVR.h"
#include "CGIVar.h"
//#include "DlgAddInternetIpcam.h"
#include "DlgAddManageCam.h"

#include "MainFrm.h"
#include "DlgSettingCamGroup.h"
#include "DlgSettingMulMonitor.h"
#include "DlgSettingRecording.h"
#include "DlgMassAssignIP.h"
#include "DlgFtp.h"
//#include "DlgSettingEventAction.h"
#include "DlgNAS.h"
//#include "DlgSettingEvent.h"
//#include "DlgSettingAC.h"
//#include "DlgSettingGate.h"


#define TREEW 185
#define BUTTONH 30
#define CAMERABKCOLOR RGB(0,0,0)
#define STARTID 3001
#define MATRIX_INDEX 100
#define SETTING_INDEX 101
#define APP_INDEX 102
#define ADDCAMERAID 1
#define ASSIGNIPID 9
#define EVENTID 3
#define CAMERAGROUPID 4
#define RECORDING 5
#define MULMONID 6
#define SYSTEMSETTINGID 7
#define FTPID 8
#define ACCESSID 23
#define EVENTACTION 25


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIPCamTreeView::CIPCamTreeView()
{
	pDoc = new CIPCamDiscovery();//this);
	m_pvtNode = pDoc->GetNodePtr();
	//CDlgMatrixTag	*lDlg = new CDlgMatrixTag;
	bool lb_open=true;
}

CIPCamTreeView::~CIPCamTreeView()
{
	//delete (CIPCamDiscovery*)pDoc;
}

BEGIN_MESSAGE_MAP(CIPCamTreeView, CDockablePane)
	//ON_BN_CLICKED(IDC_TREE_REFRESH, OnBnClickedRefreshTree)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_DESTROY()	
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(ID_CHANGE_BMP, OnChangeBMP)
    //ON_COMMAND_RANGE(IDC_TREECTRL+2, IDC_TREECTRL+4, OnRangeCmds)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyBar message handlers

int CIPCamTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

//	SetSCBStyle(GetSCBStyle() | SCBS_SHOWEDGES | SCBS_SIZECHILD);
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP ;
	BOOL bRet = m_TreeCtrl.Create(dwStyle, CRect(0,0,185,500), this, IDC_TREECTRL+1);
	//InitCreate();

	// Load Matrix Bmp according to INI files

	CIniReader m_ini;
	m_ini.setINIFullFileName(_T("\\ec.ini"));

	CString csCellSize = m_ini.getKeyValue(_T("matrix"),_T("video"));
	if(csCellSize.IsEmpty()) csCellSize = _T("1"); 	
	int nCellSize = _ttoi(csCellSize);
	
	//bChangeBmp(nCellSize);

	UINT nRet;
	HANDLE hThRefresh = (HANDLE)_beginthreadex(NULL, 0, RefreshThread, this, 0, &nRet);
	CloseHandle( hThRefresh );

	return 0;
}



UINT  CIPCamTreeView::RefreshThread(LPVOID lpParam)
{
	CIPCamTreeView* pThis = (CIPCamTreeView*) lpParam;
	int ci =0;
	HANDLE hNewIPCamEvent;
	HANDLE hExitEvent, hCleanEvent;
	hNewIPCamEvent = CreateEvent(NULL, FALSE, TRUE,  _T("Global\\NEW_IPCAM_AVAILABLE"));
	hExitEvent = 	 CreateEvent(NULL, TRUE,  FALSE, _T("Global\\IPCAM_EXPLORER_EXIT"));
	hCleanEvent = 	 CreateEvent(NULL, FALSE, FALSE, _T("Global\\CLEAN_TREE_TABLE"));

	HANDLE hEvents[]={hNewIPCamEvent, hExitEvent, hCleanEvent};
	DWORD dwRet = -1;	
	while (dwRet != (WAIT_OBJECT_0+1))
	{
		dwRet = ::WaitForMultipleObjects(3, hEvents, FALSE, INFINITE);
		switch (dwRet)
		{
		case WAIT_OBJECT_0:
			for (; ci < pThis->m_pvtNode->size() ; ci++)
			{
				vector <NODEITEM*>& pvtNode = *(pThis->m_pvtNode);
				NODEITEM* pNode =  pvtNode[ci];
				//CGS::vTraceItem(pNode);
				pThis->m_TreeCtrl.vAttach(pNode);

				/***** This will causing program to hang while closing...Comment it out for now *******/
				//#ifndef FACTORY
				//	pNode->pIPCamCGIInfo = new CGIVar;//CIPCamCGIInfo;
				//	((CGIVar*) pNode->pIPCamCGIInfo)->vAttach(pNode);//->ip, pNode->nWebPort, pNode->szUser, pNode->szPass);
				//#endif
			}

			break;
			
		case WAIT_OBJECT_0+1:
			ETROLOG(_T("Exit Event"));
			break;
		case WAIT_OBJECT_0+2:
			ci = 0;
			break;
		default:
			SDKERRMSG(1);
			ASSERT(FALSE);
			break;
		}
	}
	::CloseHandle(hNewIPCamEvent);
	::CloseHandle(hExitEvent);
	::CloseHandle(hCleanEvent);

	return 0;
}

void CIPCamTreeView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	TRACE(_T("Windows pWnd:%x\tpoint x:%d\tpoint y:%d\r\n"),pWnd, point.x, point.y);
	m_TreeCtrl.OnContextMenu(pWnd, point);
	
}

void CIPCamTreeView::OnDestroy() 
{
	CDockablePane::OnDestroy();
	//if (m_TreeButton) m_TreeButton.DestroyWindow();


	for (int ci=0; ci < m_pvtNode->size(); ci++)
	{
		if ((*m_pvtNode)[ci]->pIPCamCGIInfo)
		{
			delete (CGIVar*)((*m_pvtNode)[ci]->pIPCamCGIInfo);
			(*m_pvtNode)[ci]->pIPCamCGIInfo = NULL;
		}
	}
	delete (CIPCamDiscovery*)pDoc;

	//if (pMatrix >0) delete pMatrix;
	//if (pSetting>0) delete pSetting;
	//if (pApplication >0) delete pApplication;
	//if (pstMatrix >0) delete pstMatrix;
	//if (pstSetting>0) delete  pstSetting;
	//if (pstApp >0) delete pstApp;

}

void CIPCamTreeView::vLiveView() 
{
	m_TreeCtrl.vLiveView();
}


void CIPCamTreeView::OnAddInternetIpCam()
{
	vSettingManager(ADDCAMERAID); //Reroute the codes into Setting Manager

	/******* Original Codes *****/
	//NODEITEM aNode;// = new NODEITEM;
	//::ZeroMemory(&aNode, sizeof(NODEITEM));
	//CDlgAddInternetIpcam dlg(&aNode);
	//dlg.DoModal();
	//pDoc->vAddInternetIpcam(&aNode);
}

void CIPCamTreeView::vResetData()
{
	m_TreeCtrl.DeleteAllItems();
	m_TreeCtrl.InsertDefaultRoot();
	pDoc->vResetData();
}

void CIPCamTreeView::OnSize(UINT nType, int cx, int cy)
{
	//CSizingControlBarG::OnSize(nType, cx, cy);
		//m_TreeButton.MoveWindow(60,0, cx, BUTTONH);
	m_TreeCtrl.MoveWindow(CRect(0, 0, cx, cy)); 

	//m_TreeCtrl.MoveWindow(CRect(63, BUTTONH, cx, cy));
	m_TreeCtrl.Invalidate(); // <- Add this

		
}
 


BOOL CIPCamTreeView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Set Background Color here...
	CBrush brush(CAMERABKCOLOR);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &brush);
	 
	pDC->SetBkColor(RGB(0,0,0));	
	return true;
}

HBRUSH CIPCamTreeView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
		pDC->SetBkColor(RGB(0,0,0));	
		return  m_SilverBrush;
}
//
//void CIPCamTreeView::InitCreate()
//{	
//	DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_TABSTOP ;
////AddButton(images.ExtractIcon(iIconIndex), strName, uiCmd);	
//	//Create Tree Button
//	CString ls_txt;
//	LOADSTRING(ls_txt, IDS_LIVEVIEW_GROUP);
//	//CFont* m_TreeFont = new CFont;
//	//m_TreeFont->CreateFontW(12, 0, 0, 0,  FW_NORMAL,FALSE,FALSE,0,ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("新細明體"));   
//
//	bool bRet;
//	//bRet = m_TreeButton.Create(ls_txt, dwStyle, CRect(0, 0, 185,30), this, IDC_TREECTRL);
//	//bool bRet = m_TreeButton.Create(_T("Live View Group"), dwStyle, CRect(0, 0, 185,30), this, IDC_TREECTRL);
//
//	dwStyle = WS_CHILD | WS_VISIBLE | BS_OWNERDRAW;
//	/*mp_ToolRefresh = new CBitmapButton();
//	mp_ToolRefresh->Create(NULL, dwStyle,CRect(0,30,24,54),this, IDC_TREE_REFRESH);
//	mp_ToolRefresh->LoadBitmaps(IDB_REFRESH, IDB_REFRESH, IDB_REFRESH, IDB_REFRESH);*/
//	m_SilverBrush.CreateSolidBrush(GLOBALBKCOLOR);
//	//Create Tree Control
////	bRet = m_TreeCtrl.Create(dwStyle, CRect(0,0,185,500), this, IDC_TREECTRL+1);
//
//	
//	int i_w=48, i_h=48, i_x=0, i_y=50, i_gap=90;
//    int nExtH = 25;
//	pMatrix = new CButtonST();
//	pSetting = new CButtonST();
//	pApplication = new CButtonST();
//	//pstMatrix = new CStatic();
//	//pstSetting = new CStatic();
//	//pstApp = new CStatic();
//	m_Font = this->GetFont();
//	
//	//m_Font.CreatePointFont(90, _T("宋體"));
//    
//    COLORREF clBG = GLOBALBKCOLOR;
//    int nHoveOver = (int)BTNST_AUTO_GRAY;
//	dwStyle = BS_PUSHBUTTON |BS_NOTIFY |WS_VISIBLE;
//	//Matrix Button
//	pMatrix->Create(NULL, dwStyle, CRect(i_x, i_y, i_x+i_w +10,i_y+48 +nExtH), this, IDC_TREECTRL+2);
//	pMatrix->SetBitmaps(IDB_LV3X3, clBG ,nHoveOver);
//	
//
//	CString	ls_text; //For icon text, load from string table
//	
//
//	//Matrix Static Control
//	LOADSTRING(ls_text, IDS_MAIN_MATRIX);
//	//ls_text.LoadString(CGS::g_hResStr,IDS_MAIN_MATRIX);
//    pMatrix->SetWindowText(ls_text);
//    pMatrix->SetAlign(CButtonST::ST_ALIGN_VERT);
//	pMatrix->SetFont(m_Font);
//	/*pstMatrix->Create(ls_text, dwStyle + SS_CENTER, CRect(i_x+5,  i_y + 50, i_x+47 +10,i_y+65 +10), this, STARTID + 3);
//	m_Font.CreatePointFont(90, _T("Arial Bold"));
//	pstMatrix->SetFont(&m_Font);*/
//	
//	//pMatrix->SetIndex(100);
//	i_y = i_y + i_gap;
//
//	int iAuth = ((CMainFrame*)AfxGetMainWnd())->m_iAuth;
//	switch(iAuth)
//	{
//	case LV_SET_APP:
//		
//		//Setting Button
//		pSetting->Create(NULL, dwStyle, CRect(i_x, i_y , i_x+i_w +10,i_y+i_h +nExtH), this, IDC_TREECTRL+3);//STARTID + 1);
//		pSetting->SetBitmaps(IDB_SETTING_NEW, clBG ,nHoveOver);
//
//		//pSetting->SetIndex(101);
//		LOADSTRING(ls_text, IDS_MAIN_SETTINGS);
//		//ls_text.LoadString(CGS::g_hResStr,IDS_MAIN_SETTINGS);
//		pSetting->SetWindowText(ls_text);
//		pSetting->SetAlign(CButtonST::ST_ALIGN_VERT);
//		pSetting->SetFont(m_Font);
//		//pstSetting->Create(ls_text, dwStyle + SS_CENTER, CRect(i_x,  i_y + 50, i_x+47 +10, i_y+65 +10), this, STARTID + 4);
//		////pstSetting->Create(_T("Settings"), dwStyle + SS_CENTER, CRect(i_x,  i_y + 50, i_x+47, i_y+65), this, STARTID + 4);
//		//pstSetting->SetFont(&m_Font);
//		i_y = i_y + i_gap;
//
//
//
//		//Application button
//		pApplication->Create(NULL, dwStyle, CRect(i_x, i_y , i_x+i_w +10,i_y+i_h +nExtH), this, IDC_TREECTRL+4);//STARTID + 2);
//		pApplication->SetBitmaps(IDB_APPNEW, clBG ,nHoveOver);
//		//pApplication->SetIndex(102);
//		LOADSTRING(ls_text, IDS_MAIN_APPL);
//		//ls_text.LoadString(CGS::g_hResStr,IDS_MAIN_APPL);
//		pApplication->SetWindowText(ls_text);
//		pApplication->SetAlign(CButtonST::ST_ALIGN_VERT);
//		pApplication->SetFont(m_Font);
//
//		break;
//	case LV_APP:
//
//		//Application button
//		pApplication->Create(NULL, dwStyle, CRect(i_x, i_y , i_x+i_w +10,i_y+i_h +nExtH), this, IDC_TREECTRL+4);//STARTID + 2);
//		pApplication->SetBitmaps(IDB_APPNEW, clBG ,nHoveOver);
//		//pApplication->SetIndex(102);
//		LOADSTRING(ls_text, IDS_MAIN_APPL);
//		//ls_text.LoadString(CGS::g_hResStr,IDS_MAIN_APPL);
//		pApplication->SetWindowText(ls_text);
//		pApplication->SetAlign(CButtonST::ST_ALIGN_VERT);
//		pApplication->SetFont(m_Font);
//
//		break;
//	case LV_SET:
//	
//		//Setting Button
//		pSetting->Create(NULL, dwStyle, CRect(i_x, i_y , i_x+i_w +10,i_y+i_h +nExtH), this, IDC_TREECTRL+3);//STARTID + 1);
//		pSetting->SetBitmaps(IDB_SETTING_NEW, clBG ,nHoveOver);
//
//		//pSetting->SetIndex(101);
//		LOADSTRING(ls_text, IDS_MAIN_SETTINGS);
//		//ls_text.LoadString(CGS::g_hResStr,IDS_MAIN_SETTINGS);
//		pSetting->SetWindowText(ls_text);
//		pSetting->SetAlign(CButtonST::ST_ALIGN_VERT);
//		pSetting->SetFont(m_Font);
//		//pstSetting->Create(ls_text, dwStyle + SS_CENTER, CRect(i_x,  i_y + 50, i_x+47 +10, i_y+65 +10), this, STARTID + 4);
//		////pstSetting->Create(_T("Settings"), dwStyle + SS_CENTER, CRect(i_x,  i_y + 50, i_x+47, i_y+65), this, STARTID + 4);
//		//pstSetting->SetFont(&m_Font);
//		i_y = i_y + i_gap;
//		break;
//	case LV_ONLY:
//		break;
//	default:
//		ASSERT(true);
//		break;
//	};
//
//	////Setting Button
//	//pSetting->Create(NULL, dwStyle, CRect(i_x, i_y , i_x+i_w +10,i_y+i_h +nExtH), this, IDC_TREECTRL+3);//STARTID + 1);
//	//pSetting->SetBitmaps(IDB_SETTING_NEW, clBG ,nHoveOver);
//
//	////pSetting->SetIndex(101);
//	//ls_text.LoadString(CGS::g_hResStr,IDS_MAIN_SETTINGS);
// //   pSetting->SetWindowText(ls_text);
// //   pSetting->SetAlign(CButtonST::ST_ALIGN_VERT);
//	//pSetting->SetFont(m_Font);
//	////pstSetting->Create(ls_text, dwStyle + SS_CENTER, CRect(i_x,  i_y + 50, i_x+47 +10, i_y+65 +10), this, STARTID + 4);
//	//////pstSetting->Create(_T("Settings"), dwStyle + SS_CENTER, CRect(i_x,  i_y + 50, i_x+47, i_y+65), this, STARTID + 4);
//	////pstSetting->SetFont(&m_Font);
//	//i_y = i_y + i_gap;
//
//
//
//	////Application button
//	//pApplication->Create(NULL, dwStyle, CRect(i_x, i_y , i_x+i_w +10,i_y+i_h +nExtH), this, IDC_TREECTRL+4);//STARTID + 2);
//	//pApplication->SetBitmaps(IDB_APPNEW, clBG ,nHoveOver);
//	////pApplication->SetIndex(102);
//	//ls_text.LoadString(CGS::g_hResStr,IDS_MAIN_APPL);
// //   pApplication->SetWindowText(ls_text);
// //   pApplication->SetAlign(CButtonST::ST_ALIGN_VERT);
//	//pApplication->SetFont(m_Font);
//
//	////pstApp->Create(ls_text, dwStyle + SS_CENTER, CRect(i_x+10,  i_y + 50, i_x+47, i_y+65), this, STARTID + 4);
//	////pstApp->SetFont(&m_Font);
//}
//
// 

void CIPCamTreeView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此加入您的訊息處理常式程式碼和 (或) 呼叫預設值
	//AfxMessageBox(_T("TEST"));
	//CSizingControlBarG::OnLButtonDown(nFlags, point);
}

//void CIPCamTreeView::OnBnClickedRefreshTree()
//{
//	vRefreshTree();
//}

//
//void CIPCamTreeView::OpenDialogTag(CString o_mode)
//{
// 
//	int lret;
//	if (o_mode == "Matrix")
//	{		
//		lret = m_MatrixDlg.DoModal();	
//		if (lret==2) 
//		{
//			return; //Escape key
//		}
//		
//		if (lret>0) //Click on one of the Matrix
//		{
//			if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
//			{
//				m_TreeCtrl.vFlyaway(lret);
//			}else
//			{
//				if(((CMainFrame*)AfxGetMainWnd())->m_bIPWallSelected == true)
//				{
//					m_TreeCtrl.vFlyaway(lret);
//					((CMainFrame*)AfxGetMainWnd())->m_bIPWallSelected = false;
//				}
//				else
//				{
//					((CMainFrame*)AfxGetMainWnd())->m_OrgLayoutIdx = lret;
//					 vLiveView();
//					 //bChangeBmp(lret);
//				}
//			}
//			 
//		}
//		else if (lret==0) // Full Screen
//		{
//			((CMainFrame*)AfxGetMainWnd())->RunFullScreen();
//		}
//	}
//	else if (o_mode == "Setting")
//	{		
//		lret = m_SettingDlg.DoModal();
//		vSettingManager(lret);
//	}
//	else if (o_mode == "App")
//	{
//		// return code 2 is reserved for 'ESC' key
//
//		lret = m_AppTagDlg.DoModal();
//		
//		if (lret == PLAYBACKID) //Playback
//		{
//			TCHAR apppath[MAX_PATH]={0};
//			GetModuleFileName(AfxGetInstanceHandle(),apppath,MAX_PATH-1);
//			CString csAppPath(apppath);
//	        int nPos = csAppPath.ReverseFind('\\');
//			csAppPath = csAppPath.Left(nPos + 1);
//
//			csAppPath += CGS::m_tzPlaybackName;
//			csAppPath += _T(".exe");
//
//			ShellExecute(NULL,_T("open"),csAppPath,NULL,NULL,SW_SHOWNORMAL);
//		}
//#ifdef PLUGIN
//		else if (lret == LPRID)//LPR		
//			//ShellExecute(NULL,_T("open"),_T("c:\\LPR\\lpr.exe"),NULL,NULL,SW_SHOWNORMAL);
//			((CMainFrame*)AfxGetMainWnd())->fnOpenLPR();
//		else if (lret==EMAPID) //Emap		
//			((CMainFrame*)AfxGetMainWnd())->vOpenEmapView();
//		else if (lret==ACID) //Access Control
//			((CMainFrame*)AfxGetMainWnd())->fnOpenAc();
//		else if (lret == GATEID)
//			((CMainFrame*)AfxGetMainWnd())->fnOpenGate();
//#endif
//			
//	}
//	else 
//		return;
//}
 

BOOL CIPCamTreeView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class

	return CDockablePane::OnNotify(wParam, lParam, pResult);
}

// Manage the settings - reRoute into different direction
int CIPCamTreeView::vSettingManager(int oSettingID)
{
	int ll_id = oSettingID;
	int DlgTemp;

	//CDlgAddManageCam *dlg = new CDlgAddManageCam;
		 NODEITEM aNode;
		 CDlgAddManageCam dlg(0);	
		  //CDlgSettingSystem *lSysSettingDlg = new CDlgSettingSystem;

	switch (ll_id)
	{
		//case SYSTEMSETTINGID:
		//	{
		//		//lSysSettingDlg->DoModal();
		//		break;
		//	}

		case ASSIGNIPID:
			{
			CDlgMassAssignIP dlg;
			dlg.DoModal();
			vRefreshTree();
			break;
			}

		case ADDCAMERAID:
			{
 
		  ::ZeroMemory(&aNode, sizeof(NODEITEM));
 

		  //IDD_DLG_ADDMANCAMERA
		  DlgTemp = dlg.DoModal();
			//if(DlgTemp == IDOK)
			//{
			//	pDoc->vAddInternetIpcam(&aNode);
			//}
			if(DlgTemp == IDCANCEL)
			{
				dlg.ScanMap.clear();				
			} 
			//NODEITEM aNode;// = new NODEITEM;
			//::ZeroMemory(&aNode, sizeof(NODEITEM));
			////CDlgAddInternetIpcam dlg(&aNode);

			////IDD_DLG_ADDMANCAMERA
			//dlg->DoModal();
			//pDoc->vAddInternetIpcam(&aNode);
			break;
			}

		case CAMERAGROUPID:
			{
			CDlgSettingCamGroup *dlg1 = new CDlgSettingCamGroup;
			dlg1->DoModal();
			delete dlg1;
			break;
			}

		case EVENTID:
			{
			

			/*DlgSettingEvent *eventDlg = new DlgSettingEvent;
			eventDlg->DoModal();
			delete eventDlg;
			break;*/
			}

		case RECORDING:
			{
			CDlgSettingRecording *RecordingDlg = new CDlgSettingRecording;
			RecordingDlg->DoModal();
			delete RecordingDlg;
			break;
			}
		
		case MULMONID:
			{
			CDlgSettingMulMonitor *ldlg  = new CDlgSettingMulMonitor;
			ldlg->DoModal();
			delete ldlg;
			break;
			}

		case SYSTEMSETTINGID:
			{
				CDlgFtp *ldlg  = new CDlgFtp;
				ldlg->DoModal();
				delete ldlg;
				break;
			}
#ifdef PLUGIN
		case ACCESSID:
			{
				CDlgSettingAC *ldlg  = new CDlgSettingAC;
				
				//Needs to refresh the 
				if (ldlg->DoModal() == IDOK)
				{
					if (!((CMainFrame*)AfxGetMainWnd())->m_ac) 
					{					
					}
					else{
						CDlgAc* lpAc = ((CMainFrame*)AfxGetMainWnd())->m_ac;

						if (ldlg->mb_camChanged)		//Original Codes
 							lpAc->fnGroupChanged();
	
						if (ldlg->mb_holChanged)		//Rebuild holiday structure
							lpAc->fnRefreshHolVec();

						if (ldlg->mb_priChanged)	//Refresh vector of work hour and privilege
							lpAc->fnRefreshPriVec();
					}
				} // 

				delete ldlg;
				break;
			}

		case PGATEID:
		{
			CDlgSettingGate *ldlg  = new CDlgSettingGate;
			
			//Needs to refresh the 
			if (ldlg->DoModal() == IDOK)
			{
			}
			delete ldlg;
			break;
		}

#endif
		case EVENTACTION:
		{
			/*CDlgSettingEventAction* lDlg = new CDlgSettingEventAction;
			if(lDlg->DoModal() == IDOK)
			{
			}
			delete lDlg;
			break;*/
		}

		case NAS:
		{
			CDlgNAS* lDlg = new CDlgNAS;
			if(lDlg->DoModal() == IDOK)
			{
			    //auto delete self ,don't delete again
			}else
            {
            }
			    delete lDlg;
			break;
		}

	}

	return 0;
}

bool CIPCamTreeView::vRefreshTree(void)
{
	m_TreeCtrl.DeleteAllItems();
	m_TreeCtrl.InsertDBRoot();
	
	HTREEITEM hRoot = m_TreeCtrl.GetRootItem();
	while(hRoot)
	{
		m_TreeCtrl.Expand(hRoot, TVE_EXPAND);
		hRoot = m_TreeCtrl.GetNextSiblingItem(hRoot);
	}	

	return true;
}

//bool CIPCamTreeView::bChangeBmp(int o_Matrix)
//{
//    COLORREF clBG = GLOBALBKCOLOR;
//    int nHoveOver = (int)BTNST_AUTO_GRAY;
//    int nBmpID;
// 	switch (o_Matrix)
//	{
//	case 1:
//	//	pMatrix->SetBitmap(myBmp1);
//		    nBmpID = IDB_LV1X1;
//			break;
//		case 22:
//			nBmpID = IDB_LV2X2;
//			break;
//		case 12:
//			nBmpID = IDB_EVE3;
//			break;
//		case 3:
//			nBmpID = IDB_LV3X3;
//			break;						
//		case 4:
//			nBmpID = IDB_LV4X4;
//			break;						
//		case 15:
//			nBmpID = IDB_LV1P5;
//			break;	
//		case 115:
//			nBmpID = IDB_EVE5;
//			break;
//		case 28:
//			nBmpID = IDB_LV2P8;
//			break;
//		case 128:
//			nBmpID = IDB_EVE9;
//			break;
//		case 121:
//			nBmpID = IDB_LV1P12;
//			break;
//		case 122:
//			nBmpID = IDB_LV1P12_2;
//			break;
//		case 1122:
//			nBmpID = IDB_EVE12;
//			break;
//		case 116:
//			nBmpID = IDB_LV1P16;
//			break;
//		case 1116:
//			nBmpID = IDB_EVE16;
//			break;
//		case 6:
//			nBmpID = IDB_LV6X6;
//			break;
//		case 7:
//			nBmpID = IDB_LV7X7;
//			break;
//		case 8:
//			nBmpID = IDB_LV8X8;
//			break;		
//		case 218:
//			nBmpID = IDB_LV2P18;
//			break;
//		case 232:
//			nBmpID = IDB_LV2P32;
//			break;
//        default:
//            ASSERT(false);
//
//	}
//    pMatrix->SetBitmaps(nBmpID, clBG ,nHoveOver);
//	pMatrix->SetRedraw(true);
//	pMatrix->Invalidate(false);
//	return true;
//}

LRESULT CIPCamTreeView::OnChangeBMP(WPARAM wParam, LPARAM lParam) 
{

	//bChangeBmp(wParam);
	return 0;
}
//void CIPCamTreeView::OnRangeCmds(UINT nID)
//{
//    switch (nID)
//    {
//    case IDC_TREECTRL+2:
//        OpenDialogTag(_T("Matrix"));
//	    break;
//    case IDC_TREECTRL+3:
//        OpenDialogTag(_T("Setting"));
//        break;
//    case IDC_TREECTRL+4:
//        OpenDialogTag(_T("App"));
//	    break;
//    default:
//        ASSERT(false);
//    }
//}
//