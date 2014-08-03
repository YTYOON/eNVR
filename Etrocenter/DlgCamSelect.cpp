// DlgCamSelect.cpp : implementation file
//

#include "stdafx.h"
#include "DlgCamSelect.h"
#include "GS.h"
#include "MainFrm.h"

// CDlgCamSelect dialog

IMPLEMENT_DYNAMIC(CDlgCamSelect, CDialog)

CDlgCamSelect::CDlgCamSelect(CWnd* pParent /*=NULL*/)
	: CBaseDialog(CDlgCamSelect::IDD, pParent)
{

}

CDlgCamSelect::~CDlgCamSelect()
{
}

void CDlgCamSelect::DoDataExchange(CDataExchange* pDX)
{
	CBaseDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgCamSelect, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTCAM, &CDlgCamSelect::OnLvnItemchangedListcam)
	ON_BN_CLICKED(IDOK, &CDlgCamSelect::OnBnClickedOk)
//	ON_NOTIFY(NM_DBLCLK, IDC_LISTCAM, &CDlgCamSelect::OnNMDblclkListcam)
ON_NOTIFY(NM_DBLCLK, IDC_LISTCAM, &CDlgCamSelect::OnNMDblclkListcam)
END_MESSAGE_MAP()


// CDlgCamSelect message handlers

HBRUSH CDlgCamSelect::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

BOOL CDlgCamSelect::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return CBaseDialog::OnEraseBkgnd(pDC);
}

BOOL CDlgCamSelect::OnInitDialog()
{
	CBaseDialog::OnInitDialog();

	CString csDialogCaption(_T(""));
	csDialogCaption.LoadString(CProductDefine::g_hResStr, IDS_DLG_CAM_SELECTION);
	this->SetWindowTextW(csDialogCaption);
 
	pDB = &(((CMainFrame*)AfxGetMainWnd())->m_DB);
	// TODO:  Add extra initialization here
 
	vBuildList();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

//void CDlgCamSelect::vDBConnect(void)
//{
//		//Database m_DB - 
//	CIniReader m_ini(ECINI);
//	//char* cpDns = m_ini.getKeyValue(_T("DSN"),_T("database"));
//	//char* cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
//	//char* cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
//
//	CString cpDns = m_ini.getKeyValue(_T("DSN"),_T("database")); 
//	CString cpUid = m_ini.getKeyValue(_T("UID"),_T("database"));
//	CString cpPwd = m_ini.getKeyValue(_T("PWD"),_T("database"));
// 
//	CString ls_connect;
//	ls_connect.Format(_T("ODBC;UID=%s;PWD=%s"), cpUid, cpPwd);
//	bool lb_ret = m_DB.Open((LPCTSTR)cpDns, FALSE, FALSE, ls_connect );
//}

void CDlgCamSelect::vBuildList()
{
	int ColNum=6;
	CString  ls_sql;
	//pCheckNode = NULL;
	
 	m_camList = (CListCtrl *)GetDlgItem(IDC_LISTCAM);
	LV_COLUMN ListColumn;

	WCHAR * ListTitles[5] = { _T(""), _T("Camera Index"),  _T("Camera Name"), _T("IP Address"), _T("Model")};

	for(int i = 0; i < 5; i++)
	{
		ListColumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		ListColumn.fmt = LVCFMT_LEFT;
		ListColumn.iSubItem = i;

		switch (i)
		{
			case 0: //checkbox
				ListColumn.cx = 25;
				break;

			case 1:
				ListColumn.cx = 80;
				break;

			case 2:
				ListColumn.cx = 130;
				break;

			case 3:
				ListColumn.cx = 100;
				break;

			case 4:
				ListColumn.cx = 100;
				break;
		 
			default:	
				ListColumn.cx = 90;
				break;
		}

		ListColumn.pszText = ListTitles[i];
		m_camList->InsertColumn(i, &ListColumn);
	}

	DWORD dwStyle = m_camList->GetStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP | LVS_EX_CHECKBOXES;
	m_camList->SetExtendedStyle(dwStyle);

	//Populate Data...
	ls_sql = _T("SELECT * FROM ec_camera ORDER BY camera_idx");
	CRecordset m_rs(pDB);
	m_rs.Open(CRecordset::dynaset, ls_sql);
  	m_rs.MoveFirst();

	CDBVariant  varCamid, varCamidx;

	int ll_row=0;
 	LVITEM	lvi;
	CString strItem,  ls_camname, ls_ip, ls_model;

	while (!m_rs.IsEOF())
	{
		strCAM* m_strcam = new strCAM;
	

		//Stick data into structure
		m_rs.GetFieldValue(_T("cameraid"), varCamid, SQL_C_SSHORT);
		m_strcam->CameraId = varCamid.m_iVal;

		m_rs.GetFieldValue(_T("camera_idx"), varCamidx, SQL_C_SSHORT);
		m_strcam->CameraIdx = varCamidx.m_iVal;

		m_rs.GetFieldValue(_T("cameraname"), ls_camname );

		m_rs.GetFieldValue(_T("ipaddress"), ls_ip );
		m_strcam->IP = ls_ip.Trim();

		m_rs.GetFieldValue(_T("model_code"), ls_model );
		m_strcam->csModel = ls_model.Trim();

			LVITEMW* pLitem = new LVITEM;
				pLitem->mask = LVIF_PARAM|LVIF_TEXT;
				pLitem->iItem = ll_row;
			//	pLitem->pszText = (LPWSTR)(LPCTSTR)csDesc;
	 
				pLitem->lParam = (LPARAM) m_strcam;


 	//		int	nItem = m_camList->InsertItem((ll_row, _T(""), 0);
			//				m_ListEventType->InsertItem(pLitem->mask , ci, csDesc, 0,0,0, 	pLitem->lParam);
				int	nItem = m_camList->InsertItem(pLitem->mask, ll_row, _T(""), 0,0,0, (LPARAM) pLitem->lParam);
			strItem.Format(_T("%i"), varCamidx.m_iVal);
			m_camList->SetItemText(nItem, 1, strItem);

			m_camList->SetItemText(nItem, 2, ls_camname.Trim());

			m_camList->SetItemText(nItem, 3, ls_ip.Trim());
			m_strcam->IP=ls_ip.Trim();

			m_camList->SetItemText(nItem, 4, ls_model.Trim());
			m_strcam->csModel = ls_model.Trim();

			ll_row++;
			m_rs.MoveNext();
	}//While


}



void CDlgCamSelect::OnLvnItemchangedListcam(NMHDR *pNMHDR, LRESULT *pResult)
{
			LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
			// TODO: Add your control notification handler code here
			*pResult = 0;
		 
		 NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

		 /* If the event is not fired because of any change in individual items then the oldstate and 
		 newstate members of pNMListView are not used.  So check for that.*/

		 if (!pNMListView->uOldState && !pNMListView->uNewState)
		  return;

	// TODO: Add your control notification handler code here
	 BOOL bChecked = ListView_GetCheckState(m_camList->m_hWnd, pNMListView ->iItem);
		if (bChecked)
		{
		  for(int nCount=0; nCount<m_camList->GetItemCount(); nCount++)
			if(nCount != pNMListView->iItem) //Unchecked Item
			{
				m_camList->SetCheck(nCount, FALSE);
			}
			else //Checked Item
			{
				m_camList->SetCheck(nCount, true);
			}
		}
	*pResult = 0;
}

void CDlgCamSelect::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	int nCount = m_camList->GetItemCount();
	BOOL fCheck = FALSE;

	// Set the check state of every other item to TRUE and 
	// all others to FALSE.
	strCAM* lpstrcam = new strCAM;
	LVITEM* lvi = new LVITEM;

	for (int i = 0; i < nCount; i++)
	{
		if (m_camList->GetCheck(i))
		{
			strCAM* lp = (strCAM*)m_camList->GetItemData(i);
			
			if (lp->CameraIdx>=0)
				m_camidx = lp->CameraIdx;

			OnOK();
		}
	}

}

//void CDlgCamSelect::OnNMDblclkListcam(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE>(pNMHDR);
//	// TODO: Add your control notification handler code here
//	*pResult = 0;
//}

void CDlgCamSelect::OnNMDblclkListcam(NMHDR *pNMHDR, LRESULT *pResult)
{
	 DWORD dwPos = ::GetMessagePos ();
    CPoint point ((int) LOWORD (dwPos), (int) HIWORD (dwPos));
    m_camList->ScreenToClient (&point);

    int nIndex;
    if ((nIndex = m_camList->HitTest (point)) != -1) {
        // If you are in this if block, that means an item on the list
        //   was chosen and nIndex is the index of the selected item
        //   from the list.

        // Check the checkbox & ok close this window
		m_camList->SetCheck(nIndex, true);
		CDlgCamSelect::OnBnClickedOk();
        return;
    }
  

	*pResult = 0;
}
