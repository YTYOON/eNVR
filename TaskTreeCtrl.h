#pragma once
#include "IPCamDiscovery.h"
#include "TreeButton.h"
#include "ColorBox.h"
#include "BaseBitmapButton.h"
#include "DlgAppTag.h"
#include "DlgSettingTag.h"
#include "DlgSettingSystem.h"
#include "BtnST.h"
#include "DlgMatrixTab.h"

// CTaskTreeCtrl
class CTaskTreeCtrl : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTaskTreeCtrl)
	enum {MAINGROUP=0, GROUP01 };

	struct TreeThreadParam {
		CTaskTreeCtrl* pThis;
		NODEITEM* pNode;
	};
	struct ENHANCE_FUNC_PARAM
	{
		UINT nFunID;
		NODEITEM* pNode;
	};

	struct TreeNode
	{
		enum TreeNdoeType{ Non_Node = 0, Grp_Node, Dvs_Node, Cam_Node};
		NODEITEM* pNode;
		TreeNdoeType nodeType;
		int grpId;

		TreeNode()
			:pNode(NULL),nodeType(Non_Node),grpId(0){}

		void clear()
		{
			pNode = NULL;
			nodeType = Non_Node;
			grpId = 0;
		}

	};

public:
	CTaskTreeCtrl();
	virtual ~CTaskTreeCtrl();
	vector<CString> m_vctcsGroupNames;
	vector <NODEITEM*> m_vctpAllNodesInTree;

protected:
	DECLARE_MESSAGE_MAP()
	vector<int> vectorDoubleCameraID, vectorGroupID;
	CIPCamDiscovery* pDoc;
	bool lb_open;
	int		m_GroupIdx;  //Selected Group Index
	CImageList*	m_pDragImage;
	BOOL		m_bLDragging,m_bDrag_cursor_no;
	HTREEITEM	m_hitemDrag,m_hitemDrop;
	int i_counter;
	bool m_bPtInRect;
	bool bReEntry;
	CString isPrvGroupName;
	bool ibDiffGroup;
	bool m_bRefreshVideo;
	bool m_bOnlyOneCamChoosen;
	int m_nDropRectIdx; // the index of rect where treeitem drop to; -1 means not in rect;
	bool m_bFlyAway;
	CImageList m_TreeImageList;
	CImageList m_StateImage;
	HANDLE hDlgInitialize, hAllThreadClosed;
	HCURSOR cursor_hand ;
	HCURSOR cursor_arr	;
	HCURSOR cursor_no	;
	BYTE m_nGroup;
	int m_maxGroupId;
	vector <CRect> m_rectTreeCtrl;
	HTREEITEM itemSpec[MAX_GROUP];
	map <CString,bool> m_mapGroupFlyAway;
	CNVRBk* m_pbkTalk;
    
public:
	void vCheckLVExist();
	void vInitImages();
	void SetDefaultCursor();
	void vLiveView();
	NODEITEM* GetCurrentNodeItem();
	void vModifyGroupDB(CString& csName, int o_GroupId);
	void vInsertGroupDB(CString& csName);
	void vSetGroupFlyAway(CString csGroupName);
	bool bGetGroupFlyAway(CString csGroupName);
	void vFlyaway(int nIdx);
	void vUpdateTreeItem(NODEITEM* pNode);
	void vGetGroupNodeItems(vector <NODEITEM*>& vtNodes);
	NODEITEM* vGetNodeitemFromIP(CString o_IP);
	void vGetAllNodeItems(vector <NODEITEM*>& vtNodes);
	void vGetGroupCam(vector<NODEITEM*>& vctNode);
	void vGetGroupCam(int nGroupId, vector<NODEITEM*>& vctNode);
	int nGetCurrentSelectedGroupId(void);
	bool bCheckGroupCamNum(int nGroupId);
	int	 GetSelectGrpID(HTREEITEM& hItem);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
    afx_msg void OnProductTest();
    afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult); 
	afx_msg void OnModifyDevice();
	afx_msg void OnAddInternetIpCam();
    afx_msg void OnIeExplorer();
	afx_msg void OnTreectrlNewGroup();
	afx_msg void OnTreectrlModifyGroup();
    afx_msg void OnTreectrlRemoveGroup();
	afx_msg void OnMenuRemoveipcam();
	afx_msg void OnDestroy();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	LRESULT OnUpdateView(WPARAM,LPARAM);

protected:
	virtual HTREEITEM InsertDeviceNode(const ec_Camera& nodeObj, HTREEITEM& hTreeItm, NODEITEM& camNode) = 0;
	virtual void CreateTreeGroup() = 0;
	virtual void InsertCameraNode(NODEITEM& camNode, HTREEITEM& hTreeItm, bool isChannel = false) = 0;
	virtual bool IsDragItemValid(HTREEITEM& hItem) = 0;
	virtual void DeleteChildNode(HTREEITEM& hItem, bool bOnlyNodeItm = false) = 0;
	virtual void DeleteTreeNodeByCamId(vector<int> *pvcCamId = NULL) = 0;
	virtual void PushDeleteCamId(int nCamId) = 0;
	virtual void Clear_KeepDelCamId() = 0;
	virtual void GetDeleteCamIds(vector<int>& vcCamId) = 0;
	virtual void RefreshLauoutTree() = 0;
	virtual CMenu* CreateCameraNodeMenu(CMenu& menu, HTREEITEM hItem) = 0;
	virtual void AddDevice(int nGroupId) = 0;

	void AddNewIPCam();
	bool vDeleteGroup(HTREEITEM& hItem);
	bool vChangeCamGroup(HTREEITEM& itemDrag, HTREEITEM& itemDrop);
	void FilterCameraGroup(vector<Cam_Group>& vcGroup);
	bool IsNeed2InsertNode(const NODEITEM* m_camInfo);
	void ExpandTree();
	void CreateTreeNode(TreeType byTreeType);
	void ClearTree();
	HTREEITEM InsertRootNodes(const vector<Cam_Group>& vcGroup);
	HTREEITEM InsertRootNode(const CString& strGrpName, int nGrpId);
	void UpdateValue(const Cam_Group& GroupItm, int nIdx);
	void CreateChildNode(const vector<Cam_Group>& vcGroup, byte byTreeType);
	void InsertChildNode(const vector<Cam_Group_Cam>& vcGroupCam, HTREEITEM& hTreeItm);
	bool IsNeed2InsertDeviceNode(const map<int, NODEITEM>& mapNode, const vector<Cam_Group_Cam>& vcGroupCam, int& nDeviceNodeIdx);
	void Trans_Camera2NODEITEM(const ec_Camera& camObj, NODEITEM& node, int nStreamID);
	//PROTOCOL GetStreamProtocol(const ec_Camera& camObj, int nStreamId);
	//NVR_BRAND GetNVRBrand(const NODEITEM& node);
	//WORD GetStreamID(const ec_Camera& camObj);
	WORD GetStreamPort(const ec_Camera& camObj, int nStreamId);
	void Trans_CamGrpCam2NODEITEM(const vector<Cam_Group_Cam>& vcGroupCam, map<int, NODEITEM>& mapNode);
	void ReassignCamGrpCam(const vector<Cam_Group_Cam>& vcGroupCam, vector< vector<Cam_Group_Cam> >& vcGroupCamByDevice);
	void GetNodeItmFromTreeNode(HTREEITEM& hItem, vector<NODEITEM*>& vtNode, bool bFromGrpNode = false);
	CTaskTreeCtrl::TreeNode* GetRootItemFromTreeItem(HTREEITEM& hPrtTreeItm);
	void DeleteChildNodes(HTREEITEM& hItem);
	bool CheckRemoveGroup();
	void vDispatchvideo_format2NodeItem(const CString& str, NODEITEM* pNode);
	CTaskTreeCtrl::TreeNode* GetTreeNode(HTREEITEM& hItem);
	void DeleteTreeNode(HTREEITEM& hItem);
	bool GetDragCursor(TreeNode *pTreeNode);
	void DragTreeNode2Video(const CPoint& point);
	void DropTreeNode();
	bool IsDropItemValid(HTREEITEM& hItem);
	HTREEITEM InsertNode(HTREEITEM& hParent, TreeNode *pTreeNode, const CString& strNodeName, UINT mask);
	bool vRefreshTree(void);
	void ExpandItem(HTREEITEM  hItem);
	void SetVideoFocus();
	void MoveTreeNode(HTREEITEM& itemDelete, HTREEITEM& itemInsertGroup);
	void ChangeDeviceGroup(HTREEITEM& itemDrag, HTREEITEM& itemDrop);
	void ReleaseVideo(HTREEITEM& hItem);
	void RemoveIPCam(HTREEITEM& hItem);
	void DeleteStation(int nCameraId);
	void DisplayGroupLiveView();
	void UpdateCameraId();
	void UpdateDevice(int nCamId);
	HTREEITEM GetGroupItem(HTREEITEM& hItem);
	HTREEITEM GetInsertAfterItem(HTREEITEM& hParent, TreeNode *pSortTreeNode);
	NODEITEM* GetTreeNodeFromCamId(int nCamId);

	//static UINT _stdcall RefreshThread(LPVOID lpParam);
public:
	virtual TreeType GetTreeType() = 0;
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};


class CTaskDeviceTree : public CTaskTreeCtrl
{
	DECLARE_DYNAMIC(CTaskDeviceTree)

private:
	vector<int> m_vcKeepDelCamId;
protected:
	DECLARE_MESSAGE_MAP()
	virtual HTREEITEM InsertDeviceNode(const ec_Camera& nodeObj, HTREEITEM& hTreeItm, NODEITEM& camNode);
	virtual void CreateTreeGroup();
	virtual void InsertCameraNode(NODEITEM& camNode, HTREEITEM& hTreeItm, bool isChannel = false);
	virtual bool IsDragItemValid(HTREEITEM& hItem);
	virtual void DeleteChildNode(HTREEITEM& hItem, bool bOnlyNodeItm = false);
	virtual void DeleteTreeNodeByCamId(vector<int> *pvcCamId = NULL){};
	virtual void PushDeleteCamId(int nCamId){m_vcKeepDelCamId.push_back(nCamId);};
	virtual void Clear_KeepDelCamId(){m_vcKeepDelCamId.clear();};
	virtual void GetDeleteCamIds(vector<int>& vcCamId){vcCamId = m_vcKeepDelCamId;};
	virtual void RefreshLauoutTree();
	virtual CMenu* CreateCameraNodeMenu(CMenu& menu, HTREEITEM hItem);
	virtual void AddDevice(int nGroupId);

public:
	virtual TreeType GetTreeType(){ return DeviceTree; };
	afx_msg void OnAddNewIpcam();
};

class CTaskLayoutTree : public CTaskTreeCtrl
{
	DECLARE_DYNAMIC(CTaskLayoutTree)

protected:
	DECLARE_MESSAGE_MAP()
	virtual HTREEITEM InsertDeviceNode(const ec_Camera& nodeObj, HTREEITEM& hTreeItm, NODEITEM& camNode);
	virtual void CreateTreeGroup();
	virtual void InsertCameraNode(NODEITEM& camNode, HTREEITEM& hTreeItm, bool isChannel = false);
	virtual bool IsDragItemValid(HTREEITEM& hItem);
	virtual void DeleteChildNode(HTREEITEM& hItem, bool bOnlyNodeItm = false);
	virtual void DeleteTreeNodeByCamId(vector<int> *pvcCamId = NULL);
	virtual void PushDeleteCamId(int nCamId){};
	virtual void Clear_KeepDelCamId(){};
	virtual void GetDeleteCamIds(vector<int>& vcCamId){};
	virtual void RefreshLauoutTree(){}
	virtual CMenu* CreateCameraNodeMenu(CMenu& menu, HTREEITEM hItem);
	virtual void AddDevice(int nGroupId){};

public:
	virtual TreeType GetTreeType(){ return LayoutTree; };
	afx_msg void OnRangeCmds(UINT nID);
	afx_msg void OnAddNewIpcam();	
};