// ConfigSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NVR.h"
#include "ConfigSettingDlg.h"
#include "ConfigTabView.h"

// CConfigSettingDlg dialog
IMPLEMENT_DYNAMIC(CConfigSettingDlg, CDialog)
IMPLEMENT_DYNAMIC(CConfigTabCtrl, CTabCtrl)

CConfigSettingDlg::CConfigSettingDlg(vector<NODEITEM>& vcNodeItem, CWnd* pParent /*=NULL*/)
	: CDialog(CConfigSettingDlg::IDD, pParent),m_isStation(false),m_vcNodeItem(vcNodeItem)
{

}

CConfigSettingDlg::~CConfigSettingDlg()
{
}

void CConfigSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEVICE_TREE, m_DeviceTree);
	DDX_Control(pDX, IDC_TAB, m_Tab);
}

BEGIN_MESSAGE_MAP(CConfigTabCtrl, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CConfigTabCtrl::OnTcnSelchange)
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(CConfigSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_IPCAM_BUTTON, &CConfigSettingDlg::OnBnClickedIpcamButton)
	ON_BN_CLICKED(IDC_CLIENT_BUTTON, &CConfigSettingDlg::OnBnClickedClientButton)
	ON_WM_DESTROY()
	ON_REGISTERED_MESSAGE(MsgChangeTabPage, &CConfigSettingDlg::OnChangeTabPage)
	ON_NOTIFY(TVN_SELCHANGED, IDC_DEVICE_TREE, &CConfigSettingDlg::OnTvnSelchangedDeviceTree)
	ON_BN_CLICKED(IDOK, &CConfigSettingDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDCANCEL, &CConfigSettingDlg::OnBnClickedExit)
END_MESSAGE_MAP()


// CConfigSettingDlg message handlers
void CConfigSettingDlg::OnBnClickedIpcamButton()
{
	// TODO: Add your control notification handler code here
	m_tabMgr.ShowTabs(DeviceTabs);
}

void CConfigSettingDlg::OnBnClickedClientButton()
{
	// TODO: Add your control notification handler code here
	m_tabMgr.ShowTabs(ClientTabs);
}

BOOL CConfigSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	if(m_vcNodeItem.size())
		m_SelectNode = m_vcNodeItem[0];

	InitialTree();
	InitialTabView();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigSettingDlg::InitialTabView()
{
	m_tabMgr.Initial(m_vcNodeItem, &m_Tab);

	m_tabMgr.ShowTabs(ClientTabs);
	
	if (m_vcNodeItem[0].bChannel || m_vcNodeItem[0].byDeviceType == ONVIF ||
		m_vcNodeItem[0].byModelType != MODEL_1180)
	{
		GetDlgItem(IDC_IPCAM_BUTTON)->ShowWindow(SW_HIDE);
	}
}

void CConfigSettingDlg::InitialTree()
{
	USES_CONVERSION;

	int nIdx = 0, nCount = m_vcNodeItem.size();
	TVINSERTSTRUCT tvInsert;

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
 		tvInsert.hParent = TVGN_ROOT;
 		tvInsert.hInsertAfter = TVI_LAST;
 		tvInsert.item.mask = TVIF_PARAM | TVIF_TEXT | TVIF_HANDLE | TVIF_SELECTEDIMAGE;
 		tvInsert.item.pszText = A2W(m_vcNodeItem[nIdx].camera_name);
		m_DeviceTree.InsertItem(&tvInsert);
	}
}

void CConfigSettingDlg::OnDestroy()
{
	m_tabMgr.DeletePage();
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
}

LRESULT CConfigSettingDlg::OnChangeTabPage(WPARAM wParam,LPARAM lParam)
{
	int nPage = wParam;
	m_tabMgr.ShowPage(nPage);
	return 0;
}

void CConfigSettingDlg::OnTvnSelchangedDeviceTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	USES_CONVERSION;

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	HTREEITEM hItem = m_DeviceTree.GetSelectedItem();

	CString str = m_DeviceTree.GetItemText(hItem);

	int nIdx = 0, nCount = m_vcNodeItem.size();
	for (nIdx=0 ; nIdx < nCount ; nIdx++)
	{
		if (str.Find(A2W(m_vcNodeItem[nIdx].camera_name)) != -1)
		{
			m_SelectNode = m_vcNodeItem[nIdx];
			break;
		}
	}

	*pResult = 0;
}

void CConfigSettingDlg::OnBnClickedExit()
{
	// TODO: Add your control notification handler code here
	m_tabMgr.Exsit();
	OnCancel();
}

void CConfigSettingDlg::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
	m_tabMgr.Save();
	OnOK();
}

////////////////////////////////////////////////////////////////////////// CTabManager
void CTabManager::Initial(vector<NODEITEM>& vcNodeItem, CWnd* pParent)
{
	NODEITEM node = {0};
	CRect rec, recTab;
	
	m_DataCmd.Initial(vcNodeItem, CTabManager::UpdateCgiFun, this);
	m_parent = pParent;
	((CTabCtrl*)m_parent)->GetClientRect(&rec);
	((CTabCtrl*)m_parent)->InsertItem(0, _T("System"));
	((CTabCtrl*)m_parent)->GetItemRect(0, recTab);
	rec.top = recTab.bottom;
	
	m_DataCmd.GetModifyNodeItem(node);
	CreateClientTabs(rec);
	
	if (isNeedDeviceTab())
	{
		CreateDeviceTabs(rec);
	}

	((CTabCtrl*)m_parent)->DeleteItem(0);
}

bool CTabManager::isNeedDeviceTab()
{
	NODEITEM node = {0};
	m_DataCmd.GetModifyNodeItem(node);
	bool bResult = true;

	if (node.byDeviceType == eNVR || node.byDeviceType == IPVSTATION ||
		node.byDeviceType == ONVIF || node.byModelType != MODEL_1180)
	{
		bResult = false;
	}
	return bResult;
}

void CTabManager::ShowTabs(TabsType tapstype)
{
	m_tabsType = tapstype;
	((CTabCtrl*)m_parent)->DeleteAllItems();
	int nIdx = 0, nCount = 0;
	if (tapstype == DeviceTabs)
	{
		nCount = m_vcDeviceTabs.size();
		CString str;
		for (nIdx=0; nIdx<nCount; nIdx++)
		{
			m_vcDeviceTabs[nIdx]->GetWindowText(str);
			((CTabCtrl*)m_parent)->InsertItem(nIdx, str);
			m_vcDeviceTabs[nIdx]->ShowWindow(SW_HIDE);
		}

		nCount = m_vcClientTabs.size();
		for (nIdx=0; nIdx<nCount; nIdx++)
		{
			m_vcClientTabs[nIdx]->ShowWindow(SW_HIDE);
		}

		if (m_vcDeviceTabs.size())
		{
			((CTabCtrl*)m_parent)->SetCurSel(0);
			m_vcDeviceTabs[0]->ShowWindow(SW_SHOW);
		}
	}
	else
	{
		nCount = m_vcDeviceTabs.size();
		for (nIdx=0; nIdx<nCount; nIdx++)
		{
			m_vcDeviceTabs[nIdx]->ShowWindow(SW_HIDE);
		}

		nCount = m_vcClientTabs.size();
		CString str;
		for (nIdx=0; nIdx<nCount; nIdx++)
		{
			m_vcClientTabs[nIdx]->GetWindowText(str);
			((CTabCtrl*)m_parent)->InsertItem(nIdx, str);
			m_vcClientTabs[nIdx]->ShowWindow(SW_HIDE);
		}

		if (m_vcClientTabs.size())
		{
			((CTabCtrl*)m_parent)->SetCurSel(0);
			m_vcClientTabs[0]->ShowWindow(SW_SHOW);
		}
	}
}

void CTabManager::ShowPage(int nPage)
{
	int nIdx = 0, nCount = 0;
	vector<CWnd* > vcTabs;

	if (m_tabsType == DeviceTabs)
	{
		vcTabs = m_vcDeviceTabs;
	}
	else
	{
		vcTabs = m_vcClientTabs;
	}

	nCount = vcTabs.size();
	for (nIdx=0; nIdx<nCount; nIdx++)
	{
		vcTabs[nIdx]->ShowWindow(SW_HIDE);
	}
	if (vcTabs.size() > nPage)
	{
		vcTabs[nPage]->ShowWindow(SW_SHOW);
	}
}

void CTabManager::CreateTable(CConfigTabView* tab, const CString& strTableName, const CRect& rec)
{
	CRect recTab = rec;
	recTab.DeflateRect(1,1);
	tab->Create(NULL, strTableName, WS_CHILD | WS_VISIBLE | WS_VSCROLL, recTab, m_parent, 1234);
	tab->SendMessage(WM_INITIALUPDATE);
}

void CTabManager::CreateDeviceTabs(const CRect& recTab)
{
	CVideoTab* tabVideo = new CVideoTab(&m_DataCmd);
	CreateTable(tabVideo, _T("Video"), recTab);
	m_vcDeviceTabs.push_back(tabVideo);

	/*CSetIPTab* tabSetIP = new CSetIPTab(m_pNode);
	CreateTable(tabSetIP, _T("Setting IP"), recTab);
	m_vcDeviceTabs.push_back(tabSetIP);*/

	CStreamTab* tabStream = new CStreamTab(&m_DataCmd);
	CreateTable(tabStream, _T("Stream"), recTab);
	m_vcDeviceTabs.push_back(tabStream);
}

void CTabManager::CreateClientTabs(const CRect& recTab)
{
	CInfomationTab *tabInfomation = new CInfomationTab(&m_DataCmd);
	CreateTable(tabInfomation, _T("Infomation"), recTab); 
	m_vcClientTabs.push_back(tabInfomation);

	CStreamProtocolTab *tabStreamProtocol = new CStreamProtocolTab(&m_DataCmd);
	CreateTable(tabStreamProtocol, _T("Stream Protocol"), recTab); 
	m_vcClientTabs.push_back(tabStreamProtocol);	

	CSetStreamTab *tabStreamTab = new CSetStreamTab(&m_DataCmd);
	CreateTable(tabStreamTab, _T("Set Stream"), recTab); 
	m_vcClientTabs.push_back(tabStreamTab);
}

void CTabManager::DeletePage()
{
	int nIdx = 0, nCount = m_vcDeviceTabs.size();

	for (int nIdx = 0; nIdx < nCount; nIdx++)
	{
		m_vcDeviceTabs[nIdx]->DestroyWindow();
	}

	nCount = m_vcClientTabs.size();
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		m_vcClientTabs[nIdx]->DestroyWindow();
	}
}

void CTabManager::Exsit()
{
	m_DataCmd.CloseCgiRequest();
}

void CTabManager::Save()
{
	int nIdx = 0, nCount = m_vcDeviceTabs.size();

	for (int nIdx = 0; nIdx < nCount; nIdx++)
	{
		((CConfigTabView*)m_vcDeviceTabs[nIdx])->SaveData();
	}

	nCount = m_vcClientTabs.size();

	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		((CConfigTabView*)m_vcClientTabs[nIdx])->SaveData();
	}

	m_DataCmd.SaveAllData();
}

void CTabManager::UpdateCgiFun(void* vparam)
{
	CTabManager* pthis = (CTabManager*)vparam;
	int nCount = pthis->m_vcDeviceTabs.size(), nIdx = 0;
	for (nIdx = 0; nIdx < nCount; nIdx++)
	{
		((CConfigTabView*)pthis->m_vcDeviceTabs[nIdx])->UpdateUI();
	}
}