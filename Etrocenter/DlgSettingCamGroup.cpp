// DlgSettingCamGroup.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSettingCamGroup.h"
#include "IPCamTreeView.h"
#include "MainFrm.h"
#include "AddDevice.h"
#define		TOPY	80
//#define		STARTID	1104

#define CAMERAGROUPBKCOLOR RGB(255, 255, 255)

// CDlgSettingCamGroup dialog

IMPLEMENT_DYNAMIC(CDlgSettingCamGroup, CBaseDialog)

CDlgSettingCamGroup::CDlgSettingCamGroup(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgSettingCamGroup::IDD, pParent)
	, i_cameragroupid(0), m_curGroupId(0), m_iGroupId(1),m_pDragImage(NULL)
	, m_iCameraId(0), m_pBk(NULL),m_TreeType(),m_nDropGroupIndex(0),m_pDragList(NULL)
	, m_pDropTree(NULL),m_DragItem(NULL),m_pDropWnd(NULL),pItem(NULL),m_CamList(NULL)
	, m_bAddNewDevice(false)
{

}

CDlgSettingCamGroup::~CDlgSettingCamGroup()
{
}

void CDlgSettingCamGroup::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAMGROUP_DEL, m_CamgroupDel);
	DDX_Control(pDX, IDC_EDITGROUPNAME, m_GroupName);
}


BEGIN_MESSAGE_MAP(CDlgSettingCamGroup, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_CAMGROUPTREE, &CDlgSettingCamGroup::OnTvnSelchangedCamgrouptree)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_CAMERALIST, &CDlgSettingCamGroup::OnLvnBegindragCameralist)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_CAMGROUPTREE, &CDlgSettingCamGroup::OnLvnBegindragCameratree)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CAMGROUP_DEL, &CDlgSettingCamGroup::OnBnClickedCamgroupDel)
	ON_BN_CLICKED(IDC_CAMGROUP_ADD, &CDlgSettingCamGroup::OnBnClickedCamgroupAdd)
	ON_BN_CLICKED(IDC_CAMGROUP_EDIT, &CDlgSettingCamGroup::OnBnClickedCamgroupEdit)
	ON_BN_CLICKED(IDC_CAMGROUP_ADD_ISAP, &CDlgSettingCamGroup::OnBnClickedCamgroupISAP)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_REGISTERED_MESSAGE(MsgUpdateView, OnUpdateView)
	ON_BN_CLICKED(IDCANCEL, &CDlgSettingCamGroup::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgSettingCamGroup message handlers

HBRUSH CDlgSettingCamGroup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CDlgSettingCamGroup::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

BOOL CDlgSettingCamGroup::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	CString csDialogCaption(_T(""));
	LOADSTRING(csDialogCaption, IDS_DLG_CAM_GROUP);
    csDialogCaption = csDialogCaption +  CProductDefine::m_tzProductName;
	SetWindowText(csDialogCaption);

	m_bDirty = false;
	m_curGroupId = 0;

	m_CamGroup.ModifyStyle(0, TVS_LINESATROOT|TVS_HASBUTTONS|TVS_HASLINES);
	m_CamGroup.SetBkColor(CAMERAGROUPBKCOLOR);
		
	//Camera List	 
	m_CamList = (CListCtrl*) GetDlgItem(IDC_CAMERALIST);
	m_CamList->SetExtendedStyle(LVS_EX_GRIDLINES);
	m_CamList->SetBkColor(CAMERAGROUPBKCOLOR);
	
	m_ImgList.Create(16, 16, ILC_COLOR, 2, 2);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_ICON_CAM16));
	m_CamList->SetImageList(&m_ImgList, LVSIL_SMALL);

	CString ls_st;
    LOADSTRING(ls_st, IDS_CAMERA_LIST);
    SetDlgItemText(IDC_ST_CAMERALIST, ls_st);
    LOADSTRING(ls_st, IDS_CAMERA_GROUP_SETTING_TITLE);
    SetDlgItemText(IDC_ST_HEAD, ls_st);
    LOADSTRING(ls_st, IDS_GROUP_LIST);
    SetDlgItemText(IDC_ST_GROUPLIST, ls_st);

	m_ToolTip= new CToolTipCtrl();
	m_ToolTip->Create(this);

	LOADSTRING(ls_st, IDS_TOOL_ADD_GROUP);
	m_ToolTip->AddTool(&m_CamgroupAdd, ls_st);

	LOADSTRING(ls_st, IDS_TOOL_DEL_GROUP);
	m_ToolTip->AddTool(&m_CamgroupDel,ls_st);

	LOADSTRING(ls_st, IDS_TOOL_MOD_GROUP);
	m_ToolTip->AddTool(&m_CamgroupEdit,ls_st);

	LOADSTRING(ls_st, IDS_HINT_ADD_CAMERA);
	m_ToolTip->AddTool(&m_CamgroupISAP,ls_st);
	m_ToolTip->Activate(TRUE);

	vInitImageList();
	vBuildGroupTree();
	vPopulateCamera();

	mFont.CreatePointFont(90, STDFONT);

   	LV_COLUMN ListColumn;
	WCHAR * ListTitles[1] = {_T("Camera ")};
	int ll_colw=285;

	for(int i = 0; i < 1; i++)
	{
		ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		ListColumn.fmt = LVCFMT_LEFT;
		ListColumn.iSubItem = i;

		ListColumn.cx = ll_colw;
		
		ListColumn.pszText = ListTitles[i];
		m_CamList->InsertColumn(i, &ListColumn);
	}

	AdjustDlgViewByTreeType();
	m_hArrowCursor	= ::LoadCursor(NULL, IDC_ARROW);
	m_hNoCursor	= ::LoadCursor(NULL, IDC_NO);
	m_pBk->RegistWnd(this);
	KeepOldCamera();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSettingCamGroup::KeepOldCamera()
{
	m_pBk->GetGroup(m_TreeType, m_oldCamGroups);
	
	int nIdx = 0, nCount = m_oldCamGroups.size();
	for (nIdx = 0; nIdx< nCount; nIdx++)
	{
		m_pBk->GetGroupCamera(m_oldCamGroups[nIdx].cameragroupid, m_oldCamGroupCams);
	}

	ec_Camera camObj;
	nCount = m_oldCamGroupCams.size();
	for (nIdx = 0; nIdx< nCount; nIdx++)
	{
		camObj.clear();
		m_pBk->GetCamera(m_oldCamGroupCams[nIdx].cameraid, camObj);
		m_oldCams.push_back(camObj);
	}
}

HTREEITEM CDlgSettingCamGroup::InsertRootTreeNode(int nGrpId, const CString& strGrpName)
{
	HTREEITEM hItem = NULL;
	TreeNode *pNode = new TreeNode;
	pNode->nodeType = TreeNode::Grp_Node;
	pNode->nGrpID = nGrpId;

	TVINSERTSTRUCTW itemStru;
	itemStru.hParent = TVGN_ROOT;
	itemStru.item.lParam = (LPARAM)pNode;
	itemStru.item.mask = TVIF_PARAM | TVIF_TEXT |  TVIS_BOLD;
	itemStru.item.pszText = (LPWSTR)(LPCTSTR)strGrpName;
	itemStru.item.cchTextMax = strGrpName.GetLength();
	hItem = m_CamGroup.InsertItem(&itemStru);

	return hItem;
}

int CDlgSettingCamGroup::vBuildGroupTree(void)
{
	int nGroupid = 0;
 
	USES_CONVERSION;

	vector<Cam_Group> vcGroup;
	vector<Cam_Group_Cam> vcGrpCam;
	m_pBk->GetGroup(m_TreeType, vcGroup);

	int nCount = vcGroup.size();
	for (int nIdx = 0; nIdx < nCount; nIdx++)
	{
		vcGrpCam.clear();
		nGroupid = vcGroup[nIdx].cameragroupid;
		itemSpec[nIdx] = InsertRootTreeNode(nGroupid, vcGroup[nIdx].camera_group_desc);
		m_pBk->GetGroupCamera(nGroupid, vcGrpCam);
		vBuildCamToGroup(nGroupid, vcGrpCam, itemSpec[nIdx]);

		if (vcGroup[nIdx].default_==_T("Y"))
		{
			m_CamGroup.SelectItem(itemSpec[nIdx]);
			m_CamGroup.Expand(itemSpec[nIdx],TVE_EXPAND);
			m_curGroupId = nGroupid;
		}
	}

	mi_GroupCnt = nCount;
	return 0;
}

int CDlgSettingCamGroup::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int ll_x=190, ll_w=24, ll_y=50, ll_h=24, ll_gap=5, ll_high=340;	
	int ll_x1=360,ll_x2=20, ll_width=280;
	
	if (CBaseDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pBk = CNVRBk::GetBkObject();
	m_TreeType = m_pBk->GetTreeType();
	DWORD ldwStyle = WS_VISIBLE | TVIF_IMAGE | WS_CHILD | WS_BORDER | TVS_HASBUTTONS |  TVS_HASLINES| TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
	m_CamGroup.Create(ldwStyle, CRect(ll_x2, TOPY, ll_x2+ll_width, TOPY+ll_high), this, IDC_CAMGROUPTREE);
	m_CamGroup.SetBkColor(CAMERAGROUPBKCOLOR);
		
	ldwStyle = WS_CHILD|WS_VISIBLE|BS_OWNERDRAW ;
	m_CamgroupAdd.Create(NULL, ldwStyle, CRect(ll_x, ll_y, ll_x+ll_w ,ll_y+ll_h), this, IDC_CAMGROUP_ADD);
	m_CamgroupAdd.LoadBitmaps(IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD, IDB_CAMGROUP_ADD);
	
	ll_x += ll_w + ll_gap;
	m_CamgroupDel.Create(NULL, ldwStyle, CRect(ll_x, ll_y, ll_x+ll_w ,ll_y+ll_h), this, IDC_CAMGROUP_DEL);
	m_CamgroupDel.LoadBitmaps(IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL, IDB_CAMGROUP_DEL_DIS);
	m_CamgroupDel.EnableWindow(false);

	ll_x += ll_w + ll_gap;

	m_CamgroupEdit.Create(NULL, ldwStyle, CRect(ll_x, ll_y, ll_x+ll_w ,ll_y+ll_h), this, IDC_CAMGROUP_EDIT);
	m_CamgroupEdit.LoadBitmaps(IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT, IDB_CAMGROUP_EDIT_DIS);
	m_CamgroupEdit.EnableWindow(false);

	ll_x += ll_w + ll_gap;

	m_CamgroupISAP.Create(NULL, ldwStyle, CRect(ll_x, ll_y, ll_x+ll_w, ll_y+ll_h), this, IDC_CAMGROUP_ADD_ISAP);
	m_CamgroupISAP.LoadBitmaps(IDB_ISAP_ADD, IDB_ISAP_ADD, IDB_ISAP_ADD, IDB_ISAP_ADD);

	m_bDragging = false;

	return 0;
}

void CDlgSettingCamGroup::ReassignCamGrpCam(const vector<Cam_Group_Cam>& vcGroupCam, vector< vector<Cam_Group_Cam> >& vcGroupCamByDevice)
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

bool CDlgSettingCamGroup::IsNeed2InsertDeviceNode(const vector<Cam_Group_Cam>& vcGroupCam, int& nDeviceNodeIdx)
{
	bool bResult = false;
	ec_Camera camObj;
	int nCount = vcGroupCam.size();
	for (int nIdx = 0; nIdx < nCount; nIdx++)
	{
		camObj.clear();
		m_pBk->GetCamera(vcGroupCam[nIdx].cameraid, camObj);

		if (IsDevice(camObj))
		{
			nDeviceNodeIdx = nIdx;
			bResult = true;
			break;
		}
	}

	return bResult;
}

HTREEITEM CDlgSettingCamGroup::GetInsertAfterItem(HTREEITEM& hParent, TreeNode *pSortTreeNode)
{
	if (pSortTreeNode->nodeType == TreeNode::NodeType::Dvs_Node || pSortTreeNode->nodeType == TreeNode::NodeType::Grp_Node)
	{
		return TVI_LAST;
	}
	else
	{
		HTREEITEM groupRoot = GetGroupItem(hParent);
		HTREEITEM childItem = m_CamGroup.GetChildItem(groupRoot);
		HTREEITEM previousItem = NULL;
		HTREEITEM resultItem = NULL;
		bool bChildItem = false;

		TreeNode *pTreeNode = NULL;
		while(childItem)
		{
			bChildItem = true;
			pTreeNode = GetTreeNode(childItem);

			if (pTreeNode->nodeType == TreeNode::NodeType::Dvs_Node)
			{
				childItem = m_CamGroup.GetNextItem(childItem, TVGN_NEXT);
				continue;
			}
			if (pTreeNode->nCamIndex > pSortTreeNode->nCamIndex)
			{
				resultItem = previousItem;
				break;
			}
			previousItem = childItem;
			childItem = m_CamGroup.GetNextItem(childItem, TVGN_NEXT);
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

HTREEITEM CDlgSettingCamGroup::InsertChildTreeNode(int nGroupid, TreeNode::NodeType nodeType, const ec_Camera& camObj, const CString& nodeName, HTREEITEM& hParent)
{
	HTREEITEM hItem = NULL;
	TreeNode *pNode = new TreeNode;
	pNode->nodeType = nodeType;
	pNode->nCamID = camObj.cameraid;
	pNode->nGrpID = nGroupid;
	pNode->nCamIndex = camObj.camera_idx;

	TVINSERTSTRUCTW itemStru;
	itemStru.hParent = hParent;
	itemStru.hInsertAfter = GetInsertAfterItem(hParent,pNode);
	itemStru.item.lParam = (LPARAM)pNode;
	itemStru.item.mask = TVIF_TEXT|TVIF_HANDLE|TVIF_PARAM;
	itemStru.item.pszText = (LPWSTR)(LPCTSTR)nodeName;
	itemStru.item.cchTextMax = nodeName.GetLength();
	hItem = m_CamGroup.InsertItem(&itemStru);

	return hItem;
}

int CDlgSettingCamGroup::vBuildCamToGroup(int nGroupid, const vector<Cam_Group_Cam>& vcGrpCam, HTREEITEM& hParent)
{	
	USES_CONVERSION;

	bool bDeviceNode = false;
	ec_Camera camObj;
	vector<Cam_Group_Cam> vcTempGrpCam;
	vector< vector<Cam_Group_Cam> > vcvcGrpCam;
	ReassignCamGrpCam(vcGrpCam, vcvcGrpCam);
	CString str;
	HTREEITEM hDevice = NULL, hItem = NULL;

	int nGrpCount = vcvcGrpCam.size(), nCount = 0, nDeviceNodeIdx = 0;
	for (int nGrpIdx = 0; nGrpIdx < nGrpCount; nGrpIdx++)
	{
		bDeviceNode = false;
		vcTempGrpCam.clear();
		vcTempGrpCam = vcvcGrpCam[nGrpIdx];
		nCount = vcTempGrpCam.size();

		if (IsNeed2InsertDeviceNode(vcTempGrpCam, nDeviceNodeIdx))
		{
			bDeviceNode = true;
			m_pBk->GetCamera(vcTempGrpCam[nDeviceNodeIdx].cameraid, camObj);
			
			if (m_TreeType == TreeType::DeviceTree)
			{
				hDevice = InsertChildTreeNode(nGroupid, TreeNode::Dvs_Node, camObj, camObj.cameraname, hParent);
			}
		}

		for (int nIdx = 0 ; nIdx < nCount; nIdx++)
		{
			camObj.clear();
			if (bDeviceNode && nDeviceNodeIdx == nIdx)
				continue;

			m_pBk->GetCamera(vcTempGrpCam[nIdx].cameraid, camObj);
			str.Format( _T("(%d) %s - %s"),camObj.camera_idx,camObj.cameraname, camObj.ipaddress);

			if (bDeviceNode && m_TreeType == TreeType::DeviceTree)
				hItem = InsertChildTreeNode(nGroupid, TreeNode::Cmr_Node, camObj, str, hDevice);
			else
				hItem = InsertChildTreeNode(nGroupid, TreeNode::Cmr_Node, camObj, str, hParent);

			m_CamGroup.SetItemImage(hItem,1,1);
			mi_CameraCnt++;
		}
	}

	return 0;
}

void CDlgSettingCamGroup::OnTvnSelchangedCamgrouptree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	*pResult = 0;
	bool lb_Group = true;

	m_CamgroupDel.EnableWindow(false);
	TVITEM TreeItem, CamTreeItem;
	HTREEITEM  hItem = m_CamGroup.GetSelectedItem();	
	
	//On the select group, populate the group default & other info
	if (m_CamGroup.GetParentItem(hItem) != NULL) //Camera Items
	{
		m_CamgroupDel.EnableWindow(true);
		m_curCamIdx = GetTreeNode(hItem)->nGrpID;
		hItem = m_CamGroup.GetParentItem(hItem);
		lb_Group = false;
	}
	
	TreeNode *pTreeNode = GetTreeNode(hItem);
	if (pTreeNode)
	{
		Cam_Group groupObj;
		m_nDropGroupIndex = pTreeNode->nGrpID;
		m_pBk->GetGroup(m_nDropGroupIndex, groupObj);

		if (!groupObj.isempty())
		{
			m_curGroupId = pTreeNode->nGrpID;
		}	
		m_CamgroupEdit.EnableWindow(true);
	}

	if(m_CamGroup.GetParentItem(hItem) == NULL &&
		m_CamGroup.GetNextItem(hItem,TVGN_CHILD) == NULL)
	{
		m_CamgroupDel.EnableWindow(true);
	}
}

bool CDlgSettingCamGroup::IsDevice(const ec_Camera& cameraObj)
{
	USES_CONVERSION;
	DWORD video_format = atoi(W2A(cameraObj.video_format));

	if (video_format & 0x02)
	{
		return false;
	}
	else
	{
		if (video_format & 0x0200 || video_format & 0x0100 )
			return true;
		else
			return false;
	}
}

bool CDlgSettingCamGroup::IsChannel(const ec_Camera& cameraObj)
{
	USES_CONVERSION;
	DWORD video_format = atoi(W2A(cameraObj.video_format));

	if (video_format & 0x02)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int CDlgSettingCamGroup::vPopulateCamera(void)
{
	vector<Cam_Group> vcGroup;
	vector<Cam_Group_Cam> vcGroupCam;
	ec_Camera cameraObj;

	m_pBk->GetGroup(TreeType::DeviceTree, vcGroup);

	int nGrpIdx = 0, nGrpCount = vcGroup.size();
	int nIdx = 0, nCount = 0;
	int ci = 0;

	for (nGrpIdx = 0; nGrpIdx < nGrpCount; nGrpIdx++)
	{
		vcGroupCam.clear();
		m_pBk->GetGroupCamera(vcGroup[nGrpIdx].cameragroupid, vcGroupCam);
		nCount = vcGroupCam.size();
		for (nIdx = 0; nIdx < nCount; nIdx++,ci++)
		{
			cameraObj.clear();
			m_pBk->GetCamera(vcGroupCam[nIdx].cameraid, cameraObj);
			
			if (IsDevice(cameraObj))
			{
				ci--;
				continue;
			}
			
			InsertListCtrl(ci, cameraObj);
		}
	}	
	
	if (m_TreeType == TreeType::DeviceTree)
	{
		m_CamList->EnableWindow(FALSE);
	}
	return 0;
}

void CDlgSettingCamGroup::InsertListCtrl(int nIdx, const ec_Camera& cameraObj)
{
	TCHAR lc_desc[255];
	LV_ITEM Lvitem = {0};
	wsprintf(lc_desc,_T("(%d) %s - %s"), cameraObj.camera_idx,cameraObj.cameraname, cameraObj.ipaddress);
	Lvitem.mask = LVIF_TEXT|LVIF_PARAM|LVIF_STATE;
	Lvitem.iItem = nIdx;
	Lvitem.iSubItem = 0;
	Lvitem.pszText = lc_desc;
	Lvitem.lParam = (LPARAM)cameraObj.cameraid;
	m_CamList->InsertItem(&Lvitem);
}

void CDlgSettingCamGroup::OnLvnBegindragCameratree(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(m_TreeType == TreeType::DeviceTree)
	{
		LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
		// TODO: Add your control notification handler code here
		*pResult = 0;

		NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
		
		HTREEITEM hItemDrag = pNMTreeView->itemNew.hItem;

		if (!IsDragAvalible(hItemDrag))
		{
			if(m_hNoCursor != ::GetCursor()) ::SetCursor(m_hNoCursor);
		}
		else
		{
			if(m_hNoCursor == ::GetCursor()) ::SetCursor(m_hArrowCursor);
		}

		//// Create a drag image
		POINT pt;
		int nOffset = -8;
		pt.x = nOffset;
		pt.y = nOffset;

		m_pDragImage = m_CamGroup.CreateDragImage(hItemDrag);
		m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset));
		m_pDragImage->DragEnter(GetDesktopWindow(), pt);
		m_bDragging = TRUE;
		m_DragItem = hItemDrag; 
		SetCapture ();
	}
}

void CDlgSettingCamGroup::OnLvnBegindragCameralist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	m_nDragIndex = pNMListView->iItem;

	//// Create a drag image
	POINT pt;
	int nOffset = -8;
	pt.x = nOffset;
	pt.y = nOffset;

	m_pDragImage = m_CamList->CreateDragImage(m_nDragIndex, &pt);
	ASSERT(m_pDragImage);

	CString csListText = m_CamList->GetItemText(m_CamList->GetSelectionMark(), 0);
	if(csListText.Find(_T("ISAP")) != -1)	return;

	if(m_CamList->GetSelectedCount() > 1) //more than 1 item in list is selected
	{
		CBitmap bitmap;
		m_pDragImage->Replace(0, &bitmap, &bitmap);
	}

	m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset));
	m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);
	m_bDragging = TRUE;

	m_pDragList = m_CamList; //make note of which list we are dragging from

	SetCapture ();

}

void CDlgSettingCamGroup::OnMouseMove(UINT nFlags, CPoint point)
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

		// Save current window pointer as the CListCtrl we are dropping onto
		m_pDropWnd = pDropWnd;

		// Convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient(&pt);
		 
		//If we are hovering over the Group/Camera Tree, need to selected the
		if (pDropWnd->IsKindOf(RUNTIME_CLASS (CTreeCtrl))) 
		{	 		   
			TVHITTESTINFO tvHit;
			tvHit.pt = point;
			HTREEITEM hTarget = m_CamGroup.HitTest(&tvHit);

			// Select the item that is at the point myPoint.
			UINT uFlags;
			HTREEITEM hItem = m_CamGroup.HitTest(pt, &uFlags);

 			if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
			{
				m_CamGroup.SelectItem(hItem);
			}
		}
		m_pDragImage->DragShowNolock(true);
	}		
 
	CBaseDialog::OnMouseMove(nFlags, point);
}

void CDlgSettingCamGroup::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDragging)
	{
		ReleaseCapture ();
		m_bDragging = FALSE;
		m_pDragImage->DragLeave (GetDesktopWindow ());
		m_pDragImage->EndDrag ();
		delete m_pDragImage; //must delete it because it was created at the beginning of the drag
		CPoint pt (point); //Get current mouse coordinates
		ClientToScreen (&pt); //Convert to screen coordinates
		// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd); //make sure we have a window pointer
		m_pDropTree = (CTreeCtrl*)pDropWnd; //Set pointer to the list we are dropping on
		if ( pDropWnd->GetDlgCtrlID() != IDC_CAMGROUPTREE)
			return;

		if (m_TreeType == LayoutTree)
		{
			vDropItemOnTree(m_pDragList, m_pDropTree); //Call routine to perform the actual drop
		}
		else
		{
			vDropItemOnTree(m_DragItem, m_pDropTree);
		}
	}
	CBaseDialog::OnLButtonUp(nFlags, point);
}

int CDlgSettingCamGroup::vDropItemOnTree(HTREEITEM& DragItem, CTreeCtrl* pDropTree)
{
	TreeNode *pDragTreeNode = NULL, *pDropTreeNode = NULL;

	HTREEITEM  hSelItem = m_CamGroup.GetSelectedItem(), hItem, hDragChildItem;	
	pDragTreeNode = GetTreeNode(DragItem);
	pDropTreeNode = GetTreeNode(hSelItem);

	if (!hSelItem) //No group selected
	{
		AfxMessageBox(IDS_GROUP_SELECT);
		return 0;
	}	

	if (pDragTreeNode && pDropTreeNode)
	{	
		if (IsDragAvalible(DragItem) && IsDropAvalible(hSelItem))
		{
			int nCameraID = pDragTreeNode->nCamID;
			ec_Camera camObj;
			m_pBk->GetCamera(nCameraID, camObj);

			if (pDragTreeNode->nodeType == TreeNode::NodeType::Cmr_Node)
			{
				hItem = InsertChildTreeNode(pDropTreeNode->nGrpID, TreeNode::NodeType::Cmr_Node, camObj, m_CamGroup.GetItemText(DragItem), hSelItem);
				m_CamGroup.SetItemImage(hItem,1,1);
			}
			else if (pDragTreeNode->nodeType == TreeNode::NodeType::Dvs_Node)
			{
				HTREEITEM hDeviceItem = InsertChildTreeNode(pDropTreeNode->nGrpID, TreeNode::NodeType::Dvs_Node, camObj, m_CamGroup.GetItemText(DragItem), hSelItem);
				hDragChildItem = m_CamGroup.GetChildItem(DragItem);
				while(hDragChildItem)
				{
					camObj.clear();
					pDragTreeNode = GetTreeNode(hDragChildItem);
					m_pBk->GetCamera(pDragTreeNode->nCamID, camObj);

					hItem = InsertChildTreeNode(pDropTreeNode->nGrpID, TreeNode::NodeType::Cmr_Node, camObj, m_CamGroup.GetItemText(hDragChildItem), hDeviceItem);
					m_CamGroup.SetItemImage(hItem,1,1);
					hDragChildItem = m_CamGroup.GetNextItem(hDragChildItem, TVGN_NEXT);
				}
			}
			
			DeleteTreeNode(DragItem, false);
			m_CamGroup.DeleteItem(DragItem);
			m_bDirty=true;
		}
	}
	return 0;
}

int CDlgSettingCamGroup::vDropItemOnTree(CListCtrl* pDragList, CTreeCtrl* pDropTree)
{	
	HTREEITEM  hItem = m_CamGroup.GetSelectedItem();	
	if (m_CamGroup.GetParentItem(hItem) != NULL) //Camera Item, select the parent
		hItem = m_CamGroup.GetParentItem(hItem);

	if (!hItem) //No group selected
	{
		AfxMessageBox(IDS_GROUP_SELECT);
		//AfxMessageBox(_T("Please select a camera group first."));
		return 0;
	}	

	int nItem = 0, nCameraID = 0;
	nItem = m_CamList->GetSelectedCount();
	nCameraID = m_CamList->GetItemData(m_nDragIndex);
	TreeNode *pTreeNode = GetTreeNode(hItem);
	if (pTreeNode)
	{
		ec_Camera camObj;
		m_pBk->GetCamera(nCameraID, camObj);
		hItem = InsertChildTreeNode(pTreeNode->nGrpID, TreeNode::NodeType::Cmr_Node, camObj, m_CamList->GetItemText(m_nDragIndex,0), hItem);
		m_CamGroup.SetItemImage(hItem,1,1);
		m_bDirty=true;
	}

	return 0;
}


bool CDlgSettingCamGroup::vCheckExist(int o_camidx, HTREEITEM hpitem)
{
	bool lb_ret=false;
	//HTREEITEM hmyItem = m_TreeCtrl.GetSelectedItem();

	// Delete all of the children of hmyItem.
	if (m_CamGroup.ItemHasChildren(hpitem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hCurItem = m_CamGroup.GetChildItem(hpitem);

		while (hCurItem != NULL)
		{
			if (o_camidx == GetTreeNode(hCurItem)->nGrpID) return true;

			hNextItem = m_CamGroup.GetNextItem(hCurItem, TVGN_NEXT);
			
			

			hCurItem = hNextItem;
		}
	}
	else
	{
			return lb_ret;  //Return false if no child
	}

	return lb_ret;
}

void CDlgSettingCamGroup::OnOK()
{
	if (m_EntryMode == _T("AMC"))
	{
	}

	// TODO: Add your specialized code here and/or call the base class
	if (m_bDirty && m_EntryMode!=_T("AMC")) 
	{
		vSaveDB();	
	}

	CBaseDialog::OnOK();
}

int CDlgSettingCamGroup::InsertGroup(const CString& str)
{
	Cam_Group groupObj;
	groupObj.cameragroupid = m_pBk->GetLastGroupIdx() + 1;
	groupObj.camera_group_desc = str;
	groupObj.default_ = _T("N");
	groupObj.category_code.Format(_T("%d"),m_TreeType);
	groupObj.module_code = _T("LIVEVIEW");
	m_pBk->InsertGroup(groupObj);

	return groupObj.cameragroupid;
}

void CDlgSettingCamGroup::InsertStation(HTREEITEM& hParItem, int nGroupId)
{
	TreeNode* pChildTreeNode = NULL;
	Cam_Group_Cam groupCam;
	ec_Camera camObj;
	pChildTreeNode = GetTreeNode(hParItem);
	if (pChildTreeNode)
	{
		m_pBk->GetCamera(pChildTreeNode->nCamID, camObj);
		groupCam.cameragroupid = nGroupId;
		groupCam.cameraid = pChildTreeNode->nCamID;
		groupCam.camera_idx = camObj.camera_idx;
		groupCam.numb_ = m_pBk->GetLastDeviceIdx()+1;
		groupCam.streamid = 1;
		m_pBk->InsertGroupCamera(groupCam);

		HTREEITEM hItem;
		hItem = m_CamGroup.GetChildItem(hParItem);
		while(hItem)
		{
			groupCam.clear();
			camObj.clear();
			pChildTreeNode = GetTreeNode(hItem);
			if (pChildTreeNode)
			{
				m_pBk->GetCamera(pChildTreeNode->nCamID, camObj);
				groupCam.cameragroupid = nGroupId;
				groupCam.cameraid = pChildTreeNode->nCamID;
				groupCam.camera_idx = camObj.camera_idx;
				groupCam.numb_ = m_pBk->GetLastDeviceIdx();
				groupCam.streamid = 1;
				m_pBk->InsertGroupCamera(groupCam);
				hItem = m_CamGroup.GetNextItem(hItem, TVGN_NEXT);
			}
			else
				break;
		}
	}
}

void CDlgSettingCamGroup::InsertCamera(HTREEITEM& hParItem, int nGroupId)
{
	HTREEITEM hItem;
	hItem = m_CamGroup.GetChildItem(hParItem);
	TreeNode* pChildTreeNode = NULL;
	Cam_Group_Cam groupCam;
	ec_Camera camObj;

	while(hItem)
	{
		groupCam.clear();
		camObj.clear();
		pChildTreeNode = GetTreeNode(hItem);
		if (pChildTreeNode)
		{
			m_pBk->GetCamera(pChildTreeNode->nCamID, camObj);

			if(m_pBk->GetDevice(camObj.video_format) != IPCAMERA &&
				m_pBk->GetDevice(camObj.video_format) != ONVIF)
				InsertStation(hItem,nGroupId);
			else
			{
				groupCam.cameragroupid = nGroupId;
				groupCam.cameraid = pChildTreeNode->nCamID;
				groupCam.camera_idx = camObj.camera_idx;
				groupCam.numb_ = 0;
				groupCam.streamid = 1;
				m_pBk->InsertGroupCamera(groupCam);
			}
			hItem = m_CamGroup.GetNextItem(hItem, TVGN_NEXT);
		}
		else
			break;
	}
}

void CDlgSettingCamGroup::SaveLayoutTreeDB()
{
	DeleteGroupCamera();
	DeleteGroup();
	
	HTREEITEM hItem =  m_CamGroup.GetRootItem(), hChildItem = NULL;
	TreeNode* pTreeNode = NULL;
	CString strItemText;
	int nGroupId = 0;

	while(hItem)
	{
		pTreeNode = GetTreeNode(hItem);
		if (pTreeNode)
		{
			strItemText = m_CamGroup.GetItemText(hItem);
			nGroupId = InsertGroup(strItemText);
			InsertCamera(hItem, nGroupId);
		}
		hItem = m_CamGroup.GetNextItem(hItem, TVGN_NEXT);
	}
}

void CDlgSettingCamGroup::DeleteGroupCamera()
{
	vector<Cam_Group> vcGroup;
	m_pBk->GetGroup(m_TreeType, vcGroup);

	int nIdx = 0, nCount = vcGroup.size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		m_pBk->DeleteGroupCameraByGroup(vcGroup[nIdx].cameragroupid);
	}

	TreeNode *pTreeNode = NULL;
	HTREEITEM hItem = m_CamGroup.GetRootItem();
	while(hItem)
	{
		pTreeNode = GetTreeNode(hItem);
		m_pBk->DeleteGroupCameraByGroup(pTreeNode->nGrpID);
		hItem = m_CamGroup.GetNextItem(hItem, TVGN_NEXT);
	}
}

void CDlgSettingCamGroup::DeleteGroup()
{
	vector<Cam_Group> vcGroup;
	m_pBk->DeleteGroupByTreeType(m_TreeType);

	m_pBk->GetGroup(AllTree, vcGroup, false);
	int nIdx = 0, nCount = vcGroup.size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		m_pBk->UpdateGroupId(nIdx+1, vcGroup[nIdx].cameragroupid);
	}
}

void CDlgSettingCamGroup::GetCameraIdFromTree(HTREEITEM& hItem, vector<int>& vcCamId)
{
	HTREEITEM hChildItem = NULL;
	TreeNode* pTreeNode = NULL;

	while(hItem)
	{
		hChildItem = m_CamGroup.GetChildItem(hItem);
		if (hChildItem)
		{
			GetCameraIdFromTree(hChildItem, vcCamId);
		}
		pTreeNode = GetTreeNode(hItem);
		if (pTreeNode->nodeType != TreeNode::NodeType::Grp_Node)
		{
			vcCamId.push_back(pTreeNode->nCamID);
		}
		hItem = m_CamGroup.GetNextItem(hItem, TVGN_NEXT);
	}
}

void CDlgSettingCamGroup::DeleteCamera(const vector<int>& vcOldCamId, const vector<int>& vcNewCamId)
{
	int nIdx = 0, nCount = vcOldCamId.size();
	vector<int>::const_iterator it;
	ec_Camera camObj;
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		it = find(vcNewCamId.begin(), vcNewCamId.end(), vcOldCamId[nIdx]);
		if (it == vcNewCamId.end())
		{
			m_pBk->GetCamera(vcOldCamId[nIdx],camObj);
			m_pBk->DeleteCamRecTime(camObj.camera_idx);
			m_pBk->DeleteCamRec(camObj.camera_idx);
			m_pBk->DeleteCamera(vcOldCamId[nIdx]);
		}
	}
}

void CDlgSettingCamGroup::SaveDeviceTreeDB()
{
	HTREEITEM hRootItem =  m_CamGroup.GetRootItem();
	vector<int> vcOldCamId, vcNewCamId;
	vcNewCamId.clear();
	vcOldCamId.clear();

	m_pBk->GetAllCameraId(vcOldCamId);
	GetCameraIdFromTree(hRootItem, vcNewCamId);
	DeleteCamera(vcOldCamId, vcNewCamId);

	DeleteGroupCamera();
	DeleteGroup();

	HTREEITEM hItem =  m_CamGroup.GetRootItem(), hChildItem = NULL;
	TreeNode* pTreeNode = NULL;
	CString strItemText;
	int nGroupId = 0;

	while(hItem)
	{
		pTreeNode = GetTreeNode(hItem);
		if (pTreeNode)
		{
			strItemText = m_CamGroup.GetItemText(hItem);
			nGroupId = InsertGroup(strItemText);
			InsertCamera(hItem, nGroupId);
		}
		hItem = m_CamGroup.GetNextItem(hItem, TVGN_NEXT);
	}
}

int CDlgSettingCamGroup::vSaveDB(void)
{
	if (m_TreeType == TreeType::LayoutTree)
	{
		SaveLayoutTreeDB();
	}
	else
	{
		SaveDeviceTreeDB();
	}
	ClearNonexistGroupCam();
	return 0;
}	

void CDlgSettingCamGroup::ClearNonexistGroupCam()
{
	vector<Cam_Group_Cam> vcGroupCam;
	m_pBk->GetAllGroupCamera(vcGroupCam);
	Cam_Group group;

	int nIdx = 0, nCount = vcGroupCam.size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		group.clear();
		m_pBk->GetGroup(vcGroupCam[nIdx].cameragroupid, group);
		if (group.isempty())
		{
			m_pBk->DeleteGroupCamera(vcGroupCam[nIdx].cameraid,vcGroupCam[nIdx].cameragroupid);
		}
	}
}

bool CDlgSettingCamGroup::IsLastGroup()
{
	bool bResult = false;
	int nGroupIdx = 1;
	HTREEITEM	hItem = NULL, hRootItem = NULL, hGroupItem = NULL;	

	hRootItem = m_CamGroup.GetRootItem();
	hGroupItem = m_CamGroup.GetNextSiblingItem(hRootItem);
	while(hGroupItem)
	{
		nGroupIdx++;
		hGroupItem = m_CamGroup.GetNextSiblingItem(hGroupItem);
	}

	if(nGroupIdx <= 1)
	{
		CString str;
		LOADSTRING(str, IDS_TREECTRL_NOT_GROUP);
		AfxMessageBox(str);
		bResult = true;
	}

	return bResult;
}

void CDlgSettingCamGroup::OnBnClickedCamgroupDel()
{
	HTREEITEM  hItem = m_CamGroup.GetSelectedItem();
	CString ls_desc, ls_confirm, ls_confirm1, ls_confirm2;
	TVITEM TreeItem;
	
	if (!IsLastGroup())
	{
		if (m_CamGroup.GetParentItem(hItem) == NULL) //Group Items
		{
			ls_desc = m_CamGroup.GetItemText(hItem);
			LOADSTRING(ls_confirm1, IDS_DELETE_GROUP1);
			LOADSTRING(ls_confirm2, IDS_DELETE_GROUP2);
			ls_confirm.Format(_T("%s %s %s"), ls_confirm1, ls_desc, ls_confirm2);

			//--check if it's ISAP
			if(ls_desc.Find(_T("ISAP")) != -1)
			{

			}
		}
		else //Camera Item
		{
			ls_desc = m_CamGroup.GetItemText(hItem);
			LOADSTRING(ls_confirm1, IDS_REMOVE_CAM);
			ls_confirm.Format(_T("%s %s?"), ls_confirm1, ls_desc);

			TreeNode* pTreeNode =  GetTreeNode(hItem);
			if (pTreeNode)
			{
				DeleteTreeNode(hItem, false);
			}
			else
			{
				AfxMessageBox(AFX_IDS_DLG_SETGROUP2);
				return;
			}
		}

		m_CamGroup.DeleteItem(hItem);
		m_bDirty = true;
	}
	// TODO: Add your control notification handler code here
}

void CDlgSettingCamGroup::OnBnClickedCamgroupAdd()
{
	// TODO: Add your control notification handler code here
	// Add Group here....
	vOpenExt(0);
}

void CDlgSettingCamGroup::OnBnClickedCamgroupEdit()
{
	if (m_curGroupId<=0) return;
	vOpenExt(m_curGroupId);
	// TODO: Add your control notification handler code here
}

//Open Group Edit/Add screen
HTREEITEM CDlgSettingCamGroup::GetGroupItem(HTREEITEM& hItem)
{
	HTREEITEM hItemParant = m_CamGroup.GetParentItem(hItem);
	if (hItemParant == NULL)
	{
		hItemParant = hItem;
	}
	else
	{
		HTREEITEM hPreviousItem = NULL;
		while(hItemParant)
		{
			hPreviousItem = hItemParant;
			hItemParant = m_CamGroup.GetParentItem(hItemParant);
		}
		hItemParant = hPreviousItem;
	}

	return hItemParant;
}

int CDlgSettingCamGroup::vOpenExt(int nGroupid)
{
	HTREEITEM  hItem = m_CamGroup.GetSelectedItem();
	hItem = GetGroupItem(hItem);

	CString csGroupName = m_CamGroup.GetItemText(hItem);
	CString ls_desc, ls_module;
	CDlgSettingCamGroup_Ext	dlg(nGroupid,csGroupName);
	if( dlg.DoModal() != IDOK )
	{
		return 0;
	}

	m_bDirty = true;
	if (nGroupid<=0) //Add New Group
	{
		mi_GroupCnt++;
		ls_desc = dlg.m_GroupName.Trim();
		int nLastGroupId = m_pBk->GetLastGroupIdx() + 1;
		InsertRootTreeNode(nLastGroupId, ls_desc);
		m_curGroupId = mi_GroupCnt;
		
	}
	else //Modify Existing Group 
	{
		ls_desc =  dlg.m_GroupName;
		HTREEITEM  hItem = m_CamGroup.GetSelectedItem();
		hItem = GetGroupItem(hItem);
		m_CamGroup.SetItemText( hItem, ls_desc);	//New desc into tree
	}

	return 0;
}

BOOL CDlgSettingCamGroup::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_ToolTip != NULL)
	   m_ToolTip->RelayEvent(pMsg);

	switch ( pMsg->message )
	{
		case WM_KEYDOWN:
		{
			switch( pMsg->wParam )
			{
				case VK_ESCAPE:
				if (m_bDirty )
				{
					int ll_ret = AfxMessageBox(IDS_SAVE_DATA, MB_YESNO|MB_ICONQUESTION);		
					if (ll_ret==IDYES) 	
					{				
						OnOK();
					}
					::DispatchMessage( pMsg );
				}
				break;
			} //switch
		} // Case WM_KEYDOWN:
	}
	return CBaseDialog::PreTranslateMessage(pMsg);
}

int CDlgSettingCamGroup::vInitImageList(void)
{
	//Initilize Images into the list...
	int nResult(0);
	m_TreeImageList.Create(16, 16, 0, 5, 5);
	//m_TreeImageList.SetBkColor(GLOBALTREEBKCOLOR);//m_TreeImageList.GetBkColor());

	//Folder Icon - Group First Create Image List
	nResult = m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_FOLDER));
	nResult = m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_CAM16));
 	
	m_CamGroup.SetImageList(&m_TreeImageList,TVSIL_NORMAL);
		
	return 0;
}


void CDlgSettingCamGroup::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (m_bDirty )
	{
		int ll_ret = AfxMessageBox(IDS_SAVE_DATA, MB_YESNO|MB_ICONQUESTION);			
		if (ll_ret==IDYES) 	
		{				
			OnOK();
		}
	}
	CBaseDialog::OnClose();
	//return 0;
}

void CDlgSettingCamGroup::OnBnClickedCamgroupISAP()
{
	//CDlgAddISAP m_pISAP;

	
	HTREEITEM  hItem = m_CamGroup.GetSelectedItem();
	if(!hItem)
		hItem = m_CamGroup.GetRootItem();

	int nGroupId = GetTreeNode(hItem)->nGrpID;
	CAddDevice dlg(nGroupId);
	if( dlg.DoModal() == IDOK && !dlg.isExistDevice())
	{
		m_bAddNewDevice = true;
		Cam_Group_Cam grpCam;
		m_pBk->GetLastGroupCamera(grpCam);
		nGroupId = grpCam.cameragroupid;

		vector<Cam_Group_Cam> vcGroupCam;
		int nCamId = m_pBk->GetLastCameraId();
		if (!grpCam.isempty())
		{
			if (grpCam.numb_ > 0)
			{
				int nDeviceId = m_pBk->GetLastDeviceIdx();
				m_pBk->GetGroupCamera(nGroupId, vcGroupCam);
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

		vBuildCamToGroup(nGroupId, vcGroupCam, hItem);
		return;
	}
}
void CDlgSettingCamGroup::OnDestroy()
{
	HTREEITEM hItem = m_CamGroup.GetRootItem();
	DeleteTreeNode(hItem);
	m_CamGroup.DeleteAllItems();
	m_pBk->UnregistWnd(this);
	CBaseDialog::OnDestroy();

	// TODO: Add your message handler code here
}

void CDlgSettingCamGroup::DeleteTreeNode(HTREEITEM& hItem, bool bSiblingNode)
{
	HTREEITEM hChildItem;
	TreeNode* pNode = NULL;
	while(hItem)
	{
		hChildItem = m_CamGroup.GetChildItem(hItem);
		if (hChildItem)
		{
			DeleteTreeNode(hChildItem);
		}

		pNode = GetTreeNode(hItem);
		delete pNode;
		pNode = NULL;
		
		if (bSiblingNode)
			hItem = m_CamGroup.GetNextItem(hItem, TVGN_NEXT);
		else
			break;
	}
}

CDlgSettingCamGroup::TreeNode* CDlgSettingCamGroup::GetTreeNode(HTREEITEM& hItem)
{
	TreeNode* pNodeitem = NULL;
	TVITEM lTreeItem = {0};
	lTreeItem.hItem = hItem;
	lTreeItem.mask = TVIF_HANDLE;
	m_CamGroup.GetItem(&lTreeItem);
	pNodeitem = (TreeNode*)lTreeItem.lParam;

	return pNodeitem;
}

void CDlgSettingCamGroup::AdjustDlgViewByTreeType()
{
	if (m_TreeType == TreeType::DeviceTree)
	{
		CRect recDlg;
 		GetWindowRect(recDlg);
  		recDlg.right = recDlg.right*0.7;
 		
  		MoveWindow(recDlg);
		GetWindowRect(recDlg);
		ScreenToClient(recDlg);
		MoveBitmapBtn(recDlg);
		MoveOKCancleBtn(recDlg);
		GetDlgItem(IDC_CAMERALIST)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_ST_CAMERALIST)->ShowWindow(SW_HIDE);
		CenterWindow();
	}
	else
	{
		GetDlgItem(IDC_CAMGROUP_ADD_ISAP)->ShowWindow(SW_HIDE);
	}
}

void CDlgSettingCamGroup::MoveBitmapBtn(const CRect& recDlg)
{
	CRect recBtn, rec, recTree, recHead;
	m_CamgroupAdd.GetWindowRect(recBtn);
	ScreenToClient(recBtn);

	GetDlgItem(IDC_ST_HEAD)->GetWindowRect(recHead);
	ScreenToClient(recHead);
	recBtn.bottom = recHead.bottom + 5 + recBtn.Height();
	recBtn.top = recHead.bottom + 5;

	recTree.left = recDlg.left + 15;
	recTree.top = recBtn.bottom + 5;
	recTree.right = recDlg.right - 15;
	recTree.bottom = recDlg.bottom - 50;
	GetDlgItem(IDC_CAMGROUPTREE)->MoveWindow(recTree);
	
	rec = recBtn;
	rec.right = recTree.right;
	rec.left = rec.right - recBtn.Width();
	m_CamgroupISAP.MoveWindow(rec);

	rec.right = rec.left-5;
	rec.left = rec.right - recBtn.Width();
	m_CamgroupEdit.MoveWindow(rec);
	
	rec.right = rec.left-5;
	rec.left = rec.right - recBtn.Width();
	m_CamgroupDel.MoveWindow(rec);

	rec.right = rec.left-5;
	rec.left = rec.right - recBtn.Width();
	m_CamgroupAdd.MoveWindow(rec);

}

void CDlgSettingCamGroup::MoveOKCancleBtn(const CRect& recDlg)
{
	CRect recOK, recCancle;
	int nRemainspace = 0;

	GetDlgItem(IDOK)->GetWindowRect(recOK);
	ScreenToClient(recOK);

	GetDlgItem(IDCANCEL)->GetWindowRect(recCancle);
	ScreenToClient(recCancle);

	nRemainspace = recDlg.Width() - recOK.Width() - recCancle.Width() - 3;
	recOK.left = recDlg.left + (nRemainspace/2); 
	recOK.right = recOK.left + recCancle.Width();

	recCancle.right = recDlg.right - (nRemainspace/2); 
	recCancle.left = recCancle.right - recOK.Width();

	recOK.bottom = recDlg.bottom - 10; 
	recOK.top = recOK.bottom - recCancle.Height();

	recCancle.top = recOK.top; 
	recCancle.bottom = recOK.bottom;

	GetDlgItem(IDOK)->MoveWindow(recOK);
	GetDlgItem(IDCANCEL)->MoveWindow(recCancle);
}

bool CDlgSettingCamGroup::IsDragAvalible(HTREEITEM& hItemDrag)
{
	bool bResult = true;
	
	TreeNode *pTreeNode = GetTreeNode(hItemDrag);
	if (pTreeNode)
	{
		if (pTreeNode->nodeType == TreeNode::NodeType::Grp_Node)
		{
			bResult = false;
		}
		else
		{
			HTREEITEM hParantItem = m_CamGroup.GetParentItem(hItemDrag);
			pTreeNode = GetTreeNode(hParantItem);
			if (pTreeNode->nodeType  == TreeNode::NodeType::Dvs_Node)
			{
				bResult = false;
			}
		}
	}

	return bResult;
}

bool CDlgSettingCamGroup::IsDropAvalible(HTREEITEM& hItemDrop)
{
	bool bResult = true;

	TreeNode *pTreeNode = GetTreeNode(hItemDrop);
	if (pTreeNode)
	{
		if (pTreeNode->nodeType == TreeNode::NodeType::Cmr_Node ||
			pTreeNode->nodeType == TreeNode::NodeType::Dvs_Node)
		{
			bResult = false;
		}
	}

	return bResult;
}
void CDlgSettingCamGroup::OnSize(UINT nType, int cx, int cy)
{
	CBaseDialog::OnSize(nType, cx, cy);
	if (cx && cy)
	{
		//AdjustDlgViewByTreeType();
	}
	
	// TODO: Add your message handler code here
}

void CDlgSettingCamGroup::DeleteStation(int nCameraId)
{
	HTREEITEM hItem = m_CamGroup.GetRootItem();
	TreeNode* pTreeNode = NULL;
	bool breakFlag = false;
	while(hItem)
	{
		HTREEITEM hItemChild = m_CamGroup.GetChildItem(hItem);
		while(hItemChild)
		{
			pTreeNode = GetTreeNode(hItemChild);
			if (pTreeNode->nCamID == nCameraId) 
			{
				m_CamGroup.DeleteItem(hItemChild);
				breakFlag = true;
				break;
			}

			if (pTreeNode->nodeType == TreeNode::NodeType::Dvs_Node)
			{
				HTREEITEM hItemChannel = m_CamGroup.GetChildItem(hItemChild);
				while(hItemChannel)
				{
					pTreeNode = GetTreeNode(hItemChannel);
					if (pTreeNode->nCamID == nCameraId) 
					{
						m_CamGroup.DeleteItem(hItemChannel);
						breakFlag = true;
						break;
					}

					hItemChannel = m_CamGroup.GetNextItem(hItemChannel, TVGN_NEXT);
				}
			}

			if (breakFlag)
				break;
			hItemChild = m_CamGroup.GetNextItem(hItemChild, TVGN_NEXT);
		}
		if (breakFlag)
			break;
		hItem = m_CamGroup.GetNextItem(hItem, TVGN_NEXT);
	}
}

LRESULT CDlgSettingCamGroup::OnUpdateView(WPARAM wParam,LPARAM lParam)
{
	switch (wParam)
	{
	case UM_DELETE_STATION:
		{
			int nCameraId = (int)lParam;
			DeleteStation(nCameraId);
		}
		break;
	default:
		break;
	}

	return 0;
}

void CDlgSettingCamGroup::DeleteTableData()
{
	vector<ec_Camera> vcCams;
	m_pBk->DeleteGroupByTreeType(m_TreeType);

	DeleteGroupCamera();

	m_pBk->GetAllCameras(vcCams);
	int nCount = vcCams.size();
	for (int nIdx = 0; nIdx< nCount; nIdx++)
	{
		m_pBk->DeleteCamera(vcCams[nIdx].cameraid);
	}
}

void CDlgSettingCamGroup::RestoreOldTableData()
{
	m_pBk->InsertGroups(m_oldCamGroups);
	m_pBk->InsertCameras(m_oldCams);
	Cam_Group_Cam groupCam;
	ec_Camera camObj;

	int nIdx = 0, nCount = m_oldCamGroupCams.size();
	for (nIdx = 0; nIdx< nCount; nIdx++)
	{
		groupCam = m_oldCamGroupCams[nIdx];
		m_pBk->GetCamera(groupCam.camera_idx, camObj, true);
		groupCam.cameraid = camObj.cameraid;
		m_pBk->InsertGroupCamera(groupCam);
	}
}

void CDlgSettingCamGroup::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here.
	if (m_bAddNewDevice)
	{
		DeleteTableData();
		RestoreOldTableData();
		ClearNonexistRecCam();
		m_pBk->SendMessage(MsgUpdateView, UM_UPDATE_CAMERA_ID, NULL);
	}
	OnCancel();
}

void CDlgSettingCamGroup::ClearNonexistRecCam()
{
	vector<ec_Camera_Rec> vcRecs;
	vector<int> vcCamIdx;
	vector<int>::iterator it;

	m_pBk->GetAllCamRecs(vcRecs);
	m_pBk->GetAllCameraId(vcCamIdx,true);

	int nIdx = 0, nCount = vcRecs.size();
	for (nIdx = 0; nIdx< nCount; nIdx++)
	{
		if (find(vcCamIdx.begin(),vcCamIdx.end(),vcRecs[nIdx].camera_idx) == vcCamIdx.end())
		{
			m_pBk->DeleteCamRec(vcRecs[nIdx].camera_idx);
		}
	}
}