// DlgAutoScanGroupSetting.cpp : implementation file
//

#include "stdafx.h"
#include "DlgAutoScanGroupSetting.h"
#include "MainFrm.h"

// CDlgAutoScanGroupSetting dialog

IMPLEMENT_DYNAMIC(CDlgAutoScanGroupSetting, CDialog)

CDlgAutoScanGroupSetting::CDlgAutoScanGroupSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAutoScanGroupSetting::IDD, pParent)
{

}

CDlgAutoScanGroupSetting::~CDlgAutoScanGroupSetting()
{
}

void CDlgAutoScanGroupSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREEGROUP, m_TreeCtrlGroup);
	DDX_Control(pDX, IDC_CHECK_GROUP_AUTOSCAN_ENABLE, m_CheckBox_AutoscanEnable);
}


BEGIN_MESSAGE_MAP(CDlgAutoScanGroupSetting, CDialog)
	
END_MESSAGE_MAP()
// CDlgAutoScanGroupSetting message handlers

BOOL CDlgAutoScanGroupSetting::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_TreeCtrlGroup.ModifyStyle( TVS_CHECKBOXES, 0 );
	m_TreeCtrlGroup.ModifyStyle( 0, TVS_CHECKBOXES );
	vBuildGroupTree();
	vCheckSelectedGroup();
	m_CheckBox_AutoscanEnable.SetCheck(((CMainFrame*)AfxGetMainWnd())->m_bGroupAutoPlay);
	CString csMsg;
	LOADSTRING(csMsg,IDS_GROUP_AUTOSCAN_ENABLE);
	m_CheckBox_AutoscanEnable.SetWindowTextW(csMsg);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAutoScanGroupSetting::vBuildGroupTree(void)
{
	CRecordset m_rs(&((CMainFrame*)AfxGetMainWnd())->m_DB);
	CDBVariant varValue;
	int nGroupId;
	CString csGroupName;
	m_rs.Open(CRecordset::dynaset,_T("SELECT cameragroupid, camera_group_desc FROM ec_camera_group order by cameragroupid"));
	m_rs.MoveLast();
	m_rs.MoveFirst();

	while (!m_rs.IsEOF()) 
	{ 					
		m_rs.GetFieldValue(_T("cameragroupid"), varValue, SQL_C_SSHORT);
		nGroupId = varValue.m_iVal;
		m_rs.GetFieldValue(_T("camera_group_desc"), csGroupName);
		TVINSERTSTRUCTW itemStru;
		itemStru.hParent = TVGN_ROOT;
		//Store the tree item in lparm, will used to get info from selectchanged
		itemStru.item.lParam = nGroupId;
		itemStru.item.mask = TVIF_PARAM | TVIF_TEXT |  TVIS_BOLD;	 
		itemStru.item.pszText = (LPWSTR)(LPCTSTR)csGroupName;
		itemStru.item.cchTextMax =csGroupName.GetLength();
			
		if(((CMainFrame*)AfxGetMainWnd())->bCheckGroupCamNum(nGroupId))
		{	
			HTREEITEM hInsert = m_TreeCtrlGroup.InsertItem(&itemStru);	
			m_TreeCtrlGroup.SelectItem(hInsert);
			m_vcthTree.push_back(hInsert);
		}

		m_rs.MoveNext();  		
	} 
	
}

void CDlgAutoScanGroupSetting::OnOK()
{
	vSetSelectedGroupId();
	CDialog::OnOK();
}

void CDlgAutoScanGroupSetting::vCheckSelectedGroup(void)
{
	if(((CMainFrame*)AfxGetMainWnd())->m_vctnGroupAutoPlayIdx.size()==0)
	{
		int nSelectedGroupId = ((CMainFrame*)AfxGetMainWnd())->nGetCurrentSelectedGroupId();

		for(int i =0;i<m_vcthTree.size();i++)
		{
			TVITEM TreeItem;
			TreeItem.hItem = m_vcthTree[i];
			TreeItem.mask = TVIF_HANDLE  ;
			m_TreeCtrlGroup.GetItem(&TreeItem);

			if(TreeItem.lParam == nSelectedGroupId)
			{
				m_TreeCtrlGroup.SetCheck(m_vcthTree[i],true);			
			}		
		}
	}
	else
	{
		for(int i = 0; i<((CMainFrame*)AfxGetMainWnd())->m_vctnGroupAutoPlayIdx.size();i++)
		{
			int nSelectedGroupId = ((CMainFrame*)AfxGetMainWnd())->m_vctnGroupAutoPlayIdx[i];
			for(int j =0;j<m_vcthTree.size();j++)
			{
				TVITEM TreeItem;
				TreeItem.hItem = m_vcthTree[j];
				TreeItem.mask = TVIF_HANDLE  ;
				m_TreeCtrlGroup.GetItem(&TreeItem);

				if(TreeItem.lParam == nSelectedGroupId)
				{
					m_TreeCtrlGroup.SetCheck(m_vcthTree[j],true);	
					break;
				}		
			}			
		}
	}
}

void CDlgAutoScanGroupSetting::vSetSelectedGroupId(void)
{
	((CMainFrame*)AfxGetMainWnd())->m_vctnGroupAutoPlayIdx.clear();
	for(int i =0;i<m_vcthTree.size();i++)
	{
		TVITEM TreeItem;
		TreeItem.hItem = m_vcthTree[i];
		TreeItem.mask = TVIF_HANDLE  ;
		m_TreeCtrlGroup.GetItem(&TreeItem);

		if(m_TreeCtrlGroup.GetCheck(m_vcthTree[i]) != 0 )
		{
			int nGroup(0);
			nGroup = TreeItem.lParam;
			if(nGroup !=0)
			((CMainFrame*)AfxGetMainWnd())->m_vctnGroupAutoPlayIdx.push_back(nGroup);
		}
	}

	//if(((CMainFrame*)AfxGetMainWnd())->m_vctnGroupAutoPlayIdx.size() > 1)
	//{
	//	((CMainFrame*)AfxGetMainWnd())->vSetGroupAutoScan(true); // set on		
	//}
	//else
	//{
	//	((CMainFrame*)AfxGetMainWnd())->vSetGroupAutoScan(false); // set off		
	//}
	((CMainFrame*)AfxGetMainWnd())->vSetGroupAutoScan(m_CheckBox_AutoscanEnable.GetCheck());
}

