// EtroExplorerView.cpp : implementation of the CEtroExplorerView class
//

#include "stdafx.h"
#include "NVR.h"
//#include "AudioRender.h"
#include "EtroExplorerDoc.h"
#include "EtroExplorerView.h"
#include "Video.h"
#include "MainFrm.h"
#include <math.h>
#include "INI.h"
#include "Wizard.h"
#include "Winsvc.h"

#define BORDERCOLOR RGB(100,100,100)
//#define BKGNDCOLOR RGB(0,0,0)
#define FOCUSCOLOR RGB(231, 41, 15)

//#include "NVSNetLib.h"
//#include <NVSADPCMLib.h>
//#pragma comment(lib, "Version.lib")
//#pragma comment(lib, "ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DATABASE_FOLDER		_T("C:\\database\\")
#define DATABASE_EXT_NAME	_T("bak")
#define REG_EXT_NAME	_T("reg")

/////////////////////////////////////////////////////////////////////////////
// CEtroExplorerView

#define IDC_ZOOM_WINDOWS                32778
#define IDC_SCREEN                      1005 

IMPLEMENT_DYNCREATE(CEtroExplorerView, CView)

BEGIN_MESSAGE_MAP(CEtroExplorerView, CView)
	//{{AFX_MSG_MAP(CEtroExplorerView)
	ON_COMMAND(IDC_ZOOM_WINDOWS, OnZoomWindows)
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_LBUTTONUP()
	ON_WM_DROPFILES()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(ID_SET_OSD,vSetOSD)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CEtroExplorerView construction/destruction

CEtroExplorerView::CEtroExplorerView()
: nCamIndex(0), nFocusVideo(0)
, ib_SingleDisplay(false),m_nLayout(0) /* for OnSize adjustment	*/,m_bDragging(false),m_bOutOfRange(false),m_bDisplayOSD(true),m_pDragImage(NULL),m_bSwap(false),m_bOnFlyAway(false),m_pLiveView(NULL)

{
}

CEtroExplorerView::~CEtroExplorerView()
{
	vFreeVideoSet();
	m_videos.clear();
	m_vctpNode.clear();
	DELETEOBJ(m_pLiveView);
}

BOOL CEtroExplorerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	/*CMainFrame *pFrame=(CMainFrame*)GetParent();
	pFrame->SetAppName((LPCTSTR)_T("New Application Name"));
	pFrame->OnUpdateFrameTitle(TRUE);*/
	//cs.style = 
	m_GlobalScreenW = 0;
	return CView::PreCreateWindow(cs);
}

void CEtroExplorerView::OnDraw(CDC* pDC)
{	
	if(m_pLiveView)
		m_pLiveView->OnPaint();
}

void CEtroExplorerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ShowWindow(SW_MAXIMIZE);	

	CRect rect;
	GetClientRect(&rect);

	m_pLiveView = new CLiveView ;
	m_pLiveView->Create(NULL,WS_CHILD,rect,this,NULL);
	m_pLiveView->SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_SHOWWINDOW);
	m_pLiveView->m_bOutOfRange =&m_bOutOfRange;
	CNVRBk::GetBkObject()->RegistWnd(m_pLiveView);

	// set grids of matrix according to the previous form stored in INI file 
	m_ini.setINIFullFileName(_T("\ec.ini"));

	int nCellSize=0;
	CString csCellSize = m_ini.getKeyValue(_T("matrix"),_T("video"));
	if(csCellSize.IsEmpty()) nCellSize =3;
	else	nCellSize = _ttoi(csCellSize);

	m_rects.clear();
	m_pLiveView->vSetGrids(nCellSize);
	m_rects=m_pLiveView->m_rects;
	
	CString csEventLV = m_ini.getKeyValue(_T("EventLV"),_T("video"));
	if( _ttoi(csEventLV) == 1) 
	{
		((CMainFrame*)AfxGetMainWnd())->m_bEventLV = true;
	}
	
	m_gs.bGetNodeItem();
	if(!m_gs.m_vctNode.empty())
	{
		vector <NODEITEM*> vctpNode;
		vector <NODEITEM*> vctpCurrentNode;
		
		for(int i=0;i<m_gs.m_vctNode.size();i++)
		{
			vctpNode.push_back(&(m_gs.m_vctNode[i]));
		}
		// get current ALL nodeitems from TreeCtrl
		((CTaskTreeCtrl*)((CMainFrame*)AfxGetMainWnd())->GetTreeView())->vGetAllNodeItems(vctpCurrentNode);
		
		if(!vctpCurrentNode.empty())
		{
			for(int i=0;i<vctpNode.size();i++)
			{
				for(int j=0;j<vctpCurrentNode.size();j++)
				{							
					if(vctpNode[i]->camera_idx == vctpCurrentNode[j]->camera_idx)
					{
						//vctpNode[i]->hTreeItem = vctpCurrentNode[j]->hTreeItem;
						vctpNode[i] = vctpCurrentNode[j];
					}
				}
			}

			//vctpNode.push_back(vctpCurrentNode[0]);
			// select on the first video's group or first video
			if(vctpNode[0]->hTreeItem)
			{
				((CTaskTreeCtrl*)((CMainFrame*)AfxGetMainWnd())->GetTreeView())->SelectItem(vctpNode[0]->hTreeItem);
			}
			else
			{
				HTREEITEM hParent = ((CTaskTreeCtrl*)((CMainFrame*)AfxGetMainWnd())->GetTreeView())->GetParentItem(vctpNode[0]->hTreeItem);
				if( hParent != NULL)
					((CTaskTreeCtrl*)((CMainFrame*)AfxGetMainWnd())->GetTreeView())->SelectItem(hParent);
			}
		}
		m_pLiveView->vAttach(vctpNode); 
	}
	m_dropTarget.Register(this);
    ((CMainFrame*)AfxGetMainWnd())->vSetStartMonitor(); // set mainfrm startup monitor if monitor more than 1 		
}

/////////////////////////////////////////////////////////////////////////////
// CEtroExplorerView diagnostics

#ifdef _DEBUG
void CEtroExplorerView::AssertValid() const
{
	CView::AssertValid();
}

void CEtroExplorerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEtroExplorerDoc* CEtroExplorerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEtroExplorerDoc)));
	return (CEtroExplorerDoc*)m_pDocument;
}
#endif //_DEBUG


void CEtroExplorerView::vAttach(vector <NODEITEM*> pNode)
{
 	m_pLiveView->vAttach(pNode);
}

void CEtroExplorerView::vNewVideoSet(NODEITEM* pNode)
{
	m_pLiveView->vNewVideoSet(pNode);

}
bool CEtroExplorerView::bPresentIPCam(char *szHostName, WORD nPort, char *szUserID, char *szUserPassword)
{
	return true;
}

void CEtroExplorerView::vSetGrids(int nGrid)
{ 	
	m_rects.clear();
	m_pLiveView->vSetGrids(nGrid);
	m_rects=m_pLiveView->m_rects;
	((CMainFrame*)AfxGetMainWnd())->vSetFocusVideoSet((CNetSession*)NULL, NULL);	
}

void CEtroExplorerView::vFreeVideoSet()
{
	m_pLiveView->vFreeVideoSet();	
}

void CEtroExplorerView::vFreeVideoSet(int o_idx)
{
	// delete the choosen video 
	if(m_videos.size() == 0)
		return;

	int nSize = m_videos.size();
	
	if( o_idx <= nSize && o_idx>0 && m_videos[o_idx]!=NULL)
	{
				/*m_videos[o_idx]->MoveWindow(CRect(0,0,0,0));*/
				m_videos[o_idx]->vStopPlay();
				m_videos[o_idx]->vStopRecv();		
				m_videos[o_idx]->DestroyWindow();
				delete m_videos[o_idx];
				m_videos[o_idx] = NULL;
				nCamIndex--;
				Invalidate(true);			
	}
	
}
CRect CEtroExplorerView::bGetRect()
{
	CRect rect;
	GetClientRect(rect);
	return rect;
}
void CEtroExplorerView::vScreenToViewClientRect(POINT *pt)
{	
	ScreenToClient(pt);
}
void CEtroExplorerView::vSetWindowSize(int nWidth, int nHeight)
{
	RECT rect;
	this->GetWindowRect(&rect);
	rect.right = rect.right*2;
	rect.bottom = rect.bottom*2;
	this->MoveWindow(&rect, true);
//	GetParentFrame()->RecalcLayout();
//	SIZE size={rect.right, rect.bottom};
//	SetScrollSizes(MM_TEXT, size);
//	ResizeParentToFit(true);
}

void CEtroExplorerView::OnZoomWindows() 
{
	vSetWindowSize(0,0);	
}

void CEtroExplorerView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	//ASSERT(pWnd == this);	
	m_pLiveView->OnContextMenu(pWnd,point);		
}

BOOL CEtroExplorerView::PreTranslateMessage(MSG* pMsg) 
{	
	if(m_pLiveView) m_pLiveView->PreTranslateMessage(pMsg);
	return CView::PreTranslateMessage(pMsg);
}

BOOL CEtroExplorerView::OnEraseBkgnd(CDC* pDC) 
{
	pDC->SetBkColor(GLOBALFONTCOLOR);//BKCOLOR
	CRect rect;
	GetClientRect(rect);
	CBrush bkColor(GLOBALFONTCOLOR);
	pDC->FillRect(&rect,&bkColor);
	//CGS::GradientFillRect(pDC, rect, BKCOLOR, RGB(0,0, 0), GRADIENT_FILL_RECT_V);
	return true;
}

void CEtroExplorerView::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	m_pLiveView->OnLButtonDown(nFlags,point);
}

void CEtroExplorerView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_pLiveView->OnLButtonUp(nFlags,point);
	
}

void CEtroExplorerView::OnSize(UINT nType, int cx, int cy)
{
	if(m_pLiveView)
	{
	   m_pLiveView->OnSize(nType,cx,cy);
	}
}

void CEtroExplorerView::vIsDragging(bool m_bLDragging)
{	
	m_pLiveView->m_bDragging = m_bLDragging;	

}
bool CEtroExplorerView::bIsOutOfRange(int nNodeCount)
{
	int nVideo=m_pLiveView->m_videos.size();
	int nRect=m_pLiveView->m_rects.size();

	int nVideoCheck=nVideo;
	for (int n=0;n<nVideoCheck;n++)
	{
		if (m_pLiveView->m_videos[n] == NULL)
		{
			nVideo--;
		}
	}


	//if(m_pLiveView->m_videos.size() + nNodeCount > m_pLiveView->m_rects.size() || m_pLiveView->m_videos.size() + nNodeCount >  MAXIPCAMCOUNT)
	if (nVideo + nNodeCount > nRect || nVideo+ nNodeCount >  MAXIPCAMCOUNT)
		m_bOutOfRange = true;

	return m_bOutOfRange;
}
LRESULT CEtroExplorerView::vSetOSD(WPARAM wParam, LPARAM lParam)
{
	bool bOSD = wParam;
	m_pLiveView->m_bDisplayOSD = bOSD;
	
	for(int ci=0; ci < m_pLiveView->m_videos.size(); ci++)
	{
		m_pLiveView->m_videos[ci]->bResetOSD(wParam);
			
	}

	return 0;
}

void CEtroExplorerView::DoDataExchange(CDataExchange* pDX)
{
	// TODO: Add your specialized code here and/or call the base class

	CView::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_MEDIACTRL1, m_mediaCtrl);
}

void CEtroExplorerView::OnMouseMove(UINT nFlags, CPoint point)
{	
	if(m_pLiveView)
		m_pLiveView->OnMouseMove(nFlags,point);	
}
void CEtroExplorerView::vGetCurrentGroupNodeItems(vector<NODEITEM*> &vctpNode)
{
	vector <NODEITEM*> vctpCurrentNode;
	
	((CTaskTreeCtrl*)((CMainFrame*)AfxGetMainWnd())->GetTreeView())->vGetGroupNodeItems(vctpCurrentNode);
	
	vctpNode.clear();
	for(int i = 0; i<vctpCurrentNode.size();i++)
	{
		vctpNode.push_back(vctpCurrentNode[i]);
	}
}

void CEtroExplorerView::vStartWizard()
{
	int nCams =  ((CTaskTreeCtrl*)((CMainFrame*)AfxGetMainWnd())->GetTreeView())->m_vctpAllNodesInTree.size();
	bool bEnable = ((CMainFrame*)AfxGetMainWnd())->m_bWizardEnable;
	HANDLE handle = GetCurrentProcess();
	if( 0 == nCams && bEnable)
	{
		HANDLE handle = GetCurrentProcess();
		if (!CheckBackupFile())
		{
			CString csMsg;
		/*	LOADSTRING(csMsg,IDS_WIZARD_ENABLE);
			if( IDYES == AfxMessageBox(csMsg,MB_YESNO))*/
			{
			
				CWizard dlg(_T(""), this, 0);
				if(ID_WIZFINISH == dlg.DoModal())
				{					
					//TRACE(_T("finish"));
					CString csWiz(_T("0"));
					if(((CMainFrame*)AfxGetMainWnd())->m_bWizardEnable)
					{	
						csWiz = _T("1");		
					}
					m_ini.setKey(csWiz,_T("EnableWizard"),_T("system"));
				}
			}
		}
		else
		{
			//Terminate process than use health server to restart process --by lynn
			HANDLE handle = GetCurrentProcess();
			TerminateProcess(handle, 0);
		}
	}
}
void  CEtroExplorerView::vResetOSD(void)
{
	for(int i = 0;i <m_pLiveView->m_videos.size();i++)
	{
		if(m_pLiveView->m_videos[i])
		{
			m_pLiveView->m_videos[i]->m_pOSD->vResetOSD();
		}
	}

}

//////////////////////////////////////////////////////////////////////////For imp/exp database by Lynn
void CEtroExplorerView::GetDatabaseFileName(CString& strFileName)
{
	if (CNVRBk::GetBkObject()->GetOEMCompany() == Etro_Company)
		strFileName = _T("etrocenter");
	else
		strFileName = _T("cms");
}

bool CEtroExplorerView::CheckBackupFile()
{
	bool bResult = false;
	int nResult = IDYES;
	CString	strFileBakPath(_T("")), strFileName(_T("")),strFileRegPath(_T("")),strMsg(_T(""));
	DWORD dwBakFile = INVALID_FILE_ATTRIBUTES, dwRegFile = INVALID_FILE_ATTRIBUTES;

	GetDatabaseFileName(strFileName);
	strFileBakPath.Format(_T("%s%s.%s"), DATABASE_FOLDER, strFileName, DATABASE_EXT_NAME);
	strFileRegPath.Format(_T("%s%s.%s"), DATABASE_FOLDER, strFileName, REG_EXT_NAME);

	if ((dwBakFile = GetFileAttributes(strFileBakPath)) != INVALID_FILE_ATTRIBUTES)
	{
		if ((dwRegFile = GetFileAttributes(strFileRegPath)) != INVALID_FILE_ATTRIBUTES)
			strMsg.Format(_T("Find out files \"%s.%s\" and \"%s.%s\".\nDo you want to import them?"), strFileName, DATABASE_EXT_NAME, strFileName, REG_EXT_NAME);
		else
			strMsg.Format(_T("Find out the file \"%s.%s\".\nDo you want to import it?"), strFileName, DATABASE_EXT_NAME);
		nResult = AfxMessageBox(strMsg, MB_YESNO);
	}

	if (nResult == IDYES)
	{
		if (dwBakFile != INVALID_FILE_ATTRIBUTES || dwRegFile!= INVALID_FILE_ATTRIBUTES)
		{
			if(dwBakFile != INVALID_FILE_ATTRIBUTES)
				ImportReg(strFileRegPath);
			if(dwRegFile != INVALID_FILE_ATTRIBUTES)
				ImportDatabase(strFileBakPath);
			AfxMessageBox(_T("Restart process, please wait!"),MB_ICONEXCLAMATION);
			((CMainFrame*)AfxGetMainWnd())->m_bRestartByHealth = true;
			::PostMessage(((CMainFrame*)AfxGetMainWnd())->m_hWnd,WM_CLOSE,NULL,NULL);
			bResult = true;
		}
	}

	return bResult;
}

void CEtroExplorerView::ImportDatabase(const CString& strFilePath)
{
	char str[128];
	CString strFileName(_T(""));

	sprintf(str, "net stop mssql$sqlexpress /Y");
	system(str);
	sprintf(str, "net start mssql$sqlexpress /Y");
	system(str);

	USES_CONVERSION;
	GetDatabaseFileName(strFileName);
	sprintf(str, "SQLCMD -E -S (local)\\SQLEXPRESS  -Q \"RESTORE DATABASE %s FROM DISK='%s' WITH REPLACE\"", W2A(strFileName), W2A(strFilePath));
	system(str);

	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::EVENT));
	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::RECORD));
	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::STREAM));
	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::HEALTH));
	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::REMOTE));
}

void CEtroExplorerView::ImportReg(const CString& strFilePath)
{
	char str[128];

	USES_CONVERSION;
	sprintf(str, "regedit.exe /s %s", W2A(strFilePath));
	system(str);

	RestartService(CNVRBk::GetBkObject()->GetServicesName(enumServiceType::HEALTH));
}

void CEtroExplorerView::RestartService(const CString& str)
{
	bool bCloseSCManager = false;
	SC_HANDLE schSCManager = OpenSCManager( NULL, NULL, SC_MANAGER_ALL_ACCESS); 
	if (schSCManager!=0) 
	{
		SC_HANDLE schService = OpenService( schSCManager, str, SERVICE_ALL_ACCESS);
		bool bCloseService = false;
		if (schService!=0) 
		{	
			SERVICE_STATUS status;

			int nRetryStart = 5;
			int nRetryQuery = 3;

			while(nRetryQuery--)
			{			
				SERVICE_STATUS ss;
				if(QueryServiceStatus(schService, &ss))
				{
					if(ss.dwCurrentState == SERVICE_STOPPED)
					{
						nRetryQuery = 0;							
					}
				}
			}

			while(nRetryStart--)
			{
				int nRet= StartService(schService, 0, (const TCHAR**)NULL);
				if( nRet == S_OK)
				{						
					CloseServiceHandle(schService);
					CloseServiceHandle(schSCManager); 

					bCloseService = true;
					bCloseSCManager = true;
					nRetryStart = 0;
				}
			}
		}
		if(!bCloseService)
			CloseServiceHandle(schService); 
	}

	if(!bCloseSCManager)
		CloseServiceHandle(schSCManager);
}