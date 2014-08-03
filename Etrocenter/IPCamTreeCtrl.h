#include "afxdb.h"
#include "EventReceive.h"
#include "DlgFlyAway.h"
#ifdef PLUGIN
#include "DlgANPR.h"
#endif

#if !defined(AFX_IPCAMTREECTRL_H__E8E1FECD_4CCA_45C4_9E60_2C4156C9DD39__INCLUDED_)
#define AFX_IPCAMTREECTRL_H__E8E1FECD_4CCA_45C4_9E60_2C4156C9DD39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IPCamTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIPCamTreeCtrl window
//#include "VividTree.h"

class CIPCamTreeCtrl : public CTreeCtrl
{
	friend class CIPCamTreeView;
	//enum {UNASSIGNED=0, GROUP01 };
	enum {MAINGROUP=0, GROUP01 };

public:
	struct TreeThreadParam {
		CIPCamTreeCtrl* pThis;
		NODEITEM* pNode;
	};
	struct ENHANCE_FUNC_PARAM
	{
		UINT nFunID;
		NODEITEM* pNode;
	};
	struct struTreeGroup {
		int		 GroupId;
		CString GroupName;
	};

	CIPCamTreeCtrl();

// Attributes
public:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPCamTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void vResetData();
#ifdef FACTORY
	void SetGroupSelectedItem(int nGroup);
#endif

	void vUpdateTreeItem(NODEITEM* pNode);
	NODEITEM* GetCurrentNodeItem();
	void vSyncSelectedNode(NODEITEM* pNode);
	void vLiveView();
	void vAttach(NODEITEM* pNode);
	bool vCheckTreeSelect();

	virtual ~CIPCamTreeCtrl();
protected:
//	CWnd* pWndParent;
	CImageList m_TreeImageList;
	CImageList m_StateImage;
	HANDLE hDlgInitialize/*, hCloseDlg*/, hAllThreadClosed;
	
#define  MAX_CGI_FUNCTION  100
	CGISECTIONINFO cgiinfo[MAX_CGI_FUNCTION];

	// Generated message map functions
protected:
	int vCheckCamStatus(NODEITEM* pNode);
	static UINT fnPollingIPCamera(LPVOID lpParam);
	bool bReEntry;
	void SetDefaultCursor();
//*********** CURSORS  *********************
	HCURSOR cursor_hand ;
	HCURSOR cursor_arr	;
	HCURSOR cursor_no	;
//******************************************

	CImageList*	m_pDragImage;
	BOOL		m_bLDragging;
	HTREEITEM	m_hitemDrag,m_hitemDrop;
	int i_counter;

//************ LAYOUT ********************************
	int			m_LayoutIdx;	//Layout Index for the layout type	

//*****************************************************

	//void vSynchronizeAuthInfo(NODEITEM* pNodeitem);
	HTREEITEM itemSpec[MAX_GROUP];
	
	
	//CString m_csGroup[MAX_GROUP];
	struTreeGroup m_csGroup[MAX_GROUP];

	BYTE m_nGroup;
	void InsertDefaultRoot();
	int  vGrowUpIPCamInfoTree(NODEITEM *pNode, HTREEITEM hParent = TVGN_ROOT);
	void vInitCgiInfo();
	//{{AFX_MSG(CIPCamTreeCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnRefresh();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSetAuthorizationInfo();
	afx_msg void OnTreectrlNewGroup();
	afx_msg void OnTreectrlModifyGroup();
	afx_msg void OnEventSetting();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnIeExplorer();
	//afx_msg void OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTreectrlRemoveGroup();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLiveView();
	//afx_msg void OnRecordingOn();
	afx_msg void OnPaint();
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnFirsttimeInitialize();
	afx_msg void OnAddInternetIpCam();
	afx_msg void OnUpgradeFirmware();
	afx_msg void vEnhanceFunc(UINT nID);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	void vInsertGroup(CString& csName);
	void vModifyGroup(CString& csName);
	void vModifyGroupDB(CString& csName, int o_GroupId);
	void vInsertGroupDB(CString& csName);
	
	void vInitImages();
	//static UINT _stdcall CreateTreeThread(LPVOID lpParam);
	HTREEITEM MoveChildItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter);
	//LRESULT OnSetCheck(WPARAM wParam, LPARAM lParam);
	LRESULT CIPCamTreeCtrl::OnDragList(WPARAM wParam, LPARAM lParam);

	HTREEITEM iCurGroupItem;
	HTREEITEM iCurTreeItem;
	
	//public:
	//	int CIPCamTreeCtrl::vCheckCamStatus(LPTVINSERTSTRUCT lpInsertStruct);
	int vGetVideoNumber(int o_LayoutNo);
public:
	afx_msg void OnAddNewIpcam();
protected:

public:
	bool ibDiffGroup;
	afx_msg void OnMenuRemoveipcam32815();
	afx_msg void OnProductTest();
	int vAddMoreCamera(void);
	int m_maxGroupId;
	//Store Previous Selected Group Name
	CString isPrvGroupName;
	vector <CRect> m_rectTreeCtrl;
	
protected:
	void InsertDBRoot();
public:
	CDatabase* m_pDB;
	void InsertDBCamera(int o_groupid, HTREEITEM hParent);
	// Array of Node Item
	NODEITEM gNodeItem[20];
	int vInsertNodeItem(int o_index, NODEITEM* pNodeItem);
	NODEITEM vGetNodeItem(int oCamIndex);
protected:
	bool vDeleteGroup(int o_GroupId);
public:
//	bool vRefreshTree(void);
	//bool vDBConnect(void);
	//static void vTreeCrlCallBack(CEventReceive *m_CEventReceive, void *lpParam);
	//CCriticalSection m_CCriticalSection;
	bool vSelectCam(NODEITEM* pNode);
	void vEventLiveview(HTREEITEM pItem, int oi_AlertLevel);
	NODEITEM* vGetNodeitemFromIP(CString o_IP);
	afx_msg void OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	bool m_bRefreshVideo;
	bool m_bOnlyOneCamChoosen;
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	bool m_bPtInRect;
	void vGetGroupNodeItems(vector <NODEITEM*>& vtNodes);
	void vGetAllNodeItems(vector <NODEITEM*>& vtNodes);
	vector <NODEITEM*> m_vctpAllNodesInTree;
	int nNextAvailableLayOut(int nCurrentLayOut);
	bool vChangeCamGroup(int o_OldGroupid, int o_NewGroupid, int o_CamId);
	afx_msg void OnMenuFlyaway();
	void vFlyaway(int nIdx);
	bool m_bFlyAway;
	int m_nDropRectIdx; // the index of rect where treeitem drop to; -1 means not in rect;
	map <CString,bool> m_mapGroupFlyAway;
	void vSetGroupFlyAway(CString csGroupName);
	bool bGetGroupFlyAway(CString csGroupName);
	CDlgFlyAway* m_pDlgFlyAway;
	vector<CDlgFlyAway*> m_vctDlgFlyAwayRecycleBin;
	vector<int> vectorDoubleCameraID, vectorGroupID;
	void vCheckLVExist();
	void vGetGroupCam(vector<NODEITEM*>& vctNode);
	void vGetGroupCam(int nGroupId, vector<NODEITEM*>& vctNode);
	bool bCheckGroupCamNum(int nGroupId);
	int nGetCurrentSelectedGroupId(void);
	vector<CString> m_vctcsGroupNames;

	//--ANPR
	afx_msg void vANPR();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPCAMTREECTRL_H__E8E1FECD_4CCA_45C4_9E60_2C4156C9DD39__INCLUDED_)
