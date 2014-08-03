// DlgModifyStreamPTZ.cpp : implementation file
//

#include "stdafx.h"
#include "DlgModifyStreamPTZ.h"

#define WHITECOLOR RGB(255,255,255)

// CDlgModifyStreamPTZ dialog

IMPLEMENT_DYNAMIC(CDlgModifyStreamPTZ, CDialog)

BOOL CDlgModifyStreamPTZ::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillRect(&rect, &m_SilverBrush);

	//return CBaseDialog::OnEraseBkgnd(pDC);
	
	return true;
}

HBRUSH CDlgModifyStreamPTZ::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CBaseDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	//return hbr;
	if( nCtlColor == CTLCOLOR_EDIT )//|| nCtlColor == CTLCOLOR_MSGBOX)
	{
		pDC->SetBkColor(WHITECOLOR);
		return m_WhiteBrush;
	}
	
	pDC->SetBkColor(GLOBALBKCOLOR);	
	//RecordDirtyFlag = false;
	return  m_SilverBrush;
}

CDlgModifyStreamPTZ::CDlgModifyStreamPTZ(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifyStreamPTZ::IDD, pParent)
{
	m_SilverBrush.CreateSolidBrush(GLOBALBKCOLOR);
	m_WhiteBrush.CreateSolidBrush(WHITECOLOR);
}

CDlgModifyStreamPTZ::~CDlgModifyStreamPTZ()
{
	delete m_CDlgModifyPTZ;
	for(int iTemp = 0; iTemp < veDlgStream.size(); iTemp++)
	{
		delete veDlgStream[iTemp];
	}
}

void CDlgModifyStreamPTZ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MODIFY_STREAM_PTZ, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CDlgModifyStreamPTZ, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MODIFY_STREAM_PTZ, &CDlgModifyStreamPTZ::OnTcnSelchangeTab1)
	ON_BN_CLICKED(IDOK, &CDlgModifyStreamPTZ::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgModifyStreamPTZ message handlers

BOOL CDlgModifyStreamPTZ::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CString csDialogCaption(_T(""));
	LOADSTRING(csDialogCaption,IDS_DLG_STREAM_PTZ);
	//csDialogCaption.LoadString(CGS::g_hResStr, IDS_DLG_STREAM_PTZ);
	this->SetWindowTextW(csDialogCaption);

	CRect r;
	m_TabCtrl.GetClientRect(&r);

	iStreamNum = m_veMoStreamInfo.size();
	CString csTitle;

	for(int iTemp = 0; iTemp <= iStreamNum; iTemp++)
	{
		if(iTemp == iStreamNum)
		{
			csTitle = _T("PTZ / DI / DO");
			//CDialog * m_Dlg = new CDialog();
			m_CDlgModifyPTZ = new CDlgModifyPTZ;

			TCITEM item;
			item.mask = TCIF_TEXT;
			item.pszText = (LPWSTR)(LPCTSTR) csTitle;

			m_TabCtrl.InsertItem(iTemp, &item);
			m_CDlgModifyPTZ->Create(IDD_DLG_MODIFY_PTZ, &m_TabCtrl);

			m_CDlgModifyPTZ->vSetMoPTZInfo(m_MoPTZInfo);

			//veDlgStream.push_back(m_Dlg);

			m_CDlgModifyPTZ->SetWindowPos(NULL, 0, 20, r.right, r.bottom, SWP_HIDEWINDOW);
		}
		else
		{
			csTitle.Format(_T("Stream%d"), iTemp + 1);
			//CDialog * m_Dlg = new CDialog();
			CDlgModifyStream * m_Dlg = new CDlgModifyStream;

			TCITEM item;
			item.mask = TCIF_TEXT;
			item.pszText = (LPWSTR)(LPCTSTR) csTitle;

			m_TabCtrl.InsertItem(iTemp, &item);
			m_Dlg->Create(IDD_DLG_MODIFY_STREAM, &m_TabCtrl);

			m_Dlg->vSetMoStreamInfo(m_veMoStreamInfo[iTemp]);

			veDlgStream.push_back(m_Dlg);

			if(iTemp == 0)
				m_Dlg->SetWindowPos(NULL, 0, 20, r.right, r.bottom, SWP_SHOWWINDOW);
			else
				m_Dlg->SetWindowPos(NULL, 0, 20, r.right, r.bottom, SWP_HIDEWINDOW);
		}		
	}

	if(iStreamNum == 0)
	{
		m_CDlgModifyPTZ->SetWindowPos(NULL, 0, 20, r.right, r.bottom, SWP_SHOWWINDOW);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgModifyStreamPTZ::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	CRect r;
	m_TabCtrl.GetClientRect (&r);

	if( m_TabCtrl.GetCurSel() == m_TabCtrl.GetItemCount() - 1 )
	{
		m_CDlgModifyPTZ->SetWindowPos(NULL, 0, 20, r.right, r.bottom, SWP_SHOWWINDOW);

		for(int iTemp = 0; iTemp < m_TabCtrl.GetItemCount() - 1; iTemp++)
		{
			this->veDlgStream[iTemp]->SetWindowPos(NULL, 0, 20, r.right, r.bottom, SWP_HIDEWINDOW);
		}
	}
	else
	{
		m_CDlgModifyPTZ->SetWindowPos(NULL, 0, 20, r.right, r.bottom, SWP_HIDEWINDOW);
		
		for(int iTemp = 0; iTemp < m_TabCtrl.GetItemCount() - 1; iTemp++)
		{
			if(iTemp == m_TabCtrl.GetCurSel())
				this->veDlgStream[iTemp]->SetWindowPos(NULL, 0, 20, r.right, r.bottom, SWP_SHOWWINDOW);
			else
				this->veDlgStream[iTemp]->SetWindowPos(NULL, 0, 20, r.right, r.bottom, SWP_HIDEWINDOW);
		}
	}

	*pResult = 0;
}

void CDlgModifyStreamPTZ::vSetStreamPTZInfo(MoPTZInfo o_MoPTZInfo, std::vector<MoStreamInfo> o_veMoStreamInfo)
{
	m_MoPTZInfo = o_MoPTZInfo;

	if(o_veMoStreamInfo.size() != 0)
		m_veMoStreamInfo.assign(o_veMoStreamInfo.begin(), o_veMoStreamInfo.end());
	else
		m_veMoStreamInfo.clear();
}
void CDlgModifyStreamPTZ::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	m_MoPTZInfo = m_CDlgModifyPTZ->mGetMoPTZInfo();

	m_veMoStreamInfo.clear();

	for(int iTemp = 0; iTemp < iStreamNum; iTemp++)
	{
		MoStreamInfo m_MoStreamInfo = veDlgStream[iTemp]->mGetMoStreamInfo();
		m_veMoStreamInfo.push_back(m_MoStreamInfo);
	}

	OnOK();
}
