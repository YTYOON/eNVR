  // IPCamTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
//#include "EtroLiveview.h"
#include "IPCamTreeCtrl.h"
#include <afxinet.h>
#include "DlgAuthorizationSetting.h"
//#include "DlgGroupName.h"
#include "DlgSettingCamGroup_Ext.h"
#include "MainFrm.h"
#include "CGIVar.h"
#include "EncpSettingPage.h"
#include "DlgProductTest.h"
#include "DlgAddManageCam.h"
//#include "DlgSettingEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#define TREEBKCOLOR RGB(255,255,250)

/////////////////////////////////////////////////////////////////////////////
// CIPCamTreeCtrl

CIPCamTreeCtrl::CIPCamTreeCtrl():m_pDragImage(NULL),m_bLDragging(false),m_hitemDrag(NULL),m_hitemDrop(NULL),m_bPtInRect(false),
bReEntry(false)
, isPrvGroupName(_T(""))
, ibDiffGroup(false),m_bRefreshVideo(false),m_bOnlyOneCamChoosen(false),m_nDropRectIdx(-1),m_bFlyAway(false)
,iCurGroupItem(NULL), iCurTreeItem(NULL), m_pDB(NULL)
{
//	vInitCgiInfo();
	hDlgInitialize = ::CreateEvent(NULL, TRUE, FALSE, _T(""));
//	hCloseDlg = ::CreateEvent(NULL, TRUE, FALSE, _T("")); 
	hAllThreadClosed = ::CreateEvent(NULL, TRUE, FALSE, _T(""));
	m_rectTreeCtrl.clear();
}

CIPCamTreeCtrl::~CIPCamTreeCtrl()
{
	::CloseHandle(hDlgInitialize);
	/*CGS gs;
	gs.bSetKeepInfo();*/

	for(int i=0; i<m_vctpAllNodesInTree.size();i++)
	{
		if(m_vctpAllNodesInTree[i] = NULL)	continue;
		delete m_vctpAllNodesInTree[i];
	}
	m_vctpAllNodesInTree.clear();
	
	for(int i=0; i<m_vctDlgFlyAwayRecycleBin.size();i++)
	{
		if(m_vctDlgFlyAwayRecycleBin[i] = NULL)	continue;
		delete m_vctDlgFlyAwayRecycleBin[i];
	}
	m_vctDlgFlyAwayRecycleBin.clear();
	
}


BEGIN_MESSAGE_MAP(CIPCamTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CIPCamTreeCtrl)
	ON_WM_CREATE()
	ON_COMMAND(IDC_REFRESH, OnRefresh)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDC_SET_AUTHORIZATION_INFO, OnSetAuthorizationInfo)
	ON_COMMAND(IDC_TREECTRL_NEW_GROUP, OnTreectrlNewGroup)
	ON_COMMAND(IDC_TREECTRL_MODIFY_GROUP, OnTreectrlModifyGroup)
	//ON_COMMAND(ID_MENU_EVENTSETTING, OnEventSetting)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_DESTROY()
	ON_COMMAND(IDC_IE_EXPLORER, OnIeExplorer)
	//ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)  //
	ON_WM_MOUSEMOVE() //
	ON_WM_LBUTTONUP() //
	ON_WM_LBUTTONDOWN() //
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_COMMAND(IDC_TREECTRL_REMOVE_GROUP, OnTreectrlRemoveGroup)
	ON_WM_ERASEBKGND()
	ON_COMMAND(IDC_LIVE_VIEW, OnLiveView)
 
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	
	//Resume Initilize Config.
	ON_COMMAND(IDC_FIRSTTIME_INITIALIZE, vInitCgiInfo)

	//Modify  IPCAM
	ON_COMMAND(ID_MODIFY_IPCAM, OnFirsttimeInitialize)
	
	ON_COMMAND(ID_ADD_INTERNET_IP_CAM, OnAddInternetIpCam)
	ON_COMMAND(ID_UPGRADE_FIRMWARE, OnUpgradeFirmware)
	//ON_COMMAND_RANGE(ID_SET_REBOOT, ID_SET_TURN_LED_OFF, vEnhanceFunc)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_ADD_NEW_IPCAM, &CIPCamTreeCtrl::OnAddNewIpcam)
	ON_COMMAND(ID_MENU_REMOVEIPCAM32815, &CIPCamTreeCtrl::OnMenuRemoveipcam32815)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT,OnTvnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CIPCamTreeCtrl::OnTvnBegindrag)
	//ON_COMMAND(ID_MENU_FLYAWAY, OnMenuFlyaway)
	//ON_COMMAND(ID_MENU_ANPR, &CIPCamTreeCtrl::vANPR)
	
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CIPCamTreeCtrl message handlers
void CIPCamTreeCtrl::vInitImages()
{
	//Initilize Images into the list...
	int nResult(0);
	m_TreeImageList.Create(16, 16, 0, 5, 5);
	m_TreeImageList.SetBkColor(GLOBALTREEBKCOLOR);//m_TreeImageList.GetBkColor());

	//Folder Icon - Group First Create Image List
	nResult = m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_FOLDER));
	nResult = m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_CAMRED));
	nResult = m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_CAMGRE));
	nResult = m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_CAMYEL));
	nResult = m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_CAM16));
	
	SetImageList(&m_TreeImageList,TVSIL_NORMAL);
		
	CDC *pDC = GetDC();
	ASSERT(pDC);
	COLORREF crWindow;
 
	ReleaseDC(pDC);

	SetImageList(&m_StateImage, TVSIL_STATE);
}


int CIPCamTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	//Avoid Create Error
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1) 	return -1;	

	LONG nStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	nStyle = nStyle | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | WS_BORDER | TVS_SHOWSELALWAYS;
	SetWindowLong(m_hWnd, GWL_STYLE, nStyle);

	//vDBConnect();
    m_pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
    if (m_pDB->m_hdbc == NULL)
        return -1;
 
	//--Make sure every camera has a LV.
	vCheckLVExist();
	
	vInitImages();
	SetDefaultCursor();
	InsertDBRoot();
	//InsertDefaultRoot();

	::SetEvent( hDlgInitialize );

	SetTextColor(GLOBALFONTCOLOR);
	SetBkColor(GLOBALTREEBKCOLOR);

	return 0;
}


//Initilize IPCAM Configuration ...
void CIPCamTreeCtrl::vInitCgiInfo()
{
	int ci =-1;
	//CString ls_confirm=_T("Are you sure to initilize the IPCAM configuration?");
	//int ll_ret = AfxMessageBox(ID_TREECTRL_CONFIRM, MB_OKCANCEL|MB_ICONQUESTION);
	//if (ll_ret==IDCANCEL) return;


	cgiinfo[++ci].csTitle = _T("Video Stream 1");	cgiinfo[ci].csURL = _T("");
	cgiinfo[++ci].csTitle = _T("Video Stream 2");	cgiinfo[ci].csURL = _T("");

	cgiinfo[++ci].csTitle = _T("Device Information");	cgiinfo[ci].csURL = _T("");
	cgiinfo[++ci].csTitle = _T("Basic Information");	cgiinfo[ci].csURL = _T("common/info.cgi");
	cgiinfo[++ci].csTitle = _T("Extended Information");	cgiinfo[ci].csURL = _T("config/info_ex.cgi");
	cgiinfo[++ci].csTitle = _T("Verify User");      	cgiinfo[ci].csURL = _T("users/verify.cgi");
	cgiinfo[++ci].csTitle = _T("Camera Info");	        cgiinfo[ci].csURL = _T("config/camera_info.cgi");
	cgiinfo[++ci].csTitle = _T("System Date and Time"); cgiinfo[ci].csURL = _T("config/datetime.cgi");
	
	cgiinfo[++ci].csTitle = _T("Users and Groups");     cgiinfo[ci].csURL = _T("");
	cgiinfo[++ci].csTitle = _T("Users");                cgiinfo[ci].csURL = _T("config/user_list.cgi");
	cgiinfo[++ci].csTitle = _T("Supported Privileges"); cgiinfo[ci].csURL = _T("config/privilege_info.cgi");

	cgiinfo[++ci].csTitle = _T("Video, Sensor, Audio"); cgiinfo[ci].csURL = _T("");
	cgiinfo[++ci].csTitle = _T("Stream Information");   cgiinfo[ci].csURL = _T("config/stream_info.cgi");
	cgiinfo[++ci].csTitle = _T("Video Config 1");       cgiinfo[ci].csURL = _T("config/video.cgi?profileid=1");
	cgiinfo[++ci].csTitle = _T("Video Config 2");       cgiinfo[ci].csURL = _T("config/video.cgi?profileid=2");
	cgiinfo[++ci].csTitle = _T("Sensors Information");  cgiinfo[ci].csURL = _T("config/sensor_info.cgi");
	cgiinfo[++ci].csTitle = _T("Sensors Config");       cgiinfo[ci].csURL = _T("config/sensor.cgi");
	cgiinfo[++ci].csTitle = _T("Audio Config 1");       cgiinfo[ci].csURL = _T("config/audio.cgi?profileid=1");
	cgiinfo[++ci].csTitle = _T("Audio Config 2");       cgiinfo[ci].csURL = _T("config/audio.cgi?profileid=2");

	cgiinfo[++ci].csTitle = _T("Microphone");           cgiinfo[ci].csURL = _T("config/mic.cgi");
	cgiinfo[++ci].csTitle = _T("Speaker");              cgiinfo[ci].csURL = _T("config/speaker.cgi");

	cgiinfo[++ci].csTitle = _T("Network");              cgiinfo[ci].csURL = _T("");
	cgiinfo[++ci].csTitle = _T("Network Config");       cgiinfo[ci].csURL = _T("config/network.cgi");
	cgiinfo[++ci].csTitle = _T("PPPoE");                cgiinfo[ci].csURL = _T("config/pppoe.cgi");
	cgiinfo[++ci].csTitle = _T("DDNS Providers");       cgiinfo[ci].csURL = _T("config/ddnsproviders.cgi");
	cgiinfo[++ci].csTitle = _T("DDNS Settings");        cgiinfo[ci].csURL = _T("config/ddns.cgi");
	cgiinfo[++ci].csTitle = _T("UPnP Information");     cgiinfo[ci].csURL = _T("config/upnp.cgi");
	cgiinfo[++ci].csTitle = _T("TCP Port Number for HTTP");cgiinfo[ci].csURL = _T("config/httpport.cgi");

	cgiinfo[++ci].csTitle = _T("Network Security");     cgiinfo[ci].csURL = _T("");
	cgiinfo[++ci].csTitle = _T("Security Status");      cgiinfo[ci].csURL = _T("config/security.cgi");
	cgiinfo[++ci].csTitle = _T("Security Level");       cgiinfo[ci].csURL = _T("config/security_level.cgi");
	cgiinfo[++ci].csTitle = _T("Security Rule");        cgiinfo[ci].csURL = _T("config/security_rule.cgi");
	                                                    
	cgiinfo[++ci].csTitle = _T("Event Handling");       cgiinfo[ci].csURL = _T("");
	cgiinfo[++ci].csTitle = _T("Motion Detection");     cgiinfo[ci].csURL = _T("config/motion.cgi");
	cgiinfo[++ci].csTitle = _T("Digital Input");        cgiinfo[ci].csURL = _T("config/event_input.cgi");
	cgiinfo[++ci].csTitle = _T("Video Loss Time");      cgiinfo[ci].csURL = _T("config/event_vloss.cgi");
	cgiinfo[++ci].csTitle = _T("Event Handlers Information");cgiinfo[ci].csURL = _T("config/event_info.cgi");  
	cgiinfo[++ci].csTitle = _T("FTP Action");           cgiinfo[ci].csURL = _T("config/action_ftp.cgi");
	cgiinfo[++ci].csTitle = _T("Mail Action");          cgiinfo[ci].csURL = _T("config/action_mail.cgi");
	cgiinfo[++ci].csTitle = _T("Pre/Post Setting");     cgiinfo[ci].csURL = _T("config/record_prepost.cgi");

	cgiinfo[++ci].csTitle = _T("System Tools");         cgiinfo[ci].csURL = _T("");
	cgiinfo[++ci].csTitle = _T("Digital Input/Output"); cgiinfo[ci].csURL = _T("config/io.cgi");
	cgiinfo[++ci].csTitle = _T("LED");                  cgiinfo[ci].csURL = _T("config/led.cgi"); 
	cgiinfo[++ci].csTitle = _T("RS-485 Settings");      cgiinfo[ci].csURL = _T("config/rs485.cgi"); 
	ASSERT( ci < sizeof(cgiinfo)/sizeof(CGISECTIONINFO) );
	TRACE(_T("%d\r\n"),ci);
	
}


void CIPCamTreeCtrl::vAttach(NODEITEM* pNode)
{
	
/*
	TreeThreadParam* pParam = new TreeThreadParam;
	pParam->pThis = this;
	pParam->pNode = pNode;
	HANDLE h = (HANDLE)_beginthreadex(NULL, 0, CreateTreeThread, pParam, 0, 0);
	CloseHandle( h );


 	TreeThreadParam* pParam = new TreeThreadParam;
 	pParam->pThis = this;
 	pParam->pNode = pNode;

 	strlen(pNode->szUser) ? NULL : strcpy(pNode->szUser, "root");
 	strlen(pNode->szPass) ? NULL : strcpy(pNode->szPass, "pass");
	UINT nRet(0);
 	pNode->hGrowupThread = (HANDLE)_beginthreadex(NULL, 0, CreateTreeThread, pParam, 0, &nRet);
 	CloseHandle( pNode->hGrowupThread  );
 */
}

//UINT CIPCamTreeCtrl::CreateTreeThread(LPVOID lpParam)
//{
//	TreeThreadParam* pLoginInfo = ((TreeThreadParam*) (lpParam));	
//	CGS gs;
//
//	//AuthhInfo is from Registry...
//	//vector <KEEP_AUTH_INFO>& AuthInfo = gs.GetAuthInfo();
//	HTREEITEM* pItemSpec = &pLoginInfo->pThis->itemSpec[MAINGROUP];
//
//	for (int ci =0; ci < AuthInfo.size(); ci++)	//AuthhInfo is from Registry...
//	{
//		if (NULL == memcmp(AuthInfo[ci].mac, pLoginInfo->pNode->mac, sizeof(pLoginInfo->pNode->mac)))
//		{
//			pItemSpec = &pLoginInfo->pThis->itemSpec[AuthInfo[ci].nGroup];
//			memcpy(pLoginInfo->pNode->szUser, AuthInfo[ci].szUser, sizeof(pLoginInfo->pNode->szUser));
//			memcpy(pLoginInfo->pNode->szPass, AuthInfo[ci].szPass, sizeof(pLoginInfo->pNode->szPass));
//			memcpy(pLoginInfo->pNode->wcAlias, AuthInfo[ci].wcAlias, sizeof(pLoginInfo->pNode->wcAlias)/sizeof(TCHAR));
//			memcpy(AuthInfo[ci].ip, pLoginInfo->pNode->ip, sizeof(AuthInfo[ci].ip));
//			pLoginInfo->pNode->nGroup = AuthInfo[ci].nGroup;
//			break;
//		}
//	}
//	int nRet = pLoginInfo->pThis->vGrowUpIPCamInfoTree(pLoginInfo->pNode, *pItemSpec);
//	delete pLoginInfo;
//
////	if (nRet == 0) SetEvent(pLoginInfo->pThis->hAllThreadClosed);
//	return(0);
//}

int CIPCamTreeCtrl::vGrowUpIPCamInfoTree(NODEITEM *pNode, HTREEITEM hParent)
{
	//This functions to build Child Items
	USES_CONVERSION;
	WORD nPort(0);
	WORD nEvent(0);
	TCHAR tzIP[32];
	TCHAR tzUser[32];
	TCHAR tzPass[32];
 
	int ci=0;
	i_counter=0;
	nPort = pNode->nWebPort;//pLoginInfo->nPort;

	wcscpy(tzIP,   A2W(pNode->ip));
	wcscpy(tzUser, A2W(pNode->szUser));//pLoginInfo->szUser
	wcscpy(tzPass, A2W(pNode->szPass));//pLoginInfo->szPass

	const int MAXBUF = 4096;
	CGS gs;
//	vector <KEEP_AUTH_INFO>& AuthInfo = gs.GetAuthInfo();

	//Sleep(1000);
	TRACE(_T("hDlgInitialize : %X\r\n"), hDlgInitialize);
	::WaitForSingleObject(hDlgInitialize, INFINITE);
	bool bExist = false;
	

	HTREEITEM itemRoot = GetNextItem(hParent, LVNI_BELOW );
	while (itemRoot)
	{	
		TVITEM Item;
		Item.hItem = itemRoot;
		Item.mask = TVIF_HANDLE;
		GetItem(&Item);
		NODEITEM* pNodeitem =(NODEITEM*) Item.lParam;
		if (0 == memcmp(pNodeitem->mac, pNode->mac, sizeof(pNode->mac)))
		{
			itemRoot = Item.hItem;
			bExist = true;
			break;
		}
		itemRoot = GetNextSiblingItem(itemRoot);
	}

	if (!bExist)
	{
		TVINSERTSTRUCTW itemStru;
		itemStru.hParent = hParent;
		itemStru.item.lParam =(long) pNode;
		itemStru.item.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

		if (wcslen(pNode->wcAlias) == 0)	//Alias is Empty, display ip only
		{
			itemStru.item.pszText = A2W(pNode->ip);
			itemStru.item.cchTextMax = strlen(pNode->ip);
		}
		else	//With value in Alias - display alias + ip
		{
			TCHAR buffer[65];
			_stprintf(buffer, _T("%s%s%s\n"), pNode->wcAlias,_T(" - ") , A2W(pNode->ip));
			itemStru.item.pszText =buffer;
			itemStru.item.cchTextMax = wcslen(pNode->wcAlias);
		}

		//Demo to display differnet status icons
		int i_status, ll_okstatus=1, ll_warnstatus=2, ll_badstatus=3;
		//int ll_status = vCheckCamStatus(pNode);

		i_status = ll_okstatus;
		if (i_counter==5) {
			i_status=ll_warnstatus;
		}
		if (i_counter==8){
			i_status=ll_badstatus;
		}

		itemStru.item.iImage = i_status;
		itemStru.item.iSelectedImage = i_status;
		i_counter++;

		TRACE(tzIP);
		//Insert ipcam item here...
		itemRoot = InsertItem(&itemStru);
		pNode->hTreeItem = itemRoot;
		SortChildren(hParent);
	}

	Invalidate(true);
	pNode->hGrowupThread = NULL;
//#ifdef FACTORY	
	Expand(hParent, TVE_EXPAND);
//#endif
	return 0;//nTotalEntry;
}

int CIPCamTreeCtrl::vCheckCamStatus(NODEITEM* pNode)
{
	//int nID = ((ENHANCE_FUNC_PARAM*)param)->nFunID;
	// NODEITEM* pNode =  ((ENHANCE_FUNC_PARAM*)param)->pNode;
	CGIVar cgi;
 
	int ll_ret = cgi.vCheckVideoStatus(pNode);

	return ll_ret;
}

void CIPCamTreeCtrl::OnRefresh() 
{
	HANDLE hScanRequire =  OpenEvent(EVENT_ALL_ACCESS, TRUE, _T("Global\\SCAN_REQUIRE"));
	::SetEvent(hScanRequire);
	::CloseHandle(hScanRequire);
/*
	HTREEITEM hItem = GetSelectedItem();
	CString cs = GetItemText(hItem);
	TCHAR buffer[255]={0};
	TVITEM aItem;
	aItem.hItem = hItem;
	aItem.mask =  TVIF_TEXT|TVIF_HANDLE;
	aItem.pszText = buffer;
	aItem.cchTextMax = 255;
	if (GetItem(&aItem) == TRUE && aItem.lParam)
	{
		vAttach((NODEITEM*)aItem.lParam);
	}
*/
}

//Right Mouse Click to display the Popup Menu...
void CIPCamTreeCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	bool lb_ckRecording = false;
	TVITEM TreeItem;
	CPoint pointTrack = point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	UINT flags = 0;
	HTREEITEM hItem = HitTest(point, &flags);

	TRACE(_T("Windows pWnd:%x\tpoint x:%d\tpoint y:%d\r\n"),pWnd, point.x,point.y);
	SelectItem(hItem);
	 
	TreeItem.hItem = hItem;
	TreeItem.mask = TVIF_HANDLE;
	GetItem(&TreeItem);
	
	//Return if NOT a tree item
	if ( TreeItem.hItem == NULL) 	return;
	CMenu m_menu;	
	UINT menuid;

	//if (itemSpec[UNASSIGNED] == TreeItem.hItem || GetParentItem(TreeItem.hItem) == itemSpec[UNASSIGNED])
	int iAuth = ((CMainFrame*)AfxGetMainWnd())->m_iAuth;
	if( iAuth/10 == 0)	return;

	if (itemSpec[MAINGROUP] == TreeItem.hItem  || GetParentItem(TreeItem.hItem) == NULL) //Group Item
	{
		menuid = IDR_MENU_IPCAM_GROUP;
		//HTREEITEM hItem = GetSelectedItem();
		iCurGroupItem = GetSelectedItem();
		CString csGroupName = GetItemText(iCurGroupItem);
		m_mapGroupFlyAway.insert(pair<CString,bool>(csGroupName,false)); // set bFlyAway to false at initial
		//menuid = IDR_MENU_UNASSIGNED;
				m_menu.LoadMenu(menuid);
				//m_menu.CheckMenuItem(ID_MENU_FLYAWAY, bGetGroupFlyAway(csGroupName) ? MF_CHECKED : MF_UNCHECKED);
				if( !((((CMainFrame*)AfxGetMainWnd())->m_iAuth)/10) )
				{
					m_menu.EnableMenuItem(IDC_TREECTRL_MODIFY_GROUP, 1);
					m_menu.EnableMenuItem(IDC_TREECTRL_NEW_GROUP, 1);
					m_menu.EnableMenuItem(IDC_TREECTRL_REMOVE_GROUP, 1);
					m_menu.EnableMenuItem(ID_ADD_NEW_IPCAM, 1);
				}
				else if(csGroupName.Find(_T("NVR")) != -1)
				{
					//m_menu.EnableMenuItem(IDC_TREECTRL_MODIFY_GROUP, 1);
					m_menu.EnableMenuItem(ID_ADD_NEW_IPCAM, 1);
					//GetDlgItem(IDC_TREECTRL_MODIFY_GROUP)->EnableWindow(0);
					//GetDlgItem(ID_ADD_NEW_IPCAM)->EnableWindow(0);
				}
				

	}else //Camera Item
	{		
		iCurTreeItem = TreeItem.hItem;
		menuid = IDR_MENU_IPCAM_TREE;
		
		// let's say you have your menu in m_MainMenu
		HTREEITEM hItem = GetSelectedItem();
		if (NULL == hItem) 	return;

		TVITEM TreeItem;
		TreeItem.hItem = hItem;
		TreeItem.mask = TVIF_HANDLE;
		GetItem(&TreeItem);	

		CString csCameraName = GetItemText(hItem);

		m_menu.LoadMenu(menuid);	
		NODEITEM* pNodeitem = (NODEITEM*) TreeItem.lParam;
		if (pNodeitem->bRecording)
			lb_ckRecording = true;

		menuid = IDR_MENU_IPCAM_TREE;	
	
		if (lb_ckRecording)
			m_menu.CheckMenuItem(IDC_RECORDINGON, MF_CHECKED);
		else
			m_menu.CheckMenuItem(IDC_RECORDINGON, MF_UNCHECKED);

		m_menu.EnableMenuItem(IDC_RECORDINGON, true);

		if( !((((CMainFrame*)AfxGetMainWnd())->m_iAuth)/10) )
		{
			m_menu.EnableMenuItem(ID_ADD_INTERNET_IP_CAM, 1);
			m_menu.EnableMenuItem(ID_MODIFY_IPCAM, 1);
			m_menu.EnableMenuItem(ID_MENU_REMOVEIPCAM32815, 1);
		}
		else if(csCameraName.Find(_T("Channel")) != -1)
		{
			m_menu.EnableMenuItem(ID_ADD_INTERNET_IP_CAM, 1);
			//m_menu.EnableMenuItem(ID_MODIFY_IPCAM, 1);
		}
// 		else if(pNodeitem->nENVR == NODEITEM::REMOTE_ETROCENTER)
// 		{
// 			m_menu.EnableMenuItem(ID_ADD_INTERNET_IP_CAM, 1);
// 			//m_menu.EnableMenuItem(ID_MODIFY_IPCAM, 1);
// 		}
		

	}
    CGS::SetMenuString(m_menu.m_hMenu, IDC_TREECTRL_MODIFY_GROUP, IDS_TREECTRL_MODIFY_GROUP);
    CGS::SetMenuString(m_menu.m_hMenu, IDC_TREECTRL_NEW_GROUP, IDS_TREECTRL_NEW_GROUP);
    CGS::SetMenuString(m_menu.m_hMenu, IDC_TREECTRL_REMOVE_GROUP, IDS_TREECTRL_REMOVE_GROUP);
    CGS::SetMenuString(m_menu.m_hMenu, ID_ADD_NEW_IPCAM, IDS_ADD_NEW_IPCAM);
    //CGS::SetMenuString(m_menu.m_hMenu, ID_MENU_FLYAWAY, IDS_FLYAWAY);
    CGS::SetMenuString(m_menu.m_hMenu, ID_ADD_INTERNET_IP_CAM, IDS_ADD_NEW_IPCAM);
    CGS::SetMenuString(m_menu.m_hMenu, ID_MODIFY_IPCAM, IDS_MODIFY_IPCAM);
    CGS::SetMenuString(m_menu.m_hMenu, ID_MENU_REMOVEIPCAM32815, IDS_REMOVEIPCAM32815);
    CGS::SetMenuString(m_menu.m_hMenu, IDC_IE_EXPLORER, IDS_IE_EXPLORER);

	m_menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pointTrack.x, pointTrack.y, this);		
}


void CIPCamTreeCtrl::InsertDefaultRoot()
{
//#ifdef FACTORY
//	wcscpy(m_csGroup[GROUP01].GroupName, _T("未設定"));
//	m_csGroup[GROUP01] =  _T("IP設定完成");
//	m_csGroup[2] =  _T("韌體更新完成");
//	const int nDefaultSize = 3;
//#else
//	CString ls = _T("Etro Center Group");
//
//	wcscpy(m_csGroup[GROUP01].GroupName, ls);
// 
//	m_csGroup[GROUP01].GroupName =  _T("Group01");
//	const int nDefaultSize = 2;
////#endif
//	m_nGroup = MAX_GROUP;
//	CGS gs;
//	if (!gs.bGetGroupName(m_csGroup, &m_nGroup))
//	{
//		m_nGroup = nDefaultSize;
//	}
//	for (int ci=0; ci < m_nGroup; ci++)
//	{
//		TVINSERTSTRUCTW itemStru;
//		itemStru.hParent = TVGN_ROOT;
//		itemStru.item.lParam =NULL;
//		itemStru.item.mask = TVIF_PARAM | TVIF_TEXT;
//		itemStru.item.pszText = m_csGroup[ci].GroupName->LockBuffer();
//		itemStru.item.cchTextMax = m_csGroup[ci].GroupName->GetLength();
//		itemSpec[ci] = InsertItem(&itemStru);
//	}
	//SelectItem(itemSpec[1]);
}
//
//void CIPCamTreeCtrl::OnEventSetting() 
//{
//		HTREEITEM hItem = GetSelectedItem();
//		if (NULL == hItem) 	return;
//
//		TVITEM TreeItem;
//		TreeItem.hItem = hItem;
//		TreeItem.mask = TVIF_HANDLE;
//		GetItem(&TreeItem);	
// 
//		NODEITEM* pNodeitem = (NODEITEM*) TreeItem.lParam;
//		int ll_camidx = pNodeitem->camera_idx;
//		DlgSettingEvent lDlg;
//		lDlg.m_entryCamidx = ll_camidx;
//
//		lDlg.DoModal();
// 	 
//}

void CIPCamTreeCtrl::OnSetAuthorizationInfo() 
{
	USES_CONVERSION;
	CDlgAuthorizationSetting dlg(GetParent()->GetParent());
	HTREEITEM hItem = GetSelectedItem();
	if (NULL == hItem)
	{
		return;
	}	
	TVITEM TreeItem;
	TreeItem.hItem = hItem;
	TreeItem.mask = TVIF_HANDLE ;
	GetItem(&TreeItem);	
	
//	if (GetParentItem(TreeItem.hItem) != GetRootItem())
	//{
	//	NODEITEM* pNodeitem =(NODEITEM*) TreeItem.lParam;
	//	dlg.SetNodeItemInfo(*pNodeitem, m_csGroup);
	//}
	if (dlg.DoModal() != IDOK)
	{
		return;
	}
	int ci = 0;
	int nGroupIdx = -1;
	while (m_csGroup[ci].GroupName.GetLength() >0)
	{
		if (dlg.m_csSelectedGroup == m_csGroup[ci].GroupName)
		{
			nGroupIdx = ci;
			break;
		}
		ci++;
	}
	if (nGroupIdx == -1)
	{
		vInsertGroup(dlg.m_csSelectedGroup);
		nGroupIdx = m_nGroup;
	}

	TVITEM itemNewGroup;
	itemNewGroup.pszText = m_csGroup[nGroupIdx].GroupName.LockBuffer();
	itemNewGroup.mask = TVIF_TEXT | TVIF_HANDLE ;
	GetItem(&itemNewGroup);

	if (GetParentItem(TreeItem.hItem) == NULL) //Group Tree Item
	{
		HTREEITEM itemRoot = GetNextItem(TreeItem.hItem, TVGN_CHILD);
		while (itemRoot)
		{				
			TVITEM Item;
			Item.hItem = itemRoot;
			Item.mask = TVIF_HANDLE | TVIF_PARAM ;
			GetItem(&Item);
			NODEITEM* pNodeitem =(NODEITEM*) Item.lParam;
		
			if ( GetCheck(Item.hItem) && pNodeitem && dlg.bSetAuthDataInfo(*pNodeitem) )
			{	
//				if (pNodeitem->nGroup == UNASSIGNED) pNodeitem->nGroup = GROUP01;
				if (pNodeitem->nGroup != nGroupIdx)
				{
					itemRoot = GetNextSiblingItem(Item.hItem);					
					HTREEITEM htiNew = MoveChildItem( Item.hItem, itemSpec[nGroupIdx], TVI_LAST );
				//	DeleteItem(Item.hItem);
					//SelectItem( htiNew );
				}
				else
				{
					if (wcslen(pNodeitem->wcAlias) ==0)
						SetItemText(Item.hItem, A2W(pNodeitem->ip));
					else
						SetItemText(Item.hItem, pNodeitem->wcAlias);
					itemRoot = GetNextSiblingItem(Item.hItem);
				}
				pNodeitem->nGroup = nGroupIdx;
				//vSynchronizeAuthInfo(pNodeitem);
			}else
			{
				itemRoot = GetNextSiblingItem(Item.hItem);
			}
		}
		SortChildren( itemSpec[nGroupIdx]);
	}else //Camera Tree Item...
	{
		TVITEM Item;
		Item.hItem = TreeItem.hItem;
		Item.mask = TVIF_HANDLE ;
		GetItem(&Item);
		NODEITEM* pNodeitem =(NODEITEM*) Item.lParam;
		if ( pNodeitem && dlg.bSetAuthDataInfo(*pNodeitem) )
		{
			if (pNodeitem->nGroup != nGroupIdx)
			{

				//itemRoot = GetNextSiblingItem(Item.hItem);					
				HTREEITEM htiNew = MoveChildItem( Item.hItem, itemSpec[nGroupIdx], TVI_LAST );
				//DeleteItem(Item.hItem);
				SelectItem( htiNew );
				SortChildren(GetParentItem(htiNew));

			}else	
			{
				if (wcslen(pNodeitem->wcAlias) ==0) //No Alias - Put the IP address only
					SetItemText(Item.hItem, A2W(pNodeitem->ip));
				else	//if had Alias, append the ip after
				{
					TCHAR buffer[65];
					_stprintf(buffer, _T("%s%s%s\n"), pNodeitem->wcAlias,_T(" - "), A2W(pNodeitem->ip));
					//itemStru.item.pszText = buffer;
					SetItemText(Item.hItem, buffer);
				}
			} 
			pNodeitem->nGroup = nGroupIdx;
			//vSynchronizeAuthInfo(pNodeitem);
		}
	}
}


//void CIPCamTreeCtrl::vSynchronizeAuthInfo(NODEITEM* pNodeitem)
//{
//	if (pNodeitem == NULL) return;
//	CGS gs;
//	//vector <KEEP_AUTH_INFO>& vtAuthInfo = gs.GetAuthInfo();
//	bool bExit = false;
//	int ci;
//	for (ci=0; ci < vtAuthInfo.size(); ci++)
//	{
//		if (NULL == memcmp(vtAuthInfo[ci].mac, pNodeitem->mac, sizeof(pNodeitem->mac)))
//		{
//			bExit = true;
//			break;
//		}
//	}
//	KEEP_AUTH_INFO* pAuthInfo = NULL;
//	if (bExit)
//	{
//		pAuthInfo = &vtAuthInfo[ci];
//	}else
//	{
//		KEEP_AUTH_INFO AuthInfo={0};
//		vtAuthInfo.push_back(AuthInfo);
//		pAuthInfo = &vtAuthInfo[vtAuthInfo.size()-1];;
//		memcpy(pAuthInfo->mac, pNodeitem->mac, sizeof(pNodeitem->mac));
//	}
//	memcpy(pAuthInfo->ip, pNodeitem->ip, sizeof(pNodeitem->ip));
//	memcpy(pAuthInfo->szPass, pNodeitem->szPass, sizeof(pNodeitem->szPass));
//	memcpy(pAuthInfo->szUser, pNodeitem->szUser, sizeof(pNodeitem->szUser));
//	memcpy(pAuthInfo->wcAlias, pNodeitem->wcAlias, sizeof(pNodeitem->wcAlias)/sizeof(TCHAR));
//	pAuthInfo->nGroup =  pNodeitem->nGroup;
//
//	//gs.bSetKeepInfo();
//}

void CIPCamTreeCtrl::vInsertGroup(CString& csName)
{
	m_nGroup++;
	m_csGroup[m_nGroup-1].GroupName =  csName;//dlg.csGetGroupName();

	TVINSERTSTRUCTW itemStru = {0};
	itemStru.hParent = TVGN_ROOT;
	itemStru.item.lParam =NULL;
	itemStru.item.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE;
	itemStru.item.pszText = m_csGroup[m_nGroup-1].GroupName.LockBuffer();
	itemStru.item.cchTextMax = m_csGroup[m_nGroup-1].GroupName.GetLength();
	itemStru.item.iImage = 0;
	itemStru.item.iSelectedImage =0;
	itemSpec[m_nGroup-1] = InsertItem(&itemStru);

	/*CGS gs;
	gs.bSaveGroupName(m_csGroup, m_nGroup);*/
}

void CIPCamTreeCtrl::vInsertGroupDB(CString& csName)
{

	//m_nGroup = m_csGroup
	m_nGroup++;
	m_csGroup[m_nGroup-1].GroupName =  csName;//dlg.csGetGroupName();

	TVINSERTSTRUCTW itemStru = {0};
	itemStru.hParent = TVGN_ROOT;
	itemStru.item.lParam =NULL;
	itemStru.item.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE;
	itemStru.item.pszText = m_csGroup[m_nGroup-1].GroupName.LockBuffer();
	itemStru.item.cchTextMax = m_csGroup[m_nGroup-1].GroupName.GetLength();
	itemStru.item.iImage = 0;
	itemStru.item.iSelectedImage =0;


	//Update DB
	CString ls_sql;
	//ls_sql.Format(_T("INSERT INTO ec_camera_group ( SET camera_group_desc='%s' WHERE cameragroupid= %d"), csNewName, o_GroupId);	
	m_maxGroupId++;
	ls_sql.Format(_T("INSERT INTO ec_camera_group ( cameragroupid, camera_group_desc, module_code, sort_order) VALUES ( %d, '%s', '%s', %d)"), m_maxGroupId, csName, _T("LIVEVIEW"), m_maxGroupId);				
	m_pDB->ExecuteSQL(ls_sql);
				
	itemSpec[m_nGroup-1] = InsertItem(&itemStru);
 
}

//vModify Group
void CIPCamTreeCtrl::vModifyGroup(CString& csNewName)
{
	HTREEITEM hItem = GetSelectedItem();
	if (hItem==NULL ) return;

	CString csGroupName = GetItemText(hItem);

	for (int i=0; i< MAX_GROUP; i++)
	{
		if (m_csGroup[i].GroupName == csGroupName)
		{
			m_csGroup[i].GroupName = csNewName;
			break;
		}
	}
 	 
 	//Set the item text
	SetItemText(hItem, csNewName);

	//CGS gs;
	//gs.bSaveGroupName(m_csGroup, m_nGroup);

}

void CIPCamTreeCtrl::vModifyGroupDB(CString& csNewName, int o_GroupId)
{
	CString ls_sql;
	HTREEITEM hItem = GetSelectedItem();
	if (hItem==NULL ) return;

	CString csGroupName = GetItemText(hItem); // Get Original Group Name
	
 	//Set the item text
	SetItemText(hItem, csNewName);

	//Update DB
	ls_sql.Format(_T("UPDATE ec_camera_group SET camera_group_desc='%s' WHERE cameragroupid= %d"), csNewName, o_GroupId);				
				
	try 
	{
		m_pDB->ExecuteSQL(ls_sql);
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
	}
 
	//CGS gs;
	//gs.bSaveGroupName(m_csGroup, m_nGroup);

}

//Modify current Group
void CIPCamTreeCtrl::OnTreectrlModifyGroup()
{
	CDlgSettingCamGroup_Ext dlg;

	HTREEITEM hItem = GetSelectedItem();
	if (hItem==NULL ) return;

	CString csGroupName = GetItemText(hItem);
	int ll_groupid=0, ll_idx=-1;

	for (int i=0; i< MAX_GROUP; i++)
	{
		if (m_csGroup[i].GroupName == csGroupName)
		{
			ll_groupid = m_csGroup[i].GroupId;
			ll_idx = i;
			break;
		}
	}
 	 
	dlg.m_Groupid = ll_groupid;
	dlg.m_GroupName = csGroupName;
	dlg.m_EntryMode = "L"; //Entry from Live View

	if (IDOK == dlg.DoModal(iCurGroupItem))
	{
		//Reset the Array value
		if (ll_idx>-1) m_csGroup[ll_idx].GroupName = dlg.m_GroupName.Trim();
		vModifyGroupDB(dlg.m_GroupName, dlg.m_Groupid);
	}	
}

void CIPCamTreeCtrl::OnTreectrlNewGroup() 
{
	//CDlgGroupName dlg;
	CDlgSettingCamGroup_Ext dlg;
	dlg.m_GroupName = _T("");
	dlg.m_EntryMode="L"; 

	if (IDOK == dlg.DoModal())
	{
		vInsertGroupDB(dlg.m_GroupName);
	}
	else return;
	// refresh tree after new a group
	this->DeleteAllItems();
	this->InsertDBRoot();
}

void CIPCamTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM  hItem = GetSelectedItem();
	HTREEITEM parentItem = GetParentItem(hItem);
	m_bLDragging = FALSE;
	((CMainFrame*)AfxGetMainWnd())->vIsDragging(m_bLDragging);

	if( ((CMainFrame*)AfxGetMainWnd())->m_bGroupAutoPlay && ((CMainFrame*)AfxGetMainWnd())->m_bAutoScan)
	{
		CString csMsg;
		LOADSTRING(csMsg,IDS_GROUP_AUTOSCAN_ASK);// = _T("Group AutoScan is running, would you like to Pause it ?");
		if(IDYES == AfxMessageBox(csMsg,MB_YESNO))
		{
			// pause autoscan
			((CMainFrame*)AfxGetMainWnd())->vPauseLiveView();
		}
		else
		{
			((CMainFrame*)AfxGetMainWnd())->vSetGroupAutoScan(false);
		}	
	}

	if(parentItem == NULL)// group item, go liveview
	{
		//((CMainFrame*)AfxGetMainWnd())->vSetGroupAutoScan(false);// stop Group Autoscan
		vLiveView();
	}
	else // node item , use 1 matrix to display the video
	{
		((CMainFrame*)AfxGetMainWnd())->m_bEventLV = false;
		((CMainFrame*)AfxGetMainWnd())->vSetEventLV();
		TVITEM lTreeItem={0};
		lTreeItem.hItem = hItem;
		lTreeItem.mask = TVIF_HANDLE  ;
		int ll_ret = GetItem(&lTreeItem);
		NODEITEM* pNodeitem =(NODEITEM*) lTreeItem.lParam;
		vector <NODEITEM*> vtNode;
		vtNode.push_back(pNodeitem);
		if (pNodeitem != NULL)
		{	
			((CMainFrame*)AfxGetMainWnd())->vAlocateViewCell(1); // allocate only 1 matrix cell
			((CMainFrame*)AfxGetMainWnd())->vVideoPresent(vtNode);
			//((CMainFrame*)AfxGetMainWnd())->vSetAutoScan();
		
			m_bOnlyOneCamChoosen = true;
			isPrvGroupName = GetItemText(hItem);				
		}
		::PostMessage(GetParent()->m_hWnd,ID_CHANGE_BMP,(WPARAM)1,NULL);// send msg to TreeView to change bmp
	}
	return;
}


void CIPCamTreeCtrl::OnDestroy() 
{
/*	
	::SetCursor(LoadCursor(NULL, IDC_WAIT));
	WORD nRet = ::SignalObjectAndWait(hCloseDlg, hAllThreadClosed, 6000, false);
	switch (nRet)
	{
		case WAIT_IO_COMPLETION:
			TRACE(_T("WAIT_IO_COMPLETION\r\n"));
			break;
		case WAIT_OBJECT_0:
			TRACE(_T("WAIT_OBJECT_0\r\n"));
			break;
		case STATUS_TIMEOUT:
			TRACE(_T("STATUS_TIMEOUT\r\n"));
			break;
		default:
			TRACE(_T("DEFAULT\r\n"));
			break;
	}
	::CloseHandle(hCloseDlg);
*/	::CloseHandle(hAllThreadClosed);
	
	CTreeCtrl::OnDestroy();
	
}


void CIPCamTreeCtrl::vLiveView() 
{
	TVITEM*	ltv_item;
	TVITEM lTreeItem ={0};
	HTREEITEM  hItem = GetSelectedItem();
	HTREEITEM  hrItem = GetRootItem();
	int ll_maxNode;
	int li_layoutIdx = ((CMainFrame*)AfxGetMainWnd())->m_OrgLayoutIdx;
	
	if (hItem==NULL )
	{		 
			AfxMessageBox(IDS_TREECTRL_SELECT_GROUP);
			return;
	}
	
	HTREEITEM parentItem = GetParentItem(hItem);
	HTREEITEM itemIPCam;
 
	if (parentItem == NULL) //Click on the group
	{			
	 	//selected group changed, needs to free video sets videos[] in order to retrieve the different videos
		iCurGroupItem = hItem;			
		CString csGroupName = GetItemText(hItem);
		
		if(csGroupName != isPrvGroupName)
		{
			ibDiffGroup = true;
			m_bRefreshVideo = true;
		}
		else if(csGroupName == isPrvGroupName)
		{
			ibDiffGroup = false;
			m_bRefreshVideo = false;
		}
	
		itemIPCam = GetNextItem(hItem, TVGN_CHILD );

		// if the only one view cell is choosen, 
		if(li_layoutIdx == 1 && !(((CMainFrame*)AfxGetMainWnd())->m_bAutoScan))
		{		
			
			
				lTreeItem.hItem = itemIPCam;
				lTreeItem.mask = TVIF_HANDLE ;
				int ll_ret = GetItem(&lTreeItem);
				NODEITEM* pNodeitem =(NODEITEM*) lTreeItem.lParam;
				vector <NODEITEM*> vtNode;
				vtNode.push_back(pNodeitem);
				if (pNodeitem != NULL)
				{						
					
					//((CMainFrame*)AfxGetMainWnd())->vFreeVideoSet();
					((CMainFrame*)AfxGetMainWnd())->vIsDragging(false);
					((CMainFrame*)AfxGetMainWnd())->vAlocateViewCell(1);
					((CMainFrame*)AfxGetMainWnd())->vVideoPresent(vtNode);
					m_bOnlyOneCamChoosen = true;
					isPrvGroupName = GetItemText(hItem);
					
				}
				else
				{
					((CMainFrame*)AfxGetMainWnd())->vIsDragging(false);
					((CMainFrame*)AfxGetMainWnd())->vAlocateViewCell(1);
					vtNode.clear();
					((CMainFrame*)AfxGetMainWnd())->vVideoPresent(vtNode);
				}

			return;
		}
					
		
		CString ls = GetItemText(hItem);
		vector <NODEITEM*> vtNode;

		try{
			while (itemIPCam)
			{					
				lTreeItem.hItem = itemIPCam;
				ls = GetItemText(itemIPCam);
				
				lTreeItem.mask = TVIF_HANDLE;
				GetItem(&lTreeItem);

				//long ll_idx =(long) lTreeItem.lParam;
				//if (ll_idx>0)
				//{
					NODEITEM* pNodeitem = (NODEITEM*)lTreeItem.lParam;
					//NODEITEM* pNodeitem = &gNodeItem[ll_idx];
					vtNode.push_back(pNodeitem);
					
				//}
	 
				itemIPCam = GetNextSiblingItem(lTreeItem.hItem);
			}
		}
		catch(exception* e)
		{
			AfxMessageBox(IDS_TREECTRL_EXCEPTION_LIVEVIEW);
		}
		
		// Alocate the View Cell, Call CEtroExplorerView::vSetGrids()
		if(vtNode.size()==1)((CMainFrame*)AfxGetMainWnd())->vAlocateViewCell(li_layoutIdx); // there's only one camera in a group
		else ((CMainFrame*)AfxGetMainWnd())->vAlocateViewCell(vtNode.size());

		//Get the Maximum Node to avoid crash...
		// Take the smaller number of the node
		if (vtNode.size()< MAXVIDEO )
			ll_maxNode = vtNode.size();
		else
			ll_maxNode = MAXVIDEO - 1;

		  			
		////Get the real Video to present number
		//int li_layoutIdx = ((CMainFrame*)AfxGetMainWnd())->m_OrgLayoutIdx;
		//int il_no = vGetVideoNumber(li_layoutIdx);
		//if (il_no<ll_maxNode) 		
		//{// If Matrix number < total videos of the group,  should view videos in turns
		//	ll_maxNode = il_no;
		//	
		//}
			
		//for (int ci=0; ci < min(il_no,vtNode.size()) ; ci++) // 
		//{			
		//	((CMainFrame*)AfxGetMainWnd())->vVideoPresent(vtNode[ci]);
		//}
			((CMainFrame*)AfxGetMainWnd())->vIsDragging(false);
			((CMainFrame*)AfxGetMainWnd())->vVideoPresent(vtNode);
			
		m_bOnlyOneCamChoosen = false;		
		isPrvGroupName = csGroupName;

	}
	else //Click on the video branch item
	{
		HTREEITEM hItemParent = GetParentItem(hItem);
		while ( hItemParent != NULL)
		{
			lTreeItem.hItem = hItem;
			lTreeItem.mask = TVIF_HANDLE  ;
			int ll_ret = GetItem(&lTreeItem);
			NODEITEM* pNodeitem =(NODEITEM*) lTreeItem.lParam;
			vector <NODEITEM*> vtNode;
			vtNode.push_back(pNodeitem);
			if (pNodeitem != NULL)
			{					
				((CMainFrame*)AfxGetMainWnd())->vIsDragging(false);				
				((CMainFrame*)AfxGetMainWnd())->vAlocateViewCell(li_layoutIdx);
				((CMainFrame*)AfxGetMainWnd())->vVideoPresent(vtNode);
				//((CMainFrame*)AfxGetMainWnd())->vSetAutoScan();
				//m_bOnlyOneCamChoosen = true;
				isPrvGroupName = GetItemText(hItem);
				break;
			}
			else	//Avoid infinite loop
				break;
		}
	}

}


void CIPCamTreeCtrl::OnIeExplorer() 
{
	USES_CONVERSION;
	TVITEM TreeItem;
	HTREEITEM  hItem = GetSelectedItem();
	while (GetParentItem(hItem) != NULL)
	{
		TreeItem.hItem = hItem;
		TreeItem.mask = TVIF_HANDLE  ;
		GetItem(&TreeItem);
		NODEITEM* pNodeitem =(NODEITEM*) TreeItem.lParam;
		if (pNodeitem != NULL)
		{
			CString csURL;
			csURL.Format(_T("http://%s:%d"),A2W(pNodeitem->ip), pNodeitem->nWebPort);
			TRACE(csURL);
			HINSTANCE  nResult = ShellExecute(NULL, _T("open"), csURL.LockBuffer(), _T(""), _T(""), SW_SHOW);
			break;

		}
	}
}

//void CIPCamTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
//{
	//Diable the Drag and Drop // We don't want to re-organize the cameras here...
	//NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	//*pResult = 0;
	//
	//// So user cant drag root node
	//if (GetParentItem(pNMTreeView->itemNew.hItem) == NULL) return ; 

	//// Item user started dragging ...
	//m_hitemDrag = pNMTreeView->itemNew.hItem;
	//m_hitemDrop = NULL;



	//m_pDragImage = CreateDragImage(m_hitemDrag);  // get the image list for dragging
	//// CreateDragImage() returns NULL if no image list
	//// associated with the tree view control
	//if( !m_pDragImage )
	//	return;

	//m_bLDragging = TRUE;
	//m_pDragImage->BeginDrag(0, CPoint(-15,-15));
	//POINT pt = pNMTreeView->ptDrag;
	//ClientToScreen( &pt );
	//m_pDragImage->DragEnter(NULL, pt);
	//SetCapture();
//}

void CIPCamTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	HTREEITEM	hitem;
	UINT		flags;

	if (m_bLDragging)
	{
		POINT pt = point;
		ClientToScreen( &pt );
		((CMainFrame*)AfxGetMainWnd())->vScreenToViewClientRect(&pt);
		CImageList::DragMove(pt);
		
		/*CRect rc = m_rects[ci];
		if (rc.PtInRect(pt))
			break;*/
		m_rectTreeCtrl.clear();
		CRect rc =((CMainFrame*)AfxGetMainWnd())->bGetRect();
		m_rectTreeCtrl = ((CMainFrame*)AfxGetMainWnd())->GetRectVct();

		if(m_rectTreeCtrl.size()==0) 
		{
			if(cursor_no != ::GetCursor())
					::SetCursor(cursor_no);
					m_bPtInRect =  false;
		}
		
			
			if(rc.PtInRect(pt))
			{		
				m_bPtInRect=true;
				if (cursor_arr != ::GetCursor()) ::SetCursor(cursor_arr);
				if(((CMainFrame*)AfxGetMainWnd())->bOutOfRange(1))	::SetCursor(cursor_no);
				
			}
			else
			{			
				if(cursor_no != ::GetCursor())
					::SetCursor(cursor_no);
					m_bPtInRect =  false;
			}
		
		
	 	//// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT(pDropWnd); //make sure we have a window

	 
		// Convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient(&pt);
		 
 
			TVHITTESTINFO tvHit;
			tvHit.pt = point;
			HTREEITEM hTarget = HitTest(&tvHit);

			// Select the item that is at the point myPoint.
			UINT uFlags;
			HTREEITEM hItem = HitTest(pt, &uFlags);

 			if ((hItem != NULL) && (TVHT_ONITEM & uFlags))			
				SelectItem(hItem);	
			if (cursor_arr != ::GetCursor()) ::SetCursor(cursor_arr);
		
	}
	else 
	{
		// Set cursor to arrow if not dragged
		// Otherwise, cursor will stay hand or arrow depen. on prev setting
		if(cursor_no != ::GetCursor()) ::SetCursor(cursor_arr);
	}	
	//CTreeCtrl::OnMouseMove(nFlags, point);
}

void CIPCamTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bLDragging)
	{		
		m_bLDragging = FALSE;
		((CMainFrame*)AfxGetMainWnd())->vIsDragging(m_bLDragging);
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();

		
		if(m_pDragImage != NULL) 
		{ 
			delete m_pDragImage; 
			m_pDragImage = NULL; 
		} 
		
		

		if(m_bPtInRect) //Drag into the video area...
		{
		
			BeginWaitCursor();

				vector <NODEITEM*> vtNode;
				HTREEITEM hItem = GetSelectedItem();
				if(GetParentItem(m_hitemDrag)==NULL)//  group item
				{	
					int nNodeCount = 0;
						
						HTREEITEM itemIPCam = GetNextItem(m_hitemDrag, TVGN_CHILD );
						while (itemIPCam)
						{	
							TVITEM lTreeItem={0};				
							lTreeItem.hItem = itemIPCam;
							 GetItemText(itemIPCam);
							
							lTreeItem.mask = TVIF_HANDLE;
							GetItem(&lTreeItem);
							if(lTreeItem.lParam!=NULL)
							{
								NODEITEM* pNodeitem = (NODEITEM*)lTreeItem.lParam;
								// insert hTreeitem into nodeitem, so it can be selected when click its view
								pNodeitem->hTreeItem = itemIPCam;
					     		vtNode.push_back(pNodeitem);
								nNodeCount++;
								itemIPCam = GetNextSiblingItem(lTreeItem.hItem);
							}
						}
						((CMainFrame*)AfxGetMainWnd())->vIsDragging(false);
						
						((CMainFrame*)AfxGetMainWnd())->vVideoPresent(vtNode);

				}
				else // one node
				{
					// find out the index of m_rect where the video drop to
					POINT pt = point;
					ClientToScreen( &pt );
					for(int i=0;i<m_rectTreeCtrl.size();i++)
					{

						CRect rect = m_rectTreeCtrl[i];
						if(rect.PtInRect(pt))
						{
							m_nDropRectIdx = i;
							break;
						}
						else m_nDropRectIdx = -1;
						
					}


					TVITEM lTreeItem={0};
					lTreeItem.hItem = m_hitemDrag;
					lTreeItem.mask = TVIF_HANDLE ;
					int ll_ret = GetItem(&lTreeItem);

					NODEITEM* pNodeitem =(NODEITEM*) lTreeItem.lParam;
					// insert hTreeitem into nodeitem, so it can be selected when click its view
					pNodeitem->hTreeItem = m_hitemDrag;
					vector <NODEITEM*> vtNode;
					vtNode.push_back(pNodeitem);
					((CMainFrame*)AfxGetMainWnd())->vIsDragging(true);

				/*	if(((CMainFrame*)AfxGetMainWnd())->bOutOfRange(1))
					{
						int nCurrentLayOut = ((CMainFrame*)AfxGetMainWnd())->m_OrgLayoutIdx;
						((CMainFrame*)AfxGetMainWnd())->m_OrgLayoutIdx = nNextAvailableLayOut( nCurrentLayOut);
						((CMainFrame*)AfxGetMainWnd())->vAlocateViewCell(((CMainFrame*)AfxGetMainWnd())->m_OrgLayoutIdx);
					}*/

					((CMainFrame*)AfxGetMainWnd())->vVideoPresent(vtNode);
				}
				EndWaitCursor();
		}
		else //Drag Not in the video area
		{
		int ll_Draggroupid=0,ll_Dropgroupid=0;			
			m_bLDragging = FALSE;
			CImageList::DragLeave(this);
			CImageList::EndDrag();
			ReleaseCapture();

		if(m_pDragImage != NULL) 
		{ 
			delete m_pDragImage; 
			m_pDragImage = NULL; 
		} 
		
	 
			BeginWaitCursor();

			vector <NODEITEM*> vtNode;
			HTREEITEM hItem = GetSelectedItem();

			if(GetParentItem(m_hitemDrag)==NULL)//  drag group item		
					return;
 
				//Get the drag parent item
				HTREEITEM hpItem =  GetParentItem(m_hitemDrag);

				//Get drag Item - Camera Info
				TVITEM lTreeItem={0};
				lTreeItem.hItem = m_hitemDrag;
				lTreeItem.mask = TVIF_HANDLE ;
				int ll_ret = GetItem(&lTreeItem);
				NODEITEM* pNodeitem =(NODEITEM*) lTreeItem.lParam;
				
				int ll_camid =  pNodeitem->cameraid;

				//Drag Parent Group
				CString csDragGroupName = GetItemText(hpItem);
				for (int i=0; i< MAX_GROUP; i++)
				{
						if (m_csGroup[i].GroupName == csDragGroupName)
						{
							ll_Draggroupid = m_csGroup[i].GroupId;
							break;
						}
				}

				//Drop Item
				if(GetParentItem(hItem)!=NULL)// Drop into camera item
					hItem = 	GetParentItem(hItem);

				CString csGroupName = GetItemText(hItem);

				//--Get GroupID
				int iDropGroupID = 0;
				HTREEITEM hTempGroup = this->GetRootItem();
				while(hTempGroup)
				{
					iDropGroupID++;
					if(hTempGroup == hItem)	break;
					
					HTREEITEM hTempItem = hTempGroup;
					hTempGroup = this->GetNextSiblingItem(hTempItem);
				}

				for (int i=0; i< MAX_GROUP; i++)
				{
						/*if (m_csGroup[i].GroupName == csGroupName)
						{
							ll_Dropgroupid = m_csGroup[i].GroupId;
							break;
						}*/
					if(m_csGroup[i].GroupId == iDropGroupID)
					{
						ll_Dropgroupid = m_csGroup[i].GroupId;
						break;
					}
				}

					if (vChangeCamGroup(ll_Draggroupid, ll_Dropgroupid,  ll_camid))
					{
						ibDiffGroup = true;
						m_bRefreshVideo = true;
						((CMainFrame*)AfxGetMainWnd())->vIsDragging(false);
						//vLiveView();
					}
			
			return;
		}
		
		
	}
	CTreeCtrl::OnLButtonUp(nFlags, point);
}


HTREEITEM CIPCamTreeCtrl::MoveChildItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	TV_INSERTSTRUCT tvstruct;
	HTREEITEM hNewItem;
    CString sText;

    // get information of the source item
    tvstruct.item.hItem = hItem;
    tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    GetItem(&tvstruct.item);  
    sText = GetItemText( hItem );
        
    tvstruct.item.cchTextMax = sText.GetLength();
    tvstruct.item.pszText = sText.LockBuffer();

    //insert the item at proper location
    tvstruct.hParent = htiNewParent;
    tvstruct.hInsertAfter = htiAfter;
    tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
    hNewItem = InsertItem(&tvstruct);

    //now copy item data and item state.
    SetItemData(hNewItem,GetItemData(hItem));
    SetItemState(hNewItem,GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);
	
	CString csGroup = GetItemText(htiNewParent);
	int ci = 0;
	while (m_csGroup[ci].GroupName.GetLength() >0)
	{
		TRACE(m_csGroup[ci].GroupName);
		TRACE(_T("\r\n"));
		if ( csGroup == m_csGroup[ci].GroupName)
		{
			break;
		}
		ci++;
	}	
	TVITEM Item;
	Item.hItem = hNewItem;
	Item.mask = TVIF_HANDLE | TVIF_PARAM  ;
	GetItem(&Item);
	NODEITEM* pNodeitem =(NODEITEM*) Item.lParam;
	pNodeitem->nGroup = ci;
	pNodeitem->hTreeItem = hNewItem;
	//vSynchronizeAuthInfo(pNodeitem);
	//now delete the old item
	DeleteItem(hItem);
    return hNewItem;
}

void CIPCamTreeCtrl::SetDefaultCursor()
{
    CString strWndDir;
    GetWindowsDirectory(strWndDir.GetBuffer(MAX_PATH), MAX_PATH);

    strWndDir += _T("\\winhlp32.exe");
    // This retrieves cursor #106 from winhlp32.exe, which is a hand pointer
    HMODULE hModule = LoadLibrary(strWndDir);
    if (hModule) {
        HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
        if (hHandCursor)
		{
            cursor_hand = CopyCursor(hHandCursor);
		}			  
        FreeLibrary(hModule);
    }

	cursor_arr	= ::LoadCursor(NULL, IDC_ARROW);
	cursor_no	= ::LoadCursor(NULL, IDC_NO) ;
}

void CIPCamTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	::GetCursorPos(&point);
	ScreenToClient(&point);
	UINT flags = 0;
	HTREEITEM hItem = HitTest(point, &flags);
	SelectItem(hItem);
	TRACE(_T("LButtonDown"));
	CTreeCtrl::OnLButtonDown(nFlags, point);
}



void CIPCamTreeCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
//	HTREEITEM hitem = GetSelectedItem();
//	if ( hitem  == NULL) return ;

	CPoint point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	UINT flags = 0;	
	HTREEITEM	hitem;
	// Make sure pt is over some item
	//if ( ((hitem = HitTest(point, &flags)) == NULL) || !(flags & TVHT_ONITEMSTATEICON)) return ;
	if ((hitem = this->GetSelectedItem()) == NULL)	return;

	if (GetParentItem(hitem) == NULL) // Group Item
	{
		BOOL bChecked = GetCheck(hitem);
		iCurGroupItem = hitem;
//		int nState = GetStateImage(hItem);
//		nState |= HDCheckboxImageList::HOT_INDEX;


		SetItemState(hitem, INDEXTOSTATEIMAGEMASK(!bChecked), TVIS_STATEIMAGEMASK);
		HTREEITEM itemRoot = GetNextItem(hitem, TVGN_CHILD);
		while (itemRoot)
		{
			SetCheck(itemRoot, !bChecked);
			itemRoot = GetNextSiblingItem(itemRoot);
		}
	}else if (GetParentItem(GetParentItem(hitem)) == NULL)
	{
		BOOL bChecked = GetCheck(hitem);
		bChecked = !bChecked;
		SetItemState(hitem, INDEXTOSTATEIMAGEMASK(bChecked), TVIS_STATEIMAGEMASK);

		//--Recover Image after click.
		this->SetItemImage(hitem, 4, 4);
		
		HTREEITEM itemSibling = hitem;
		bool bCoincide = true;
		if (itemSibling = GetChildItem((GetParentItem(hitem))))
		{
			do{
				if (bChecked != GetCheck(itemSibling) && (itemSibling != hitem))
				{
					bCoincide = false;
					break;
				}
			}while (itemSibling  = GetNextSiblingItem(itemSibling));
		}
		HTREEITEM hParent = GetParentItem(hitem);
		if (bCoincide)
		{
			SetCheck(hParent, bChecked);
		}else
		{		
			SetItemState(hParent, INDEXTOSTATEIMAGEMASK(3), TVIS_STATEIMAGEMASK);
		}
	}
	*pResult = 0;
}

void CIPCamTreeCtrl::OnTreectrlRemoveGroup() 
{
	CPoint point;
	HTREEITEM	hItem, hRootItem = NULL, hGroupItem = NULL;	
	int ci = 0, iGroupCount = 1;
	UINT flags = 0;

	::GetCursorPos(&point);
	ScreenToClient(&point);

	hRootItem = this->GetRootItem();
	hGroupItem = this->GetNextSiblingItem(hRootItem);
	while(hGroupItem)
	{
		iGroupCount++;
		hGroupItem = this->GetNextSiblingItem(hGroupItem);
	}

	if(iGroupCount <= 1)
	{
		AfxMessageBox(_T("Can not remove the last group."));
		return ;
	}

	if ( ((hItem = GetSelectedItem()) == NULL) ) 
	{
		AfxMessageBox(IDS_TREECTRL_NOT_GROUP); //Shouldn't happen
		return ;
	}

	if (GetParentItem(hItem) == NULL)
	{
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
		{
			if( AfxMessageBox( _T("All Cameras will be removed.\r\nDo you want to continue?"), MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
			{
				CString csGroupName = this->GetItemText(hItem);
				int ll_groupid = -1;
				//Get the selected GroupID
				for (int i=0; i< MAX_GROUP; i++)
				{
					if (m_csGroup[i].GroupName == csGroupName)
					{
						ll_groupid = m_csGroup[i].GroupId;
						break;
					}
				}

				bool lb_Success = vDeleteGroup(ll_groupid);

				HTREEITEM hCamera = GetChildItem(hItem);
				while(hCamera)
				{
					HTREEITEM hCurrentCamera = hCamera;
					hCamera = this->GetNextItem(hCamera, TVGN_NEXT);

					TVITEM TreeItem;
					TreeItem.hItem = hCurrentCamera;
					TreeItem.mask = TVIF_PARAM|TVIF_HANDLE ;
					GetItem(&TreeItem);	

					NODEITEM*  lpItem  =(NODEITEM*) TreeItem.lParam;

					int iCameraID = lpItem->cameraid, iCameraIDX = lpItem->camera_idx;
					bool bDelete = true;

					/*for(int iTemp =0; iTemp < vectorDoubleCameraID.size(); iTemp++)
					{
						if(vectorDoubleCameraID[iTemp] == iCameraID)
						{
							bDelete = false;
							break;
						}
					}*/
					
					CString csSQL;
					if(bDelete)
					{
						csSQL.Format(_T("DECLARE @REC_ID int; SELECT @REC_ID = camera_recordingid FROM ec_camera_recording WHERE camera_idx = %d; DELETE FROM ec_camera_recording_time WHERE camera_recordingid = @REC_ID; DELETE FROM ec_camera_group_camera WHERE cameraid = %d; DELETE FROM ec_stream WHERE cameraid = %d; DELETE FROM ec_camera WHERE cameraid = %d; DELETE FROM ec_camera_recording WHERE camera_idx = %d"), iCameraIDX, iCameraID, iCameraID, iCameraID, iCameraIDX);
					}
					else
					{
						csSQL.Format(_T("DELETE FROM ec_camera_group_camera WHERE cameraid = %d"), iCameraID);
					}
					try
					{
						TRACE(csSQL);
						m_pDB->ExecuteSQL(csSQL);
						DeleteItem(hCurrentCamera);

					}
					catch(CDBException* p)
					{
						AfxMessageBox(p->m_strError);
						p->Delete();
					}
				}

				if (lb_Success) DeleteItem(hItem);

				CNVRBk::GetBkObject()->SendMessage(UM_REFRESH_ALLTREE, NULL, NULL);
				//((CTaskTreeCtrl*)((CMainFrame*)AfxGetMainWnd())->GetTreeView())->vRefreshTree();
				HTREEITEM hRoot = GetRootItem();
				this->SelectItem(hRoot);
				this->vLiveView();

				//--notify recording service
				CSA csa;
				HANDLE hCameraRecording = CreateEvent(&csa.m_sa, false, false, RECORDING_STATUS_CHANGE);
				if (hCameraRecording == NULL)	TRACE(_T("Error code :%d"), ::GetLastError());
				SetEvent( hCameraRecording );

				//--notify Event service
				HANDLE hRestartEvent = ::CreateEvent(&csa.m_sa, false, false, _T("Global\\ETROCENTER_EVENT_RESTART"));
				if(hRestartEvent == NULL)
				{TRACE(_T("Create Restart Event Error Code: %d"), ::GetLastError());}
				SetEvent(hRestartEvent);
				CloseHandle(hRestartEvent);
			}

			//CString csGroupName = this->GetItemText(hItem);
			//int ll_groupid = -1;
			////Get the selected GroupID
			//for (int i=0; i< MAX_GROUP; i++)
			//{
			//	if (m_csGroup[i].GroupName == csGroupName)
			//	{
			//		ll_groupid = m_csGroup[i].GroupId;
			//		break;
			//	}
			//}

			//bool lb_Success = vDeleteGroup(ll_groupid);

			//if (lb_Success) DeleteItem(hItem);

		}
		else
		{

			//--Camera in this group or not
			if( GetChildItem(hItem) != NULL)
			{
				AfxMessageBox(_T("Can not remove this group with camera in this group."));
				return;
			}

			CString csGroupName = GetItemText(hItem);
			CString cs,csDeleteGroup, csDeleteGroup2;
			int	ll_groupid;	
			//csDeleteGroup.LoadString(CGS::g_hResStr, IDS_DELETE_GROUP1);
			LOADSTRING(csDeleteGroup,IDS_DELETE_GROUP1);
			cs.Format(_T("%s: %s ?"), csDeleteGroup,csGroupName);	
			//csDeleteGroup2.LoadString(IDS_DELETE_GROUP2);
			//cs.Format(_T("%s: %s, %s"), csDeleteGroup,csGroupName,csDeleteGroup2);		
			
			if ( AfxMessageBox(cs, MB_OKCANCEL,0 )== IDCANCEL) return;

			//Get the selected GroupID
			for (int i=0; i< MAX_GROUP; i++)
			{
				if (m_csGroup[i].GroupName == csGroupName)
				{
					ll_groupid = m_csGroup[i].GroupId;
					break;
				}
			}

			while (ci < m_nGroup)
			{
				if (csGroupName == m_csGroup[ci].GroupName)
				{
					HTREEITEM hSibling = hItem;;
					while (hSibling = GetNextSiblingItem(hSibling))
					{
						HTREEITEM hChild= GetNextItem(hSibling, TVGN_CHILD);
						while (	hChild ) 
						{
							TVITEM aItem;
							aItem.hItem = hChild;
							aItem.mask = TVIF_PARAM;
							if (GetItem(&aItem))
							{	
								NODEITEM* pNode = (NODEITEM*)aItem.lParam;
								pNode->nGroup = ci;
								//vSynchronizeAuthInfo(pNode);
								hChild= GetNextSiblingItem(hChild);
							}else
							{
								ETROLOG(_T("OnTreectrlRemoveGroup Error GetItem"));
								return;
							}
						}
						m_csGroup[ci]=m_csGroup[ci+1];
						ci++;
					}
					m_csGroup[m_nGroup].GroupName = "";
					m_nGroup --;

					bool lb_Success = vDeleteGroup(ll_groupid);

					if (lb_Success) DeleteItem(hItem);
				/*	CGS gs;		
					gs.bSaveGroupName(m_csGroup, m_nGroup);
					gs.bSetKeepInfo();*/
					break;
				}
				ci++;
			}

			//--notify recording service
			CSA csa;
			HANDLE hCameraRecording = CreateEvent(&csa.m_sa, false, false, RECORDING_STATUS_CHANGE);
			if (hCameraRecording == NULL)	TRACE(_T("Error code :%d"), ::GetLastError());
			SetEvent( hCameraRecording );

			//--notify Event service
			HANDLE hRestartEvent = ::CreateEvent(&csa.m_sa, false, false, _T("Global\\ETROCENTER_EVENT_RESTART"));
			if(hRestartEvent == NULL)
			{TRACE(_T("Create Restart Event Error Code: %d"), ::GetLastError());}
			SetEvent(hRestartEvent);
			CloseHandle(hRestartEvent);
		}

	}

	try
	{
		CString csSQL(_T("UPDATE ec_camera_group SET default_ = 'Y' WHERE cameragroupid = 1"));
		m_pDB->ExecuteSQL(csSQL);
	}
	catch(CDBException* p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}


BOOL CIPCamTreeCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return CTreeCtrl::OnEraseBkgnd(pDC);
	pDC->SetBkColor(BKCOLOR);
	CBrush brush(BKCOLOR);
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &brush);
	return true;
}

void CIPCamTreeCtrl::OnLiveView() 
{
	vLiveView();
}

void CIPCamTreeCtrl::OnPaint() 
{
    CPaintDC paintDc(this);
}

void CIPCamTreeCtrl::vSyncSelectedNode(NODEITEM *pNode)
{
	HTREEITEM hParent = GetNextItem(NULL, TVGN_ROOT);
	TRACE(GetItemText(hParent));
	TRACE(_T("\r\n"));
	while (hParent) 
	{
		HTREEITEM hItem = GetNextItem(hParent, TVGN_CHILD);
		while (hItem)
		{
			TV_ITEM TreeItem;
			TreeItem.hItem = hItem;
			TreeItem.mask =  TVIF_HANDLE | TVIF_PARAM |TVIF_TEXT;
			BOOL bRet = GetItem(&TreeItem);
			TRACE(GetItemText(hItem));
			TRACE(_T("\r\n"));
			if ((NODEITEM*)TreeItem.lParam == pNode)
			{
				bReEntry = true;
				SelectItem(hItem);
				Expand( hItem, TVE_EXPAND ) ;

				return;
			}
			hItem = GetNextItem(hItem, TVGN_NEXT );
		}
		hParent = GetNextItem(hParent, TVGN_NEXT);
		TRACE(GetItemText(hParent));
		TRACE(_T("\r\n"));
	}
}

void CIPCamTreeCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	TVITEM TreeItem;
	HTREEITEM  hItem = GetSelectedItem();	
	
	if (GetParentItem(hItem) == NULL) //Group Items
	{
 
		TreeItem.hItem = hItem;
		TreeItem.mask = TVIF_HANDLE;
		GetItem(&TreeItem);		
		
		CString csGroupName = GetItemText(hItem);
	/*	if (csGroupName != isPrvGroupName)
		{			
			isPrvGroupName = csGroupName;
      		ibDiffGroup = true;
			m_bRefreshVideo = true;
		}*/
		
		

	}
	
/*******************************************************/

	if (bReEntry)
	{
		bReEntry = false;
		return;
	}
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	if (pNMTreeView->itemNew.lParam == NULL) 
	{
		((CMainFrame*)AfxGetMainWnd())->vItemChanged( NULL);
		return;
	}

	//if (( (NODEITEM*)pNMTreeView->itemNew.lParam )->nStatus == NODEITEM::LIVE)
	//	((CMainFrame*)AfxGetMainWnd())->vItemChanged((NODEITEM*)pNMTreeView->itemNew.lParam);
	//else
	//{
	/*	if (pNMTreeView->itemOld.hItem) 
		{
			bReEntry = true;
			SelectItem(pNMTreeView->itemOld.hItem);
		}*/
	//}
	bReEntry = false;
	//pNMTreeView->itemNew.iImage=1;
	*pResult = 0;
}


NODEITEM* CIPCamTreeCtrl::GetCurrentNodeItem()
{
	HTREEITEM	hItem;
	if ( ((hItem = GetSelectedItem()) == NULL) ) return NULL;

	TVITEM TreeItem;
	TreeItem.hItem = hItem;
	TreeItem.mask =  TVIF_HANDLE  ;
	GetItem(&TreeItem);
	NODEITEM* pNodeitem =(NODEITEM*) TreeItem.lParam;
	//int ll_idx =  TreeItem.lParam;
	//return  &gNodeItem[ll_idx];
	return pNodeitem;
}

//void CIPCamTreeCtrl::OnFirsttimeInitialize() 
//{
//	CEncpSettingPage dlg;
//	vector <HENCPITEM> vthENCPItem;
//	vector <HTREEITEM> vthitem;
//	NODEITEM* pNode = GetCurrentNodeItem();
//	if (pNode)
//	{
//		vthitem.push_back(GetSelectedItem());
//		vthENCPItem.push_back(pNode->hEncp);
//		dlg.SetENCPItem(&vthENCPItem);
//	}
//	else
//	{	
//		HTREEITEM item = GetNextItem(GetSelectedItem(), TVGN_CHILD);
//		while (item)
//		{				
//			TVITEM Item;
//			Item.hItem = item;
//			Item.mask = TVIF_HANDLE ;
//			GetItem(&Item);
//			NODEITEM* pNodeitem =(NODEITEM*) Item.lParam;		
//			if ( GetCheck(Item.hItem) && pNodeitem)
//			{
//				vthitem.push_back(item);
//				vthENCPItem.push_back(pNodeitem->hEncp);
//			}
//			TRACE(GetItemText(item));
//			item = GetNextItem(item, TVGN_NEXT);
//		}
//		if (0 == vthENCPItem.size()) return;
//		dlg.SetENCPItem(&vthENCPItem);
//	}
//	if (IDOK == dlg.DoModal())
//	{
////		::SendMessage(AfxGetMainWnd()->m_hWnd, WM_COMMAND, IDC_OVERVIEW_VIEW, NULL);
//#ifdef FACTORY
//		HTREEITEM hIPSetOkGroup;
//		hIPSetOkGroup = GetRootItem();
//		hIPSetOkGroup = GetNextSiblingItem(hIPSetOkGroup);	
//#endif
//		for (int ci=0; ci < vthitem.size(); ci++)
//		{
//			TVITEM Item;
//			Item.hItem = vthitem[ci];
//			Item.mask = TVIF_HANDLE ;
//			GetItem(&Item);
//			NODEITEM* pNodeitem =(NODEITEM*) Item.lParam;		
//			pNodeitem->nStatus = NODEITEM::RESETTING;
//#ifndef FACTORY
//			delete ((CGIVar*) pNodeitem->pIPCamCGIInfo);
//#endif
//			pNodeitem->pIPCamCGIInfo = NULL;
//			SetItemText(vthitem[ci], _T("Resetting..."));	
//#ifdef FACTORY
//			MoveChildItem(vthitem[ci], hIPSetOkGroup, TVI_LAST );
//#endif
//		}
//		SelectItem(NULL);
//		AfxBeginThread(fnPollingIPCamera, NULL);
//	}
//	return;
///*
//	CDlgFirstTimeInitialize dlg2;
//	if (IDOK ==dlg.DoModal())
//	{
//		CDlgFirstTimeInitialize::PARAMINFO info;
//		dlg2.GetParamInfo(info);
//		HENCPITEM hItem;
//	//	ENCPNETWORK* pNetInfo;
//		encp_SetNetworkInfo(hItem, "root", info.password, &info.NetInfo);
//	}
//*/
//}

void CIPCamTreeCtrl::OnFirsttimeInitialize() 
{
	 
	CEncpSettingPage dlg;
	vector <HENCPITEM> vthENCPItem;
	vector <HTREEITEM> vthitem;
	NODEITEM* pNode = GetCurrentNodeItem();
	if (pNode)
	{
		vthitem.push_back(GetSelectedItem());
		vthENCPItem.push_back(pNode->hEncp);
		dlg.SetENCPItem(&vthENCPItem);

		if( strstr(pNode->camera_name, "NVR") || strstr(pNode->camera_name, "Channel") )
		{
			CDlgAddManageCam DlgAdd(pNode->cameraid, 4);
			DlgAdd.DoModal();
		}
		else
		{
			CDlgAddManageCam DlgAdd(pNode->cameraid, 3);
			DlgAdd.DoModal();
		}


		

	}
	else
	{	
		HTREEITEM item = GetNextItem(GetSelectedItem(), TVGN_CHILD);
		while (item)
		{				
			TVITEM Item;
			Item.hItem = item;
			Item.mask = TVIF_HANDLE ;
			GetItem(&Item);
			NODEITEM* pNodeitem =(NODEITEM*) Item.lParam;		
			if ( GetCheck(Item.hItem) && pNodeitem)
			{
				vthitem.push_back(item);
				vthENCPItem.push_back(pNodeitem->hEncp);
			}
			TRACE(GetItemText(item));
			item = GetNextItem(item, TVGN_NEXT);
		}
		if (0 == vthENCPItem.size()) return;
		dlg.SetENCPItem(&vthENCPItem);
	}
	 
	return;


}

void CIPCamTreeCtrl::vUpdateTreeItem(NODEITEM* pNode)
{
	USES_CONVERSION;
	if (pNode->hTreeItem)
	{
		if ( wcslen(pNode->wcAlias) ==0 )
			SetItemText(pNode->hTreeItem, A2W(pNode->ip));
		else
			SetItemText(pNode->hTreeItem, pNode->wcAlias);
		SelectItem(pNode->hTreeItem);
	}
}

void CIPCamTreeCtrl::OnAddInternetIpCam() 
{
	//((CIPCamTreeView*)GetParent())->OnAddInternetIpCam();

	HTREEITEM hItem = GetSelectedItem();
	if (hItem==NULL ) return;

	HTREEITEM pItem = GetParentItem(hItem);

	CString csGroupName = GetItemText(pItem);

	CDlgAddManageCam dlg(csGroupName, 1);

	if( dlg.DoModal() == IDOK)
	{
		return;
	}

}

void CIPCamTreeCtrl::OnUpgradeFirmware() 
{
	CDlgProductTest dlg;

	TVITEM TreeItem;
	HTREEITEM  hItem = GetSelectedItem();

	vector <NODEITEM*> vtNode;
	if (GetParentItem(hItem) == NULL)
	{
		HTREEITEM itemIPCam = GetNextItem(hItem, TVGN_CHILD );
		while (itemIPCam)
		{
			TreeItem.hItem = itemIPCam;
			if (GetCheck(TreeItem.hItem)) 
			{
				TreeItem.mask = TVIF_HANDLE  ;
				GetItem(&TreeItem);
				NODEITEM* pNodeitem =(NODEITEM*) TreeItem.lParam;
				vtNode.push_back(pNodeitem);
			}
			itemIPCam = GetNextSiblingItem(TreeItem.hItem);
		}
	}else
	{
		while (GetParentItem(hItem) != NULL)
		{
			TreeItem.hItem = hItem;
			TreeItem.mask = TVIF_HANDLE  ;
			GetItem(&TreeItem);
			NODEITEM* pNodeitem =(NODEITEM*) TreeItem.lParam;
			if (pNodeitem != NULL)
			{
				vtNode.push_back(pNodeitem);
				break;
			}
		}
	}
	dlg.vAssignNode(&vtNode);
	dlg.DoModal();
#ifdef FACTORY
	vector <NODEITEM*>* pvtNodeOK = dlg.GetUpgradeOKNode();
	HTREEITEM hIPSetOkGroup;
	hIPSetOkGroup = GetRootItem();
	hIPSetOkGroup = GetNextSiblingItem(hIPSetOkGroup);
	hIPSetOkGroup = GetNextSiblingItem(hIPSetOkGroup);		
	for (int ci=0; ci < pvtNodeOK->size(); ci++)
	{		
		MoveChildItem((*pvtNodeOK)[ci]->hTreeItem, hIPSetOkGroup, TVI_LAST );
	}
#endif	
}

void CIPCamTreeCtrl::OnProductTest() 
{
	CDlgProductTest dlg;

	TVITEM TreeItem;
	HTREEITEM  hItem = GetSelectedItem();

	vector <NODEITEM*> vtNode;
	if (GetParentItem(hItem) == NULL)
	{
		HTREEITEM itemIPCam = GetNextItem(hItem, TVGN_CHILD );
		while (itemIPCam)
		{
			TreeItem.hItem = itemIPCam;
			if (GetCheck(TreeItem.hItem)) 
			{
				TreeItem.mask = TVIF_HANDLE  ;
				GetItem(&TreeItem);
				NODEITEM* pNodeitem =(NODEITEM*) TreeItem.lParam;
				vtNode.push_back(pNodeitem);
			}
			itemIPCam = GetNextSiblingItem(TreeItem.hItem);
		}
	}else
	{
		while (GetParentItem(hItem) != NULL)
		{
			TreeItem.hItem = hItem;
			TreeItem.mask = TVIF_HANDLE  ;
			GetItem(&TreeItem);
			NODEITEM* pNodeitem =(NODEITEM*) TreeItem.lParam;
			if (pNodeitem != NULL)
			{
				vtNode.push_back(pNodeitem);
				break;
			}
		}
	}
	dlg.vAssignNode(&vtNode);
	dlg.DoModal();
#ifdef FACTORY
	vector <NODEITEM*>* pvtNodeOK = dlg.GetUpgradeOKNode();
	HTREEITEM hIPSetOkGroup;
	hIPSetOkGroup = GetRootItem();
	hIPSetOkGroup = GetNextSiblingItem(hIPSetOkGroup);
	hIPSetOkGroup = GetNextSiblingItem(hIPSetOkGroup);		
	for (int ci=0; ci < pvtNodeOK->size(); ci++)
	{		
		MoveChildItem((*pvtNodeOK)[ci]->hTreeItem, hIPSetOkGroup, TVI_LAST );
	}
#endif
}
#ifdef FACTORY
void CIPCamTreeCtrl::SetGroupSelectedItem(int nGroup)
{
	HTREEITEM hRoot, hSelectedGroup;
	hRoot = hSelectedGroup= GetRootItem();
	for (int ci=0; ci < nGroup; ci++)
	{
		hSelectedGroup = GetNextSiblingItem(hRoot);
		CString csName = GetItemText(hRoot);
	}
	SetCheck(hSelectedGroup, true);
	HTREEITEM itemSibling;
	if (itemSibling = GetChildItem(hSelectedGroup))
	{
		SetCheck(itemSibling, true);
		while (itemSibling)
		{
			SetCheck(itemSibling, true);
			itemSibling = GetNextSiblingItem(itemSibling);
		}
	}
	Select(hSelectedGroup, TVGN_CARET);
	Expand(hSelectedGroup, TVE_EXPAND ) ;
}
#endif

UINT CIPCamTreeCtrl::fnPollingIPCamera(LPVOID lpParam)
{
	HANDLE hScanRequire =  OpenEvent(EVENT_ALL_ACCESS, TRUE, _T("Global\\SCAN_REQUIRE"));
	HANDLE hExitEvent  = OpenEvent(SYNCHRONIZE, FALSE, _T("Global\\IPCAM_EXPLORER_EXIT"));
	HANDLE hPauseScanIPCam =  CreateEvent(NULL, TRUE, TRUE, _T("Global\\PAUSE_IPCAM_DISCOVERY"));
	int ci=0;
	::Sleep(40000);
	::ResetEvent( hPauseScanIPCam );
	::CloseHandle(hPauseScanIPCam);
	::SetEvent(hScanRequire);
	while (WAIT_OBJECT_0 != (::WaitForSingleObject (hExitEvent, 10000)))
	{
		::SetEvent(hScanRequire);
		ci++;
		if (ci >9)
			break;
	}

	::CloseHandle(hScanRequire);
	return 0;
}

void CIPCamTreeCtrl::vGetGroupNodeItems(vector <NODEITEM*>& vtNodes)
{
	TVITEM TreeItem;
	HTREEITEM  hItem = GetSelectedItem();
	if (GetParentItem(hItem) != NULL)
	{
		hItem = GetParentItem(hItem);
	}
	HTREEITEM itemIPCam = GetNextItem(hItem,TVGN_CHILD);
	while (itemIPCam)
	{
		TreeItem.hItem = itemIPCam;
		TreeItem.mask = TVIF_HANDLE  ;
		GetItem(&TreeItem);
		NODEITEM* pNodeitem =(NODEITEM*) TreeItem.lParam;
		if (pNodeitem != NULL)
		{
			pNodeitem->hTreeItem = hItem;
			vtNodes.push_back(pNodeitem);				
		}				
		itemIPCam = GetNextSiblingItem(TreeItem.hItem);	
		//hItem = itemParents;
	}
}//vGetNodeItems
void CIPCamTreeCtrl::vGetAllNodeItems(vector <NODEITEM*>& vtNodes)
{
	if(!m_vctpAllNodesInTree.empty())
	{
		for(int i=0;i<m_vctpAllNodesInTree.size();i++)
		{
			if(m_vctpAllNodesInTree[i])
			vtNodes.push_back(m_vctpAllNodesInTree[i]);
		}
	}
}
void CIPCamTreeCtrl::vResetData()
{
}

LRESULT CIPCamTreeCtrl::OnDragList(WPARAM wParam, LPARAM lParam)
{
		return DefWindowProc(BM_SETSTYLE,
		(wParam & ~BS_TYPEMASK) | BS_OWNERDRAW, lParam);
}

 

int CIPCamTreeCtrl::vGetVideoNumber(int o_LayoutNo)
{
	//This function is to retrieve
	switch(o_LayoutNo)
	{
		case 1:
		case 3:	
		case 4:
		case 5:
		case 6:
			return o_LayoutNo * o_LayoutNo;
			break;

		case 22:
			return 4;
			break;

		case 15:
			return 6;
			break;
			
		case 17:
			return 8;
			break;

		case 28:
			return 10;
			break;

		case 121:
		case 122:
			return 13;
			break;

 	}

	return 1;
}

void CIPCamTreeCtrl::OnAddNewIpcam()
{
	//((CIPCamTreeView*)GetParent())->OnAddInternetIpCam();
	// TODO: Add your command handler code here

	HTREEITEM hItem = GetSelectedItem();
	if (hItem==NULL ) return;

	CString csGroupName = GetItemText(hItem);

	CDlgAddManageCam dlg(csGroupName, 1);

	if( dlg.DoModal() == IDOK)
	{
		return;
	}
}


void CIPCamTreeCtrl::OnMenuRemoveipcam32815()
{
	// TODO: Add your command handler code here
	//Get Camera Name 
	int ll_camid, ll_camidx, iCamera_idx;

	HTREEITEM  hItem = GetSelectedItem();

	TVITEM TreeItem;
	TreeItem.hItem = hItem;
	TreeItem.mask = TVIF_PARAM|TVIF_HANDLE ;
	GetItem(&TreeItem);	

	NODEITEM*  lpItem  =(NODEITEM*) TreeItem.lParam;
	
	CString ls_desc, ls_sql, csQuestion1, csQuestion2;
	CString ls_cam(lpItem->camera_name); 
	//csQuestion1.LoadString(CGS::g_hResStr, IDS_TREECTRL_REMOVE_CAM1);
	LOADSTRING(csQuestion1, IDS_TREECTRL_REMOVE_CAM1);
	ls_desc.Format(_T("%s %s"), csQuestion1,ls_cam ); 
	//csQuestion2.LoadString(IDS_TREECTRL_REMOVE_CAM2);
 	//ls_desc.Format(_T("%s %s %s"), csQuestion1,ls_cam,csQuestion2  ); 
	
	ll_camid = lpItem->cameraid;
	iCamera_idx = lpItem->camera_idx;

	if (AfxMessageBox( ls_desc, MB_OKCANCEL|MB_ICONQUESTION) ==2) return;   //Cancel
	

	//Get Group ID
	HTREEITEM  hpItem = GetParentItem(hItem);
	TVITEM Item;
		Item.hItem = hpItem;
		Item.mask = TVIF_HANDLE;
		GetItem(&Item);
		int ll_groupid =  Item.lParam;

		//ls_sql.Format( _T(" DELETE  FROM ec_camera_group_camera WHERE cameragroupid=%d AND cameraid=%d"), ll_groupid, ll_camid);
		ls_sql.Format(_T("DECLARE @REC_ID int; SELECT @REC_ID = camera_recordingid FROM ec_camera_recording WHERE camera_idx = %d; DELETE FROM ec_camera_recording_time WHERE camera_recordingid = @REC_ID; DELETE FROM ec_camera_group_camera WHERE cameraid = %d; DELETE FROM ec_stream WHERE cameraid = %d; DELETE FROM ec_camera WHERE cameraid = %d; DELETE FROM ec_camera_recording WHERE camera_idx = %d; DELETE FROM ecac_camera WHERE camera_idx = %d"), iCamera_idx, ll_camid, ll_camid, ll_camid, iCamera_idx, iCamera_idx);

		try
		{
				TRACE(ls_sql);
				m_pDB->ExecuteSQL(ls_sql);
				 DeleteItem(hItem);
		}
		catch(CDBException * p)
		{
				AfxMessageBox(p->m_strError);
		} 
	
		CString csTreeIDX(_T(""));
		csTreeIDX.Format(_T("(%d)"), iCamera_idx);

		//--Delete the Camera on the Tree Control.

		HTREEITEM hParentItem = GetRootItem();
		while(hParentItem)
		{
			HTREEITEM hChildItem = this->GetChildItem(hParentItem);
			while(hChildItem)
			{
				CString csItemText = this->GetItemText(hChildItem);
				if(csItemText.Find(csTreeIDX) != -1)	this->DeleteItem(hChildItem);

				hChildItem = this->GetNextItem(hChildItem, TVGN_NEXT);
			}
			hParentItem = this->GetNextSiblingItem(hParentItem);
		}

		//--Select Group Item 
		//CGS* m_Cgs = new CGS();
		//m_Cgs->bGetNodeItem();

		////USES_CONVERSION;

		//HTREEITEM hParentItem = GetRootItem();
		//while(hParentItem)
		//{
		//	HTREEITEM hChildItem = this->GetChildItem(hParentItem);
		//	int iMatchNum=0;
		//	while(hChildItem)
		//	{
		//		CString csItemText = this->GetItemText(hChildItem);
		//		for(int iTemp = 0;iTemp < m_Cgs->m_vctNode.size(); iTemp++)
		//		{
		//			CString csNodeItemIDX(_T(""));
		//			csNodeItemIDX.Format(_T("(%d)"), m_Cgs->m_vctNode[iTemp].camera_idx);
		//			if(csItemText.Find(csNodeItemIDX) != -1)	iMatchNum++;
		//		}

		//		hChildItem = this->GetNextItem(hChildItem, TVGN_NEXT);
		//	}
		//	if(iMatchNum == m_Cgs->m_vctNode.size()-1)	break;

		//	hParentItem = this->GetNextSiblingItem(hParentItem);
		//}

		//delete m_Cgs;

		//HTREEITEM hSelectItem = GetSelectedItem();
		//HTREEITEM hParentItem = this->GetParentItem(hSelectItem);
		this->SelectItem(hpItem);
		this->vLiveView();

		

		//--notify recording service
		CSA csa;
		HANDLE hCameraRecording = CreateEvent(&csa.m_sa, false, false, RECORDING_STATUS_CHANGE);
		if (hCameraRecording == NULL)	TRACE(_T("Error code :%d"), ::GetLastError());
		SetEvent( hCameraRecording );

		//--notify Event service
		HANDLE hRestartEvent = ::CreateEvent(&csa.m_sa, false, false, _T("Global\\ETROCENTER_EVENT_RESTART"));
		if(hRestartEvent == NULL)
		{TRACE(_T("Create Restart Event Error Code: %d"), ::GetLastError());}
		SetEvent(hRestartEvent);
		CloseHandle(hRestartEvent);
}

int CIPCamTreeCtrl::vAddMoreCamera(void)
{
	//This function is to add more cameras based on the settings in ec.ini file
	//Temp use ec.ini and will use database instead later.
		char* lpip; 
		CString ls_cam;
	int ll_cnt=0;
	CString ls_ip;

	CIniReader m_ini;
	m_ini.setINIFullFileName(_T("\\ec.ini"));

	

	//Based on the [video] totalcamera=2 to loop into the ini file to get the information
	//because, when deleting the camera might skip the camera number, and add the new camera
	//will use the min number available

	//Get the total camera number from video section of the ini file
	//char* lptotal = m_ini.getKeyValue(_T("totalcamera"),_T("video"));
	//int l_totalcam = atoi(lptotal); 
	CString lptotal = m_ini.getKeyValue(_T("totalcamera"),_T("video"));
	int l_totalcam = _ttoi( lptotal.GetBuffer( 0 ) ); 

	//if ll_totalcam
	while (true)
	{
		ll_cnt++;	
		ls_cam.Format(_T("camera%d"), ll_cnt);
		

		//lpip = m_ini.getKeyValue(_T("ip"),ls_cam);
		//CString ls_ip = (LPCTSTR)lpip;
		CString ls_ip = m_ini.getKeyValue(_T("ip"),ls_cam);
		if (ls_ip == _T("")) break;
	}
	return 0;
}

void CIPCamTreeCtrl::InsertDBRoot()
{

	//Get the group from ec_camera_group
	bool lb_ret;
	CString ls_desc, ls_id, ls_default;
	int ci=0, ll_groupid;
	CDBVariant varValue;	
	HTREEITEM itemDefault;
	m_vctpAllNodesInTree.clear();
	m_vctcsGroupNames.clear();

	m_csGroup[MAINGROUP].GroupName =   CProductDefine::m_tzProductName;
    m_csGroup[MAINGROUP].GroupName +=  _T(" Group");
	const int nDefaultSize = 2;
 
	CRecordset m_rs(m_pDB);
	m_rs.Open(CRecordset::dynaset,_T("select * from ec_camera_group order by cameragroupid"));
	m_rs.MoveLast();
	m_nGroup = m_rs.GetRecordCount();
	m_rs.MoveFirst();
	
	LPWSTR mynew;
	HTREEITEM itemSpec[MAX_GROUP];

	while (!m_rs.IsEOF()) { 
				
		m_rs.GetFieldValue(_T("cameragroupid"), varValue, SQL_C_SSHORT);
		ll_groupid = varValue.m_iVal;
		m_rs.GetFieldValue(_T("camera_group_desc"), ls_desc);
		m_rs.GetFieldValue(_T("default_"), ls_default);

		TVINSERTSTRUCTW itemStru;
		itemStru.hParent = TVGN_ROOT;
		itemStru.item.lParam = ll_groupid; //Save groupid into lparam
		itemStru.item.mask = TVIF_PARAM | TVIF_TEXT;
	 
		itemStru.item.pszText = (LPWSTR)(LPCTSTR)ls_desc;
		itemStru.item.cchTextMax =ls_desc.GetLength();

		itemSpec[ci] = InsertItem(itemStru.item.mask, (LPCTSTR)ls_desc, 0,0,0,0, ll_groupid, NULL, TVI_LAST);
		//Select the default group
		if (ls_default.Trim().MakeUpper()==_T("Y")) 
			itemDefault = itemSpec[ci];

		InsertDBCamera(ll_groupid, itemSpec[ci]);
		
		//Build Array
		m_csGroup[ci].GroupId = ll_groupid;
		m_csGroup[ci].GroupName = ls_desc.Trim();
		m_vctcsGroupNames.push_back(ls_desc.Trim());
		m_maxGroupId = ll_groupid;

		ci++;
		m_nGroup = ci;
		m_rs.MoveNext();  
		
	} 

	m_rs.Close();
	if (itemDefault != NULL) SelectItem(itemDefault);

}


void CIPCamTreeCtrl::InsertDBCamera(int o_groupid, HTREEITEM hParent)
{
	USES_CONVERSION;
	//Get Groups
	//m_csGroup[MAINGROUP].GroupName = _T("Etro Center Group");// _T("UnAssigned");
	//m_csGroup[GROUP01].GroupName =  _T("Group01");
	//const int nDefaultSize = 2;
	HTREEITEM lprvItem;

	char cIp[16];
	CDBVariant varCamidx, varStreamPort, varCamIdx, varCamid,varWebPort;
	CString ls_sql, ls_sql1, ls_desc, ls_camname, ls_ipaddress, ls_macaddress, ls_idx, ls_username, ls_password, ls_sprotocol,ls_webPort;
	CString csStreamName, csModelCode, csStreamURL;
	
	NODEITEM lNode={0};
	NODEITEM* lpNode;
	lpNode = &lNode;

	CRecordset m_rs(m_pDB);

	 
	ls_sql1 = _T("SELECT stream_url, stream_name, ec_camera.cameraid, ec_camera.camera_idx,  ec_camera.cameraname, ec_camera.ipaddress, ec_camera.mac_address,ec_camera.httpport, ec_camera.username, ec_camera.password, ec_camera.model_code, ec_stream.stream_port, ec_stream.stream_protocol, ec_camera_group_camera.cameragroupid ");
    //ls_sql1 += _T(" FROM ec_camera INNER JOIN ec_camera_group_camera ON ec_camera_group_camera.cameraid = ec_camera.cameraid LEFT OUTER JOIN ec_stream ON ec_stream.cameraid = ec_camera.cameraid and ec_stream.streamid = (SELECT Top 1 streamid from ec_stream where ec_stream.cameraid=ec_camera.cameraid) ");
	ls_sql1 += _T(" FROM ec_camera INNER JOIN ec_camera_group_camera ON ec_camera_group_camera.cameraid = ec_camera.cameraid LEFT OUTER JOIN ec_stream ON ec_stream.cameraid = ec_camera.cameraid");
	ls_sql1 +=	_T(" WHERE stream_type like '%LV%' and ec_camera_group_camera.cameragroupid= ");
    ls_sql.Format(_T("%s %d"), ls_sql1 , o_groupid);
	ls_sql = ls_sql + _T(" AND active_='Y' ORDER BY ec_camera.camera_idx");

	bool lb_ret = m_rs.Open(CRecordset::dynamic, ls_sql);

	if (m_rs.GetRecordCount()<=0)
	{
		m_rs.Close();
		return;
	}
		 
	m_rs.MoveFirst();
	int ci=0;
	LPWSTR mynew;

	long  ll_camIdxField = 0, ll_streamPortField=1;
	 
	
	while(!m_rs.IsEOF()   ) 
	{ 	  
		NODEITEM* m_camInfo = new NODEITEM;
		::ZeroMemory(m_camInfo, sizeof(NODEITEM));
		TVINSERTSTRUCTW itemStru={0};
		
		//--Get Stream URL
		m_rs.GetFieldValue(_T("stream_url"), csStreamURL);
		csStreamURL.TrimRight();
		_tcscpy(m_camInfo->stream_url, csStreamURL.GetBuffer());

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
			//lpNode->cameraid = varCamid.m_iVal;

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
		//ls_ipaddress.Trim();
		//strncpy(lpNode->ip, W2A(ls_ipaddress), ls_ipaddress.Trim().GetLength());
		//_tcscpy(lpNode->ip, ls_ipaddress.GetBuffer(32));	
		ls_ipaddress = ls_ipaddress.Trim() +'\0';
		//strncpy(lc_ip, W2A(ls_ipaddress), ls_ipaddress.GetLength());
		strncpy(m_camInfo->ip , W2A(ls_ipaddress), ls_ipaddress.GetLength());
		//strncpy(m_camInfo->ip, W2A(ls_ipaddress), 12);
	
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
		//strncpy(lNode.szUser, W2A(ls_username), ls_username.GetLength());
		strncpy(m_camInfo->szUser, W2A(ls_username), ls_username.GetLength());

		//password
		m_rs.GetFieldValue(_T("password"), ls_password);
		ls_password = ls_password.Trim()+'\0';
		strncpy(m_camInfo->szPass, W2A(ls_password), ls_password.GetLength());

		//--NVR Brand
// 		m_rs.GetFieldValue(_T("model_code"), csModelCode);
// 		csModelCode.Trim();
// 		if(csModelCode.Find(_T("REMOTE_ETROCENTER")) != -1)
// 			m_camInfo->nENVR = NODEITEM::REMOTE_ETROCENTER;
// 		else if(csModelCode.Find(_T("ISAP")) != -1)
// 			m_camInfo->nENVR = NODEITEM::ISAP;
// 		else if(csModelCode.Find(_T("NVR")) != -1)
// 			m_camInfo->nENVR = NODEITEM::REMOTE_ETROCENTER;
// 		else
// 			m_camInfo->nENVR = NODEITEM::LOCAL;

        strcpy(m_camInfo->modelname, W2A(csModelCode));

		//Stream port
		m_rs.GetFieldValue(_T("stream_port"), varStreamPort, SQL_C_SLONG);
		if (varStreamPort.m_iVal < 0)
			//lNode.nAPPort = DEFAULTSPORT;
			m_camInfo->nAPPort = DEFAULTSPORT;
		else
			m_camInfo->nAPPort = varStreamPort.m_iVal;
			//lNode.nAPPort = varStreamPort.m_iVal;

		//stream protocol
		m_rs.GetFieldValue(_T("stream_protocol"), ls_sprotocol);
		ls_sprotocol.Trim();
		if  (ls_sprotocol=="AVIFILE")
			m_camInfo->nProtocol = AVIFILE;
		else if (ls_sprotocol=="RTPTCP")
			m_camInfo->nProtocol = RTPOVERTCP;
		else if (ls_sprotocol=="RTPHTTP")
        {
            m_camInfo->nProtocol = RTPOVERHTTP;
            m_camInfo->nAPPort = m_camInfo->nWebPort;
        }
		else if (ls_sprotocol=="RTSP")
			m_camInfo->nProtocol = RTSP;
		else if (ls_sprotocol=="CGI")
			m_camInfo->nProtocol = CGI;
		else if (ls_sprotocol=="CGI_ISAP")
		{
			m_camInfo->nProtocol = CGI_ISAP;

		}
		else
			m_camInfo->nProtocol = VSMP;


		/*itemStru.item.mask = TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM ;
		itemStru.item.pszText = (LPWSTR)(LPCTSTR)ls_desc;
		itemStru.item.cchTextMax =ls_desc.GetLength();*/

		//itemStru.item.lParam = varField.m_iVal;

		ls_desc.Format( _T("(%d) %s - %s"),varCamidx.m_iVal, ls_camname, ls_ipaddress);

		lpNode = &lNode;
		HTREEITEM lItem = InsertItem( TVIF_TEXT | TVIF_HANDLE | TVIF_PARAM, (LPCTSTR)ls_desc, 0,0,0,0, (LPARAM)m_camInfo, hParent, TVI_LAST);
			//HTREEITEM lItem = m_CamGroup.InsertItem(itemStru.item.mask, (LPCTSTR)ls_desc, 0,0,0,0, varField.m_iVal, hParent, TVI_LAST);

		// Save the current grown tree items into member vector
		m_camInfo->hTreeItem = lItem;
		m_camInfo->nGroup = o_groupid;
		m_vctpAllNodesInTree.push_back(m_camInfo);
		//====== 
	
		SetItemImage(lItem,4,4);

	/*	if (varField.m_iVal >=0)
		{
			gNodeItem[varField.m_iVal] = lNode;
		}
	      */
      m_rs.MoveNext(); 
	} 

	m_rs.Close();
}

int CIPCamTreeCtrl::vInsertNodeItem(int o_index, NODEITEM* pNodeItem)
{
	 
	//gNodeItem[o_index]. = pNodeItem;
	return 0;
}

//NODEITEM* CIPCamTreeCtrl::vGetNodeItem(int o_index)
//{
//	NODEITEM* lcurNode;
//	lcurNode = *gNodeItem[o_index];
//	
//	return 0;
//}

//NODEITEM CIPCamTreeCtrl::vGetNodeItem(int oCamIndex)
//{
//	return gNodeItem[oCamIndex];
//}

bool CIPCamTreeCtrl::vDeleteGroup(int o_GroupId)
{
	if (o_GroupId <=0) return false;
	try 
	{
		CString ls_sql(_T("")), csCameraID(_T("")), csGroupID(_T(""));

		/*ls_sql.Format(_T("SELECT cameraid FROM ec_camera_group_camera GROUP BY cameraid HAVING count(*)>1 INTERSECT SELECT cameraid FROM ec_camera_group_camera WHERE cameragroupid = %d"), o_GroupId);
		CRecordset rCameraID(m_pDB);
		rCameraID.Open(CRecordset::forwardOnly, ls_sql);
		while(!rCameraID.IsEOF()) {

			rCameraID.GetFieldValue(_T("cameraid"), csCameraID);
			vectorDoubleCameraID.push_back(::_ttoi(csCameraID));
		
			rCameraID.MoveNext();
		}*/

		ls_sql.Format(_T("SELECT cameragroupid FROM ec_camera_group WHERE cameragroupid > %d"), o_GroupId);
		CRecordset rGroupID(m_pDB);
		rGroupID.Open(CRecordset::forwardOnly, ls_sql);
		while(!rGroupID.IsEOF()){
		
			rGroupID.GetFieldValue(_T("cameragroupid"), csGroupID);
			vectorGroupID.push_back(::_ttoi(csGroupID));
	
			rGroupID.MoveNext();
		}

		ls_sql.Format(_T("DELETE FROM ec_camera_group WHERE cameragroupid=%d"), o_GroupId);
		m_pDB->ExecuteSQL(ls_sql);

		for(int iTemp = 0; iTemp < vectorGroupID.size(); iTemp++)
		{
			ls_sql.Format(_T("UPDATE ec_camera_group SET cameragroupid='%d', camera_group_code='GROUP%d' WHERE cameragroupid = '%d'"), o_GroupId+iTemp, o_GroupId+iTemp, vectorGroupID[iTemp]);
			m_pDB->ExecuteSQL(ls_sql);

			ls_sql.Format(_T("UPDATE ec_camera_group_camera SET cameragroupid='%d' WHERE cameragroupid='%d'"), o_GroupId+iTemp, vectorGroupID[iTemp]);
			m_pDB->ExecuteSQL(ls_sql);
		}

		/*ls_sql.Format(_T("DELETE FROM ec_camera_group_camera WHERE cameragroupid=%d"), o_GroupId);
		m_pDB->ExecuteSQL(ls_sql);*/
	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	return true;
}

//bool CIPCamTreeCtrl::vRefreshTree(void)
//{
//	vDBConnect();
//	vDeleteTree();
//	InsertDBRoot();
//	return false;
//}


//bool CIPCamTreeCtrl::vDBConnect(void)
//{
//	// Database m_DB - 
//
//	
//	//m_DB = ((CMainFrame*)AfxGetMainWnd())->m_DB;
//
//	//CIniReader m_ini(ECINI);
//
//	//CString cpDns = m_ini.getKeyValue(_T("DSN"),_T("database"));
//	//CString cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
//	//CString cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
// //
//	//CString ls_connect;
//	//ls_connect.Format(_T("ODBC;UID=%s;PWD=%s"), cpUid, cpPwd);
//	//bool lb_ret = m_DB.Open((LPCTSTR)cpDns, FALSE, FALSE, ls_connect );
//	return false;
//}

bool CIPCamTreeCtrl::vSelectCam(NODEITEM* pNode)
{
	/* This function is used when click on a video, then highlight a tree item*/
	HTREEITEM  hrItem =iCurGroupItem;
	

	if (hrItem==NULL ) return false;
    
	HTREEITEM  hItem = GetChildItem(hrItem);
	
	while (hItem)
	{	
		TVITEM Item;
		Item.hItem = hItem;
		Item.mask = TVIF_HANDLE;
		GetItem(&Item);
		NODEITEM* pNodeitem =(NODEITEM*) Item.lParam;
		if (pNodeitem ==  pNode)
		{
			SelectItem(hItem);
			return true;
		}
		hItem = GetNextSiblingItem(hItem);
		
	}

	return false;
}

void CIPCamTreeCtrl::vEventLiveview(HTREEITEM pItem, int oi_AlertLevel)
{
	/* This function is called from Event Triggered, then display live view at cell 1 */
		TVITEM Item;
		Item.hItem = pItem;
		Item.mask = TVIF_HANDLE;
		GetItem(&Item);
		
		//((CMainFrame*)AfxGetMainWnd())->vFreeVideoSet();
		//((CMainFrame*)AfxGetMainWnd())->vAlocateViewCell(1);
		//NODEITEM* pNodeitem =(NODEITEM*) Item.lParam;
	/*	if (pNodeitem>0)
			((CMainFrame*)AfxGetMainWnd())->vVideoPresent(pNodeitem, true);*/

}

NODEITEM* CIPCamTreeCtrl::vGetNodeitemFromIP(CString o_IP)
{
	USES_CONVERSION;
	CString	csIP=_T("");
	HTREEITEM itemRoot = GetRootItem();
	HTREEITEM itemChild;

	while (itemRoot) //Loop into all the groups/Cameras to find the noteitem
	{	
		itemChild = GetChildItem(itemRoot);
		while (itemChild)
		{
			TVITEM Item;
			Item.hItem = itemChild;
			Item.mask = TVIF_HANDLE;
			GetItem(&Item);
			NODEITEM* pNodeitem =(NODEITEM*) Item.lParam;
			csIP.Format(_T("%s"), A2W(pNodeitem->ip));
			if (csIP.Trim()==o_IP.Trim())
			{
				return pNodeitem;
				//return true;
			}
			itemChild = GetNextSiblingItem(itemChild);
		}
	  	itemRoot = GetNextSiblingItem(itemRoot);
	}
	return NULL;
}

void CIPCamTreeCtrl::OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// Return -1 will force tree control NOT allow edit
	*pResult = -1;
}


void CIPCamTreeCtrl::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;

	// Item user started dragging ...
	m_hitemDrag = pNMTreeView->itemNew.hItem;
	m_hitemDrop = NULL;

	m_pDragImage = CreateDragImage(m_hitemDrag);  // get the image list for dragging
	// CreateDragImage() returns NULL if no image list
	// associated with the tree view control
	if( !m_pDragImage )
		return;

	m_bLDragging = TRUE;
	((CMainFrame*)AfxGetMainWnd())->vIsDragging(m_bLDragging);
	m_nDropRectIdx = -1;
	m_pDragImage->BeginDrag(0, CPoint(-15,-15));
	POINT pt = pNMTreeView->ptDrag;
	ClientToScreen( &pt );
	m_pDragImage->DragEnter(NULL, pt);
	SetCapture();
}
int CIPCamTreeCtrl::nNextAvailableLayOut(int nCurrentLayOut)
{
	switch(nCurrentLayOut)
	{
	case 0:
		return 22;
		break;
	case 1:
		return 22;
		break;
	case 22:
		return 15;
		break;
	case 3:
		return 10;
		break;
	case 4:
		return 17;
		break;
	case 28:
		return 4;
		break;
	case 121:
		return 4;
		break;
	case 122:
		return 4;
		break;
	case 116:
		return 116;
		break;
	default :
		return 4;
		break;
	}

}
bool CIPCamTreeCtrl::vChangeCamGroup(int o_OldGroupid, int o_NewGroupid, int o_CamId)
{
	if (o_OldGroupid<1 || o_NewGroupid<1 || o_CamId<1) 
		return false;

	//Check if the camera already in the group
	CRecordset m_rs(m_pDB);
	CString ls_sql;

	try{
		ls_sql.Format(_T("SELECT * FROM  ec_camera_group_camera WHERE cameragroupid = %d AND cameraid=%d"),o_NewGroupid, o_CamId);
		
		m_rs.Open(CRecordset::dynaset, ls_sql);
 
		int ll_tot = m_rs.GetRecordCount();
		if (ll_tot>0) return false;  //Already has such rows

		ls_sql.Format(_T("UPDATE  ec_camera_group_camera SET cameragroupid = %d WHERE cameraid=%d AND cameragroupid=%d "), o_NewGroupid, o_CamId, o_OldGroupid);
		m_pDB->ExecuteSQL(ls_sql);
	}
		catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}

	DeleteAllItems();
	InsertDBRoot();

	HTREEITEM hRoot = this->GetRootItem();
	while(hRoot)
	{
		this->Expand(hRoot, TVE_EXPAND);
		hRoot = this->GetNextSiblingItem(hRoot);
	}	

	return true;
}
void CIPCamTreeCtrl::OnMenuFlyaway()
{
	HTREEITEM hItem = GetSelectedItem();

	HTREEITEM hItemParent = GetParentItem(hItem);

	if(hItemParent) 
	{
		vFlyaway(1);
	}
	else
	{
		HTREEITEM itemIPCam = GetNextItem(hItem, TVGN_CHILD);
		
		if(!itemIPCam) return;

		int lret(0);
		CDlgMatrixTag* lMatrixDlg = new CDlgMatrixTag;
		lMatrixDlg->m_bSetStyle = true;
		lret = lMatrixDlg->DoModal();
		
		if (lret==2 || lret == -1 || lret == -2 || lret == -3) 
		{
			if (lMatrixDlg) delete lMatrixDlg;
			return; //Escape key
		}
		if(lret == 0) 
			lret = 1;
		if (lMatrixDlg) 
			delete lMatrixDlg;		
		vFlyaway(lret);
	}
}
void CIPCamTreeCtrl::vFlyaway(int nIdx)
{
	vector <NODEITEM*> vtNode;
	HTREEITEM hItem = GetSelectedItem();
	CString csGroupName = GetItemText(hItem);

	vSetGroupFlyAway(csGroupName);
	bool bFlyAway = bGetGroupFlyAway(csGroupName);
	

	HTREEITEM itemIPCam = GetNextItem(hItem, TVGN_CHILD);
	if (itemIPCam)
	{
		TVITEM lTreeItem={0};		
		while (itemIPCam)
		{					
			lTreeItem.hItem = itemIPCam;
			lTreeItem.mask = TVIF_HANDLE;
			GetItem(&lTreeItem);
			NODEITEM* pNodeitem = (NODEITEM*)lTreeItem.lParam;
			vtNode.push_back(pNodeitem);					
			itemIPCam = GetNextSiblingItem(lTreeItem.hItem);
		}
		
	}else
	{
		TVITEM lTreeItem={0};		
		lTreeItem.hItem = hItem;
		lTreeItem.mask = TVIF_HANDLE;
		GetItem(&lTreeItem);
		NODEITEM* pNodeitem = (NODEITEM*)lTreeItem.lParam;
		vtNode.push_back(pNodeitem);					
	}

	CRect rect;
	GetDesktopWindow()->GetWindowRect(&rect);  

	((CMainFrame*)AfxGetMainWnd())->vCreateDlgFlyAway(nIdx, vtNode, rect, rect, ((CMainFrame*)AfxGetMainWnd())->m_bAutoScan);

}


void CIPCamTreeCtrl::vSetGroupFlyAway(CString csGroupName)
{
	map<CString,bool>::iterator it;

	for(it=m_mapGroupFlyAway.begin();it!=m_mapGroupFlyAway.end();it++)
	{
		if(it->first == csGroupName)
		{
			it->second^=1;
			break;
		}
	}
}

bool CIPCamTreeCtrl::bGetGroupFlyAway(CString csGroupName)
{
	map<CString,bool>::iterator it;

	for(it = m_mapGroupFlyAway.begin();it!=m_mapGroupFlyAway.end();it++)
	{

		if(it->first == csGroupName)
		{
			return it->second;
			break;
		}
	}

	return false;
	 
}

void CIPCamTreeCtrl::vCheckLVExist()
{
	CString csSQL(_T("")), csCameraID(_T("")), csUpdate(_T(""));
	vector<CString> veCameraID;
	CRecordset m_rs(m_pDB);

	try
	{
		csSQL.Format(_T("SELECT DISTINCT cameraid FROM ec_stream"));
		CRecordset rCameraID(m_pDB);
		rCameraID.Open(CRecordset::forwardOnly, csSQL);
		while(!rCameraID.IsEOF()){
		
			rCameraID.GetFieldValue(_T("cameraid"), csCameraID);
			veCameraID.push_back(csCameraID);
	
			rCameraID.MoveNext();
		}

		rCameraID.Close();

		for(int iTemp = 0; iTemp < veCameraID.size(); iTemp++)
		{
			csSQL = _T("SELECT stream_name FROM ec_stream WHERE stream_type like '%LV%' AND cameraid = ") + veCameraID[iTemp];
			
			m_rs.Open(CRecordset::dynaset, csSQL);
			int iLVExist = m_rs.GetRecordCount();
			m_rs.Close();
	
			if(iLVExist == 0)
			{
				csUpdate.Format(_T("UPDATE ec_stream SET stream_type = 'LV' WHERE cameraid = %s AND stream_name = 'Stream 1'"), veCameraID[iTemp]);

				m_pDB->ExecuteSQL(csUpdate);
			}
		}

	}
	catch(CDBException * p)
	{
		AfxMessageBox(p->m_strError);
		p->Delete();
	}
}

void CIPCamTreeCtrl::vGetGroupCam(vector<NODEITEM*>& vctNode)
{
	int nGroupId(0);

	for(int j = 0; j < 	((CMainFrame*)AfxGetMainWnd())->m_vctnGroupAutoPlayIdx.size(); j++)
	{
		nGroupId = ((CMainFrame*)AfxGetMainWnd())->m_vctnGroupAutoPlayIdx[j];

		for(int i=0;i<m_vctpAllNodesInTree.size();i++)
		{
			if(m_vctpAllNodesInTree[i]->nGroup == nGroupId)
			{
				vctNode.push_back(m_vctpAllNodesInTree[i]);
			}
		}
	}
}

void CIPCamTreeCtrl::vGetGroupCam(int nGroupId, vector<NODEITEM*>& vctNode)
{	
	for(int i=0;i<m_vctpAllNodesInTree.size();i++)
	{
		if(m_vctpAllNodesInTree[i]->nGroup == nGroupId)
		{
			vctNode.push_back(m_vctpAllNodesInTree[i]);
		}		
	}
}
// check if there is cam within a group
bool CIPCamTreeCtrl::bCheckGroupCamNum(int nGroupId)
{
	for(int i=0;i<m_vctpAllNodesInTree.size();i++)
	{
		if(m_vctpAllNodesInTree[i]->nGroup == nGroupId)
		{
			return true;
		}
	}

	return false;
}

int CIPCamTreeCtrl::nGetCurrentSelectedGroupId(void)
{
	TVITEM TreeItem;
	HTREEITEM  hItem = GetSelectedItem();

	if (GetParentItem(hItem) != NULL)
	{
		hItem = GetParentItem(hItem);
	}
	
	if (hItem)
	{
		int nGroupId(0);
		TreeItem.hItem = hItem;
		TreeItem.mask = TVIF_HANDLE  ;
		GetItem(&TreeItem);
		nGroupId = TreeItem.lParam;	
		return nGroupId;							
	}	
	else
	{
		return -1;
	}

}
#ifdef PLUGIN
void CIPCamTreeCtrl::vANPR()
{
	HTREEITEM  hItem = GetSelectedItem();
	HTREEITEM parentItem = GetParentItem(hItem);
	m_bLDragging = FALSE;
	((CMainFrame*)AfxGetMainWnd())->vIsDragging(m_bLDragging);

	((CMainFrame*)AfxGetMainWnd())->m_bEventLV = false;
	((CMainFrame*)AfxGetMainWnd())->vSetEventLV();
	TVITEM lTreeItem={0};
	lTreeItem.hItem = hItem;
	lTreeItem.mask = TVIF_HANDLE  ;
	int ll_ret = GetItem(&lTreeItem);
	NODEITEM* pNodeitem =(NODEITEM*) lTreeItem.lParam;
	
	CDlgANPR m_ANPR(pNodeitem);
	m_ANPR.DoModal();
}
#endif