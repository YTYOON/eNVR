#pragma once
#include "resource.h"
#include "BaseDialog.h"
#include "afxcmn.h"
#include "afxdb.h"
#include "INI.h"
#include "ColorBox.h"
#include "afxwin.h"
#include "afxext.h"
#include "DlgSettingCamGroup_Ext.h"
#include "DlgAddISAP.h"

#define		MAXGROUP 20
#define		MAXCAMERA 100
#define		CAMERA_EXIST 999

// CDlgSettingCamGroup dialog

class CDlgSettingCamGroup : public CBaseDialog
{
	DECLARE_DYNAMIC(CDlgSettingCamGroup)

public:
	CDlgSettingCamGroup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSettingCamGroup();

// Dialog Data
	enum { IDD = IDD_SETTING_CAMGROUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	struct TreeNode
	{
		enum NodeType{Non_Node = 0, Grp_Node, Dvs_Node, Cmr_Node };
		NodeType nodeType;
		int nCamID;
		int nGrpID;
		int nCamIndex;

		TreeNode()
			:nodeType(Non_Node),nCamID(0),nGrpID(0),nCamIndex(0){}

		void clear()
		{
			nodeType = Non_Node;
			nCamID = 0;
			nGrpID = 0;
			nCamIndex = 0;
		}
	};


public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTvnSelchangedCamgrouptree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBegindragCameralist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBegindragCameratree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCamgroupDel();
	afx_msg void OnBnClickedCamgroupAdd();
	afx_msg void OnBnClickedCamgroupEdit();
	afx_msg void OnBnClickedCamgroupISAP();
	afx_msg void OnClose();
	afx_msg void OnDestroy();

	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
private:
	HCURSOR m_hArrowCursor;
	HCURSOR m_hNoCursor;

	CTreeCtrl m_CamGroup;
	HTREEITEM itemSpec[MAX_GROUP];
	CString m_Module[5];
	int m_nDragIndex;
	CImageList*		m_pDragImage;
	CListCtrl*		m_pDragList;
	CTreeCtrl*		m_pDropTree;	
	HTREEITEM		m_DragItem;	
	CWnd*	m_pDropWnd;
	LVITEM* pItem; 
	CListCtrl* m_CamList;

	int m_nDropGroupIndex;
	int m_iGroupId, m_iCameraId;
	
	bool	m_bDragging;
	bool	m_bDirty;
	
	vector<int> vectorDlgGroupID;
	CBitmapButton m_CamgroupAdd;
	CBitmapButton m_CamgroupDel;
	CBitmapButton m_CamgroupEdit;
	CBitmapButton m_CamgroupISAP;
	int i_cameragroupid;
	CImageList m_ImgList;
	
	CEdit m_GroupName;
	CImageList m_TreeImageList;
	CFont	mFont;
	int m_curGroupId;
	int mi_GroupCnt;
	int m_curCamIdx;
	int mi_CameraCnt;
	CArray <int,int> ma_CamIdx;
	CString m_EntryMode;
	CToolTipCtrl* m_ToolTip;
	int ia_group[MAX_GROUP];

	//////////////////////////////////////////////////////////////////////////Lynn
	CNVRBk *m_pBk;
	TreeType m_TreeType;
	vector<Cam_Group> m_oldCamGroups;
	vector<Cam_Group_Cam> m_oldCamGroupCams;
	vector<ec_Camera> m_oldCams;
	vector<ec_Camera_Rec> m_oldRecs;
	bool m_bAddNewDevice;

	int vBuildGroupTree(void);
	int vBuildCamToGroup(int nGroupid, const vector<Cam_Group_Cam>& vcGrpCam, HTREEITEM& hParent);
	int vPopulateCamera(void);
	int vDropItemOnTree(CListCtrl* pDragList, CTreeCtrl* pDropTree);
	int vDropItemOnTree(HTREEITEM& DragItem, CTreeCtrl* pDropTree);
	bool vCheckExist(int o_camidx, HTREEITEM hpitem);
	int vSaveDB(void);
	int vOpenExt(int nGroupid);
	int vInitImageList(void);
	bool IsChannel(const ec_Camera& cameraObj);
	bool IsDevice(const ec_Camera& cameraObj);
	void ReassignCamGrpCam(const vector<Cam_Group_Cam>& vcGroupCam, vector< vector<Cam_Group_Cam> >& vcGroupCamByDevice);
	bool IsNeed2InsertDeviceNode(const vector<Cam_Group_Cam>& vcGroupCam, int& nDeviceNodeIdx);
	CDlgSettingCamGroup::TreeNode* GetTreeNode(HTREEITEM& hItem);
	HTREEITEM InsertChildTreeNode(int nGroupid, TreeNode::NodeType nodeType, const ec_Camera& camObj, const CString& nodeName, HTREEITEM& hParent);
	void DeleteTreeNode(HTREEITEM& hItem, bool bSiblingNode = true);
	void InsertListCtrl(int nIdx, const ec_Camera& cameraObj);
	int InsertGroup(const CString& str);
	void InsertCamera(HTREEITEM& hParItem, int nGroupId);
	void InsertStation(HTREEITEM& hParItem, int nGroupId);
	HTREEITEM InsertRootTreeNode(int nGrpId, const CString& strGrpName);
	void SaveLayoutTreeDB();
	void SaveDeviceTreeDB();
	void DeleteGroupCamera();
	void DeleteGroup();
	void GetCameraIdFromTree(HTREEITEM& hItem, vector<int>& vcCamId);
	void DeleteCamera(const vector<int>& vcOldCamId, const vector<int>& vcNewCamId);
	void AdjustDlgViewByTreeType();
	bool IsDragAvalible(HTREEITEM& hItemDrag);
	bool IsDropAvalible(HTREEITEM& hItemDrag);
	void MoveOKCancleBtn(const CRect& recDlg);
	void MoveBitmapBtn(const CRect& recDlg);
	LRESULT OnUpdateView(WPARAM,LPARAM);
	void DeleteStation(int nCameraId);
	bool IsLastGroup();
	HTREEITEM GetGroupItem(HTREEITEM& hItem);
	HTREEITEM GetInsertAfterItem(HTREEITEM& hParent, TreeNode *pSortTreeNode);
	void KeepOldCamera();
	void DeleteTableData();
	void RestoreOldTableData();
	void ClearNonexistGroupCam();
	void ClearNonexistRecCam();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedCancel();
};
