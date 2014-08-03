#include "stdafx.h"
#include "DlgScanNetworkCameras.h"
#include "IPCamDiscovery.h"
#include "INI.h"
//#include "UpnpClient.h"

// CDlgScanNetworkCameras dialog

IMPLEMENT_DYNAMIC(CDlgScanNetworkCameras, CDialog)
int CALLBACK SortCol(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

CDlgScanNetworkCameras::CDlgScanNetworkCameras(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgScanNetworkCameras::IDD, pParent), m_pbk(NULL)
	, m_AllScanListCount(0), m_FirstFlag(true),m_pList(NULL),m_isONVIF(false)
{

}

CDlgScanNetworkCameras::CDlgScanNetworkCameras(bool isONVIF, CWnd* pParent)
: CDialog(CDlgScanNetworkCameras::IDD, pParent), m_pbk(NULL)
, m_AllScanListCount(0), m_FirstFlag(true),m_pList(NULL),m_isONVIF(isONVIF)
{

}

CDlgScanNetworkCameras::~CDlgScanNetworkCameras()
{
	if(m_pList) delete m_pList;

	if (m_pONVIFNode.size())
	{
		int nIdx= 0, nCount = m_pONVIFNode.size();
		for (nIdx=0;nIdx<nCount;nIdx++)
		{
			delete m_pONVIFNode[nIdx];
		}
	}
}

void CDlgScanNetworkCameras::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgScanNetworkCameras, CDialog)
	ON_BN_CLICKED(IDC_RESCAN, &CDlgScanNetworkCameras::OnBnClickedRescan)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SCAN_CAMERAS, &CDlgScanNetworkCameras::OnLvnItemchangedListScanCameras)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_SCAN_CAMERAS, &CDlgScanNetworkCameras::OnLvnColumnclickListScanCameras)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SCAN_CAMERAS, &CDlgScanNetworkCameras::OnNMDblclkListScanCameras)
	ON_BN_CLICKED(IDOK, &CDlgScanNetworkCameras::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgScanNetworkCameras message handlers

BOOL CDlgScanNetworkCameras::OnInitDialog(void)
{
	m_pbk = CNVRBk::GetBkObject();

	CString csDialogCaption(_T(""));
	LOADSTRING(csDialogCaption, IDS_DLG_SCAN_CAMERA);
	this->SetWindowTextW(csDialogCaption);
	CString csListTitles;
	LOADSTRING(csListTitles, IDS_SCAN_TITLES);

	m_pCheckNode = NULL;
	SETITEMTEXT(IDC_RESCAN, IDS_SCAN_SCAN_BUTTON);
	SETITEMTEXT(IDC_SCAN_STATIC, IDS_SCAN_STATIC);
	CListCtrl * pScanList = (CListCtrl *)this->GetDlgItem(IDC_LIST_SCAN_CAMERAS);

	LV_COLUMN ListColumn;

	CString ListTitles[9] ={};
	for(int iTemp =1; iTemp <= 8; iTemp++)
	{
		::AfxExtractSubString(ListTitles[iTemp], csListTitles, iTemp-1, ';');
	}
    DWORD nWidth[]={16, 145, 90, 90, 130, 70, 70, 50};
	ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	ListColumn.fmt = LVCFMT_LEFT;	
	int nCount = 0;
	if (m_isONVIF)
	{
		nCount = 8;
	} 
	else
	{
		nCount = 7;
	}
    for(int i = 0; i < nCount; i++)
	{
		ListColumn.iSubItem = i;
        ListColumn.cx = nWidth[i];
		ListColumn.pszText = (LPWSTR)(LPCTSTR)ListTitles[i];
		pScanList->InsertColumn(i, &ListColumn);
	}

	DWORD dwStyle = pScanList->GetStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	pScanList->SetExtendedStyle(dwStyle);

	pScanList->ModifyStyle(0, LVS_SINGLESEL);

	m_SmallImg.Create(16, 16, ILC_COLOR | ILC_MASK, 0, 1);
	CBitmap bIcon;
	bIcon.LoadBitmapW(IDB_CHECKRED);
	m_SmallImg.Add(&bIcon, RGB(0, 0, 0));
	pScanList->SetImageList(&m_SmallImg, LVSIL_SMALL);

	InsertNodes2List();
	m_FirstFlag = false;
	return true;
}

void CDlgScanNetworkCameras::GetONVIF()
{
	if (m_isONVIF)
	{
		vector<NODEITEM> vcNodes;
		m_pbk->GetONVIFNodes(vcNodes);
		
		if(vcNodes.size())
		{
			FilterSameIP(vcNodes);
			InsertONVIF(vcNodes);
		}
	}
}

void CDlgScanNetworkCameras::FilterSameIP(vector<NODEITEM>& vcONVIFNodes)
{
	USES_CONVERSION;
	vector<CString> vcStrIP;
	vector<CString>::iterator it;
	CString str;
	int nIdx = 0, nCount = vcONVIFNodes.size();
	for(nIdx = 0; nIdx < nCount; nIdx++)
	{
		str.Empty();
		str = A2W(vcONVIFNodes[nIdx].ip);
		vcStrIP.push_back(str);
	}

	nCount = m_pvtNode.size();
	for(nIdx = 0; nIdx < nCount; nIdx++)
	{
		str.Empty();
		str = A2W(m_pvtNode[nIdx]->ip);
		it = find(vcStrIP.begin(),vcStrIP.end(),str);
		if (it != vcStrIP.end())
		{
			int nIdx = it-vcStrIP.begin();
			vcStrIP.erase(it);
			vcONVIFNodes.erase(vcONVIFNodes.begin()+nIdx);
		}
	}
}

void CDlgScanNetworkCameras::InsertONVIF(vector <NODEITEM>& vcONVIFNode)
{
	int nidx=0, ncount=vcONVIFNode.size();
	int camera_idx = 0;
	for(nidx=0; nidx < ncount; nidx++)
	{
		NODEITEM *pNode = new NODEITEM;
		memcpy(pNode,&(vcONVIFNode[nidx]),sizeof(NODEITEM));

		m_pONVIFNode.push_back(pNode);
		m_pvtNode.push_back(pNode);
	}
}

void CDlgScanNetworkCameras::InsertDevice(CListCtrl * pScanList)
{
	USES_CONVERSION;
	CString csIP, csName, csModel, csMac, csPort1, csPort2, csDBMac, csCamIDX;
	NODEITEM* pNode = NULL;
	map<int, CString>::iterator iPos;
	bool bCheck = false;
	int nItem = 0;
	int nidx=0, ncount=0;
	ncount = m_pvtNode.size();
	for(nidx=0; nidx < ncount; nidx++)
	{
		bCheck = false;
		csMac.Empty();
		pNode = m_pvtNode[nidx];

		csName.Format(_T("%s - %s"), A2W(pNode->ip), A2W(pNode->modelname));
		csIP.Format(_T("%s"), A2W(pNode->ip));
		csModel.Format(_T("%s"), A2W(pNode->modelname));
		//m_pbk->TransMac2String(pNode->mac,csMac,false);

		csMac = A2W(pNode->ip);
		csPort1.Format(_T("%d"), pNode->nAPPort);
		csPort2.Format(_T("%d"), pNode->nAP2Port);

		for(iPos = m_mapMacIdx.begin(); iPos != m_mapMacIdx.end(); iPos++)
		{
			if(csMac.MakeUpper() == iPos->second.MakeUpper())
				bCheck = true;
		}

		csMac.Empty();
		m_pbk->TransMac2String(pNode->mac,csMac,true);	

		if(bCheck)		//Check Data Exit or not
		{
			nItem = pScanList->InsertItem(0, _T(""), 0);
			pScanList->SetItemText(nItem, 1, csName);
			pScanList->SetItemText(nItem, 2, csIP);
			pScanList->SetItemText(nItem, 3, csModel);
			pScanList->SetItemText(nItem, 4, csMac);
			pScanList->SetItemText(nItem, 5, csPort1);
			pScanList->SetItemText(nItem, 6, csPort2);
		}
		else
		{
			nItem = pScanList->InsertItem(0, _T(""), 1);
			pScanList->SetItemText(nItem, 1, csName);
			pScanList->SetItemText(nItem, 2, csIP);
			pScanList->SetItemText(nItem, 3, csModel);
			pScanList->SetItemText(nItem, 4, csMac);
			pScanList->SetItemText(nItem, 5, csPort1);
			pScanList->SetItemText(nItem, 6, csPort2);
		}
		if (m_isONVIF)
		{
			if(pNode->byDeviceType == ONVIF)
				pScanList->SetItemText(nItem, 7, _T("yes"));
			else
				pScanList->SetItemText(nItem, 7, _T("no"));
		}
	}
}

void CDlgScanNetworkCameras::OnBnClickedRescan()
{
	vResetNodeItem();
	InsertNodes2List();
}

int CDlgScanNetworkCameras::vScanListItemCount(void)
{
	CListCtrl * pScanList = (CListCtrl *)this->GetDlgItem(IDC_LIST_SCAN_CAMERAS);
	return pScanList->GetItemCount();
}

void CDlgScanNetworkCameras::OnLvnItemchangedListScanCameras(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	CListCtrl * pScanList = (CListCtrl *)this->GetDlgItem(IDC_LIST_SCAN_CAMERAS);

	CString csSelectMac = pScanList->GetItemText(pScanList->GetSelectionMark(), 4);
	CString csMac;
	for(int Temp = 0; Temp < m_pvtNode.size(); Temp++)
	{
		m_pbk->TransMac2String(m_pvtNode[Temp]->mac,csMac,true);
		if(csMac == csSelectMac)
		{
			m_pCheckNode = m_pvtNode[Temp];
			break;
		}
	}

	*pResult = 0;
}

void CDlgScanNetworkCameras::OnLvnColumnclickListScanCameras(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW*  pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);

	CListCtrl * pScanList = (CListCtrl *)this->GetDlgItem(IDC_LIST_SCAN_CAMERAS);

	for(int i = 0;   i < pScanList->GetItemCount(); i++) 
	{pScanList->SetItemData(i, i);} 

	if(m_pList==NULL)
		m_pList = new ListSort;

	m_pList->lcMyList = pScanList;
	m_pList->iSub = pNMListView->iSubItem;

	int sortnum = (int)GetProp(pScanList->GetSafeHwnd(), _T("SORT_COLUMN"));
    int sortasc = (int)GetProp(pScanList->GetSafeHwnd(), _T("SORT_DIRECTION"));
    if(sortnum == pNMListView->iSubItem)
	{
        sortasc=(sortasc+1)%2;
        SetProp(pScanList->GetSafeHwnd(), _T("SORT_DIRECTION"), (HANDLE)sortasc);
    }
    SetProp(pScanList->GetSafeHwnd(), _T("SORT_COLUMN"), (HANDLE)pNMListView->iSubItem);
    m_pList->seq = sortasc;

	pScanList->SortItems( (PFNLVCOMPARE)SortCol, (LPARAM)m_pList );

	*pResult = 0;
}


int CALLBACK SortCol(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	ListSort * pList = NULL;
	
	pList = (ListSort *)lParamSort;

    LVFINDINFO findInfo; 
    findInfo.flags = LVFI_PARAM; 

    findInfo.lParam = lParam1; 
	int iItem1 = pList->lcMyList->FindItem(&findInfo, -1); 
    findInfo.lParam = lParam2; 
    int iItem2 = pList->lcMyList->FindItem(&findInfo, -1); 

	CString strItem1 = pList->lcMyList->GetItemText(iItem1, pList->iSub); 
	CString strItem2 = pList->lcMyList->GetItemText(iItem2, pList->iSub);

	char *cBufferItem1 = new char[strItem1.GetLength() + 1];
	wcstombs_s(NULL, cBufferItem1, strItem1.GetLength() + 1, strItem1, _TRUNCATE);

	char *cBufferItem2 = new char[strItem2.GetLength() + 1];
	wcstombs_s(NULL, cBufferItem2, strItem2.GetLength() + 1, strItem2, _TRUNCATE);
    
	if(pList->seq)
          return strcmp(cBufferItem2, cBufferItem1);
     else
          return -strcmp(cBufferItem2, cBufferItem1);
}

void CDlgScanNetworkCameras::OnNMDblclkListScanCameras(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CListCtrl * pScanList = (CListCtrl *)this->GetDlgItem(IDC_LIST_SCAN_CAMERAS);
	
	CString csSelectMac = pScanList->GetItemText(pScanList->GetSelectionMark(), 4);
	CString csMac;
	for(int Temp = 0; Temp < m_pvtNode.size(); Temp++)
	{	
		m_pbk->TransMac2String(m_pvtNode[Temp]->mac, csMac, true);
		if(csMac == csSelectMac)
		{
			m_pCheckNode = m_pvtNode[Temp];
			break;
		}
	}

	this->EndModalLoop(1);
	*pResult = 0;
}

void CDlgScanNetworkCameras::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CListCtrl * pScanList = (CListCtrl *)this->GetDlgItem(IDC_LIST_SCAN_CAMERAS);
	
	CString csSelectMac = pScanList->GetItemText(pScanList->GetSelectionMark(), 4);
	CString csMac;
	for(int Temp = 0; Temp < m_pvtNode.size(); Temp++)
	{
		m_pbk->TransMac2String(m_pvtNode[Temp]->mac, csMac, true);
		if(csMac == csSelectMac)
		{
			m_pCheckNode = m_pvtNode[Temp];
			break;
		}
	}

	OnOK();
}

void CDlgScanNetworkCameras::vResetNodeItem()
{
	m_pvtNode.clear();
	HANDLE hScanRequire = CreateEvent(NULL, FALSE, TRUE, SCAN_REQUIRE);
	::SetEvent(hScanRequire);
	::CloseHandle(hScanRequire);
	::Sleep(1000);

	m_pbk->scan_ONVIF();
}

void CDlgScanNetworkCameras::InsertNodes2List()
{
	m_pvtNode = CIPCamDiscovery::GetNodePtr();
	GetONVIF();
	SETITEMTEXT(IDC_RESCAN, IDS_SCAN_SCAN_BUTTON);
	CListCtrl * pScanList = (CListCtrl *)this->GetDlgItem(IDC_LIST_SCAN_CAMERAS);
	pScanList->SetRedraw(FALSE); //Lock List Control
	pScanList->DeleteAllItems();

	int nidx=0, ncount=0;
	vector<ec_Camera> vcCamobj;
	m_pbk->GetAllCameras(vcCamobj);
	ncount = vcCamobj.size();
	for (nidx=0; nidx<ncount; nidx++)
	{
		if(!m_pbk->IsChannel(vcCamobj[nidx].video_format))
			m_mapMacIdx.insert(::pair<int, CString>(vcCamobj[nidx].camera_idx, vcCamobj[nidx].ipaddress));
	}

	InsertDevice(pScanList);

	pScanList->SetRedraw(TRUE);		//Unlock List control
	pScanList->Invalidate();
	pScanList->UpdateWindow();
	m_AllScanListCount = pScanList->GetItemCount();
}