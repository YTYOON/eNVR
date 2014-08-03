// TaskTreeCtrl.cpp : implementation file
//

#include "stdafx.h" 
#include "NVR.h"
#include "TaskTreeCtrl.h"
#include "IPCamTreeView.h"
#include "IPCamTreeCtrl.h"
#include "NVR.h"
#include "CGIVar.h"
//#include "DlgAddManageCam.h"
#include "MainFrm.h"
#include "DlgSettingCamGroup.h"
#include "DlgSettingMulMonitor.h"
#include "DlgSettingRecording.h"
#include "DlgMassAssignIP.h"
#include "DlgFtp.h"
#include "DlgNAS.h"
#include "DlgProductTest.h"
#include "EncpSettingPage.h"
#include "AddDevice.h"
#include "ConfigSettingDlg.h"
#include "VirtualKeyboard.h"
// CTaskTreeCtrl


IMPLEMENT_DYNAMIC(CTaskTreeCtrl, CTreeCtrl)
IMPLEMENT_DYNAMIC(CTaskDeviceTree, CTaskTreeCtrl)
IMPLEMENT_DYNAMIC(CTaskLayoutTree, CTaskTreeCtrl)

CTaskTreeCtrl::CTaskTreeCtrl():m_pDragImage(NULL),m_bLDragging(false),m_hitemDrag(NULL),m_hitemDrop(NULL),m_bPtInRect(false),
bReEntry(false), isPrvGroupName(_T("")), ibDiffGroup(false),m_bRefreshVideo(false),m_bOnlyOneCamChoosen(false),m_nDropRectIdx(-1),m_bFlyAway(false)
{
	pDoc = new CIPCamDiscovery();
	bool lb_open=true;
	m_bDrag_cursor_no=0;
}

CTaskTreeCtrl::~CTaskTreeCtrl()
{
    delete (CIPCamDiscovery*)pDoc;
}

BEGIN_MESSAGE_MAP(CTaskTreeCtrl, CTreeCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CTaskTreeCtrl::OnTvnBegindrag)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CTaskTreeCtrl::OnDblclk)
    ON_NOTIFY_REFLECT(NM_RCLICK, &CTaskTreeCtrl::OnRClick)
	ON_COMMAND(ID_MODIFY_IPCAM, &CTaskTreeCtrl::OnModifyDevice)	
	ON_COMMAND(ID_ADD_INTERNET_IP_CAM, &CTaskTreeCtrl::OnAddInternetIpCam)
	ON_COMMAND(IDC_IE_EXPLORER, &CTaskTreeCtrl::OnIeExplorer)
	ON_COMMAND(ID_MENU_REMOVEIPCAM32815, &CTaskTreeCtrl::OnMenuRemoveipcam)
	ON_COMMAND(IDC_TREECTRL_NEW_GROUP, &CTaskTreeCtrl::OnTreectrlNewGroup)
	ON_COMMAND(IDC_TREECTRL_MODIFY_GROUP, &CTaskTreeCtrl::OnTreectrlModifyGroup)
	ON_COMMAND(IDC_TREECTRL_REMOVE_GROUP, &CTaskTreeCtrl::OnTreectrlRemoveGroup)
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(MsgUpdateView, OnUpdateView)
	//ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CTaskTreeCtrl::OnNMCustomdraw)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CTaskDeviceTree, CTaskTreeCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CTaskTreeCtrl::OnTvnBegindrag)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CTaskTreeCtrl::OnDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CTaskTreeCtrl::OnRClick)
	ON_COMMAND(ID_MODIFY_IPCAM, &CTaskTreeCtrl::OnModifyDevice)	
	ON_COMMAND(ID_ADD_INTERNET_IP_CAM, &CTaskTreeCtrl::OnAddInternetIpCam)
	ON_COMMAND(IDC_IE_EXPLORER, &CTaskTreeCtrl::OnIeExplorer)
	ON_COMMAND(ID_MENU_REMOVEIPCAM32815, &CTaskTreeCtrl::OnMenuRemoveipcam)
	ON_COMMAND(IDC_TREECTRL_NEW_GROUP, &CTaskTreeCtrl::OnTreectrlNewGroup)
	ON_COMMAND(IDC_TREECTRL_MODIFY_GROUP, &CTaskTreeCtrl::OnTreectrlModifyGroup)
	ON_COMMAND(IDC_TREECTRL_REMOVE_GROUP, &CTaskTreeCtrl::OnTreectrlRemoveGroup)
	ON_COMMAND(ID_ADD_NEW_IPCAM, &CTaskDeviceTree::OnAddNewIpcam)
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(MsgUpdateView, OnUpdateView)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CTaskLayoutTree, CTaskTreeCtrl)
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CTaskTreeCtrl::OnTvnBegindrag)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CTaskTreeCtrl::OnDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CTaskTreeCtrl::OnRClick)
	ON_COMMAND(ID_MODIFY_IPCAM, &CTaskTreeCtrl::OnModifyDevice)	
	ON_COMMAND(ID_ADD_INTERNET_IP_CAM, &CTaskTreeCtrl::OnAddInternetIpCam)
	ON_COMMAND(IDC_IE_EXPLORER, &CTaskTreeCtrl::OnIeExplorer)
	ON_COMMAND(ID_MENU_REMOVEIPCAM32815, &CTaskTreeCtrl::OnMenuRemoveipcam)
	ON_COMMAND(IDC_TREECTRL_NEW_GROUP, &CTaskTreeCtrl::OnTreectrlNewGroup)
	ON_COMMAND(IDC_TREECTRL_MODIFY_GROUP, &CTaskTreeCtrl::OnTreectrlModifyGroup)
	ON_COMMAND(IDC_TREECTRL_REMOVE_GROUP, &CTaskTreeCtrl::OnTreectrlRemoveGroup)
	ON_COMMAND(ID_ADD_NEW_IPCAM, &CTaskLayoutTree::OnAddNewIpcam)
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(MsgUpdateView, OnUpdateView)
	ON_COMMAND_RANGE(ID_SELECTSREAM_STREAM, ID_SELECTSREAM_STREAM+4, &CTaskLayoutTree::OnRangeCmds)
END_MESSAGE_MAP()

void CTaskTreeCtrl::OnRClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TRACE0("CMyTreeCtrl::OnRClick()\n");
	// Send WM_CONTEXTMENU to self
	SendMessage(WM_CONTEXTMENU, (WPARAM) m_hWnd, GetMessagePos());
	// Mark message as handled and suppress default handling
	*pResult = 1;
}
bool CTaskTreeCtrl::bCheckGroupCamNum(int nGroupId)
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

void CTaskTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM  hItem = GetSelectedItem();
	ExpandItem(hItem);
	HTREEITEM parentItem = GetParentItem(hItem);
	m_bLDragging = FALSE;
	m_pbkTalk->GetMainFrame()->vIsDragging(m_bLDragging);

	if( m_pbkTalk->GetMainFrame()->m_bGroupAutoPlay && m_pbkTalk->GetMainFrame()->m_bAutoScan)
	{
		CString csMsg;
		LOADSTRING(csMsg,IDS_GROUP_AUTOSCAN_ASK);// = _T("Group AutoScan is running, would you like to Pause it ?");
		if(IDYES == AfxMessageBox(csMsg,MB_YESNO))
		{
			// pause autoscan
			m_pbkTalk->GetMainFrame()->vPauseLiveView();
		}
		else
		{
			m_pbkTalk->GetMainFrame()->vSetGroupAutoScan(false);
		}	
	}

	TreeNode* pNodeitem = GetTreeNode(hItem);
	if( pNodeitem &&
		(pNodeitem->nodeType == TreeNode::TreeNdoeType::Grp_Node || 
		pNodeitem->nodeType == TreeNode::TreeNdoeType::Dvs_Node))
	{
		m_pbkTalk->SendMessage(MsgUpdateView,UM_FREE_VIDEO,NULL);
		vLiveView();
	}
	else // node item , use 1 matrix to display the video
	{
		m_pbkTalk->GetMainFrame()->m_bEventLV = false;
		m_pbkTalk->GetMainFrame()->vSetEventLV();
		vector <NODEITEM*> vtNode;
		GetNodeItmFromTreeNode(hItem, vtNode);
		if (vtNode.size())
		{	
			m_pbkTalk->SendMessage(MsgUpdateView,UM_FREE_VIDEO,NULL);
			m_pbkTalk->GetMainFrame()->vAlocateViewCell(1); // allocate only 1 matrix cell
			m_pbkTalk->GetMainFrame()->vVideoPresent(vtNode);
		
			m_bOnlyOneCamChoosen = true;
			isPrvGroupName = GetItemText(hItem);

			//Larry add 102-12-10 fix Matrix show 1x1 on  Dblclk//
			m_pbkTalk->GetMainFrame()->vSetMatrixBtnHot(IDB_BITMAP_1X1);
			/////////////////////////////////////////////////////////
		}
		::PostMessage(GetParent()->m_hWnd,ID_CHANGE_BMP,(WPARAM)1,NULL);// send msg to TreeView to change bmp
	}
	return;
}

int CTaskTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CIniReader m_ini;
	m_ini.setINIFullFileName(_T("\\ec.ini"));

	CString csCellSize = m_ini.getKeyValue(_T("matrix"),_T("video"));
	if(csCellSize.IsEmpty()) csCellSize = _T("1"); 	
	int nCellSize = _ttoi(csCellSize);

	//UINT nRet;
	//HANDLE hThRefresh = (HANDLE)_beginthreadex(NULL, 0, RefreshThread, this, 0, &nRet);
	//CloseHandle( hThRefresh );

	LONG nStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	nStyle = nStyle | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | WS_BORDER | TVS_SHOWSELALWAYS | TVS_NOHSCROLL;
	SetWindowLong(m_hWnd, GWL_STYLE, nStyle);
	
	m_pbkTalk = CNVRBk::GetBkObject();

	//--Make sure every camera has a LV.
	//vCheckLVExist();	
	vInitImages();
	SetDefaultCursor();
	CreateTreeNode(GetTreeType());

	::SetEvent( hDlgInitialize );

	SetTextColor(FONTCOLOR);
	SetBkColor(GLOBALTREEBKCOLOR);

	return 0;
}

void CTaskTreeCtrl::vCheckLVExist()
{
	map<int, vector<ec_Stream> > mapStream;
	map<int, vector<ec_Stream> >::iterator it;
	vector<ec_Stream>::iterator itvc;
	vector<ec_Stream> vcUpdateStream;
	bool bFound = false;

	m_pbkTalk->GetAllStreamByCamid(mapStream);

	vcUpdateStream.clear();
	it = mapStream.begin();
	while(it != mapStream.end())
	{
		itvc = it->second.begin();
		while(itvc != it->second.end())
		{
			if (itvc->stream_type.Find(_T("LV")) != -1)
			{
				bFound = true;
				break;
			}
			itvc++;
		}

		if (!bFound)
		{
			vcUpdateStream.push_back(*(itvc-1));
		}
		it++;
	}

	if (vcUpdateStream.size())
	{
		m_pbkTalk->UpdateStreamType(vcUpdateStream);
	}
}

void CTaskTreeCtrl::vInitImages()
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

void CTaskTreeCtrl::SetDefaultCursor()
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

bool CTaskTreeCtrl::IsNeed2InsertNode(const NODEITEM* m_camInfo)
{
	bool bResult = false;
	if (m_camInfo->byDeviceType != eNVR && m_camInfo->byDeviceType != IPVSTATION)
	{
		bResult = true;
	}
	else
	{
		if (m_camInfo->bChannel)
		{
			bResult = true;
		}
	}
	return bResult;
}

void CTaskTreeCtrl::GetNodeItmFromTreeNode(HTREEITEM& hItem, vector<NODEITEM*>& vtNode, bool bFromGrpNode)
{
	HTREEITEM itemIPCam = hItem;

	while ( itemIPCam != NULL )
	{					
		TreeNode* pNodeitem = GetTreeNode(itemIPCam);
		if (pNodeitem->nodeType == TreeNode::TreeNdoeType::Grp_Node)
		{
			HTREEITEM itemChild = itemIPCam;
			int nIdx = 0;
			itemChild = GetChildItem(itemChild);
			GetNodeItmFromTreeNode(itemChild, vtNode, true);
			break;
		} 
		else if (pNodeitem->nodeType == TreeNode::TreeNdoeType::Dvs_Node)
		{
			HTREEITEM itemChild = itemIPCam;
			itemChild = GetChildItem(itemChild);
			GetNodeItmFromTreeNode(itemChild, vtNode, true);
			if (!bFromGrpNode)
				break;
		}

		if (pNodeitem && pNodeitem->pNode && pNodeitem->nodeType == TreeNode::TreeNdoeType::Cam_Node)
		{	
			vtNode.push_back(pNodeitem->pNode);
			if (!bFromGrpNode)
				break;
		}
		
		CString str = GetItemText(itemIPCam);
		itemIPCam = GetNextItem(itemIPCam, TVGN_NEXT);
		str = GetItemText(itemIPCam);
	}
}

void CTaskTreeCtrl::DisplayGroupLiveView()
{
	HTREEITEM itemIPCam;
	HTREEITEM  hItem = GetSelectedItem();
	int ll_maxNode;
	int li_layoutIdx = m_pbkTalk->GetMainFrame()->m_OrgLayoutIdx;

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
	if(li_layoutIdx == 1 && !(m_pbkTalk->GetMainFrame()->m_bAutoScan))
	{		
		TreeNode* pNodeitem = GetTreeNode(itemIPCam);
		vector <NODEITEM*> vtNode;
		if (pNodeitem && pNodeitem->pNode)
		{
			vtNode.push_back(pNodeitem->pNode);			
			m_pbkTalk->GetMainFrame()->vIsDragging(false);
			m_pbkTalk->GetMainFrame()->vAlocateViewCell(1);
			m_pbkTalk->GetMainFrame()->vVideoPresent(vtNode);
			m_bOnlyOneCamChoosen = true;
			isPrvGroupName = GetItemText(hItem);
		}
		else
		{
			m_pbkTalk->GetMainFrame()->vIsDragging(false);
			m_pbkTalk->GetMainFrame()->vAlocateViewCell(1);
			vtNode.clear();
			m_pbkTalk->GetMainFrame()->vVideoPresent(vtNode);
		}
		return;
	}

	vector <NODEITEM*> vtNode;
	GetNodeItmFromTreeNode(hItem, vtNode);
	if(vtNode.size()==1)
		m_pbkTalk->GetMainFrame()->vAlocateViewCell(li_layoutIdx); // there's only one camera in a group
	else 
	{
		m_pbkTalk->GetMainFrame()->vAlocateViewCell(vtNode.size());
	}

	if (vtNode.size()< MAXVIDEO )
		ll_maxNode = vtNode.size();
	else
		ll_maxNode = MAXVIDEO - 1;

	m_pbkTalk->GetMainFrame()->vIsDragging(false);
	m_pbkTalk->GetMainFrame()->vVideoPresent(vtNode);
	m_bOnlyOneCamChoosen = false;		
	isPrvGroupName = csGroupName;
}

void CTaskTreeCtrl::vLiveView()
{
	TVITEM*	ltv_item;
	TVITEM lTreeItem ={0};
	HTREEITEM  hItem = GetSelectedItem();

	int ll_maxNode;
	int li_layoutIdx = m_pbkTalk->GetMainFrame()->m_OrgLayoutIdx;
	
	if (hItem == NULL)
	{
		hItem = GetRootItem();
		SelectItem(hItem);
	}

	HTREEITEM parentItem = GetParentItem(hItem);
	if (parentItem == NULL) //Click on the group
	{	
		DisplayGroupLiveView();	
	}
	else
	{
		vector <NODEITEM*> vtNode;
		GetNodeItmFromTreeNode(hItem, vtNode);
		TreeNode* pTreeNode = GetTreeNode(hItem);
		if (pTreeNode->nodeType == TreeNode::TreeNdoeType::Cam_Node)
		{
			vtNode.clear();
			vtNode.push_back(pTreeNode->pNode);
		}
		m_pbkTalk->GetMainFrame()->vIsDragging(false);				
		m_pbkTalk->GetMainFrame()->vAlocateViewCell(li_layoutIdx);
		m_pbkTalk->GetMainFrame()->vVideoPresent(vtNode);
	}
}

void CTaskTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	HTREEITEM	hitem;
	UINT		flags;

	if (m_bLDragging)
	{
		POINT pt = point;
		
		ClientToScreen( &pt );
		CImageList::DragMove(pt);

		m_rectTreeCtrl.clear();
		CRect rc = m_pbkTalk->GetMainFrame()->bGetRect();
		m_rectTreeCtrl = m_pbkTalk->GetMainFrame()->GetRectVct();

		if(m_rectTreeCtrl.size()==0) 
		{
			m_bPtInRect =  false;
		}
		
		CPoint ptLiveView=pt;
		m_pbkTalk->GetMainFrame()->vScreenToViewClientRect(&ptLiveView);
		if(rc.PtInRect(ptLiveView))
		{		
			m_bPtInRect=true;
			
			if (!m_bDrag_cursor_no)
			{
				if(m_pbkTalk->GetMainFrame()->bOutOfRange(1))	
					::SetCursor(cursor_no);
				else
					::SetCursor(cursor_arr);
			}
			else
				::SetCursor(cursor_arr);
			
		}
		else
		{			
			m_bPtInRect =  false;
		}
		
		
	 	//// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT(pDropWnd); //make sure we have a window

	 
		// Convert from screen coordinates to drop target client coordinates
		CRect rcTree;
		GetClientRect(&rcTree);
		pDropWnd->ScreenToClient(&pt);
		 
		if (m_bPtInRect == false)
		{
			if (rcTree.PtInRect(point))
			{
				TVHITTESTINFO tvHit;
				tvHit.pt = point;
				HTREEITEM hTarget = HitTest(&tvHit);

				// Select the item that is at the point myPoint.
				UINT uFlags;
				HTREEITEM hItem = HitTest(pt, &uFlags);

				if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
					SelectItem(hItem);

				if (!m_bDrag_cursor_no)
				{
					if (cursor_no == GetCursor()) ::SetCursor(cursor_arr);
				}
			}
			else
			{
				SetCursor(cursor_no);
			}
		}		
	}
	else 
	{
		if(cursor_no == ::GetCursor()) ::SetCursor(cursor_arr);
	}	

}

void CTaskTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bLDragging)
	{
		BOOL bOutRange = false;
		if (!m_bDrag_cursor_no)
			bOutRange=m_pbkTalk->GetMainFrame()->bOutOfRange(1);
		
		m_bDrag_cursor_no=0;
		m_bLDragging = FALSE;
		m_pbkTalk->GetMainFrame()->vIsDragging(m_bLDragging);
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();
		
		if(m_pDragImage != NULL) 
		{ 
			delete m_pDragImage; 
			m_pDragImage = NULL; 
		}

	
		if(m_bPtInRect && !bOutRange) //Drag into the video area...
		{	
			DragTreeNode2Video(point);
		}
		else //Drag Not in the video area
		{
			DropTreeNode();
			return;
		}		
	}
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CTaskTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLDragging = false;
	m_bDrag_cursor_no = false;

	UINT flags = 0;
	HTREEITEM hItem = HitTest(point, &flags);
	SelectItem(hItem);
	SetVideoFocus();

	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CTaskTreeCtrl::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;

	m_hitemDrag = pNMTreeView->itemNew.hItem;
	m_hitemDrop = NULL;

	TreeNode *pTreeNode = GetTreeNode(m_hitemDrag);
	if (pTreeNode)
	{
		if (!GetDragCursor(pTreeNode))
		{
			m_bDrag_cursor_no=1;
			if(cursor_no != ::GetCursor()) ::SetCursor(cursor_no);
		}
		else
		{
			m_bDrag_cursor_no=0;
			if(cursor_no == ::GetCursor()) ::SetCursor(cursor_arr);
		}
	}
	
	m_pDragImage = CreateDragImage(m_hitemDrag);  // get the image list for dragging
	if( !m_pDragImage )
		return;

	m_bLDragging = TRUE;
	m_pbkTalk->GetMainFrame()->vIsDragging(m_bLDragging);
	m_nDropRectIdx = -1;
	m_pDragImage->BeginDrag(0, CPoint(-15,-15));
	POINT pt = pNMTreeView->ptDrag;
	m_pDragImage->DragEnter(NULL, pt);
	SetCapture();
}

//Right Mouse Click to display the Popup Menu...
void CTaskTreeCtrl::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	TVITEM TreeItem;
	CPoint pointTrack = point;
	::GetCursorPos(&point);
	ScreenToClient(&point);

	UINT flags = 0;
	HTREEITEM hItem = HitTest(point, &flags);
	SelectItem(hItem);
	TreeNode *pTreeNode = GetTreeNode(hItem);
	if(pTreeNode)
	{
		CMenu m_menu;	
		CMenu* psubmenu = NULL;
		UINT menuid;
		int iAuth = m_pbkTalk->GetMainFrame()->m_iAuth;
		if( iAuth/10 == 0)	return;

		if (pTreeNode->nodeType == TreeNode::TreeNdoeType::Grp_Node) //Group Item
		{
			menuid = IDR_MENU_IPCAM_GROUP;
			CString csGroupName = GetItemText(hItem);
			m_mapGroupFlyAway.insert(pair<CString,bool>(csGroupName,false)); // set bFlyAway to false at initial
			m_menu.LoadMenu(menuid);
			psubmenu = m_menu.GetSubMenu(0);
			if( !((m_pbkTalk->GetMainFrame()->m_iAuth)/10) )
			{
				m_menu.EnableMenuItem(IDC_TREECTRL_MODIFY_GROUP, MF_GRAYED);
				m_menu.EnableMenuItem(IDC_TREECTRL_NEW_GROUP, MF_GRAYED);
				m_menu.EnableMenuItem(IDC_TREECTRL_REMOVE_GROUP, MF_GRAYED);
				m_menu.EnableMenuItem(ID_ADD_NEW_IPCAM, MF_GRAYED);
			}
			else if(csGroupName.Find(_T("NVR")) != -1)
			{
				//m_menu.EnableMenuItem(ID_ADD_NEW_IPCAM, MF_GRAYED);
			}	
		}
		else //Camera Item
		{
			psubmenu = CreateCameraNodeMenu(m_menu, hItem);
		}
		CGS::SetMenuString(m_menu.m_hMenu, IDC_TREECTRL_MODIFY_GROUP, IDS_TREECTRL_MODIFY_GROUP);
		CGS::SetMenuString(m_menu.m_hMenu, IDC_TREECTRL_NEW_GROUP, IDS_TREECTRL_NEW_GROUP);
		CGS::SetMenuString(m_menu.m_hMenu, IDC_TREECTRL_REMOVE_GROUP, IDS_TREECTRL_REMOVE_GROUP);
		CGS::SetMenuString(m_menu.m_hMenu, ID_ADD_NEW_IPCAM, IDS_ADD_NEW_IPCAM);
		CGS::SetMenuString(m_menu.m_hMenu, ID_ADD_INTERNET_IP_CAM, IDS_ADD_NEW_IPCAM);
		CGS::SetMenuString(m_menu.m_hMenu, ID_MODIFY_IPCAM, IDS_MODIFY_IPCAM);
		CGS::SetMenuString(m_menu.m_hMenu, ID_MENU_REMOVEIPCAM32815, IDS_REMOVEIPCAM32815);
		CGS::SetMenuString(m_menu.m_hMenu, IDC_IE_EXPLORER, IDS_IE_EXPLORER);

		if(psubmenu)
			psubmenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pointTrack.x, pointTrack.y, this);		
	}
}



void CTaskTreeCtrl::OnIeExplorer() 
{
	USES_CONVERSION;
	HTREEITEM  hItem = GetSelectedItem();
	while (GetParentItem(hItem) != NULL)
	{
		TreeNode* pNodeitem = GetTreeNode(hItem);
		if (pNodeitem != NULL)
		{
			CString csURL;
			csURL.Format(_T("http://%s:%d"),A2W(pNodeitem->pNode->ip), pNodeitem->pNode->nWebPort);
			TRACE(csURL);
			HINSTANCE  nResult = ShellExecute(NULL, _T("open"), csURL.LockBuffer(), _T(""), _T(""), SW_SHOW);
			break;
		}
	}
}

void CTaskTreeCtrl::OnAddInternetIpCam() 
{
	AddNewIPCam();
}

NODEITEM* CTaskTreeCtrl::GetCurrentNodeItem()
{
	HTREEITEM	hItem;
	if ( ((hItem = GetSelectedItem()) == NULL) ) return NULL;

	TreeNode* pTreeNode = NULL;
	NODEITEM* pNodeitem = NULL;
	pTreeNode = GetTreeNode(hItem);
	if (pTreeNode && pTreeNode->pNode)
	{
		pNodeitem = pTreeNode->pNode;
	}
	return pNodeitem;
}
void CTaskTreeCtrl::OnModifyDevice() 
{
	NODEITEM node;
	vector<NODEITEM> vcNodeItem;
	HTREEITEM hItem = GetSelectedItem();

	memcpy(&node, GetCurrentNodeItem(), sizeof(NODEITEM));
	vcNodeItem.push_back(node);
    VK vk;
	CConfigSettingDlg dlg(vcNodeItem);
	if (dlg.DoModal() == IDOK)
	{
		m_pbkTalk->SendMessage(MsgUpdateView, UM_UPDATE_DEVICE, node.cameraid);
	}
}

void CTaskTreeCtrl::vGetGroupCam(vector<NODEITEM*>& vctNode)
{
	int nGroupId(0);

	for(int j = 0; j < 	m_pbkTalk->GetMainFrame()->m_vctnGroupAutoPlayIdx.size(); j++)
	{
		nGroupId = m_pbkTalk->GetMainFrame()->m_vctnGroupAutoPlayIdx[j];

		for(int i=0;i<m_vctpAllNodesInTree.size();i++)
		{
			if(m_vctpAllNodesInTree[i]->nGroup == nGroupId)
			{
				vctNode.push_back(m_vctpAllNodesInTree[i]);
			}
		}
	}
}

void CTaskTreeCtrl::vGetGroupCam(int nGroupId, vector<NODEITEM*>& vctNode)
{	
	for(int i=0;i<m_vctpAllNodesInTree.size();i++)
	{
		if(m_vctpAllNodesInTree[i]->nGroup == nGroupId)
		{
			vctNode.push_back(m_vctpAllNodesInTree[i]);
		}		
	}
}
int CTaskTreeCtrl::nGetCurrentSelectedGroupId(void)
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
void CTaskTreeCtrl::vGetAllNodeItems(vector <NODEITEM*>& vtNodes)
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

NODEITEM* CTaskTreeCtrl::vGetNodeitemFromIP(CString o_IP)
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
			TreeNode* pNodeitem = GetTreeNode(itemChild);
			if (pNodeitem)
			{
				csIP.Format(_T("%s"), A2W(pNodeitem->pNode->ip));
				if (csIP.Trim()==o_IP.Trim())
				{
					return pNodeitem->pNode;
				}
			}
			itemChild = GetNextSiblingItem(itemChild);
		}
	  	itemRoot = GetNextSiblingItem(itemRoot);
	}
	return NULL;
}


void CTaskTreeCtrl::OnProductTest() 
{

}

bool CTaskTreeCtrl::vRefreshTree(void)
{
	ClearTree();
	CreateTreeNode(GetTreeType());
	ExpandTree();
	return true;
}

void CTaskTreeCtrl::vGetGroupNodeItems(vector <NODEITEM*>& vtNodes)
{
	HTREEITEM  hItem = GetSelectedItem();
	GetNodeItmFromTreeNode(hItem, vtNodes);
}

void CTaskTreeCtrl::vUpdateTreeItem(NODEITEM* pNode)
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

 
void CTaskTreeCtrl::vSetGroupFlyAway(CString csGroupName)
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

void CTaskTreeCtrl::vFlyaway(int nIdx)
{
	vector <NODEITEM*> vtNode;
	HTREEITEM hItem = GetSelectedItem();
	if(hItem == NULL)
	{
		hItem = GetRootItem();
		SelectItem(hItem);
	}
	HTREEITEM parentItem = GetParentItem(hItem);
	CString csGroupName = GetItemText(hItem);
	vSetGroupFlyAway(csGroupName);
	bool bFlyAway = bGetGroupFlyAway(csGroupName);
	GetNodeItmFromTreeNode(hItem, vtNode);

	if (parentItem == NULL)
	{
		if (!vtNode.size())
		{
			return;
		}
	}

	CRect rect;
	GetDesktopWindow()->GetWindowRect(&rect);  

	m_pbkTalk->GetMainFrame()->vCreateDlgFlyAway(nIdx, vtNode, rect, rect, m_pbkTalk->GetMainFrame()->m_bAutoScanIPWall);
}


bool CTaskTreeCtrl::bGetGroupFlyAway(CString csGroupName)
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

void CTaskTreeCtrl::OnTreectrlModifyGroup()
{
	HTREEITEM hItem = GetSelectedItem();
	if (hItem == NULL ) return;

	CString csGroupName = GetItemText(hItem);
	int groupid=0;
	TreeNode *pTreeNode = GetTreeNode(hItem);
	if (pTreeNode)
	{
		groupid = pTreeNode->grpId;
	}
 	VK vk; 
	CDlgSettingCamGroup_Ext dlg(groupid,csGroupName);

	if (IDOK == dlg.DoModal(hItem))
	{
		//Reset the Array value
		vModifyGroupDB(dlg.m_GroupName.Trim(), dlg.m_Groupid);
	}	
}

void CTaskTreeCtrl::OnTreectrlNewGroup() 
{
    VK vk;
	CDlgSettingCamGroup_Ext dlg;

	if (IDOK == dlg.DoModal())
	{
		vInsertGroupDB(dlg.m_GroupName);
	}

// 	ClearTree();
// 	CreateTreeNode(GetTreeType());
}

void CTaskTreeCtrl::vModifyGroupDB(CString& csNewName, int o_GroupId)
{
	CString ls_sql;
	HTREEITEM hItem = GetSelectedItem();
	if (hItem==NULL ) return;

	CString csGroupName = GetItemText(hItem); // Get Original Group Name
	
	SetItemText(hItem, csNewName);
	m_pbkTalk->UpdateGroupName(csNewName, o_GroupId);
}


void CTaskTreeCtrl::vInsertGroupDB(CString& csName)
{
	Cam_Group grpObj;
	
	grpObj.clear();
	grpObj.camera_group_desc = csName;
	grpObj.cameragroupid = m_pbkTalk->GetLastGroupIdx()+1;
	grpObj.category_code.Format(_T("%d"),GetTreeType());
	grpObj.default_ = _T("N");
	grpObj.module_code = _T("LIVEVIEW");
	m_pbkTalk->InsertGroup(grpObj);

	InsertRootNode(csName, grpObj.cameragroupid);
	UpdateValue(grpObj, grpObj.cameragroupid-1);
}

int CTaskTreeCtrl::GetSelectGrpID(HTREEITEM& hItem)
{
	HTREEITEM hRootItem = hItem;
	int nGroupId(0);
	TreeNode *pTreeNode = NULL;
	
	while (GetParentItem(hRootItem) != NULL)
	{
		hRootItem = GetParentItem(hRootItem);
	}	
	if (hRootItem)
	{
		TreeNode* lpItem = GetTreeNode(hRootItem);
		if (lpItem)
		{
			nGroupId = lpItem->grpId;
		}
	}

	return nGroupId;
}

void CTaskTreeCtrl::DeleteChildNodes(HTREEITEM& hItem)
{
	HTREEITEM hCamera = GetChildItem(hItem), hTemp;

	while(hCamera)
	{
		hTemp = hCamera;
		hCamera = GetNextItem(hTemp, TVGN_NEXT);
		DeleteChildNode(hTemp);
	}
}

bool CTaskTreeCtrl::CheckRemoveGroup()
{
	bool bResult = true;
	int iGroupCount = 1;
	HTREEITEM	hItem = NULL, hRootItem = NULL, hGroupItem = NULL;	

	hRootItem = GetRootItem();
	hGroupItem = GetNextSiblingItem(hRootItem);
	while(hGroupItem)
	{
		iGroupCount++;
		hGroupItem = this->GetNextSiblingItem(hGroupItem);
	}

	if(iGroupCount <= 1)
	{
		CString str;
		LOADSTRING(str, IDS_TREECTRL_NOT_GROUP);
		AfxMessageBox(str);
		bResult = false;
	}

	if ( ((hItem = GetSelectedItem()) == NULL) ) 
	{
		CString str;
		LOADSTRING(str, IDS_TREECTRL_NOT_GROUP);
		AfxMessageBox(str); //Shouldn't happen
		bResult = false;
	}

	return bResult;
}

void CTaskTreeCtrl::OnTreectrlRemoveGroup() 
{
	HTREEITEM	hItem = NULL;	
	int ci = 0;

	if(!CheckRemoveGroup()) return;
	hItem = GetSelectedItem();

	if (GetParentItem(hItem) == NULL)
	{
		if ((GetAsyncKeyState(VK_CONTROL) & 0x8000))
		{
			CString csString(_T(""));
			LOADSTRING(csString, IDC_REMOVE_GROUP);
			if( AfxMessageBox( csString, MB_OKCANCEL|MB_ICONQUESTION) == IDOK)
			{
				Clear_KeepDelCamId();
				ReleaseVideo(hItem);
				DeleteChildNodes(hItem);
				vDeleteGroup(hItem);
				RefreshLauoutTree();
				m_pbkTalk->NotifyRecordSvr();
				m_pbkTalk->NotifyEventSvr();
				m_pbkTalk->NotifyHttpSvr();
			}
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
			CString cs,csDeleteGroup;
			LOADSTRING(csDeleteGroup,IDS_DELETE_GROUP1);
			cs.Format(_T("%s: %s ?"), csDeleteGroup,csGroupName);		
			if ( AfxMessageBox(cs, MB_OKCANCEL,0 )== IDCANCEL) return;

			vDeleteGroup(hItem);
			m_pbkTalk->NotifyRecordSvr();
			m_pbkTalk->NotifyEventSvr();
			m_pbkTalk->NotifyHttpSvr();
		}

	}
	m_pbkTalk->UpdateDefaultGroup();
}

bool CTaskTreeCtrl::vDeleteGroup(HTREEITEM& hItem)
{
	bool bResult = false;
	int o_GroupId = GetSelectGrpID(hItem);
	if (o_GroupId <=0) return false;
	
	vector<Cam_Group> vcGroup;
	bResult = m_pbkTalk->DeleteGroup(o_GroupId);
// 	m_pbkTalk->GetGroup(AllTree, vcGroup);
// 	int nIdx = 0, nCount = vcGroup.size();
// 
// 	for (nIdx = 0; nIdx < nCount; nIdx++)
// 	{
// 		bResult = m_pbkTalk->UpdateGroupId(nIdx+1, vcGroup[nIdx].cameragroupid);
// 	}

	TreeNode *pTreeNode = GetTreeNode(hItem);
	if (pTreeNode)
	{
		delete pTreeNode;
		pTreeNode = NULL;
		DeleteItem(hItem);
	}
	return true;
}

void CTaskTreeCtrl::AddNewIPCam()
{
	CMainFrame* pCMainFrame = m_pbkTalk->GetMainFrame();
	if(!pCMainFrame->bGetCameraCount())
	{
		AfxMessageBox(_T("Over limitation of product"));
		return;
	}

	HTREEITEM hItem = GetSelectedItem();
	if (hItem == NULL ) return;

	int nGroupID = GetSelectGrpID(hItem);
    VK vk;
	CAddDevice dlg(nGroupID);
	if( dlg.DoModal() == IDOK && !dlg.isExistDevice())
	{
		m_pbkTalk->SendMessage(MsgUpdateView, UM_INSERT_TREENODE, NULL);
		m_pbkTalk->NotifyRecordSvr();
		m_pbkTalk->NotifyEventSvr();
		m_pbkTalk->NotifyHttpSvr();
		return;
	}
}

void CTaskTreeCtrl::ReleaseVideo(HTREEITEM& hItem)
{
	if (hItem)
	{
		TreeNode* pTreeNode = NULL;
		pTreeNode = GetTreeNode(hItem);
		if (pTreeNode->nodeType == TreeNode::TreeNdoeType::Cam_Node)
		{
			m_pbkTalk->SendMessage(MsgUpdateView,UM_FREE_VIDEO,(LPARAM)pTreeNode->pNode);
		}
		else
		{
			HTREEITEM hItemChild = GetChildItem(hItem);
			while(hItemChild)
			{
				ReleaseVideo(hItemChild);
				hItemChild = GetNextItem(hItemChild, TVGN_NEXT);
			}
		}
	}
}

void CTaskTreeCtrl::RemoveIPCam(HTREEITEM& hItem)
{
	Clear_KeepDelCamId();
	ReleaseVideo(hItem);
	DeleteChildNode(hItem);
	RefreshLauoutTree();
	m_pbkTalk->NotifyRecordSvr();
	m_pbkTalk->NotifyEventSvr();
	m_pbkTalk->NotifyHttpSvr();
}

void CTaskTreeCtrl::OnMenuRemoveipcam()
{
	// TODO: Add your command handler code here

	HTREEITEM  hItem = GetSelectedItem();
	TreeNode* pTreeNode = NULL;
	pTreeNode = GetTreeNode(hItem);

	if(!pTreeNode) return;
	
	CString csQuestion1;
	LOADSTRING(csQuestion1, IDS_TREECTRL_REMOVE_CAM1);
	if (AfxMessageBox( csQuestion1, MB_OKCANCEL|MB_ICONQUESTION) == 2) return;   //Cancel

	RemoveIPCam(hItem);
}

//Lynn modified for surveillance show at 2013/10/25 
void CTaskTreeCtrl::vDispatchvideo_format2NodeItem(const CString& str,  NODEITEM* pNode)
{
	pNode->bFishEye = m_pbkTalk->IsFishEye(str);
	pNode->bChannel = m_pbkTalk->IsChannel(str);
	pNode->byDeviceType = m_pbkTalk->GetDevice(str);
	pNode->byModelType = m_pbkTalk->GetModelType(str);
	pNode->byPanoromaType = m_pbkTalk->GetPanarama(str);
}

bool CTaskTreeCtrl::vChangeCamGroup(HTREEITEM& itemDrag, HTREEITEM& itemDrop)
{
	TreeNode *pDragTreeNode = GetTreeNode(itemDrag);
	TreeNode *pDropTreeNode = GetTreeNode(itemDrop);

	int nOldGroupid = pDragTreeNode->grpId;
	int nNewGroupid = pDropTreeNode->grpId;
	int nCamId = pDragTreeNode->pNode->cameraid;

	bool bResult = false;

	if (nOldGroupid<1 || nNewGroupid<1 || nCamId<1) 
		return false;

	if (m_pbkTalk->UpdateGroupCameraGrpId(nOldGroupid, nNewGroupid, nCamId))
	{
		MoveTreeNode(itemDrag,itemDrop);
		return true;
	}
	else
		return false;
}
//////////////////////////////////////////////////////////////////////////Lynn
void CTaskTreeCtrl::DeleteTreeNode(HTREEITEM& hItem)
{
	HTREEITEM hChildItem;
	TreeNode* pTreeNode = NULL;
	while(hItem)
	{
		hChildItem = GetChildItem(hItem);
		if (hChildItem)
		{
			DeleteTreeNode(hChildItem);
		}
		pTreeNode = GetTreeNode(hItem);

		if (pTreeNode->nodeType != TreeNode::TreeNdoeType::Grp_Node)
		{
			delete (pTreeNode->pNode);
			pTreeNode->pNode = NULL;
		}
		delete pTreeNode;
		pTreeNode = NULL;

		hItem = GetNextItem(hItem, TVGN_NEXT);
	}
}

void CTaskTreeCtrl::ClearTree()
{
	HTREEITEM hItem = GetRootItem();
	DeleteTreeNode(hItem);
	DeleteAllItems();
	m_vctcsGroupNames.clear();
	m_vctpAllNodesInTree.clear();
}

void CTaskTreeCtrl::UpdateValue(const Cam_Group& GroupItm, int nIdx)
{
	m_vctcsGroupNames.push_back(GroupItm.camera_group_desc);
	m_maxGroupId = GroupItm.cameragroupid;
	m_nGroup = GroupItm.cameragroupid;
}

HTREEITEM CTaskTreeCtrl::InsertRootNode(const CString& strGrpName, int nGrpId)
{
	HTREEITEM hTreeItm = TVGN_ROOT;
	CString strName(strGrpName);
	TreeNode *pTreeNode = new TreeNode;
	pTreeNode->nodeType = TreeNode::TreeNdoeType::Grp_Node;
	pTreeNode->grpId = nGrpId;

	hTreeItm = InsertNode(hTreeItm, pTreeNode, strName, TVIF_PARAM | TVIF_TEXT);

	return hTreeItm;
}

HTREEITEM CTaskTreeCtrl::InsertRootNodes(const vector<Cam_Group>& vcGroup)
{
	int nIdx = 0, nCount = vcGroup.size();
	HTREEITEM hTreeItm = NULL, itemDefault = NULL;
	CString camera_group_desc(_T(""));

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		hTreeItm = InsertRootNode(vcGroup[nIdx].camera_group_desc, vcGroup[nIdx].cameragroupid);
		camera_group_desc = vcGroup[nIdx].camera_group_desc;

		if (camera_group_desc.MakeUpper()==_T("Y")) 
			itemDefault = hTreeItm;

		//Build Array
		UpdateValue(vcGroup[nIdx], nIdx);
	}

	if (itemDefault == NULL)
	{
		itemDefault = GetNextItem(TVI_ROOT, TVGN_ROOT);
	}

	return itemDefault;
}

void CTaskTreeCtrl::CreateTreeNode(TreeType byTreeType)
{
	vector<Cam_Group> vcGroup;
	int nIdx = 0, nCount = 0;

	CreateTreeGroup();
	m_pbkTalk->GetGroup(byTreeType, vcGroup);

	CString str;
	str.Format(_T("%d"),vcGroup.size());
//	WritePrivateProfileString(_T("CreateTreeNode"),_T("Count"),str,_T("D:\\Lynn.ini"));

	FilterCameraGroup(vcGroup);
	InsertRootNodes(vcGroup);
	CreateChildNode(vcGroup, byTreeType);
}

void CTaskTreeCtrl::CreateChildNode(const vector<Cam_Group>& vcGroup, byte byTreeType)
{
	int nIdx = 0, nCount = vcGroup.size();
	vector<Cam_Group_Cam> vcGroupCam;

	HTREEITEM hTreeItm = GetNextItem(TVI_ROOT, TVGN_ROOT);

	for (nIdx = 0 ; nIdx < nCount; nIdx++)
	{
		vcGroupCam.clear();
		m_pbkTalk->GetGroupCamera(vcGroup[nIdx].cameragroupid, vcGroupCam);
		InsertChildNode(vcGroupCam, hTreeItm);

		hTreeItm = GetNextSiblingItem(hTreeItm);
	}
}

bool CTaskTreeCtrl::IsNeed2InsertDeviceNode(const map<int, NODEITEM>& mapNode, const vector<Cam_Group_Cam>& vcGroupCam, int& nDeviceNodeIdx)
{
	bool bResult = false;
	nDeviceNodeIdx = 0;
	NODEITEM node = {0};
	map<int, NODEITEM>::const_iterator mapit;

	vector<Cam_Group_Cam>::const_iterator it = vcGroupCam.begin();
	while(it != vcGroupCam.end())
	{
		mapit = mapNode.find(it->cameraid);
		if (mapit != mapNode.end())
		{
			if ((mapit->second.byDeviceType == IPVSTATION || mapit->second.byDeviceType == eNVR) 
				&& mapit->second.bChannel == false)
			{
				bResult = true;
				nDeviceNodeIdx = it - vcGroupCam.begin();
				break;
			}
		}
		it++;
	}

	return bResult;
}

void CTaskTreeCtrl::Trans_CamGrpCam2NODEITEM(const vector<Cam_Group_Cam>& vcGroupCam, map<int, NODEITEM>& mapNode)
{
	int nCount = vcGroupCam.size(), nIdx = 0;
	ec_Camera camObj;
	Cam_Group_Cam groupcam;

	for (nIdx=0 ; nIdx<nCount ; nIdx++)
	{
		camObj.clear();
		NODEITEM node = {0};
		m_pbkTalk->GetCamera(vcGroupCam[nIdx].cameraid, camObj);
		if (vcGroupCam[nIdx].streamid == 0)
		{
			groupcam.clear();
			groupcam = vcGroupCam[nIdx];
			groupcam.streamid = 1;
			m_pbkTalk->UpdateGroupCamera(groupcam);
			Trans_Camera2NODEITEM(camObj, node, groupcam.streamid);
		} 
		else
		{
			Trans_Camera2NODEITEM(camObj, node, vcGroupCam[nIdx].streamid);
		}
		
		mapNode[camObj.cameraid] = node;
	}
}

void CTaskTreeCtrl::ReassignCamGrpCam(const vector<Cam_Group_Cam>& vcGroupCam, vector< vector<Cam_Group_Cam> >& vcGroupCamByDevice)
{
	int nCount = vcGroupCam.size(), nIdx = 0;
	vector<Cam_Group_Cam> vcGroup;
	vector< vector<Cam_Group_Cam> >::iterator it;

	for (nIdx=0 ; nIdx<nCount ; nIdx++)
	{
		vcGroup.clear();
		if (vcGroupCam[nIdx].numb_ == 0)
		{
			vcGroup.push_back(vcGroupCam[nIdx]);
			vcGroupCamByDevice.push_back(vcGroup);
		}
		else
		{
			it = vcGroupCamByDevice.begin();
			bool nFound = false;

			while(it != vcGroupCamByDevice.end())
			{
				if ( it->begin()->numb_ == vcGroupCam[nIdx].numb_ )
				{
					it->push_back(vcGroupCam[nIdx]);
					nFound = true;
					break;
				}
				it++;
			}

			if (nFound == false)
			{
				vcGroup.push_back(vcGroupCam[nIdx]);
				vcGroupCamByDevice.push_back(vcGroup);
			}
		}
	}
}

void CTaskTreeCtrl::InsertChildNode(const vector<Cam_Group_Cam>& vcGroupCam, HTREEITEM& hTreeItm)
{
	int nIdx = 0, nCount = vcGroupCam.size();
	int nDeviceNodeIdx = 0;
	bool bDeviceNode = false;
	ec_Camera camObj;
	HTREEITEM hPrtTreeItm = hTreeItm;
	map<int, NODEITEM> mapNode;
	vector< vector<Cam_Group_Cam> > vcGroupCamByDevice;
	int nGrpIdx = 0, nGrpCount = 0;
	NODEITEM node = {0};
	map<int, NODEITEM>::iterator it;
	vector<Cam_Group_Cam> vcGroup;
	
	Trans_CamGrpCam2NODEITEM(vcGroupCam, mapNode);
	ReassignCamGrpCam(vcGroupCam, vcGroupCamByDevice);
	nGrpCount = vcGroupCamByDevice.size();

	for (nGrpIdx = 0; nGrpIdx < nGrpCount; nGrpIdx++)
	{
		bDeviceNode = false;
		hPrtTreeItm = hTreeItm;
		vcGroup.clear();
		vcGroup = vcGroupCamByDevice[nGrpIdx];
		nCount = vcGroup.size();

		if (IsNeed2InsertDeviceNode(mapNode, vcGroup, nDeviceNodeIdx))
		{
			bDeviceNode = true;
			m_pbkTalk->GetCamera(vcGroup[nDeviceNodeIdx].cameraid, camObj);
			it = mapNode.find(vcGroup[nDeviceNodeIdx].cameraid);
			if (it != mapNode.end())
			{
				hPrtTreeItm = InsertDeviceNode(camObj, hTreeItm, it->second);
			}
		}

		for (nIdx = 0 ; nIdx < nCount; nIdx++)
		{
			camObj.clear();
			if (bDeviceNode && nDeviceNodeIdx == nIdx)
				continue;
			
			it = mapNode.find(vcGroup[nIdx].cameraid);
			if (it != mapNode.end())
			{
				InsertCameraNode(it->second, hPrtTreeItm, bDeviceNode);
			}
		}
	}
}

CTaskTreeCtrl::TreeNode* CTaskTreeCtrl::GetRootItemFromTreeItem(HTREEITEM& hPrtTreeItm)
{
	HTREEITEM hPrtItm;
	TreeNode* pTreeNode = NULL;

	pTreeNode = GetTreeNode(hPrtTreeItm);

	hPrtItm = hPrtTreeItm;
	while((hPrtItm = GetParentItem(hPrtItm)) != NULL)
	{
		pTreeNode = GetTreeNode(hPrtItm);
	}

	return pTreeNode;
}

void CTaskTreeCtrl::Trans_Camera2NODEITEM(const ec_Camera& camObj, NODEITEM& node, int nStreamID)
{
	USES_CONVERSION;
	CString str;

	str = camObj.stream_url;
	_tcscpy(node.stream_url, str.GetBuffer());	
	node.nStreamID = nStreamID;
	node.cameraid = camObj.cameraid;
	node.camera_idx = camObj.camera_idx;

	str = camObj.cameraname;
	strcpy(node.camera_name , W2A(str));
	_tcscpy(node.wcAlias, str.GetBuffer(32));	

	str = camObj.ipaddress;
	strcpy(node.ip , W2A(str));
	m_pbkTalk->TransString2Mac(camObj.mac_address, node.mac);
	node.nWebPort = camObj.httpport;

	str = camObj.username;
	strcpy(node.szUser, W2A(str));
	
	str = camObj.password;
	strcpy(node.szPass, W2A(str));
	vDispatchvideo_format2NodeItem(camObj.video_format, &node);
	//node.nENVR = GetNVRBrand(node);

	str = camObj.model_code;
	strcpy(node.modelname, W2A(str));

	node.nAPPort = GetStreamPort(camObj,nStreamID);
	node.nProtocol = m_pbkTalk->GetStreamProtocol(camObj,nStreamID-1);

	if (camObj.ptz_support.Find(_T("Y"))!=-1)
		node.bSupportPTZ = true;

	node.hPatrolThread = NULL;
	node.hAutoScanThread = NULL;
	node.bRecording = false;
	int recId = 0;
	int nIdx = 0, nCount = camObj.vcStream.size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		if (camObj.vcStream[nIdx].stream_type == _T("RE"))
		{
			recId = nIdx+1;
			if (recId == node.nStreamID)
			{
				node.bRecording = true;
			}
			break;
		}
	}

	str.ReleaseBuffer();
}

WORD CTaskTreeCtrl::GetStreamPort(const ec_Camera& camObj, int nStreamId)
{
	WORD nAPPort = 0;

	if (nStreamId && camObj.vcStream.size() && camObj.vcStream.size() >= nStreamId)
	{
		if (camObj.vcStream[nStreamId-1].stream_port < 0)
			nAPPort = DEFAULTSPORT;
		else
			nAPPort = camObj.vcStream[nStreamId-1].stream_port;
	}

	return nAPPort;
}

// WORD CTaskTreeCtrl::GetStreamID(const ec_Camera& camObj)
// {
// 	WORD nStreamID = 0;
// 
// 	if (camObj.vcStream.size())
// 	{
// 		nStreamID = _wtol(camObj.vcStream[0].stream_name.Right(1).GetBuffer());
// 	}
// 		
// 	if (nStreamID <=0)
// 	{
// 		nStreamID = 1;
// 	}
// 
// 	return nStreamID;
// }

//NVR_BRAND CTaskTreeCtrl::GetNVRBrand(const NODEITEM& node)
//{
//	NVR_BRAND NvrBrandType;
// 	
//	if(node.byDeviceType == eNVR)
// 		NvrBrandType = REMOTE_ETROCENTER;
//	else if (node.byDeviceType == ISAP)
//		NvrBrandType = ISAP;
//	else
//		NvrBrandType = LOCAL;
//
//	return NvrBrandType;
//}

//PROTOCOL CTaskTreeCtrl::GetStreamProtocol(const ec_Camera& camObj, int nStreamId)
//{
//	PROTOCOL protocolType = VSMP;
//
//	if (camObj.vcStream.size() >= nStreamId)
//	{
//		if  (camObj.vcStream[nStreamId-1].stream_protocol == _T("AVIFILE"))
//			protocolType = AVIFILE;
//		else if (camObj.vcStream[nStreamId-1].stream_protocol == _T("RTPTCP"))
//			protocolType = RTPOVERTCP;
//		else if (camObj.vcStream[nStreamId-1].stream_protocol == _T("RTSP"))
//			protocolType = RTSP;
//		else if (camObj.vcStream[nStreamId-1].stream_protocol == _T("RTPHTTP"))
//			protocolType = RTPOVERHTTP;
//		else if (camObj.vcStream[nStreamId-1].stream_protocol == _T("CGI"))
//			protocolType = CGI;
//		else if (camObj.vcStream[nStreamId-1].stream_protocol == _T("CGI_ISAP"))
//			protocolType = CGI_ISAP;
//		else
//			protocolType = VSMP;
//	}
//
//	return protocolType;
//}

void CTaskTreeCtrl::FilterCameraGroup(vector<Cam_Group>& vcGroup)
{
	int nCount = vcGroup.size();

	if(nCount > MAX_GROUP)
	{
		vector<Cam_Group> vcTemGroup;
		vcTemGroup.insert(vcTemGroup.begin(),vcGroup.begin(), vcGroup.begin()+MAX_GROUP);
		vcGroup = vcTemGroup;
	}
}

void CTaskTreeCtrl::ExpandTree()
{
	HTREEITEM hRoot = GetRootItem();
	while(hRoot)
	{
		Expand(hRoot, TVE_EXPAND);
		hRoot = GetNextSiblingItem(hRoot);
	}	
}
void CTaskTreeCtrl::OnDestroy()
{
	ClearTree();
	CTreeCtrl::OnDestroy();

	// TODO: Add your message handler code here
}

CTaskTreeCtrl::TreeNode* CTaskTreeCtrl::GetTreeNode(HTREEITEM& hItem)
{
	TreeNode* pNodeitem = NULL;
	TVITEM lTreeItem = {0};
	lTreeItem.hItem = hItem;
	lTreeItem.mask = TVIF_HANDLE;
	GetItem(&lTreeItem);
	pNodeitem = (TreeNode*)lTreeItem.lParam;

	return pNodeitem;
}

bool CTaskTreeCtrl::GetDragCursor(TreeNode *pTreeNode)
{
	bool bResult = true;
	if (pTreeNode->nodeType == TreeNode::TreeNdoeType::Grp_Node)
	{
		bResult = false;
	}
	else if(pTreeNode->pNode)
	{
		if (pTreeNode->pNode->bChannel)
		{
			bResult = false;
		}
	}

	return bResult;
}

void CTaskTreeCtrl::DragTreeNode2Video(const CPoint& point)
{
	BeginWaitCursor();
	vector <NODEITEM*> vtNode;
	HTREEITEM hItem;
	if (m_hitemDrag)
	{
		hItem=m_hitemDrag;
		 SelectItem(m_hitemDrag);
	}
	else
	{
		hItem=GetSelectedItem();
	}

	TreeNode *pTreeNode = GetTreeNode(hItem);
	if (pTreeNode->nodeType == TreeNode::TreeNdoeType::Grp_Node || 
		pTreeNode->nodeType == TreeNode::TreeNdoeType::Dvs_Node)
	{
		m_pbkTalk->GetMainFrame()->vIsDragging(false);
		m_pbkTalk->SendMessage(MsgUpdateView,UM_FREE_VIDEO,NULL);
		GetNodeItmFromTreeNode(hItem, vtNode);
		m_pbkTalk->GetMainFrame()->vVideoPresent(vtNode);
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
		GetNodeItmFromTreeNode(hItem, vtNode);
		m_pbkTalk->GetMainFrame()->vIsDragging(true);
		//m_pbkTalk->SendMessage(MsgUpdateView,UM_FREE_VIDEO,NULL);
		m_pbkTalk->GetMainFrame()->vVideoPresent(vtNode);
	}
	EndWaitCursor();
}

void CTaskTreeCtrl::DropTreeNode()
{
	bool bResult = false;		

	BeginWaitCursor();
	HTREEITEM hItem = GetSelectedItem();
	if (hItem)
	{
		TreeNode *pTreeNode = GetTreeNode(hItem);
		if (pTreeNode)
		{
			if ( pTreeNode->nodeType == TreeNode::TreeNdoeType::Cam_Node )
			{
				hItem =GetGroupItem(hItem);
			}			
		}
	}

	if ( IsDragItemValid(m_hitemDrag) && IsDropItemValid(hItem) )
	{
		TreeNode *pDragTreeNode = GetTreeNode(m_hitemDrag);
		TreeNode *pDropTreeNode = GetTreeNode(hItem);

		if (pDragTreeNode && pDropTreeNode)
		{
			if (pDragTreeNode->nodeType == TreeNode::TreeNdoeType::Dvs_Node)
			{
				ChangeDeviceGroup(m_hitemDrag, hItem);
				bResult = true;
			}
			else
			{
				if (vChangeCamGroup(m_hitemDrag, hItem))
				{
					bResult = true;
				}
			}

			if (bResult)
			{
				ibDiffGroup = true;
				m_bRefreshVideo = true;
				m_pbkTalk->GetMainFrame()->vIsDragging(false);
			}
		}
	}
}

void CTaskTreeCtrl::ChangeDeviceGroup(HTREEITEM& itemDrag, HTREEITEM& itemDrop)
{
	TreeNode *pDragTreeNode = GetTreeNode(itemDrag);
	TreeNode *pDropTreeNode = GetTreeNode(itemDrop);
	TreeNode *pChannelTreeNode = NULL;

	USES_CONVERSION;
	m_pbkTalk->UpdateGroupCameraGrpId(pDragTreeNode->grpId, pDropTreeNode->grpId, pDragTreeNode->pNode->cameraid);
	NODEITEM* pNode = pDragTreeNode->pNode;
	pNode->nGroup = pDropTreeNode->grpId;
	pDragTreeNode->grpId = pDropTreeNode->grpId;
	HTREEITEM hDeviceItm = InsertNode(itemDrop, pDragTreeNode, A2W(pNode->camera_name), TVIF_PARAM | TVIF_TEXT | TVIF_HANDLE);
	pNode->hTreeItem = hDeviceItm;
	
	HTREEITEM hDragChildItem = GetChildItem(itemDrag);
	HTREEITEM itemDelete;
	while(hDragChildItem)
	{
		pChannelTreeNode = GetTreeNode(hDragChildItem);
		m_pbkTalk->UpdateGroupCameraGrpId(pChannelTreeNode->grpId, pDropTreeNode->grpId, pChannelTreeNode->pNode->cameraid);

		itemDelete = hDragChildItem;
		hDragChildItem = GetNextItem(hDragChildItem, TVGN_NEXT);
		MoveTreeNode(itemDelete,hDeviceItm);
	}

	DeleteItem(itemDrag);
}

bool CTaskTreeCtrl::IsDropItemValid(HTREEITEM& hItem)
{
	bool bResult = false;

	TreeNode *pTreeNode = GetTreeNode(hItem);
	if (pTreeNode)
	{
		if ( pTreeNode->nodeType == TreeNode::TreeNdoeType::Grp_Node )
			bResult = true;
	}
	return bResult;
}

HTREEITEM CTaskTreeCtrl::GetGroupItem(HTREEITEM& hItem)
{
	HTREEITEM hItemParant = GetParentItem(hItem);
	if (hItemParant == NULL)
	{
		hItemParant = hItem;
	}
	else
	{
		HTREEITEM hItemTemp = hItemParant;
		while(hItemTemp)
		{
			hItemParant = hItemTemp;
			hItemTemp = GetParentItem(hItemParant);
		}
	}
	return hItemParant;
}

HTREEITEM CTaskTreeCtrl::GetInsertAfterItem(HTREEITEM& hParent, TreeNode *pSortTreeNode)
{
	if (pSortTreeNode->nodeType == TreeNode::TreeNdoeType::Dvs_Node || pSortTreeNode->nodeType == TreeNode::TreeNdoeType::Grp_Node)
	{
		return TVI_LAST;
	}
	else
	{
		HTREEITEM groupRoot = GetGroupItem(hParent);
		HTREEITEM childItem = GetChildItem(groupRoot);
		HTREEITEM previousItem = NULL;
		HTREEITEM resultItem = NULL;
		bool bChildItem = false;

		TreeNode *pTreeNode = NULL;
		while(childItem)
		{
			bChildItem = true;
			pTreeNode = GetTreeNode(childItem);

			if (pTreeNode->nodeType == TreeNode::TreeNdoeType::Dvs_Node)
			{
				childItem = GetNextItem(childItem, TVGN_CHILD);
				continue;
			}
			if (pTreeNode->pNode->camera_idx > pSortTreeNode->pNode->camera_idx)
			{
				resultItem = previousItem;
				break;
			}
			previousItem = childItem;
			childItem = GetNextItem(childItem, TVGN_NEXT);
		}

		if (resultItem  == NULL)
		{
			if (!bChildItem)
				resultItem = TVI_LAST;
			else if (bChildItem && previousItem)
				resultItem = previousItem;
			else if (bChildItem && !previousItem)
				resultItem = TVI_FIRST;
		}

		return resultItem;
	}
}

HTREEITEM CTaskTreeCtrl::InsertNode(HTREEITEM& hParent, TreeNode *pTreeNode, const CString& strNodeName, UINT mask)
{
	HTREEITEM hItem;
	TVINSERTSTRUCTW itemStru;

	itemStru.hParent = hParent;
	itemStru.hInsertAfter = GetInsertAfterItem(hParent, pTreeNode);
	itemStru.itemex.lParam = (LPARAM)pTreeNode;
	itemStru.itemex.mask = mask;
	itemStru.itemex.pszText = (LPWSTR)(LPCTSTR)strNodeName;
	itemStru.itemex.cchTextMax = strNodeName.GetLength();

	hItem = InsertItem(&itemStru);

	return hItem;
}

void CTaskTreeCtrl::DeleteStation(int nCameraId)
{
	HTREEITEM hItem = GetRootItem();
	TreeNode* pTreeNode = NULL;
	bool breakFlag = false;

	while(hItem)
	{
		HTREEITEM hItemChild = GetChildItem(hItem);
		while(hItemChild)
		{
			pTreeNode = GetTreeNode(hItemChild);
			if (pTreeNode->pNode->cameraid == nCameraId) 
			{
				RemoveIPCam(hItemChild);
				break;
			}

			if (pTreeNode->nodeType == TreeNode::TreeNdoeType::Dvs_Node)
			{
				HTREEITEM hItemChannel = GetChildItem(hItemChild);
				while(hItemChannel)
				{
					pTreeNode = GetTreeNode(hItemChannel);
					if (pTreeNode->pNode->cameraid == nCameraId) 
					{
						RemoveIPCam(hItemChannel);
						break;
					}

					hItemChannel = GetNextItem(hItemChannel, TVGN_NEXT);
				}
			}
			if (breakFlag)
				break;
			hItemChild = GetNextItem(hItemChild, TVGN_NEXT);
		}
		if (breakFlag)
			break;
		hItem = GetNextItem(hItem, TVGN_NEXT);
	}
}

LRESULT CTaskTreeCtrl::OnUpdateView(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case UM_REFRESH_ALLTREE:
		{
			vRefreshTree();
		}
		break;

	case UM_REFRESH_LAYOUTTREE:
		{
			if (GetTreeType() == LayoutTree)
			{
				vRefreshTree();
			}
		}
		break;

	case UM_REFRESH_DEVICETREE:
		{
			if (GetTreeType() == DeviceTree)
			{
				vRefreshTree();
			}
		}
		break;

	case UM_DELETE_TREENODE:
		{
			vector<int>* pvcCamId = (vector<int>*)lParam;
			if (pvcCamId)
			{
				DeleteTreeNodeByCamId(pvcCamId);
			}
		}
		break;
	case UM_SET_TREENODE_FOCUS:
		{
			NODEITEM *pNode = (NODEITEM*)lParam;
			if (pNode)
			{
				SelectItem(pNode->hTreeItem);
			}
		}
		break;
	case UM_INSERT_TREENODE:
		{
			Cam_Group_Cam grpCam;
			m_pbkTalk->GetLastGroupCamera(grpCam);
			AddDevice(grpCam.cameragroupid);
		}
		break;
	case UM_DELETE_STATION:
		{
			int nCameraId = (int)lParam;
			DeleteStation(nCameraId);
		}
		break;

	case UM_UPDATE_CAMERA_ID:
		{
			UpdateCameraId();
		}
		break;

	case UM_UPDATE_DEVICE:
		{
			int camid = lParam;
			UpdateDevice(camid);
			RedrawWindow(NULL, NULL);
		}
		break;

	default:
		break;
	}
	return 0;
}

void CTaskTreeCtrl::UpdateDevice(int nCamId)
{
	USES_CONVERSION;
	HTREEITEM hItem = GetRootItem();
	TreeNode* pTreeNode = NULL;
	HTREEITEM hItemChild;
	ec_Camera camObj;
	CString str;
	vector<Cam_Group_Cam> vcGrpCam;
	HTREEITEM itemDrag=NULL,itemDrop=NULL;
	NODEITEM *pNode = GetTreeNodeFromCamId(nCamId);

	if (pNode)
	{
		m_pbkTalk->GetGroupCamera(pNode->cameraid, vcGrpCam, false);
		if (vcGrpCam.size())
		{
			itemDrag = pNode->hTreeItem;
			pTreeNode = GetTreeNode(itemDrag);

			m_pbkTalk->GetCamera(pNode->cameraid,camObj);
			memset(pTreeNode->pNode->camera_name,0x00,sizeof(pTreeNode->pNode->camera_name));
			strncpy(pTreeNode->pNode->camera_name , W2A(camObj.cameraname), camObj.cameraname.GetLength());
			pTreeNode->pNode->nProtocol = m_pbkTalk->GetStreamProtocol(camObj,vcGrpCam[0].streamid-1);
			pTreeNode->pNode->nAPPort = GetStreamPort(camObj,vcGrpCam[0].streamid);
			pTreeNode->pNode->nStreamID = vcGrpCam[0].streamid;
			str.Format( _T("(%d) %s - %s"),pTreeNode->pNode->camera_idx, A2W(pTreeNode->pNode->camera_name), A2W(pTreeNode->pNode->ip));
			SetItemText(pNode->hTreeItem,str);
			
			pTreeNode->pNode->bRecording = false;
			int recId = 0;
			int nIdx = 0, nCount = camObj.vcStream.size();
			for (nIdx = 0; nIdx < nCount; nIdx++)
			{
				if (camObj.vcStream[nIdx].stream_type == _T("RE"))
				{
					recId = nIdx+1;
					if (recId == pTreeNode->pNode->nStreamID)
					{
						pTreeNode->pNode->bRecording = true;
					}
					break;
				}
			}
			
				
			if (vcGrpCam[0].cameragroupid != pNode->nGroup)
			{
				while(hItem)
				{
					pTreeNode = GetTreeNode(hItem);
					if (pTreeNode->grpId == vcGrpCam[0].cameragroupid)
					{
						itemDrop = hItem;
					}
					hItem = GetNextItem(hItem, TVGN_NEXT);
				}
			}
		}
	}

	if (itemDrag && itemDrop && GetTreeNode(itemDrag)->grpId != GetTreeNode(itemDrop)->grpId)
	{
		MoveTreeNode(itemDrag,itemDrop);
	}
}

NODEITEM* CTaskTreeCtrl::GetTreeNodeFromCamId(int nCamId)
{
	HTREEITEM hItem = GetRootItem();
	TreeNode* pTreeNode = NULL;
	HTREEITEM hItemChild;
	HTREEITEM hItemChannel;
	ec_Camera camObj;
	NODEITEM* pNode = NULL;
	bool bBreak = false;

	while(hItem)
	{
		hItemChild = GetChildItem(hItem);
		while(hItemChild)
		{
			pTreeNode = GetTreeNode(hItemChild);
			if (pTreeNode->nodeType == TreeNode::TreeNdoeType::Dvs_Node)
			{
				hItemChannel = GetChildItem(hItemChild);
				while(hItemChannel)
				{
					pTreeNode = GetTreeNode(hItemChannel);
					if (pTreeNode->pNode->cameraid == nCamId)
					{
						pNode = pTreeNode->pNode;
						bBreak = true;
						break;
					}
					hItemChannel = GetNextItem(hItemChannel, TVGN_NEXT);
				}
			}
			else
			{
				if (pTreeNode->pNode->cameraid == nCamId)
				{
					pNode = pTreeNode->pNode;
					bBreak = true;
					break;
				}
			}
			if(bBreak)
				break;
			hItemChild = GetNextItem(hItemChild, TVGN_NEXT);
		}
		if(bBreak)
			break;
		hItem = GetNextItem(hItem, TVGN_NEXT);
	}
	return pNode;
}

void CTaskTreeCtrl::UpdateCameraId()
{
	HTREEITEM hItem = GetRootItem();
	TreeNode* pTreeNode = NULL;
	HTREEITEM hItemChild;
	HTREEITEM hItemChannel;
	ec_Camera camObj;

	while(hItem)
	{
		hItemChild = GetChildItem(hItem);
		while(hItemChild)
		{
			pTreeNode = GetTreeNode(hItemChild);
			m_pbkTalk->GetCamera(pTreeNode->pNode->camera_idx,camObj,true);
			pTreeNode->pNode->cameraid = camObj.cameraid;

			if (pTreeNode->nodeType == TreeNode::TreeNdoeType::Dvs_Node)
			{
				hItemChannel = GetChildItem(hItemChild);
				while(hItemChannel)
				{
					pTreeNode = GetTreeNode(hItemChannel);
					m_pbkTalk->GetCamera(pTreeNode->pNode->camera_idx,camObj,true);
					pTreeNode->pNode->cameraid = camObj.cameraid;
					hItemChannel = GetNextItem(hItemChannel, TVGN_NEXT);
				}
			}
			hItemChild = GetNextItem(hItemChild, TVGN_NEXT);
		}
		hItem = GetNextItem(hItem, TVGN_NEXT);
	}
}

void CTaskTreeCtrl::ExpandItem(HTREEITEM  hItem)
{
	HTREEITEM hmyItem = GetChildItem(hItem);
	HTREEITEM hvisibleItem = GetNextVisibleItem(hItem);
	if (hmyItem != hvisibleItem)
		Expand(hItem, TVE_EXPAND);
	else
		Expand(hItem, TVE_COLLAPSE);
}

void CTaskTreeCtrl::SetVideoFocus()
{
	HTREEITEM  hItem = GetSelectedItem();
	TreeNode *ptreeNode = GetTreeNode(hItem);
	if (ptreeNode && ptreeNode->nodeType == TreeNode::TreeNdoeType::Cam_Node)
	{
		m_pbkTalk->SendMessage(MsgUpdateView, UM_SET_VIDEO_FOCUS, (LPARAM)ptreeNode->pNode);
	}
}

void CTaskTreeCtrl::MoveTreeNode(HTREEITEM& itemDelete, HTREEITEM& itemInsertGroup)
{
	TreeNode *pDeleteTreeNode = GetTreeNode(itemDelete);
	TreeNode *pInsertTreeNode = GetTreeNode(itemInsertGroup);

	USES_CONVERSION;
	CString str;
	NODEITEM* pNode = pDeleteTreeNode->pNode;
	TreeNode *pTreeNode = pDeleteTreeNode;
	pNode->nGroup = pInsertTreeNode->grpId;
	pTreeNode->grpId = pInsertTreeNode->grpId;

	str.Format( _T("(%d) %s - %s"),pNode->camera_idx, A2W(pNode->camera_name), A2W(pNode->ip));
	HTREEITEM hTreeItm = InsertNode(itemInsertGroup, pTreeNode, str, TVIF_PARAM | TVIF_TEXT | TVIF_HANDLE);
	pNode->hTreeItem = hTreeItm;
	SetItemImage(hTreeItm,4,4);
	DeleteItem(itemDelete);
}


void CTaskTreeCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTVCUSTOMDRAW *pcd = (NMTVCUSTOMDRAW   *)pNMHDR;
	UINT nFlag = TVIS_SELECTED;
	switch ( pcd->nmcd.dwDrawStage )
	{
	case CDDS_PREPAINT: 
		*pResult = CDRF_NOTIFYITEMDRAW;     
		break;

	case CDDS_ITEMPREPAINT : 
		{
			HTREEITEM   hItem = (HTREEITEM)pcd->nmcd.dwItemSpec;

			if ( GetItemState(hItem,nFlag ))
			{
				pcd->clrText = GetSysColor(COLOR_HIGHLIGHTTEXT);    
				pcd->clrTextBk = RGB(200,0,0);
				//m_pMainFrame->m_wndTaskPane.CollapseAllGroups(false);
			}

			*pResult = CDRF_DODEFAULT;// do not set *pResult = CDRF_SKIPDEFAULT
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////	CTaskDeviceTree virtual function
HTREEITEM CTaskDeviceTree::InsertDeviceNode(const ec_Camera& camObj, HTREEITEM& hTreeItm, NODEITEM& camNode)
{
	HTREEITEM hDvTreeItm = NULL;
	TreeNode *pTreeNode = new TreeNode;
	NODEITEM *pNode = new NODEITEM;
	memcpy(pNode, &camNode , sizeof(camNode));
	pTreeNode->nodeType = TreeNode::TreeNdoeType::Dvs_Node;
	pTreeNode->pNode = pNode;
	
	TreeNode* p = GetRootItemFromTreeItem(hTreeItm);
	if (p)
	{
		pNode->nGroup = p->grpId;
		pTreeNode->grpId = p->grpId;
	}

	hDvTreeItm = InsertNode(hTreeItm, pTreeNode, camObj.cameraname, TVIF_PARAM | TVIF_TEXT | TVIF_HANDLE);
	return hDvTreeItm;
}

void CTaskDeviceTree::InsertCameraNode(NODEITEM& node, HTREEITEM& hPrtTreeItm, bool isChannel)
{
	USES_CONVERSION;
	CString str;
	HTREEITEM hTreeItm = NULL;
	TCHAR camera_name[128];
	TCHAR ip[16];
	NODEITEM* pnewNode = new NODEITEM;
	TreeNode *pTreeNode = new TreeNode;
	memcpy(pnewNode, &node , sizeof(node));
	pTreeNode->nodeType = TreeNode::TreeNdoeType::Cam_Node;
	pTreeNode->pNode = pnewNode;

	str.Format( _T("(%d) %s - %s"),node.camera_idx, A2W(node.camera_name), A2W(node.ip));
	TreeNode* p = GetRootItemFromTreeItem(hPrtTreeItm);;
	if (p)
	{
		pnewNode->nGroup = p->grpId;
		pTreeNode->grpId = p->grpId;
	}

	hTreeItm = InsertNode(hPrtTreeItm, pTreeNode, str, TVIF_PARAM | TVIF_TEXT | TVIF_HANDLE);
	pnewNode->hTreeItem = hTreeItm;
	m_vctpAllNodesInTree.push_back(pnewNode);
	SetItemImage(hTreeItm,4,4);
}

bool CTaskDeviceTree::IsDragItemValid(HTREEITEM& hItem)
{
	TreeNode *pTreeNode = GetTreeNode(m_hitemDrag);
	if (pTreeNode->nodeType == TreeNode::TreeNdoeType::Grp_Node || 
		pTreeNode->pNode->bChannel )
		return false;
	else
		return true;
}

void CTaskDeviceTree::DeleteChildNode(HTREEITEM& hItem, bool bOnlyNodeItm)
{
	if (hItem)
	{
		TreeNode*  pTreeNode  = GetTreeNode(hItem);
		if (pTreeNode)
		{
			if (pTreeNode->nodeType == TreeNode::TreeNdoeType::Dvs_Node)
			{
				DeleteChildNodes(hItem);
			}
			m_pbkTalk->DeleteCamRecTime(pTreeNode->pNode->camera_idx);
			m_pbkTalk->DeleteCamRec(pTreeNode->pNode->camera_idx);
			m_pbkTalk->DeleteGroupCamera(pTreeNode->pNode->cameraid);
			m_pbkTalk->DeleteCamera(pTreeNode->pNode->cameraid);
			
			PushDeleteCamId(pTreeNode->pNode->cameraid);

			delete pTreeNode->pNode;
			pTreeNode->pNode = NULL;
			delete pTreeNode;
			pTreeNode = NULL;
			DeleteItem(hItem);
		}
	}
}

void CTaskDeviceTree::CreateTreeGroup()
{
//	WritePrivateProfileString(_T("CreateTreeGroup"),_T("Success"),_T(""),_T("D:\\Lynn.ini"));
	if (!m_pbkTalk->CheckTreeGroup(GetTreeType()))
	{
//		WritePrivateProfileString(_T("CreateTreeGroup"),_T("Fail"),_T(""),_T("D:\\Lynn.ini"));
		vector<Cam_Group> vcGroup;
		m_pbkTalk->GetGroup(AllTree, vcGroup);
		int nIdx = 0, nCount = vcGroup.size();
		
		for (nIdx = 0; nIdx < nCount; nIdx++)
		{
			m_pbkTalk->UpdateGroupType(GetTreeType(),vcGroup[nIdx].cameragroupid);
		}

		vector<ec_Camera> vcCam;
		m_pbkTalk->GetAllCameras(vcCam);
		nCount = vcCam.size();
		DWORD video_format = 0;
		byte byDevice = 0;

		for (nIdx = 0; nIdx < nCount; nIdx++)
		{
			video_format = _ttoi(vcCam[nIdx].video_format);
			byDevice = (video_format & FILTER_SECOND_BYTE) >> 8;

			if (byDevice == eNVR || byDevice == IPVSTATION)
			{
				video_format += CHANNEL;
				vcCam[nIdx].video_format.Format(_T("%d"), video_format);
				m_pbkTalk->UpdateCamera(vcCam[nIdx], false);
			}
		}
	}
}

void CTaskDeviceTree::RefreshLauoutTree()
{
	vector<int> vcCamId;
	GetDeleteCamIds(vcCamId);
	m_pbkTalk->SendMessage(MsgUpdateView, UM_DELETE_TREENODE, (LPARAM)&vcCamId);
}

CMenu* CTaskDeviceTree::CreateCameraNodeMenu(CMenu& menu, HTREEITEM hItem)
{
	CString csCameraName = GetItemText(hItem);
	menu.LoadMenu(IDR_MENU_IPCAM_TREE);	
	CMenu* psubmenu = menu.GetSubMenu(0);
	TreeNode* pNodeitem = GetTreeNode(hItem);
	if (psubmenu && pNodeitem && pNodeitem->pNode)
	{
		psubmenu->EnableMenuItem(IDC_RECORDINGON, MF_GRAYED);
		if( !((m_pbkTalk->GetMainFrame()->m_iAuth)/10) )
		{
			psubmenu->EnableMenuItem(ID_ADD_INTERNET_IP_CAM, MF_GRAYED);
			psubmenu->EnableMenuItem(ID_MODIFY_IPCAM, MF_GRAYED);
			psubmenu->EnableMenuItem(ID_MENU_REMOVEIPCAM32815, MF_GRAYED);
		}
		if( pNodeitem->pNode->byDeviceType == eNVR || 
			pNodeitem->pNode->byDeviceType == IPVSTATION || 
			pNodeitem->pNode->byDeviceType == ISAP)
		{
			psubmenu->EnableMenuItem(ID_ADD_INTERNET_IP_CAM, MF_GRAYED);
			psubmenu->EnableMenuItem(ID_MODIFY_IPCAM, MF_GRAYED);
		}

		if (pNodeitem->pNode->bChannel)
		{
			psubmenu->EnableMenuItem(ID_MODIFY_IPCAM, MF_ENABLED);
		}
	}

	return psubmenu;
}

void CTaskDeviceTree::OnAddNewIpcam()
{
	AddNewIPCam();
}

void CTaskDeviceTree::AddDevice(int nGroupId)
{
	if (nGroupId)
	{
		vector<Cam_Group_Cam> vcGroupCam;
		vcGroupCam.clear();
		Cam_Group_Cam grpCam;
		m_pbkTalk->GetLastGroupCamera(grpCam);

		if (grpCam.cameragroupid == nGroupId)
		{
			if (grpCam.numb_ > 0)
			{
				int nDeviceId = grpCam.numb_;
				m_pbkTalk->GetGroupCamera(nGroupId, vcGroupCam);
				vector<Cam_Group_Cam>::iterator it = vcGroupCam.begin();

				while(it != vcGroupCam.end())
				{
					if (it->numb_ != nDeviceId)
					{
						it = vcGroupCam.erase(it);
						continue;
					}
					it++;
				}
			}
			else
			{
				vcGroupCam.push_back(grpCam);
			}
		}

		HTREEITEM hTreeItm = GetNextItem(TVI_ROOT, TVGN_ROOT);
		TreeNode* pTreeNode;
		while (hTreeItm)
		{
			pTreeNode = GetTreeNode(hTreeItm);
			if (pTreeNode && pTreeNode->grpId == nGroupId)
			{
				InsertChildNode(vcGroupCam, hTreeItm);
				Invalidate(false);
				break;
			}
			hTreeItm = GetNextSiblingItem(hTreeItm);
		}
	}
}

//////////////////////////////////////////////////////////////////////////	CTaskLayoutTree virtual function
HTREEITEM CTaskLayoutTree::InsertDeviceNode(const ec_Camera& camObj, HTREEITEM& hTreeItm, NODEITEM& camNode)
{
	return hTreeItm;
}

void CTaskLayoutTree::CreateTreeGroup()
{
	if (!m_pbkTalk->CheckTreeGroup(GetTreeType()))
	{
		vector<Cam_Group> vcGroup;
		Cam_Group groupObj;
		vector<Cam_Group_Cam> vcGroupCam;
		Cam_Group_Cam groupCamObj;

		int nLastGroup = m_pbkTalk->GetLastGroupIdx()+1;

		m_pbkTalk->GetGroup(GetTreeType(), vcGroup);
		int nGrpIdx = 0, nGrpCount = vcGroup.size();
		int nIdx = 0, nCount = 0;
		for (nGrpIdx = 0; nGrpIdx < nGrpCount; nGrpIdx++, nLastGroup++)
		{
			groupObj.clear();
			vcGroupCam.clear();
			groupObj = vcGroup[nGrpIdx];
			m_pbkTalk->GetGroupCamera(groupObj.cameragroupid,vcGroupCam);
			groupObj.cameragroupid = nLastGroup;
			groupObj.category_code.Format(_T("%d"),GetTreeType());
			m_pbkTalk->InsertGroup(groupObj);
			
			nCount = vcGroupCam.size();
			for (nIdx = 0; nIdx < nCount; nIdx++)
			{
				groupCamObj.clear();
				groupCamObj = vcGroupCam[nIdx];
				groupCamObj.cameragroupid = nLastGroup;
				m_pbkTalk->InsertGroupCamera(groupCamObj);
			}
		}
	}
}

void CTaskLayoutTree::InsertCameraNode(NODEITEM& node, HTREEITEM& hPrtTreeItm, bool isChannel)
{
	USES_CONVERSION;
	CString str;
	HTREEITEM hTreeItm = NULL;
	TCHAR camera_name[128];
	TCHAR ip[16];
	NODEITEM* pnewNode = new NODEITEM;
	TreeNode *pTreeNode = new TreeNode;
	memcpy(pnewNode, &node , sizeof(node));
	pTreeNode->nodeType = TreeNode::TreeNdoeType::Cam_Node;
	pTreeNode->pNode = pnewNode;

	str.Format( _T("(%d) %s - %s"),node.camera_idx, A2W(node.camera_name), A2W(node.ip));
	TreeNode* p = GetRootItemFromTreeItem(hPrtTreeItm);;
	if (p)
	{
		pnewNode->nGroup = p->grpId;
		pTreeNode->grpId = p->grpId;
	}

	hTreeItm = InsertNode(hPrtTreeItm, pTreeNode, str, TVIF_PARAM | TVIF_TEXT | TVIF_HANDLE);
	pnewNode->hTreeItem = hTreeItm;
	m_vctpAllNodesInTree.push_back(pnewNode);
	SetItemImage(hTreeItm,4,4);
}

bool CTaskLayoutTree::IsDragItemValid(HTREEITEM& hItem)
{
	TreeNode *pTreeNode = GetTreeNode(m_hitemDrag);
	if (pTreeNode->nodeType == TreeNode::TreeNdoeType::Grp_Node)
		return false;
	else
		return true;
}

void CTaskLayoutTree::DeleteChildNode(HTREEITEM& hItem, bool bOnlyNodeItm)
{
	if (hItem)
	{
		TreeNode*  pTreeNode  = GetTreeNode(hItem);
		if (pTreeNode)
		{
			if (!bOnlyNodeItm)
			{
				vector<Cam_Group_Cam> vcGroupCam;
				m_pbkTalk->GetGroupCamera(pTreeNode->grpId, pTreeNode->pNode->cameraid, vcGroupCam);
				if(vcGroupCam.size())
					m_pbkTalk->DeleteGroupCameraByGroupCameraId(vcGroupCam[0].cameragroupcameraid);
			} 
			delete pTreeNode;
			pTreeNode = NULL;
			DeleteItem(hItem);
		}
	}
}

void CTaskLayoutTree::DeleteTreeNodeByCamId(vector<int> *pvcCamId)
{
	HTREEITEM hGroupItem = GetRootItem();
	HTREEITEM hChildItem, hTemp;
	TreeNode *pTreeNode = NULL;
	vector<int>::iterator it;

	while(hGroupItem)
	{
		hChildItem = GetChildItem(hGroupItem);
		while(hChildItem)
		{
			hTemp = hChildItem;
			hChildItem = GetNextItem(hTemp, TVGN_NEXT);
			pTreeNode = GetTreeNode(hTemp);	
			it = find(pvcCamId->begin(), pvcCamId->end(), pTreeNode->pNode->cameraid);
			if (it != pvcCamId->end())
			{
				DeleteChildNode(hTemp);
			}
		}
		hGroupItem = GetNextItem(hGroupItem, TVGN_NEXT);
	}
}

CMenu* CTaskLayoutTree::CreateCameraNodeMenu(CMenu& menu, HTREEITEM hItem)
{
	menu.LoadMenu(IDR_MENU_IPCAM_TREE);	
	CMenu *pMenu = menu.GetSubMenu(1);
	CMenu *psubMenu = pMenu->GetSubMenu(0);
	NODEITEM *pNode = GetCurrentNodeItem();

	if (psubMenu && pNode)
	{
		ec_Camera camObj;
		m_pbkTalk->GetCamera(pNode->cameraid, camObj);
		int nIdx = 0, nCount = camObj.vcStream.size();
		CString str;
		if (nCount > 1)
		{
			bool bCheck = false;
			for(int nIdx = 1; nIdx < nCount ; nIdx++)
			{
				str.Format(_T("Stream %d"), nIdx+1);
				psubMenu->InsertMenu(nIdx,MF_UNCHECKED|MF_STRING, ID_SELECTSREAM_STREAM+nIdx, str);
			}
			psubMenu->CheckMenuItem(ID_SELECTSREAM_STREAM+(pNode->nStreamID-1), MF_CHECKED);
		}
	}

	return pMenu;
}

void CTaskLayoutTree::OnRangeCmds(UINT nID)
{
	CString str;
	CMenu menu;	
	CMenu* psubmenu = NULL;
	menu.LoadMenu(IDR_MENU_IPCAM_TREE);	
	CMenu *pMenu = menu.GetSubMenu(1);
	CMenu *psubMenu = pMenu->GetSubMenu(0);
	int nStream = nID - ID_SELECTSREAM_STREAM;

	NODEITEM *pNode = GetCurrentNodeItem();
	ec_Camera camObj;
	m_pbkTalk->GetCamera(pNode->cameraid, camObj);

	if (pNode && nStream >= 0 && nStream < camObj.vcStream.size())
	{
		int nIdx = 0, nCount = camObj.vcStream.size();

		for(int nIdx = 0; nIdx < nCount ; nIdx++)
		{
			if (nStream == nIdx)
			{
				pNode->nStreamID = nIdx+1;
				psubMenu->CheckMenuItem(nID, MF_CHECKED);
			} 
			else
			{
				psubMenu->CheckMenuItem(nID, MF_UNCHECKED);
			}
			
		}
	}
}

void CTaskLayoutTree::OnAddNewIpcam()
{
    VK vk;
	CDlgSettingCamGroup dlg;
	dlg.DoModal();
}

